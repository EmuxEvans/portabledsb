#pragma once

#include "Common/defines.h"

#include <alljoyn/InterfaceDescription.h>
#include <vector>

namespace bridge
{

class BridgeDevice;
class IAdapterProperty;

class PropertyInterface
{
public:
  PropertyInterface(BridgeDevice&, const shared_ptr<IAdapterProperty>&);
  virtual ~PropertyInterface();
  ajn::InterfaceDescription* GetInterfaceDescription();

private:
  BridgeDevice& m_bridgeDevice;
  shared_ptr<IAdapterProperty> m_adapterProperty;
};

}
