#pragma once

#include <memory>

namespace tsoft
{
  template<typename transport, typename container>
  class connector
  {
    std::auto_ptr<transport> memory_to_write;

  public:
    template<typename... _Args>
    static connector *ConstructContainer(transport *, unsigned long required_size, _Args &&...);
    void DestructContainer();

    connector(transport *, unsigned long required_size);
    ~connector();

    container *operator->() const;
  };
}

#include "connector.hpp"