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
  \file terralib/postgis/DataSetTypePersistence.cpp

  \brief DataSetTypePersistence implementation for PostGIS driver.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/Sequence.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"
#include "CatalogLoader.h"
#include "DataSetTypePersistence.h"
#include "DataSource.h"
#include "Exception.h"
#include "Transactor.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/algorithm/string.hpp>

te::pgis::DataSetTypePersistence::DataSetTypePersistence(Transactor* t)
  : m_t(t)
{
  assert(m_t);
}

te::pgis::DataSetTypePersistence::~DataSetTypePersistence()
{
}

void te::pgis::DataSetTypePersistence::create(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  std::string sql  = "CREATE TABLE ";
              sql += dt->getName();
              sql += "()";

  m_t->execute(sql);

// let's find table oid!
  std::auto_ptr<CatalogLoader> loader(m_t->getPGCatalogLoader());

  unsigned int dtid = loader->getTableId(dt->getName());

  dt->setId(dtid);

// add the properties
  std::size_t ncols = dt->size();

  for(std::size_t i = 0; i < ncols; ++i)
    add(dt, dt->getProperty(i), false);

// add the primary key
  if(dt->getPrimaryKey())
    add(dt, dt->getPrimaryKey(), false);

// add unique keys
  std::size_t nukeys = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < nukeys; ++i)
    add(dt, dt->getUniqueKey(i), false);

// add indexes... just if no primary key or unique key with the same name exists!
  std::size_t nidxs = dt->getNumberOfIndexes();

  for(std::size_t i = 0; i < nidxs; ++i)
      add(dt, dt->getIndex(i), options);

// add fks
  std::size_t nfks = dt->getNumberOfForeignKeys();

  for(std::size_t i = 0; i < nfks; ++i)
    add(dt, dt->getForeignKey(i));

// add constraints
  std::size_t nccs = dt->getNumberOfCheckConstraints();

  for(std::size_t i = 0; i < nccs; ++i)
    add(dt, dt->getCheckConstraint(i));
  
  loader->getIndexes(dt);
  loader->getCheckConstraints(dt);
}

void te::pgis::DataSetTypePersistence::clone(const std::string& /*datasetName*/, const std::string& /*newDatasetName*/, const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSetTypePersistence::drop(const std::string& /*datasetName*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));

  //std::string sql;

  //if(dt->hasGeom())
  //{
  //  std::string tSchema, tName;
  //  SplitTableName(dt->getName(), m_t->getPGDataSource()->getCurrentSchema(), tSchema, tName);

  //  sql = "SELECT DropGeometryTable('";
  //  sql += te::common::Convert2LCase(tSchema);
  //  sql += "', '";
  //  sql += te::common::Convert2LCase(tName);
  //  sql += "')";
  //}
  //else
  //{
  //  sql += "DROP TABLE ";
  //  sql += dt->getName();
  //}

  //m_t->execute(sql);
 
  //if(dt->getCatalog())
  //{
  //  te::da::DataSourceCatalog* catalog = dt->getCatalog();
  //  catalog->remove(dt, true);
  //}
  //else
  //  delete dt;
}

void te::pgis::DataSetTypePersistence::rename(te::da::DataSetType* dt, const std::string& newName)
{
  std::string newTableName, newTableSchema, oldTableName, oldTableSchema;

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " RENAME TO ";

  SplitTableName(newName, m_t->getPGDataSource()->getCurrentSchema(), newTableSchema, newTableName);

  sql += newTableName;

  m_t->execute(sql);

// if table has a geometry column, we need to propagate changes to geometry columns table
  if(dt->hasGeom())
  {
    SplitTableName(dt->getName(), m_t->getPGDataSource()->getCurrentSchema(), oldTableSchema, oldTableName);

    sql  = "UPDATE geometry_columns SET f_table_name = '";
    sql += newTableName;
    sql += "' WHERE f_table_name = '";
    sql += oldTableName;
    sql += "' AND f_table_schema ='";
    sql += oldTableSchema;
    sql += "'";

    m_t->execute(sql);
  }
  
  if(dt->getCatalog())
  {
    te::da::DataSourceCatalog* catalog = dt->getCatalog();
    catalog->rename(dt, newName);
  }
  else
    dt->setName(newName);

  dt->setCompositeName(newName);
  
  //load constraints from data source 
  std::auto_ptr<CatalogLoader> loader(m_t->getPGCatalogLoader());
  loader->getCheckConstraints(dt);
}

void te::pgis::DataSetTypePersistence::add(te::da::DataSetType* dt, te::dt::Property* p)
{
  add(dt, p, true);
}

void te::pgis::DataSetTypePersistence::drop(te::dt::Property* p)
{
  drop(p, true);
}  

void te::pgis::DataSetTypePersistence::rename(te::dt::Property* p, const std::string& newName)
{  
  rename(p, newName, true); 
}

void te::pgis::DataSetTypePersistence::update(te::dt::Property* oldP, te::dt::Property* newP)
{
  update(oldP, newP, true);
}

void te::pgis::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::PrimaryKey* pk)
{
  add(dt, pk, true);  
}

void te::pgis::DataSetTypePersistence::drop(te::da::PrimaryKey* pk)
{
  drop(pk, true);
}

void te::pgis::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::UniqueKey* uk)
{
  add(dt, uk, true);
}

void te::pgis::DataSetTypePersistence::drop(te::da::UniqueKey* uk)
{
  drop(uk, true);
} 

void te::pgis::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::Index* index, const std::map<std::string, std::string>& /*options*/)
{
// let's see if the index is associated to a UK or PK
  if(dt->getPrimaryKey() && (dt->getPrimaryKey()->getAssociatedIndex() == index))
    return;

  std::size_t size = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < size; ++i)
    if(dt->getUniqueKey(i)->getAssociatedIndex() == index)
      return;

// if there is not a uk or pk associated, let's create the index!
  std::string sql("CREATE INDEX ");
              sql += index->getName();
              sql += " ON ";
              sql += dt->getName();
 
  if(index->getIndexType() == te::da::HASH_TYPE)
    sql += " USING HASH (";
  else if(index->getIndexType() == te::da::R_TREE_TYPE)
    sql += " USING GIST (";
  else if(index->getIndexType() == te::da::B_TREE_TYPE)
    sql += " USING BTREE (";
  else
    throw Exception(TR_PGIS("Index type not supported!"));

  size = index->getProperties().size();

  for(size_t i = 0; i < size; ++i)
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
}

void te::pgis::DataSetTypePersistence::drop(te::da::Index* index)
{
  te::da::DataSetType* dt = index->getDataSetType();

  std::string sql("DROP INDEX ");
              sql += index->getName();

  m_t->execute(sql);

  dt->remove(index);
}

void te::pgis::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::ForeignKey* fk)
{
  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " ADD CONSTRAINT ";
              sql += fk->getName();
              sql += " FOREIGN KEY (";
  
  std::size_t size = fk->getProperties().size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += fk->getProperties()[i]->getName();
  }

  sql += ") REFERENCES " + fk->getReferencedDataSetType()->getName() + " (";

  size = fk->getReferencedProperties().size();

  for(size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += fk->getReferencedProperties()[i]->getName();
  }

  sql += ") ON DELETE ";

  switch(fk->getOnDeleteAction())
  {
    case te::da::NO_ACTION:
      sql += " NO ACTION ";
      break;

    case te::da::RESTRICT:
      sql += " RESTRICT "; 
      break;

    case te::da::CASCADE:
      sql += " CASCADE "; 
      break;

    case te::da::SET_NULL: 
      sql += "SET NULL ";
      break;

    case te::da::SET_DEFAULT:
    default:
      sql += "SET DEFAULT ";
      break;
  }

  sql += " ON UPDATE ";

  switch(fk->getOnUpdateAction())
  {
    case te::da::NO_ACTION:
      sql += " NO ACTION ";
    break;

    case te::da::RESTRICT:
      sql += " RESTRICT "; 
    break;

    case te::da::CASCADE:
      sql += " CASCADE "; 
    break;

    case te::da::SET_NULL: 
      sql += "SET NULL ";
    break;

    case te::da::SET_DEFAULT:
    default:
      sql += "SET DEFAULT ";
    break;
  }

  m_t->execute(sql);

  if(fk->getDataSetType() == 0)
    dt->add(fk);
}

void te::pgis::DataSetTypePersistence::drop(te::da::ForeignKey* fk)
{
  te::da::DataSetType* dt = fk->getDataSetType();

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " DROP CONSTRAINT ";
              sql += fk->getName();

  m_t->execute(sql);

  dt->remove(fk);
}

void te::pgis::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::CheckConstraint* cc)
{
  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " ADD CONSTRAINT ";
              sql += cc->getName();
              sql += " CHECK(";
              sql += cc->getExpression();
              sql += ")";

  m_t->execute(sql);

  if(cc->getDataSetType() == 0)
    dt->add(cc);
}

void te::pgis::DataSetTypePersistence::drop(te::da::CheckConstraint* cc)
{
  te::da::DataSetType* dt = cc->getDataSetType();

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " DROP CONSTRAINT ";
              sql += cc->getName();

  m_t->execute(sql);
  
  dt->remove(cc);
}

void te::pgis::DataSetTypePersistence::create(te::da::Sequence* sequence)
{
  std::string sql("CREATE SEQUENCE ");
              sql += sequence->getName();
              sql += " INCREMENT BY ";
              sql += te::common::Convert2String(sequence->getIncrement());
              sql += " MINVALUE ";
              sql += te::common::Convert2String(sequence->getMinValue());
              sql += " MAXVALUE ";
              sql += te::common::Convert2String(sequence->getMaxValue());
              sql += " START WITH ";
              sql += te::common::Convert2String(sequence->getStartValue());
              sql += " CACHE ";
              sql += te::common::Convert2String(sequence->getCachedValues());
              
  if(sequence->isCycled() == false)
    sql += " NO";

  sql += " CYCLE ";

  if(sequence->getOwner())
  {
    sql += " OWNED BY ";
    sql += sequence->getOwner()->getParent()->getName();
    sql += ".";
    sql += sequence->getOwner()->getName();
  }

  m_t->execute(sql);

  std::auto_ptr<CatalogLoader> loader(m_t->getPGCatalogLoader());

  unsigned int id = loader->getTableId(sequence->getName());

  sequence->setId(id);
}

void te::pgis::DataSetTypePersistence::drop(te::da::Sequence* sequence)
{
  std::string sql("DROP SEQUENCE ");
              sql += sequence->getName();

  m_t->execute(sql);

  te::da::DataSourceCatalog* catalog = sequence->getCatalog();

  if(catalog)
    catalog->remove(sequence);
  else
    delete sequence;
}

te::da::DataSourceTransactor* te::pgis::DataSetTypePersistence::getTransactor() const
{
  return m_t;
}

void te::pgis::DataSetTypePersistence::add(te::da::DataSetType* dt, te::dt::Property* p, const bool refresh)
{
  std::string sql;
 
  if(p->getType() == te::dt::GEOMETRY_TYPE)
  {
    te::gm::GeometryProperty* gp = static_cast<te::gm::GeometryProperty*>(p);

    sql  = "SELECT AddGeometryColumn('";

// split schema-name.table-name
    std::size_t pos = dt->getName().find(".");

    if(pos == std::string::npos)
    {
      sql += te::common::Convert2LCase(dt->getName());
      sql += "', '";
    }
    else
    {
      sql += te::common::Convert2LCase(dt->getName().substr(0, pos));
      sql += "', '";
      sql += te::common::Convert2LCase(dt->getName().substr(pos + 1));
      sql += "', '";
    }

    sql += gp->getName();
    sql += "', ";
    sql += te::common::Convert2String(gp->getSRID());
    sql += ", '";
    sql += GetGeometryName(gp->getGeometryType() == te::gm::UnknownGeometryType ? te::gm::GeometryType : gp->getGeometryType());
    sql += "', ";
    sql += te::common::Convert2String(te::gm::GetCoordDimension(gp->getGeometryType()));
    sql += ")";
  }
  else
  {
    sql = "ALTER TABLE ";
    sql += dt->getName();
    sql += " ADD COLUMN ";
    sql += p->getName();
    sql += " ";

    SetColumnDef(sql, p);
  }

  m_t->execute(sql); 
  
  // add property to dataset type
  if(p->getParent() == 0)
    dt->add(p); 
    
  if(p->getType() == te::dt::GEOMETRY_TYPE)  
    dt->setDefaultGeomProperty(static_cast<te::gm::GeometryProperty*>(p));
  
  // get property id in the database
  std::auto_ptr<CatalogLoader> loader(m_t->getPGCatalogLoader());
  loader->getPropertyId(p);
  
  if(refresh)
  {  
    loader->getCheckConstraints(dt);
    loader->getIndexes(dt);
  }
}
             
void te::pgis::DataSetTypePersistence::drop(te::dt::Property* p, const bool refresh)
{
  if((p->getParent() == 0) || (p->getParent()->getType() != te::dt::DATASET_TYPE))
    throw Exception(TR_PGIS("The informed property is not valid!"));
  
  te::da::DataSetType* dt = static_cast<te::da::DataSetType*>(p->getParent());

  std::string sql;

  if(p->getType() == te::dt::GEOMETRY_TYPE)
  {
    sql  =  "SELECT DropGeometryColumn('";

// split schema-name.table-name if needed
    std::size_t pos = dt->getName().find(".");

    if(pos == std::string::npos)
    {
      sql += *(m_t->getPGDataSource()->getCurrentSchema());
      sql += "', '";
      sql += te::common::Convert2LCase(dt->getName());      
    }
    else
    {
      sql += te::common::Convert2LCase(dt->getName().substr(0, pos));    
      sql += "', '";
      sql += te::common::Convert2LCase(dt->getName().substr(pos + 1));
    }

    sql += "', '";
    sql += te::common::Convert2LCase(p->getName()) + "'";
    sql += ")";    
  }
  else
  {
    sql  = " ALTER TABLE ";
    sql += p->getParent()->getName();
    sql += " DROP COLUMN ";
    sql += p->getName();
  }

  m_t->execute(sql);

  dt->remove(p);

  if(refresh)
  {
    std::auto_ptr<CatalogLoader> loader(m_t->getPGCatalogLoader());
    loader->getPrimaryKey(dt);
    loader->getIndexes(dt);
    loader->getCheckConstraints(dt);
    loader->getUniqueKeys(dt);
  }
}
        
void te::pgis::DataSetTypePersistence::rename(te::dt::Property* p, const std::string& newName, const bool refresh)
{
  if((p->getParent()==0) || (p->getParent()->getType() != te::dt::DATASET_TYPE))
    throw Exception(TR_PGIS("The informed property is not valid!"));
    
  te::da::DataSetType* dt = static_cast<te::da::DataSetType*>(p->getParent());

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " RENAME COLUMN ";
              sql += p->getName() + " TO ";
              sql += newName;

  m_t->execute(sql);

  if(p->getType() == te::dt::GEOMETRY_TYPE)
  {
    sql = "UPDATE geometry_columns SET f_geometry_column = '" + newName + "'";
    sql += " WHERE f_table_name = '" ;

// split schema-name.table-name
    std::size_t pos = dt->getName().find(".");
    
    if(pos == std::string::npos)
    {
      sql += dt->getName();
      sql += "' AND f_table_schema ='";
      sql += *(m_t->getPGDataSource()->getCurrentSchema());
      sql += "'";
    }
    else
    {
      sql += dt->getName().substr(pos + 1);
      sql += "' AND f_table_schema ='";
      sql += dt->getName().substr(0, pos);
      sql += "'";
    }

    sql += " AND f_geometry_column = '" + p->getName() + "'";

    m_t->execute(sql);
  }

  p->setName(newName);

  if(refresh)
  {
    std::auto_ptr<CatalogLoader> loader(m_t->getPGCatalogLoader());
    loader->getCheckConstraints(dt);
  }  
}
        
void te::pgis::DataSetTypePersistence::update(te::dt::Property* oldP, te::dt::Property* newP, const bool refresh)
{
  if((oldP->getParent()==0) || (oldP->getParent()->getType() != te::dt::DATASET_TYPE))
    throw Exception(TR_PGIS("The informed property is not valid!"));

  if(oldP->getType() == te::dt::COMPOSITE_TYPE || 
     oldP->getType() == te::dt::DATASET_TYPE   || 
     oldP->getType() == te::dt::RASTER_TYPE    ||
     newP->getType() == te::dt::COMPOSITE_TYPE || 
     newP->getType() == te::dt::DATASET_TYPE   || 
     newP->getType() == te::dt::RASTER_TYPE)
    throw Exception(TR_PGIS("The informed property is not valid!"));
  
  te::da::DataSetType* dt = static_cast<te::da::DataSetType*>(oldP->getParent());
  te::dt::SimpleProperty* sOldP = static_cast<te::dt::SimpleProperty*>(oldP);
  te::dt::SimpleProperty* sNewP = static_cast<te::dt::SimpleProperty*>(newP);  

// has the attribute name changed?
  if(oldP->getName() != newP->getName())
    rename(oldP, newP->getName(), false);

// common sql
  std::string SQL("ALTER TABLE ");
              SQL += dt->getName();
              SQL += " ALTER COLUMN ";
              SQL += oldP->getName();

// has the type changed?
  //Check if the newP is of array type
  if(oldP->getType() == te::dt::ARRAY_TYPE)
  {
    te::dt::ArrayProperty* oldArrayP = static_cast<te::dt::ArrayProperty*>(oldP);
    te::dt::ArrayProperty* newArrayP = static_cast<te::dt::ArrayProperty*>(newP);

    te::dt::Property* oldArrayElemP = oldArrayP->getElementType();
    te::dt::Property* newArrayElemP = newArrayP->getElementType();
    while(oldArrayElemP->getType() == te::dt::ARRAY_TYPE)
    {
      oldArrayElemP = static_cast<te::dt::ArrayProperty*>(oldArrayElemP)->getElementType();
      newArrayElemP = static_cast<te::dt::ArrayProperty*>(newArrayElemP)->getElementType();
    }

    if(oldArrayElemP->getType() != newArrayElemP->getType())
    {
      std::string sql(SQL);
                  sql += " TYPE " ;

      SetColumnDef(sql, newP, true);

      m_t->execute(sql);
    }
  }
  else
  {
    if(oldP->getType() != newP->getType())
    {
      std::string sql(SQL);
                  sql += " TYPE " ;

      SetColumnDef(sql, newP, true);

      m_t->execute(sql);
    }
  }

// has the default dropped?
  if( sOldP->getDefaultValue() != 0 &&  
     (sNewP->getDefaultValue() == 0 || (*sOldP->getDefaultValue() != *sNewP->getDefaultValue())))
  {
    std::string sql(SQL);
                sql += " DROP DEFAULT";

    m_t->execute(sql);
  }
  
  // if has default, has it changed?
  if(sNewP->getDefaultValue() != 0 && 
    (sOldP->getDefaultValue() == 0 || (*sOldP->getDefaultValue() != *sNewP->getDefaultValue())))
  {
    std::string sql(SQL);
                sql += " SET DEFAULT ";

    if(newP->getType() == te::dt::STRING_TYPE || newP->getType() == te::dt::ARRAY_TYPE)
      sql += "'" + *sNewP->getDefaultValue() + "'";
    else
      sql += *sNewP->getDefaultValue();
  
    m_t->execute(sql);
  }

// was required and now is not required?
  if(sOldP->isRequired() && !sNewP->isRequired())
  {
    std::string sql(SQL);
                sql += " DROP NOT NULL";

    m_t->execute(sql);
  }
  
// was not required and now is required?
  if(!sOldP->isRequired() && sNewP->isRequired())
  {
    std::string sql(SQL);
                sql += " SET NOT NULL";

    m_t->execute(sql);
  }

  // replace attributes in the datasettype
  dt->replace(oldP, newP);
  
  if(refresh)
  {
    std::auto_ptr<CatalogLoader> loader(m_t->getPGCatalogLoader());
    loader->getCheckConstraints(dt);
    loader->getIndexes(dt);
    loader->getUniqueKeys(dt);
  }    
}
        
void te::pgis::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::PrimaryKey* pk, const bool refresh)
{
  if(pk->getName().empty())
  {
    std::string pkname = dt->getName() + "_pk";

    boost::replace_all(pkname, ".", "_");

    pk->setName(pkname);
  }

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " ADD CONSTRAINT ";
              sql += pk->getName();
              sql += " PRIMARY KEY (";

  const std::vector<te::dt::Property*>& properties = pk->getProperties();

  std::size_t size = properties.size();

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
    std::auto_ptr<CatalogLoader> loader(m_t->getPGCatalogLoader());
    loader->getIndexes(dt);
  }
}
        
void te::pgis::DataSetTypePersistence::drop(te::da::PrimaryKey* pk, const bool refresh)
{
  te::da::DataSetType* dt = pk->getDataSetType();

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " DROP CONSTRAINT " + pk->getName();

  m_t->execute(sql);
  
  dt->setPrimaryKey(0);
  
  if(refresh)
  {
    std::auto_ptr<CatalogLoader> loader(m_t->getPGCatalogLoader());
    loader->getIndexes(dt);    
  }  
}
        
void te::pgis::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::UniqueKey* uk, const bool refresh)
{
  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " ADD CONSTRAINT ";
              sql += uk->getName();
              sql += " UNIQUE (";

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
    std::auto_ptr<CatalogLoader> loader(m_t->getPGCatalogLoader());
    loader->getIndexes(dt);    
  }  
} 

void te::pgis::DataSetTypePersistence::drop(te::da::UniqueKey* uk, const bool refresh)
{
  te::da::DataSetType* dt = uk->getDataSetType();

  std::string sql("ALTER TABLE ");
              sql += dt->getName();
              sql += " DROP CONSTRAINT ";
              sql += uk->getName();

  m_t->execute(sql);
  dt->remove(uk);
  
  if(refresh)
  {
    std::auto_ptr<CatalogLoader> loader(m_t->getPGCatalogLoader());
    loader->getIndexes(dt);    
  }  
}
