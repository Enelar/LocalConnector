#pragma once

#include <string>

namespace tsoft
{
  template<unsigned long minimal_size, int host>
  struct shared_memory_file
  {
    shared_memory_file(const std::string &filename);

    typedef unsigned char byte;
    byte *Memory() const;
    unsigned long Size() const;

    ~shared_memory_file();
  };

  template<unsigned long minimal_size, int host>
  shared_memory_file<minimal_size, host>::shared_memory_file(const std::string &filename);

  template<unsigned long minimal_size, int host>
  shared_memory_file<minimal_size, host>::~shared_memory_file();
}

#include "shared_memory_file.hpp"