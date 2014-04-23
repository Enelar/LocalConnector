#include "header.h"

using namespace queue;

header::header(long s)
: size(s)
{

}

bool header::IsEmpty() const
{
  return first == last;
}

unsigned long header::Available() const
{
  long available = size - (last - first);
  if (available < 0)
    throw "Available less than zero";
  return available;
}

unsigned long header::First() const
{
  return first % Size();
}

unsigned long header::Last() const
{
  return last % Size();
}

unsigned long header::Size() const
{
  return size;
}