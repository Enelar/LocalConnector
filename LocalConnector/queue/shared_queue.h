#pragma once

#include "header.h"
#include "block.h"
#include "filler.h"

namespace queue
{
  // multiplie enqueuer, one dequeuer
  template<long size_in_bytes>
  struct shared_queue
  {
    static_assert(size_in_bytes % 4 == 0, "Shared queue should have size x multiplied 4");
    typedef unsigned char byte;

    bool inited = false;
    header h;
    byte storage[size_in_bytes];

    shared_queue();

    template<typename T>
    T Pop();
    raw_block *Pop();

    template<typename T>
    void Push( const T& data );
    void Push( const byte *arr, long size );

    unsigned long AvailableBytes() const;
    bool IsEmpty() const;

  private:
    raw_block *GetLastPlace(long size);

    template<int buf_size>
    block<filler<buf_size>> *ExtractFirst();
  };

  struct shared_queue_fault
  {};

  struct push_fault : shared_queue_fault
  {};
  struct overloaded : push_fault
  {};

  struct pop_fault : shared_queue_fault
  {};
  struct not_ready : pop_fault
  {};
  struct empty : pop_fault
  {};
};

#include "shared_queue.hpp"