/*  Copyright (C) 2010-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/stmemory/DataSet.cpp

  \brief  Implementation of a data set in memory that contains spatiotemporal observations indexed 
          by time and space.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/ByteArray.h"
#include "../datatype/DataType.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../datatype/StringProperty.h"
#include "../datatype/Utils.h"
#include "../datatype/DateTimeUtils.h"
#include "../datatype/DateTimeInstant.h"
#include "../datatype/DateTimePeriod.h"
#include "../datatype/DateTime.h"
#include "../datatype/Enums.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"
#include "../memory/DataSetItem.h"
#include "../sam/rtree/Index.h"

#include "DataSet.h"
#include "Exception.h"

// STL
#include <limits>

te::stmem::DataSet::DataSet(const te::da::DataSetType* type, int tpPropIdx)
  : m_items(),
    m_RTree(),  
    m_iterator(),
    m_beforeFirst(true),  
    m_pnames(),
    m_ptypes(),
    m_begTimePropIdx(tpPropIdx),
    m_endTimePropIdx(-1),
    m_geomPropIdx(-1)
{
  te::da::GetPropertyInfo(type, m_pnames, m_ptypes);

  // Fill char-encodings
  for(std::size_t i = 0; i < type->size(); ++i)
  {
    te::dt::StringProperty* p = dynamic_cast<te::dt::StringProperty*>(type->getProperty(i));
    if(p != 0)
      m_encodings[i] = p->getCharEncoding();
  }

  //create the internal empty RTree 
  m_RTree.reset(new te::sam::rtree::Index<te::mem::DataSetItem*>());
}

te::stmem::DataSet::DataSet(const te::da::DataSetType* type, int tpPropIdx, int gmPropIdx)
  : m_items(),
    m_RTree(),  
    m_iterator(),
    m_beforeFirst(true),  
    m_pnames(),
    m_ptypes(),
    m_begTimePropIdx(tpPropIdx),
    m_endTimePropIdx(-1),
    m_geomPropIdx(gmPropIdx)
{
  te::da::GetPropertyInfo(type, m_pnames, m_ptypes);

  // Fill char-encodings
  for(std::size_t i = 0; i < type->size(); ++i)
  {
    te::dt::StringProperty* p = dynamic_cast<te::dt::StringProperty*>(type->getProperty(i));
    if(p != 0)
      m_encodings[i] = p->getCharEncoding();
  }

  //create the internal empty RTree 
  m_RTree.reset(new te::sam::rtree::Index<te::mem::DataSetItem*>());
}

te::stmem::DataSet::DataSet( const te::da::DataSetType* type, int begTimePropIdx, 
                             int endTimePropIdx, int gmPropIdx)
  : m_items(),
    m_RTree(),  
    m_iterator(),
    m_beforeFirst(true),  
    m_pnames(),
    m_ptypes(),
    m_begTimePropIdx(begTimePropIdx),
    m_endTimePropIdx(endTimePropIdx),
    m_geomPropIdx(gmPropIdx)
{
  te::da::GetPropertyInfo(type, m_pnames, m_ptypes);

  // Fill char-encodings
  for(std::size_t i = 0; i < type->size(); ++i)
  {
    te::dt::StringProperty* p = dynamic_cast<te::dt::StringProperty*>(type->getProperty(i));
    if(p != 0)
      m_encodings[i] = p->getCharEncoding();
  }

  //create the internal empty RTree 
  m_RTree.reset(new te::sam::rtree::Index<te::mem::DataSetItem*>());
}

te::stmem::DataSet::DataSet(const std::vector<std::string>& pnames, const std::vector<int>& ptypes, const std::map<int, te::common::CharEncoding>& encodings,
                            int begTimePropIdx, int endTimePropIdx, int gmPropIdx)
  : m_items(),
    m_RTree(),  
    m_iterator(),
    m_beforeFirst(true),  
    m_pnames(pnames),
    m_ptypes(ptypes),
    m_encodings(encodings),
    m_begTimePropIdx(begTimePropIdx),
    m_endTimePropIdx(endTimePropIdx),
    m_geomPropIdx(gmPropIdx)
{
  //create the internal empty RTree 
  m_RTree.reset(new te::sam::rtree::Index<te::mem::DataSetItem*>());
}

te::stmem::DataSet::DataSet(te::da::DataSet* ds, int tpPropIdx, int gmPropIdx, unsigned int limit)
  : m_items(),
    m_RTree(),  
    m_iterator(),
    m_beforeFirst(true),  
    m_pnames(),
    m_ptypes(),
    m_begTimePropIdx(tpPropIdx),
    m_endTimePropIdx(-1),
    m_geomPropIdx(gmPropIdx)
{
  te::da::GetPropertyInfo(ds, m_pnames, m_ptypes);

  // Fill char-encodings
  for(std::size_t i = 0; i < m_ptypes.size(); ++i)
    if(m_ptypes[i] == te::dt::STRING_TYPE)
      m_encodings[i] = ds->getPropertyCharEncoding(i);
  
  //create the internal empty RTree 
  m_RTree.reset(new te::sam::rtree::Index<te::mem::DataSetItem*>());
  
  //copy the items
  copy(ds, limit);
}

te::stmem::DataSet::DataSet(te::da::DataSet* ds, int begTimePropIdx, int endTimePropIdx, int gmPropIdx, unsigned int limit)
  : m_items(),
    m_RTree(),  
    m_iterator(),
    m_beforeFirst(true),  
    m_pnames(),
    m_ptypes(),
    m_begTimePropIdx(begTimePropIdx),
    m_endTimePropIdx(endTimePropIdx),
    m_geomPropIdx(gmPropIdx)
{
  te::da::GetPropertyInfo(ds, m_pnames, m_ptypes);

  // Fill char-encodings
  for(std::size_t i = 0; i < m_ptypes.size(); ++i)
    if(m_ptypes[i] == te::dt::STRING_TYPE)
      m_encodings[i] = ds->getPropertyCharEncoding(i);
  
  //create the internal empty RTree 
  m_RTree.reset(new te::sam::rtree::Index<te::mem::DataSetItem*>());
  
  //copy the items
  copy(ds, limit);
}

te::stmem::DataSet::DataSet(const DataSet& rhs, const bool deepCopy)
  : m_items(),
    m_RTree(),  
    m_iterator(),
    m_beforeFirst(true),  
    m_pnames(rhs.m_pnames),
    m_ptypes(rhs.m_ptypes),
    m_encodings(rhs.m_encodings),
    m_begTimePropIdx(rhs.m_begTimePropIdx),
    m_endTimePropIdx(rhs.m_endTimePropIdx),
    m_geomPropIdx(rhs.m_geomPropIdx)
{
  //create the internal empty RTree 
  m_RTree.reset(new te::sam::rtree::Index<te::mem::DataSetItem*>());

  TimeToDataSetItemMap::const_iterator it = rhs.m_items.begin();
  while(it != rhs.m_items.end())
  {
    if(deepCopy)
      //clone the DataSetItem
      add(it->second->clone().release()); //add to m_items and m_RTree
    else
      add(*it); //add to m_items and m_RTree

    ++it;
  }
}

te::stmem::DataSet& te::stmem::DataSet::operator= (const te::stmem::DataSet& other)
{
  if (this != &other) 
  {
    m_RTree.reset(new te::sam::rtree::Index<te::mem::DataSetItem*>());
    m_beforeFirst = true;
    m_pnames = other.m_pnames;
    m_ptypes = other.m_ptypes;
    m_encodings = other.m_encodings;
    m_begTimePropIdx = other.m_begTimePropIdx;
    m_endTimePropIdx = other.m_endTimePropIdx;
    m_geomPropIdx = other.m_geomPropIdx;
    
    m_items.clear();
    TimeToDataSetItemMap::const_iterator it = other.m_items.begin();
    while(it != other.m_items.end())
    {
      add(*it); //add to m_items and m_RTree
      ++it;
    }   
  }
  return *this;
}

te::stmem::DataSet::~DataSet()
{
  //TO DO: verificar se está OK!!!
  TimeToDataSetItemMap::iterator it = m_items.begin();
  while(it!=m_items.end())
  {
    delete it->first; //Só apaga os DateTime
    ++it;
  }
  m_items.clear();
}

void te::stmem::DataSet::copy(te::da::DataSet* src, unsigned int limit)
{
  std::vector<std::size_t> properties;

  const std::size_t np = src->getNumProperties();

  for(std::size_t i = 0; i != np; ++i)
    properties.push_back(i);

  copy(src, properties, limit);
}

void te::stmem::DataSet::copy(te::da::DataSet* src, const std::vector<std::size_t>& properties, unsigned int limit)
{
  bool unlimited = true;

  if(limit == 0)
     limit = std::numeric_limits<std::size_t>::max();
  else
    unlimited = false;
  
  unsigned int i = 0;
  
  do
  {
    std::auto_ptr<te::mem::DataSetItem> item(new te::mem::DataSetItem(this));
       
    for(std::size_t c = 0; c < properties.size(); ++c)
    {
      if(!src->isNull(properties[c]))
        item->setValue(properties[c], src->getValue(properties[c]).release());
      else
        item->setValue(properties[c], 0);
    }

    add(item.release()); 
    
    ++i;
   //TODO: Verificar se ele está decrementando o apontador para "item" quando sai do escopo!!!
  } while(src->moveNext() && (i < limit));

  if(!unlimited & (i < limit))
    throw Exception("The source dataset has few items than requested copy limit!");
}

const te::stmem::TimeToDataSetItemMap& te::stmem::DataSet::getData() const
{
  return m_items;
}

int te::stmem::DataSet::getGeomPropIdx() const
{
  return m_geomPropIdx; 
}

int te::stmem::DataSet::getBeginTimePropIdx() const
{
  return m_begTimePropIdx; 
}

int te::stmem::DataSet::getEndTimePropIdx() const
{
  return m_endTimePropIdx; 
}

void te::stmem::DataSet::add(const std::pair<te::dt::DateTime*, DateSetItemShrPtr>& p)
{
  te::dt::DateTime* dt = static_cast<te::dt::DateTime*>(p.first->clone());
  m_items.insert(std::pair<te::dt::DateTime*, DateSetItemShrPtr>(dt,p.second)); 
   
  //insert into the RTree
  if(m_geomPropIdx<0)
    return;

  std::auto_ptr<te::gm::Geometry> geom(p.second->getGeometry(m_geomPropIdx));
  if(geom.get()!=0)
  {
    const te::gm::Envelope* env = geom->getMBR();
    m_RTree->insert(*env, p.second.get());
  }     
  return; 
}

void te::stmem::DataSet::add(te::mem::DataSetItem* dsItem)
{
  DateSetItemShrPtr item(dsItem);

  //Get the phenomemon properties
  std::auto_ptr<te::dt::DateTime> phBegTime;
  if(m_begTimePropIdx>=0)
    phBegTime = item->getDateTime(m_begTimePropIdx);

  std::auto_ptr<te::dt::DateTime> phEndTime;
  if(m_endTimePropIdx>=0)
    phEndTime = item->getDateTime(m_endTimePropIdx);
  
  //the phenomenon time is an instant
  if(phBegTime.get()!=0 && phEndTime.get()==0)
  {
    std::pair<te::dt::DateTime*, DateSetItemShrPtr> p(phBegTime.get(),item);
    add(p); 
  }
  //the phenomenon time is a period. We have to build a period.
  else if(phBegTime.get()!=0 && phEndTime.get()!=0)
  {
    std::auto_ptr<te::dt::DateTime> aux (te::dt::GetTimePeriod(static_cast<te::dt::DateTimeInstant*>(phBegTime.get()),
                                         static_cast<te::dt::DateTimeInstant*>(phEndTime.get())));
    std::pair<te::dt::DateTime*, DateSetItemShrPtr> p(aux.get(),item);
    add(p);
  }   
 
  return;
}

std::auto_ptr<te::dt::DateTimePeriod> te::stmem::DataSet::getTemporalExtent() const
{
  const te::dt::DateTime* tbegin =  m_items.begin()->first;
  const te::dt::DateTime* tend =  m_items.rbegin()->first;
  return std::auto_ptr<te::dt::DateTimePeriod>(te::dt::GetTemporalExtent(tbegin, tend));
}

std::auto_ptr<te::stmem::DataSet>  te::stmem::DataSet::clone() const
{
  return std::auto_ptr<DataSet>(new te::stmem::DataSet(*this, true));
}

std::auto_ptr<te::stmem::DataSet> te::stmem::DataSet::filter(const te::gm::Envelope* e, te::gm::SpatialRelation r) const
{
  if(r!=te::gm::INTERSECTS)
    return std::auto_ptr<te::stmem::DataSet>(0);
  
  std::vector<te::mem::DataSetItem*> report;
  m_RTree->search(*e, report);

  std::auto_ptr<te::stmem::DataSet> result(new DataSet(m_pnames, m_ptypes, m_encodings, m_begTimePropIdx, m_endTimePropIdx, m_geomPropIdx));

  for(unsigned int i=0; i<report.size(); ++i)
    result->add(report[i]->clone().release());
    
  return result;
}

std::auto_ptr<te::stmem::DataSet> te::stmem::DataSet::filter(const te::gm::Geometry* g, te::gm::SpatialRelation r) const
{
  if(r!=te::gm::INTERSECTS)
    return std::auto_ptr<te::stmem::DataSet>(0);
  
  std::vector<te::mem::DataSetItem*> report;
  m_RTree->search(*g->getMBR(), report);

  std::auto_ptr<te::stmem::DataSet> result(new DataSet(m_pnames, m_ptypes, m_encodings, m_begTimePropIdx, m_endTimePropIdx, m_geomPropIdx));

  for(unsigned int i=0; i<report.size(); ++i)
  {
    std::auto_ptr<te::gm::Geometry> geom(report[i]->getGeometry(m_geomPropIdx));
    if(geom->intersects(g))
      result->add(report[i]->clone().release());
  }
  return result;
}

std::auto_ptr<te::stmem::DataSet> te::stmem::DataSet::filter(const te::dt::DateTime* dt, te::dt::TemporalRelation r) const
{
  TimeToDataSetItemMap::const_iterator itb = m_items.end();
  TimeToDataSetItemMap::const_iterator ite = m_items.end();

  std::auto_ptr<te::stmem::DataSet> result(new DataSet(m_pnames, m_ptypes, m_encodings, m_begTimePropIdx, m_endTimePropIdx, m_geomPropIdx));
  std::auto_ptr<te::dt::DateTime> dtaux(static_cast<te::dt::DateTime*>(dt->clone()));

  if(r==te::dt::AFTER) //2
  {
    itb = m_items.upper_bound(dtaux.get());
  }
  else if(r==(te::dt::AFTER | te::dt::EQUALS)) // 2 OU 8 = 10 
  {
    itb = m_items.find(dtaux.get());
    if(itb==m_items.end())
      itb = m_items.upper_bound(dtaux.get());
  }
  else if(r==te::dt::BEFORE) // 1
  {
    itb = m_items.begin();
    ite = m_items.find(dtaux.get());
    if(ite==m_items.end())
      ite = m_items.upper_bound(dtaux.get());
  }
  else if(r==(te::dt::BEFORE | te::dt::EQUALS)) // 1 OU 8 = 9
  {
    itb = m_items.begin();
    ite = m_items.upper_bound(dtaux.get());   
  }
  else if(r==te::dt::DURING) //4
  {
    std::auto_ptr<te::dt::DateTime> t1(static_cast<te::dt::DateTimePeriod*>(dtaux.get())->getInitialInstant());
    std::auto_ptr<te::dt::DateTime> t2(static_cast<te::dt::DateTimePeriod*>(dtaux.get())->getFinalInstant());
    itb = m_items.find(t1.get());
    if(itb==m_items.end())
      itb = m_items.upper_bound(t1.get());
    ite = m_items.upper_bound(t2.get()); 
  }
  else if(r==te::dt::EQUALS) //8
  {
    std::pair<TimeToDataSetItemMap::const_iterator, TimeToDataSetItemMap::const_iterator> itPair;
    itPair = m_items.equal_range(dtaux.get());
    itb = itPair.first;
    ite = itPair.second;
  }

  while(itb!=ite)
  {
    result->add(*itb);
    ++itb;
  } 
  return result;
}

std::auto_ptr<te::stmem::DataSet> te::stmem::DataSet::filter(const te::gm::Envelope* e, te::gm::SpatialRelation r, 
                                      const te::dt::DateTime* dt, te::dt::TemporalRelation tr) const
{
  std::auto_ptr<te::stmem::DataSet> result1 = filter(e,r);
  return result1->filter(dt, tr);
}

std::auto_ptr<te::stmem::DataSet> te::stmem::DataSet::filter(const te::gm::Geometry* g, te::gm::SpatialRelation r, 
                                      const te::dt::DateTime* dt, te::dt::TemporalRelation tr) const
{
  std::auto_ptr<te::stmem::DataSet> result1 = filter(g,r);
  return result1->filter(dt, tr);
}

std::auto_ptr<te::stmem::DataSet> te::stmem::DataSet::nearestObservations(const te::dt::DateTime* time, int n) const
{
  TimeToDataSetItemMap::const_iterator itlower, itupper, itbegin, itend;
  itbegin = m_items.begin();
  itend = m_items.end();
  std::auto_ptr<te::dt::DateTime> dtaux(static_cast<te::dt::DateTime*>(time->clone()));
  itlower = m_items.lower_bound(dtaux.get());
  itupper = m_items.upper_bound(dtaux.get());
  
  //We get n observations upper and n observations lower with their distance
  std::multimap<long, TimeToDataSetItemMap::const_iterator> result;
  int numObs = 0;
  while(itlower!=itbegin && itlower!=itend && numObs<n)
  {
    long dist = GetDistance(itlower->first, time);
    result.insert(std::pair<long, TimeToDataSetItemMap::const_iterator>(dist, itlower));
    --itlower;
    ++numObs;
  }
  
  numObs = 0;
  while(itupper!=itend && numObs<n)
  {
    long dist = GetDistance(itlower->first, time);
    result.insert(std::pair<long, TimeToDataSetItemMap::const_iterator>(dist, itlower));
    --itlower;
    ++numObs;
  }
  
  //After that, we get the first n observation of the result.
  std::auto_ptr<te::stmem::DataSet> ds(new DataSet(m_pnames, m_ptypes, m_encodings, m_begTimePropIdx, m_endTimePropIdx, m_geomPropIdx));
  std::multimap<long, TimeToDataSetItemMap::const_iterator>::iterator it = result.begin();
  numObs = 0;
  while(it != result.end() && numObs<n)
  {
    ds->add(*it->second);
    ++it;
    ++numObs;
  }
  
  return ds; 
}

te::common::TraverseType te::stmem::DataSet::getTraverseType() const 
{ 
  return te::common::BIDIRECTIONAL;
}

te::common::AccessPolicy te::stmem::DataSet::getAccessPolicy() const 
{
  return te::common::RWAccess;
}

std::size_t te::stmem::DataSet::getNumProperties() const
{
  return m_pnames.size();
}

int te::stmem::DataSet::getPropertyDataType(std::size_t pos) const
{
  return m_ptypes[pos];
}

std::string te::stmem::DataSet::getPropertyName(std::size_t pos) const
{
  return m_pnames[pos];
}

te::common::CharEncoding te::stmem::DataSet::getPropertyCharEncoding(std::size_t i) const
{
  std::map<int, te::common::CharEncoding>::const_iterator it = m_encodings.find(i);
  assert(it != m_encodings.end());

  return it->second;
}

std::string te::stmem::DataSet::getDatasetNameOfProperty(std::size_t /*pos*/) const
{
  return "";
}

bool te::stmem::DataSet::isEmpty() const
{
  return m_items.empty();
}

bool te::stmem::DataSet::isConnected() const
{
  return false;  
}

std::size_t te::stmem::DataSet::size() const
{
  return m_items.size();
}

std::auto_ptr<te::gm::Envelope> te::stmem::DataSet::getExtent(std::size_t i)
{
  if(i==m_geomPropIdx)
    return std::auto_ptr<te::gm::Envelope>(new te::gm::Envelope(m_RTree->getMBR()));
  
  std::auto_ptr<te::gm::Envelope> mbr(new te::gm::Envelope);

  TimeToDataSetItemMap::const_iterator it = m_items.begin();
  while(it != m_items.end())
  {
    std::auto_ptr<te::gm::Geometry> geom(it->second->getGeometry(i));
    mbr->Union(*(geom->getMBR()));
    ++it; 
  }

  return mbr;  
}

bool te::stmem::DataSet::moveNext() 
{
  if(m_beforeFirst)
    return moveFirst();
  m_iterator++;
  return m_iterator != m_items.end();
}

bool te::stmem::DataSet::movePrevious() 
{
  if(m_beforeFirst || m_iterator==m_items.begin())
    return false;
  --m_iterator;
  return true;
}

bool te::stmem::DataSet::moveFirst() 
{
  m_iterator = m_items.begin();
  m_beforeFirst = false;
  return m_iterator!=m_items.end();
}

bool te::stmem::DataSet::moveBeforeFirst()
{
  m_beforeFirst=true;
  return true;
}

bool te::stmem::DataSet::moveLast() 
{
  m_iterator = m_items.end();
  --m_iterator;
  m_beforeFirst=false;
  return true;
}

bool te::stmem::DataSet::move(std::size_t i)
{
  if(i>=m_items.size())
    return false;

  m_iterator = m_items.begin();
  std::size_t c = 0;
  while(m_iterator!=m_items.end() && c < i)
  {
    ++m_iterator;
    ++c;
  }
  return true;
}

bool te::stmem::DataSet::isAtBegin() const
{
  return m_iterator == m_items.begin();
}

bool te::stmem::DataSet::isBeforeBegin() const
{
  return m_beforeFirst;
}

bool te::stmem::DataSet::isAtEnd() const
{
  TimeToDataSetItemMap::const_iterator aux = m_items.end();
  --aux;
  return m_iterator==aux;
}

bool te::stmem::DataSet::isAfterEnd() const
{
  return m_iterator == m_items.end();
}

char te::stmem::DataSet::getChar(std::size_t i) const
{
  return m_iterator->second->getChar(i);
}

void te::stmem::DataSet::setChar(std::size_t i, char value)
{
  m_iterator->second->setChar(i, value);
}

void te::stmem::DataSet::setChar(const std::string& name, char value) 
{
  m_iterator->second->setChar(name, value);
}

unsigned char te::stmem::DataSet::getUChar(std::size_t i) const
{
  return m_iterator->second->getUChar(i);
}

void te::stmem::DataSet::setUChar(std::size_t i, unsigned char value)
{
  m_iterator->second->setUChar(i, value);
}

void te::stmem::DataSet::setUChar(const std::string& name, unsigned char value)
{
  m_iterator->second->setUChar(name, value);
}

boost::int16_t te::stmem::DataSet::getInt16(std::size_t i) const
{
  return m_iterator->second->getInt16(i);
}

void te::stmem::DataSet::setInt16(std::size_t i, boost::int16_t value)
{
  m_iterator->second->setInt16(i, value);
}

void te::stmem::DataSet::setInt16(const std::string& name, boost::int16_t value)
{
  m_iterator->second->setInt16(name, value);
}

boost::int32_t te::stmem::DataSet::getInt32(std::size_t i) const
{
  return m_iterator->second->getInt32(i);
}

void te::stmem::DataSet::setInt32(std::size_t i, boost::int32_t value)
{
  m_iterator->second->setInt32(i, value);
}

void te::stmem::DataSet::setInt32(const std::string& name, boost::int32_t value)
{
  m_iterator->second->setInt32(name, value);
}

boost::int64_t te::stmem::DataSet::getInt64(std::size_t i) const
{
  return m_iterator->second->getInt64(i);
}

void te::stmem::DataSet::setInt64(std::size_t i, boost::int64_t value)
{
  m_iterator->second->setInt64(i, value);
}

void te::stmem::DataSet::setInt64(const std::string& name, boost::int64_t value) 
{
  m_iterator->second->setInt64(name, value);
}

bool te::stmem::DataSet::getBool(std::size_t i) const
{
  return m_iterator->second->getBool(i);
}

void te::stmem::DataSet::setBool(std::size_t i, bool value)
{
  m_iterator->second->setBool(i, value);
}

void te::stmem::DataSet::setBool(const std::string& name, bool value)
{
  m_iterator->second->setBool(name, value);
}

float te::stmem::DataSet::getFloat(std::size_t i) const
{
  return m_iterator->second->getFloat(i);
}

void te::stmem::DataSet::setFloat(std::size_t i, float value)
{
  m_iterator->second->setFloat(i, value);
}

double te::stmem::DataSet::getDouble(std::size_t i) const
{
  return m_iterator->second->getDouble(i);
}

void te::stmem::DataSet::setDouble(std::size_t i, double value)
{
  m_iterator->second->setDouble(i, value);
}

void te::stmem::DataSet::setDouble(const std::string& name, double value)
{
  m_iterator->second->setDouble(name, value);
}

std::string te::stmem::DataSet::getNumeric(std::size_t i) const
{
  return m_iterator->second->getNumeric(i);
}

void te::stmem::DataSet::setNumeric(std::size_t i, const std::string& value)
{
  m_iterator->second->setNumeric(i, value);
}

void te::stmem::DataSet::setNumeric(const std::string& name, const std::string& value) 
{
  m_iterator->second->setNumeric(name, value);
}

std::string te::stmem::DataSet::getString(std::size_t i) const
{
  return m_iterator->second->getString(i);
}

void te::stmem::DataSet::setString(std::size_t i, const std::string& value) 
{
  m_iterator->second->setString(i, value);
}

void te::stmem::DataSet::setString(const std::string& name, const std::string& value) 
{
  m_iterator->second->setString(name, value);
}

std::auto_ptr<te::dt::ByteArray> te::stmem::DataSet::getByteArray(std::size_t i) const
{
  return std::auto_ptr<te::dt::ByteArray>(m_iterator->second->getByteArray(i));
}

void te::stmem::DataSet::setByteArray(std::size_t i, te::dt::ByteArray* value)
{
  m_iterator->second->setByteArray(i, value);
}

void te::stmem::DataSet::setByteArray(const std::string& name, te::dt::ByteArray* value) 
{
  m_iterator->second->setByteArray(name, value);
}

std::auto_ptr<te::gm::Geometry> te::stmem::DataSet::getGeometry(std::size_t i) const
{
  return m_iterator->second->getGeometry(i);
}

void te::stmem::DataSet::setGeometry(std::size_t i, te::gm::Geometry* value)
{
  m_iterator->second->setGeometry(i, value);
}

void te::stmem::DataSet::setGeometry(const std::string& name, te::gm::Geometry* value) 
{
  m_iterator->second->setGeometry(name, value);
}

std::auto_ptr<te::rst::Raster> te::stmem::DataSet::getRaster(std::size_t i) const
{
  return m_iterator->second->getRaster(i);
}

void te::stmem::DataSet::setRaster(std::size_t i, te::rst::Raster* value)
{
  m_iterator->second->setRaster(i, value);
}

void te::stmem::DataSet::setRaster(const std::string& name, te::rst::Raster* value)
{
  m_iterator->second->setRaster(name, value);
}

std::auto_ptr<te::dt::DateTime> te::stmem::DataSet::getDateTime(std::size_t i) const
{
  return m_iterator->second->getDateTime(i);
}

void te::stmem::DataSet::setDateTime(std::size_t i, te::dt::DateTime* value) 
{
  m_iterator->second->setDateTime(i, value);
}

void te::stmem::DataSet::setDateTime(const std::string& name, te::dt::DateTime* value) 
{
  m_iterator->second->setDateTime(name, value);
}

std::auto_ptr<te::dt::Array> te::stmem::DataSet::getArray(std::size_t i) const
{
  return std::auto_ptr<te::dt::Array>(0);
}

std::auto_ptr<te::dt::AbstractData> te::stmem::DataSet::getValue(std::size_t i) const
{
  return m_iterator->second->getValue(i);
}

void te::stmem::DataSet::setValue(std::size_t i, te::dt::AbstractData* ad)
{
  m_iterator->second->setValue(i, ad);
}

void te::stmem::DataSet::setValue(const std::string& name, te::dt::AbstractData* ad)
{
  m_iterator->second->setValue(name, ad);
}

bool te::stmem::DataSet::isNull(std::size_t i) const
{
  return m_iterator->second->isNull(i);
}






