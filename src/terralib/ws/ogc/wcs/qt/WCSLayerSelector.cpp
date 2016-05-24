#include "WCSLayerSelector.h"

te::ws::ogc::wcs::qt::WCSLayerSelector::WCSLayerSelector(QWidget *parent, Qt::WindowFlags f)
  : te::qt::widgets::AbstractLayerSelector(parent, f)
{

}

te::ws::ogc::wcs::qt::WCSLayerSelector::~WCSLayerSelector()
{

}


void te::ws::ogc::wcs::qt::WCSLayerSelector::set(const std::list<te::da::DataSourceInfoPtr> &datasources)
{

}


std::list<te::map::AbstractLayerPtr> te::ws::ogc::wcs::qt::WCSLayerSelector::getLayers()
{
  return std::list<te::map::AbstractLayerPtr>();
}
