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
//#include "../dataaccess2/dataset/ObjectIdSet.h"
#include "../dataaccess2/datasource/DataSourceCatalog.h"
#include "../dataaccess2/datasource/DataSourceCapabilities.h"
#include "../dataaccess2/query/SQLDialect.h"
#include "../dataaccess2/utils/Utils.h"
#include "../geometry/Envelope.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>


te::da::DataSourceCapabilities te::mem::DataSource::sm_capabilities;

const te::da::SQLDialect te::mem::DataSource::sm_dialect;

te::mem::DataSource::DataSource()
  : m_numDatasets(0),
    m_maxNumDatasets(TE_MEMORY_MAX_DATASETS),
    m_isOpened(false),
    m_deepCopy(false)
{
}

te::mem::DataSource::~DataSource()
{
  close();
}

std::string te::mem::DataSource::getType() const
{
  return TE_MEMORY_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::mem::DataSource::getConnectionInfo() const
{
  return m_connInfo;
}

void te::mem::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connInfo = connInfo;
}

void te::mem::DataSource::open()
{
  // Assure we are in a closed state
  close();

  // Check if it is required a different dataset limit
  std::map<std::string, std::string>::const_iterator it = m_connInfo.find("MAX_DATASETS");

  if(it != m_connInfo.end())
    m_maxNumDatasets = boost::lexical_cast<std::size_t>(it->second);

  // Check operation mode
  it = m_connInfo.find("OPERATION_MODE");

  if((it != m_connInfo.end()) && (boost::to_upper_copy(it->second) == "NON-SHARED"))
    m_deepCopy = true;

  m_isOpened = true;
}

void te::mem::DataSource::close()
{
 if(!m_isOpened)
    return;

  m_datasets.clear();
  m_schemas.clear();

  m_numDatasets = 0;
  m_maxNumDatasets = TE_MEMORY_MAX_DATASETS;

  m_isOpened = false;
  m_deepCopy = false;
}

bool te::mem::DataSource::isOpened() const
{
  return m_isOpened;
}

bool te::mem::DataSource::isValid() const
{
  return m_isOpened;
}

const te::da::DataSourceCapabilities& te::mem::DataSource::getCapabilities() const
{
  return sm_capabilities;
}

const te::da::SQLDialect* te::mem::DataSource::getDialect() const
{
  return &sm_dialect;
}

std::auto_ptr<te::da::DataSet> te::mem::DataSource::getDataSet(const std::string& name,
                                                               te::common::TraverseType /*travType*/)
{
  if(!dataSetExists(name))
    throw Exception((boost::format(TR_MEMORY("There is no dataset with this name: \"%1%\"!")) % name).str());

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  std::map<std::string, te::da::DataSetPtr>::const_iterator it = m_datasets.find(name);

  return std::auto_ptr<te::da::DataSet>(new DataSet(*(it->second), m_deepCopy));
}

std::vector<std::string> te::mem::DataSource::getDataSetNames()
{
  std::vector<std::string> datasetNames;

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it;
  for(it = m_schemas.begin(); it != m_schemas.end(); ++it)
    datasetNames.push_back(it->first);

  return datasetNames;
}

const te::da::DataSetTypePtr& te::mem::DataSource::getDataSetType(const std::string& datasetName)
{
  if(!dataSetExists(datasetName))
    throw Exception((boost::format(TR_MEMORY("There is no dataset with this name: \"%1%\"!")) % datasetName).str());

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_schemas.find(datasetName);

  return it->second;
}

std::vector<std::string> te::mem::DataSource::getPropertyNames(const std::string& datasetName)
{
  if(!dataSetExists(datasetName))
    throw Exception((boost::format(TR_MEMORY("There is no dataset with this name: \"%1%\"!")) % datasetName).str());

  std::vector<std::string> pNames;

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_schemas.find(datasetName);

  const te::da::DataSetTypePtr& dt = it->second;

  std::vector<te::dt::Property*>& properties = dt->getProperties();
  for(std::size_t i = 0; i < properties.size(); ++i)
    pNames.push_back(properties[i]->getName());

  return pNames;
}

std::size_t te::mem::DataSource::getNumberOfProperties(const std::string& datasetName)
{
  return getPropertyNames(datasetName).size();
}

bool te::mem::DataSource::propertyExists(const std::string& datasetName, const std::string& name)
{
  std::vector<std::string> pNames = getPropertyNames(datasetName);

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  if(std::find(pNames.begin(), pNames.end(), name) != pNames.end())
    return true;

  return false;
}

boost::ptr_vector<te::dt::Property> te::mem::DataSource::getProperties(const std::string& datasetName)
{
  boost::ptr_vector<te::dt::Property> properties;

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_schemas.find(datasetName);

  if(it == m_schemas.end())
    return properties;

  const te::da::DataSetTypePtr& dt = it->second;

  const std::size_t np = dt->size();

  for(std::size_t i = 0; i < np; ++i)
    properties.push_back(dt->getProperty(i)->clone());

  return properties;
}

te::dt::Property* te::mem::DataSource:: getProperty(const std::string& datasetName, const std::string& name)
{
  if(!propertyExists(datasetName,name))
    throw Exception((boost::format(TR_MEMORY("The dataset \"%1%\" has no property with this name: \"%2%\"!")) % datasetName % name).str());

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  te::dt::Property* p = dt->getProperty(name);

  return p->clone();
}

te::dt::Property* te::mem::DataSource::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  if(!dataSetExists(datasetName))
    throw Exception((boost::format(TR_MEMORY("There is no dataset with this name: \"%1%\"!")) % datasetName).str());

  assert(propertyPos < getNumberOfProperties(datasetName));

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  te::dt::Property* p = dt->getProperty(propertyPos);

  return p->clone();
}

void te::mem::DataSource::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  std::string pName = p->getName();

  if(propertyExists(datasetName, p->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset already \"%1%\" has a property with this name \"%2%\"!")) % datasetName % pName).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  dt->add(p);

  // Adjust the dataset properties
  std::auto_ptr<te::da::DataSet> dataset = getDataSet(datasetName);
  te::mem::DataSet* datasetp = static_cast<te::mem::DataSet*>(dataset.release());

  datasetp->add(pName, p->getType());
}

void te::mem::DataSource::dropProperty(const std::string& datasetName, const std::string& propertyName)
{
  if(!propertyExists(datasetName, propertyName))
    throw Exception((boost::format(TR_MEMORY("The dataset \"%1%\" has no property with this name \"%2%\"!")) % datasetName % propertyName).str());

  std::auto_ptr<te::da::DataSet> dataset = getDataSet(datasetName);

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  te::mem::DataSet* datasetp = static_cast<te::mem::DataSet*>(dataset.release());
  std::size_t pos = te::da::GetPropertyPos(datasetp, propertyName);

  datasetp->drop(pos);

  te::dt::Property* p = dt->getProperty(propertyName);

  dt->remove(p);
}

void te::mem::DataSource::renameProperty(const std::string& datasetName,
                                         const std::string& name,
                                         const std::string& newName)
{
  if(!propertyExists(datasetName, name))
    throw Exception((boost::format(TR_MEMORY("The dataset \"%1%\" has no property with this name \"%2%\"!")) % datasetName % name).str());

  if(!isPropertyNameValid(newName))
    throw Exception((boost::format(TR_MEMORY("The new property name \"%1%\" is not valid!")) % newName).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  // Update the property name in the dataset
  std::auto_ptr<te::da::DataSet> dataset = getDataSet(datasetName);

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  te::mem::DataSet* datasetp = static_cast<te::mem::DataSet*>(dataset.release());

  std::size_t pos = te::da::GetPropertyPos(datasetp, name);
  datasetp->setPropertyName(newName, pos);

  // Update the property name in the schema
  dt->getProperty(pos)->setName(newName);
}

std::size_t te::mem::DataSource::getNumberOfItems(const std::string& datasetName)
{
  if(!dataSetExists(datasetName))
    throw Exception((boost::format(TR_MEMORY("There is no dataset with this name: \"%1%\"!")) % datasetName).str());

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  te::da::DataSetPtr& dataset = m_datasets.find(datasetName)->second;

  return dataset.get()->size();
}

bool te::mem::DataSource::hasDataSets()
{
  return (m_numDatasets == 0) ? false : true;
}

bool te::mem::DataSource::dataSetExists(const std::string& name)
{
  std::vector<std::string> datasetNames = getDataSetNames();

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  if(std::find(datasetNames.begin(), datasetNames.end(), name) != datasetNames.end())
     return true;

  return false;
}

void te::mem::DataSource::createDataSet(te::da::DataSetType* dt,
                                        const std::map<std::string, std::string>& /*options*/)
{
  std::string datasetName = dt->getName();

  if(m_schemas.find(datasetName) != m_schemas.end())
    throw Exception((boost::format(TR_MEMORY("A dataset with the same name (%1%) already exists!")) % datasetName).str());

  if((m_numDatasets + 1) > m_maxNumDatasets)
    throw Exception((boost::format(TR_MEMORY("The maximum number of datasets was exceeded!"))).str());

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  te::da::DataSetTypePtr dtp(dt);
  m_schemas[datasetName] = dtp;

  DataSetPtr dataset(new DataSet(dt));

  m_datasets[datasetName] = dataset;

  ++m_numDatasets;
}

void te::mem::DataSource::cloneDataSet(const std::string& name,
                                       const std::string& cloneName,
                                       const std::map<std::string, std::string>& /*options*/)
{
  if(!dataSetExists(name))
    throw Exception((boost::format(TR_MEMORY("There is no dataset with this name: \"%1%\"!")) % name).str());

  if(!isDataSetNameValid(cloneName))
    throw Exception((boost::format(TR_MEMORY("The dataset clone name \"%1%\" is not valid!")) % cloneName).str());

  if(!dataSetExists(cloneName))
    throw Exception((boost::format(TR_MEMORY("There is already a dataset with this name: \"%1%\"!")) % cloneName).str());

  // Clone the schema
  const te::da::DataSetTypePtr& dtp = m_schemas[name];

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);
  
  te::da::DataSetType* clonedt = static_cast<te::da::DataSetType*>(dtp->clone());
  te::da::DataSetTypePtr clonedtp(clonedt);
  m_schemas[cloneName] = clonedtp;

  // Clone the dataset
  te::da::DataSetPtr& datasetp = m_datasets[name];

  te::da::DataSetPtr datasetClone(getDataSet(name).release());

  m_datasets[cloneName] = datasetClone;

  ++m_numDatasets;
}

void te::mem::DataSource::dropDataSet(const std::string& name)
{
  if(!dataSetExists(name))
    throw Exception((boost::format(TR_MEMORY("There is no dataset with this name: \"%1%\"!")) % name).str());

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  m_schemas.erase(m_schemas.find(name));
  m_datasets.erase(m_datasets.find(name));

  --m_numDatasets;
}

void te::mem::DataSource::renameDataSet(const std::string& name, const std::string& newName)
{
  if(!dataSetExists(name))
    throw Exception((boost::format(TR_MEMORY("There is no dataset with this name: \"%1%\"!")) % name).str());

  if(!isDataSetNameValid(newName))
    throw Exception((boost::format(TR_MEMORY("The new name \"%1%\" is not valid!")) % newName).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(name);

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  dt->setName(newName);
  dt->setCompositeName(newName);

  m_schemas.erase(m_schemas.find(name));
  m_schemas[newName] = dt;

  te::da::DataSetPtr& dataset = m_datasets[name];
  m_datasets.erase(m_datasets.find(name));
  m_datasets[newName] = dataset;
}

void te::mem::DataSource::add(const std::string& datasetName,
                              te::da::DataSet* d,
                              const std::map<std::string, std::string>& /*options*/,
                              std::size_t limit)
{
  if(!dataSetExists(datasetName))
    throw Exception((boost::format(TR_MEMORY("There is no dataset with this name: \"%1%\"!")) % datasetName).str());

  boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  te::da::DataSetPtr& dataset = m_datasets[datasetName];
  te::mem::DataSet* datasetp = static_cast<te::mem::DataSet*>(m_datasets[datasetName].get());
  datasetp->copy(*d, limit);
}

void te::mem::DataSource::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  if(!dataSetExists(datasetName))
    throw Exception((boost::format(TR_MEMORY("There is no dataset with this name: \"%1%\"!")) % datasetName).str());

  //boost::lock_guard<boost::recursive_mutex> lock(m_mtx);

  //std::auto_ptr<te::da::DataSet> datasetp = getDataSet(datasetName, oids);

  //te::mem::DataSet* dataset = static_cast<te::mem::DataSet*>(datasetp.get());

  //while(dataset->moveNext())
  //  dataset->remove();
}

void te::mem::DataSource::update(const std::string& /*datasetName*/,
                                 te::da::DataSet* /*dataset*/,
                                 const std::vector<std::size_t>& /*properties*/,
                                 const te::da::ObjectIdSet* /*oids*/,
                                 const std::map<std::string, std::string>& /*options*/,
                                 std::size_t /*limit*/)
{
}
