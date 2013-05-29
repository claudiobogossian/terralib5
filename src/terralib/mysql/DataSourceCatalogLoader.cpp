/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mysql/DataSourceCatalogLoader.cpp

  \brief DataSourceCatalogLoader class implementation for MySQL data access driver.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../datatype/Property.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterProperty.h"
#include "Connection.h"
#include "DataSource.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <memory>

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// MySQL
#include <driver/mysql_public_iface.h>

te::mysql::DataSourceCatalogLoader::DataSourceCatalogLoader(DataSourceTransactor* parent)
  : m_t(parent)
{
  m_md = m_t->getConnection()->getConn()->getMetaData();

  m_catalogName = m_md->getConnection()->getCatalog();
  m_schemaName = m_md->getConnection()->getSchema();
}

te::mysql::DataSourceCatalogLoader::~DataSourceCatalogLoader()
{
}

void te::mysql::DataSourceCatalogLoader::getDataSets(boost::ptr_vector<std::string>& datasets)
{
  std::string sql;

  getDataSets(sql);

// get table list from database
  {
    std::auto_ptr<te::da::DataSet> dsets(m_t->query(sql));

    while(dsets->moveNext())
    {
      std::string tname = dsets->getString(0);

      datasets.push_back(new std::string(tname));
    }
  }

// add rasters dataset
  if(datasetExists("te_tiled_rasters"))
  {
    std::auto_ptr<te::da::DataSet> rdsets(m_t->query("SELECT r_name FROM te_tiled_rasters"));

    while(rdsets->moveNext())
    {
      std::string tname = rdsets->getString(0);

      datasets.push_back(new std::string(tname));
    }
  }

//// get table list from database
//  std::list<sql::SQLString> table_types;
//
//  table_types.push_back("TABLE");
//  table_types.push_back("VIEW");
//
//  std::auto_ptr<sql::ResultSet> result(m_md->getTables(m_catalogName, m_schemaName, "%", table_types));
//
//  while(result->next())
//  {
//    std::string tname = result->getString("TABLE_NAME");
//
//    datasets.push_back(new std::string(tname));
//  }
}

te::da::DataSetType* te::mysql::DataSourceCatalogLoader::getDataSetType(const std::string& datasetName,
                                                                        const bool full)
{
// get table type
  std::list<sql::SQLString> table_types;

  table_types.push_back("TABLE");
  table_types.push_back("VIEW");

  std::auto_ptr<sql::ResultSet> result(m_md->getTables(m_catalogName, m_schemaName, datasetName, table_types));

  if(!result->next())
  {
    if(m_t->getMyDataSource()->isRaster(datasetName))
      return getRasterDataSet(datasetName);
    else
      throw Exception((boost::format(TR_MYSQL("Could not find information for dataset: %1%!")) % datasetName).str());
  }

  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(datasetName, 0));

  dt->setCategory(GetTableCategory(result->getString("TABLE_TYPE")));

  dt->setTitle(datasetName);

  getProperties(dt.get());

  //if(full)
  //{
    getPrimaryKey(dt.get());
    getUniqueKeys(dt.get());
    getIndexes(dt.get());
    //TODO:
    //1) getCheckConstraints(dt.get());
    //2) retrieve Foreign Keys
  //}

  return dt.release();
}

void te::mysql::DataSourceCatalogLoader::getProperties(te::da::DataSetType* dt)
{
  dt->clear();

// get columns info
  std::auto_ptr<sql::ResultSet> columnsResult(m_md->getColumns(m_catalogName, m_schemaName, dt->getName(), "%"));

  while(columnsResult->next())
  {
    te::dt::Property* p = Convert2TerraLibProperty(columnsResult.get());

    dt->add(p);
  }
}

te::dt::Property* te::mysql::DataSourceCatalogLoader::getProperty(const std::string& datasetName, const std::string& propertyName)
{
// get columns info
  std::auto_ptr<sql::ResultSet> columnsResult(m_md->getColumns(m_catalogName, m_schemaName, datasetName, propertyName));

  if(!columnsResult->next())
    throw Exception(TR_MYSQL("Could not find table column definition!"));

  te::dt::Property* p = Convert2TerraLibProperty(columnsResult.get());

  return p;
}

void te::mysql::DataSourceCatalogLoader::getPrimaryKey(te::da::DataSetType* dt)
{
  std::auto_ptr<sql::ResultSet> result(m_md->getPrimaryKeys(m_catalogName, m_schemaName, dt->getName()));

  if(!result->next())
  {
    dt->setPrimaryKey(0);
    return;
  }

  //std::string pkName  = pkInfo->getString("PK_NAME"); -> this will return always PRIMARY
  std::string pkName  = result->getString("TABLE_NAME") + ".PRIMARY";

  std::auto_ptr<te::da::PrimaryKey> pk(new te::da::PrimaryKey(pkName));

  do
  {
    std::string colName = result->getString("COLUMN_NAME");

    te::dt::Property* p = dt->getProperty(colName);

    if(p == 0)
      throw Exception(TR_MYSQL("Could not found a column that takes part of a primary key!"));

    pk->add(p);

  }while(result->next());

// try to link the pk to an index
  te::da::Index* idx = dt->getIndex(pkName);
  pk->setAssociatedIndex(idx);

// add pk to dt
  dt->setPrimaryKey(pk.release());
}

void te::mysql::DataSourceCatalogLoader::getUniqueKeys(te::da::DataSetType* dt)
{
  dt->clearUniqueKeys();

  std::auto_ptr<sql::ResultSet> result(m_md->getIndexInfo(m_catalogName, m_schemaName, dt->getName(), true, false));

  if(!result->next())
    return;

  std::string ukName = result->getString("INDEX_NAME");

  std::auto_ptr<te::da::UniqueKey> uk(new te::da::UniqueKey(ukName));

  do
  {
    std::string colName = result->getString("COLUMN_NAME");
    
    te::dt::Property* p = dt->getProperty(colName);

    uk->add(p);

    if(p == 0)
      throw Exception(TR_MYSQL("Could not found a column that takes part of a unique key!"));

    if(!result->next())
    {
      if(ukName != "PRIMARY")
      {
        uk->setName(dt->getName() + "." + ukName);

// try to link the uk to an index
        te::da::Index* idx = dt->getIndex(uk->getName());
        uk->setAssociatedIndex(idx);

// add uk
        dt->add(uk.release());
      }
      else
      {
        uk.reset(0);  // release uk it will be reported as a primary key
      }

      break;
    }

    if(ukName != result->getString("INDEX_NAME"))
    {
      if(ukName != "PRIMARY")
      {
        uk->setName(dt->getName() + "." + ukName);
        
// try to link the uk to an index
        te::da::Index* idx = dt->getIndex(uk->getName());
        uk->setAssociatedIndex(idx);

// add uk
        dt->add(uk.release());
      }
      else
      {
        uk.reset(0);  // release uk it will be reported as a primary key
      }

      ukName = result->getString("INDEX_NAME");
      uk.reset(new te::da::UniqueKey(ukName));
    }
  }while(true);
}

void te::mysql::DataSourceCatalogLoader::getForeignKeys(te::da::DataSetType* /*dt*/, std::vector<std::string>& /*fkNames*/, std::vector<std::string>& /*rdts*/)
{
  //throw Exception(TR_MYSQL("Not implemented yet!"));
}

te::da::ForeignKey* te::mysql::DataSourceCatalogLoader::getForeignKey(const std::string& /*fkName*/, te::da::DataSetType* /*dt*/, te::da::DataSetType* /*rdt*/)
{
  //throw Exception(TR_MYSQL("Not implemented yet!"));
  return 0;
}

void te::mysql::DataSourceCatalogLoader::getIndexes(te::da::DataSetType* dt)
{
  dt->clearIndexes();

  std::string indexInfo("SELECT * FROM information_schema.statistics WHERE table_schema = '");
              indexInfo += m_schemaName;
              indexInfo += "' and table_name = '";
              indexInfo += dt->getName();
              indexInfo += "' ORDER BY NULLABLE, INDEX_TYPE, INDEX_NAME, SEQ_IN_INDEX";

  std::auto_ptr<te::da::DataSet> result(m_t->query(indexInfo));

  if(!result->moveNext())
    return;

  std::string idxName = result->getString("INDEX_NAME");
  std::string idxType = result->getString("INDEX_TYPE");
  bool isUnique = result->getInt64("NON_UNIQUE") == 0;

  std::auto_ptr<te::da::Index> idx(new te::da::Index(idxName, Convert2TerraLibIndexType(idxType)));

  do
  {
    std::string colName = result->getString("COLUMN_NAME");
    
    te::dt::Property* p = dt->getProperty(colName);

    idx->add(p);

    if(p == 0)
      throw Exception(TR_MYSQL("Could not found a column that takes part of an index!"));

    if(!result->moveNext())
    {
      idx->setName(dt->getName() + "." + idxName);

// look if there is an association with a pk or uk
      if(dt->getPrimaryKey() && (idxName == "PRIMARY") && isUnique)
      {
        dt->getPrimaryKey()->setAssociatedIndex(idx.get());
      }
      else if(isUnique)
      {
        te::da::UniqueKey* uk = dt->getUniqueKey(idx->getName());

        if(uk)
          uk->setAssociatedIndex(idx.get());
      }

// add idx
      dt->add(idx.release());

      break;
    }

    if(idxName != result->getString("INDEX_NAME"))
    {
      idx->setName(dt->getName() + "." + idxName);

// look if there is an association with a pk or uk
      if(dt->getPrimaryKey() && (idxName == "PRIMARY") && isUnique)
      {
        dt->getPrimaryKey()->setAssociatedIndex(idx.get());
      }
      else if(isUnique)
      {
        te::da::UniqueKey* uk = dt->getUniqueKey(idx->getName());

        if(uk)
          uk->setAssociatedIndex(idx.get());
      }

// add idx
      dt->add(idx.release());

      idxName = result->getString("INDEX_NAME");
      idxType = result->getString("INDEX_TYPE");
      isUnique = result->getInt64("NON_UNIQUE") == 0;;
      idx.reset(new te::da::Index(idxName, Convert2TerraLibIndexType(idxType)));
    }
  }while(true);
}

void te::mysql::DataSourceCatalogLoader::getCheckConstraints(te::da::DataSetType* /*dt*/)
{
  //throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSourceCatalogLoader::getSequences(std::vector<std::string*>& /*sequences*/)
{
  //throw Exception(TR_MYSQL("Not implemented yet!"));
}

te::da::Sequence* te::mysql::DataSourceCatalogLoader::getSequence(const std::string& /*seqName*/)
{
  //throw Exception(TR_MYSQL("Not implemented yet!"));
  return 0;
}

te::gm::Envelope* te::mysql::DataSourceCatalogLoader::getExtent(const te::dt::Property* sp)
{
  assert(sp->getParent());

  const std::string& tableName = sp->getParent()->getName();
  const std::string& geomColName = sp->getName();
  
  std::string sql("SELECT MIN(X(PointN(ExteriorRing(Envelope(");
              sql += geomColName;
              sql += ")), 1))), MIN(Y(PointN(ExteriorRing(Envelope(";
              sql += geomColName;
              sql += ")), 1))), MAX(X(PointN(ExteriorRing(Envelope(";
              sql += geomColName;
              sql += ")), 3))), MAX(Y(PointN(ExteriorRing(Envelope(";
              sql += geomColName;
              sql += ")), 3))) FROM ";
              sql += tableName;

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext() == false)
    throw Exception((boost::format(TR_MYSQL("Could not get a MBR for table/view: %1%!")) % tableName).str());

  double xmin = result->getDouble(0);
  double ymin = result->getDouble(1);
  double xmax = result->getDouble(2);
  double ymax = result->getDouble(3);

  return new te::gm::Envelope(xmin, ymin, xmax, ymax);  
}

void te::mysql::DataSourceCatalogLoader::loadCatalog(const bool full)
{
  te::da::DataSourceCatalog* catalog = m_t->getDataSource()->getCatalog();

  catalog->clear();

  boost::ptr_vector<std::string> datasets; 

  getDataSets(datasets);

  std::size_t size = datasets.size();

  for(std::size_t i = 0; i < size; i++) 
  {
    te::da::DataSetTypePtr dt(getDataSetType(datasets[i], full));

    dt->setId(static_cast<unsigned int>(i));

    catalog->add(dt);
  }
}

bool te::mysql::DataSourceCatalogLoader::hasDataSets()
{
  std::string sql;

  getDataSets(sql);

// get table list from database
  std::auto_ptr<te::da::DataSet> dsets(m_t->query(sql));

  if(dsets->moveNext())
    return true;

// last shot: see raster dataset
  if(datasetExists("te_tiled_rasters"))
  {
    std::auto_ptr<te::da::DataSet> rdsets(m_t->query("SELECT r_name FROM te_tiled_rasters"));

    return rdsets->moveNext();
  }

  return false;
}

bool te::mysql::DataSourceCatalogLoader::datasetExists(const std::string& name)
{
  std::list<sql::SQLString> table_types;

  table_types.push_back("TABLE");
  table_types.push_back("VIEW");

  std::auto_ptr<sql::ResultSet> result(m_md->getTables(m_catalogName, m_schemaName, name, table_types));

  return result->next();
}

bool te::mysql::DataSourceCatalogLoader::primarykeyExists(const std::string& name)
{
  std::auto_ptr<sql::ResultSet> result(0);
  
  std::size_t pos = name.find(".");

  if(pos == std::string::npos)  
  {
// if only table name is present, use it!
    result.reset(m_md->getPrimaryKeys(m_catalogName, m_schemaName, name));
  }
  else
  {
// we must split table-name.PRIMARY
    std::string tblName = name.substr(pos + 1);
    result.reset(m_md->getPrimaryKeys(m_catalogName, m_schemaName, tblName));
  }

  return result->next();
}

bool te::mysql::DataSourceCatalogLoader::uniquekeyExists(const std::string& name)
{
  std::size_t pos = name.find(".");

  if(pos == std::string::npos)  
    throw Exception(TR_MYSQL("Unique key names must have table name as prefix: <table-name>.<unique-key-name>"));

  std::string tblName = name.substr(0, pos);
  std::string ukName = name.substr(pos + 1);

  std::auto_ptr<sql::ResultSet> result(m_md->getIndexInfo(m_catalogName, m_schemaName, tblName, true, false));

  while(result->next())
  {
    std::string cukName = result->getString("INDEX_NAME");

    if(cukName == ukName)
      return true;
  }

  return false;
}

bool te::mysql::DataSourceCatalogLoader::foreignkeyExists(const std::string& /*name*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

bool te::mysql::DataSourceCatalogLoader::checkConstraintExists(const std::string& /*name*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

bool te::mysql::DataSourceCatalogLoader::indexExists(const std::string& name)
{
  std::size_t pos = name.find(".");

  if(pos == std::string::npos)  
    throw Exception(TR_MYSQL("Index names must have table name as prefix: <table-name>.<idx-name>"));

  std::string tblName = name.substr(0, pos);
  std::string idxName = name.substr(pos + 1);

  std::auto_ptr<sql::ResultSet> result(m_md->getIndexInfo(m_catalogName, m_schemaName, tblName, false, false));

  while(result->next())
  {
    std::string cidxName = result->getString("INDEX_NAME");

    if(cidxName == idxName)
      return true;
  }

  return false;
}

bool te::mysql::DataSourceCatalogLoader::sequenceExists(const std::string& /*name*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::mysql::DataSourceCatalogLoader::getTransactor() const
{
  return m_t;
}

std::string te::mysql::DataSourceCatalogLoader::getTableEngine(const std::string& name)
{
  std::string sql("SELECT engine FROM information_schema.tables WHERE table_schema = '");
              sql += m_schemaName;
              sql += "' and table_name = '";
              sql += name;
              sql += "'";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(!result->moveNext())
    throw Exception((boost::format(TR_MYSQL("Could not find database engine for table %1%")) % name).str());

  return result->getString(0);
}

te::da::DataSetType* te::mysql::DataSourceCatalogLoader::getRasterDataSet(const std::string& name)
{
  std::map<std::string, te::da::DataSetType*>& rasters = m_t->getMyDataSource()->getRasters();

  std::map<std::string, te::da::DataSetType*>::iterator it = rasters.find(name);

  if((it != rasters.end()) && (it->second != 0))
    return static_cast<te::da::DataSetType*>(it->second->clone());

  std::string sql("SELECT * FROM te_tiled_rasters WHERE r_name = '");
              sql += name;
              sql += "'";

  std::auto_ptr<te::da::DataSet> rdset(m_t->query(sql));

  if(!rdset->moveNext())
    throw Exception((boost::format(TR_MYSQL("Raster %1% should exist?")) % name).str());

  int r_id = rdset->getInt32("r_id");
  std::string r_name = rdset->getString("r_name");
  std::string r_title = !rdset->isNull("r_title") ? rdset->getString("r_title") : std::string("");
  std::string r_description = !rdset->isNull("r_description") ? rdset->getString("r_description") : std::string("");
  int r_srid = rdset->getInt32("r_srid");
  int r_nrows = rdset->getInt32("r_nrows");
  int r_ncols = rdset->getInt32("r_ncols");
  int r_blk_width = rdset->getInt32("r_blk_width");
  int r_blk_height = rdset->getInt32("r_blk_height");
  double r_resx = rdset->getDouble("r_resx");
  double r_resy = rdset->getDouble("r_resy");
  double r_minx = rdset->getDouble("r_minx");
  double r_miny = rdset->getDouble("r_miny");
  double r_maxx = rdset->getDouble("r_maxx");
  double r_maxy = rdset->getDouble("r_maxy");
  bool r_split_bands = rdset->getBool("r_split_bands");
  
  int nblocksx = (r_ncols + r_blk_width - 1) / r_blk_width;
  int nblocksy = (r_nrows + r_blk_height - 1) / r_blk_height;

  te::gm::Envelope* mbr = new te::gm::Envelope(r_minx, r_miny, r_maxx, r_maxy);

  std::auto_ptr<te::rst::Grid> grid(new te::rst::Grid(r_ncols, r_nrows, r_resx, r_resy, mbr, r_srid));

  std::map<std::string, std::string> rinfo;
  rinfo["MY_RASTER_ID"] = boost::lexical_cast<std::string>(r_id);
  //rinfo["MY_RASTER_TITLE"] = r_title;
  rinfo["MY_RASTER_DESCRIPTION"] = r_description;
  rinfo["MY_RASTER_SPLIT_BANDS"] = r_split_bands ? "TRUE" : "FALSE";  

  sql  = "SELECT * FROM te_tiled_raster_band WHERE r_id = ";
  sql += boost::lexical_cast<std::string>(r_id);
  sql += " ORDER BY b_band_num";

  std::auto_ptr<te::da::DataSet> bandsdset(m_t->query(sql));

// todo: make bands vector more memory safer
  std::vector<te::rst::BandProperty*> bands;

  while(bandsdset->moveNext())
  {
    int b_band_id = bandsdset->getInt32("b_band_id");
    int b_band_num = bandsdset->getInt32("b_band_num");
    int b_band_datatype = bandsdset->getInt32("b_band_datatype");
    double b_nodata = bandsdset->getInt32("b_nodata");
    std::string b_description = bandsdset->getString("b_description");
    std::string b_band_table_name = bandsdset->getString("b_band_table_name");

    te::rst::BandProperty* bp = new te::rst::BandProperty(b_band_num, b_band_datatype, b_description);
    bp->m_blkh = r_blk_height;
    bp->m_blkw = r_blk_width;
    bp->m_noDataValue = b_nodata;
    bp->m_nblocksx = nblocksx;
    bp->m_nblocksy = nblocksy;
    //bp->m_metadata["b_band_table_name"] = b_band_table_name;
    //bp->m_metadata["b_band_id"] = boost::lexical_cast<std::string>(b_band_id);

    bands.push_back(bp);
  }

  std::auto_ptr<te::rst::RasterProperty> rproperty(new te::rst::RasterProperty(grid.release(), bands, rinfo));

  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(name));

  dt->add(rproperty.release());

  dt->setTitle(r_title);

  rasters[name] = static_cast<te::da::DataSetType*>(dt->clone());

  return dt.release();
}

void te::mysql::DataSourceCatalogLoader::getDataSets(std::string& sql)
{
// create the list of tables to be omitted
  std::vector<std::string> omittTables;

  const std::map<std::string, std::string>& dinfo = m_t->getDataSource()->getConnectionInfo();

  std::map<std::string, std::string>::const_iterator it = dinfo.find("MY_HIDE_TABLES");
  std::map<std::string, std::string>::const_iterator itend = dinfo.end();

  if(it != itend)
    te::common::Tokenize(it->second, omittTables, ",");

  it = dinfo.find("MY_HIDE_METADATA_TABLES");

  if((it != itend) && (te::common::Convert2UCase(it->second) == "TRUE"))
  {
    omittTables.push_back("te_tiled_rasters");
    omittTables.push_back("te_tiled_raster_band");
    omittTables.push_back("te_tiled_raster_pyramids");
    omittTables.push_back("te_raster_columns");
    omittTables.push_back("te_raster_tables");
    omittTables.push_back("geometry_columns");
    omittTables.push_back("spatial_ref_sys");
  }

  it = dinfo.find("MY_HIDE_RASTER_TABLES");

  if((it != itend) && datasetExists("te_tiled_raster_band"))
  {
    std::auto_ptr<te::da::DataSet> rasters(m_t->query("SELECT b_band_table_name FROM te_tiled_raster_band"));

    while(rasters->moveNext())
      omittTables.push_back(rasters->getString(0));
  }

// create query
  sql  = "SELECT TABLE_NAME FROM information_schema.tables WHERE table_schema = '";
  sql += m_schemaName;
  sql += "'";

// exclude omitted tables
  if(omittTables.size() > 0)
  {
    sql += " AND table_name NOT IN (";

    for(std::size_t i = 0; i < omittTables.size(); ++i)
    {
      if(i != 0)
        sql += ",";

      sql += "'";
      sql += omittTables[i];
      sql += "'";
    }

    sql += ")";
  }
}

