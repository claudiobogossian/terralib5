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
  \file terralib/maptools/DataSetAdapterLayer.cpp

  \brief A layer with reference to a dataset.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/dataset/FilteredDataSet.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/Fields.h"
#include "../dataaccess/query/From.h"
#include "../dataaccess/query/FromItem.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/Where.h"
#include "../dataaccess/utils/Utils.h"
#include "DataSetAdapterLayer.h"
#include "Exception.h"
#include "RendererFactory.h"

// Boost
#include <boost/format.hpp>

// STL
#include <memory>

const std::string te::map::DataSetAdapterLayer::sm_type("DATASETADAPTERLAYER");

te::map::DataSetAdapterLayer::DataSetAdapterLayer(AbstractLayer* parent)
  : AbstractLayer(parent)
{
}

te::map::DataSetAdapterLayer::DataSetAdapterLayer(const std::string& id, AbstractLayer* parent)
  : AbstractLayer(id, parent)
{
}

te::map::DataSetAdapterLayer::DataSetAdapterLayer(const std::string& id,
                                    const std::string& title,
                                    AbstractLayer* parent)
  : AbstractLayer(id, title, parent)
{
}

te::map::DataSetAdapterLayer::~DataSetAdapterLayer()
{
  m_rtree.clear();
}

std::auto_ptr<te::map::LayerSchema> te::map::DataSetAdapterLayer::getSchema() const
{
  std::auto_ptr<te::da::DataSetType> type;
  type.reset(dynamic_cast<te::da::DataSetType*>(m_converter->getResult()->clone()));
  return type;
}

std::auto_ptr<te::da::DataSet> te::map::DataSetAdapterLayer::getData(te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  assert(!m_datasetName.empty());
  std::auto_ptr<te::da::DataSet> inputData, outputDataSet;

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  inputData = ds->getDataSet(m_datasetName, travType, accessPolicy);
  outputDataSet.reset(te::da::CreateAdapter(inputData.release(), m_converter.get(), true));

  return outputDataSet;
}

std::auto_ptr<te::da::DataSet> te::map::DataSetAdapterLayer::getData(const std::string& propertyName,
                                                              const te::gm::Envelope* e,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  assert(!m_datasetName.empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  std::auto_ptr<te::da::DataSetType> dsType = ds->getDataSetType(m_datasetName);

  // Gets all data
  std::auto_ptr<te::da::DataSet> inputData = ds->getDataSet(m_datasetName, travType, accessPolicy);
  
  // Creates the data set adapter
  std::auto_ptr<te::da::DataSet> adaptedDataSet;
  adaptedDataSet.reset(te::da::CreateAdapter(inputData.release(), m_converter.get(), true));

  std::vector<std::size_t> positions;

  m_rtree.search(*(e), positions);

  adaptedDataSet->moveBeforeFirst();

  std::auto_ptr<te::da::DataSet> result(new te::da::FilteredDataSet(adaptedDataSet.release(), positions, true));

  return result;
}

std::auto_ptr<te::da::DataSet> te::map::DataSetAdapterLayer::getData(const std::string& propertyName,
                                                              const te::gm::Geometry* g,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  assert(!m_datasetName.empty());
  std::auto_ptr<te::da::DataSet> inputData, outputDataSet;

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  inputData = ds->getDataSet(m_datasetName, propertyName, g, r, travType, accessPolicy);
  outputDataSet.reset(te::da::CreateAdapter(inputData.release(), m_converter.get(), true));

  return outputDataSet;
}

std::auto_ptr<te::da::DataSet> te::map::DataSetAdapterLayer::getData(te::da::Expression* restriction,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  assert(!m_datasetName.empty());
  std::auto_ptr<te::da::DataSet> inputData, outputDataSet;

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

  inputData = ds->query(select.get(), travType, accessPolicy);
  outputDataSet.reset(te::da::CreateAdapter(inputData.release(), m_converter.get(), true));

  return outputDataSet;
}

std::auto_ptr<te::da::DataSet> te::map::DataSetAdapterLayer::getData(const te::da::ObjectIdSet* oids,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  assert(oids);
  std::auto_ptr<te::da::DataSet> inputData, outputDataSet;

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  inputData = ds->getDataSet(m_datasetName, oids, travType, accessPolicy);
  outputDataSet.reset(te::da::CreateAdapter(inputData.release(), m_converter.get(), true));

  return outputDataSet;
}

bool te::map::DataSetAdapterLayer::isValid() const
{
  if(m_datasourceId.empty() || m_datasetName.empty())
    return false;

  if(m_converter.get() == 0)
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

void te::map::DataSetAdapterLayer::draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  if(m_rendererType.empty())
    throw Exception((boost::format(TR_MAP("Could not draw the data set layer %1%. The renderer type is empty!")) % getTitle()).str());

  std::auto_ptr<te::da::DataSetType> dsType = getSchema();

  if(!dsType->hasGeom())
    return;

  // Try get the defined renderer
  std::auto_ptr<AbstractRenderer> renderer(RendererFactory::make(m_rendererType));
  if(renderer.get() == 0)
    throw Exception((boost::format(TR_MAP("Could not draw the data set layer %1%. The renderer %2% could not be created!")) % getTitle() % m_rendererType).str());

  renderer->draw(this, canvas, bbox, srid);
}

te::da::DataSetTypeConverter* te::map::DataSetAdapterLayer::getConverter() const
{
  return m_converter.get();
}

void te::map::DataSetAdapterLayer::setConverter(std::auto_ptr<te::da::DataSetTypeConverter> converter)
{
  m_converter = converter;

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_datasourceId, true);

  std::auto_ptr<te::da::DataSetType> dsType = ds->getDataSetType(m_datasetName);

  if(m_converter->getResult()->hasGeom())
  {
     m_rtree.clear();

    // Gets all data
    std::auto_ptr<te::da::DataSet> inputData = ds->getDataSet(m_datasetName);

    // Creates the data set adapter
    std::auto_ptr<te::da::DataSet> adaptedDataSet;
    adaptedDataSet.reset(te::da::CreateAdapter(inputData.release(), m_converter.get(), true));

    std::size_t geomPropPos = te::da::GetFirstSpatialPropertyPos(adaptedDataSet.get());

    std::size_t pos = 0;

    while(adaptedDataSet->moveNext())
    {
      std::auto_ptr<te::gm::Geometry> geom(adaptedDataSet->getGeometry(geomPropPos));
      assert(geom.get());

      te::gm::Envelope env(*geom->getMBR());

      m_rtree.insert(env, pos);

      ++pos;
    }
  }
}

const std::string& te::map::DataSetAdapterLayer::getType() const
{
  return sm_type;
}

const std::string& te::map::DataSetAdapterLayer::getDataSetName() const
{
  return m_datasetName;
}

void te::map::DataSetAdapterLayer::setDataSetName(const std::string& name)
{
  m_datasetName = name;
}

const std::string& te::map::DataSetAdapterLayer::getDataSourceId() const
{
  return m_datasourceId;
}

void te::map::DataSetAdapterLayer::setDataSourceId(const std::string& id)
{
  m_datasourceId = id;
}

const std::string& te::map::DataSetAdapterLayer::getRendererType() const
{
  return m_rendererType;
}

void te::map::DataSetAdapterLayer::setRendererType(const std::string& t)
{
  m_rendererType = t;
}
