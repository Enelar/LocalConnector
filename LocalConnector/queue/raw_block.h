#pragma once

namespace queue
{
  struct raw_block
  {
    bool dirty = false;
    long size = 0;

    char *Begin() const;
    char *End() const;
    bool IsDirty() const;
    raw_block *Next() const;

    ~raw_block()
    {
      dirty = true;
    }
  };
}