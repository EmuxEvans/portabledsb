#include "DeviceSignal.h"

bridge::DeviceSignal::DeviceSignal(DeviceMain& parent, const shared_ptr<IAdapterSignal>& adapterSignal)
  : m_parent(parent)
  , m_adapterSignal(adapterSignal)
{
}

bridge::DeviceSignal::~DeviceSignal()
{
}

std::string
bridge::DeviceSignal::GetName() const
{
  return m_exposedName;
}
