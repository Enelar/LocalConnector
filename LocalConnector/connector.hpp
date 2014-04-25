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
  template<typename... _Args>
  static connector<transport, container> *connector<transport, container>::ConstructContainer(transport *t, unsigned long required_size, _Args &&...args)
  {
    std::unique_ptr<connector<transport, container>> ret;
    try
    {
      ret = std::make_unique<connector<transport, container>>(t, required_size);
      container *ptr = (ret->operator->());

      new (ptr)container(std::forward<_Args>(args)...);
      return ret.release();
    }
    catch (...)
    {
      if (ret)
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