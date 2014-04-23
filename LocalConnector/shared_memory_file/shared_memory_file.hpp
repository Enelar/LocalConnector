#pragma once

#include "shared_memory_file.h"

namespace tsoft
{
  template<unsigned long minimal_size, int is_host>
  typename shared_memory_file<minimal_size, is_host>::byte *shared_memory_file<minimal_size, is_host>::Memory() const
  {
  }

  template<unsigned long minimal_size, int is_host>
  unsigned long shared_memory_file<minimal_size, is_host>::Size() const
  {
  }
}