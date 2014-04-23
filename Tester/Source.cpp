#include "../LocalConnector/queue/shared_queue.h"

using namespace queue;

void main()
{
  shared_queue<12> q;
  int a = 5;
  q.Push(a);
  int b = q.Pop<int>();
  q.Push(a);
  b = q.Pop<int>();
}