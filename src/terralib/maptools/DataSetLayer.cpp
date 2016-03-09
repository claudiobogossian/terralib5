/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/Fields.h"
#include "../dataaccess/query/From.h"
#include "../dataaccess/query/FromItem.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/SpatialQueryProcessor.h"
#include "../dataaccess/query/Where.h"
#include "../dataaccess/utils/Utils.h"
#include "../geometry/GeometryProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterProperty.h"
#include "DataSetLayer.h"
#include "Exception.h"
#include "RendererFactory.h"

// Boost
#include <boost/format.hpp>

// STL
#include <memory>

const std::string te::map::DataSetLayer::sm_type("DATASETLAYER");

te::map::DataSetLayer::DataSetLayer(AbstractLayer* parent)
  : AbstractLayer(parent),
    m_schema(0)
{
}

te::map::DataSetLayer::DataSetLayer(const std::string& id, AbstractLayer* parent)
  : AbstractLayer(id, parent),
    m_schema(0)
{
}

te::map::DataSetLayer::DataSetLayer(const std::string& id,
                                    const std::string& title,
                                    AbstractLayer* parent)
  : AbstractLayer(id, title, parent),
    m_schema(0)
{
}

te::map::DataSetLayer::~DataSetLayer()
{
  delete m_schema;
}

void te::map::DataSetLayer::setSRID(int srid)
{
  // set the srid associated to the parent AbstractLayer
  m_srid=srid;
  
  // if there is no shema cached try to retrieve one
  if (!m_schema)
  {
    // this guard is to deal with empty dataset layers
    if (!m_datasourceId.empty() && !m_datasetName.empty())
      loadSchema();
  }
  
  // propagate it to my cached dataset schema
  if(m_schema)
  {
    if (m_schema->hasGeom())
    {
      gm::GeometryProperty* myGeom = te::da::GetFirstGeomProperty(m_schema);
      myGeom->setSRID(srid);
    }
    else if (m_schema->hasRaster())
    {
      rst::RasterProperty* rstProp = te::da::GetFirstRasterProperty(m_schema);
      rstProp->getGrid()->setSRID(srid);
    }
  }
}

void te::map::DataSetLayer::loadSchema() const
{
  if(m_schema)
  {
    te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);
    
    std::auto_ptr<LayerSchema> type = ds->getDataSetType(m_datasetName);
    
    // Cache the schema from datasource
    if(type.get())
      m_schema = static_cast<LayerSchema*>(type->clone());
  }
}

std::auto_ptr<te::map::LayerSchema> te::map::DataSetLayer::getSchema() const
{
  if(m_schema == 0 && !m_datasourceId.empty() && !m_datasetName.empty())
  {
    te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

    std::auto_ptr<LayerSchema> type = ds->getDataSetType(m_datasetName);

    if(!type.get())
      return std::auto_ptr<te::da::DataSetType>();

    // Cache the schema from datasource
    m_schema = static_cast<LayerSchema*>(type->clone());

    return type;
  }
  return std::auto_ptr<LayerSchema>(static_cast<LayerSchema*>(m_schema->clone()));
}

std::auto_ptr<te::da::DataSet> te::map::DataSetLayer::getData(te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  return ds->getDataSet(m_datasetName, travType, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::map::DataSetLayer::getData(const std::string& propertyName,
                                                              const te::gm::Envelope* e,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  return ds->getDataSet(m_datasetName, propertyName, e, r, travType, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::map::DataSetLayer::getData(const std::string& propertyName,
                                                              const te::gm::Geometry* g,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  return ds->getDataSet(m_datasetName, propertyName, g, r, travType, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::map::DataSetLayer::getData(te::da::Expression* restriction,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  assert(restriction);
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

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

  te::da::SpatialQueryProcessor sqp;
  return sqp.getDataSet(ds, select.get(), travType);
}

std::auto_ptr<te::da::DataSet> te::map::DataSetLayer::getData(const te::da::ObjectIdSet* oids,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  assert(oids);

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  return ds->getDataSet(m_datasetName, oids, travType, accessPolicy);
}

bool te::map::DataSetLayer::isValid() const
{
  if(m_datasourceId.empty() || m_datasetName.empty())
    return false;

  te::da::DataSourcePtr ds;
  try
  {
    ds = te::da::GetDataSource(m_datasourceId, true);
    this->getSchema().get();
  }
  catch(...)
  {
    return false;
  }

  if(ds.get() == 0 || !ds->isValid() || !ds->isOpened())
    return false;

  return true;
}

void te::map::DataSetLayer::draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid, const double& scale, bool* cancel)
{
  if(m_rendererType.empty())
    throw Exception((boost::format(TE_TR("Could not draw the data set layer %1%. The renderer type is empty!")) % getTitle()).str());

  // Try get the defined renderer
  std::auto_ptr<AbstractRenderer> renderer(RendererFactory::make(m_rendererType));
  if(renderer.get() == 0)
    throw Exception((boost::format(TE_TR("Could not draw the data set layer %1%. The renderer %2% could not be created!")) % getTitle() % m_rendererType).str());

  renderer->draw(this, canvas, bbox, srid, scale, cancel);
}

const std::string& te::map::DataSetLayer::getType() const
{
  return sm_type;
}

const std::string& te::map::DataSetLayer::getRendererType() const
{
  return m_rendererType;
}

void te::map::DataSetLayer::setRendererType(const std::string& t)
{
  m_rendererType = t;
}

void te::map::DataSetLayer::setOutOfDate()
{
  delete m_schema;
  m_schema = 0;
}
