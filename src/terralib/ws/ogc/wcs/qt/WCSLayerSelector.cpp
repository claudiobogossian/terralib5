#include "WCSLayerSelector.h"

// TerraLib
#include "../../../../core/translator/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../../../qt/widgets/dataset/selector/DataSetSelectorDialog.h"
#include "../../../../qt/widgets/layer/utils/DataSet2Layer.h"

te::ws::ogc::wcs::qt::WCSLayerSelector::WCSLayerSelector(QWidget *parent, Qt::WindowFlags f)
  : te::qt::widgets::AbstractLayerSelector(parent, f)
{

}

te::ws::ogc::wcs::qt::WCSLayerSelector::~WCSLayerSelector()
{

}


void te::ws::ogc::wcs::qt::WCSLayerSelector::set(const std::list<te::da::DataSourceInfoPtr> &datasources)
{
  m_datasources = datasources;
}


std::list<te::map::AbstractLayerPtr> te::ws::ogc::wcs::qt::WCSLayerSelector::getLayers()
{
  std::list<te::map::AbstractLayerPtr> layers;

  for(std::list<te::da::DataSourceInfoPtr>::iterator it = m_datasources.begin(); it != m_datasources.end(); ++it)
  {
    te::da::DataSourcePtr datasource = te::da::DataSourceManager::getInstance().find((*it)->getId());

    if(datasource.get() == 0)
    {
      datasource = te::da::DataSourceManager::getInstance().get((*it)->getId(), (*it)->getAccessDriver(), (*it)->getConnInfo());

      //if(datasource.get() == 0)
        //throw Exception(TE_TR("Could not retrieve the data source instance!"));
    }

    if(!datasource->isOpened())
      datasource->open();

    std::auto_ptr<te::qt::widgets::DataSetSelectorDialog> ldialog(new te::qt::widgets::DataSetSelectorDialog(static_cast<QWidget*>(parent())));

    ldialog->set(*it, true);

    int retval = ldialog->exec();

    if(retval == QDialog::Rejected)
      continue;

    std::list<te::da::DataSetTypePtr> datasets = ldialog->getCheckedDataSets();
    std::list<std::string> geomProps = ldialog->getCheckedGeomProperties();

    if (datasets.size() == 0)
    {
      return std::list<te::map::AbstractLayerPtr>();
    }
    else
    {
      std::transform(datasets.begin(), datasets.end(), geomProps.begin(), std::back_inserter(layers), te::qt::widgets::DataSet2Layer((*it)->getId()));
    }
  }

  return layers;
}
