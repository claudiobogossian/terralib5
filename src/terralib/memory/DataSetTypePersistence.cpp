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
  \file terralib/memory/DataSetTypePersistence.cpp

  \brief DataSetTypePersistence implementation for the In-Memory Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/Sequence.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "DataSet.h"
#include "DataSetTypePersistence.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"

// STL
#include <memory>

// Boost
#include <boost/format.hpp>

te::mem::DataSetTypePersistence::DataSetTypePersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::mem::DataSetTypePersistence::~DataSetTypePersistence()
{
}

void te::mem::DataSetTypePersistence::create(te::da::DataSetType* dt, const std::map<std::string, std::string>& /*options*/)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("A dataset with the same name (%1%) already exists!")) % dt->getName()).str());

  DataSet* dataset = new DataSet(static_cast<te::da::DataSetType*>(dt->clone()));

  ds->add(dt->getName(), dataset);
}

void te::mem::DataSetTypePersistence::drop(te::da::DataSetType* dt)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());
    
  ds->remove(dt->getName());

  if(dt->getCatalog())
    ds->getCatalog()->remove(dt); 
}

void te::mem::DataSetTypePersistence::rename(te::da::DataSetType* dt, const std::string& newName)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());
  
  std::string oldName = dt->getName();

  if(dt->getCatalog())
    ds->getCatalog()->rename(dt, newName);

  ds->rename(oldName, newName);
}

void te::mem::DataSetTypePersistence::add(te::da::DataSetType* dt, te::dt::Property* p)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(dt->getProperty(p->getName()))
    throw Exception((boost::format(TR_MEMORY("A property with the same name (%1%) already exists!")) % p->getName()).str());
  
  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str()); 
  
  dt->add(p);
  ds->getDataSet(dt->getName())->add(p->clone());
}

void te::mem::DataSetTypePersistence::drop(te::dt::Property* p)
{
  DataSource* ds = m_t->getMemDataSource();

  te::da::DataSetType* dt = dynamic_cast<te::da::DataSetType*>(p->getParent());

  if(dt == 0)
    throw Exception((boost::format(TR_MEMORY("Dataset for property %1% is unknown!")) % p->getName()).str());

  DataSource::LockWrite l(ds);

  ds->getDataSet(dt->getName())->drop(p);
  dt->remove(p);
  
}

void te::mem::DataSetTypePersistence::rename(te::dt::Property* p, const std::string& newName) 
{
  DataSource* ds = m_t->getMemDataSource();

  te::da::DataSetType* dt = dynamic_cast<te::da::DataSetType*>(p->getParent());

  if(dt == 0)
    throw Exception((boost::format(TR_MEMORY("Dataset for property %1% is unknown!")) % p->getName()).str());

  DataSource::LockWrite l(ds);
  
  ds->getDataSet(p->getName())->getType()->getProperty(p->getName())->setName(newName);
  
  dt->getProperty(p->getName())->setName(newName);
}

void te::mem::DataSetTypePersistence::update(te::dt::Property* /*oldP*/, te::dt::Property* /*newP*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));

  /*DataSource* ds = m_t->getMemDataSource();

  te::da::DataSetType* dt = dynamic_cast<te::da::DataSetType*>(oldP->getParent());

  if(dt == 0)
    throw Exception((boost::format(TR_MEMORY("Dataset for property %1% is unknown!")) % oldP->getName()).str());

  // If different names, check if already exists a dataset with the new name
  if(oldP->getName() != newP->getName())
    if(ds->getDataSet(newP->getName()))
      throw Exception((boost::format(TR_MEMORY("A dataset with the same name (%1%) already exists!")) % newP->getName()).str());

  DataSource::LockWrite l(ds);

  dt->replace(dt->getProperty(oldP->getName()), newP);

  ds->getCatalog()->getDataSetType(oldP->getName())->setName(oldP->getName());

  if(dt->getProperty(oldP->getName())->getType() != newP->getType())
    ds->getDataSet(dt->getName())->update(newP);*/
}

void te::mem::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::PrimaryKey* pk) 
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  if(ds->getDataSet(dt->getName())->getType()->getPrimaryKey())
    throw Exception((boost::format(TR_MEMORY("The dataset %1% already has a Primary Key!")) % dt->getName()).str());

  te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  te::da::PrimaryKey* newPk = new te::da::PrimaryKey(pk->getName(), dsDt);

  for(std::size_t i = 0; i < pk->getProperties().size(); ++i)
  {
     const te::dt::Property* p = pk->getProperties()[i];

     te::dt::Property* dsP = dsDt->getProperty(p->getName());

     newPk->add(dsP);
  }

  dt->setPrimaryKey(pk);
}

void te::mem::DataSetTypePersistence::drop(te::da::PrimaryKey* pk)
{
  DataSource* ds = m_t->getMemDataSource();

  te::da::DataSetType* dt = pk->getDataSetType();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  if(!dsDt->getPrimaryKey())
    throw Exception((boost::format(TR_MEMORY("The dataset %1% has not a Primary Key!")) % dsDt->getName()).str());

  if(dsDt->getPrimaryKey()->getName() != pk->getName())
    throw Exception((boost::format(TR_MEMORY("The dataset %1% has not a Primary Key named %2%!")) % dsDt->getName() % pk->getName()).str());
    
  if(dsDt != dt)
    dsDt->setPrimaryKey(0);

  if(ds->getCatalog()->getDataSetType(dsDt->getName()))
    ds->getCatalog()->getDataSetType(dsDt->getName())->setPrimaryKey(0);
}

void te::mem::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::UniqueKey* uk) 
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  if(ds->getDataSet(dt->getName())->getType()->getUniqueKey(uk->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% already has a Unique Key with this name (%2%)!")) % dt->getName() % uk->getName()).str());

  te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  te::da::UniqueKey* newUk = new te::da::UniqueKey(uk->getName(), dsDt);

  for(std::size_t i = 0; i < uk->getProperties().size(); ++i)
  {
     const te::dt::Property* p = uk->getProperties()[i];

     te::dt::Property* dsP = dsDt->getProperty(p->getName());

     newUk->add(dsP);
  }

  dt->add(uk);
}

void te::mem::DataSetTypePersistence::drop(te::da::UniqueKey* uk)
{
  DataSource* ds = m_t->getMemDataSource();

  te::da::DataSetType* dt = uk->getDataSetType();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());
  
  te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  if(!dsDt->getUniqueKey(uk->getName()))
    throw Exception((boost::format(TR_MEMORY("The Unique Key %1% not exists!")) % uk->getName()).str());
    
  if(dsDt != dt)
    dsDt->remove(dsDt->getUniqueKey(uk->getName()));

  if(ds->getCatalog()->getDataSetType(dsDt->getName()))
    ds->getCatalog()->getDataSetType(dsDt->getName())->remove(dsDt->getUniqueKey(uk->getName()));
}

void te::mem::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::Index* index, const std::map<std::string, std::string>& /*options*/) 
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  if(ds->getDataSet(dt->getName())->getType()->getIndex(index->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% already has a Index with this name (%2%)!")) % dt->getName() % index->getName()).str());

  te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  te::da::Index* newIndex = new te::da::Index(index->getName(), index->getIndexType(), dsDt);

  for(std::size_t i = 0; i < index->getProperties().size(); ++i)
  {
     const te::dt::Property* p = index->getProperties()[i];

     te::dt::Property* dsP = dsDt->getProperty(p->getName());

     newIndex->add(dsP);
  }

  dt->add(index);
}

void te::mem::DataSetTypePersistence::drop(te::da::Index* index)
{
  DataSource* ds = m_t->getMemDataSource();

  te::da::DataSetType* dt = index->getDataSetType();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());
  
  te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  if(!dsDt->getIndex(index->getName()))
    throw Exception((boost::format(TR_MEMORY("The Index %1% not exists!")) % index->getName()).str());

  dsDt->remove(dsDt->getIndex(index->getName()));

  if(dsDt != dt)
    dt->remove(index);

  if(ds->getCatalog()->getDataSetType(dsDt->getName()))
    ds->getCatalog()->getDataSetType(dsDt->getName())->remove(index);
}

void te::mem::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::ForeignKey* fk)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  te::da::ForeignKey* newFk = new te::da::ForeignKey(fk->getName()); 

  for(std::size_t i = 0; i < fk->getProperties().size(); ++i)
  {
     const te::dt::Property* p = fk->getProperties()[i];

     te::dt::Property* dsP = dsDt->getProperty(p->getName());

     newFk->add(dsP);
  }

  for(std::size_t i = 0; i < fk->getReferencedProperties().size(); ++i)
  {
     const te::dt::Property* p = fk->getProperties()[i];

     te::da::DataSetType* currDt = static_cast<te::da::DataSetType*>(p->getParent());

     if(!ds->datasetExists(currDt->getName()))
       throw Exception((boost::format(TR_MEMORY("The dataset %1% referenced not exists!")) % currDt->getName()).str());

     te::da::DataSetType* currRefDt = ds->getDataSet(currDt->getName())->getType();

     te::dt::Property* refP = currRefDt->getProperty(fk->getReferencedProperties()[i]->getName());

     newFk->addRefProperty(refP);
  }

  dsDt->add(newFk);
}

void te::mem::DataSetTypePersistence::drop(te::da::ForeignKey* fk)
{
  DataSource* ds = m_t->getMemDataSource();

  te::da::DataSetType* dt = fk->getDataSetType();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());
  
  te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  te::da::ForeignKey* dsFk = dsDt->getForeignKey(fk->getName());
  
  if(!dsFk)
    throw Exception((boost::format(TR_MEMORY("The Foreign Key %1% not exists!")) % fk->getName()).str());
    
  if(dsDt != dt)
    dsDt->remove(dsFk);

  if(ds->getCatalog()->getDataSetType(dsDt->getName()))
    ds->getCatalog()->getDataSetType(dsDt->getName())->remove(dsFk);
}

void te::mem::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::CheckConstraint* cc)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  if(ds->getDataSet(dt->getName())->getType()->getCheckConstraint(cc->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% already has a Check Constraint with this name (%2%)!")) % dt->getName() % cc->getName()).str());

  te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  te::da::CheckConstraint* newCc = new te::da::CheckConstraint(cc->getName(), dsDt);  
 
  newCc->setExpression(cc->getExpression());
}

void te::mem::DataSetTypePersistence::drop(te::da::CheckConstraint* cc)
{
  DataSource* ds = m_t->getMemDataSource();

  te::da::DataSetType* dt = cc->getDataSetType();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());
  
  te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  if(!dsDt->getCheckConstraint(cc->getName()))
    throw Exception((boost::format(TR_MEMORY("The Check Constraint %1% not exists!")) % cc->getName()).str());  

  if(dsDt != dt)
    dsDt->remove(dsDt->getCheckConstraint(cc->getName()));

  if(ds->getCatalog()->getDataSetType(dsDt->getName()))
    ds->getCatalog()->getDataSetType(dsDt->getName())->remove(dsDt->getCheckConstraint(cc->getName()));
}

void te::mem::DataSetTypePersistence::create(te::da::Sequence* sequence)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(ds->getCatalog()->getSequence(sequence->getName()))
    throw Exception((boost::format(TR_MEMORY("The datasource already has a Sequence with this name (%1%)!")) % sequence->getName()).str());
     
  te::da::Sequence* newSeq = new te::da::Sequence(*sequence);
  newSeq->setCatalog(ds->getCatalog());

  //getting the datasettype by the name of the sequence datasettype property involved
  te::da::DataSetType* dsDt = ds->getCatalog()->getDataSetType(static_cast<te::da::DataSetType*>(sequence->getOwner()->getParent())->getName());

  newSeq->setOwner(dsDt->getProperty(sequence->getOwner()->getName()));
}

void te::mem::DataSetTypePersistence::drop(te::da::Sequence* sequence)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(ds->getCatalog()->getSequence(sequence->getName()))
    throw Exception((boost::format(TR_MEMORY("The datasource already has a Sequence with this name (%1%)!")) % sequence->getName()).str());

  ds->getCatalog()->remove(ds->getCatalog()->getSequence(sequence->getName()));
}

te::da::DataSourceTransactor* te::mem::DataSetTypePersistence::getTransactor() const
{
  return m_t;
}

