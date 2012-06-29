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
  \file terralib/sqlite/DataSourceCatalogLoader.cpp

  \brief DataSourceCatalogLoader class implementation for the TerraLib SQLite Data Access Driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../datatype/Property.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "DataSource.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "FwDataSet.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

te::sqlite::DataSourceCatalogLoader::DataSourceCatalogLoader(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::sqlite::DataSourceCatalogLoader::~DataSourceCatalogLoader()
{
}

void te::sqlite::DataSourceCatalogLoader::getDataSets(boost::ptr_vector<std::string>& datasets)
{
  std::auto_ptr<te::da::DataSet> tables(getDataSets());

  while(tables->moveNext())
  {
    std::string* name = new std::string(tables->getString(1));
    datasets.push_back(name);
  }
}

te::da::DataSetType* te::sqlite::DataSourceCatalogLoader::getDataSetType(const std::string& datasetName,
                                                                         const bool full)
{
// get table id and type
  std::string sql("SELECT rowid, type FROM sqlite_master WHERE name = '");
              sql += datasetName;
              sql += "'";

  std::auto_ptr<te::da::DataSet> tinfo(m_t->query(sql));

  if(!tinfo->moveNext())
    throw Exception(TR_SQLITE("Could not find table/view in sqlite_master table!"));

  unsigned int tid = tinfo->getInt32(0);
  
  int tcategory = Convert2TerraLibCategory(tinfo->getString(1));

// get attribute list
  sql =  "PRAGMA table_info(";
  sql += datasetName;
  sql += ")";

  std::auto_ptr<te::da::DataSet> attributes(m_t->query(sql));

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
      dt->setDefaultGeomProperty(static_cast<te::gm::GeometryProperty*>(p));
      getGeometryInfo(dt->getDefaultGeomProperty());
    }
  }

// if we have added a property to the primary we must add the primary key to the dataset type otherwise (if it is empty) just release it!
  if(!pk->getProperties().empty())
    dt->setPrimaryKey(pk.release());

  if(full)
  {
    getIndexes(dt.get());
    getUniqueKeys(dt.get());
    //getCheckConstraints(dt.get());
    dt->setFullLoaded(true);
  }

  return dt.release();
}

void te::sqlite::DataSourceCatalogLoader::getProperties(te::da::DataSetType* dt)
{
  dt->clear();

// get attribute list
  std::string sql =  "PRAGMA table_info(";
              sql += dt->getName();
              sql += ")";

  std::auto_ptr<te::da::DataSet> attributes(m_t->query(sql));

  while(attributes->moveNext())
  {
    int col = attributes->getInt32(0);

    std::string colName = attributes->getString(1);

    std::string colType = attributes->getString(2);

    bool required =  attributes->getInt32(3) != 0;

    std::string* defaultValue =  attributes->isNull(4) ? 0 : new std::string(attributes->getString(4));

    te::dt::Property* p = Convert2TerraLib(col, colName, colType, required, defaultValue);

    dt->add(p);

    if(p->getType() == te::dt::GEOMETRY_TYPE)
    {
      dt->setDefaultGeomProperty(static_cast<te::gm::GeometryProperty*>(p));
      getGeometryInfo(dt->getDefaultGeomProperty());
    }
  }
}

te::dt::Property* te::sqlite::DataSourceCatalogLoader::getProperty(const std::string& /*datasetName*/, const std::string& /*propertyName*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

void te::sqlite::DataSourceCatalogLoader::getPrimaryKey(te::da::DataSetType* dt)
{
// get attribute list
  std::string sql  =  "PRAGMA table_info(";
              sql += dt->getName();
              sql += ")";

  std::auto_ptr<te::da::DataSet> attributes(m_t->query(sql));

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

  std::auto_ptr<te::da::DataSet> indexes(m_t->query(sql));

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

void te::sqlite::DataSourceCatalogLoader::getForeignKeys(te::da::DataSetType* dt, std::vector<std::string>& fkNames, std::vector<std::string>& rdts)
{
// retrieves the list of fks and related tables
  std::string sql("PRAGMA foreign_key_list(");
              sql += dt->getName();
              sql += ")";

// The restuned dataset will have the following columns
// 0 -> id
// 1 -> seq
// 2 -> table (referenced table)
// 3 -> from
// 4 -> to
// 5 -> on_update
// 6 -> on_delete
// 7 -> match
  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  int id = -1;

  while(result->moveNext())
  {
    int fkid = result->getInt32(0);

    if(fkid != id)
    {
      std::string fkname = dt->getName() + "." + boost::lexical_cast<std::string>(result->getInt32(0));
      std::string tname = result->getString(2);

      fkNames.push_back(fkname);
      rdts.push_back(tname);

      id = fkid;
    }
  }
}

te::da::ForeignKey* te::sqlite::DataSourceCatalogLoader::getForeignKey(const std::string& fkName, te::da::DataSetType* dt, te::da::DataSetType* rdt)
{
  std::size_t pos = fkName.find(".");

  if(pos == std::string::npos)
    throw Exception((boost::format(TR_SQLITE("Wrong foreign key name: %1%.")) % fkName).str());

  std::string tableName = fkName.substr(0, pos);
  std::string fkid = fkName.substr(pos + 1);

// retrieves the list of fks and related tables
  std::string sql("PRAGMA foreign_key_list(");
              sql += tableName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  int id = boost::lexical_cast<int>(fkid);

  std::auto_ptr<te::da::ForeignKey> fk(new te::da::ForeignKey(fkName, id));

  fk->setDataSetType(dt);
  fk->setReferencedDataSetType(rdt);

  while(result->moveNext())
  {
    if(id != result->getInt32(0))
      continue;

    std::string onUpdate = result->getString(5);
    std::string onDeletion = result->getString(6);
    
    fk->setOnUpdateAction(GetAction(onUpdate));
    fk->setOnDeleteAction(GetAction(onDeletion));

    std::string from = result->getString(3);
    std::string to = result->getString(4);

    te::dt::Property* fromP = dt->getProperty(from);
    te::dt::Property* toP = dt->getProperty(to);

    fk->add(fromP);
    fk->addRefProperty(toP);
  }

  return fk.release();
}

void te::sqlite::DataSourceCatalogLoader::getIndexes(te::da::DataSetType* dt)
{
  dt->clearIndexes();

  std::string sql("PRAGMA index_list(");
              sql += dt->getName();
              sql += ")";

  std::auto_ptr<te::da::DataSet> indexes(m_t->query(sql));

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

void te::sqlite::DataSourceCatalogLoader::getCheckConstraints(te::da::DataSetType* /*dt*/)
{
  //throw Exception(TR_SQLITE("Not implemented yet!"));
}

void te::sqlite::DataSourceCatalogLoader::getSequences(std::vector<std::string*>& /*sequences*/)
{
  //throw Exception(TR_SQLITE("Not implemented yet!"));
}

te::da::Sequence* te::sqlite::DataSourceCatalogLoader::getSequence(const std::string& /*seqName*/)
{
  //throw Exception(TR_SQLITE("Not implemented yet!"));
  return 0;
}

te::gm::Envelope* te::sqlite::DataSourceCatalogLoader::getExtent(const te::gm::GeometryProperty* gp)
{
  assert(gp->getParent());

  return getExtent(gp->getParent()->getName(), gp->getName());
}

void te::sqlite::DataSourceCatalogLoader::loadCatalog(const bool full)
{
  te::da::DataSourceCatalog* catalog = m_t->getDataSource()->getCatalog();

  catalog->clear();

  boost::ptr_vector<std::string> datasets;

  getDataSets(datasets);

  const std::size_t ndatasets = datasets.size();

  for(std::size_t i = 0; i < ndatasets; ++i)
  {
    te::da::DataSetTypePtr dt(getDataSetType(datasets[i], full));

    catalog->add(dt);
  }
}

bool te::sqlite::DataSourceCatalogLoader::hasDataSets()
{
  std::auto_ptr<te::da::DataSet> datasets(getDataSets());

  return datasets->moveNext();  
}

bool te::sqlite::DataSourceCatalogLoader::datasetExists(const std::string& name)
{
// SQLite tables are numbered from 1, so the comparison below is valid!
  std::string sql("SELECT rowid FROM sqlite_master WHERE name = '");
              sql += name;
              sql += "'";

  std::auto_ptr<te::da::DataSet> table(m_t->query(sql));

  return table->moveNext();
}

bool te::sqlite::DataSourceCatalogLoader::primarykeyExists(const std::string& name)
{
// get attribute list
  std::string sql  =  "PRAGMA table_info(";
              sql += name;
              sql += ")";

  std::auto_ptr<te::da::DataSet> attributes(m_t->query(sql));

  while(attributes->moveNext())
  {
    bool isPK = attributes->getInt32(5) != 0;

    if(isPK)
      return true;
  }

  return false;
}

bool te::sqlite::DataSourceCatalogLoader::uniquekeyExists(const std::string& name)
{
  return indexExists(name);
}

bool te::sqlite::DataSourceCatalogLoader::foreignkeyExists(const std::string& name)
{
  std::size_t pos = name.find(".");

  if(pos == std::string::npos)
    return false;

  std::string tableName = name.substr(0, pos);
  std::string fkid = name.substr(pos + 1);

// retrieves the list of fks and related tables
  std::string sql("PRAGMA foreign_key_list(");
              sql += tableName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  while(result->moveNext())
  {
    if(fkid == boost::lexical_cast<std::string>(result->getInt32(0)))
      return true;
  }

  return false;
}

bool te::sqlite::DataSourceCatalogLoader::checkConstraintExists(const std::string& /*name*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

bool te::sqlite::DataSourceCatalogLoader::indexExists(const std::string& name)
{
  std::size_t pos = name.find(".");

  if(pos == std::string::npos)
    return false;

  std::string tableName = name.substr(0, pos);
  std::string idxName = name.substr(pos + 1);

// retrieves the list of fks and related tables
  std::string sql("PRAGMA index_list(");
              sql += tableName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  while(result->moveNext())
  {
    if(idxName == result->getString(1))
      return true;
  }

  return false;
}

bool te::sqlite::DataSourceCatalogLoader::sequenceExists(const std::string& /*name*/)
{
  throw Exception(TR_SQLITE("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::sqlite::DataSourceCatalogLoader::getTransactor() const
{
  return m_t;
}

te::da::DataSet* te::sqlite::DataSourceCatalogLoader::getDataSets()
{
// search for all tables except indexes and system-tables
  std::string sql("SELECT rowid, name, tbl_name, type FROM sqlite_master "
                  "WHERE type IN ('table', 'view') "
                  "AND name NOT LIKE 'sqlite_%'");

// check if we have to ommit more tables
  std::vector<std::string> ommitTables;

  GetHiddenTables(m_t->getDataSource(), ommitTables);

  sql += " AND name NOT IN ('SpatialIndex'";

  for(std::size_t i = 0; i < ommitTables.size(); ++i)
  {
    sql += ", '";
    sql += ommitTables[i];
    sql += "'";
  }

  sql += ")";

  
// query and mount the table list
  return m_t->query(sql);
}

void te::sqlite::DataSourceCatalogLoader::getGeometryInfo(te::gm::GeometryProperty* gp)
{
  assert(gp && gp->getParent() && (gp->getParent()->getType() == te::dt::DATASET_TYPE));

  te::da::DataSetType* tinfo = static_cast<te::da::DataSetType*>(gp->getParent());

  std::auto_ptr<te::da::DataSet> ftable(0);

  if(tinfo->getCategory() == te::da::TABLE_TYPE)
  {
    ftable.reset(getGeometryInfo(tinfo->getName(), gp->getName()));
  }
  else  // te::da::DataSetType::VIEW or QUERY
  {
    std::string sql  = "SELECT f_table_name,  f_geometry_column "
                       "FROM views_geometry_columns "
                       "WHERE view_name = '";
                sql += tinfo->getName();
                sql += "' AND view_geometry = '";
                sql += gp->getName();
                sql += "'";

    std::auto_ptr<te::da::DataSet> feature(m_t->query(sql));

    if(!feature->moveNext())
      return;

    std::string tname = feature->getString(0);
    std::string gcol = feature->getString(1);

    ftable.reset(getGeometryInfo(tname, gcol));
  }

  if(!ftable->moveNext())
    return;

  std::string gtype = ftable->getString(0);

  std::string coorddim = ftable->getString(1);

  te::gm::GeomType t = Convert2TerraLibGeomType(gtype, coorddim);

  gp->setGeometryType(t);

  int srid = ftable->getInt32(2);

  gp->setSRID(srid);

  //bool hasSPIDX = ftable->getInt32(3) != 0;
}

te::da::DataSet* te::sqlite::DataSourceCatalogLoader::getGeometryInfo(const std::string& tableName, const std::string& geomColName)
{
  std::string sql  = "SELECT type, coord_dimension, srid, spatial_index_enabled "
                     "FROM geometry_columns "
                     "WHERE f_table_name = '";
              sql += tableName;

  if(!geomColName.empty())
  {
    sql += "' AND f_geometry_column = '";
    sql += geomColName;
  }

  sql += "'";

  return m_t->query(sql);
}

te::gm::Envelope* te::sqlite::DataSourceCatalogLoader::getExtent(const std::string& tableName, const std::string& geomColName)
{
  std::string sql("SELECT MIN(MBRMinX(");
              sql += geomColName;
              sql += ")), MIN(MBRMinY(";
              sql += geomColName;
              sql += ")), MAX(MBRMaxX(";
              sql += geomColName;
              sql += ")), MAX(MBRMaxY(";
              sql += geomColName;
              sql += ")) FROM ";
              sql += tableName;

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext() == false)
    throw Exception((boost::format(TR_SQLITE("Could not get a MBR for table/view: %1%!")) % tableName).str());

  double xmin = result->getDouble(0);
  double ymin = result->getDouble(1);
  double xmax = result->getDouble(2);
  double ymax = result->getDouble(3);

  return new te::gm::Envelope(xmin, ymin, xmax, ymax);
}

void te::sqlite::DataSourceCatalogLoader::getIndex(te::da::DataSetType* dt,
                                                   int idxId,
                                                   const std::string& idxName,
                                                   bool isUnique)
{
  std::string sql("PRAGMA index_info(");
              sql += idxName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> indexInfo(m_t->query(sql));
  
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

void te::sqlite::DataSourceCatalogLoader::getUniqueKey(te::da::DataSetType* dt,
                                                       int idxId,
                                                       const std::string& idxName)
{
  std::string sql("PRAGMA index_info(");
              sql += idxName;
              sql += ")";

  std::auto_ptr<te::da::DataSet> indexInfo(m_t->query(sql));
  
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

  std::auto_ptr<te::da::DataSet> attributes(m_t->query(sql));

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

      std::auto_ptr<te::da::DataSet> feature(m_t->query(sql));

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


