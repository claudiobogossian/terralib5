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
  \file terralib/dataaccess/datasource/DataSourceCatalog.cpp

  \brief It represents the system catalog of a DataSource.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../dataset/DataSetType.h"
#include "../dataset/Sequence.h"
#include "../dataset/ForeignKey.h"
#include "../Exception.h"
#include "DataSource.h"
#include "DataSourceCatalog.h"

// STL
#include <cassert>

// Boost
#include <boost/format.hpp>

te::da::DataSetTypePtr te::da::DataSourceCatalog::sm_nullds;

te::da::DataSourceCatalog::DataSourceCatalog()
  : m_id(0),
    m_ds(0)
{
}

te::da::DataSourceCatalog::~DataSourceCatalog()
{
  te::common::FreeContents(m_sequences.begin(), m_sequences.end());
}

void te::da::DataSourceCatalog::clear()
{
// datasets
  m_datasets.clear();

// sequences
  te::common::FreeContents(m_sequences.begin(), m_sequences.end());
  m_sequences.clear();

  m_seqFTIdx.clear();

// foreign key
  m_dependentFkIdx.clear();
}

bool te::da::DataSourceCatalog::datasetExists(const std::string& name) const
{
  dataset_idx_type::const_iterator it = m_datasets.find(name);

  return (it != m_datasets.end());
}

void te::da::DataSourceCatalog::add(const DataSetTypePtr& dt)
{
  if(dt.get() == 0)
    throw Exception(TE_TR("Can not add a NULL dataset schema to the catalog!"));

  if(dt->getName().empty())
    throw Exception(TE_TR("Can not add a dataset schema with an empty name to the catalog!"));

  if(datasetExists(dt->getName()))
    throw Exception((boost::format(TE_TR("Can not add dataset schema %1% to the catalog. There is already another one with this name!")) % dt->getName()).str());

  if(dt->getCatalog() != 0)
    throw Exception((boost::format(TE_TR("Can not add dataset schema %1% to the catalog. It is already attached to one!")) % dt->getName()).str());

  checkFKsDependency(dt.get());

  m_datasets.insert(dt);
  dt->setCatalog(this);
  indexFKs(dt.get());
}

void te::da::DataSourceCatalog::remove(DataSetType* dt, const bool cascade)
{
  if(dt == 0)
    throw Exception(TE_TR("Can not remove a NULL dataset schema from the catalog!"));

  if(dt->getName().empty())
    throw Exception(TE_TR("Can not remove a dataset schema with an empty name from the catalog!"));

  if(dt->getCatalog() != this)
    throw Exception(TE_TR("Can not remove a dataset from another catalog!"));

  assert(datasetExists(dt->getName()));
  assert(m_datasets.find(dt->getName()) != m_datasets.end());
  assert((*m_datasets.find(dt->getName())).get() == dt);

  if(cascade)
  {
    dropDependentSequences(dt);
    dropDependentFKs(dt);
  }
  else
  {
    m_seqFTIdx.erase(dt);
    m_dependentFkIdx.erase(dt);
  }

  m_datasets.erase(dt->getName());
}

void te::da::DataSourceCatalog::rename(DataSetType* dt, const std::string& newName)
{
  if(dt == 0)
    throw Exception(TE_TR("Can not rename a NULL dataset schema!"));

  if(datasetExists(newName))
    throw Exception(TE_TR("Could not rename DataSetType because the new name already exist in the catalog!"));

  if(dt->getCatalog() != this)
    throw Exception(TE_TR("Could not rename a dataset from another catalog!"));

  dataset_idx_type::iterator it = m_datasets.find(dt->getName());

  if(it == m_datasets.end())
    throw Exception(TE_TR("Could not find dataset schema in the catalog!"));

  if((*it).get() != dt)
    throw Exception(TE_TR("The dataset schema is not registered in this catalog!"));

  DataSetTypePtr aux = *it;

  m_datasets.erase(it);

  aux->setName(newName);

  m_datasets.insert(aux);
}

const te::da::DataSetTypePtr& te::da::DataSourceCatalog::getDataSetType(std::size_t i) const
{
  assert(i < m_datasets.size());

  const dataset_idx_type::nth_index<1>::type& pos_idx = m_datasets.get<1>();

  return pos_idx[i];
}

const te::da::DataSetTypePtr& te::da::DataSourceCatalog::getDataSetType(const std::string& name) const
{
  assert(!name.empty());

  dataset_idx_type::const_iterator it = m_datasets.find(name);

  if(it != m_datasets.end())
    return *it;

  return sm_nullds;
}

void te::da::DataSourceCatalog::add(Sequence* s)
{
  if(getSequence(s->getName()))
    throw Exception(TE_TR("Could not add Sequence because there is already another Sequence with the same name in the catalog!"));

  checkSequenceDependency(s);
  m_sequences.insert(s);
  s->setCatalog(this);
  indexSequenceDependency(s);
}

void te::da::DataSourceCatalog::remove(Sequence* s)
{
  detach(s);
  delete s;
}

void te::da::DataSourceCatalog::detach(Sequence* s)
{
  m_sequences.erase(s->getName());
  dropDependentSequenceEntry(s);
}

te::da::Sequence* te::da::DataSourceCatalog::getSequence(std::size_t i) const
{
  const sequence_idx_type::nth_index<1>::type& pos_idx = m_sequences.get<1>();

  return pos_idx[i];
}

te::da::Sequence* te::da::DataSourceCatalog::getSequence(const std::string& name) const
{
  sequence_idx_type::const_iterator it = m_sequences.find(name);

  if(it != m_sequences.end())
    return *it;

  return 0;
}

void te::da::DataSourceCatalog::addRef(ForeignKey* fk)
{
  te::da::DataSetType* refFT = fk->getReferencedDataSetType();

  if((refFT == 0) || (getDataSetType(refFT->getName()).get() != refFT))
    throw Exception(TE_TR("Could not find the DataSetType referenced in foreign key!"));

  m_dependentFkIdx.insert(std::pair<DataSetType*, ForeignKey*>(refFT, fk));
}

void te::da::DataSourceCatalog::removeRef(ForeignKey* fk)
{
  te::da::DataSetType* refFT = fk->getReferencedDataSetType();

  if((refFT == 0) || (getDataSetType(refFT->getName()).get() != refFT))
    throw Exception(TE_TR("Could not find the DataSetType referenced in foreign key!"));

  std::pair<std::multimap<DataSetType*, ForeignKey*>::iterator,
            std::multimap<DataSetType*, ForeignKey*>::iterator> range = m_dependentFkIdx.equal_range(refFT);

  while(range.first != range.second)
  {
    std::multimap<DataSetType*, ForeignKey*>::iterator it = range.first; // keep a pointer to current element... we must erase it!

    ++range.first;  // go to next element before erasing current!

// is the iterator pointing to the right entry?
    if(it->second == fk)
    {
      m_dependentFkIdx.erase(it);
      return;
    }
  }

  throw Exception(TE_TR("Something went wrong when removing the association of a foreign key and a DataSetType in the DataSourceCatalog!"));
}

void te::da::DataSourceCatalog::dropDependentSequences(te::dt::Property* p)
{
  te::dt::Property* parent = p;

  while(parent)
  {
    parent = parent->getParent();

    if(parent &&
       (parent->getType() == te::dt::DATASET_TYPE) &&
       (getDataSetType(parent->getName()).get() == parent))
    {
      te::da::DataSetType* dt = static_cast<te::da::DataSetType*>(parent);

// is the dt owner of a sequence?
      std::pair<std::multimap<DataSetType*, sequence_idx_type::iterator>::iterator,
                std::multimap<DataSetType*, sequence_idx_type::iterator>::iterator> range = m_seqFTIdx.equal_range(dt);

      while(range.first != range.second)
      {
        std::multimap<DataSetType*, sequence_idx_type::iterator>::iterator it = range.first;
        ++range.first;

        te::da::Sequence* s = *(it->second);

// is the property owner of the sequence?
        if(s->getOwner() == p)
        {
          m_sequences.erase(it->second);
          delete s;
          m_seqFTIdx.erase(it);
        }
      }

      return;
    }
  }
}

void te::da::DataSourceCatalog::checkSequenceDependency(Sequence* s) const
{
  if(s->getOwner() == 0)
    return;

  te::dt::Property* parent = s->getOwner();

  while(parent)
  {
    parent = parent->getParent();

// if the parent is a FetaureType and it is in the catalog, stop!
    if(parent &&
       (parent->getType() == te::dt::DATASET_TYPE) &&
       (getDataSetType(parent->getName()).get() == parent))
        return;
  }

  throw Exception(TE_TR("Could not find in the catalog the DataSetType that owns the given sequence!"));
}

void te::da::DataSourceCatalog::indexSequenceDependency(Sequence* s)
{
  if(s->getOwner() == 0)
    return;

  te::dt::Property* parent = s->getOwner();

  sequence_idx_type::iterator it = m_sequences.find(s->getName());

  while(parent)
  {
    parent = parent->getParent();

// if the parent is a FetaureType and it is in the catalog, make index and stop!
    if(parent &&
       (parent->getType() == te::dt::DATASET_TYPE) &&
       (getDataSetType(parent->getName()).get() == parent))
      {
        te::da::DataSetType* dt = static_cast<te::da::DataSetType*>(parent);
        m_seqFTIdx.insert(std::pair<DataSetType*, sequence_idx_type::iterator>(dt, it));
        return;
      }
  }

  throw Exception(TE_TR("Could not find in the catalog the DataSetType that owns the given sequence!"));
}

void te::da::DataSourceCatalog::dropDependentSequences(DataSetType* dt)
{
  std::pair<std::multimap<DataSetType*, sequence_idx_type::iterator>::iterator,
            std::multimap<DataSetType*, sequence_idx_type::iterator>::iterator> range = m_seqFTIdx.equal_range(dt);

  while(range.first != range.second)
  {
    std::multimap<DataSetType*, sequence_idx_type::iterator>::iterator it = range.first;
    ++range.first;

// remove sequence and all its entry in the catalog
// !! DON'T CALL remove(s) THIS WILL NEED EXTRA COMPUTATIONS AND ALSO WILL NEED TO CHANGE THE LOGIC OF CODE BLOCK!!
    te::da::Sequence* s = *(it->second);
    m_sequences.erase(it->second);
    delete s;
    m_seqFTIdx.erase(it);
  }
}

void te::da::DataSourceCatalog::dropDependentSequenceEntry(Sequence* s)
{
  if(s->getOwner() == 0)
    return;

  te::dt::Property* parent = s->getOwner();
  
  sequence_idx_type::iterator itpos = m_sequences.find(s->getName());

  while(parent)
  {
    parent = parent->getParent();

// if the parent is a FetaureType and it is in the catalog, make index and stop!
    if(parent &&
       (parent->getType() == te::dt::DATASET_TYPE) &&
       (getDataSetType(parent->getName()).get() == parent))
      {
        te::da::DataSetType* dt = static_cast<te::da::DataSetType*>(parent);

        std::pair<std::multimap<DataSetType*, sequence_idx_type::iterator>::iterator,
                  std::multimap<DataSetType*, sequence_idx_type::iterator>::iterator> range = m_seqFTIdx.equal_range(dt);

        while(range.first != range.second)
        {
          std::multimap<DataSetType*, sequence_idx_type::iterator>::iterator it = range.first;
          ++range.first;

          if(*(it->second) == *itpos)
          {
            m_seqFTIdx.erase(it);
            return;
          }
        }
      }
  }

  throw Exception(TE_TR("Could not find in the catalog the DataSetType that owns the given sequence in order to remove the sequence entry!"));
}

void te::da::DataSourceCatalog::checkFKsDependency(DataSetType* dt) const
{
  std::size_t size = dt->getNumberOfForeignKeys();

  for(std::size_t i = 0; i < size; ++i)
  {
    te::da::ForeignKey* fk = dt->getForeignKey(i);
    te::da::DataSetType* refFT = fk->getReferencedDataSetType();

    if((refFT == 0) || (getDataSetType(refFT->getName()).get() != refFT))
      throw Exception(TE_TR("There is a foreign key in the DataSetType referencing another DataSetType that is not in the catalog!"));
  }
}

void te::da::DataSourceCatalog::indexFKs(DataSetType* dt)
{
  std::size_t size = dt->getNumberOfForeignKeys();

  for(std::size_t i = 0; i < size; ++i)
  {
    te::da::ForeignKey* fk = dt->getForeignKey(i);
    te::da::DataSetType* refFT = fk->getReferencedDataSetType();
    
    assert(getDataSetType(refFT->getName()).get() == refFT);

    m_dependentFkIdx.insert(std::pair<DataSetType*, ForeignKey*>(refFT, fk));
  }
}

void te::da::DataSourceCatalog::dropDependentFKs(DataSetType* dt)
{
  std::pair<std::multimap<DataSetType*, ForeignKey*>::iterator,
            std::multimap<DataSetType*, ForeignKey*>::iterator> range = m_dependentFkIdx.equal_range(dt);

  while(range.first != range.second)
  {
    std::multimap<DataSetType*, ForeignKey*>::iterator it = range.first; // keep a pointer to current element... we must erase it!

    ++range.first;  // go to next element before erasing current!

// who is the owner of the fk? ask it to drop the fk!
    te::da::ForeignKey* fk = it->second;
    te::da::DataSetType* fkOwner = fk->getDataSetType();
    fkOwner->remove(fk);
  }
}

te::da::DataSourceCatalog::dataset_name_cmp::result_type te::da::DataSourceCatalog::dataset_name_cmp::operator()(const DataSetTypePtr& dt) const
{
  return dt->getName();
}

te::da::DataSourceCatalog::sequence_name_cmp::result_type te::da::DataSourceCatalog::sequence_name_cmp::operator()(const Sequence* const s) const
{
  return s->getName();
}

