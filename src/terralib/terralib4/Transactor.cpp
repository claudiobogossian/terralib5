/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/terralib4/Transactor.cpp

  \brief DataSourceTransactor implementation for TerraLib 4.x API.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/Sequence.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../dataaccess/datasource/ScopedTransaction.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Array.h"
#include "../datatype/Date.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"
#include "../geometry/Geometry.h"
#include "../memory/DataSet.h"
#include "../raster/RasterProperty.h"
#include "VectorDataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "Globals.h"
#include "RasterDataSet.h"
#include "TableDataSet.h"
#include "ThemeInfo.h"
#include "Transactor.h"
#include "Utils.h"

// Terralib 4.x
#include <terralib4/kernel/TeDatabase.h>
#include <terralib4/kernel/TeLayer.h>
#include <terralib4/kernel/TeTable.h>
#include <terralib4/kernel/TeTheme.h>
#include <terralib4/kernel/TeRasterTransform.h>
#include <terralib4/kernel/TeVisual.h>

// STL
#include <cassert>
//#include <cstring>
#include <memory>
#include <iostream>

// Boost
//#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
//#include <boost/thread.hpp>

int getViewId(const std::string& viewName, TeViewMap& viewMap)
{
  std::map<int, TeView*>::iterator it = viewMap.begin();

  while(it != viewMap.end())
  {
    if(it->second->name() == viewName)
      return it->second->id();

    ++it;
  }

  return -1;
}

terralib4::Transactor::Transactor(DataSource* ds, TeDatabase* db)
  : m_ds(ds),
    m_db(db),
    m_isInTransaction(false),
    m_layerMap(m_db->layerMap()),
    m_viewMap(m_db->viewMap()),
    m_themeMap(m_db->themeMap())
{
}

terralib4::Transactor::~Transactor()
{
}

te::da::DataSource* terralib4::Transactor::getDataSource() const
{
  return m_ds;
}

void terralib4::Transactor::begin()
{
  m_db->beginTransaction();
  m_isInTransaction = true;
}

void terralib4::Transactor::commit()
{
  m_db->commitTransaction();
  m_isInTransaction = false;
}

void terralib4::Transactor::rollBack()
{
  m_db->rollbackTransaction();
  m_isInTransaction = false;
}

bool terralib4::Transactor::isInTransaction() const
{
  return m_isInTransaction;
}

std::auto_ptr<te::da::DataSet> terralib4::Transactor::getDataSet(const std::string& name,
                                                                 te::common::TraverseType /*travType*/,
                                                                 bool /*connected*/,
                                                                 const te::common::AccessPolicy /*accessPolicy*/)
{
  TeLayer* layer = 0;

  if(m_db->layerExist(name))
  {
    std::map<int, TeLayer*>::iterator it = m_layerMap.begin();

    while(it != m_layerMap.end())
    {
      if(it->second->name() == name)
      {
        layer = it->second;
        break;
      }
      ++it;
    }

    if(layer->hasGeometry(TeRASTER))
    {
      return std::auto_ptr<te::da::DataSet>(new RasterDataSet(layer->raster()));
    }
    else
      return std::auto_ptr<te::da::DataSet>(new VectorDataSet(layer));
  }
  else
  {
    TeAttrTableVector tables;
    m_db->getAttrTables(tables);

    TeTable table;

    for(std::size_t i = 0; i < tables.size(); ++i)
    {
      if(tables[i].tableType() == TeAttrExternal && tables[i].name() == name)
      {
        table = tables[i];
        break;
      }
    }


    return std::auto_ptr<te::da::DataSet>(new TableDataSet(m_db, table));
  }
}

std::auto_ptr<te::da::DataSet> terralib4::Transactor::getDataSet(const std::string& name,
                                                                 const std::string& propertyName,
                                                                 const te::gm::Envelope* e,
                                                                 te::gm::SpatialRelation r,
                                                                 te::common::TraverseType travType,
                                                                 bool /*connected*/,
                                                                 const te::common::AccessPolicy /*accessPolicy*/)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::auto_ptr<te::da::DataSet> terralib4::Transactor::getDataSet(const std::string& name,
                                                                 const std::string& propertyName,
                                                                 const te::gm::Geometry* g,
                                                                 te::gm::SpatialRelation r,
                                                                 te::common::TraverseType travType,
                                                                 bool /*connected*/,
                                                                 const te::common::AccessPolicy /*accessPolicy*/)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::auto_ptr<te::da::DataSet> terralib4::Transactor::getDataSet(const std::string& name,
                                                                 const ObjectIdSet* oids, 
                                                                 te::common::TraverseType travType, 
                                                                 bool /*connected*/,
                                                                 const te::common::AccessPolicy /*accessPolicy*/)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}



std::auto_ptr<te::da::DataSet> terralib4::Transactor::query(const te::da::Select& /*q*/,
                                                            te::common::TraverseType /*travType*/,
                                                            bool /*connected*/,
                                                            const te::common::AccessPolicy /*accessPolicy*/)
{
  throw Exception(TE_TR("TerraLib 4.x driver doesn't support queries!"));
}

std::auto_ptr<te::da::DataSet> terralib4::Transactor::query(const std::string& /*query*/,
                                                            te::common::TraverseType /*travType*/,
                                                            bool /*connected*/,
                                                            const te::common::AccessPolicy /*accessPolicy*/)
{
  throw Exception(TE_TR("TerraLib 4.x driver doesn't support queries!"));
}

void terralib4::Transactor::execute(const te::da::Query& /*command*/)
{
  throw Exception(TE_TR("TerraLib 4.x driver doesn't support command execution!"));
}

void terralib4::Transactor::execute(const std::string& /*command*/)
{
  throw Exception(TE_TR("TerraLib 4.x driver doesn't support command execution!"));
}

std::auto_ptr<te::da::PreparedQuery> terralib4::Transactor::getPrepared(const std::string& qName)
{
  throw Exception(TE_TR("TerraLib 4.x driver doesn't support prepared queries!"));
}

std::auto_ptr<te::da::BatchExecutor> terralib4::Transactor::getBatchExecutor()
{
  throw Exception(TE_TR("TerraLib 4.x driver doesn't support prepared batch executors!"));
}

void terralib4::Transactor::cancel()
{
}

boost::int64_t terralib4::Transactor::getLastGeneratedId()
{
  throw Exception(TE_TR("TerraLib 4.x driver is read-only!"));
}

std::string terralib4::Transactor::escape(const std::string& value)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

bool terralib4::Transactor::isDataSetNameValid(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("TerraLib 4.x driver is read-only!"));
}

bool terralib4::Transactor::isPropertyNameValid(const std::string& /*propertyName*/)
{
  throw Exception(TE_TR("TerraLib 4.x driver is read-only!"));
}

std::vector<std::string> terralib4::Transactor::getDataSetNames()
{
  std::map<int, TeLayer*>::iterator it = m_layerMap.begin();

  std::vector<std::string> dataSets;

  while(it != m_layerMap.end())
  {
    dataSets.push_back(it->second->name());

    ++it;
  }

  TeAttrTableVector tableVector;
  m_db->getAttrTables(tableVector);

  for(std::size_t i = 0; i < tableVector.size(); ++i)
  {
    if(tableVector[i].tableType() == TeAttrExternal)
      dataSets.push_back(tableVector[i].name());
  }

  return dataSets;
}

std::size_t terralib4::Transactor::getNumberOfDataSets()
{
  return getDataSetNames().size();
}

std::auto_ptr<te::da::DataSetType> terralib4::Transactor::getDataSetType(const std::string& name)
{
  TeLayer* layer = 0;

  if(m_db->layerExist(name))
  {
    std::map<int, TeLayer*>::iterator it = m_layerMap.begin();

    while(it != m_layerMap.end())
    {
      if(it->second->name() == name)
      {
        layer = it->second;
        break;
      }
      ++it;
    }
  }

  TeTable table;

  if(!layer)
  {
    TeAttrTableVector tables;
    m_db->getAttrTables(tables);

    for(std::size_t i = 0; i < tables.size(); i++)
    {
      if(tables[i].tableType() == TeAttrExternal && name == tables[i].name())
      {
        table = tables[i];
        break;
      }
    }
  }
  else
  {

// Is a layer

    if(layer->hasGeometry(TeRASTER))
    {
      std::auto_ptr<te::da::DataSetType> dst(new te::da::DataSetType(layer->name(), 0));

  // TODO: handle rasters with multiple objectid!
      te::rst::RasterProperty* prop = Convert2T5(layer->raster()->params());
      dst->add(prop);
      return dst;
    }

    TeAttrTableVector tables;
    layer->getAttrTables(tables);

    table = tables[0];
  }

  std::auto_ptr<te::da::DataSetType> mainDst(terralib4::Convert2T5(table));
  mainDst->setTitle(table.name());

  std::vector<std::string> pkey;
  table.primaryKeys(pkey);

  te::da::PrimaryKey* pk = 0;

  if(!pkey.empty())
  {
    pk = new te::da::PrimaryKey(table.name() + "_pk", mainDst.get());

    std::vector<te::dt::Property*> pkProps;
    for(std::size_t i = 0; i < pkey.size(); ++i)
    {
      te::dt::Property* p = mainDst->getProperty(pkey[i]);
      pkProps.push_back(p);
    }

    pk->setProperties(pkProps);
  }

  if(layer)
  {
    TeAttrTableVector tables;
    layer->getAttrTables(tables);

    if(tables.size() > 1)
    {
      for(std::size_t i = 1; i < tables.size(); ++i)
      {
        TeTable table = tables[i];

        std::auto_ptr<te::da::DataSetType> dst(terralib4::Convert2T5(table));

        std::vector<te::dt::Property*> props = dst->getProperties();

        for(std::size_t j = 0; j < props.size(); ++j)
        {
          te::dt::Property* prop = props[j]->clone();
          prop->setName(dst->getName() + "_" + prop->getName());

          mainDst->add(prop);
        }
      }
    }

    TeRepresPointerVector vec = layer->vectRepres();

    if(!vec.empty())
    {
      TeGeomRep geomRep = vec[0]->geomRep_;

      int srid = layer->projection()->epsgCode();

      if(srid == 4979)
        srid = 4326;

      te::gm::GeometryProperty* geomProp = new te::gm::GeometryProperty("spatial_data", 
        srid, terralib4::Convert2T5GeomType(geomRep));
      mainDst->add(geomProp);
    }
  }

  return mainDst;
}

boost::ptr_vector<te::dt::Property> terralib4::Transactor::getProperties(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSetType> dt = getDataSetType(datasetName);

  std::vector<te::dt::Property*> dtProperties = dt->getProperties();

  boost::ptr_vector<te::dt::Property> properties;

  for(std::size_t i = 0; i < dtProperties.size(); ++i)
    properties.push_back(dtProperties[i]->clone());

  return properties;
}

std::auto_ptr<te::dt::Property> terralib4::Transactor::getProperty(const std::string& datasetName, const std::string& name)
{
   if(!propertyExists(datasetName, name))
    throw Exception((boost::format(TE_TR("The dataset \"%1%\" has no property with this name \"%2%\"!")) % datasetName % name).str());

  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  return std::auto_ptr<te::dt::Property>(dt->getProperty(name)->clone());
}

std::auto_ptr<te::dt::Property> terralib4::Transactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  assert(propertyPos < dt->size());

  return std::auto_ptr<te::dt::Property>(dt->getProperty(propertyPos)->clone());
}

std::vector<std::string> terralib4::Transactor::getPropertyNames(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  std::vector<std::string> pNames;

  std::size_t numProperties = dt->size();

  for(std::size_t i = 0; i < numProperties; ++i)
    pNames.push_back(dt->getProperty(i)->getName());

  return pNames;
}

std::size_t terralib4::Transactor::getNumberOfProperties(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  return dt->size();
}

bool terralib4::Transactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  std::vector<std::string> pNames = getPropertyNames(datasetName);

  if(std::find(pNames.begin(), pNames.end(), name) != pNames.end())
    return true;

  return false;
}

void terralib4::Transactor::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  std::string name = p->getName();
  if(propertyExists(datasetName, name))
    throw Exception((boost::format(TE_TR("The dataset already \"%1%\" has a property with this name \"%2%\"!")) % datasetName % name).str());

  int pType = p->getType();

  TeAttributeRep newProperty;

  newProperty.name_ = name;
  
  if(p->getType() != te::dt::GEOMETRY_TYPE)
    newProperty.type_ = terralib4::Convert2T4(p->getType());
  else
  {
    te::gm::Geometry* geom = dynamic_cast<te::gm::Geometry*>(p);
    newProperty.type_ = terralib4::Convert2T4GeomType(geom->getGeomTypeId());
  }

  std::map<int, TeLayer*>::iterator it = m_layerMap.begin();

  TeLayer* layer = 0;

  while(it != m_layerMap.end())
  {
    if(it->second->name() == datasetName)
    {
      layer = it->second;
      break;
    }
    ++it;
  }

  TeAttrTableVector tables;
  layer->getAttrTables(tables);
  std::string tableName = tables[0].name();

  m_db->addColumn(tableName, newProperty);
}

void terralib4::Transactor::dropProperty(const std::string& datasetName, const std::string& name)
{
  throw;
}

void terralib4::Transactor::renameProperty(const std::string& datasetName,
                    const std::string& propertyName,
                    const std::string& newPropertyName)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::auto_ptr<te::da::PrimaryKey> terralib4::Transactor::getPrimaryKey(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  return std::auto_ptr<te::da::PrimaryKey>(static_cast<te::da::PrimaryKey*>(dt->getPrimaryKey()->clone()));
}

bool terralib4::Transactor::primaryKeyExists(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<te::da::DataSetType> dt(getDataSetType(datasetName));

  if(dt->getPrimaryKey()->getName() == name)
    return true;

  return false;
}

void terralib4::Transactor::addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::dropPrimaryKey(const std::string& datasetName)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::auto_ptr<te::da::ForeignKey> terralib4::Transactor::getForeignKey(const std::string& datasetName, const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::vector<std::string> terralib4::Transactor::getForeignKeyNames(const std::string& datasetName)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

bool terralib4::Transactor::foreignKeyExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::auto_ptr<te::da::UniqueKey> terralib4::Transactor::getUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::vector<std::string> terralib4::Transactor::getUniqueKeyNames(const std::string& datasetName)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

bool terralib4::Transactor::uniqueKeyExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::auto_ptr<te::da::CheckConstraint> terralib4::Transactor::getCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::vector<std::string> terralib4::Transactor::getCheckConstraintNames(const std::string& datasetName)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

bool terralib4::Transactor::checkConstraintExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::dropCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::auto_ptr<te::da::Index> terralib4::Transactor::getIndex(const std::string& datasetName, const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::vector<std::string> terralib4::Transactor::getIndexNames(const std::string& datasetName)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

bool terralib4::Transactor::indexExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::addIndex(const std::string& datasetName, te::da::Index* idx,
              const std::map<std::string, std::string>& options)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::dropIndex(const std::string& datasetName, const std::string& idxName)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::auto_ptr<te::da::Sequence> terralib4::Transactor::getSequence(const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::vector<std::string> terralib4::Transactor::getSequenceNames()
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

bool terralib4::Transactor::sequenceExists(const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::addSequence(te::da::Sequence* sequence)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::dropSequence(const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::auto_ptr<te::gm::Envelope> terralib4::Transactor::getExtent(const std::string& datasetName,
                                          const std::string& /*propertyName*/)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::auto_ptr<te::gm::Envelope> terralib4::Transactor::getExtent(const std::string& datasetName,
                                                               std::size_t /*propertyPos*/)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::size_t terralib4::Transactor::getNumberOfItems(const std::string& datasetName)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

bool terralib4::Transactor::hasDataSets()
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

bool terralib4::Transactor::dataSetExists(const std::string& name)
{
  std::vector<std::string> names = getDataSetNames();

  for(std::size_t i = 0; i < names.size(); ++i)
    if(names[i] == name)
      return true;

  return false;
}

void terralib4::Transactor::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::cloneDataSet(const std::string& name,
                  const std::string& cloneName,
                  const std::map<std::string, std::string>& options)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::dropDataSet(const std::string& name)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::renameDataSet(const std::string& name, const std::string& newName)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::add(const std::string& datasetName,
                              te::da::DataSet* d,
                              const std::map<std::string, std::string>& options,
                              std::size_t limit)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::update(const std::string& datasetName,
                                 te::da::DataSet* dataset,
                                 const std::vector<std::size_t>& properties,
                                 const te::da::ObjectIdSet* oids,
                                 const std::map<std::string, std::string>& options,
                                 std::size_t limit)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Transactor::optimize(const std::map<std::string, std::string>& opInfo)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::vector<std::string> terralib4::Transactor::getTL4Layers()
{
  std::vector<std::string> layers;

  std::map<int, TeLayer*>::iterator it = m_layerMap.begin();

  while(it != m_layerMap.end())
  {
    layers.push_back(it->second->name());
    ++it;
  }

  return layers;
}

std::vector<std::string> terralib4::Transactor::getTL4Tables()
{
  std::vector<std::string> tablesVec;

  TeAttrTableVector tables;
  m_db->getAttrTables(tables, TeAttrExternal);

  for(std::size_t i = 0; i < tables.size(); i++)
    tablesVec.push_back(tables[i].name());

  return tablesVec;
}

std::vector<::terralib4::ThemeInfo> terralib4::Transactor::getTL4Themes()
{
  std::vector<::terralib4::ThemeInfo> themes;

  TeViewMap& vMap = m_db->viewMap();
  TeThemeMap& tMap = m_db->themeMap();
  
  std::map<int, TeView*>::iterator it = vMap.begin();

  while(it != vMap.end())
  {
    TeView* view = it->second;

    std::map<int, TeAbstractTheme*>::iterator itT = tMap.begin();

    while(itT != tMap.end())
    {
      TeAbstractTheme* abTheme = itT->second;

      if(abTheme->type() == TeTHEME)
      {
        TeTheme* theme = dynamic_cast<TeTheme*>(abTheme);

        if(theme->view() == view->id())
        {
          ::terralib4::ThemeInfo themeInfo;
          themeInfo.m_name = theme->name();
          themeInfo.m_viewName = view->name();
          themeInfo.m_layerName = theme->layer()->name();

          themes.push_back(themeInfo);
        }
      }
      ++itT;
    }
    ++it;
  }

  return themes;
}

TeTheme* terralib4::Transactor::getTL4Theme(const ::terralib4::ThemeInfo theme)
{
  std::map<int, TeAbstractTheme*>::iterator it = m_themeMap.begin();

  while(it != m_themeMap.end())
  {
    if(it->second->view() == getViewId(theme.m_viewName, m_viewMap))
    {
      TeAbstractTheme* abTheme = it->second;

      if(abTheme->type() == TeTHEME)
      {
        TeTheme* tl4Theme = dynamic_cast<TeTheme*>(abTheme);

        if(tl4Theme->layer()->name() == theme.m_layerName)
        {
          return tl4Theme;
        }
      }
    }

    ++it;
  }

  return 0;
}

int terralib4::Transactor::getLayerSRID(const std::string & layerName)
{
  std::map<int, TeLayer*>::iterator it = m_layerMap.begin();

  while(it != m_layerMap.end())
  {
    if(it->second->name() == layerName)
    {
      int srid = it->second->projection()->epsgCode();
      if(srid == 4979)
        srid = 4326;

      return srid;
    }

    ++it;
  }

  return 0;
}

te::common::CharEncoding terralib4::Transactor::getEncoding()
{
  return te::common::UTF8;
}