#include "../LocalConnector/queue/shared_queue.h"
#include "../LocalConnector/connector.h"
#include "../LocalConnector/shared_memory_file/shared_memory_file.h"

using namespace queue;

void main()
{
  const int size = 12;
  typedef queue::shared_queue<size> wished_container;

  int a = 5, b;

  shared_queue<size> q;
  q.Push(a);
  b = q.Pop<int>();
  q.Push(a);
  b = q.Pop<int>();

  auto *transport = new tsoft::shared_memory_file<sizeof(wished_container), false>("W:\\a.mem");
  auto &shared = *
    tsoft::connector
    <
    tsoft::shared_memory_file<sizeof(wished_container), false>,
    wished_container
    >::ConstructContainer(transport, size);

  shared->Push(a);
  b = shared->Pop<int>();
  shared->Push(a);
  b = shared->Pop<int>();

}