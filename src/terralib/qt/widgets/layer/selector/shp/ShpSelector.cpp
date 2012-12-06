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
  \file terralib/qt/widgets/layer/selector/shp/ShpSelector.cpp

  \brief ....
*/

// TerraLib
#include "../../../../../common/Translator.h"
#include "../../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../../dataaccess/datasource/DataSource.h"
#include "../../../../../dataaccess/datasource/DataSourceCatalog.h"
#include "../../../../../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/GeometryProperty.h"
#include "../../../../../maptools/AbstractLayer.h"
#include "../../../../../maptools/Layer.h"
#include "../../../../../maptools/QueryLayer.h"
#include "../../../Exception.h"
#include "ShpSelector.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::qt::widgets::ShpSelector::ShpSelector(QWidget* parent, Qt::WindowFlags f)
  : AbstractLayerSelector(parent, f)
{
}

te::qt::widgets::ShpSelector::~ShpSelector()
{
}

void te::qt::widgets::ShpSelector::set(const te::da::DataSourceInfoPtr& ds)
{
  m_datasource = ds;
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::ShpSelector::getLayers()
{
  if(m_datasource.get() == 0)
    throw Exception(TR_QT_WIDGETS("Can not select layer from a NULL data source!"));

  std::list<te::map::AbstractLayerPtr> layers;

  const std::string& dsId = m_datasource->getId();

  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().find(dsId);

  if(ds.get() == 0)
    ds = te::da::DataSourceManager::getInstance().open(dsId, m_datasource->getAccessDriver(), m_datasource->getConnInfo());

  te::da::DataSourceCatalog* catalog = ds->getCatalog();

  assert(catalog != 0);

  te::da::DataSetTypePtr dt = catalog->getDataSetType(m_datasource->getTitle());

  if(dt.get() == 0)
  {
    std::auto_ptr<te::da::DataSourceTransactor> transactor(ds->getTransactor());

    std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

    dt.reset(cloader->getDataSetType(m_datasource->getTitle(), true));

    if(dt->hasGeom())
    {
      std::auto_ptr<te::gm::Envelope> mbr(cloader->getExtent(dt->getDefaultGeomProperty()));
      dt->getDefaultGeomProperty()->setExtent(mbr.release());
    }

    catalog->add(dt);
  }

  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  te::map::LayerPtr layer(new te::map::Layer(id, dt->getTitle().empty() ? dt->getName() : dt->getTitle()));
  layer->setDataSetName(dt->getName());
  layer->setDataSource(ds->getId());
  layer->setVisibility(te::map::NOT_VISIBLE);

  if(dt->hasGeom())
  {
    te::gm::GeometryProperty* gp = dt->getDefaultGeomProperty();
    layer->setSRID(gp->getSRID());
    layer->setExtent(gp->getExtent() ? new te::gm::Envelope(*(gp->getExtent())) : new te::gm::Envelope);
  }
  else
  {
    layer->setSRID(4326);
    layer->setExtent(new te::gm::Envelope(-180.0, -90.0, 180.0, 90.0));
  }

  layers.push_back(layer);

  return layers;
}

