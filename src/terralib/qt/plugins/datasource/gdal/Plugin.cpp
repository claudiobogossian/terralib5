#include "Plugin.h"
#include "GDALType.h"

//! TerraLib include files
#include <terralib/common/Config.h>
#include <terralib/common/Translator.h>
#include <terralib/common/Logger.h>
#include <terralib/qt/widgets/datasource/core/DataSourceManager.h>
#include <terralib/qt/widgets/datasource/core/DataSourceTypeManager.h>

namespace plg_gdal = qt_af::plugin::gdal;
namespace te_qt = te::qt::widgets;

plg_gdal::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo) :
te::plugin::Plugin(pluginInfo)
{
}

plg_gdal::Plugin::~Plugin() 
{
}

void plg_gdal::Plugin::startup()
{
  if(m_initialized)
    return;

  te_qt::DataSourceTypeManager::getInstance().add(new GDALType);

// it initializes the Translator support for the TerraLib PostGIS driver support
  TE_ADD_TEXT_DOMAIN(AF_GDAL_TEXT_DOMAIN, AF_GDAL_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(AF_TR_GDAL("TerraLib Qt-GDAL driver startup!"));

  m_initialized = true;
}

void plg_gdal::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te_qt::DataSourceManager::getInstance().removeByType("GDAL");
  te_qt::DataSourceTypeManager::getInstance().remove("GDAL");

  TE_LOG_TRACE(AF_TR_GDAL("TerraLib GDAL driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(plg_gdal::Plugin)
