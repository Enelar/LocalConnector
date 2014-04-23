#include "shared_memory_file.h"
#include <windows.h>

using namespace tsoft;

struct shared_memory_file_impl
{
  HANDLE file, mapped;
};

template<unsigned long minimal_size, int host>
shared_memory_file<minimal_size, host>::~shared_memory_file()
{
}

template<unsigned long minimal_size, int host>
shared_memory_file<minimal_size, host>::shared_memory_file(const std::string &filename)
{
  head = std::make_unique<shared_memory_file_impl>();

  head->file =
    CreateFile(
     filename.c_str(),
      GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_DELETE,
      NULL,
      CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL
        | FILE_ATTRIBUTE_TEMPORARY
        | FILE_ATTRIBUTE_SYSTEM
        | FILE_ATTRIBUTE_ARCHIVE
        | FILE_FLAG_DELETE_ON_CLOSE
        | FILE_FLAG_OVERLAPPED
        | FILE_FLAG_RANDOM_ACCESS
      ,
      NULL
    );

  if (file == INVALID_HANDLE_VALUE)
    throw GetLastError();

  head->mapped =
    CreateFileMapping(
      file,
      NULL,
      PAGE_READWRITE,
      0,
      0,
      NULL);

  if (mapped == NULL)
    throw GetLastError();
}