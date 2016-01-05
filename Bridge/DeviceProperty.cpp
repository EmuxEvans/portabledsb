#include "DeviceProperty.h"

#include "Common/Log.h"

namespace
{
  DSB_DECLARE_LOGNAME(DeviceProperty);
}

bridge::DeviceProperty::DeviceProperty(shared_ptr<IAdapterProperty> adapterProperty, shared_ptr<PropertyInterface> propertyInterface, BridgeDevice& bridgeDevice)
  : m_adapterProperty(adapterProperty)
  , m_propertyInterface(propertyInterface)
  , m_bridgeDevice(bridgeDevice)
{
}

QStatus Initialize()
{
  DSBLOG_NOT_IMPLEMENTED();
  return ER_NOT_IMPLEMENTED;
}
