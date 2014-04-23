#pragma once

#include <string>
#include <memory>

namespace tsoft
{
  struct shared_memory_file_impl;
  template<unsigned long minimal_size, int host>
  struct shared_memory_file
  {
    std::unique_ptr<shared_memory_file_impl> head;
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