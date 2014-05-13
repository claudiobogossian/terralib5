/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/qt/widgets/layer/utils/DataSet2Layer.cpp

  \brief ....
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../../../raster/Grid.h"
#include "../../../../raster/RasterProperty.h"
#include "../../../../se/Utils.h"
#include "../../../../srs/Config.h"
#include "../../Exception.h"
#include "DataSet2Layer.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::qt::widgets::DataSet2Layer::DataSet2Layer(const std::string& datasourceId)
  : m_datasourceId(datasourceId)
{
}

te::map::DataSetLayerPtr te::qt::widgets::DataSet2Layer::operator()(const te::da::DataSetTypePtr& dataset) const
{
  static boost::uuids::basic_random_generator<boost::mt19937> gen;

  if(dataset.get() == 0)
    throw Exception(TR_QT_WIDGETS("Can not convert a NULL dataset to a layer!"));

  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  std::string title = dataset->getTitle().empty() ? dataset->getName() : dataset->getTitle();
  
  te::map::DataSetLayerPtr layer(new te::map::DataSetLayer(id, title));
  layer->setDataSetName(dataset->getName());
  layer->setDataSourceId(m_datasourceId);
  layer->setVisibility(te::map::NOT_VISIBLE);
  layer->setRendererType("ABSTRACT_LAYER_RENDERER");

  if(dataset->size() == 0)
  {
    te::da::DataSourcePtr ds(te::da::DataSourceManager::getInstance().find(m_datasourceId));
    te::da::LoadProperties(dataset.get(), m_datasourceId);
  }

  if(dataset->hasGeom())
  {
    te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(dataset.get());
    std::auto_ptr<te::gm::Envelope> mbr(te::da::GetExtent(dataset->getName(), gp->getName(), m_datasourceId));
    layer->setSRID(gp->getSRID());
    layer->setExtent(*mbr);
    layer->setStyle(te::se::CreateFeatureTypeStyle(gp->getGeometryType()));
  }
  else if(dataset->hasRaster())
  {
    te::rst::Grid* grid = te::da::GetFirstRasterProperty(dataset.get())->getGrid();

    layer->setSRID(grid->getSRID());
    layer->setExtent(*(grid->getExtent()));
    layer->setStyle(te::se::CreateCoverageStyle(te::da::GetFirstRasterProperty(dataset.get())->getBandProperties()));
  }
  else
  {
    layer->setSRID(TE_UNKNOWN_SRS);
    //layer->setExtent(te::gm::Envelope());
  }

  return layer;
}

te::map::DataSetLayerPtr te::qt::widgets::DataSet2Layer::operator()(const te::da::DataSetTypePtr& dataset, const std::string& geomPropertyName) const
{
  static boost::uuids::basic_random_generator<boost::mt19937> gen;

  if(dataset.get() == 0)
    throw Exception(TR_QT_WIDGETS("Can not convert a NULL dataset to a layer!"));

  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  std::string title = dataset->getTitle().empty() ? dataset->getName() : dataset->getTitle();
  
  te::map::DataSetLayerPtr layer(new te::map::DataSetLayer(id, title));
  layer->setDataSetName(dataset->getName());
  layer->setDataSourceId(m_datasourceId);
  layer->setVisibility(te::map::NOT_VISIBLE);
  layer->setRendererType("ABSTRACT_LAYER_RENDERER");

  if(dataset->size() == 0)
  {
    te::da::DataSourcePtr ds(te::da::DataSourceManager::getInstance().find(m_datasourceId));
    te::da::LoadProperties(dataset.get(), m_datasourceId);
  }

  if(dataset->hasGeom())
  {
    te::gm::GeometryProperty* gp = 0;
    geomPropertyName.empty() ?  gp = te::da::GetFirstGeomProperty(dataset.get()) : gp = dynamic_cast<te::gm::GeometryProperty*>(dataset->getProperty(geomPropertyName));

    assert(gp);

    std::auto_ptr<te::gm::Envelope> mbr(te::da::GetExtent(dataset->getName(), gp->getName(), m_datasourceId));
    layer->setSRID(gp->getSRID());
    layer->setExtent(*mbr);
    layer->setStyle(te::se::CreateFeatureTypeStyle(gp->getGeometryType()));
    layer->setGeomPropertytName(geomPropertyName);
  }
  else if(dataset->hasRaster())
  {
    te::rst::RasterProperty* rp = 0;
    geomPropertyName.empty() ?  rp = te::da::GetFirstRasterProperty(dataset.get()) : rp = dynamic_cast<te::rst::RasterProperty*>(dataset->getProperty(geomPropertyName));

    assert(rp);

    te::rst::Grid* grid = rp->getGrid();

    layer->setSRID(grid->getSRID());
    layer->setExtent(*(grid->getExtent()));
    layer->setStyle(te::se::CreateCoverageStyle(te::da::GetFirstRasterProperty(dataset.get())->getBandProperties()));
    layer->setGeomPropertytName(geomPropertyName);
  }
  else
  {
    layer->setSRID(TE_UNKNOWN_SRS);
    //layer->setExtent(te::gm::Envelope());
  }

  return layer;
}
