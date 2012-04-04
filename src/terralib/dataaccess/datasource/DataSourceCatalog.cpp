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

te::da::DataSourceCatalog::DataSourceCatalog()
  : m_id(0),
    m_ds(0)
{
}

te::da::DataSourceCatalog::~DataSourceCatalog()
{
  te::common::FreeContents(m_dsets);
  te::common::FreeContents(m_sequences);  
}

void te::da::DataSourceCatalog::clear()
{
// FT
  te::common::FreeContents(m_dsets);
  m_dsets.clear();
  m_dtIdIdx.clear();
  m_dtNameIdx.clear();

// Sequences
  te::common::FreeContents(m_sequences);  
  m_sequences.clear();
  m_seqNameIdx.clear();  
  m_seqIdIdx.clear();
  m_seqFTIdx.clear();

// FK
  m_dependentFkIdx.clear();
}

te::da::DataSourceCatalog* te::da::DataSourceCatalog::clone() const
{
  return new DataSourceCatalog(*this);
}

void te::da::DataSourceCatalog::add(DataSetType* dt)
{
  if(getDataSetType(dt->getName()))
    throw Exception(TR_DATAACCESS("Could not add DataSetType because there is already another DataSetType with the same name in the catalog!"));

  if(getDataSetTypeById(dt->getId()))
    throw Exception(TR_DATAACCESS("Could not add DataSetType because there is already another DataSetType with the same id in the catalog!"));

  checkFKsDependency(dt);  
  std::size_t pos = m_dsets.size();
  m_dtIdIdx[dt->getId()] = pos;
  m_dtNameIdx[dt->getName()] = pos;
  dt->setCatalog(this);
  m_dsets.push_back(dt);
  indexFKs(dt);
}

void te::da::DataSourceCatalog::add(const std::vector<DataSetType*>& dsets)
{
  std::size_t size = dsets.size();

  for(std::size_t i = 0; i < size; ++i)
    add(dsets[i]);
}

void te::da::DataSourceCatalog::remove(DataSetType* dt, const bool cascade)
{
  detach(dt, cascade);
  delete dt;
}

void te::da::DataSourceCatalog::detach(DataSetType* dt, const bool cascade)
{
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

  std::size_t pos = getDataSetTypePos(dt);
  m_dtNameIdx.erase(dt->getName());
  m_dtIdIdx.erase(dt->getId());
  m_dsets.erase(m_dsets.begin() + pos);
}

void te::da::DataSourceCatalog::rename(DataSetType* dt, const std::string& newName)
{
  if(getDataSetType(newName))
    throw Exception(TR_DATAACCESS("Could not rename DataSetType because the new name already exist in the catalog!"));

  if((dt == 0) || (getDataSetType(dt->getName()) != dt))
    throw Exception(TR_DATAACCESS("Could not rename DataSetType because it was not found in the catalog a DataSetType with the given name to change its name!"));

  std::size_t pos = getDataSetTypePos(dt);
  m_dtNameIdx.erase(dt->getName());
  dt->setName(newName);
  m_dtNameIdx[dt->getName()] = pos;
}

void te::da::DataSourceCatalog::setId(DataSetType* dt, unsigned int newId)
{
  if(getDataSetTypeById(newId))
    throw Exception(TR_DATAACCESS("Could not change the DataSetType id because the new id already exist in the catalog!"));

  if((dt == 0) || (getDataSetTypeById(dt->getId()) != dt))
    throw Exception(TR_DATAACCESS("Could not change DataSetType id because it was not found in the catalog a DataSetType with the given id to change its id!"));

  std::size_t pos = getDataSetTypePos(dt);
  m_dtIdIdx.erase(dt->getId());
  dt->setId(newId);
  m_dtIdIdx[dt->getId()] = pos;
}

te::da::DataSetType* te::da::DataSourceCatalog::getDataSetType(const std::string& name) const
{
  std::map<std::string, std::size_t>::const_iterator it = m_dtNameIdx.find(name);

  if(it != m_dtNameIdx.end())
    return m_dsets[it->second];

  return 0;
}

te::da::DataSetType* te::da::DataSourceCatalog::getDataSetTypeById(std::size_t id) const
{
  std::map<std::size_t, std::size_t>::const_iterator it = m_dtIdIdx.find(id);

  if(it != m_dtIdIdx.end())
    return m_dsets[it->second];

  return 0;
}

std::size_t te::da::DataSourceCatalog::getDataSetTypePos(DataSetType* dt) const
{
  std::map<std::size_t, std::size_t>::const_iterator it = m_dtIdIdx.find(dt->getId());

  if(it != m_dtIdIdx.end())
    return it->second;

  return std::string::npos;
}

void te::da::DataSourceCatalog::add(Sequence* s)
{
  if(getSequence(s->getName()))
    throw Exception(TR_DATAACCESS("Could not add Sequence because there is already another Sequence with the same name in the catalog!"));

  if(getSequenceById(s->getId()))
    throw Exception(TR_DATAACCESS("Could not add Sequence because there is already another Sequence with the same id in the catalog!"));

  checkSequenceDependency(s);
  std::size_t pos = m_sequences.size();
  m_seqIdIdx[s->getId()] = pos;
  m_seqNameIdx[s->getName()] = pos;
  s->setCatalog(this);
  m_sequences.push_back(s);
  indexSequenceDependency(s);
}

void te::da::DataSourceCatalog::add(const std::vector<Sequence*>& sqs)
{
  std::size_t size = sqs.size();

  for(std::size_t i = 0; i < size; ++i)
    add(sqs[i]);
}

void te::da::DataSourceCatalog::remove(Sequence* s)
{
  detach(s);
  delete s;
}

void te::da::DataSourceCatalog::setId(Sequence* s, unsigned int newId)
{
  if(getSequenceById(newId))
    throw Exception(TR_DATAACCESS("Could not change the sequence id because the new id already exist in the catalog!"));

  if((s == 0) || (getSequenceById(s->getId()) != s))
    throw Exception(TR_DATAACCESS("Could not change sequence id because it was not found in the catalog a sequence with the given id to change its id!"));

  std::size_t pos = getSequencePos(s);
  m_seqIdIdx.erase(s->getId());
  s->setId(newId);
  m_seqIdIdx[s->getId()] = pos;
}

void te::da::DataSourceCatalog::detach(Sequence* s)
{
  std::size_t pos = getSequencePos(s);
  m_seqNameIdx.erase(s->getName());
  m_seqIdIdx.erase(s->getId());
  m_sequences.erase(m_sequences.begin() + pos);
  dropDependentSequenceEntry(s);
}

te::da::Sequence* te::da::DataSourceCatalog::getSequence(const std::string& name) const
{
  std::map<std::string, std::size_t>::const_iterator it = m_seqNameIdx.find(name);

  if(it != m_seqNameIdx.end())
    return m_sequences[it->second];

  return 0;
}

te::da::Sequence* te::da::DataSourceCatalog::getSequenceById(std::size_t id) const
{
  std::map<std::size_t, std::size_t>::const_iterator it = m_seqIdIdx.find(id);

  if(it != m_seqIdIdx.end())
    return m_sequences[it->second];

  return 0;
}

std::size_t te::da::DataSourceCatalog::getSequencePos(Sequence* s) const
{
  std::map<std::size_t, std::size_t>::const_iterator it = m_seqIdIdx.find(s->getId());

  if(it != m_seqIdIdx.end())
    return it->second;

  return std::string::npos;
}

void te::da::DataSourceCatalog::addRef(ForeignKey* fk)
{
  te::da::DataSetType* refFT = fk->getReferencedDataSetType();

  if((refFT == 0) || (getDataSetTypeById(refFT->getId()) != refFT))
    throw Exception(TR_DATAACCESS("Could not find the DataSetType referenced in foreign key!"));

  m_dependentFkIdx.insert(std::pair<DataSetType*, ForeignKey*>(refFT, fk));
}

void te::da::DataSourceCatalog::removeRef(ForeignKey* fk)
{
  te::da::DataSetType* refFT = fk->getReferencedDataSetType();

  if((refFT == 0) || (getDataSetTypeById(refFT->getId()) != refFT))
    throw Exception(TR_DATAACCESS("Could not find the DataSetType referenced in foreign key!"));

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

  throw Exception(TR_DATAACCESS("Something went wrong when removing the association of a foreign key and a DataSetType in the DataSourceCatalog!"));
}

void te::da::DataSourceCatalog::dropDependentSequences(te::dt::Property* p)
{
  te::dt::Property* parent = p;

  while(parent)
  {
    parent = parent->getParent();

    if(parent &&
       (parent->getType() == te::dt::DATASET_TYPE) &&
       (getDataSetTypeById(parent->getId()) == parent))
    {
      te::da::DataSetType* dt = static_cast<te::da::DataSetType*>(parent);

// is the dt owner of a sequence?
      std::pair<std::multimap<DataSetType*, std::size_t>::iterator,
                std::multimap<DataSetType*, std::size_t>::iterator> range = m_seqFTIdx.equal_range(dt);

      while(range.first != range.second)
      {
        std::multimap<DataSetType*, std::size_t>::iterator it = range.first;
        ++range.first;

        std::size_t pos = it->second;
        te::da::Sequence* s = m_sequences[pos];

// is the property owner of the sequence?
        if(s->getOwner() == p)
        {
          m_seqNameIdx.erase(s->getName());
          m_seqIdIdx.erase(s->getId());
          m_sequences.erase(m_sequences.begin() + pos);
          delete s;
          m_seqFTIdx.erase(it);
        }
      }

      return;
    }
  }

}

te::da::DataSourceCatalog::DataSourceCatalog(const DataSourceCatalog& /*rhs*/)
{
  ////////IMPLEMENTAR
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
       (getDataSetTypeById(parent->getId()) == parent))
        return;
  }

  throw Exception(TR_DATAACCESS("Could not find in the catalog the DataSetType that owns the given sequence!"));
}

void te::da::DataSourceCatalog::indexSequenceDependency(Sequence* s)
{
  if(s->getOwner() == 0)
    return;

  te::dt::Property* parent = s->getOwner();
  std::size_t pos = getSequencePos(s);

  while(parent)
  {
    parent = parent->getParent();

// if the parent is a FetaureType and it is in the catalog, make index and stop!
    if(parent &&
       (parent->getType() == te::dt::DATASET_TYPE) &&
       (getDataSetTypeById(parent->getId()) == parent))
      {
        te::da::DataSetType* dt = static_cast<te::da::DataSetType*>(parent);        
        m_seqFTIdx.insert(std::pair<DataSetType*, std::size_t>(dt, pos));
        return;
      }
  }

  throw Exception(TR_DATAACCESS("Could not find in the catalog the DataSetType that owns the given sequence!"));
}

void te::da::DataSourceCatalog::dropDependentSequences(DataSetType* dt)
{
  std::pair<std::multimap<DataSetType*, std::size_t>::iterator,
            std::multimap<DataSetType*, std::size_t>::iterator> range = m_seqFTIdx.equal_range(dt);

  while(range.first != range.second)
  {
    std::multimap<DataSetType*, std::size_t>::iterator it = range.first;
    ++range.first;

// remove sequence and all its entry in the catalog
// !! DON'T CALL remove(s) THIS WILL NEED EXTRA COMPUTATIONS AND ALSO WILL NEED TO CHANGE THE LOGIC OF CODE BLOCK!!
    std::size_t pos = it->second;
    te::da::Sequence* s = m_sequences[pos];
    m_seqNameIdx.erase(s->getName());
    m_seqIdIdx.erase(s->getId());
    m_sequences.erase(m_sequences.begin() + pos);
    delete s;
    m_seqFTIdx.erase(it);
  }
}

void te::da::DataSourceCatalog::dropDependentSequenceEntry(Sequence* s)
{
   if(s->getOwner() == 0)
    return;

  te::dt::Property* parent = s->getOwner();
  std::size_t pos = getSequencePos(s);

  while(parent)
  {
    parent = parent->getParent();

// if the parent is a FetaureType and it is in the catalog, make index and stop!
    if(parent &&
       (parent->getType() == te::dt::DATASET_TYPE) &&
       (getDataSetTypeById(parent->getId()) == parent))
      {
        te::da::DataSetType* dt = static_cast<te::da::DataSetType*>(parent);        

        std::pair<std::multimap<DataSetType*, std::size_t>::iterator,
                  std::multimap<DataSetType*, std::size_t>::iterator> range = m_seqFTIdx.equal_range(dt);

        while(range.first != range.second)
        {
          std::multimap<DataSetType*, std::size_t>::iterator it = range.first;
          ++range.first;

          if(it->second == pos)
          {
            m_seqFTIdx.erase(it);
            return;
          }
        }
      }
  }

  throw Exception(TR_DATAACCESS("Could not find in the catalog the DataSetType that owns the given sequence in order to remove the sequence entry!"));
}

void te::da::DataSourceCatalog::checkFKsDependency(DataSetType* dt) const
{
  std::size_t size = dt->getNumberOfForeignKeys();

  for(std::size_t i = 0; i < size; ++i)
  {
    te::da::ForeignKey* fk = dt->getForeignKey(i);
    te::da::DataSetType* refFT = fk->getReferencedDataSetType();

    if((refFT == 0) || (getDataSetTypeById(refFT->getId()) != refFT))
      throw Exception(TR_DATAACCESS("There is a foreign key in the DataSetType referencing another DataSetType that is not in the catalog!"));
  }
}

void te::da::DataSourceCatalog::indexFKs(DataSetType* dt)
{
  std::size_t size = dt->getNumberOfForeignKeys();

  for(std::size_t i = 0; i < size; ++i)
  {
    te::da::ForeignKey* fk = dt->getForeignKey(i);
    te::da::DataSetType* refFT = fk->getReferencedDataSetType();
    
    assert(getDataSetTypeById(refFT->getId()) == refFT);

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


