#include "WMSLayer.h"

#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../maptools/AbstractRenderer.h"
#include "../../../../maptools/RendererFactory.h"
#include "../../../core/Exception.h"
#include "DataSource.h"

//Boost
#include <boost/format.hpp>

const std::string te::ws::ogc::wms::WMSLayer::sm_type("OGCWMSLAYER");

te::ws::ogc::wms::WMSLayer::WMSLayer(te::map::AbstractLayer *parent)
  : te::map::AbstractLayer(parent),
    m_rendererType("OGC_WMS_LAYER_RENDERER")
{

}

te::ws::ogc::wms::WMSLayer::WMSLayer(const std::string &id, te::map::AbstractLayer *parent)
  : te::map::AbstractLayer(id, parent),
    m_rendererType("OGC_WMS_LAYER_RENDERER")
{

}

te::ws::ogc::wms::WMSLayer::WMSLayer(const std::string &id, const std::string &title, te::map::AbstractLayer *parent)
  : te::map::AbstractLayer(id, title, parent),
    m_rendererType("OGC_WMS_LAYER_RENDERER")
{

}

te::ws::ogc::wms::WMSLayer::~WMSLayer()
{

}

std::auto_ptr<te::map::LayerSchema> te::ws::ogc::wms::WMSLayer::getSchema() const
{
  if(m_schema.get())
    return std::auto_ptr<te::map::LayerSchema>(static_cast<te::map::LayerSchema*>(m_schema->clone()));

  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  m_schema = ds->getDataSetType(m_datasetName);

  return std::auto_ptr<te::map::LayerSchema>(static_cast<te::map::LayerSchema*>(m_schema->clone()));
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wms::WMSLayer::getData(te::common::TraverseType travType, const te::common::AccessPolicy accessPolicy) const
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getDataSet() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wms::WMSLayer::getData(const std::string &propertyName, const te::gm::Envelope *e, te::gm::SpatialRelation r, te::common::TraverseType travType, const te::common::AccessPolicy accessPolicy) const
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getDataSet() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wms::WMSLayer::getData(const std::string &propertyName, const te::gm::Geometry *g, te::gm::SpatialRelation r, te::common::TraverseType travType, const te::common::AccessPolicy accessPolicy) const
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getDataSet() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wms::WMSLayer::getData(te::da::Expression *restriction, te::common::TraverseType travType, const te::common::AccessPolicy accessPolicy) const
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("Query operations is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wms::WMSLayer::getData(const te::da::ObjectIdSet *oids, te::common::TraverseType travType, const te::common::AccessPolicy accessPolicy) const
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The ObjectIdSet concept is not supported by the WMS driver!"));
}

bool te::ws::ogc::wms::WMSLayer::isValid() const
{
  if(m_datasourceId.empty() || m_getMapRequest.m_layers.empty())
    return false;

  if(m_getMapRequest.m_width == 0 || m_getMapRequest.m_height == 0 || m_getMapRequest.m_format.empty())
    return false;

  te::da::DataSourcePtr ds;
  try
  {
    ds = te::da::GetDataSource(m_datasourceId, true);
  }
  catch(...)
  {
    return false;
  }

  if(ds.get() == 0 || !ds->isValid() || !ds->isOpened())
    return false;

  return true;
}

void te::ws::ogc::wms::WMSLayer::draw(te::map::Canvas *canvas,
                                      const te::gm::Envelope &bbox, int srid,
                                      const double &scale, bool *cancel)
{
  if(m_rendererType.empty())
    throw te::ws::core::Exception() << te::ErrorDescription((boost::format(TE_TR("Could not draw the WMS layer %1%. The renderer type is empty!")) % getTitle()).str());

  // Try get the defined renderer
  std::auto_ptr<te::map::AbstractRenderer> renderer(te::map::RendererFactory::make(m_rendererType));

  if(renderer.get() == 0)
    throw te::ws::core::Exception() << te::ErrorDescription((boost::format(TE_TR("Could not draw the WMS layer %1%. The renderer %2% could not be created!"))  % getTitle() % m_rendererType).str());

  renderer->draw(this, canvas, bbox, srid, scale, cancel);
}

const std::string &te::ws::ogc::wms::WMSLayer::getType() const
{
  return sm_type;
}

const std::string &te::ws::ogc::wms::WMSLayer::getDataSourceId() const
{
  return m_datasourceId;
}

void te::ws::ogc::wms::WMSLayer::setDataSourceId(const std::string &datasourceId)
{
  m_datasourceId = datasourceId;
}

const std::string &te::ws::ogc::wms::WMSLayer::getRendererType() const
{
  return m_rendererType;
}

void te::ws::ogc::wms::WMSLayer::setRendererType(const std::string &rendererType)
{
  m_rendererType = rendererType;
}

te::ws::ogc::wms::WMSGetMapRequest te::ws::ogc::wms::WMSLayer::getRequest() const
{
  return m_getMapRequest;
}

void te::ws::ogc::wms::WMSLayer::setGetMapRequest(const te::ws::ogc::wms::WMSGetMapRequest &getMapRequest)
{
  m_getMapRequest = getMapRequest;
}

const te::ws::ogc::wms::WMSGetMapResponse te::ws::ogc::wms::WMSLayer::getMap() const
{

  te::da::DataSourcePtr ds;
  try
  {
    ds = te::da::GetDataSource(m_datasourceId, true);
  }
  catch(...)
  {
    return te::ws::ogc::wms::WMSGetMapResponse();
  }

  if(ds.get() == 0 || !ds->isValid() || !ds->isOpened())
    return te::ws::ogc::wms::WMSGetMapResponse();

  te::ws::ogc::wms::da::DataSource* wmsDataSource = dynamic_cast<te::ws::ogc::wms::da::DataSource*>(ds.get());

  if(wmsDataSource == nullptr)
  {
    return te::ws::ogc::wms::WMSGetMapResponse();
  }

  return wmsDataSource->getMap(m_getMapRequest);
}







