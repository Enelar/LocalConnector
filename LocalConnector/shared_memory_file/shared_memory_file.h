#pragma once

#include <string>
#include <memory>

namespace tsoft
{
  struct shared_memory_file_impl;

  struct shared_memory_file_fictive_parent
  {
    typedef unsigned char byte;
    byte *memory;
    shared_memory_file_impl *head;
    unsigned long size;
  };

  template<unsigned long minimal_size, int host>
  struct shared_memory_file : shared_memory_file_fictive_parent
  {
    shared_memory_file(const std::string &filename);

    byte *Memory() const;
    unsigned long Size() const;

    ~shared_memory_file();


    friend void SystemDeinit(shared_memory_file_fictive_parent &obj, unsigned long minimal_size, int host);
    friend void SystemInit(shared_memory_file_fictive_parent &obj, const std::string &filename, unsigned long minimal_size, int host);
  };
}

#include "shared_memory_file.hpp"