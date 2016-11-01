#include "WMS2Layer.h"

//TerraLib
#include "../../../core/Exception.h"
#include "../../../../core/translator/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../geometry/Envelope.h"
#include "Transactor.h"
#include "../client/DataTypes.h"
#include "../../../../common/StringUtils.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

te::ws::ogc::wms::WMS2Layer::WMS2Layer(const std::string dataSourceId)
{
  m_dataSourceId = dataSourceId;
}

te::ws::ogc::wms::WMSLayerPtr te::ws::ogc::wms::WMS2Layer::operator()(const te::ws::ogc::wms::WMSGetMapRequest &request) const
{
  static boost::uuids::basic_random_generator<boost::mt19937> gen;

  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  te::da::DataSourcePtr ds(te::da::DataSourceManager::getInstance().find(m_dataSourceId));

  if(ds.get() == 0)
  {
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("DataSource is NULL for this DataSetType."));
  }

  te::ws::ogc::wms::da::Transactor* transactor = dynamic_cast<te::ws::ogc::wms::da::Transactor*>(ds->getTransactor().release());

  te::ws::ogc::wms::Layer wmsLayer = transactor->getLayerByName(request.m_layers[0]);

  std::string title = wmsLayer.m_title.empty() ? wmsLayer.m_name : wmsLayer.m_title;

  te::ws::ogc::wms::WMSLayerPtr layer(new te::ws::ogc::wms::WMSLayer(id, title));

  //Using the default box and CRS but it should be choosen by the user.
  te::ws::ogc::wms::BoundingBox bbox = request.m_boundingBox;
  std::string crs = request.m_srs;

  te::gm::Envelope extent (bbox.m_minX, bbox.m_minY, bbox.m_maxX, bbox.m_maxY);

  std::vector<std::string> crsValues = te::common::SplitString(crs, ':');

  // Integer Value.
  crs = crsValues[1];

  int srid = boost::lexical_cast<int>(crs.c_str());
  layer->setSRID(srid);
  layer->setDataSourceId(m_dataSourceId);
  layer->setExtent(extent);
  layer->setDataSetName(request.m_layers[0]);
  layer->setVisibility(te::map::NOT_VISIBLE);

  layer->setGetMapRequest(request);

  return layer;
}
