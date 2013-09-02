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
  \file terralib/stmemory/DataSource.cpp

  \brief Implements a DataSource that contains st memory DataSets indexed by space and time. 
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess2/dataset/DataSet.h"
#include "../dataaccess2/dataset/DataSetType.h"
#include "../dataaccess2/dataset/CheckConstraint.h"
#include "../dataaccess2/dataset/ForeignKey.h"
#include "../dataaccess2/dataset/Index.h"
#include "../dataaccess2/dataset/PrimaryKey.h"
#include "../dataaccess2/dataset/Sequence.h"
#include "../dataaccess2/dataset/UniqueKey.h"

#include "DataSet.h"
#include "DataSource.h"
#include "Transactor.h"
#include "Exception.h"
#include "Globals.h"

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

te::da::DataSourceCapabilities te::stmem::DataSource::sm_capabilities;
const te::da::SQLDialect te::stmem::DataSource::sm_dialect;

te::stmem::DataSource::DataSource()
  : m_connInfo(),
    m_datasets(),
    m_schemas(),
    m_maxdatasets(TE_STMEMORY_DRIVER_MAX_DATASETS),
    m_isOpened(false),
    m_deepCopy(false),
    m_transactor(0)
{
}

te::stmem::DataSource::~DataSource()
{
}

std::string te::stmem::DataSource::getType() const 
{
  return Globals::sm_driverIdentifier;
}

const std::map<std::string, std::string>& te::stmem::DataSource::getConnectionInfo() const 
{
  return m_connInfo;
}

void te::stmem::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connInfo = connInfo;
}

std::auto_ptr<te::da::DataSourceTransactor> te::stmem::DataSource::getTransactor()
{
  return std::auto_ptr<te::da::DataSourceTransactor>(new Transactor(this));
}

void te::stmem::DataSource::open() 
{
// assure we are in a closed state
  close();

  m_transactor.reset(new Transactor(this));

// check if it is required a different dataset limit
  std::map<std::string, std::string>::const_iterator it = m_connInfo.find("MAX_DATASETS");

  if(it != m_connInfo.end())
    m_maxdatasets = boost::lexical_cast<std::size_t>(it->second);

// check operation mode
  it = m_connInfo.find("OPERATION_MODE");

  if((it != m_connInfo.end()) && (te::common::Convert2UCase(it->second) == "NON-SHARED"))
    m_deepCopy = true;
 
  m_isOpened = true;
}

void te::stmem::DataSource::close() 
{
  if(!m_isOpened)
    return;

  m_datasets.clear();
  
  m_schemas.clear();

  m_maxdatasets = TE_STMEMORY_DRIVER_MAX_DATASETS;

  m_isOpened = false;

  m_deepCopy = false;

  m_transactor.reset(0);
}

bool te::stmem::DataSource::isOpened() const 
{
  return m_isOpened;
}

bool te::stmem::DataSource::isValid() const 
{
  return m_isOpened;
}

const te::da::DataSourceCapabilities& te::stmem::DataSource::getCapabilities() const 
{
  return sm_capabilities;
}

const te::da::SQLDialect* te::stmem::DataSource::getDialect() const 
{
  return &sm_dialect;
}

void te::stmem::DataSource::add(const std::string& name, te::da::DataSetType* t, DataSet* d)
{
  m_transactor->add(name, t, d);
}

std::auto_ptr<te::da::DataSet> te::stmem::DataSource::getDataSet(const std::string& name, const te::dt::DateTime* dt, te::dt::TemporalRelation r,
                                          te::common::TraverseType travType, bool connected)
{
  return m_transactor->getDataSet(name, dt, r, travType, connected);
}

std::auto_ptr<te::da::DataSet> te::stmem::DataSource::getDataSet(const std::string& name, 
                                                  const te::gm::Geometry* geom, te::gm::SpatialRelation sr,
                                                  const te::dt::DateTime* dt, te::dt::TemporalRelation tr,
                                                  te::common::TraverseType travType, bool connected)
{
  return m_transactor->getDataSet(name, geom, sr, dt, tr, travType, connected);
}

std::auto_ptr<te::da::DataSet> te::stmem::DataSource::getDataSet(const std::string& name, 
                                                  const te::gm::Envelope* e, te::gm::SpatialRelation sr,
                                                  const te::dt::DateTime* dt, te::dt::TemporalRelation tr,
                                                  te::common::TraverseType travType, bool connected)
{
  return m_transactor->getDataSet(name, e, sr, dt, tr, travType, connected);
}

///protected Methods
void te::stmem::DataSource::create(const std::map<std::string, std::string>& /*dsInfo*/) 
{
  return;
}

void te::stmem::DataSource::drop(const std::map<std::string, std::string>& /*dsInfo*/) 
{
  return;
}

bool te::stmem::DataSource::exists(const std::map<std::string, std::string>& /*dsInfo*/) 
{
  return false;
}

std::vector<std::string> te::stmem::DataSource::getDataSourceNames(const std::map<std::string, std::string>& /*dsInfo*/) throw(te::da::Exception)
{
  return std::vector<std::string>();
}

std::vector<std::string> te::stmem::DataSource::getEncodings(const std::map<std::string, std::string>& /*dsInfo*/)
{
  return std::vector<std::string>();
}








