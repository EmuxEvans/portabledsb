#include "DeviceMethod.h"

#include "AllJoynHelper.h"
#include "Common/Log.h"
#include "DeviceMain.h"

#include <sstream>

namespace
{
  DSB_DECLARE_LOGNAME(DeviceMethod);
}

bridge::DeviceMethod::DeviceMethod(DeviceMain& parent, const shared_ptr<IAdapterMethod>& adapterMethod)
  : m_parent(parent)
  , m_adapterMethod(adapterMethod)
{
}

bridge::DeviceMethod::~DeviceMethod()
{
}

uint32_t
bridge::DeviceMethod::InvokeMethod(ajn::Message const&, ajn::MsgArg*, size_t)
{
  return 0;
}

QStatus
bridge::DeviceMethod::Initialize()
{
  if (!m_adapterMethod.get())
  {
    DSBLOG_WARN("DeviceMethod has invalid ptr to AdapterMethod");
    return ER_INVALID_DATA;
  }

  QStatus st = SetName(m_adapterMethod->GetName());
  if (st != ER_OK)
    return st;

  st = BuildSignature(m_adapterMethod->GetInputParams(), m_inSignature, m_parameterNames);
  if (st != ER_OK)
    return st;

  st = BuildSignature(m_adapterMethod->GetOutputParams(), m_outSignature, m_parameterNames);
  if (st != ER_OK)
    return st;

  st = m_parent.GetInterfaceDescription()->AddMethod(m_exposedName.c_str(), m_inSignature.c_str(), m_outSignature.c_str(), m_parameterNames.c_str());
  if (st != ER_OK)
  {
    DSBLOG_WARN("Failed to add method %s to interface description: %s", m_exposedName.c_str(),  QCC_StatusText(st));
    return st;
  }

  return ER_OK;
}

QStatus
bridge::DeviceMethod::SetName(std::string const& name)
{
  if (name.empty())
    return ER_INVALID_DATA;

  m_exposedName = AllJoynHelper::EncodePropertyOrMethodOrSignalName(name);
  if (!m_parent.IsMethodNameUnique(m_exposedName))
  {
    m_exposedName += '_';
    std::ostringstream tmp;
    tmp << m_parent.GetIndexForMethod();
    m_exposedName += tmp.str();
  }
  return ER_OK;
}

QStatus
bridge::DeviceMethod::BuildSignature(AdapterValueVector const& adapterValues, std::string& signature, std::string& parameterNames)
{
  signature.clear();

  for (auto adapterValue : adapterValues)
  {

  }
  return ER_NOT_IMPLEMENTED;
}

