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
  \file terralib/dataaccess/core/dataset/DataSetType.cpp

  \brief A class that models the description of a dataset.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../common/Translator.h"
#include "../../../datatype/Enums.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../raster/RasterProperty.h"
#include "../datasource/DataSourceCatalog.h"
#include "../Exception.h"
#include "CheckConstraint.h"
#include "DataSetType.h"
#include "ForeignKey.h"
#include "Index.h"
#include "PrimaryKey.h"
#include "Sequence.h"
#include "UniqueKey.h"

// STL
#include <cassert>

te::da::core::DataSetType::DataSetType(const std::string& name)
  : CompositeProperty(name, name, te::dt::DATASET_TYPE),
    m_catalog(0),
    m_pk(0),
    m_category(te::da::UNKNOWN_DATASET_TYPE)
{
}

te::da::core::DataSetType::DataSetType(const DataSetType& rhs)
 : CompositeProperty(rhs),
   m_catalog(0),
   m_pk(0),
   m_title(rhs.m_title),
   m_category(rhs.m_category)
{

// copy indexes
  {
    const std::size_t size = rhs.m_idxs.size();

    for(std::size_t i = 0; i != size; ++i)
    {
      const Index* rhsIdx = rhs.m_idxs[i];

      Index* idx = new Index(*rhsIdx);

      idx->setDataSetType(this);

      m_idxs.push_back(idx);
    }
  }

// copy primary key
  if(rhs.m_pk)
  {
    m_pk = new PrimaryKey(*rhs.m_pk);

    m_pk->setDataSetType(this);
  }

// copy unique keys
  {
    const std::size_t size = rhs.m_uniqueKeys.size();

    for(std::size_t i = 0; i != size; ++i)
    {
      UniqueKey* rhsUk = rhs.m_uniqueKeys[i];

      UniqueKey* uk = new UniqueKey(rhsUk->getName());

      uk->setDataSetType(this);

      m_uniqueKeys.push_back(uk);
    }
  }

// copy check constraints
  {
    const std::size_t size = rhs.m_checkConstraints.size();

    for(std::size_t i = 0; i != size; ++i)
    {
      CheckConstraint* rhsCk = rhs.m_checkConstraints[i];

      CheckConstraint* ck = new CheckConstraint(*rhsCk);

      ck->setDataSetType(this);

      m_checkConstraints.push_back(ck);
    }
  }

// copy foreign keys
  {
    const std::size_t size = rhs.m_foreignKeys.size();
  
    for(std::size_t i = 0; i != size; ++i)
    {
      ForeignKey* rhsFk = rhs.m_foreignKeys[i];

      ForeignKey* fk = new ForeignKey(*rhsFk);

      fk->setDataSetType(this);

      m_foreignKeys.push_back(fk);
    }
  }
}

te::da::core::DataSetType::~DataSetType()
{
  delete m_pk;
  te::common::FreeContents(m_foreignKeys);
  te::common::FreeContents(m_checkConstraints);
  te::common::FreeContents(m_idxs);
  te::common::FreeContents(m_uniqueKeys);
}

te::da::core::DataSetType& te::da::core::DataSetType::operator=(const DataSetType& rhs)
{
  if(this != &rhs)
  {
    throw Exception(TR_DATAACCESS("Not implemented yet!"));
  }

  return *this;
}

void te::da::core::DataSetType::setPrimaryKey(std::auto_ptr<PrimaryKey> pk)
{
  if(pk.get() == m_pk)
    return;

  delete m_pk;

  if(pk.get())
    pk->setDataSetType(this);

  m_pk = pk.release();
}

te::da::core::UniqueKey* te::da::core::DataSetType::getUniqueKey(const std::string& name) const
{
  std::size_t size = m_uniqueKeys.size();
  
  for(std::size_t i = 0; i < size; ++i)
  {
    if(m_uniqueKeys[i]->getName() == name)
      return m_uniqueKeys[i];
  }

  return 0;
}

void te::da::core::DataSetType::remove(UniqueKey* uk)
{
  std::size_t size = m_uniqueKeys.size();
  
  for(std::size_t i = 0; i < size; ++i)
  {
    if(m_uniqueKeys[i] == uk)
    {
      m_uniqueKeys.erase(m_uniqueKeys.begin() + i);
      delete uk;
      break;
    }
  }
}

void te::da::core::DataSetType::clearUniqueKeys()
{
  te::common::FreeContents(m_uniqueKeys);
  m_uniqueKeys.clear();
}

te::da::core::CheckConstraint* te::da::core::DataSetType::getCheckConstraint(const std::string& name) const
{
  std::size_t size = m_checkConstraints.size();
  
  for(std::size_t i = 0; i < size; ++i)
  {
    if(m_checkConstraints[i]->getName() == name)
      return m_checkConstraints[i];
  }

  return 0;
}

void te::da::core::DataSetType::remove(CheckConstraint* cc)
{
  std::size_t size = m_checkConstraints.size();
  
  for(std::size_t i = 0; i < size; ++i)
  {
    if(m_checkConstraints[i] == cc)
    {
      m_checkConstraints.erase(m_checkConstraints.begin() + i);
      delete cc;
      break;
    }
  }
}

void te::da::core::DataSetType::clearCheckConstraints()
{
  te::common::FreeContents(m_checkConstraints);
  m_checkConstraints.clear();
}

void te::da::core::DataSetType::add(std::auto_ptr<Index> idx)
{
  assert(idx.get());
  idx->setDataSetType(this);
  m_idxs.push_back(idx.release());
}

te::da::core::Index* te::da::core::DataSetType::getIndex(const std::string& name) const
{
  std::size_t size = m_idxs.size();

  for(std::size_t i = 0; i < size; ++i)
    if(m_idxs[i]->getName() == name)
      return m_idxs[i];

  return 0;
}

void te::da::core::DataSetType::remove(Index* idx)
{
// let's find the index to drop it
  std::size_t size = m_idxs.size();
    
  for(std::size_t i = 0; i != size; ++i)
  {
    if(m_idxs[i] == idx)
    {
      m_idxs.erase(m_idxs.begin() + i);
      delete idx;
      break;
    }
  }
}

void te::da::core::DataSetType::clearIndexes()
{
  te::common::FreeContents(m_idxs);
  m_idxs.clear();
}

te::da::core::ForeignKey* te::da::core::DataSetType::getForeignKey(const std::string& name) const
{
  std::size_t size = m_foreignKeys.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(m_foreignKeys[i]->getName() == name)
      return m_foreignKeys[i];
  }

  return 0;
}

void te::da::core::DataSetType::add(std::auto_ptr<ForeignKey> fk)
{
  assert(fk.get());

  if(m_catalog)
    m_catalog->addRef(fk.get());

  fk->setDataSetType(this);

  m_foreignKeys.push_back(fk.release());
}

void te::da::core::DataSetType::remove(ForeignKey* fk)
{
  assert(fk);

  if(fk->getDataSetType() != this)
    throw Exception(TR_DATAACCESS("The foreign key is not associated to this DataSetType!"));

  if(m_catalog)
    m_catalog->removeRef(fk);

  std::size_t size = m_foreignKeys.size();
  
  for(std::size_t i = 0; i < size; ++i)
  {
    if(m_foreignKeys[i] == fk)
    {
      m_foreignKeys.erase(m_foreignKeys.begin() + i);
      delete fk;
      break;
    }
  }
}

void te::da::core::DataSetType::remove(Property* p)
{
// TODO: Check Constraints...

// if the property is associated to the pk, let's remove the pk!
  if(m_pk && m_pk->has(p))
    setPrimaryKey(std::auto_ptr<PrimaryKey>(0));

// if the property is associated to a uk, let's remove the uk!
  removeUniqueKeys(p);

// if the property is associated to an index, let's remove the index!
  removeIndexes(p);

// if the property takes part of a fk or has dataset types referencing it, let's remove the fks!
  removeForeignKeys(p);

// if the property is the owner of the sequence, let's drop it
  if(m_catalog)
    m_catalog->dropDependentSequences(p);

// now the property can be removed!
  CompositeProperty::remove(p);
}

void te::da::core::DataSetType::clear()
{
  delete m_pk;
  m_pk = 0;

  te::common::FreeContents(m_foreignKeys);
  m_foreignKeys.clear();

  te::common::FreeContents(m_checkConstraints);
  m_checkConstraints.clear();

  te::common::FreeContents(m_idxs);
  m_idxs.clear();

  te::common::FreeContents(m_uniqueKeys);
  m_uniqueKeys.clear();
}

void te::da::core::DataSetType::replace(Property* p, Property* pp)
{
// let's replace primary key attributes
  if(m_pk)
    m_pk->replace(p, pp);

// let's replace unique keys
  {
    std::size_t size = m_uniqueKeys.size();

    for(std::size_t i = 0; i < size; ++i)
      m_uniqueKeys[i]->replace(p, pp);
  }

// let's replace indexes
  {
    std::size_t size = m_idxs.size();

    for(std::size_t i = 0; i < size; ++i)
      m_idxs[i]->replace(p, pp);
  }

// if dataset type is associated to the data source catalog, let's fix foreign keys and sequences
  if(m_catalog)
  {
// replace sequences
// TODO: !!in the future try to explorer the catalog index for dt and sequences!!
    {
      std::size_t size = m_catalog->getNumberOfSequences();

      for(std::size_t i = 0; i < size; ++i)
        if(m_catalog->getSequence(i)->getOwner() == p)
          m_catalog->getSequence(i)->setOwner(pp);
    }

// replace foreign keys in all datasets
// TODO: !!in the future try to explore the catalog index for dt and fk!!
    {
      std::size_t size = m_catalog->getNumberOfDataSets();

      for(std::size_t i = 0; i < size; ++i)
      {
        const DataSetTypePtr& dt = m_catalog->getDataSetType(i);

        std::size_t ssize = dt->getNumberOfForeignKeys();

        for(std::size_t j = 0; j < ssize; ++j)
          dt->getForeignKey(j)->replace(p, pp);
      }
    }
  }
  else
  {
// let's replace foreign key in this dataset
    std::size_t size = m_foreignKeys.size();

    for(std::size_t i = 0; i < size; ++i)
      m_foreignKeys[i]->replace(p, pp);
  }

// and now... to finish... let's replace p by pp in the properties vector
  {
    std::size_t size = m_properties.size();

    for(std::size_t i = 0; i < size; ++i)
      if(m_properties[i] == p)
      {
        m_properties[i] = pp;
        break;
      }
  }
}

te::dt::Property* te::da::core::DataSetType::clone() const
{
  return new DataSetType(*this);
}

void te::da::core::DataSetType::removeUniqueKeys(Property* p)
{
  std::size_t size = m_uniqueKeys.size();
  std::size_t i = 0;

  while(i < size)
  {
    if(m_uniqueKeys[i]->has(p))
    {
      delete m_uniqueKeys[i];
      m_uniqueKeys.erase(m_uniqueKeys.begin() + i);
      --size;
    }
    else
      ++i;
  }
}

void te::da::core::DataSetType::removeIndexes(Property* p)
{
  std::size_t size = m_idxs.size();
  std::size_t i = 0;

  while(i < size)
  {
    if(m_idxs[i]->has(p))
    {
      delete m_idxs[i];
      m_idxs.erase(m_idxs.begin() + i);
      --size;
    }
    else
      ++i;
  }
}

void te::da::core::DataSetType::removeForeignKeys(Property* p)
{
// first of all, let's see if property takes part
// in a self foreign key.
  {
    std::size_t size = m_foreignKeys.size();
    std::size_t i = 0;

    while(i < size)
    {
      if(m_foreignKeys[i]->has(p))
      {
        if(m_catalog)
          m_catalog->removeRef(m_foreignKeys[i]);

        delete m_foreignKeys[i];
        m_foreignKeys.erase(m_foreignKeys.begin() + i);
        --size;
      }
      else
        ++i;
    }
  }

// is there in the catalog another foreign key referencing this attribute?
  {
    if(m_catalog == 0)
      return;

    std::pair<std::multimap<DataSetType*, ForeignKey*>::const_iterator,
              std::multimap<DataSetType*, ForeignKey*>::const_iterator> range = m_catalog->getRefFK(this);

    while(range.first != range.second)
    {
      std::multimap<DataSetType*, ForeignKey*>::const_iterator it = range.first; // keep a pointer to current element... maybe we will erase it!
      ++range.first;

      if(it->second->isReferenced(p))
      {
        DataSetType* fkFT = it->second->getDataSetType();
        fkFT->remove(it->second);
      }
    }
  }
}


