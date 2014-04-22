#include "../LocalConnector/queue/shared_queue.h"

using namespace queue;

void main()
{
  shared_queue<100> q;
  int a = 5;
  q.Push<int>(a);
  int b = q.Pop<int>();
}