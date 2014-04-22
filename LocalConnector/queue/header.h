#pragma once

namespace queue
{
  struct header
  {
    const unsigned long size;
    unsigned long
      first = 0,
      last = 0;

    bool IsEmpty() const;
    unsigned long Available() const;

    unsigned long First() const;
    unsigned long Last() const;
    unsigned long Size() const;

    header(long size);
  };
}