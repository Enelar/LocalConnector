#pragma once

#include "shared_memory_file.h"

namespace tsoft
{
  template<unsigned long minimal_size, int is_host>
  typename shared_memory_file<minimal_size, is_host>::byte *shared_memory_file<minimal_size, is_host>::Memory() const
  {
    return memory;
  }

  template<unsigned long minimal_size, int is_host>
  unsigned long shared_memory_file<minimal_size, is_host>::Size() const
  {
    return size;
  }

  template<unsigned long minimal_size, int host>
  shared_memory_file<minimal_size, host>::shared_memory_file(const std::string &filename)
  {
    SystemInit(*this, filename, minimal_size, host);
  }

  template<unsigned long minimal_size, int host>
  shared_memory_file<minimal_size, host>::~shared_memory_file()
  {
    SystemDeinit(*this, minimal_size, host);
  }
}