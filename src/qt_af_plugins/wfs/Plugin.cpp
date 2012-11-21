#include "Plugin.h"
#include "WFSType.h"

//! TerraLib include files
#include <terralib/common/Config.h>
#include <terralib/common/Translator.h>
#include <terralib/common/Logger.h>
#include <terralib/qt/widgets/datasource/core/DataSourceTypeManager.h>

namespace plg_wfs = qt_af::plugin::wfs;
namespace te_qt = te::qt::widgets;

plg_wfs::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo) :
te::plugin::Plugin(pluginInfo)
{
}

plg_wfs::Plugin::~Plugin() 
{
}

void plg_wfs::Plugin::startup()
{
  if(m_initialized)
    return;

  te_qt::DataSourceTypeManager::getInstance().add(new WFSType);

// it initializes the Translator support for the TerraLib WCS driver support
  TE_ADD_TEXT_DOMAIN(AF_WFS_TEXT_DOMAIN, AF_WFS_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(AF_TR_WFS("TerraLib Qt-WFS driver startup!"));

  m_initialized = true;
}

void plg_wfs::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te_qt::DataSourceTypeManager::getInstance().remove("WFS");

  TE_LOG_TRACE(AF_TR_WFS("TerraLib WFS driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(plg_wfs::Plugin)
