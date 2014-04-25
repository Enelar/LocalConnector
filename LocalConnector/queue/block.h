#pragma once

#include "raw_block.h"
#include "filler.h"

namespace queue
{
  template<typename T>
  struct block : raw_block
  {
    typedef filler<sizeof(T)> fillerT;
    fillerT data;

    block(const T &orig)
    {
      new (&data)fillerT(orig);
      size = sizeof(T);
    }

    ~block()
    {
      data.~T();
    }

    T &Data()
    {
      return *reinterpret_cast<T *>(&data);
    }
  };
}