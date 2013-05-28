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
  \file terralib/sqlite/DataSetTypePersistence.cpp

  \brief DataSetTypePersistence implementation for SQLite.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../datatype/Enums.h"
#include "../datatype/SimpleProperty.h"
#include "../geometry/GeometryProperty.h"
#include "DataSetTypePersistence.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <memory>
#include <iostream>

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

te::sqlite::DataSetTypePersistence::DataSetTypePersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::sqlite::DataSetTypePersistence::~DataSetTypePersistence()
{
}

void te::sqlite::DataSetTypePersistence::create(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  const std::size_t nproperties = dt->size();

  if(nproperties == 0)
    throw Exception(TR_SQLITE("SQLite doesn't support empty table definition!"));

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

// we need to add compound primary keys
  if(dt->getPrimaryKey() && (dt->getPrimaryKey()->getProperties().size() > 1))
  {
    sql += ", PRIMARY KEY(";

    const std::size_t nkeys = dt->getPrimaryKey()->getProperties().size();

    for(std::size_t i = 0; i < nkeys; ++i)
    {
      if(i != 0)
        sql += ", ";

      sql += dt->getPrimaryKey()->getProperties()[i]->getName();
    }

    sql += ")";
  }

// we need to add fks during table creation!
  std::size_t nfks = dt->getNumberOfForeignKeys();

  for(std::size_t i = 0; i < nfks; ++i)
  {
    te::da::ForeignKey* fk = dt->getForeignKey(i);

    fk->setName(dt->getName() + "." + boost::lexical_cast<std::string>(i));

    if(i != 0)
      sql += ", ";

    sql += "FOREIGN KEY(";

    const std::size_t nkeys = fk->getProperties().size();

    for(std::size_t j = 0; j < nkeys; ++j)
    {
      if(j != 0)
        sql += ", ";

      sql += fk->getProperties()[j]->getName();
    }

    sql += ") REFERENCES ";

    sql += fk->getReferencedDataSetType()->getName();

    sql += "(";

    for(std::size_t j = 0; j < nkeys; ++j)
    {
      if(j != 0)
        sql += ", ";

      sql += fk->getReferencedProperties()[j]->getName();
    }

    sql += ")";
  }

  sql += ")";

  m_t->execute(sql);

// let's find table id
  std::auto_ptr<DataSourceCatalogLoader> cloader(m_t->getSQLiteCatalogLoader());

// register geometry metadata
  for(std::size_t i = 0; i < nproperties; ++i)
  {
    const te::dt::Property* p = dt->getProperty(i);

    if(p->getType() != te::dt::GEOMETRY_TYPE)
      continue;

    const te::gm::GeometryProperty* gp = static_cast<const te::gm::GeometryProperty*>(p);

    std::string gtype;
    std::string cdim;

    Convert2SpatiaLiteGeom(gp->getGeometryType(), gtype, cdim);

    sql  = "SELECT RecoverGeometryColumn('";
    sql += dt->getName();
    sql += "', '";
    sql += gp->getName();
    sql += "', ";
    sql += boost::lexical_cast<std::string>(gp->getSRID());
    sql += ", '";
    sql += gtype;
    sql += "', '";
    sql += cdim;
    sql += "')";

    m_t->execute(sql);
  }

// add unique keys
  std::size_t nukeys = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < nukeys; ++i)
    add(dt, dt->getUniqueKey(i));

// add indexes... just if no primary key or unique key with the same name exists!
  std::size_t nidxs = dt->getNumberOfIndexes();

  for(std::size_t i = 0; i < nidxs; ++i)
    add(dt, dt->getIndex(i), options);

//// add constraints
//  std::size_t nccs = dt->getNumberOfCheckConstraints();
//
//  for(std::size_t i = 0; i < nccs; ++i)
//    add(dt, dt->getCheckConstraint(i));
}

void te::sqlite::DataSetTypePersistence::clone(const std::string& datasetName, const std::string& newDatasetName, const std::map<std::string, std::string>& /*options*/)
{
  boost::format sql("CREATE TABLE %1% AS SELECT * FROM %2%");
  sql.bind_arg(1, newDatasetName);
  sql.bind_arg(2, datasetName);

  m_t->execute(sql.str());

  std::cout << "CLonada..." << std::endl;

// TODO: criar os indices e todos os modificadores necessarios
  boost::format pk("CREATE UNIQUE INDEX %1%_ukidx ON %1% (cell_id)");
  pk.bind_arg(1, newDatasetName);

  m_t->execute(pk.str());

  std::cout << "Indexada..." << std::endl;
}

void te::sqlite::DataSetTypePersistence::drop(te::da::DataSetType* dt)
{
  if(dt->getCategory() == te::da::VIEW_TYPE)
  {
    if(dt->hasGeom())
    {
      std::string sql  = "DELETE FROM views_geometry_columns WHERE view_name = '";
                  sql += dt->getName();
                  sql += "'";

      m_t->execute(sql);
    }

    std::string sql  = "DROP VIEW ";
                sql += dt->getName();

    m_t->execute(sql);
  }
  else  // it is a table
  {
// if table has geometric properties we must remove its metadata from the database
    if(dt->hasGeom())
    {
      const std::size_t nproperties = dt->size();

      for(std::size_t i = 0; i < nproperties; ++i)
      {
        if(dt->getProperty(i)->getType() != te::dt::GEOMETRY_TYPE)
          continue;

        std::string sql  = "SELECT DisableSpatialIndex('";
                    sql += dt->getName();
                    sql += "', '";
                    sql += dt->getProperty(i)->getName();
                    sql += "')";

        m_t->execute(sql);

        sql  = "SELECT DiscardGeometryColumn('";
        sql += dt->getName();
        sql += "', '";
        sql += dt->getProperty(i)->getName();
        sql += "')";

        m_t->execute(sql);
      }
    }

    std::string sql  = "DROP TABLE ";
                sql += dt->getName();

    m_t->execute(sql);
  }
 
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

void te::sqlite::DataSetTypePersistence::drop(const std::string& /*datasetName*/)
{
  throw Exception(TR_SQLITE("Not implmented yet!"));
}

void te::sqlite::DataSetTypePersistence::rename(te::da::DataSetType* dt, const std::string& newName)
{
  std::string oldName = dt->getName();

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " RENAME TO ";
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

// if table has a geometry column, we need to propagate changes to geometry columns table
  if(dt->hasGeom())
  {
    sql  = "UPDATE geometry_columns SET f_table_name = '";
    sql += newName;
    sql += "' WHERE f_table_name = '";
    sql += oldName;
    sql += "'";

    m_t->execute(sql);  
  }

// rename primary key
  if(dt->getPrimaryKey() != 0)
    dt->getPrimaryKey()->setName(newName);

// rename uk
  {
    const std::size_t nuks = dt->getNumberOfUniqueKeys();

    for(std::size_t i = 0; i < nuks; ++i)
    {
      te::da::UniqueKey* uk = dt->getUniqueKey(i);

      std::size_t pos = uk->getName().find(".");

      if(pos != std::string::npos)
        uk->setName(newName + "." + uk->getName().substr(pos + 1));
      else
        uk->setName(newName + "." + uk->getName());
    }
  }

// rename indexes
  {
    const std::size_t nidxs = dt->getNumberOfIndexes();

    for(std::size_t i = 0; i < nidxs; ++i)
    {
      te::da::Index* idx = dt->getIndex(i);

      std::size_t pos = idx->getName().find(".");

      if(pos != std::string::npos)
        idx->setName(newName + "." + idx->getName().substr(pos + 1));
      else
        idx->setName(newName + "." + idx->getName());
    }
  }
}

void te::sqlite::DataSetTypePersistence::add(te::da::DataSetType* dt, te::dt::Property* p)
{
  std::string sql;

  if(p->getType() == te::dt::GEOMETRY_TYPE)
  {
    te::gm::GeometryProperty* gp = static_cast<te::gm::GeometryProperty*>(p);

    std::string gtype;
    std::string cdim;

    Convert2SpatiaLiteGeom(gp->getGeometryType(), gtype, cdim);

    sql  = "SELECT AddGeometryColumn('";

    sql += te::common::Convert2LCase(dt->getName());
    sql += "', '";
    sql += te::common::Convert2LCase(p->getName());
    sql += "', ";
    sql += boost::lexical_cast<std::string>(gp->getSRID());
    sql += ", '";
    sql += gtype;
    sql += "', '";
    sql += cdim;

    if(gp->isRequired())
      sql += "', 1)";
    else
      sql += "')";
  }
  else
  {
    sql = "ALTER TABLE ";
    sql += dt->getName();
    sql += " ADD COLUMN ";
    sql += p->getName();
    sql += " ";
    sql += GetSQLType(p);
  }

  m_t->execute(sql); 
  
// add property to dataset type
  if(p->getParent() == 0)
    dt->add(p);

// get property id in the database
  p->setId(dt->size() - 1);
}

void te::sqlite::DataSetTypePersistence::drop(te::dt::Property* /*p*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

void te::sqlite::DataSetTypePersistence::rename(te::dt::Property* /*p*/, const std::string& /*newName*/) 
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

void te::sqlite::DataSetTypePersistence::update(te::dt::Property* /*oldP*/, te::dt::Property* /*newP*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

void te::sqlite::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::PrimaryKey* /*pk*/) 
{
  throw Exception(TR_SQLITE("SQLite doesn't support to alter a table to add a primary key. This must be done during table creation!"));
}

void te::sqlite::DataSetTypePersistence::drop(te::da::PrimaryKey* /*pk*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

void te::sqlite::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::UniqueKey* uk) 
{
// rename uk if needed in order to preserve table name
  std::string ukname = uk->getName();

  std::size_t pos = ukname.find(".");

  if(pos != std::string::npos)
    ukname = ukname.substr(pos + 1);
  else
    uk->setName(dt->getName() + "." + uk->getName());

  std::string sql =  "CREATE UNIQUE INDEX IF NOT EXISTS ";
              sql += ukname;
              sql += " ON ";
              sql += dt->getName();
              sql += "(";
              
  std::size_t size = uk->getProperties().size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += uk->getProperties()[i]->getName();
  }

  sql += ")";

  m_t->execute(sql);

  if(uk->getDataSetType() == 0)
    dt->add(uk);
}

void te::sqlite::DataSetTypePersistence::drop(te::da::UniqueKey* uk)
{
  std::string ukname = uk->getName();

  std::size_t pos = ukname.find(".");

  if(pos != std::string::npos)
    ukname = ukname.substr(pos + 1);

  std::string sql = "DROP INDEX IF EXISTS ";
              sql += ukname;

  m_t->execute(sql);

  if(uk->getDataSetType() != 0)
  {
    te::da::DataSetType* dt = uk->getDataSetType();
    dt->remove(uk);
  }
  else
  {
    delete uk;
  }
}

void te::sqlite::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::Index* index, const std::map<std::string, std::string>& /*options*/) 
{
// find out if it is a unique index or not
  if(dt->getPrimaryKey() && (dt->getPrimaryKey()->getAssociatedIndex() == index))
    return;

  std::size_t size = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < size; ++i)
    if(dt->getUniqueKey(i)->getAssociatedIndex() == index)
      return;

// rename index if needed in order to preserve table name
  std::string idxname = index->getName();

  std::size_t pos = idxname.find(".");

  if(pos != std::string::npos)
    idxname = idxname.substr(pos + 1);
  else
    index->setName(dt->getName() + "." + idxname);

  std::string sql;

  if(index->getIndexType() == te::da::B_TREE_TYPE)
  {
    sql  = "CREATE ";

    //if(isunique)
    //  sql += "UNIQUE ";
    
    sql += "INDEX IF NOT EXISTS ";
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
  }
  else if(index->getIndexType() == te::da::R_TREE_TYPE)
  {
    if(index->getProperties().size() != 1)
      throw Exception(TR_SQLITE("In order to create a spatial index you must inform just one spatial column in index definition!"));

    sql  = "SELECT CreateSpatialIndex('";
    sql += dt->getName();
    sql += "', '";
    sql += index->getProperties()[0]->getName();
    sql += "')";

    index->setName(dt->getName() + ".idx_" + dt->getName() + "_" + index->getProperties()[0]->getName());
  }

  m_t->execute(sql);

// add index to dataset type if it was not yet!
  if(index->getDataSetType() == 0)
    dt->add(index);  
}

void te::sqlite::DataSetTypePersistence::drop(te::da::Index* index)
{
  assert(index->getDataSetType());

// extract just index name
  std::string idxname = index->getName();

  std::size_t pos = idxname.find(".");

  if(pos != std::string::npos)
    idxname = idxname.substr(pos + 1);

  te::da::DataSetType* dt = index->getDataSetType();

  std::string sql;

  if(index->getIndexType() == te::da::R_TREE_TYPE)
  {
// first we need to disable the index
    sql  = "SELECT DisableSpatialIndex('";
    sql += dt->getName();
    sql += "', '";
    sql += index->getProperties()[0]->getName();
    sql += "')";

    m_t->execute(sql);

// then we can drop it... but r-tree is a virtual table!
    sql  = "DROP TABLE ";
    sql += idxname;
  }
  else
  {
    sql  = "DROP INDEX ";
    sql += idxname;
  }

  m_t->execute(sql);

  dt->remove(index);
}

void te::sqlite::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::ForeignKey* /*fk*/)
{
  throw Exception(TR_SQLITE("SQLite doesn't support to alter a table to add a foreign key. This must be done during table creation!"));
}

void te::sqlite::DataSetTypePersistence::drop(te::da::ForeignKey* /*fk*/)
{
  throw Exception(TR_SQLITE("SQLite doesn't support to alter a table to drop a foreign key!"));
}

void te::sqlite::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::CheckConstraint* /*cc*/)
{
  throw Exception(TR_SQLITE("SQLite doesn't support to alter a table to add a check constraint!"));
}

void te::sqlite::DataSetTypePersistence::drop(te::da::CheckConstraint* /*cc*/)
{
  throw Exception(TR_SQLITE("SQLite doesn't support to alter a table to drop a check constraint!"));
}

void te::sqlite::DataSetTypePersistence::create(te::da::Sequence* /*sequence*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

void te::sqlite::DataSetTypePersistence::drop(te::da::Sequence* /*sequence*/)
{
  throw Exception(TR_SQLITE("Not supported by SQLite!"));
}

//te::da::DataSourceTransactor* te::sqlite::DataSetTypePersistence::getTransactor() const
//{
//  return m_t;
//}

