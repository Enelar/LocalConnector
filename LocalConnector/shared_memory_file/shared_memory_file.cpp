#include "shared_memory_file.h"
#include <windows.h>

using namespace tsoft;

namespace tsoft
{
  struct shared_memory_file_impl
  {
    HANDLE file, mapped;
  };
}

namespace tsoft
{
  void SystemDeinit(shared_memory_file_fictive_parent &obj, unsigned long minimal_size, int host)
  {
    // TODO: Free resources

    delete obj.head;
  }

  void SystemInit(shared_memory_file_fictive_parent &obj, const std::string &filename, unsigned long minimal_size, int host)
  {
    obj.head = new shared_memory_file_impl();

    obj.head->file =
      CreateFile(
#ifndef _MAGIC_SENSE_
        filename.c_str(),
        GENERIC_READ | GENERIC_WRITE,
          FILE_SHARE_READ 
          | FILE_SHARE_WRITE
          | FILE_SHARE_DELETE,
        NULL,
        host ? CREATE_ALWAYS : OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
#else
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
#endif
        );

    if (obj.head->file == INVALID_HANDLE_VALUE)
    {
      int err = GetLastError();
      throw err;
    }
      

    if (host)
    {
      for (unsigned long i = 0; i < minimal_size;)
      {
        int zero = 0;
        const int chunk_size = 1000;
        WriteFile(obj.head->file, &zero, chunk_size, NULL, NULL);
        i += 1000;
      }
      FlushFileBuffers(obj.head->file);
    }

    obj.head->mapped =
      CreateFileMapping(
        obj.head->file,
        NULL,
        PAGE_READWRITE,
        0,
        minimal_size,
        NULL);

    if (obj.head->mapped == NULL)
    {
      int err = GetLastError();
      throw err;
    }

    obj.memory = (byte *)
      MapViewOfFile(
        obj.head->mapped,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        minimal_size);
    obj.size = minimal_size;
  }
}