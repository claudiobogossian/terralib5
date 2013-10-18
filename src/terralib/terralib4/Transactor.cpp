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

terralib4::Transactor::Transactor()
{
}

terralib4::Transactor::~Transactor()
{
}

te::da::DataSource* terralib4::Transactor::getDataSource() const
{
  throw;
}

void terralib4::Transactor::begin()
{
  throw;
}

void terralib4::Transactor::commit()
{
  throw;
}

void terralib4::Transactor::rollBack()
{
  throw;
}

bool terralib4::Transactor::isInTransaction() const
{
  throw;
}

std::auto_ptr<te::da::DataSet> terralib4::Transactor::getDataSet(const std::string& name,
                                                                te::common::TraverseType travType,
                                                                bool connected)
{
  throw;
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
  throw;
}

void terralib4::Transactor::execute(const te::da::Query& command)
{
  throw;
}

void terralib4::Transactor::execute(const std::string& command)
{
  throw;
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
  throw;
}

bool terralib4::Transactor::isPropertyNameValid(const std::string& propertyName)
{
  throw;
}

std::vector<std::string> terralib4::Transactor::getDataSetNames()
{
  throw;
}

std::size_t terralib4::Transactor::getNumberOfDataSets()
{
  throw;
}

std::auto_ptr<te::da::DataSetType> terralib4::Transactor::getDataSetType(const std::string& name)
{
  throw;
}

boost::ptr_vector<te::dt::Property> terralib4::Transactor::getProperties(const std::string& datasetName)
{
  throw;
}

std::auto_ptr<te::dt::Property> terralib4::Transactor::getProperty(const std::string& datasetName, const std::string& name)
{
  throw;
}

std::auto_ptr<te::dt::Property> terralib4::Transactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  throw;
}

std::vector<std::string> terralib4::Transactor::getPropertyNames(const std::string& datasetName)
{
  throw;
}

std::size_t terralib4::Transactor::getNumberOfProperties(const std::string& datasetName)
{
  throw;
}

bool terralib4::Transactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  throw;
}

void terralib4::Transactor::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  throw;
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
  throw;
}

bool terralib4::Transactor::primaryKeyExists(const std::string& datasetName, const std::string& name)
{
  throw;
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