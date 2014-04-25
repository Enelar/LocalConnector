#pragma once

#include "../stdafx.h"

namespace queue
{
  struct raw_block
  {
    unsigned long 
      dirty : 1,
      size : (sizeof(long) * 8 - 1);

    char *Begin() const;
    char *End() const;
    bool IsDirty() const;
    raw_block *Next() const;

    raw_block()
      : dirty(true), size(0)
    {

    }

    ~raw_block()
    {
      dirty = true;
    }
  };

  //static_assert(sizeof(raw_block) == 4, "Block header should be 4 bytes long");
  STATIC_ASSERT(sizeof(raw_block) == 4);
}