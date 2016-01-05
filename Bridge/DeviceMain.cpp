#include "DeviceMain.h"

#include "AdapterConstants.h"
#include "AllJoynHelper.h"
#include "BridgeDevice.h"
#include "DeviceMethod.h"
#include "DeviceSignal.h"
#include "Common/Log.h"

namespace
{
  DSB_DECLARE_LOGNAME(DeviceMain);
  static const std::string INTERFACE_NAME_FOR_MAIN_DEVICE(".MainInterface");
}

static std::string BuildBusObjectPath(const std::string& name)
{
  std::string encodedName;
  bridge::AllJoynHelper::EncodeBusObjectName(name, encodedName);
  return "/" + encodedName;
}

bridge::DeviceMain::DeviceMain(BridgeDevice& parent, const shared_ptr<IAdapterDevice>& adapterDevice)
  : ajn::BusObject(BuildBusObjectPath(adapterDevice->GetName()).c_str(), false)
  , m_parent(parent)
  , m_adapterDevice(adapterDevice)
  , m_indexForSignal(1)
  , m_indexForMethod(1)
  , m_interfaceDescription(NULL)
  , m_registeredOnAllJoyn(false)
{
}

bridge::DeviceMain::~DeviceMain()
{
  Shutdown();
}

void
bridge::DeviceMain::Shutdown()
{
}

QStatus
bridge::DeviceMain::Initialize()
{
  QStatus st = ER_OK;

  st = CreateMethodsAndSignals();
  if (st != ER_OK)
    return st;

  if (!m_interfaceDescription)
  {
    DSBLOG_ERROR("Interface description does not exist for bus object");
    return ER_FAIL;
  }

  st = AddInterface(*m_interfaceDescription);
  if (st != ER_OK)
  {
    DSBLOG_WARN("Failed to add interface to bus object: %s", QCC_StatusText(st));
    return st;
  }

  for (auto i : m_deviceMethods)
  {
    const char* methodName = i.first.c_str();
    const ajn::InterfaceDescription::Member* member = m_interfaceDescription->GetMember(methodName);
    if (!member) {
      DSBLOG_WARN("Failed to find InterfaceDescriptionMember for %s", methodName);
      return ER_INVALID_DATA;
    }

    st = AddMethodHandler(member, static_cast<ajn::MessageReceiver::MethodHandler>(&DeviceMain::AJMethod));
    if (st != ER_OK)
    {
      DSBLOG_WARN("Failed to add method handler: %s", QCC_StatusText(st));
      return st;
    }
  }

  st = m_parent.GetBusAttachment().RegisterBusObject(*this);
  if (st != ER_OK)
  {
    DSBLOG_WARN("Failed to register bus object: %s", QCC_StatusText(st));
    return st;
  } else {
    DSBLOG_INFO("Registered bus object for %s", m_interfaceName.c_str());
  }

  m_registeredOnAllJoyn = true;
  return st;
}

bool
bridge::DeviceMain::IsMethodNameUnique(std::string const& name)
{
  return m_deviceMethods.find(name) != m_deviceMethods.end();
}

bool
bridge::DeviceMain::IsSignalNameUnique(std::string const& name)
{
  return m_deviceSignals.find(name) != m_deviceSignals.end();
}

QStatus
bridge::DeviceMain::CreateMethodsAndSignals()
{
  m_interfaceName = m_parent.GetRootNameForInterface();
  std::string tmp = AllJoynHelper::EncodeStringForServiceName(m_parent.GetAdapterDevice()->GetName());
  if (!tmp.empty())
  {
    m_interfaceName += ".";
    m_interfaceName += tmp;
  }
  m_interfaceName += INTERFACE_NAME_FOR_MAIN_DEVICE;

  // TODO: Handle security
  QStatus st = m_parent.GetBusAttachment().CreateInterface(m_interfaceName.c_str(), m_interfaceDescription, ajn::InterfaceSecurityPolicy::AJ_IFC_SECURITY_INHERIT);
  if (st != ER_OK || m_interfaceDescription == NULL)
  {
    DSBLOG_WARN("Failed to create interface for %s: %s", m_interfaceName.c_str(), QCC_StatusText(st));
    return st;
  }

  for (auto adapterMethod : m_adapterDevice->GetMethods())
  {
    auto i = m_deviceMethods.insert(std::make_pair(adapterMethod->GetName(), DeviceMethod(*this, adapterMethod)));
    st = i.first->second.Initialize();
    if (st != ER_OK)
      return st;
  }

  for (auto adapterSignal : m_adapterDevice->GetSignals())
  {
    // change of value signal only concerns IAdapterProperty hence not this class
    if (adapterSignal->GetName() == Constants::CHANGE_OF_VALUE_SIGNAL)
      continue;

    m_deviceSignals.insert(std::make_pair(adapterSignal->GetName(), DeviceSignal(*this, adapterSignal)));
  }

  m_interfaceDescription->Activate();

  return st;
}

ajn::InterfaceDescription*
bridge::DeviceMain::GetInterfaceDescription()
{
  return m_interfaceDescription;
}

void
bridge::DeviceMain::AJMethod(const ajn::InterfaceDescription::Member*, ajn::Message&)
{
  DSBLOG_NOT_IMPLEMENTED();
}
