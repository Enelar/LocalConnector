#pragma once

#include "connector.h"

namespace tsoft
{
  template<typename transport, typename container>
  connector<transport, container>::connector(transport *t, unsigned long required_size)
    : memory_to_write(t)
  {
  }

  template<typename transport, typename container>
  connector<transport, container>::~connector()
  {
  }

  template<typename transport, typename container>
  connector<transport, container> *connector<transport, container>::ConstructContainer(transport *t, unsigned long required_size)
  {
    std::auto_ptr<connector<transport, container>> ret;
    try
    {
      ret.reset(new connector<transport, container>(t, required_size));
      container *ptr = (ret->operator->());

      new (ptr)container();
      return ret.release();
    }
    catch (...)
    {
      if (ret.get())
        ret->memory_to_write.release();
      throw;
    }
  }

  template<typename transport, typename container>
  void connector<transport, container>::DestructContainer()
  {
    container *ptr = (this->operator->());
    ptr->~container();
  }

  template<typename transport, typename container>
  container *connector<transport, container>::operator->() const
  {
    return reinterpret_cast<container *>(memory_to_write->Memory());
  }
}