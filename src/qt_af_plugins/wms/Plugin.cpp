#include "Plugin.h"
#include "WMSType.h"

//! TerraLib include files
#include <terralib/common/Config.h>
#include <terralib/common/Translator.h>
#include <terralib/common/Logger.h>
#include <terralib/qt/widgets/datasource/core/DataSourceTypeManager.h>

namespace plg_wms = qt_af::plugin::wms;
namespace te_qt = te::qt::widgets;

plg_wms::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo) :
te::plugin::Plugin(pluginInfo)
{
}

plg_wms::Plugin::~Plugin() 
{
}

void plg_wms::Plugin::startup()
{
  if(m_initialized)
    return;

  te_qt::DataSourceTypeManager::getInstance().add(new WMSType);

// it initializes the Translator support for the TerraLib WCS driver support
  TE_ADD_TEXT_DOMAIN(AF_WMS_TEXT_DOMAIN, AF_WMS_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(AF_TR_WMS("TerraLib Qt-WMS driver startup!"));

  m_initialized = true;
}

void plg_wms::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te_qt::DataSourceTypeManager::getInstance().remove("WMS");

  TE_LOG_TRACE(AF_TR_WMS("TerraLib WMS driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(plg_wms::Plugin)
