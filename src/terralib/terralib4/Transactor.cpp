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

  \brief ????
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
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "Globals.h"
#include "Transactor.h"
#include "Utils.h"

// Terralib 4.x
#include <terralib/kernel/TeDatabase.h>
#include <terralib/kernel/TeLayer.h>

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

terralib4::Transactor::Transactor(DataSource* ds, TeDatabase* db)
  : m_ds(ds),
    m_db(db),
    m_isInTransaction(false)
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
                                                                te::common::TraverseType travType,
                                                                bool connected)
{
  TeDatabasePortal* portal = m_db->getPortal();

  if(portal->query("SELECT * FROM " + name))
    return std::auto_ptr<te::da::DataSet>(new terralib4::DataSet(portal));

  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> terralib4::Transactor::getDataSet(const std::string& name,
                                                                 const std::string& propertyName,
                                                                 const te::gm::Envelope* e,
                                                                 te::gm::SpatialRelation r,
                                                                 te::common::TraverseType travType,
                                                                 bool connected)
{
  throw;
}

std::auto_ptr<te::da::DataSet> terralib4::Transactor::getDataSet(const std::string& name,
                                                               const std::string& propertyName,
                                                               const te::gm::Geometry* g,
                                                               te::gm::SpatialRelation r,
                                                               te::common::TraverseType travType,
                                                               bool connected)
{
  throw;
}

std::auto_ptr<te::da::DataSet> terralib4::Transactor::getDataSet(const std::string& name,
                                                               const ObjectIdSet* oids, 
                                                               te::common::TraverseType travType, 
                                                               bool connected)
{
  throw;
}



std::auto_ptr<te::da::DataSet> terralib4::Transactor::query(const te::da::Select& q,
                                      te::common::TraverseType travType,
                                      bool connected)
{
  throw;
}

std::auto_ptr<te::da::DataSet> terralib4::Transactor::query(const std::string& query,
                                      te::common::TraverseType travType,
                                      bool connected)
{
  TeDatabasePortal* portal = m_db->getPortal();

  if(portal->query(query))
    return std::auto_ptr<te::da::DataSet>(new terralib4::DataSet(portal));

  std::auto_ptr<te::da::DataSet>(0);
}

void terralib4::Transactor::execute(const te::da::Query& command)
{
  throw;
}

void terralib4::Transactor::execute(const std::string& command)
{
  m_db->execute(command);
}

std::auto_ptr<te::da::PreparedQuery> terralib4::Transactor::getPrepared(const std::string& qName)
{
  throw;
}

std::auto_ptr<te::da::BatchExecutor> terralib4::Transactor::getBatchExecutor()
{
  throw;
}

void terralib4::Transactor::cancel()
{
}

boost::int64_t terralib4::Transactor::getLastGeneratedId()
{
  throw;
}

std::string terralib4::Transactor::escape(const std::string& value)
{
  throw;
}

bool terralib4::Transactor::isDataSetNameValid(const std::string& datasetName)
{
  return true;
}

bool terralib4::Transactor::isPropertyNameValid(const std::string& propertyName)
{
  return true;
}

std::vector<std::string> terralib4::Transactor::getDataSetNames()
{
  TeLayerMap map = m_db->layerMap();

  std::map<int, TeLayer*>::iterator it = map.begin();

  std::vector<std::string> layers;

  TeLayer* layer = 0;

  while(it != map.end())
  {
    layers.push_back(it->second->name());

    ++it;
  }

  return layers;
}

std::size_t terralib4::Transactor::getNumberOfDataSets()
{
  return m_db->layerMap().size();
}

std::auto_ptr<te::da::DataSetType> terralib4::Transactor::getDataSetType(const std::string& name)
{
  if(!m_db->layerExist(name))
    return std::auto_ptr<te::da::DataSetType>(0);

  TeLayerMap map = m_db->layerMap();

  std::map<int, TeLayer*>::iterator it = map.begin();

  TeLayer* layer = 0;

  while(it != map.end())
  {
    if(it->second->name() == name)
    {
      layer = it->second;
      break;
    }
    ++it;
  }

  TeAttrTableVector tables;
  layer->getAttrTables(tables);

  TeTable table = tables[0];

  std::auto_ptr<te::da::DataSetType> dst(terralib4::Convert2T5(table));

  return dst;
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
    throw Exception((boost::format(TR_TERRALIB4("The dataset \"%1%\" has no property with this name \"%2%\"!")) % datasetName % name).str());

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
    throw Exception((boost::format(TR_TERRALIB4("The dataset already \"%1%\" has a property with this name \"%2%\"!")) % datasetName % name).str());

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

  TeLayerMap map = m_db->layerMap();

  std::map<int, TeLayer*>::iterator it = map.begin();

  TeLayer* layer = 0;

  while(it != map.end())
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

  /*switch(pType)
  {
    case te::dt::CHAR_TYPE:
    case te::dt::UCHAR_TYPE:
    case te::dt::INT16_TYPE:
    case te::dt::INT32_TYPE:
    case te::dt::INT64_TYPE:
    case te::dt::FLOAT_TYPE:
    case te::dt::DOUBLE_TYPE:
    case te::dt::BOOLEAN_TYPE:
    case te::dt::BYTE_ARRAY_TYPE:
    case te::dt::GEOMETRY_TYPE:
    case te::dt::ARRAY_TYPE:
    case te::dt::DATETIME_TYPE:
    {
      const te::dt::SimpleProperty* simple = static_cast<const te::dt::SimpleProperty*>(p);

      if(!simple->isRequired())
        newColumn->Attributes = ADOX::adColNullable;

      pTable->Columns->Append(newColumn->Name, newColumn->Type, newColumn->DefinedSize);

      break;
    }

    case te::dt::STRING_TYPE:
    {
      const te::dt::StringProperty* sp = static_cast<const te::dt::StringProperty*>(p);

      if(sp->size() != 0)
        newColumn->DefinedSize = (long)sp->size();

      if(!sp->isRequired())
        newColumn->Attributes = ADOX::adColNullable;

      pTable->Columns->Append(newColumn->Name, newColumn->Type, newColumn->DefinedSize);

      break;
    }

    default:
      throw te::ado::Exception(TR_ADO("The informed type could not be mapped to ADO type system!"));
    break;
  }*/
}

void terralib4::Transactor::dropProperty(const std::string& datasetName, const std::string& name)
{
  throw;
}

void terralib4::Transactor::renameProperty(const std::string& datasetName,
                    const std::string& propertyName,
                    const std::string& newPropertyName)
{
  throw;
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
  throw;
}

void terralib4::Transactor::dropPrimaryKey(const std::string& datasetName)
{
  throw;
}

std::auto_ptr<te::da::ForeignKey> terralib4::Transactor::getForeignKey(const std::string& datasetName, const std::string& name)
{
  throw;
}

std::vector<std::string> terralib4::Transactor::getForeignKeyNames(const std::string& datasetName)
{
  throw;
}

bool terralib4::Transactor::foreignKeyExists(const std::string& datasetName, const std::string& name)
{
  throw;
}

void terralib4::Transactor::addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk)
{
  throw;
}

void terralib4::Transactor::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
  throw;
}

std::auto_ptr<te::da::UniqueKey> terralib4::Transactor::getUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw;
}

std::vector<std::string> terralib4::Transactor::getUniqueKeyNames(const std::string& datasetName)
{
  throw;
}

bool terralib4::Transactor::uniqueKeyExists(const std::string& datasetName, const std::string& name)
{
  throw;
}

void terralib4::Transactor::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
{
  throw;
}

void terralib4::Transactor::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw;
}

std::auto_ptr<te::da::CheckConstraint> terralib4::Transactor::getCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw;
}

std::vector<std::string> terralib4::Transactor::getCheckConstraintNames(const std::string& datasetName)
{
  throw;
}

bool terralib4::Transactor::checkConstraintExists(const std::string& datasetName, const std::string& name)
{
  throw;
}

void terralib4::Transactor::addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc)
{
  throw;
}

void terralib4::Transactor::dropCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw;
}

std::auto_ptr<te::da::Index> terralib4::Transactor::getIndex(const std::string& datasetName, const std::string& name)
{
  throw;
}

std::vector<std::string> terralib4::Transactor::getIndexNames(const std::string& datasetName)
{
  throw;
}

bool terralib4::Transactor::indexExists(const std::string& datasetName, const std::string& name)
{
  throw;
}

void terralib4::Transactor::addIndex(const std::string& datasetName, te::da::Index* idx,
              const std::map<std::string, std::string>& options)
{
  throw;
}

void terralib4::Transactor::dropIndex(const std::string& datasetName, const std::string& idxName)
{
  throw;
}

std::auto_ptr<te::da::Sequence> terralib4::Transactor::getSequence(const std::string& name)
{
  throw;
}

std::vector<std::string> terralib4::Transactor::getSequenceNames()
{
  throw;
}

bool terralib4::Transactor::sequenceExists(const std::string& name)
{
  return false;
}

void terralib4::Transactor::addSequence(te::da::Sequence* sequence)
{

}

void terralib4::Transactor::dropSequence(const std::string& name)
{

}

std::auto_ptr<te::gm::Envelope> terralib4::Transactor::getExtent(const std::string& datasetName,
                                          const std::string& /*propertyName*/)
{
  throw;
}

std::auto_ptr<te::gm::Envelope> terralib4::Transactor::getExtent(const std::string& datasetName,
                                                               std::size_t /*propertyPos*/)
{
  throw;
}

std::size_t terralib4::Transactor::getNumberOfItems(const std::string& datasetName)
{
  throw;
}

bool terralib4::Transactor::hasDataSets()
{
  throw;
}

bool terralib4::Transactor::dataSetExists(const std::string& name)
{
  throw;
}

void terralib4::Transactor::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  throw;
}

void terralib4::Transactor::cloneDataSet(const std::string& name,
                  const std::string& cloneName,
                  const std::map<std::string, std::string>& options)
{
  // TODO
}

void terralib4::Transactor::dropDataSet(const std::string& name)
{
  throw;
}

void terralib4::Transactor::renameDataSet(const std::string& name, const std::string& newName)
{
  throw;
}

void terralib4::Transactor::add(const std::string& datasetName,
                              te::da::DataSet* d,
                              const std::map<std::string, std::string>& options,
                              std::size_t limit)
{
  throw;
}

void terralib4::Transactor::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  throw;
}

void terralib4::Transactor::update(const std::string& datasetName,
                                 te::da::DataSet* dataset,
                                 const std::vector<std::size_t>& properties,
                                 const te::da::ObjectIdSet* oids,
                                 const std::map<std::string, std::string>& options,
                                 std::size_t limit)
{
  //TODO
}

void terralib4::Transactor::optimize(const std::map<std::string, std::string>& opInfo)
{

}