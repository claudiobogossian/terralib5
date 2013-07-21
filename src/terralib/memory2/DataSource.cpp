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
  \file terralib/memory/DataSource.cpp

  \brief Implementation of a random-access dataset class for the TerraLib In-Memory Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess2/dataset/DataSetType.h"
#include "../dataaccess2/dataset/DataSet.h"
#include "../dataaccess2/datasource/DataSourceCatalog.h"
#include "../dataaccess2/datasource/DataSourceCapabilities.h"
#include "../dataaccess2/query/SQLDialect.h"
#include "../geometry/Envelope.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

class te::mem::DataSource::Impl
{
  public:

    Impl()
      : m_ndatasets(0),
        m_maxdatasets(TE_MEMORY_MAX_DATASETS),
        m_isOpened(false),
        m_deepCopy(false)
    {
    }

    ~Impl()
    {
    }

    DataSet* getDataSet(const std::string& name) const
    {
      boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

      std::map<std::string, DataSetPtr>::const_iterator it = m_datasets.find(name);

      if(it == m_datasets.end())
        return 0;

      return new DataSet(*(it->second), m_deepCopy);
    }

    std::vector<std::string> getDataSets() const
    {
      std::vector<std::string> datasets;

      boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

      std::map<std::string, DataSetPtr>::const_iterator it = m_datasets.begin();
      std::map<std::string, DataSetPtr>::const_iterator itend = m_datasets.end();

      while(it != itend)
      {
        datasets.push_back(std::string(it->first));
        ++it;
      }

      return datasets;
    }

    const te::da::DataSetTypePtr& getDataSetType(const std::string& name)
    {
      boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

      std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_schemas.find(name);

      if(it == m_schemas.end())
      {
        te::da::DataSetTypePtr dt(new te::da::DataSetType(name));
        m_schemas[name] = dt;
        return m_schemas[name];
      }

      return it->second;
    }

    //te::da::DataSetType* getDataSetType(const std::string& name)
    //{
    //  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

    //  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_schemas.find(name);

    //  if(it == m_schemas.end())
    //    return 0;

    //  return static_cast<te::da::DataSetType*>(it->second->clone());
    //}

    std::size_t getNumberOfProperties(const std::string& datasetName)
    {
      boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

      std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_schemas.find(datasetName);

      return (it == m_schemas.end()) ? 0 : it->second->size();
    }

    boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName)
    {
      boost::ptr_vector<te::dt::Property> properties;

      boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

      std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_schemas.find(datasetName);

      if(it == m_schemas.end())
        return properties;

      const te::da::DataSetTypePtr& dt = it->second;

      const std::size_t np = dt->size();

      for(std::size_t i = 0; i != np; ++i)
      {
        properties.push_back(dt->getProperty(i)->clone());
      }

      return properties;
    }

    te::dt::Property* getProperty(const std::string& datasetName,
                                  const std::string& propertyName)
    {
      boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

      std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_schemas.find(datasetName);

      if(it == m_schemas.end())
        return 0;

      te::dt::Property* p = it->second->getProperty(propertyName);

      return (p == 0) ? 0 : p->clone();
    }

    te::dt::Property* getProperty(const std::string& datasetName,
                                  std::size_t propertyPos)
    {
      boost::ptr_vector<te::dt::Property> properties;

      boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

      std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_schemas.find(datasetName);

      if(it == m_schemas.end())
        return 0;

      te::dt::Property* p = it->second->getProperty(propertyPos);

      return (p == 0) ? 0 : p->clone();
    }

    std::map<std::string, std::string> m_connInfo;                  //!< DataSource information.
    std::map<std::string, DataSetPtr> m_datasets;                   //!< The set of datasets stored in memory.
    std::map<std::string, te::da::DataSetTypePtr> m_schemas;        //!< The set of dataset schemas.
    mutable boost::recursive_mutex m_mtx;                           //!< The internal mutex.
    std::size_t m_ndatasets;                                        //!< The number of datasets kept in the data source.
    std::size_t m_maxdatasets;                                      //!< The maximum number of datasets to be handled by the data source.
    bool m_isOpened;                                                //!< A flag to control the state of the data source.
    bool m_deepCopy;                                                //!< If true each dataset is cloned in the getDataSet method.

    static te::da::DataSourceCapabilities sm_capabilities;    //!< The Memory data source capabilities.
    static const te::da::SQLDialect sm_dialect;               //!< A dummy dialect.
};

te::da::DataSourceCapabilities te::mem::DataSource::Impl::sm_capabilities;

const te::da::SQLDialect te::mem::DataSource::Impl::sm_dialect;

te::mem::DataSource::DataSource()
  : m_pImpl(0)
{
  m_pImpl = new Impl;
}

te::mem::DataSource::~DataSource()
{
  delete m_pImpl;
}

std::string te::mem::DataSource::getType() const
{
  return TE_MEMORY_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::mem::DataSource::getConnectionInfo() const
{
  return m_pImpl->m_connInfo;
}

void te::mem::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_pImpl->m_connInfo = connInfo;
}

void te::mem::DataSource::open()
{
// assure we are in a closed state
  close();

// check if it is required a different dataset limit
  std::map<std::string, std::string>::const_iterator it = m_pImpl->m_connInfo.find("MAX_DATASETS");

  if(it != m_pImpl->m_connInfo.end())
    m_pImpl->m_maxdatasets = boost::lexical_cast<std::size_t>(it->second);

// check operation mode
  it = m_pImpl->m_connInfo.find("OPERATION_MODE");

  if((it != m_pImpl->m_connInfo.end()) && (boost::to_upper_copy(it->second) == "NON-SHARED"))
    m_pImpl->m_deepCopy = true;

  m_pImpl->m_isOpened = true;
}

void te::mem::DataSource::close()
{
  if(!m_pImpl->m_isOpened)
    return;

  m_pImpl->m_datasets.clear();

  m_pImpl->m_schemas.clear();

  m_pImpl->m_ndatasets = 0;

  m_pImpl->m_maxdatasets = TE_MEMORY_MAX_DATASETS;

  m_pImpl->m_isOpened = false;

  m_pImpl->m_deepCopy = false;
}

bool te::mem::DataSource::isOpened() const
{
  return m_pImpl->m_isOpened;
}

bool te::mem::DataSource::isValid() const
{
  return m_pImpl->m_isOpened;
}

const te::da::DataSourceCapabilities& te::mem::DataSource::getCapabilities() const
{
  return Impl::sm_capabilities;
}

const te::da::SQLDialect* te::mem::DataSource::getDialect() const
{
  return &Impl::sm_dialect;
}

void te::mem::DataSource::begin()
{
// Not supported by In-Memory driver!
}

void te::mem::DataSource::commit()
{
// Not supported by In-Memory driver!
}

void te::mem::DataSource::rollBack()
{
// Not supported by In-Memory driver!
}

bool te::mem::DataSource::isInTransaction() const
{
  return false;
}

std::auto_ptr<te::da::DataSet> te::mem::DataSource::getDataSet(const std::string& name,
                                                                         te::common::TraverseType /*travType*/)
{

  DataSet* d = m_pImpl->getDataSet(name);

  return std::auto_ptr<te::da::DataSet>(d);
}

std::auto_ptr<te::da::DataSet> te::mem::DataSource::getDataSet(const std::string& /*name*/,
                                                                         const std::string& /*propertyName*/,
                                                                         const te::gm::Envelope* /*e*/,
                                                                         te::gm::SpatialRelation /*r*/,
                                                                         te::common::TraverseType /*travType*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::auto_ptr<te::da::DataSet> te::mem::DataSource::getDataSet(const std::string& /*name*/,
                                                                         const std::string& /*propertyName*/,
                                                                         const te::gm::Geometry* /*g*/,
                                                                         te::gm::SpatialRelation /*r*/,
                                                                         te::common::TraverseType /*travType*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::auto_ptr<te::da::DataSet> te::mem::DataSource::query(const te::da::Select& /*q*/,
                                                                    te::common::TraverseType /*travType*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::auto_ptr<te::da::DataSet> te::mem::DataSource::query(const std::string& /*query*/, 
                                                                    te::common::TraverseType /*travType*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::execute(const te::da::Query& /*command*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::execute(const std::string& /*command*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::cancel()
{
}

boost::int64_t te::mem::DataSource::getLastGeneratedId()
{
  return 0;
}

std::string te::mem::DataSource::escape(const std::string& value)
{
  return value;
}

bool te::mem::DataSource::isDataSetNameValid(const std::string& /*datasetName*/)
{
  return true;
}

bool te::mem::DataSource::isPropertyNameValid(const std::string& /*propertyName*/)
{
  return true;
}

std::vector<std::string> te::mem::DataSource::getDataSetNames()
{
  return m_pImpl->getDataSets();
}

const te::da::DataSetTypePtr& te::mem::DataSource::getDataSetType(const std::string& name)
{
  //return std::auto_ptr<te::da::DataSetType>(m_pImpl->getDataSetType(name));
  return m_pImpl->getDataSetType(name);
}

std::vector<std::string> te::mem::DataSource::getPropertyNames(const std::string& datasetName)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::size_t te::mem::DataSource::getNumberOfProperties(const std::string& datasetName)
{
  return m_pImpl->getNumberOfProperties(datasetName);
}

bool te::mem::DataSource::propertyExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

boost::ptr_vector<te::dt::Property> te::mem::DataSource::getProperties(const std::string& datasetName)
{
  return m_pImpl->getProperties(datasetName);
}

te::dt::Property* te::mem::DataSource:: getProperty(const std::string& datasetName, const std::string& name)
{
  return m_pImpl->getProperty(datasetName, name);
}

te::dt::Property* te::mem::DataSource::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  return m_pImpl->getProperty(datasetName, propertyPos);
}

void te::mem::DataSource::addProperty(const std::string& /*datasetName*/, te::dt::Property* /*p*/)
{
//  DataSource* ds = m_t->getMemDataSource();
//
//  DataSource::LockWrite l(ds);
//
//  if(dt->getProperty(p->getName()))
//    throw Exception((boost::format(TR_MEMORY("A property with the same name (%1%) already exists!")) % p->getName()).str());
//
//  if(!ds->datasetExists(dt->getName()))
//    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str()); 
//
//  dt->add(p);
//
//// adjust the dataset properties
//  DataSet* dataset = ds->getDataSet(dt->getName());
//
//  dataset->add(p->getName(), p->getType());
//
//// adjust dataset schema
//  te::da::DataSetType* dschema = ds->getDataSetType(dt->getName());
//
//  dschema->add(p->clone());
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::dropProperty(const std::string& /*datasetName*/,
                                           const std::string& /*propertyName*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = dynamic_cast<te::da::DataSetType*>(p->getParent());

  //if(dt == 0)
  //  throw Exception((boost::format(TR_MEMORY("Dataset for property %1% is unknown!")) % p->getName()).str());

  //DataSource::LockWrite l(ds);

  //DataSet* dataset = ds->getDataSet(dt->getName());

  //te::da::DataSetType* dschema = ds->getDataSetType(dt->getName());

  //std::size_t pos = te::da::GetPropertyPos(dataset, p->getName());

  //dataset->drop(pos);

  //te::dt::Property* pp = dschema->getProperty(p->getName());

  //dschema->remove(pp);

  //dt->remove(p);

  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::renameProperty(const std::string& /*datasetName*/,
                                         const std::string& /*name*/,
                                         const std::string& /*newName*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = dynamic_cast<te::da::DataSetType*>(p->getParent());

  //if(dt == 0)
  //  throw Exception((boost::format(TR_MEMORY("Dataset for property %1% is unknown!")) % p->getName()).str());

  //DataSource::LockWrite l(ds);

  //te::da::DataSetType* dataset = ds->getDataSetType(p->getName());

  //dataset->getProperty(p->getName())->setName(newName);

  //p->setName(newName);

  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::PrimaryKey* te::mem::DataSource::getPrimaryKey(const std::string& /*datasetName*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSource::primaryKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::addPrimaryKey(const std::string& /*datasetName*/, te::da::PrimaryKey* /*pk*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));

  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  //if(ds->getDataSet(dt->getName())->getType()->getPrimaryKey())
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% already has a Primary Key!")) % dt->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::PrimaryKey* newPk = new te::da::PrimaryKey(pk->getName(), dsDt);

  //for(std::size_t i = 0; i < pk->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = pk->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newPk->add(dsP);
  //}

  //dt->setPrimaryKey(pk);
}

void te::mem::DataSource::dropPrimaryKey(const std::string& /*datasetName*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = pk->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //if(!dsDt->getPrimaryKey())
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% has not a Primary Key!")) % dsDt->getName()).str());

  //if(dsDt->getPrimaryKey()->getName() != pk->getName())
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% has not a Primary Key named %2%!")) % dsDt->getName() % pk->getName()).str());
  //  
  //if(dsDt != dt)
  //  dsDt->setPrimaryKey(0);

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->setPrimaryKey(0);
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::vector<std::string> te::mem::DataSource::getForeignKeyNames(const std::string& /*datasetName*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSource::foreignKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::ForeignKey* te::mem::DataSource::getForeignKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::addForeignKey(const std::string& /*datasetName*/, te::da::ForeignKey* /*fk*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::ForeignKey* newFk = new te::da::ForeignKey(fk->getName()); 

  //for(std::size_t i = 0; i < fk->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = fk->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newFk->add(dsP);
  //}

  //for(std::size_t i = 0; i < fk->getReferencedProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = fk->getProperties()[i];

  //   te::da::DataSetType* currDt = static_cast<te::da::DataSetType*>(p->getParent());

  //   if(!ds->datasetExists(currDt->getName()))
  //     throw Exception((boost::format(TR_MEMORY("The dataset %1% referenced not exists!")) % currDt->getName()).str());

  //   te::da::DataSetType* currRefDt = ds->getDataSet(currDt->getName())->getType();

  //   te::dt::Property* refP = currRefDt->getProperty(fk->getReferencedProperties()[i]->getName());

  //   newFk->addRefProperty(refP);
  //}

  //dsDt->add(newFk);
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::dropForeignKey(const std::string& /*datasetName*/,
                                             const std::string& /*fkName*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = fk->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());
  //
  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::ForeignKey* dsFk = dsDt->getForeignKey(fk->getName());
  //
  //if(!dsFk)
  //  throw Exception((boost::format(TR_MEMORY("The Foreign Key %1% not exists!")) % fk->getName()).str());
  //  
  //if(dsDt != dt)
  //  dsDt->remove(dsFk);

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->remove(dsFk);
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::vector<std::string> te::mem::DataSource::getUniqueKeyNames(const std::string& /*datasetName*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSource::uniqueKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::UniqueKey* te::mem::DataSource::getUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::addUniqueKey(const std::string& /*datasetName*/, te::da::UniqueKey* /*uk*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  //if(ds->getDataSet(dt->getName())->getType()->getUniqueKey(uk->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% already has a Unique Key with this name (%2%)!")) % dt->getName() % uk->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::UniqueKey* newUk = new te::da::UniqueKey(uk->getName(), dsDt);

  //for(std::size_t i = 0; i < uk->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = uk->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newUk->add(dsP);
  //}

  //dt->add(uk);
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::dropUniqueKey(const std::string& /*datasetName*/,
                                            const std::string& /*uniqueKeyName*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = uk->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());
  //
  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //if(!dsDt->getUniqueKey(uk->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The Unique Key %1% not exists!")) % uk->getName()).str());
  //  
  //if(dsDt != dt)
  //  dsDt->remove(dsDt->getUniqueKey(uk->getName()));

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->remove(dsDt->getUniqueKey(uk->getName()));
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::vector<std::string> te::mem::DataSource::getCheckConstraintNames(const std::string& /*datasetName*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSource::checkConstraintExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::CheckConstraint* te::mem::DataSource::getCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::addCheckConstraint(const std::string& /*datasetName*/, te::da::CheckConstraint* /*cc*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  //if(ds->getDataSet(dt->getName())->getType()->getCheckConstraint(cc->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% already has a Check Constraint with this name (%2%)!")) % dt->getName() % cc->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::CheckConstraint* newCc = new te::da::CheckConstraint(cc->getName(), dsDt);  
 
  //newCc->setExpression(cc->getExpression());
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::dropCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = cc->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());
  //
  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //if(!dsDt->getCheckConstraint(cc->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The Check Constraint %1% not exists!")) % cc->getName()).str());  

  //if(dsDt != dt)
  //  dsDt->remove(dsDt->getCheckConstraint(cc->getName()));

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->remove(dsDt->getCheckConstraint(cc->getName()));
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::vector<std::string> te::mem::DataSource::getIndexNames(const std::string& /*datasetName*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSource::indexExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::Index* te::mem::DataSource::getIndex(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::addIndex(const std::string& /*datasetName*/, te::da::Index* /*idx*/,
                                   const std::map<std::string, std::string>& /*options*/) 
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  //if(ds->getDataSet(dt->getName())->getType()->getIndex(index->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% already has a Index with this name (%2%)!")) % dt->getName() % index->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::Index* newIndex = new te::da::Index(index->getName(), index->getIndexType(), dsDt);

  //for(std::size_t i = 0; i < index->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = index->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newIndex->add(dsP);
  //}

  //dt->add(index);
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::dropIndex(const std::string& /*datasetName*/,
                                        const std::string& /*idxName*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = index->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());
  //
  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //if(!dsDt->getIndex(index->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The Index %1% not exists!")) % index->getName()).str());

  //dsDt->remove(dsDt->getIndex(index->getName()));

  //if(dsDt != dt)
  //  dt->remove(index);

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->remove(index);
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::vector<std::string> te::mem::DataSource::getSequenceNames()
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSource::sequenceExists(const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::Sequence* te::mem::DataSource::getSequence(const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::createSequence(const te::da::Sequence* /*sequence*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(ds->getCatalog()->getSequence(sequence->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The datasource already has a Sequence with this name (%1%)!")) % sequence->getName()).str());
  //   
  //te::da::Sequence* newSeq = new te::da::Sequence(*sequence);
  //newSeq->setCatalog(ds->getCatalog());

  ////getting the datasettype by the name of the sequence datasettype property involved
  //const te::da::DataSetTypePtr& dsDt = ds->getCatalog()->getDataSetType(static_cast<te::da::DataSetType*>(sequence->getOwner()->getParent())->getName());

  //newSeq->setOwner(dsDt->getProperty(sequence->getOwner()->getName()));
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::dropSequence(const std::string& /*name*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(ds->getCatalog()->getSequence(sequence->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The datasource has no Sequence with this name (%1%)!")) % sequence->getName()).str());

  //ds->getCatalog()->remove(ds->getCatalog()->getSequence(sequence->getName()));
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::mem::DataSource::getExtent(const std::string& /*datasetName*/, const std::string& /*propertyName*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::mem::DataSource::getExtent(const std::string& /*datasetName*/, std::size_t /*propertyPos*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

std::size_t te::mem::DataSource::getNumberOfItems(const std::string& /*datasetName*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSource::hasDataSets()
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSource::dataSetExists(const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::createDataSet(te::da::DataSetType* dt,
                                        const std::map<std::string, std::string>& /*options*/)
{
  boost::lock_guard<boost::recursive_mutex> lock(m_pImpl->m_mtx);

  if(m_pImpl->m_schemas.find(dt->getName()) != m_pImpl->m_schemas.end())
    throw Exception((boost::format(TR_MEMORY("A dataset with the same name (%1%) already exists!")) % dt->getName()).str());

  DataSetPtr dataset(new DataSet(dt));

  m_pImpl->m_datasets[dt->getName()] = dataset;
}

void te::mem::DataSource::cloneDataSet(const std::string& /*name*/,
                                       const std::string& /*cloneName*/,
                                       const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::dropDataSet(const std::string& /*name*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(datasetName))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% doesn't exist!")) % datasetName).str());

  //ds->remove(datasetName);

  //if(ds->getCatalog() && ds->getCatalog()->datasetExists(datasetName))
  //{
  //  te::da::DataSetTypePtr dt = ds->getCatalog()->getDataSetType(datasetName);

  //  ds->getCatalog()->remove(dt.get());
  //}
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::renameDataSet(const std::string& /*name*/,
                                        const std::string& /*newName*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  //std::string oldName = dt->getName();

  //if(dt->getCatalog())
  //  ds->getCatalog()->rename(dt, newName);

  //ds->rename(oldName, newName);
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::add(const std::string& /*datasetName*/,
                              te::da::DataSet* /*d*/,
                              const std::map<std::string, std::string>& /*options*/,
                              std::size_t /*limit*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //te::mem::DataSet* idataset = ds->getDataSetRef(datasetName);

  //if(idataset == 0)
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% doesn't exist!")) % datasetName).str());

  //idataset->copy(*d, limit);
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //DataSet* dataset = ds->getDataSetRef(datasetName);

  //if(dataset == 0)
  //  throw Exception((boost::format(TR_MEMORY("The dataset %1% doesn't exist!")) % datasetName).str());

  //dataset->clear();
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::update(const std::string& /*datasetName*/,
                                 te::da::DataSet* /*dataset*/,
                                 const std::vector<std::size_t>& /*properties*/,
                                 const te::da::ObjectIdSet* /*oids*/,
                                 const std::map<std::string, std::string>& /*options*/,
                                 std::size_t /*limit*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  m_pImpl->m_connInfo = dsInfo;

  open();

  close();
}

void te::mem::DataSource::drop(const std::map<std::string, std::string>& /*dsInfo*/)
{
// Not supported by In-Memory driver!
}

bool te::mem::DataSource::exists(const std::map<std::string, std::string>& /*dsInfo*/)
{
// Not supported by In-Memory driver!
  return false;
}

std::vector<std::string> te::mem::DataSource::getDataSourceNames(const std::map<std::string, std::string>& /*info*/)
{
  return std::vector<std::string>();
}

