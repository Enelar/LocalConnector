#pragma once

#include "../stdafx.h"

#include "header.h"
#include "block.h"
#include "filler.h"

namespace queue
{
  // multiplie enqueuer, one dequeuer
  template<long size_in_bytes>
  struct shared_queue
  {
    //STATIC_ASSERT(size_in_bytes % 4 == 0);// , "Shared queue should have size x multiplied 4");
    typedef unsigned char byte;

    bool inited;
    header h;
    byte storage[size_in_bytes];

    shared_queue();

    template<typename T>
    T Pop();
    raw_block *Pop();

    template<typename T>
    void Push(const T& data);
    void Push(const byte *arr, unsigned long size);

    template<typename T>
    bool TryPop(T *& ptr);
    bool TryPop(raw_block *&ptr);

    template<typename T>
    bool TryPush(const T &ptr);
    bool TryPush(const byte *arr, unsigned long size);

    unsigned long AvailableBytes() const;
    bool IsEmpty() const;

  private:
    raw_block *GetLastPlace(unsigned long size);

    template<int buf_size>
    block<filler<buf_size>> *ExtractFirst();
  };

  typedef shared_queue<4> shared_mapped_queue;
  struct shared_queue_fault
  {};

  struct push_fault : shared_queue_fault
  {};
  struct overloaded : push_fault
  {};

  struct pop_fault : shared_queue_fault
  {
    pop_fault()
    {

    }
  };
  struct not_ready : pop_fault
  {};
  struct empty : pop_fault
  {};
};

#include "shared_queue.hpp"