#include "../LocalConnector/queue/shared_queue.h"
#include "../LocalConnector/connector.h"
#include "../LocalConnector/shared_memory_file/shared_memory_file.h"

using namespace queue;

void main()
{
  typedef queue::shared_mapped_queue wished_container;

  auto *transport = new tsoft::shared_memory_file<0, false>("W:\\a.mem");
  auto &shared = 
    tsoft::connector
    <
    tsoft::shared_memory_file<0, false>,
    wished_container
    >(transport);

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