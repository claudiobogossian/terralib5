/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/DataSetLayer.cpp

  \brief A layer with reference to a dataset.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/Fields.h"
#include "../dataaccess/query/From.h"
#include "../dataaccess/query/FromItem.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/Where.h"
#include "../dataaccess/utils/Utils.h"
#include "DataSetLayer.h"
#include "Exception.h"
#include "RendererFactory.h"
#include "Utils.h"

// Boost
#include <boost/format.hpp>

// STL
#include <memory>

const std::string te::map::DataSetLayer::sm_type("DATASETLAYER");

te::map::DataSetLayer::DataSetLayer(AbstractLayer* parent)
  : AbstractLayer(parent)
{
}

te::map::DataSetLayer::DataSetLayer(const std::string& id, AbstractLayer* parent)
  : AbstractLayer(id, parent)
{
}

te::map::DataSetLayer::DataSetLayer(const std::string& id,
                                    const std::string& title,
                                    AbstractLayer* parent)
  : AbstractLayer(id, title, parent)
{
}

te::map::DataSetLayer::~DataSetLayer()
{
}

const te::map::LayerSchema* te::map::DataSetLayer::getSchema(const bool full) const
{
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  // Get a transactor
  std::auto_ptr<te::da::DataSourceTransactor> t(ds->getTransactor());
  assert(t.get());

  // Get a catalog loader
  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(t->getCatalogLoader());
  assert(cloader.get());

  return cloader->getDataSetType(m_datasetName, full);
}

te::da::DataSet* te::map::DataSetLayer::getData(te::common::TraverseType travType, 
                                                te::common::AccessPolicy rwRole) const
{
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  // Get a transactor
  std::auto_ptr<te::da::DataSourceTransactor> t(ds->getTransactor());
  assert(t.get());

  std::auto_ptr<te::da::DataSet> dataset(t->getDataSet(m_datasetName, travType, rwRole));

  // TODO: Need review: behaviour of te::mem::DataSet + te::rst::Raster.
  std::size_t rpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::RASTER_TYPE);
  if(rpos != std::string::npos)
    return dataset.release();

  return DataSet2Memory(dataset.get());
}

te::da::DataSet* te::map::DataSetLayer::getData(const te::gm::Envelope& e,
                                                te::gm::SpatialRelation r,
                                                te::common::TraverseType travType,
                                                te::common::AccessPolicy rwRole) const
{
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  // Get a transactor
  std::auto_ptr<te::da::DataSourceTransactor> t(ds->getTransactor());
  assert(t.get());

  std::auto_ptr<te::da::DataSet> dataset(t->getDataSet(m_datasetName, &e, r, travType, rwRole));

  // TODO: Need review: behaviour of te::mem::DataSet + te::rst::Raster.
  std::size_t rpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::RASTER_TYPE);
  if(rpos != std::string::npos)
    return dataset.release();

  return DataSet2Memory(dataset.get());
}

te::da::DataSet* te::map::DataSetLayer::getData(const te::dt::Property& p,
                                                const te::gm::Envelope& e,
                                                te::gm::SpatialRelation r,
                                                te::common::TraverseType travType,
                                                te::common::AccessPolicy rwRole) const
{
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  // Get a transactor
  std::auto_ptr<te::da::DataSourceTransactor> t(ds->getTransactor());
  assert(t.get());

  std::auto_ptr<te::da::DataSet> dataset(t->getDataSet(m_datasetName, &p, &e, r, travType, rwRole));

  // TODO: Need review: behaviour of te::mem::DataSet + te::rst::Raster.
  std::size_t rpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::RASTER_TYPE);
  if(rpos != std::string::npos)
    return dataset.release();

  return DataSet2Memory(dataset.get());
}

te::da::DataSet* te::map::DataSetLayer::getData(const te::gm::Geometry& g,
                                                te::gm::SpatialRelation r,
                                                te::common::TraverseType travType, 
                                                te::common::AccessPolicy rwRole) const
{
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  // Get a transactor
  std::auto_ptr<te::da::DataSourceTransactor> t(ds->getTransactor());
  assert(t.get());

  std::auto_ptr<te::da::DataSet> dataset(t->getDataSet(m_datasetName, &g, r, travType, rwRole));

  // TODO: Need review: behaviour of te::mem::DataSet + te::rst::Raster.
  std::size_t rpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::RASTER_TYPE);
  if(rpos != std::string::npos)
    return dataset.release();

  return DataSet2Memory(dataset.get());
}

te::da::DataSet* te::map::DataSetLayer::getData(const te::dt::Property& p,
                                                const te::gm::Geometry& g,
                                                te::gm::SpatialRelation r,
                                                te::common::TraverseType travType,
                                                te::common::AccessPolicy rwRole) const
{
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  // Get a transactor
  std::auto_ptr<te::da::DataSourceTransactor> t(ds->getTransactor());
  assert(t.get());

  std::auto_ptr<te::da::DataSet> dataset(t->getDataSet(m_datasetName, &p, &g, r, travType, rwRole));

  // TODO: Need review: behaviour of te::mem::DataSet + te::rst::Raster.
  std::size_t rpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::RASTER_TYPE);
  if(rpos != std::string::npos)
    return dataset.release();

  return DataSet2Memory(dataset.get());
}

te::da::DataSet* te::map::DataSetLayer::getData(te::da::Expression* restriction,
                                                te::common::TraverseType travType,
                                                te::common::AccessPolicy rwRole) const
{
  assert(restriction);
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  // Get a transactor
  std::auto_ptr<te::da::DataSourceTransactor> t(ds->getTransactor());
  assert(t.get());

  // Where clause
  te::da::Where* filter = new te::da::Where(restriction);
  
  // All fields (?)
  te::da::Fields* all = new te::da::Fields;
  all->push_back(new te::da::Field("*"));
  
  // From the data set
  te::da::FromItem* fromItem = new te::da::DataSetName(m_datasetName);
  te::da::From* from = new te::da::From;
  from->push_back(fromItem);

  // The final Select
  std::auto_ptr<te::da::Select> select(new te::da::Select(all, from, filter));

  std::auto_ptr<te::da::DataSet> dataset(t->query(select.get(), travType, rwRole));

  return DataSet2Memory(dataset.get());
}

te::da::DataSet* te::map::DataSetLayer::getData(const te::da::ObjectIdSet* oids,
                                                te::common::TraverseType travType,
                                                te::common::AccessPolicy rwRole) const
{
  assert(oids);

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  // Get a transactor
  std::auto_ptr<te::da::DataSourceTransactor> t(ds->getTransactor());
  assert(t.get());

  std::auto_ptr<te::da::DataSet> dataset(t->getDataSet(m_datasetName, oids, travType, rwRole));

  return DataSet2Memory(dataset.get());
}

bool te::map::DataSetLayer::isValid() const
{
  return true;
}

void te::map::DataSetLayer::draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  if(m_rendererType.empty())
    throw Exception((boost::format(TR_MAP("Could not draw the data set layer %1%. The renderer type is empty!")) % getTitle()).str());

  // Try get the defined renderer
  std::auto_ptr<AbstractRenderer> renderer(RendererFactory::make(m_rendererType));
  if(renderer.get() == 0)
    throw Exception((boost::format(TR_MAP("Could not draw the data set layer %1%. The renderer %2% could not be created!")) % getTitle() % m_rendererType).str());

  renderer->draw(this, canvas, bbox, srid);
}

const std::string& te::map::DataSetLayer::getType() const
{
  return sm_type;
}

const std::string& te::map::DataSetLayer::getDataSetName() const
{
  return m_datasetName;
}

void te::map::DataSetLayer::setDataSetName(const std::string& name)
{
  m_datasetName = name;
}

const std::string& te::map::DataSetLayer::getDataSourceId() const
{
  return m_datasourceId;
}

void te::map::DataSetLayer::setDataSourceId(const std::string& id)
{
  m_datasourceId = id;
}

const std::string& te::map::DataSetLayer::getRendererType() const
{
  return m_rendererType;
}

void te::map::DataSetLayer::setRendererType(const std::string& t)
{
  m_rendererType = t;
}
