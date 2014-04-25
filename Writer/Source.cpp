#include "../LocalConnector/queue/shared_queue.h"
#include "../LocalConnector/connector.h"
#include "../LocalConnector/shared_memory_file/shared_memory_file.h"

using namespace queue;

void main()
{
  const int size = 200;
  typedef queue::shared_queue<size> wished_container;

  int a = 5, b;

  shared_queue<size> q;
  q.Push(a);
  b = q.Pop<int>();
  q.Push(a);
  b = q.Pop<int>();

  auto *transport = new tsoft::shared_memory_file<sizeof(wished_container), false>("W:\\a.mem");
  auto &shared = 
    tsoft::connector
    <
    tsoft::shared_memory_file<sizeof(wished_container), false>,
    wished_container
    >(transport, size);

  for (int i = 0; i < 1E+5; i++)
    while (true)
      try 
      { 
        shared->Push(i);
        break;
      }
      catch (overloaded &)
      {

      }
}