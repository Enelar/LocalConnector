#include "shared_queue.h"

#include <memory>
#include <algorithm>

#ifdef _MSC_VER
# include <intrin.h>
# define CAS(ptr, oldval, newval) \
  _InterlockedCompareExchange(ptr, newval, oldval)
#elif __GNUC__
# if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 1)
#  error "requires GCC 4.1 or greater"
# endif
# define CAS(ptr, oldval, newval) \
  __sync_val_compare_and_swap(ptr, oldval, newval)
#else
# error "CAS not supported on this platform"
#endif

namespace queue
{
  template<long size_in_bytes>
  shared_queue<size_in_bytes>::shared_queue()
    : h(size_in_bytes)
  {
  }

  template<long size_in_bytes>
  template<typename T>
  T shared_queue<size_in_bytes>::Pop()
  { // MEMLEAK HERE
    typedef block<filler<sizeof T>> filler_block;
    typedef block<T> return_block;
    static_assert(sizeof(return_block) <= sizeof(filler_block), "TMP memory size less than minimal required");

    filler_block *block_mem = ExtractFirst<sizeof(T)>();
    return_block *res = (return_block *)block_mem;

    T ret(res->Data());
    delete block_mem;
    return ret;
  }

  template<long size_in_bytes>
  template<typename T>
  void shared_queue<size_in_bytes>::Push(const T& data)
  {
    std::unique_ptr<byte[]> arr = std::make_unique<byte[]>(sizeof(T));
    T *ptr = reinterpret_cast<T *>(arr.get());
    try
    {
      new (ptr)T(data);
      Push(arr.get(), sizeof(T));
    }
    catch (...)
    {
      ptr->~T();
      throw;
    }
    // block will be destructed on Pop
  }

  template<long size_in_bytes>
  void shared_queue<size_in_bytes>::Push(const byte *orig, long size)
  {
    if (!orig)
      throw push_fault();
    if (size <= 0)
      throw push_fault();
    if (size > AvailableBytes())
      throw push_fault();

    std::unique_ptr<byte[]> arr = std::make_unique<byte[]>(sizeof(raw_block) + size);
    raw_block *ptr = reinterpret_cast<raw_block *>(arr.get());
    try
    {
      new (ptr)raw_block;
      ptr->dirty = false;
      ptr->size = size;
      std::copy(orig, orig + size, ptr->Begin());

      raw_block *dest = GetLastPlace(size);
      unsigned long block_size = size + sizeof(raw_block);
      unsigned long first_chunk_size = std::min(block_size, h.Size() - h.First());

      memcpy(
        (byte *)dest + sizeof(ptr->dirty),
        arr.get() + sizeof(ptr->dirty),
        first_chunk_size - sizeof(ptr->dirty)); // write all except dirty flag

      if (first_chunk_size != block_size)
        memcpy(storage, arr.get() + first_chunk_size, block_size - first_chunk_size);

      dest->dirty = false; // close read mutex
    }
    catch (...)
    {
      ptr->~raw_block();
      throw;
    }
    ptr->~raw_block();
  }

  template<long size_in_bytes>
  int shared_queue<size_in_bytes>::AvailableBytes() const
  {
    return h.Available();
  }

  template<long size_in_bytes>
  raw_block *shared_queue<size_in_bytes>::GetLastPlace(long size)
  {
    if (size <= 0)
      throw push_fault();

    long my_last, res;
    do
    {
      if (size > AvailableBytes())
        throw push_fault();

      my_last = h.last;
      res = CAS((long *)&h.last, my_last, h.last + size + sizeof(raw_block));
    } while (res != my_last);

    raw_block *ret = reinterpret_cast<raw_block *>(storage + my_last);

    new (ret)raw_block;
    ret->dirty = true; // open read mutex
    ret->size = size;
    return ret;
  }

  template<long size_in_bytes>
  template<int buf_size>
  block<filler<buf_size>> *shared_queue<size_in_bytes>::ExtractFirst()
  {
    raw_block *first = reinterpret_cast<raw_block *>(h.First() + storage);
    if (first->size > buf_size)
      throw pop_fault();

    unsigned long block_size = first->size;
    unsigned long first_chunk_size = std::min(block_size, h.Size() - h.First());

    block<filler<buf_size>> *ret = new block<filler<buf_size>>(filler<buf_size>());
    memcpy(ret->Begin(), first->Begin(), first_chunk_size);

    if (first_chunk_size != block_size)
      memcpy(ret->Begin() + first_chunk_size, first->Begin() + first_chunk_size, block_size - first_chunk_size);

    first->~raw_block();
    // Its safe because we in placement memory
    h.first += first_chunk_size + sizeof(raw_block);

    return ret;
  }
}