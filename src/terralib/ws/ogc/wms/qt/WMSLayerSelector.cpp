#include "WMSLayerSelector.h"

//TerraLib
#include "../../../core/Exception.h"
#include "../../../../core/translator/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/WMS2Layer.h"
#include "WMSLayerSelectorDialog.h"

// STL
#include <algorithm>
#include <iterator>
#include <memory>

te::ws::ogc::wms::qt::WMSLayerSelector::WMSLayerSelector(QWidget *parent, Qt::WindowFlags f)
{

}

te::ws::ogc::wms::qt::WMSLayerSelector::~WMSLayerSelector()
{

}

void te::ws::ogc::wms::qt::WMSLayerSelector::set(const std::list<te::da::DataSourceInfoPtr> &datasources)
{
  m_datasources = datasources;
}

std::list<te::map::AbstractLayerPtr> te::ws::ogc::wms::qt::WMSLayerSelector::getLayers()
{
  std::list<te::map::AbstractLayerPtr> layers;

  for(std::list<te::da::DataSourceInfoPtr>::iterator it = m_datasources.begin(); it != m_datasources.end(); ++it)
  {
    te::da::DataSourcePtr datasource = te::da::DataSourceManager::getInstance().find((*it)->getId());

    if(datasource.get() == 0)
    {
      datasource = te::da::DataSourceManager::getInstance().get((*it)->getId(), (*it)->getAccessDriver(), (*it)->getConnInfo());

      if(datasource.get() == 0)
        throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("Could not retrieve the data source instance!"));
    }

    if(!datasource->isOpened())
      datasource->open();

    std::unique_ptr<te::ws::ogc::wms::qt::WMSLayerSelectorDialog> ldialog(new te::ws::ogc::wms::qt::WMSLayerSelectorDialog(static_cast<QWidget*>(parent())));

    ldialog->set(*it, true);

    int retval = ldialog->exec();

    if(retval == QDialog::Rejected)
      continue;

    std::vector<te::ws::ogc::wms::WMSGetMapRequest> requests = ldialog->getCheckedRequests();

    std::transform(requests.begin(), requests.end(), std::back_inserter(layers), te::ws::ogc::wms::WMS2Layer((*it)->getId()));
  }

  return layers;
}

