#include "Plugin.h"
#include "WCSType.h"

//! TerraLib include files
#include <terralib/common/Config.h>
#include <terralib/common/Translator.h>
#include <terralib/common/Logger.h>
#include <terralib/qt/widgets/datasource/core/DataSourceManager.h>
#include <terralib/qt/widgets/datasource/core/DataSourceTypeManager.h>

namespace plg_wcs = qt_af::plugin::wcs;
namespace te_qt = te::qt::widgets;

plg_wcs::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo) :
te::plugin::Plugin(pluginInfo)
{
}

plg_wcs::Plugin::~Plugin() 
{
}

void plg_wcs::Plugin::startup()
{
  if(m_initialized)
    return;

  te_qt::DataSourceTypeManager::getInstance().add(new WCSType);

// it initializes the Translator support for the TerraLib WCS driver support
  TE_ADD_TEXT_DOMAIN(AF_WCS_TEXT_DOMAIN, AF_WCS_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(AF_TR_WCS("TerraLib Qt-WCS driver startup!"));

  m_initialized = true;
}

void plg_wcs::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te_qt::DataSourceManager::getInstance().removeByType("WCS");
  te_qt::DataSourceTypeManager::getInstance().remove("WCS");

  TE_LOG_TRACE(AF_TR_WCS("TerraLib WCS driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(plg_wcs::Plugin)
