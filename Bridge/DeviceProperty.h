#pragma once

#include "IAdapter.h"
#include "PropertyInterface.h"

#include "alljoyn/BusObject.h"

namespace bridge
{
  class DeviceProperty
  {
  public:
    DeviceProperty(shared_ptr<IAdapterProperty>, shared_ptr<PropertyInterface>, BridgeDevice&);
    QStatus Initialize();

    std::string GetPathName();
    ajn::BusObject& GetBusObject();
    shared_ptr<PropertyInterface> GetPropertyInterface();
    shared_ptr<IAdapterProperty> GetAdapterProperty();

  private:
    shared_ptr<IAdapterProperty> m_adapterProperty;
    shared_ptr<PropertyInterface> m_propertyInterface;
    BridgeDevice& m_bridgeDevice;
  };
}

