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
  \file terralib/mysql/DataSetTypePersistence.cpp

  \brief DataSetTypePersistence implementation for MySQL data access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../datatype/Property.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterProperty.h"
#include "DataSetTypePersistence.h"
#include "DataSource.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

te::mysql::DataSetTypePersistence::DataSetTypePersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::mysql::DataSetTypePersistence::~DataSetTypePersistence()
{
}

void te::mysql::DataSetTypePersistence::create(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  assert(dt && (dt->size() > 0));

  if(dt->getProperty(0)->getType() == te::dt::RASTER_TYPE)
  {
    createRaster(dt, options);
    return;
  }

// determine the database engine to use
  std::string engine;

  if(options.count("ENGINE") > 0)
    engine = options.find("ENGINE")->second;

  if(dt->hasGeom() && engine.empty())
  {
    const te::da::DataSource* ds = m_t->getDataSource();

    assert(ds);

    if(ds->getConnectionInfo().count("MY_DEFAULT_ENGINE_FOR_SPATIAL_TABLES") > 0)
      engine = ds->getConnectionInfo().find("MY_DEFAULT_ENGINE_FOR_SPATIAL_TABLES")->second;
  }

// create table
  const std::size_t nproperties = dt->size();

  if(nproperties == 0)
    throw Exception(TR_MYSQL("MySQL doesn't support empty table definition!"));

  std::string sql  = "CREATE TABLE ";
              sql += dt->getName();
              sql += "(";

  for(std::size_t i = 0; i < nproperties; ++i)
  {
    const te::dt::Property* p = dt->getProperty(i);

    if(i != 0)
      sql += ", ";

    sql += Convert2SQLCreate(p);
  }

  sql += ") ";

  if(!engine.empty())
  {
    sql += "ENGINE = ";
    sql += engine;
  }

  m_t->execute(sql);

// let's find table id
  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());

// add-pk
  if(dt->getPrimaryKey())
    add(dt, dt->getPrimaryKey(), false);

// add-uk 
  const std::size_t nuks = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < nuks; ++i)
    add(dt, dt->getUniqueKey(i), false);

// add indexes just if no primary key or unique key with the same name exists!
  std::size_t nidxs = dt->getNumberOfIndexes();

  for(std::size_t i = 0; i < nidxs; ++i)
  {
    te::da::Index* idx = dt->getIndex(i);

    std::string idxname = idx->getName();

    std::size_t pos = idx->getName().find(".");

    if(pos == std::string::npos)
      idxname = dt->getName() + "." + idxname.substr(pos + 1);

    if(cloader->indexExists(idxname))
      continue;
    
    add(dt, dt->getIndex(i), options, false);
  }

// refresh pk, uk, indexes
  cloader->getPrimaryKey(dt);
  cloader->getUniqueKeys(dt);
  cloader->getIndexes(dt);

//// add fks
//  std::size_t nfks = dt->getNumberOfForeignKeys();
//
//  for(std::size_t i = 0; i < nfks; ++i)
//    add(dt, dt->getForeignKey(i));
//
//// add constraints
//  std::size_t nccs = dt->getNumberOfCheckConstraints();
//
//  for(std::size_t i = 0; i < nccs; ++i)
//    add(dt, dt->getCheckConstraint(i));
//  
//  cloader->getCheckConstraints(dt);
//  cloader->getForeignKeys(dt);

// register geometry metadata if needed
  for(std::size_t i = 0; i < nproperties; ++i)
  {
    const te::dt::Property* p = dt->getProperty(i);

    if(p->getType() != te::dt::GEOMETRY_TYPE)
      continue;

    const te::gm::GeometryProperty* gp = static_cast<const te::gm::GeometryProperty*>(p);

    std::string gtype;
    std::string cdim;

    Convert2MySQLGeom(gp->getGeometryType(), gtype, cdim);

    sql  = "INSERT INTO geometry_columns VALUES(NULL, NULL, '";
    sql += dt->getName();
    sql += "', '";
    sql += gp->getName();
    sql += "', ";
    sql += cdim;
    sql += ", ";
    sql += boost::lexical_cast<std::string>(gp->getSRID());
    sql += ", '";
    sql += gtype;
    sql += "')";

    m_t->execute(sql);
  }
}

void te::mysql::DataSetTypePersistence::clone(const std::string& datasetName, const std::string& newDatasetName, const std::map<std::string, std::string>& options)
{
  std::map<std::string, std::string>::const_iterator it = options.find("ENGINE");

  boost::format sql("CREATE TABLE %1% %2% SELECT * FROM %3%");
  sql.bind_arg(1, newDatasetName);
  sql.bind_arg(2, it != options.end() ? "ENGINE = " + it->second : "");
  sql.bind_arg(3, datasetName);

  m_t->execute(sql.str());

// TODO: criar os indices e todos os modificadores necessarios
  boost::format pk("ALTER TABLE %1% ADD PRIMARY KEY(row_id)");
  pk.bind_arg(1, newDatasetName);
  m_t->execute(pk.str());
}

void te::mysql::DataSetTypePersistence::drop(te::da::DataSetType* dt)
{
  std::string sql  = "DROP TABLE ";
              sql += dt->getName();

  m_t->execute(sql);

  if(dt->getCatalog())
  {
    te::da::DataSourceCatalog* catalog = dt->getCatalog();
    catalog->remove(dt, true);
  }
  else
  {
    delete dt;
  }
}

void te::mysql::DataSetTypePersistence::drop(const std::string& /*datasetName*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSetTypePersistence::rename(te::da::DataSetType* dt, const std::string& newName)
{
  std::string newTableName, newTableSchema, oldTableName, oldTableSchema;

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " RENAME ";
              sql += newName;

  m_t->execute(sql);

  if(dt->getCatalog())
  {
    te::da::DataSourceCatalog* catalog = dt->getCatalog();
    catalog->rename(dt, newName);
  }
  else
  {
    dt->setName(newName);
  }

  dt->setCompositeName(newName);

// refresh all indexes, uks, pks, ...
  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(m_t->getCatalogLoader());

  cloader->getPrimaryKey(dt);
  cloader->getUniqueKeys(dt);
  cloader->getIndexes(dt);
  //loader->getCheckConstraints(dt);
  //loader->getForeignKeys(dt);
}

void te::mysql::DataSetTypePersistence::add(te::da::DataSetType* dt, te::dt::Property* p)
{
  std::string sql = "ALTER TABLE ";
              sql += dt->getName();
              sql += " ADD COLUMN (";
              sql += p->getName();
              sql += " ";
              sql += GetSQLType(p);
              sql += ")";

  m_t->execute(sql); 
  
// add property to dataset type
  if(p->getParent() == 0)
    dt->add(p); 

// refresh constraints
  //std::auto_ptr<DataSourceCatalogLoader> loader(m_t->getMyCatalogLoader());  
  //loader->getCheckConstraints(dt);
}

void te::mysql::DataSetTypePersistence::drop(te::dt::Property* p)
{
  if((p->getParent() == 0) || (p->getParent()->getType() != te::dt::DATASET_TYPE))
    throw Exception(TR_MYSQL("The informed property is not valid!"));
  
  te::da::DataSetType* dt = static_cast<te::da::DataSetType*>(p->getParent());

  std::string sql  = " ALTER TABLE ";
              sql += p->getParent()->getName();
              sql += " DROP COLUMN ";
              sql += p->getName();

  m_t->execute(sql);

  dt->remove(p);

  std::auto_ptr<DataSourceCatalogLoader> loader(m_t->getMyCatalogLoader());
  
  loader->getPrimaryKey(dt);
  loader->getUniqueKeys(dt);
  loader->getIndexes(dt);
  //loader->getCheckConstraints(dt);
  //loader->ForeignKeys(dt);
}

void te::mysql::DataSetTypePersistence::rename(te::dt::Property* p, const std::string& newName) 
{
  if((p->getParent() == 0) || (p->getParent()->getType() != te::dt::DATASET_TYPE))
    throw Exception(TR_MYSQL("The informed property is not valid!"));
    
  te::da::DataSetType* dt = static_cast<te::da::DataSetType*>(p->getParent());

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " CHANGE COLUMN ";
              sql += p->getName();
              sql += " ";
              sql += newName;

  m_t->execute(sql);

  p->setName(newName);

  //std::auto_ptr<DataSourceCatalogLoader> loader(m_t->getMyCatalogLoader());  
  //loader->getCheckConstraints(dt);
}

void te::mysql::DataSetTypePersistence::update(te::dt::Property* /*oldP*/, te::dt::Property* /*newP*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::PrimaryKey* pk) 
{
  add(dt, pk, true);
}

void te::mysql::DataSetTypePersistence::drop(te::da::PrimaryKey* pk)
{
  if(pk->getDataSetType() == 0)
    throw Exception(TR_MYSQL("The informed primary key is not associated to a table definition!"));

  te::da::DataSetType* dt = pk->getDataSetType();

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " DROP PRIMARY KEY";

  m_t->execute(sql);
  
  dt->setPrimaryKey(0);
  
  std::auto_ptr<te::da::DataSourceCatalogLoader> loader(m_t->getCatalogLoader());
  
  loader->getIndexes(dt);
}

void te::mysql::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::UniqueKey* uk) 
{
  add(dt, uk, true);
}

void te::mysql::DataSetTypePersistence::drop(te::da::UniqueKey* uk)
{
  if(uk->getDataSetType() == 0)
    throw Exception(TR_MYSQL("The informed unique key is not associated to a table definition!"));

  std::size_t pos = uk->getName().find(".");

  std::string ukname = uk->getName();
  
  if(pos != std::string::npos)
    ukname = uk->getName().substr(pos + 1);

  te::da::DataSetType* dt = uk->getDataSetType();

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " DROP KEY ";
              sql += ukname;

  m_t->execute(sql);

  dt->remove(uk);

  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());
  
  cloader->getIndexes(dt);
}

void te::mysql::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::Index* index, const std::map<std::string, std::string>& options) 
{
  add(dt, index, options, true);
}

void te::mysql::DataSetTypePersistence::drop(te::da::Index* index)
{
  if(index->getDataSetType() == 0)
    throw Exception(TR_MYSQL("The informed index is not associated to a table definition!"));

  std::size_t pos = index->getName().find(".");

  std::string idxname = index->getName();
  
  if(pos != std::string::npos)
    idxname = index->getName().substr(pos + 1);

  te::da::DataSetType* dt = index->getDataSetType();

  std::string sql("DROP INDEX ");
              sql += idxname;
              sql += " ON ";
              sql += dt->getName();

  m_t->execute(sql);

  dt->remove(index);

  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());
  
  cloader->getPrimaryKey(dt);
  cloader->getUniqueKeys(dt);
}

void te::mysql::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::ForeignKey* /*fk*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSetTypePersistence::drop(te::da::ForeignKey* /*fk*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::CheckConstraint* /*cc*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSetTypePersistence::drop(te::da::CheckConstraint* /*cc*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSetTypePersistence::create(te::da::Sequence* /*sequence*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::DataSetTypePersistence::drop(te::da::Sequence* /*sequence*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::mysql::DataSetTypePersistence::getTransactor() const
{
  return m_t;
}

void te::mysql::DataSetTypePersistence::checkNotNull(te::da::DataSetType* dt, te::da::Index* index) 
{
  if(index->getIndexType() != te::da::R_TREE_TYPE)
    return;

  const std::vector<te::dt::Property*>& properties = index->getProperties();

  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    te::dt::SimpleProperty* sp = static_cast<te::dt::SimpleProperty*>(properties[i]);

    if(sp->isRequired())
      continue;

    std::string sql = "ALTER TABLE ";
                sql += dt->getName();
                sql += " MODIFY COLUMN ";
                sql += sp->getName();
                sql += " ";
                sql += GetSQLType(sp);
                sql += " NOT NULL";

    m_t->execute(sql);

    sp->setRequired(true);
  }
}

void te::mysql::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::PrimaryKey* pk, const bool refresh) 
{
  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " ADD PRIMARY KEY (";

  const std::vector<te::dt::Property*>& properties = pk->getProperties();

  const std::size_t size = properties.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += properties[i]->getName();
  }

  sql += ")";

  m_t->execute(sql);

  if(pk->getDataSetType() == 0)
    dt->setPrimaryKey(pk);

  if(refresh)
  {
    std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());
    cloader->getPrimaryKey(dt);
    cloader->getIndexes(dt);
  }
}

void te::mysql::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::UniqueKey* uk, const bool refresh) 
{
  std::size_t pos = uk->getName().find(".");

  std::string ukname = uk->getName();
  
  if(pos != std::string::npos)
    ukname = uk->getName().substr(pos + 1);
  else
    uk->setName(dt->getName() + "." + uk->getName());

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " ADD UNIQUE KEY ";
              sql += ukname;
              sql += " (";

  const std::vector<te::dt::Property*>& properties = uk->getProperties();

  std::size_t size = properties.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += properties[i]->getName();
  }

  sql += ")";

  m_t->execute(sql);

  if(uk->getDataSetType() == 0)
    dt->add(uk);

  if(refresh)
  {
    std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());
    cloader->getIndexes(dt);
  }
}

void te::mysql::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::Index* index, const std::map<std::string, std::string>& /*options*/, const bool refresh) 
{
  checkNotNull(dt, index);

// find out if it is a unique index or not
  bool isunique = false;

  if(dt->getPrimaryKey() && (dt->getPrimaryKey()->getAssociatedIndex() == index))
  {
    isunique = true;
  }
  else
  {
    std::size_t size = dt->getNumberOfUniqueKeys();

    for(std::size_t i = 0; i < size; ++i)
    {
      if(dt->getUniqueKey(i)->getAssociatedIndex() == index)
      {
        isunique = true;
        break;
      }
    }
  }

  std::string idxname = index->getName();
  
  std::size_t pos = index->getName().find(".");

  if(pos != std::string::npos)
    idxname = index->getName().substr(pos + 1);
  else
    index->setName(dt->getName() + "." + index->getName());

  std::string sql = "CREATE ";

  if(index->getIndexType() == te::da::R_TREE_TYPE)
    sql += "SPATIAL ";
  else if(index->getIndexType() == te::da::HASH_TYPE)
    sql += "HASH ";

  if(isunique)
    sql += "UNIQUE ";

  sql += "INDEX ";
  sql += idxname;
  sql += " ON ";
  sql += dt->getName();
  sql += "(";

  std::size_t size = index->getProperties().size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += index->getProperties()[i]->getName();
  }

  sql += ")";
  
  m_t->execute(sql);

// add index to dataset type if it was not yet!
  if(index->getDataSetType() == 0)
    dt->add(index);

  if(isunique && refresh)
  {
    std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());
    cloader->getUniqueKeys(dt);
  }
}

void te::mysql::DataSetTypePersistence::createRaster(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  assert(m_t && m_t->getDataSource() && dt && (dt->size() > 0) && (dt->getProperty(0)->getType() == te::dt::RASTER_TYPE));

  std::string tbltype = "";

  {
    std::map<std::string, std::string>::const_iterator it = options.find("ENGINE");
    std::map<std::string, std::string>::const_iterator itend = options.end();

    if(it != itend)
      tbltype = " ENGINE = " + it->second;
    else if(m_t->getDataSource()->getConnectionInfo().count("MY_DEFAULT_ENGINE_FOR_SPATIAL_TABLES") > 0)
      tbltype = " ENGINE = " + m_t->getDataSource()->getConnectionInfo().find("MY_DEFAULT_ENGINE_FOR_SPATIAL_TABLES")->second;
  }

  te::rst::RasterProperty* rp = dynamic_cast<te::rst::RasterProperty*>(dt->getProperty(0));

  if(rp == 0)
    throw Exception(TR_MYSQL("Invalid raster property!"));

  const std::map<std::string, std::string>& rinfo = rp->getInfo();

  std::map<std::string, std::string>::const_iterator it = rinfo.find("MY_RASTER_TITLE");
  std::map<std::string, std::string>::const_iterator itend = rinfo.end();

  std::string r_title = (it != itend) ? it->second : (!dt->getTitle().empty() ? dt->getTitle() : std::string(""));

  dt->setTitle(r_title);

  it = rinfo.find("MY_RASTER_DESCRIPTION");
  std::string r_description = (it != itend) ? it->second : std::string("");

  it = rinfo.find("MY_RASTER_SPLIT_BANDS");
  std::string r_split_bands = (it != itend) ? it->second : std::string("FALSE");

  const te::rst::Grid* grid = rp->getGrid();

  const std::vector<te::rst::BandProperty*>& bands = rp->getBandProperties();

// insert raster metadata
  boost::format insertTiledRasters("INSERT INTO te_tiled_rasters (r_name, r_title, r_description, r_srid, r_nrows, r_ncols, "
                                   "r_blk_width, r_blk_height, r_resx, r_resy, r_minx, r_miny, r_maxx, "
                                   "r_maxy, r_split_bands) VALUES('%1%', '%2%', '%3%', %4%, %5%, %6%, %7%, %8%, "
                                   "%9%, %10%, %11%, %12%, %13%, %14%, %15%)");

  insertTiledRasters.bind_arg(1, dt->getName());
  insertTiledRasters.bind_arg(2, r_title);
  insertTiledRasters.bind_arg(3, r_description);
  insertTiledRasters.bind_arg(4, grid->getSRID());
  insertTiledRasters.bind_arg(5, grid->getNumberOfRows());
  insertTiledRasters.bind_arg(6, grid->getNumberOfColumns());
  insertTiledRasters.bind_arg(7, bands[0]->m_blkw);
  insertTiledRasters.bind_arg(8, bands[0]->m_blkh);
  insertTiledRasters.bind_arg(9, boost::lexical_cast<std::string>(grid->getResolutionX()));
  insertTiledRasters.bind_arg(10, boost::lexical_cast<std::string>(grid->getResolutionY()));
  insertTiledRasters.bind_arg(11, boost::lexical_cast<std::string>(grid->getExtent()->getLowerLeftX()));
  insertTiledRasters.bind_arg(12, boost::lexical_cast<std::string>(grid->getExtent()->getLowerLeftY()));
  insertTiledRasters.bind_arg(13, boost::lexical_cast<std::string>(grid->getExtent()->getUpperRightX()));
  insertTiledRasters.bind_arg(14, boost::lexical_cast<std::string>(grid->getExtent()->getUpperRightY()));
  insertTiledRasters.bind_arg(15, r_split_bands);

  m_t->execute(insertTiledRasters.str());

  boost::int64_t rid = m_t->getLastInsertId();

  if(r_split_bands == "TRUE")
  {
// split bands
    for(std::size_t i = 0; i < bands.size(); ++i)
    {
// determine table name for he band
      std::string bTableName = dt->getName() + "_band" + boost::lexical_cast<std::string>(i);

// insert band metadata
      boost::format insertTiledRasterBand("INSERT INTO te_tiled_raster_band (r_id, b_band_num, b_band_datatype, b_nodata, "
                                          "b_description, b_band_table_name) VALUES(%1%, %2%, %3%, %4%, '%5%', '%6%')");

      insertTiledRasterBand.bind_arg(1, rid);
      insertTiledRasterBand.bind_arg(2, i);
      insertTiledRasterBand.bind_arg(3, bands[i]->m_type);
      insertTiledRasterBand.bind_arg(4, boost::lexical_cast<std::string>(bands[i]->m_noDataValue));
      insertTiledRasterBand.bind_arg(5, bands[i]->m_description);
      insertTiledRasterBand.bind_arg(6, bTableName);

      m_t->execute(insertTiledRasterBand.str());

// create band data table
      boost::format createTiledRasterBand("CREATE TABLE %1% (tile_id INTEGER PRIMARY KEY, data_blk LONGBLOB) %2%");

      createTiledRasterBand.bind_arg(1, bTableName);
      createTiledRasterBand.bind_arg(2, tbltype);

      m_t->execute(createTiledRasterBand.str());

      //bands[i]->m_metadata["b_band_table_name"] = bTableName;
    }
  }
  else
  {
// don't split bands
    std::string bTableName = dt->getName() + "_bands";

    for(std::size_t i = 0; i < bands.size(); ++i)
    {
// insert band metadata
      boost::format insertTiledRasterBand("INSERT INTO tiled_raster_band (r_id, b_band_num, b_band_datatype, b_nodata, "
                                          "b_description, b_band_table_name) VALUES(%1%, %2%, %3%, %4%, '%5%', '%6%')");

      insertTiledRasterBand.bind_arg(1, rid);
      insertTiledRasterBand.bind_arg(2, i);
      insertTiledRasterBand.bind_arg(3, bands[i]->m_type);
      insertTiledRasterBand.bind_arg(4, boost::lexical_cast<std::string>(bands[i]->m_noDataValue));
      insertTiledRasterBand.bind_arg(5, bands[i]->m_description);
      insertTiledRasterBand.bind_arg(6, bTableName);

      m_t->execute(insertTiledRasterBand.str());

      //bands[i]->m_metadata["b_band_table_name"] = bTableName;
    }

// create band data table
    boost::format createTiledRasterBand("CREATE TABLE %1% (tile_id INTEGER PRIMARY KEY, data_blk LONGBLOB) %2%");

    createTiledRasterBand.bind_arg(1, bTableName);
    createTiledRasterBand.bind_arg(2, tbltype);

    m_t->execute(createTiledRasterBand.str());
  }

// register raster dataset in data source
  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getMyCatalogLoader());
  std::auto_ptr<te::da::DataSetType> aux(cloader->getRasterDataSet(dt->getName()));
}

