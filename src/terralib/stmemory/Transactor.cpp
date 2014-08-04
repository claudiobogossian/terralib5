/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/stmememory/Transactor.cpp

  \brief  An implementation of DataSourceTransactor class for the ST in-memory driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../datatype/DateTimePeriod.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Transactor.h"
#include "Exception.h"


te::stmem::Transactor::Transactor(DataSource* parent) :
  m_ds(parent)
{
}
         
te::stmem::Transactor::~Transactor()
{
}

te::da::DataSource* te::stmem::Transactor::getDataSource() const
{
  return m_ds;
}

void te::stmem::Transactor::begin()
{
  return;
}

void te::stmem::Transactor::commit()
{
  return;
}

void te::stmem::Transactor::rollBack()
{ 
  return;
}

bool te::stmem::Transactor::isInTransaction() const
{
  return false;
}
         
std::auto_ptr<te::da::DataSet> te::stmem::Transactor::getDataSet(const std::string& name, 
                                                        te::common::TraverseType /*travType*/ , 
												                                bool /*connected*/,
                                                        const te::common::AccessPolicy)
{
  //It creates a new DataSet that shares the internal observations. 
  DataSet* ds = new DataSet(*getData(name), false);
  return std::auto_ptr<te::da::DataSet>(ds);      
}

std::auto_ptr<te::da::DataSet> te::stmem::Transactor::getDataSet(const std::string& name,
                                                                 const std::string& propertyName,
                                                                 const te::gm::Envelope* e,
                                                                 te::gm::SpatialRelation r,
                                                                 te::common::TraverseType /*travType*/, 
												                                         bool /*connected*/,
                                                                 const te::common::AccessPolicy)
{
  te::da::DataSetType* type = getType(name);
  DataSet* ds = getData(name);
  if(type->getPropertyPosition(propertyName) != ds->getGeomPropIdx())
    throw Exception("Not supported by ST In-Memory driver!");       
  return std::auto_ptr<te::da::DataSet>(ds->filter(e, r));
}
         
std::auto_ptr<te::da::DataSet> te::stmem::Transactor::getDataSet(const std::string& name,
                                                                 const std::string& propertyName,
                                                                 const te::gm::Geometry* g,
                                                                 te::gm::SpatialRelation r,
                                                                 te::common::TraverseType /*travType*/, 
												                                         bool /*connected*/,
                                                                 const te::common::AccessPolicy)
{
  te::da::DataSetType* type = getType(name);
  DataSet* ds = getData(name);
  if(type->getPropertyPosition(propertyName) != ds->getGeomPropIdx())
     throw Exception("Not supported by ST In-Memory driver!");
  return std::auto_ptr<te::da::DataSet>(ds->filter(g, r));
}

std::auto_ptr<te::da::DataSet> te::stmem::Transactor::query(const te::da::Select& /*q*/,
                                    te::common::TraverseType /*travType*/, bool /*connected*/,
                                    const te::common::AccessPolicy)
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::stmem::Transactor::query(const std::string& /*query*/, 
                                    te::common::TraverseType /*travType*/, 
											              bool /*connected*/,
                                    const te::common::AccessPolicy)
{
  return std::auto_ptr<te::da::DataSet>(0);    
}
         
void te::stmem::Transactor::execute(const te::da::Query& /*command*/)
{
  return;  
}

void te::stmem::Transactor::execute(const std::string& /*command*/)
{
  return;
}
        
std::auto_ptr<te::da::PreparedQuery> 
te::stmem::Transactor::getPrepared(const std::string& /*qName*/)
{
  return std::auto_ptr<te::da::PreparedQuery> (0);
}

std::auto_ptr<te::da::BatchExecutor> 
te::stmem::Transactor::getBatchExecutor()
{
  return std::auto_ptr<te::da::BatchExecutor> (0);
}
       
void te::stmem::Transactor::cancel()
{
  return;
}

boost::int64_t te::stmem::Transactor::getLastGeneratedId()
{
  return 0;
}

std::string te::stmem::Transactor::escape(const std::string& value)
{
  return "";
}

bool te::stmem::Transactor::isDataSetNameValid(const std::string& datasetName)
{
  std::map<std::string, DataSet*>::const_iterator it = m_ds->m_datasets.find(datasetName);
  if(it==m_ds->m_datasets.end())
    return true;
  return false;
}
         
bool te::stmem::Transactor::isPropertyNameValid(const std::string& /*propertyName*/)
{
  return true;
}
       
std::vector<std::string> te::stmem::Transactor::getDataSetNames()
{
  std::vector<std::string> result;
  std::map<std::string, DataSet*>::const_iterator it = m_ds->m_datasets.begin();
  if(it!=m_ds->m_datasets.end())
  {
    result.push_back(it->first);
    ++it;
  }
  return result;  
}

std::size_t te::stmem::Transactor::getNumberOfDataSets()
{
  return m_ds->m_datasets.size();
}
    
std::auto_ptr<te::da::DataSetType> te::stmem::Transactor::getDataSetType(const std::string& name)
{
  te::da::DataSetType* type = getType(name);
  return std::auto_ptr<te::da::DataSetType>(static_cast<te::da::DataSetType*>(type->clone()));
}

boost::ptr_vector<te::dt::Property> te::stmem::Transactor::getProperties(const std::string& datasetName)
{
  boost::ptr_vector<te::dt::Property> properties;

  te::da::DataSetType* type = getType(datasetName);
 
  const std::size_t np = type->size();

  for(std::size_t i = 0; i != np; ++i)
    properties.push_back(type->getProperty(i)->clone());
  
  return properties;
}

std::auto_ptr<te::dt::Property> 
te::stmem::Transactor::getProperty(const std::string& datasetName, const std::string& name)
{
  te::da::DataSetType* type = getType(datasetName);
 
  return std::auto_ptr<te::dt::Property>(type->getProperty(name)->clone());
}

std::auto_ptr<te::dt::Property> 
te::stmem::Transactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  te::da::DataSetType* type = getType(datasetName);
 
  return std::auto_ptr<te::dt::Property>(type->getProperty(propertyPos)->clone());
}

std::vector<std::string> te::stmem::Transactor::getPropertyNames(const std::string& datasetName)
{
  std::vector<std::string> propertyNames;

  te::da::DataSetType* type = getType(datasetName);
 
  const std::size_t np = type->size();

  for(std::size_t i = 0; i != np; ++i)
    propertyNames.push_back(type->getProperty(i)->getName());
  
  return propertyNames;
}

std::size_t te::stmem::Transactor::getNumberOfProperties(const std::string& datasetName)
{
  te::da::DataSetType* type = getType(datasetName);

  return type->size();
}

bool te::stmem::Transactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  te::da::DataSetType* type = getType(datasetName);

  if(type->getProperty(name)!=0)
    return true;

  return false;      
}

void te::stmem::Transactor::addProperty(const std::string& /*datasetName*/, te::dt::Property* /*p*/)
{
  return;
}

void te::stmem::Transactor::dropProperty(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return;
}

void te::stmem::Transactor::renameProperty(const std::string& /*datasetName*/,
                          const std::string& /*propertyName*/,
                          const std::string& /*newPropertyName*/)
{
  return;
}

std::auto_ptr<te::da::PrimaryKey> te::stmem::Transactor::getPrimaryKey(const std::string& datasetName)
{
  te::da::DataSetType* type = getType(datasetName);
 
  return std::auto_ptr<te::da::PrimaryKey>(static_cast<te::da::PrimaryKey*>(type->getPrimaryKey()->clone()));
}
         
bool te::stmem::Transactor::primaryKeyExists(const std::string& datasetName, const std::string& name)
{
  te::da::DataSetType* type = getType(datasetName);
  te::da::PrimaryKey* pk = type->getPrimaryKey();

  if(pk==0)
    return false;

  if(pk->getName()==name)
    return true;

  return false;
}
         
void te::stmem::Transactor::addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk)
{
  te::da::DataSetType* type = getType(datasetName);
  type->add(pk);
}
         
void te::stmem::Transactor::dropPrimaryKey(const std::string& /*datasetName*/)
{
  return;
}
         
std::auto_ptr<te::da::ForeignKey> 
te::stmem::Transactor::getForeignKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::ForeignKey>(0);
}
         
std::vector<std::string> te::stmem::Transactor::getForeignKeyNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}  

bool te::stmem::Transactor::foreignKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}
         
void te::stmem::Transactor::addForeignKey(const std::string& /*datasetName*/, te::da::ForeignKey* /*fk*/)
{
  return;
}
         
void te::stmem::Transactor::dropForeignKey(const std::string& /*datasetName*/, const std::string& /*fkName*/)
{
  return;
}
         
std::auto_ptr<te::da::UniqueKey> te::stmem::Transactor::getUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::UniqueKey>(0);
}
         
std::vector<std::string> te::stmem::Transactor::getUniqueKeyNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>(0);
}
         
bool te::stmem::Transactor::uniqueKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}
         
void te::stmem::Transactor::addUniqueKey(const std::string& /*datasetName*/, te::da::UniqueKey* /*uk*/)
{
  return;
}
         
void te::stmem::Transactor::dropUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return;
}
         
std::auto_ptr<te::da::CheckConstraint> te::stmem::Transactor::getCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::CheckConstraint>(0);
}
         
std::vector<std::string> te::stmem::Transactor::getCheckConstraintNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>(0);
}
         
bool te::stmem::Transactor::checkConstraintExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}
         
void te::stmem::Transactor::addCheckConstraint(const std::string& /*datasetName*/, te::da::CheckConstraint* /*cc*/)
{
  return;
}
         
void te::stmem::Transactor::dropCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return;
}
         
std::auto_ptr<te::da::Index> te::stmem::Transactor::getIndex(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::Index>(0);
}
         
std::vector<std::string> te::stmem::Transactor::getIndexNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>(0);
}
         
bool te::stmem::Transactor::indexExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}
         
void te::stmem::Transactor::addIndex(const std::string& /*datasetName*/, te::da::Index* /*idx*/,
                    const std::map<std::string, std::string>& /*options*/)
{
  return;
}
         
void te::stmem::Transactor::dropIndex(const std::string& /*datasetName*/, const std::string& /*idxName*/)
{
  return;
}
         
std::auto_ptr<te::da::Sequence> te::stmem::Transactor::getSequence(const std::string& /*name*/)
{
  return std::auto_ptr<te::da::Sequence>(0);
}
         
std::vector<std::string> te::stmem::Transactor::getSequenceNames()
{
  return std::vector<std::string>(0);
}
         
bool te::stmem::Transactor::sequenceExists(const std::string& /*name*/)
{
  return false;
}
         
void te::stmem::Transactor::addSequence(te::da::Sequence* /*sequence*/)
{
  return;
}

void te::stmem::Transactor::dropSequence(const std::string& /*name*/)
{
  return;
}
      
std::auto_ptr<te::gm::Envelope> te::stmem::Transactor::getExtent(const std::string& datasetName,
                                                const std::string& propertyName)
{
  te::da::DataSetType* type = getType(datasetName);
  DataSet* ds = getData(datasetName);
  if(type->getPropertyPosition(propertyName) != ds->getGeomPropIdx())
    throw Exception("Not supported by ST In-Memory driver!");
   
  return std::auto_ptr<te::gm::Envelope>(ds->getExtent(ds->getGeomPropIdx()));
}
         
std::auto_ptr<te::gm::Envelope> te::stmem::Transactor::getExtent(const std::string& datasetName,
                                                std::size_t propertyPos)
{
   te::da::DataSetType* type = getType(datasetName);
   DataSet* ds = getData(datasetName);
   if(propertyPos != ds->getGeomPropIdx())
     throw Exception("Not supported by ST In-Memory driver!");
   
   return std::auto_ptr<te::gm::Envelope>(ds->getExtent(ds->getGeomPropIdx()));
}
         
std::size_t te::stmem::Transactor::getNumberOfItems(const std::string& datasetName)
{
  DataSet* ds = getData(datasetName);
  return ds->size();
}
         
bool te::stmem::Transactor::hasDataSets()
{
  return !m_ds->m_datasets.empty();
}
         
bool te::stmem::Transactor::dataSetExists(const std::string& name)
{
  std::map<std::string, DataSet*>::const_iterator it = m_ds->m_datasets.find(name);
  if(it== m_ds->m_datasets.end())
    return false;
  return true; 
}
         
void te::stmem::Transactor::createDataSet(te::da::DataSetType* /*dt*/, const std::map<std::string, std::string>& /*options*/)
{
  return;
}
         
void te::stmem::Transactor::cloneDataSet(const std::string& /*name*/,
                                          const std::string& /*cloneName*/,
                                          const std::map<std::string, std::string>& /*options*/)
{
  return;
}
 

void te::stmem::Transactor::dropDataSet(const std::string& name)
{
  std::map<std::string, te::da::DataSetType*>::iterator itdt =  m_ds->m_schemas.find(name);  
  if(itdt==m_ds->m_schemas.end())
    throw Exception("There is not a DataSetType with the given name!");

  std::map<std::string, DataSet*>::iterator itds = m_ds->m_datasets.find(name);
  if(itds==m_ds->m_datasets.end())
    throw Exception("There is not a DataSet with the given name!"); 
  
  delete(itdt->second);
  delete(itds->second);
  m_ds->m_schemas.erase(itdt);
  m_ds->m_datasets.erase(itds);
  return; 
}
         
void te::stmem::Transactor::renameDataSet(const std::string& name, const std::string& newName)
{
  if(dataSetExists(newName))
    throw Exception("The is already a DataSet with the new name!");

  std::map<std::string, te::da::DataSetType*>::iterator itdt =  m_ds->m_schemas.find(name);  
  if(itdt==m_ds->m_schemas.end())
    throw Exception("There is not a DataSetType with the given name!");

  std::map<std::string, DataSet*>::iterator itds = m_ds->m_datasets.find(name);
  if(itds==m_ds->m_datasets.end())
    throw Exception("There is not a DataSet with the given name!"); 
  
  te::da::DataSetType* dt = itdt->second;
  dt->setName(newName);
  DataSet* ds = itds->second;
  //ds->setName(newName); => DataSet does not have name!!

  //erase without deleting the pointer.
  m_ds->m_schemas.erase(itdt);
  m_ds->m_datasets.erase(itds);

  m_ds->m_schemas.insert(std::pair<std::string, te::da::DataSetType*>(newName,dt));
  m_ds->m_datasets.insert(std::pair<std::string, DataSet*>(newName,ds));
  
  return; 
}
         
void te::stmem::Transactor::add(const std::string& /*datasetName*/,
                                te::da::DataSet* /*d*/,
                                const std::map<std::string, std::string>& /*options*/,
                                std::size_t /*limit*/)
{
  return;
}
         
void te::stmem::Transactor::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
   return;
}
         
void te::stmem::Transactor::update( const std::string& /*datasetName*/,
                                    te::da::DataSet* /*dataset*/,
                                    const std::vector<std::size_t>& /*properties*/,
                                    const te::da::ObjectIdSet* /*oids*/,
                                    const std::map<std::string, std::string>& /*options*/,
                                    std::size_t /*limit*/)
{
   return;
}
         
void te::stmem::Transactor::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  return;
}


te::stmem::DataSource* te::stmem::Transactor::getSTMemDataSource() const
{
  return m_ds;
}

void te::stmem::Transactor::add(const std::string& name, te::da::DataSetType* t, DataSet* d)
{
  if(dataSetExists(name))
    throw Exception("There is already a DataSet with the new name!");
 
  m_ds->m_datasets.insert(std::pair<std::string, DataSet*>(name,d));
  m_ds->m_schemas.insert(std::pair<std::string, te::da::DataSetType*>(name,t));
  return;
}

 std::auto_ptr<te::da::DataSet> te::stmem::Transactor::getDataSet(const std::string& name,
                                                   const te::dt::DateTime* dt, te::dt::TemporalRelation r,
                                                   te::common::TraverseType /*travType*/, bool /*connected*/,
                                                   const te::common::AccessPolicy)
 {
    DataSet* ds = getData(name);
    return std::auto_ptr<te::da::DataSet>(ds->filter(dt, r));
 }

 std::auto_ptr<te::da::DataSet> te::stmem::Transactor::getDataSet(const std::string& name, 
                                                  const te::gm::Geometry* g, te::gm::SpatialRelation sr,
                                                  const te::dt::DateTime* dt, te::dt::TemporalRelation tr,
                                                  te::common::TraverseType /*travType*/, bool /*connected*/,
                                                  const te::common::AccessPolicy)
 {
   DataSet* ds = getData(name);
   return std::auto_ptr<te::da::DataSet>(ds->filter(g, sr, dt, tr));
 }
 
 std::auto_ptr<te::da::DataSet> te::stmem::Transactor::getDataSet(const std::string& name, 
                                                  const te::gm::Envelope* e, te::gm::SpatialRelation sr,
                                                  const te::dt::DateTime* dt, te::dt::TemporalRelation tr,
                                                  te::common::TraverseType /*travType*/, bool /*connected*/,
                                                  const te::common::AccessPolicy)
 {
   DataSet* ds = getData(name);
   return std::auto_ptr<te::da::DataSet>(ds->filter(e, sr, dt, tr));
 }

 std::auto_ptr<te::dt::DateTimePeriod> 
 te::stmem::Transactor::getTemporalExtent(const std::string& name)
 {
   DataSet* ds = getData(name);
   return std::auto_ptr<te::dt::DateTimePeriod>(ds->getTemporalExtent());
 }

// Protected Methods
te::da::DataSetType* te::stmem::Transactor::getType(const std::string& datasetName) 
{
  std::map<std::string, te::da::DataSetType*>::const_iterator it =  m_ds->m_schemas.find(datasetName);  
  if(it==m_ds->m_schemas.end())
    throw Exception("There is not a DataSetType with the given name!");
  return it->second; 
}

te::stmem::DataSet* te::stmem::Transactor::getData(const std::string& datasetName) 
{
  std::map<std::string, DataSet*>::const_iterator it = m_ds->m_datasets.find(datasetName);
  if(it==m_ds->m_datasets.end())
    throw Exception("There is not a DataSet with the given name!"); 
  return it->second; 
}

te::common::CharEncoding te::stmem::Transactor::getEncoding()
{
  return te::common::UTF8;
}