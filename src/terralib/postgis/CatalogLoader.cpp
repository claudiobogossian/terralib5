/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/postgis/CatalogLoader.cpp

  \brief It allows to retrieve information about datasets in a PostGIS data source.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/Sequence.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../datatype/Array.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../geometry/GeometryProperty.h"
#include "../raster/Grid.h"
#include "../raster/BandProperty.h"
#include "../raster/RasterProperty.h"
#include "CatalogLoader.h"
#include "Connection.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "Transactor.h"
#include "Utils.h"

// STL
#include <memory>

// libpq
#include <libpq-fe.h>

te::pgis::CatalogLoader::CatalogLoader(Transactor* t)
  : m_t(t)
{
}

te::pgis::CatalogLoader::~CatalogLoader()
{
}

void te::pgis::CatalogLoader::getDataSets(boost::ptr_vector<std::string>& datasets)
{
  std::auto_ptr<te::da::DataSet> tables(getDataSetTypes());
 
  while(tables->moveNext())
  {
    std::string* tname = new std::string(tables->getString(1) + "." + tables->getString(2));
    datasets.push_back(tname);
  }
}

te::da::DataSetType* te::pgis::CatalogLoader::getDataSetType(const std::string& datasetName, const bool full)
{
  assert(m_t->getPGDataSource() && m_t->getPGDataSource()->getCurrentSchema());
// find table oid
  unsigned int dtid = getTableId(datasetName);
  
// create dataset type and get its properties and associated constraints
  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(datasetName, dtid));
  dt->setTitle(datasetName);

  getProperties(dt.get());

  if(full)
  {
    getConstraints(dt.get());    
    getIndexes(dt.get());
  }

  dt->setFullLoaded(full);
  return dt.release();
}

void te::pgis::CatalogLoader::getProperties(te::da::DataSetType* dt)
{
  if(dt->getId() == 0)
  {
    unsigned int dtid = getTableId(dt->getName());

    dt->setId(dtid);
  }

  std::auto_ptr<te::da::DataSet> result(getProperties(dt->getId()));

  while(result->moveNext())
  {
    unsigned int attNum = result->getInt16(0);
    std::string attName = result->getString(1);
    unsigned int attType = result->getInt32(2);
    bool attNotNull = result->getBool(3);
    std::string fmt = result->getString(4);
    bool attHasDefault = result->getBool(5);
    std::string attDefValue = result->getString(6);
    int ndims = result->getInt32(7);

    te::dt::Property* p =  Convert2TerraLib(attNum, attName.c_str(), attType, attNotNull,
                                            fmt.c_str(), attHasDefault, attDefValue.c_str(),
                                            ndims, m_t->getPGDataSource()->getGeomTypeId(),
                                            m_t->getPGDataSource()->getRasterTypeId());
    dt->add(p);

    if(p->getType() == te::dt::GEOMETRY_TYPE)
    {
      dt->setDefaultGeomProperty(static_cast<te::gm::GeometryProperty*>(p));
      getGeometryInfo(dt->getName(), dt->getDefaultGeomProperty());
    }
    else if(p->getType() == te::dt::RASTER_TYPE)
    {
      dt->setDefaultRasterProperty(static_cast<te::rst::RasterProperty*>(p));
      getRasterInfo(dt->getName(), dt->getDefaultRasterProperty());
    }
  }
}

te::dt::Property* te::pgis::CatalogLoader::getProperty(const std::string& /*datasetName*/, const std::string& /*propertyName*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::CatalogLoader::getPrimaryKey(te::da::DataSetType* dt)
{
// retrieve the pk from db and if not found removes any pk in the dataset
  std::auto_ptr<te::da::DataSet> result(getConstraints(dt->getId(), 'p'));

  if(result->moveNext())
    addPrimaryKey(result.get(), dt);
  else
    dt->setPrimaryKey(0);
}

void te::pgis::CatalogLoader::getUniqueKeys(te::da::DataSetType* dt)
{
  dt->clearUniqueKeys();

  std::auto_ptr<te::da::DataSet> result(getConstraints(dt->getId(), 'u'));

  while(result->moveNext())
    addUniqueKey(result.get(), dt);  
}

void te::pgis::CatalogLoader::getForeignKeys(te::da::DataSetType* dt, std::vector<std::string>& fkNames, std::vector<std::string>& rdts)
{
// retrieves the list of fks and related tables
  std::auto_ptr<te::da::DataSet> result(getFKs(dt));

  while(result->moveNext())
  {
    std::string fkschema = result->getString(0);
    std::string fkname = result->getString(1);
    std::string sname = result->getString(2);
    std::string tname = result->getString(3);

    fkNames.push_back(fkschema + "." + fkname);
    rdts.push_back(sname + "." + tname);
  }
}

te::da::ForeignKey* te::pgis::CatalogLoader::getForeignKey(const std::string& fkName, te::da::DataSetType* dt, te::da::DataSetType* rdt)
{
// retrieves the fk info
  std::auto_ptr<te::da::DataSet> result(getFK(fkName, dt, rdt));

  if(!result->moveNext())
    return 0;

  unsigned int id = result->getInt32(0);
  char onUpdate = result->getChar(1);
  char onDeletion = result->getChar(2);
  //char matchType = result->getChar(3);
  std::vector<boost::int16_t> fkCols;
  std::vector<boost::int16_t> fkRefCols;
  result->getArray(4, fkCols);
  result->getArray(5, fkRefCols);

  assert(fkCols.size() == fkRefCols.size());

  te::da::ForeignKey* fk = new te::da::ForeignKey(fkName, id);
  fk->setOnUpdateAction(GetAction(onUpdate));
  fk->setOnDeleteAction(GetAction(onDeletion));
  fk->setReferencedDataSetType(rdt);
  
  std::size_t size = fkCols.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    fk->addRefProperty(rdt->getPropertyById(fkRefCols[i]));
    fk->add(dt->getPropertyById(fkCols[i]));
  }

  return fk;
}

void te::pgis::CatalogLoader::getIndexes(te::da::DataSetType* dt)
{
  dt->clearIndexes();

  std::auto_ptr<te::da::DataSet> result(getIndexes(dt->getId()));

  while(result->moveNext())
    addIndex(result.get(), dt);
}

void te::pgis::CatalogLoader::getCheckConstraints(te::da::DataSetType* dt)
{
  dt->clearCheckConstraints();

  std::auto_ptr<te::da::DataSet> result(getConstraints(dt->getId(), 'c'));

  while(result->moveNext())
    addCheckConstraint(result.get(), dt);  
}

void te::pgis::CatalogLoader::getSequences(std::vector<std::string*>& sequences)
{
  std::auto_ptr<te::da::DataSet> seqs(getSequences());
 
  while(seqs->moveNext())
  {
    std::string* sname = new std::string(seqs->getString(1) + "." + seqs->getString(2));
    sequences.push_back(sname);
  }
}

te::da::Sequence* te::pgis::CatalogLoader::getSequence(const std::string& seqName)
{
  std::string sql("SELECT * FROM ");
              sql += seqName;

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext() == false)
    throw Exception(TR_PGIS("Could not find information about given sequence in the database!"));

  unsigned int sid = getTableId(seqName);

  te::da::Sequence* seq = new te::da::Sequence(seqName, 0, 0, 0, sid);

// "is_cycled"
  if(result->getBool(7))
    seq->setAsCycle();
  else
    seq->setAsNoCycle();


  seq->setCachedValues(result->getInt64(5)); // "cache_value"
  seq->setIncrement(result->getInt64(2));    // "increment_by";
  seq->setMaxValue(result->getInt64(3));     // "max_value";
  seq->setMinValue(result->getInt64(4));     // "min_value";

  return seq;
}

te::gm::Envelope* te::pgis::CatalogLoader::getExtent(const te::gm::GeometryProperty* gp)
{
  assert(gp->getParent());

  std::string sql("SELECT ST_Extent(");
              sql += gp->getName();
              sql += ") FROM ";
              sql += gp->getParent()->getName();

  PGresult* result = PQexec(m_t->getConnection()->getConn(), sql.c_str());

  if(PQresultStatus(result) != PGRES_TUPLES_OK)
  {
    std::string errmsg(TR_PGIS("Could not find mbr for the given geometry property due to the following error: "));
                errmsg += PQerrorMessage(m_t->getConnection()->getConn());
    PQclear(result);
    throw Exception(errmsg);
  }

  const char* boxStr = PQgetvalue(result, 0, 0);
  
  te::gm::Envelope* mbr = 0;

  if(*boxStr != '\0')
    mbr = GetEnvelope(boxStr);

  PQclear(result);
  return mbr;
}

void te::pgis::CatalogLoader::loadCatalog(const bool full)
{
  te::da::DataSourceCatalog* catalog = m_t->getPGDataSource()->getCatalog();

  catalog->clear();

// get the list of tables in the database and create a DataSetType for each table or view
  std::auto_ptr<te::da::DataSet> tables(getDataSetTypes());

  while(tables->moveNext())
  {
    unsigned int dtid = tables->getInt32(0);

    std::string name(tables->getString(1) + "." + tables->getString(2));

    te::da::DataSetTypePtr dt(new te::da::DataSetType(name, dtid));
    dt->setTitle(name);
    dt->setFullLoaded(full);

    catalog->add(dt);

// now, let's load properties information for the table/view
    getProperties(dt.get());
  }

  if(!full)
    return;

// for each table in the catalog, finish loading its information: pk, fk, uk, indexes (note: !!this will avoid the need to load tables in the right order to insert into the catalog!!)
  std::size_t ndsets = catalog->getNumberOfDataSets();

  for(std::size_t i = 0; i < ndsets; ++i)
  {
    const te::da::DataSetTypePtr& dt = catalog->getDataSetType(i);

    getConstraints(dt.get());
    getIndexes(dt.get());
  }

// and finally... let's load all sequences in the database
  loadSequences();
}

bool te::pgis::CatalogLoader::hasDataSets()
{
  std::string sql("SELECT COUNT (pg_class.oid) "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relname !~ '^pg_' "
                  "AND pg_class.relname NOT IN ('spatial_ref_sys', 'geometry_columns', 'geography_columns', 'raster_columns', 'raster_overviews') "
                  "AND pg_class.relkind in ('r','v') "
                  "AND pg_class.relnamespace = pg_namespace.oid "
                  "AND pg_namespace.nspname NOT IN ('information_schema', 'pg_toast', 'pg_temp_1', 'pg_catalog', 'topology')");

  std::auto_ptr<te::da::DataSet> datasets(m_t->query(sql));

  if(!datasets->moveNext())
    return false;

  return datasets->getInt64(0) != 0;
}

bool te::pgis::CatalogLoader::datasetExists(const std::string& name)
{
  std::string tname, sname;
  
  SplitTableName(name, m_t->getPGDataSource()->getCurrentSchema(), sname, tname);

  std::string sql("SELECT pg_class.oid "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relnamespace = pg_namespace.oid " 
                  "AND lower(pg_class.relname) = '");

  sql += te::common::Convert2LCase(tname);
  sql += "' AND lower(pg_namespace.nspname) = '";
  sql += te::common::Convert2LCase(sname);
  sql += "'";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext())
    return true;
  else
    return false;
}

bool te::pgis::CatalogLoader::primarykeyExists(const std::string& name)
{
  return constraintExists(name, 'p');
}

bool te::pgis::CatalogLoader::uniquekeyExists(const std::string& name)
{
  return constraintExists(name, 'u');
}

bool te::pgis::CatalogLoader::foreignkeyExists(const std::string& name)
{
  return constraintExists(name, 'f');
}

bool te::pgis::CatalogLoader::checkConstraintExists(const std::string& name)
{
  return constraintExists(name, 'c');
}

bool te::pgis::CatalogLoader::indexExists(const std::string& name)
{
  std::string tname, sname;
  
  SplitTableName(name, m_t->getPGDataSource()->getCurrentSchema(), sname, tname);

  std::string sql("SELECT pg_class.oid "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relnamespace = pg_namespace.oid " 
                  "AND lower(pg_class.relname) = '");

  sql += te::common::Convert2LCase(tname);
  sql += "' AND lower(pg_namespace.nspname) = '";
  sql += te::common::Convert2LCase(sname);
  sql += "'";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext())
    return true;
  else
    return false;
}

bool te::pgis::CatalogLoader::sequenceExists(const std::string& name)
{
  std::string tname, sname;
  
  SplitTableName(name, m_t->getPGDataSource()->getCurrentSchema(), sname, tname);

  std::string sql("SELECT pg_class.oid "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relnamespace = pg_namespace.oid " 
                  "AND lower(pg_class.relname) = '");

  sql += te::common::Convert2LCase(tname);
  sql += "' AND lower(pg_namespace.nspname) = '";
  sql += te::common::Convert2LCase(sname);
  sql += "'";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext())
    return true;
  else
    return false;
}

te::da::DataSourceTransactor* te::pgis::CatalogLoader::getTransactor() const
{
  return m_t;
}

te::da::DataSet* te::pgis::CatalogLoader::getDataSetTypes()
{
  std::string sql("SELECT pg_class.oid, pg_namespace.nspname, pg_class.relname, pg_class.relkind "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relname !~ '^pg_' "
                  "AND pg_class.relname NOT IN ('spatial_ref_sys', 'geometry_columns', 'geography_columns', 'raster_columns', 'raster_overviews') "
                  "AND pg_class.relkind in ('r','v') "
                  "AND pg_class.relnamespace = pg_namespace.oid "
                  "AND pg_namespace.nspname NOT IN ('information_schema', 'pg_toast', 'pg_temp_1', 'pg_catalog', 'topology')");

  return m_t->query(sql);
}

unsigned int te::pgis::CatalogLoader::getTableId(const std::string& tableName)
{
  std::string tname, sname;
  
  SplitTableName(tableName, m_t->getPGDataSource()->getCurrentSchema(), sname, tname);

  std::string sql("SELECT pg_class.oid "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relnamespace = pg_namespace.oid " 
                  "AND lower(pg_class.relname) = '");

  sql += te::common::Convert2LCase(tname);
  sql += "' AND lower(pg_namespace.nspname) = '";
  sql += te::common::Convert2LCase(sname);
  sql += "'";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext() == false)
    throw Exception(TR_PGIS("Could not find table oid!"));

  unsigned int tableid = result->getInt32(0);

  return tableid;
}

std::string te::pgis::CatalogLoader::getTableName(unsigned int id)
{
  std::string sql("SELECT pg_namespace.nspname, pg_class.relname "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relnamespace = pg_namespace.oid " 
                  "AND pg_class.oid = ");

  sql += te::common::Convert2String(id);

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext() == false)
    throw Exception(TR_PGIS("Could not find table name!"));

  std::string tname  = result->getString(0);
              tname += ".";
              tname += result->getString(1);

  return tname;
}

void te::pgis::CatalogLoader::getPropertyId(te::dt::Property* p)
{
  if((p->getParent()==0) || (p->getParent()->getType() != te::dt::DATASET_TYPE))
    throw Exception(TR_PGIS("The informed property is not valid!"));

  std::string sql("SELECT attnum "
                  "FROM pg_attribute WHERE attrelid = ");
              sql += te::common::Convert2String(p->getParent()->getId());
              sql += " AND attisdropped = false "
                     "AND attname = '";
              sql += te::common::Convert2LCase(p->getName());
              sql += "' ";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext() == false)
    throw Exception(TR_PGIS("Could not find property ID!"));
  
  unsigned int id = result->getInt32(0);
  p->setId(id);
}

te::dt::Property* te::pgis::CatalogLoader::getProperty(unsigned int dtid, unsigned int pid)
{
  std::string sql("SELECT a.attnum, a.attname, t.oid, a.attnotnull, format_type(a.atttypid, a.atttypmod), a.atthasdef, pg_get_expr(d.adbin, d.adrelid), a.attndims "
                  "FROM pg_attribute AS a INNER JOIN pg_type AS t ON (a.atttypid = t.oid) LEFT JOIN pg_attrdef AS d ON (a.attrelid = d.adrelid AND a.attnum = d.adnum) "
                  "WHERE a.attrelid = ");
              sql += te::common::Convert2String(dtid);
              sql += " AND a.attnum = ";
              sql += te::common::Convert2String(pid);
              sql += " AND a.attisdropped = false"
                     " AND a.attnum > 0";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  std::auto_ptr<te::dt::Property> p(0);

  if(result->moveNext())
  {
    unsigned int attNum = result->getInt16(0);
    std::string attName = result->getString(1);
    unsigned int attType = result->getInt32(2);
    bool attNotNull = result->getBool(3);
    std::string fmt = result->getString(4);
    bool attHasDefault = result->getBool(5);
    std::string attDefValue = result->getString(6);
    int ndims = result->getInt32(7);

    p.reset(Convert2TerraLib(attNum, attName.c_str(), attType, attNotNull, fmt.c_str(), attHasDefault, attDefValue.c_str(), ndims, m_t->getPGDataSource()->getGeomTypeId()));

    if(p->getType() == te::dt::GEOMETRY_TYPE)
    {
      std::string datasetName = getTableName(dtid);

      getGeometryInfo(datasetName, static_cast<te::gm::GeometryProperty*>(p.get()));
    }
  }

  return p.release();
}

te::da::DataSet* te::pgis::CatalogLoader::getProperties(unsigned int dtid)
{
  std::string sql("SELECT a.attnum, a.attname, t.oid, a.attnotnull, format_type(a.atttypid, a.atttypmod), a.atthasdef, pg_get_expr(d.adbin, d.adrelid), a.attndims "
                  "FROM pg_attribute AS a INNER JOIN pg_type AS t ON (a.atttypid = t.oid) LEFT JOIN pg_attrdef AS d ON (a.attrelid = d.adrelid AND a.attnum = d.adnum) "
                  "WHERE a.attrelid = ");
              sql += te::common::Convert2String(dtid);
              sql += " AND a.attisdropped = false"
                     " AND a.attnum > 0"
                     " ORDER BY a.attnum";

  return m_t->query(sql);
}



void te::pgis::CatalogLoader::getGeometryInfo(const std::string& datasetName, te::gm::GeometryProperty* gp)
{
  std::string sql = "SELECT g.coord_dimension, g.srid, g.type "
                    "FROM geometry_columns g "
                    "WHERE g.f_table_name = '";

  std::string tname, sname;
  
  SplitTableName(datasetName, m_t->getPGDataSource()->getCurrentSchema(), sname, tname);

  sql += tname;
  sql += "' AND g.f_table_schema = '";
  sql += sname;
  sql += "' AND f_geometry_column = '";
  sql += gp->getName();
  sql += "'";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext())
  {
    //int cdim = result->getInt(0);
    int srid = result->getInt32(1);
    te::gm::GeomType t = te::gm::Geometry::getGeomTypeId(result->getString(2));
    //gp->setCoordDimension(cdim);
    gp->setSRID(srid);
    gp->setGeometryType(t);
  }
  else
  {
// Don't throw: someone can create a geometry column without using AddGeometryColumn function!!
    gp->setSRID(-1);
    gp->setGeometryType(te::gm::GeometryType);
  }
}

void te::pgis::CatalogLoader::getRasterInfo(const std::string& datasetName, te::rst::RasterProperty* rp)
{
  std::string sql = "SELECT * FROM raster_columns as r WHERE r.r_table_name = '";

  std::string tname, sname;

  SplitTableName(datasetName, m_t->getPGDataSource()->getCurrentSchema(), sname, tname);

  sql += tname;
  sql += "' AND r.r_table_schema = '";
  sql += sname;
  sql += "' AND r_raster_column = '";
  sql += rp->getName();
  sql += "'";

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext())
  {
    int srid = result->getInt32("srid");

    double scale_x = result->getDouble("scale_x");

    double scale_y = result->getDouble("scale_y");

    int blocksize_x = result->getInt32("blocksize_x");

    int blocksize_y = result->getInt32("blocksize_y");

    //bool regular_blocking = result->getBool("regular_blocking");

    int nbands = result->getInt32("num_bands");

    std::unique_ptr<te::dt::Array> pixel_types(result->getArray("pixel_types"));

    std::unique_ptr<te::dt::Array> nodata_values(result->getArray("nodata_values"));

    std::unique_ptr<te::gm::Geometry> g(result->getGeometry("extent"));

    const te::gm::Envelope* e = g->getMBR();

    std::unique_ptr<te::rst::Grid> grid(new te::rst::Grid(scale_x, scale_y, new te::gm::Envelope(*e), srid));

    rp->set(grid.release());

    for(int i = 0; i != nbands; ++i)
    {
      std::vector<std::size_t> pos(1, i);

      std::string st = pixel_types->getData(pos)->toString();

      int t = te::dt::UNKNOWN_TYPE;

      if(st == "8BI")
        t = te::dt::CHAR_TYPE;
      else if(st == "8BUI")
        t = te::dt::UCHAR_TYPE;
      else if(st == "16BI")
        t = te::dt::INT16_TYPE;
      else if(st == "16BUI")
        t = te::dt::UINT16_TYPE;
      else if(st == "32BI")
        t = te::dt::INT32_TYPE;
      else if(st == "32BUI")
        t = te::dt::UINT32_TYPE;
      else if(st == "32BF")
        t = te::dt::FLOAT_TYPE;
      else if(st == "64BF")
        t = te::dt::DOUBLE_TYPE;
      else
        throw Exception(TR_PGIS("Band data type not supported by PostGIS driver!"));

      te::rst::BandProperty* bp = new te::rst::BandProperty(i, t);

      bp->m_blkh = blocksize_y;

      bp->m_blkw = blocksize_x;

      te::dt::AbstractData* ab = nodata_values->getData(pos);

      if(ab)
        bp->m_noDataValue = static_cast<te::dt::Double*>(ab)->getValue();

      rp->add(bp);
    }
  }
  else
  {
    //throw Exception(TR_PGIS("We must add support for rasters that don't have constraints!"));
  }
}

te::da::DataSet* te::pgis::CatalogLoader::getConstraints(unsigned int dtid, char conType)
{
  std::string sql("SELECT c.oid, n.nspname, c.conname, c.contype, c.confrelid, c.confupdtype, c.confdeltype, c.confmatchtype, c.conkey, c.confkey, pg_get_constraintdef(c.oid) "
                  "FROM pg_constraint c, pg_namespace n "
                  "WHERE c.connamespace = n.oid "
                  "AND c.conrelid = ");
              sql += te::common::Convert2String(dtid);

  if(conType != '\0')
  {
    sql += " AND c.contype = '";
    sql += conType;
    sql += "'";
  }

  return m_t->query(sql);
}

void te::pgis::CatalogLoader::getConstraints(te::da::DataSetType* dt)
{
  std::auto_ptr<te::da::DataSet> result(getConstraints(dt->getId()));

  while(result->moveNext())
  {
    char contype = result->getChar(3);

    switch(contype)
    {
      case 'c':
        addCheckConstraint(result.get(), dt);
        break;

      case 'f': 
        addForeignKey(result.get(), dt);
        break;

      case 'p': 
        addPrimaryKey(result.get(), dt);
        break;

      case 'u':
        addUniqueKey(result.get(), dt);
        break;

      //default:
      //  throw Exception(TR_PGIS("Unexpected constraint type!"));
    }    
  }
}

te::da::CheckConstraint* te::pgis::CatalogLoader::addCheckConstraint(te::da::DataSet* ccInfo, te::da::DataSetType* dt)
{
  unsigned int id = ccInfo->getInt32(0);

  std::string ccName  = ccInfo->getString(2);

  te::da::CheckConstraint* cc = new te::da::CheckConstraint(ccName, dt, id);

  cc->setExpression(ccInfo->getString(10));

  return cc;
}

te::da::PrimaryKey* te::pgis::CatalogLoader::addPrimaryKey(te::da::DataSet* pkInfo, te::da::DataSetType* dt)
{
  unsigned int id = pkInfo->getInt32(0);

  std::string pkName  = pkInfo->getString(2);

  std::vector<boost::int16_t> pkCols;
  pkInfo->getArray(8, pkCols);

  std::auto_ptr<te::da::PrimaryKey> pk(new te::da::PrimaryKey(pkName, 0, id));

  std::size_t size = pkCols.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    te::dt::Property* p = dt->getPropertyById(pkCols[i]);

    if(p == 0) // property not found in dataset type list
      return 0;

    pk->add(p);
  }

  dt->setPrimaryKey(pk.release());

// try to link the pk to an index
  std::size_t nIdxs = dt->getNumberOfIndexes();

  for(std::size_t i = 0; i < nIdxs; ++i)
    if(dt->getPrimaryKey()->getName() == dt->getIndex(i)->getName())
    {
      dt->getPrimaryKey()->setAssociatedIndex(dt->getIndex(i));
      break;
    }

  return dt->getPrimaryKey();
}

te::da::UniqueKey* te::pgis::CatalogLoader::addUniqueKey(te::da::DataSet* ukInfo, te::da::DataSetType* dt)
{
  unsigned int id = ukInfo->getInt32(0);

  std::string ukName  = ukInfo->getString(2);

  std::vector<boost::int16_t> ukCols;
  ukInfo->getArray(8, ukCols);

  std::auto_ptr<te::da::UniqueKey> uk(new te::da::UniqueKey(ukName, 0, id));

  std::size_t size = ukCols.size();

  for( std::size_t i = 0; i < size; ++i)
  {
    te::dt::Property* p = dt->getPropertyById(ukCols[i]);

    if(p == 0) // property not found
      return 0;

    uk->add(p);
  }

// try to link the pk to an index
  std::size_t nIdxs = dt->getNumberOfIndexes();

  for(std::size_t i = 0; i < nIdxs; ++i)
    if(uk->getName() == dt->getIndex(i)->getName())
    {
      uk->setAssociatedIndex(dt->getIndex(i));
      break;
    }

  te::da::UniqueKey* ukAux = uk.release();
  dt->add(ukAux);

  return ukAux;
}

te::da::DataSet* te::pgis::CatalogLoader::getFKs(te::da::DataSetType* dt)
{
  std::string sql("SELECT fs.nspname, c.conname, s.nspname, t.relname "
                  "FROM pg_constraint c, pg_class t, pg_namespace s, pg_namespace fs "
                  "WHERE t.oid = c.confrelid "
                  "AND t.relnamespace = s.oid "
                  "AND c.connamespace = fs.oid "
                  "AND c.contype = 'f' "
                  "AND c.conrelid = ");
              sql += te::common::Convert2String(dt->getId());

  return m_t->query(sql);
}

te::da::DataSet* te::pgis::CatalogLoader::getFK(const std::string& fk, te::da::DataSetType* dt, te::da::DataSetType* rdt)
{
  std::string fkname, sname;
  
  SplitTableName(fk, m_t->getPGDataSource()->getCurrentSchema(), sname, fkname);

  std::string sql("SELECT c.oid, c.confupdtype, c.confdeltype, c.confmatchtype, c.conkey, c.confkey "
                  "FROM pg_constraint c, pg_namespace s "
                  "WHERE c.conrelid = ");
              sql += te::common::Convert2String(dt->getId());
              sql += " AND c.confrelid = ";
              sql += te::common::Convert2String(rdt->getId());
              sql += " AND c.contype = 'f'"
                     " AND c.conname = '";
              sql += fkname;
              sql += "' AND c.connamespace = s.oid ";
              sql += "AND s.nspname = '";
              sql += sname;
              sql += "'";

  return m_t->query(sql);
}

te::da::ForeignKey* te::pgis::CatalogLoader::addForeignKey(te::da::DataSet* fkInfo, te::da::DataSetType* dt)
{
  if(dt->getCatalog() == 0)
    return 0;

  unsigned int id = fkInfo->getInt32(0);
  unsigned int refFTId = fkInfo->getInt32(4);
  char onUpdate = fkInfo->getChar(5);
  char onDeletion = fkInfo->getChar(6);
  std::vector<boost::int16_t> fkCols;
  std::vector<boost::int16_t> fkRefCols;
  fkInfo->getArray(8, fkCols);
  fkInfo->getArray(9, fkRefCols);

  assert(fkCols.size() == fkRefCols.size());

  std::string refName = getTableName(refFTId);

  const te::da::DataSetTypePtr& refFT = dt->getCatalog()->getDataSetType(refName);

  if(refFT.get() == 0)
    return 0;

  std::string fkName  = fkInfo->getString(1);
              fkName += ".";
              fkName += fkInfo->getString(2);

  te::da::ForeignKey* fk = new te::da::ForeignKey(fkName, id);
  fk->setOnUpdateAction(GetAction(onUpdate));
  fk->setOnDeleteAction(GetAction(onDeletion));
  fk->setReferencedDataSetType(refFT.get());
  
  std::size_t size = fkCols.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    fk->addRefProperty(refFT->getPropertyById(fkRefCols[i]));
    fk->add(dt->getPropertyById(fkCols[i]));
  }

  dt->add(fk);

  return fk;
}

te::da::DataSet* te::pgis::CatalogLoader::getIndexes(unsigned int dtOid, const std::string& indexName)
{
  std::string sql("SELECT idx_table.oid, s.nspname, idx_table.relname, pg_index.indkey, pg_am.amname, pg_index.indisunique, pg_index.indisprimary " 
                  "FROM pg_index, pg_class idx_table, pg_am, pg_namespace s "
                  "WHERE s.oid = idx_table.relnamespace "
                  "AND pg_index.indexrelid = idx_table.oid "
                  "AND idx_table.relam = pg_am.oid "
                  "AND pg_index.indrelid = ");
              sql += te::common::Convert2String(dtOid);

  if(indexName.empty() == false)
  {
    sql += " AND idx_table.relname = '";
    sql += indexName;
    sql += "'";
  }

  return m_t->query(sql);
}

te::da::Index* te::pgis::CatalogLoader::addIndex(te::da::DataSet* idxInfo, te::da::DataSetType* dt)
{
  unsigned int id = idxInfo->getInt32(0);

  std::string name  = idxInfo->getString(1);
              name += ".";
              name += idxInfo->getString(2);

  std::vector<boost::int16_t> idxCols;
  idxInfo->getArray(3, idxCols);
  std::string idxType = idxInfo->getString(4);
  bool isUK = idxInfo->getBool(5);
  bool isPK = idxInfo->getBool(6);

  te::da::Index* idx = new te::da::Index(name, GetIndexType(idxType.c_str()), dt, id);

  std::size_t size = idxCols.size();

  for(std::size_t i = 0; i < size; ++i)
    idx->add(dt->getPropertyById(idxCols[i]));

// look if there is an association with a pk and uk
  name = idxInfo->getString(2);
  if(isPK && dt->getPrimaryKey() && (dt->getPrimaryKey()->getName() == name))
  {
    dt->getPrimaryKey()->setAssociatedIndex(idx);
  }
  else if(isUK)
  {
    te::da::UniqueKey* uk = dt->getUniqueKey(name);

    if(uk)
      uk->setAssociatedIndex(idx);
  }

  return idx;
}

te::da::DataSet* te::pgis::CatalogLoader::getSequences()
{
  std::string sql("SELECT c.oid, n.nspname, c.relname, c.relkind "
                  "FROM pg_class c, pg_namespace n "
                  "WHERE c.relname !~ '^pg_' "
                  "AND c.relkind = 'S' "
                  "AND c.relnamespace = n.oid "
                  "AND n.nspname NOT IN ('information_schema', 'pg_toast', 'pg_temp_1', 'pg_catalog')");

  return m_t->query(sql);
}

void te::pgis::CatalogLoader::loadSequences()
{
  te::da::DataSourceCatalog* catalog = m_t->getDataSource()->getCatalog();
  //catalog->clearSequences();

  std::auto_ptr<te::da::DataSet> seqs(getSequences());
 
  while(seqs->moveNext())
  {
    std::string sname(seqs->getString(1) + "." + seqs->getString(2));
    te::da::Sequence* seq = getSequence(sname);
    catalog->add(seq);
  }
}

bool te::pgis::CatalogLoader::constraintExists(const std::string& name, char conType)
{
  std::string cname, sname;
  
  SplitTableName(name, m_t->getPGDataSource()->getCurrentSchema(), sname, cname);

  std::string sql("SELECT c.oid, n.nspname, c.conname, c.contype "
                  "FROM pg_constraint c, pg_namespace n "
                  "WHERE c.connamespace = n.oid "
                  "AND c.conname = '");
              sql += cname;
              sql += "' AND n.nspname = '";
              sql += sname;
              sql += "'";

  if(conType != '\0')
  {
    sql += " AND c.contype = '";
    sql += conType;
    sql += "'";
  }

  std::auto_ptr<te::da::DataSet> result(m_t->query(sql));

  if(result->moveNext())
    return true;
  else
    return false;
}

