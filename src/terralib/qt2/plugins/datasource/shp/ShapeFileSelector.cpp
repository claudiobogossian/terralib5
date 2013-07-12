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
  \file terralib/qt/plugins/datasource/shp/ShapeFileSelector.h

  \brief A simple layer selector for Esri Shape File data sources.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess2/dataset/DataSetType.h"
#include "../../../../dataaccess2/datasource/DataSource.h"
//#include "../../../../dataaccess/datasource/DataSourceCatalog.h"
//#include "../../../../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../../../../dataaccess2/datasource/DataSourceManager.h"
//#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../maptools2/DataSetLayer.h"
#include "../../../widgets/Exception.h"
#include "ShapeFileSelector.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::qt::plugins::shp::ShapeFileSelector::ShapeFileSelector(QWidget* parent, Qt::WindowFlags f)
  : te::qt::widgets::AbstractLayerSelector(parent, f)
{
}

te::qt::plugins::shp::ShapeFileSelector::~ShapeFileSelector()
{
}

void te::qt::plugins::shp::ShapeFileSelector::set(const std::list<te::da::DataSourceInfoPtr>& datasources)
{
  m_datasources = datasources;
}

std::list<te::map::AbstractLayerPtr> te::qt::plugins::shp::ShapeFileSelector::getLayers()
{
  //for(std::list<te::map::AbstractLayerPtr>::iterator it = m_datasources.begin())

  //if(m_datasource.get() == 0)
  //  throw te::qt::widgets::Exception(TR_QT_WIDGETS("Can not select layer from a NULL data source!"));

  std::list<te::map::AbstractLayerPtr> layers;

  //const std::string& dsId = m_datasource->getId();

  //te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().find(dsId);

  //if(ds.get() == 0)
  //  ds = te::da::DataSourceManager::getInstance().open(dsId, m_datasource->getAccessDriver(), m_datasource->getConnInfo());

  //te::da::DataSourceCatalog* catalog = ds->getCatalog();

  //assert(catalog != 0);

  //te::da::DataSetTypePtr dt = catalog->getDataSetType(m_datasource->getTitle());

  //if(dt.get() == 0)
  //{
  //  std::auto_ptr<te::da::DataSourceTransactor> transactor(ds->getTransactor());

  //  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

  //  dt.reset(cloader->getDataSetType(m_datasource->getTitle(), true));

  //  if(dt->hasGeom())
  //  {
  //    std::auto_ptr<te::gm::Envelope> mbr(cloader->getExtent(dt->getDefaultGeomProperty()));
  //    dt->getDefaultGeomProperty()->setExtent(mbr.release());
  //  }

  //  catalog->add(dt);
  //}

  //boost::uuids::basic_random_generator<boost::mt19937> gen;
  //boost::uuids::uuid u = gen();
  //std::string id = boost::uuids::to_string(u);

  //te::map::DataSetLayerPtr layer(new te::map::DataSetLayer(id, dt->getTitle().empty() ? dt->getName() : dt->getTitle()));
  //layer->setDataSetName(dt->getName());
  //layer->setDataSourceId(ds->getId());
  //layer->setVisibility(te::map::NOT_VISIBLE);

  //if(dt->hasGeom())
  //{
  //  te::gm::GeometryProperty* gp = dt->getDefaultGeomProperty();
  //  layer->setSRID(gp->getSRID());
  //  layer->setExtent(gp->getExtent() ? te::gm::Envelope(*(gp->getExtent())) : te::gm::Envelope());
  //}
  //else
  //{
  //  layer->setSRID(4326);
  //  layer->setExtent(te::gm::Envelope(-180.0, -90.0, 180.0, 90.0));
  //}

  //layers.push_back(layer);

  return layers;
}

