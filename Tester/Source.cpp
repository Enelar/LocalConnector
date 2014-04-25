#include "../LocalConnector/queue/shared_queue.h"
#include "../LocalConnector/connector.h"
#include "../LocalConnector/shared_memory_file/shared_memory_file.h"

#include <iostream>

using namespace queue;

void main()
{
  const int size = sizeof(int) * 1E+5;
  typedef queue::shared_queue<size> wished_container;

  int a = 5, b;

  shared_queue<size> q;
  q.Push(a);
  b = q.Pop<int>();
  q.Push(a);
  b = q.Pop<int>();

  auto *transport = new tsoft::shared_memory_file<sizeof(wished_container), true>("W:\\a.mem");
  auto &shared = *
    tsoft::connector
    <
    tsoft::shared_memory_file<sizeof(wished_container), true>,
    wished_container
    >::ConstructContainer(transport, size);

  while (1)
  {
    if (shared->IsEmpty())
      continue;
    try
    {
      b = shared->Pop<int>();
      std::cout << "Received " << b << std::endl;
    }
    catch (not_ready &)
    {
      std::cout << "Locked for writing" << std::endl;
    }
    catch (empty &)
    {

    }
    catch (pop_fault &)
    {

    }
  }
}