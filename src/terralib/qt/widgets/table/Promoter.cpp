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
#include "Promoter.h"

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../common/progress/TaskProgress.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../datatype/SimpleData.h"
#include "../../../dataaccess/utils/Utils.h"

// Qt
#include <QObject>

enum COMPARISON
{
  LESSER,
  EQUAL,
  GREATER
};

COMPARISON CompareStr (const std::string& lhs, const std::string& rhs)
{
  int aux = lhs.compare(rhs);

  COMPARISON comp = (aux > 0) ? GREATER : (aux < 0) ? LESSER : EQUAL;

  return comp;
}

template <typename T>
COMPARISON CompareSimpleData(const T& lhs, const T& rhs)
{
  COMPARISON res;

  if(lhs.getValue() > rhs.getValue())
    res = GREATER;
  else if(lhs.getValue() < rhs.getValue())
    res = LESSER;
  else
    res = EQUAL;

  return res;
}

COMPARISON CompareAbsData (te::dt::AbstractData* lhs, te::dt::AbstractData* rhs)
{
  if(lhs == 0)
    return GREATER;
  if(rhs == 0)
    return LESSER;

  COMPARISON res = EQUAL;

  if(lhs->getTypeCode() == te::dt::CHAR_TYPE)
    res = CompareSimpleData<te::dt::Char>(*((te::dt::Char*)lhs), *((te::dt::Char*)rhs));
  else if(lhs->getTypeCode() == te::dt::UCHAR_TYPE)
    res = CompareSimpleData<te::dt::UChar>(*((te::dt::UChar*)lhs), *((te::dt::UChar*)rhs));
  else if(lhs->getTypeCode() == te::dt::INT16_TYPE)
    res = CompareSimpleData<te::dt::Int16>(*((te::dt::Int16*)lhs), *((te::dt::Int16*)rhs));
  else if(lhs->getTypeCode() == te::dt::UINT16_TYPE)
    res = CompareSimpleData<te::dt::UInt16>(*((te::dt::UInt16*)lhs), *((te::dt::UInt16*)rhs));
  else if(lhs->getTypeCode() == te::dt::INT32_TYPE)
    res = CompareSimpleData<te::dt::Int32>(*((te::dt::Int32*)lhs), *((te::dt::Int32*)rhs));
  else if(lhs->getTypeCode() == te::dt::UINT32_TYPE)
    res = CompareSimpleData<te::dt::UInt32>(*((te::dt::UInt32*)lhs), *((te::dt::UInt32*)rhs));
  else if(lhs->getTypeCode() == te::dt::INT64_TYPE)
    res = CompareSimpleData<te::dt::Int64>(*((te::dt::Int64*)lhs), *((te::dt::Int64*)rhs));
  else if(lhs->getTypeCode() == te::dt::UINT64_TYPE)
    res = CompareSimpleData<te::dt::UInt64>(*((te::dt::UInt64*)lhs), *((te::dt::UInt64*)rhs));
  else if(lhs->getTypeCode() == te::dt::BOOLEAN_TYPE)
    res = CompareSimpleData<te::dt::Boolean>(*((te::dt::Boolean*)lhs), *((te::dt::Boolean*)rhs));
  else if(lhs->getTypeCode() == te::dt::FLOAT_TYPE)
    res = CompareSimpleData<te::dt::Float>(*((te::dt::Float*)lhs), *((te::dt::Float*)rhs));
  else if(lhs->getTypeCode() == te::dt::DOUBLE_TYPE)
    res = CompareSimpleData<te::dt::Double>(*((te::dt::Double*)lhs), *((te::dt::Double*)rhs));
  else if(lhs->getTypeCode() == te::dt::STRING_TYPE)
    res = CompareStr(((te::dt::String*)lhs)->getValue(), ((te::dt::String*) rhs)->getValue()); 

  return res;
}

struct DataComparator
{
  bool operator() (const std::vector<te::dt::AbstractData*>& lhs, const std::vector<te::dt::AbstractData*>& rhs)
  {
    for (size_t i=0; i<lhs.size(); i++)
    {
      COMPARISON res = CompareAbsData(lhs[i], rhs[i]);

      if(res == LESSER)
        return true;
      else if (res == GREATER)
        return false;
    }

    return false;
  }
};

std::vector<std::string> GetColumnsNames(te::da::DataSet* dset, const std::vector<size_t>& colsPositions)
{
  std::vector<std::string> res;
  std::vector<size_t>::const_iterator it;

  for(it=colsPositions.begin(); it!=colsPositions.end(); ++it)
    res.push_back(dset->getPropertyName(*it));

  return res;
}

void CleanAbstractData(std::multimap<std::vector<te::dt::AbstractData*>, int, DataComparator>& d)
{
  std::multimap<std::vector<te::dt::AbstractData*>, int, DataComparator>::iterator it;
    
  for(it=d.begin(); it!=d.end(); ++it)
    te::common::FreeContents(it->first);

  d.clear();
}

te::qt::widgets::Promoter::Promoter() 
{
}

te::qt::widgets::Promoter::~Promoter()
{
  cleanPreproccessKeys();
}

void te::qt::widgets::Promoter::resetPromotion()
{
  for(size_t i=0; i<m_logicalRows.size(); i++)
    m_logicalRows[i] = i;
}

void te::qt::widgets::Promoter::cleanLogRowsAndProcessKeys()
{
  cleanPreproccessKeys();

  m_logicalRows.clear();
}

void te::qt::widgets::Promoter::preProcessKeys(te::da::DataSet* dset, const std::vector<size_t>& pkeys)
{
  if(!m_PkeysRows.empty())
    return;

  size_t setSize = dset->size();
  std::vector<std::string> colsNames = GetColumnsNames(dset, pkeys);

  cleanPreproccessKeys();

  if(m_logicalRows.empty())
    m_logicalRows.resize(setSize);

  dset->moveFirst();

  te::common::TaskProgress task(QObject::tr("Executing promotion...").toStdString(), te::common::TaskProgress::UNDEFINED, m_logicalRows.size());

  for(size_t i=0; i<m_logicalRows.size(); i++)
  {
    if(!task.isActive())
    {
      cleanLogRowsAndProcessKeys();
      return;
    }

    te::da::ObjectId* obj = te::da::GenerateOID(dset, colsNames);
    
    m_PkeysRows[obj] = i;
    m_logicalRows[i] = i;

    dset->moveNext();

    task.pulse();
  }
}

size_t te::qt::widgets::Promoter::getLogicalRow(const size_t& visualRow)
{
  return (m_logicalRows.empty()) ? visualRow : m_logicalRows[visualRow];
}

void te::qt::widgets::Promoter::cleanPreproccessKeys()
{
  std::map<te::da::ObjectId*, size_t, te::common::LessCmp<te::da::ObjectId*> >::iterator it;

  for(it=m_PkeysRows.begin(); it!=m_PkeysRows.end(); ++it)
    delete it->first;

  m_PkeysRows.clear();
}

void te::qt::widgets::Promoter::promote(const te::da::ObjectIdSet* oids)
{
  if(m_logicalRows.empty() || m_PkeysRows.empty())
    return;

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;

  size_t pos=0;

  resetPromotion();

  for(it=oids->begin(); it!=oids->end(); ++it)
  {
    size_t dsPos = map2Row(*it);

    if(m_logicalRows[dsPos] != dsPos)
    {
      size_t aux = dsPos;

      while(m_logicalRows[aux] != dsPos)
        aux++;

      dsPos = aux;
    }

    size_t value = m_logicalRows[dsPos];

    m_logicalRows.erase(m_logicalRows.begin()+dsPos);
    m_logicalRows.insert(m_logicalRows.begin()+pos, value);

    pos++;
  }
}

void te::qt::widgets::Promoter::sort(te::da::DataSet* dset, const std::vector<int>& cols)
{
  if(cols.empty())
    return;

  cleanPreproccessKeys();

  if(m_logicalRows.empty())
    m_logicalRows.resize(dset->size());

  te::common::TaskProgress task (QObject::tr("Sorting columns...").toStdString(), te::common::TaskProgress::UNDEFINED, m_logicalRows.size());

  std::multimap<std::vector<te::dt::AbstractData*>, int, DataComparator> order;
  std::multimap<std::vector<te::dt::AbstractData*>, int, DataComparator>::iterator m_it;

  dset->moveBeforeFirst();

  int i=0;

  std::vector<te::dt::AbstractData*> value;
  value.resize(cols.size());

  while (dset->moveNext())
  {
    if(!task.isActive())
    {
      cleanLogRowsAndProcessKeys();
      CleanAbstractData(order);

      return;
    }

    for(size_t j=0; j<cols.size(); j++)
      value[j] = (dset->isNull((size_t)cols[j])) ? 0 : dset->getValue((size_t)cols[j]).get();

    order.insert(std::pair<std::vector<te::dt::AbstractData*>, int>(value, i++));

    task.pulse();
  }

  task.setCurrentStep(0);

  i=0;
  for(m_it=order.begin(); m_it!=order.end(); ++m_it)
  {
    if(!task.isActive())
    {
      cleanLogRowsAndProcessKeys();
      CleanAbstractData(order);

      return;
    }

    m_logicalRows[i++] = m_it->second;
    te::common::FreeContents(m_it->first);

    task.pulse();
  }
}

size_t te::qt::widgets::Promoter::map2Row(te::da::ObjectId* oid)
{
  std::map<te::da::ObjectId*, size_t, te::common::LessCmp<te::da::ObjectId*> >::iterator it = m_PkeysRows.find(oid);

  return it->second;
}
