#include "Plugin.h"
#include "PostGISType.h"

//! TerraLib include files
#include <terralib/common/Config.h>
#include <terralib/common/Translator.h>
#include <terralib/common/Logger.h>
#include <terralib/qt/widgets/datasource/core/DataSourceTypeManager.h>

namespace plg_pgis = qt_af::plugin::pgis;
namespace te_qt = te::qt::widgets;

plg_pgis::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo) :
te::plugin::Plugin(pluginInfo)
{
}

plg_pgis::Plugin::~Plugin() 
{
}

void plg_pgis::Plugin::startup()
{
  if(m_initialized)
    return;

  te_qt::DataSourceTypeManager::getInstance().add(new PostGISType);

// it initializes the Translator support for the TerraLib PostGIS driver support
  TE_ADD_TEXT_DOMAIN(AF_PGIS_TEXT_DOMAIN, AF_PGIS_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(AF_TR_PGIS("TerraLib Qt-PGIS driver startup!"));

  m_initialized = true;
}

void plg_pgis::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te_qt::DataSourceTypeManager::getInstance().remove("POSTGIS");

  TE_LOG_TRACE(AF_TR_PGIS("TerraLib PGIS driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(plg_pgis::Plugin)
