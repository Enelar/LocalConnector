#include "raw_block.h"

using namespace queue;

char *raw_block::Begin() const
{
  const raw_block *first_data_byte_location = this + 1;
  raw_block *we_can_write_into_in_cause_it_different_object = const_cast<raw_block *>(first_data_byte_location);
  return reinterpret_cast<char *>(we_can_write_into_in_cause_it_different_object);
}

char *raw_block::End() const
{
  return Begin() + size;
}

bool raw_block::IsDirty() const
{
  return dirty;
}

raw_block *raw_block::Next() const
{
  // because we know that end of this block is begin of next
  // Caution: pointer should be cycled, if current clock was physically last.
  return reinterpret_cast<raw_block *>(End());
}