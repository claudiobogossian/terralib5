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
  \file terralib/sqlite/DataSourceCatalogLoader.cpp

  \brief DataSourceCatalogLoader manages metadata information for the TerraLib SQLite Data Access Driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../datatype/Property.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "FwDataSet.h"
#include "Utils.h"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>

class te::sqlite::DataSourceCatalogLoader::Impl
{
  public:

    Impl(DataSourceTransactor* parent);

    DataSourceTransactor* m_parent;
};

te::sqlite::DataSourceCatalogLoader::Impl::Impl(DataSourceTransactor* parent)
  : m_parent(parent)
{
}

te::sqlite::DataSourceCatalogLoader::DataSourceCatalogLoader(DataSourceTransactor* parent)
  : m_pImpl(0)
{
  m_pImpl = new Impl(parent);
}

te::sqlite::DataSourceCatalogLoader::~DataSourceCatalogLoader()
{
  delete m_pImpl;
}

std::vector<std::string> te::sqlite::DataSourceCatalogLoader::getDataSetNames()
{
  std::vector<std::string> datasets;

  std::auto_ptr<te::da::DataSet> tables(getDataSets());

  while(tables->moveNext())
  {
    std::string name = tables->getString(1);
    datasets.push_back(name);
  }

  return datasets;
}

std::auto_ptr<te::da::DataSet> te::sqlite::DataSourceCatalogLoader::getDataSets()
{
  std::string sql("SELECT rowid, name, tbl_name, type FROM sqlite_master "
                  "WHERE type IN ('table', 'view') "
                  "AND name NOT LIKE 'sqlite_%'"
                  "AND name NOT LIKE 'idx_%'");

  std::vector<std::string> ommitTables;

  GetHiddenTables(m_pImpl->m_parent->getDataSource(), ommitTables);

  sql += " AND name NOT IN ('SpatialIndex'";

  for(std::size_t i = 0; i < ommitTables.size(); ++i)
  {
    sql += ", '";
    sql += ommitTables[i];
    sql += "'";
  }

  sql += ")";

  return std::auto_ptr<te::da::DataSet>(m_pImpl->m_parent->query(sql));
}

bool te::sqlite::DataSourceCatalogLoader::hasDataSets()
{
  std::auto_ptr<te::da::DataSet> datasets(getDataSets());

  return datasets->moveNext();
}

bool te::sqlite::DataSourceCatalogLoader::datasetExists(const std::string& name)
{
  std::string sql("SELECT rowid FROM sqlite_master WHERE name = '");
              sql += name;
              sql += "'";

  std::auto_ptr<te::da::DataSet> table(m_pImpl->m_parent->query(sql));

  return table->moveNext();
}

te::da::DataSetType* te::sqlite::DataSourceCatalogLoader::getDataSetType(const std::string& datasetName)
{

  std::string sql("SELECT rowid, type FROM sqlite_master WHERE name = '");
              sql += datasetName;
              sql += "'";

  std::auto_ptr<te::da::DataSet> tinfo(m_pImpl->m_parent->query(sql));

  if(!tinfo->moveNext())
    throw te::common::Exception(TR_COMMON("Could not find table/view in sqlite_master table!"));

  unsigned int tid = tinfo->getInt32(0);
  
  int tcategory = Convert2TerraLibCategory(tinfo->getString(1));

// get attribute list
  sql =  "PRAGMA table_info(";
  sql += datasetName;
  sql += ")";

  std::auto_ptr<te::da::DataSet> attributes(m_pImpl->m_parent->query(sql));

  std::auto_ptr<te::da::PrimaryKey> pk(new te::da::PrimaryKey(datasetName, 0, 0));

  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(datasetName, tid));

  dt->setTitle(datasetName);

  dt->setCategory(tcategory);

  while(attributes->moveNext())
  {
    int col = attributes->getInt32(0);

    std::string colName = attributes->getString(1);

    std::string colType = attributes->getString(2);

    bool required =  attributes->getInt32(3) != 0;

    std::string* defaultValue =  attributes->isNull(4) ? 0 : new std::string(attributes->getString(4));

    bool isPK = attributes->getInt32(5) != 0;

    te::dt::Property* p = Convert2TerraLib(col, colName, colType, required, defaultValue);

    dt->add(p);

    if(isPK)
      pk->add(p);

    if(p->getType() == te::dt::GEOMETRY_TYPE)
    {
      te::gm::GeometryProperty* gp = static_cast<te::gm::GeometryProperty*>(p);
      getGeometryInfo(datasetName, gp);
    }
  }

// if we have added a property to the primary we must add the primary key to the dataset type otherwise (if it is empty) just release it!
  if(!pk->getProperties().empty())
    dt->setPrimaryKey(pk.release());

  getIndexes(dt.get());
  getUniqueKeys(dt.get());
  //getCheckConstraints(dt.get());

  return dt.release();
}

void te::sqlite::DataSourceCatalogLoader::getGeometryInfo(const std::string& datasetName, te::gm::GeometryProperty* gp)
{
  assert(gp);

  std::auto_ptr<te::da::DataSet> ftable(0);

  ftable = getGeometryInfo(datasetName, gp->getName());

  if(!ftable->moveNext())
    return;

  int gtype = ftable->getInt32(0);

  //int coorddim = ftable->getInt32(1);

  te::gm::GeomType t = static_cast<te::gm::GeomType>(gtype);

  gp->setGeometryType(t);

  int srid = ftable->getInt32(2);

  gp->setSRID(srid);

  //bool hasSPIDX = ftable->getInt32(3) != 0;
}

std::auto_ptr<te::da::DataSet> te::sqlite::DataSourceCatalogLoader::getGeometryInfo(const std::string& tableName, const std::string& geomColName)
{
  std::string sql  = "SELECT geometry_type, coord_dimension, srid, spatial_index_enabled "
                     "FROM geometry_columns "
                     "WHERE f_table_name = '";
              sql += boost::to_lower_copy(tableName);

  if(!geomColName.empty())
  {
    sql += "' AND f_geometry_column = '";
    sql += boost::to_lower_copy(geomColName);
  }

  sql += "'";

  return m_pImpl->m_parent->query(sql);
}

boost::ptr_vector<te::dt::Property> te::sqlite::DataSourceCatalogLoader::getProperties(const std::string& datasetName)
{
  boost::ptr_vector<te::dt::Property> pvec;

  std::string sql =  "PRAGMA table_info(";
              sql += datasetName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> attributes(m_pImpl->m_parent->query(sql));

  while(attributes->moveNext())
  {
    int col = attributes->getInt32(0);

    std::string colName = attributes->getString(1);

    std::string colType = attributes->getString(2);

    bool required =  attributes->getInt32(3) != 0;

    std::string* defaultValue =  attributes->isNull(4) ? 0 : new std::string(attributes->getString(4));

    te::dt::Property* p = Convert2TerraLib(col, colName, colType, required, defaultValue);

    pvec.push_back(p);

    if(p->getType() == te::dt::GEOMETRY_TYPE)
    {
      te::gm::GeometryProperty* gp = static_cast<te::gm::GeometryProperty*>(p);
      getGeometryInfo(datasetName, gp);
    }
  }

  return pvec;
}

void te::sqlite::DataSourceCatalogLoader::getPrimaryKey(te::da::DataSetType* dt)
{
// get attribute list
  std::string sql  =  "PRAGMA table_info(";
              sql += dt->getName();
              sql += ")";

  std::auto_ptr<te::da::DataSet> attributes(m_pImpl->m_parent->query(sql));

  std::auto_ptr<te::da::PrimaryKey> pk(new te::da::PrimaryKey(dt->getName(), 0, 0));

  while(attributes->moveNext())
  {
    bool isPK = attributes->getInt32(5) != 0;

    if(isPK)
    {
      int col = attributes->getInt32(0);
      te::dt::Property* p = dt->getProperty(col);
      pk->add(p);
    }
  }

// if we have added a property to the primary we must add the primary key to the dataset type otherwise (if it is empty) just release it!
  if(pk->getProperties().empty())
  {
    dt->setPrimaryKey(0);
  }
  else
  {
    dt->setPrimaryKey(pk.release());
  }
}

void te::sqlite::DataSourceCatalogLoader::getUniqueKeys(te::da::DataSetType* dt)
{
  dt->clearUniqueKeys();

  std::string sql("PRAGMA index_list(");
              sql += dt->getName();
              sql += ")";

  std::auto_ptr<te::da::DataSet> indexes(m_pImpl->m_parent->query(sql));

  while(indexes->moveNext())
  {
    int idxId = indexes->getInt32(0);

    std::string idxName = indexes->getString(1);

    bool isUnique = indexes->getInt32(2) != 0;

    if(isUnique && (idxName.find("sqlite_autoindex_") != std::string::npos))
      continue;

    if(isUnique)
      getUniqueKey(dt, static_cast<unsigned int>(idxId), idxName);
  }
}

void te::sqlite::DataSourceCatalogLoader::getUniqueKey(te::da::DataSetType* dt,
                                                       int idxId,
                                                       const std::string& idxName)
{
  std::string sql("PRAGMA index_info(");
              sql += idxName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> indexInfo(m_pImpl->m_parent->query(sql));
  
  std::auto_ptr<te::da::UniqueKey> uk(new te::da::UniqueKey(dt->getName() + "." + idxName, 0, idxId));

  while(indexInfo->moveNext())
  {
    int col = indexInfo->getInt32(1);
    te::dt::Property* p = dt->getProperty(col);
    uk->add(p);
  }

// we need to check if uk is instead a primary key
  sql  =  "PRAGMA table_info(";
  sql += dt->getName();
  sql += ")";

  std::auto_ptr<te::da::DataSet> attributes(m_pImpl->m_parent->query(sql));

  std::size_t nMatchAttributes = 0;
  bool mayBePK = true;

  while(mayBePK && attributes->moveNext())
  {
    bool isPK = attributes->getInt32(5) != 0;

    if(!isPK)
      continue;

    int col = attributes->getInt32(0);
    te::dt::Property* p = dt->getProperty(col);

    if(uk->has(p))
      ++nMatchAttributes;
    else
      mayBePK = false; // we have find a key from pk that is not in uk!
  }

  if(mayBePK && (nMatchAttributes == uk->getProperties().size()))
    return; // ok! it is a pk

//// look for an associated index
//  const std::size_t nidxs = dt->getNumberOfIndexes();
//
//  for(std::size_t i = 0; i < nidxs; ++i)
//  {
//    if(dt->getIndex(i)->getName() == uk->getName())
//    {
//      uk->setAssociatedIndex(dt->getIndex(i));
//      break;
//    }
//  }

  dt->add(uk.release());
}

void te::sqlite::DataSourceCatalogLoader::getIndexes(te::da::DataSetType* dt)
{
  dt->clearIndexes();

  std::string sql("PRAGMA index_list(");
              sql += dt->getName();
              sql += ")";

  std::auto_ptr<te::da::DataSet> indexes(m_pImpl->m_parent->query(sql));

  while(indexes->moveNext())
  {
    int idxId = indexes->getInt32(0);

    std::string idxName = indexes->getString(1);

    bool isUnique = indexes->getInt32(2) != 0;

    if(isUnique && (idxName.find("sqlite_autoindex_") != std::string::npos))
      continue;

    getIndex(dt, static_cast<unsigned int>(idxId), idxName, isUnique);
  }

  getSpatialIndexes(dt);
}

void te::sqlite::DataSourceCatalogLoader::getSpatialIndexes(te::da::DataSetType* dt)
{
  const std::size_t nattrs = dt->size();

  for(std::size_t i = 0; i < nattrs; ++i)
  {
    if(dt->getProperty(i)->getType() != te::dt::GEOMETRY_TYPE)
      continue;

    std::string tname = dt->getName();
    std::string gcol = dt->getProperty(i)->getName();

    if(dt->getCategory() == te::da::VIEW_TYPE)
    {
      std::string sql  = "SELECT f_table_name,  f_geometry_column "
                          "FROM views_geometry_columns "
                          "WHERE view_name = '";
                  sql += tname;
                  sql += "' AND view_geometry = '";
                  sql += gcol;
                  sql += "'";

      std::auto_ptr<te::da::DataSet> feature(m_pImpl->m_parent->query(sql));

      if(!feature->moveNext())
        return;

      tname = feature->getString(0);
      gcol = feature->getString(1);
    }

    std::auto_ptr<te::da::DataSet> ftable(getGeometryInfo(tname, gcol));

    if(!ftable->moveNext())
      continue;

    bool hasSPIDX = ftable->getInt32(3) != 0;

    if(hasSPIDX)
    {
      te::da::Index* idx = new te::da::Index(dt->getName() + ".idx_" + tname + "_" + gcol, te::da::R_TREE_TYPE, dt, 0);
      idx->add(dt->getProperty(i));
    }
  }
}

void te::sqlite::DataSourceCatalogLoader::getIndex(te::da::DataSetType* dt,
                                                   int idxId,
                                                   const std::string& idxName,
                                                   bool isUnique)
{
  std::string sql("PRAGMA index_info(");
              sql += idxName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> indexInfo(m_pImpl->m_parent->query(sql));

  std::auto_ptr<te::da::Index> idx(new te::da::Index(dt->getName() + "." + idxName, te::da::B_TREE_TYPE,  0, idxId));

  while(indexInfo->moveNext())
  {
    int col = indexInfo->getInt32(1);
    te::dt::Property* p = dt->getProperty(col);
    idx->add(p);
  }

  if(isUnique)
  {
// look for an associated unique-key
    const std::size_t nuks = dt->getNumberOfUniqueKeys();

    for(std::size_t i = 0; i < nuks; ++i)
    {
      if(dt->getUniqueKey(i)->getName() == idx->getName())
      {
        dt->getUniqueKey(i)->setAssociatedIndex(idx.get());
        break;
      }
    }

// look for an associated primary-key
    if(dt->getPrimaryKey() && (dt->getPrimaryKey()->getName() == idx->getName()))
      dt->getPrimaryKey()->setAssociatedIndex(idx.get());
  }

  dt->add(idx.release());
}

te::gm::Envelope* te::sqlite::DataSourceCatalogLoader::getExtent(const std::string& tableName, const std::string& geomColName)
{
// TODO: veificar se a tabela possui um indice espacial
  std::string sql("SELECT MIN(xmin), MIN(ymin), MAX(xmax), MAX(ymax) ");
              sql += "FROM idx_";
              sql += tableName;
              sql += "_";
              sql += geomColName;

  std::auto_ptr<te::da::DataSet> result(m_pImpl->m_parent->query(sql));

  if(result->moveNext() == false)
    throw te::common::Exception((boost::format(TR_COMMON("Could not get a MBR for table/view: %1%!")) % tableName).str());

  double xmin = result->getDouble(0);
  double ymin = result->getDouble(1);
  double xmax = result->getDouble(2);
  double ymax = result->getDouble(3);

  return new te::gm::Envelope(xmin, ymin, xmax, ymax);
}

