#include "../LocalConnector/queue/shared_queue.h"
#include "../LocalConnector/connector.h"
#include "../LocalConnector/shared_memory_file/shared_memory_file.h"

using namespace queue;

#include <windows.h>
#include <iostream>

void main()
{
  const int size = sizeof(int)* 1E+5;
  typedef queue::shared_queue<size> wished_container;

  auto *transport = new tsoft::shared_memory_file<sizeof(wished_container), false>("W:\\a.mem");
  auto &shared =
    tsoft::connector
    <
    tsoft::shared_memory_file<sizeof(wished_container), false>,
    wished_container
    >(transport, size);

  auto *q = shared.operator->();

  while (true)
  {
    header copy = q->h;
    Sleep(100);
    int
      writed = q->h.last - copy.last,
      readed = q->h.first - copy.first;

    std::cout
      << " +" << writed
      << " -" << readed
      << " D" << (writed - readed)
      << " F" << 100.0 * q->AvailableBytes() / copy.Size()
      << "     \r";
  }
}