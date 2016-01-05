#pragma once

#include <string>

#include "IAdapter.h"
#include "DeviceMain.h"

namespace bridge
{
  class DeviceSignal
  {
  public:
    DeviceSignal(DeviceMain&, const shared_ptr<IAdapterSignal>&);
    virtual ~DeviceSignal();

    std::string GetName() const;

  private:
    DeviceMain& m_parent;
    shared_ptr<IAdapterSignal> m_adapterSignal;

    std::string m_exposedName;
    std::string m_signature;
    std::string m_parameterNames;
  };
}
