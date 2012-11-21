#include "Plugin.h"
#include "OGRType.h"

//! TerraLib include files
#include <terralib/common/Config.h>
#include <terralib/common/Translator.h>
#include <terralib/common/Logger.h>
#include <terralib/qt/widgets/datasource/core/DataSourceTypeManager.h>

namespace plg_ogr = qt_af::plugin::ogr;
namespace te_qt = te::qt::widgets;

plg_ogr::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo) :
te::plugin::Plugin(pluginInfo)
{
}

plg_ogr::Plugin::~Plugin() 
{
}

void plg_ogr::Plugin::startup()
{
  if(m_initialized)
    return;

  te_qt::DataSourceTypeManager::getInstance().add(new OGRType);

// it initializes the Translator support for the TerraLib PostGIS driver support
  TE_ADD_TEXT_DOMAIN(AF_OGR_TEXT_DOMAIN, AF_OGR_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(AF_TR_OGR("TerraLib Qt-OGR driver startup!"));

  m_initialized = true;
}

void plg_ogr::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te_qt::DataSourceTypeManager::getInstance().remove("OGR");

  TE_LOG_TRACE(AF_TR_OGR("TerraLib OGR driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(plg_ogr::Plugin)
