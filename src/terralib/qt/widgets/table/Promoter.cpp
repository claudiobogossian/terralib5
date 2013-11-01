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
#include "../../../common/Translator.h"
#include "../../../common/progress/TaskProgress.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../datatype/SimpleData.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../Exception.h"
#include "../Config.h"

// Qt
#include <QObject>

// Boost
#include <boost/pool/pool_alloc.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

//STL
#include <algorithm>

typedef std::pair<int, std::vector< te::dt::AbstractData* > > data_pair;

typedef std::pair<int, std::vector<te::dt::AbstractData*>* > data_pair_ptr;

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

//bool DataComparator (const std::vector<te::dt::AbstractData*>& lhs, const std::vector<te::dt::AbstractData*>& rhs)
//{
//  for (size_t i=0; i<lhs.size(); i++)
//  {
//    COMPARISON res = CompareAbsData(lhs[i], rhs[i]);
//
//    if(res == LESSER)
//      return true;
//    else if (res == GREATER)
//      return false;
//  }
//
//  return false;
//};

bool ObjectIdComparator::operator () (te::da::ObjectId* lhs, te::da::ObjectId* rhs)
{
  boost::ptr_vector<te::dt::AbstractData> lhs_d = lhs->getValue();
  boost::ptr_vector<te::dt::AbstractData> rhs_d = rhs->getValue();

  for (size_t i=0; i<lhs_d.size(); i++)
  {
    COMPARISON res = CompareAbsData(&lhs_d[i], &rhs_d[i]);

    if(res == LESSER)
      return true;
    else if (res == GREATER)
      return false;
  }

  return false;
}

bool DataComparator (data_pair& lhs, data_pair& rhs)
{
  for (size_t i=0; i<lhs.second.size(); i++)
  {
    COMPARISON res = CompareAbsData(lhs.second[i], rhs.second[i]);

    if(res == LESSER)
      return true;
    else if (res == GREATER)
      return false;
  }

  return false;
};

struct DataComparatorPtrS
{
  bool operator() (data_pair_ptr* lhs, data_pair_ptr* rhs)
  {
    for (size_t i=0; i<lhs->second->size(); i++)
    {
      COMPARISON res = CompareAbsData((*lhs->second)[i], (*rhs->second)[i]);

      if(res == LESSER)
        return true;
      else if (res == GREATER)
        return false;
    }

    return false;
  }

} DataComparatorPtr;


std::vector<std::string> GetColumnsNames(te::da::DataSet* dset, const std::vector<size_t>& colsPositions)
{
  std::vector<std::string> res;
  std::vector<size_t>::const_iterator it;

  for(it=colsPositions.begin(); it!=colsPositions.end(); ++it)
    res.push_back(dset->getPropertyName(*it));

  return res;
}

//void CleanAbstractData(std::multimap<std::vector<te::dt::AbstractData*>, int, DataComparator>& d)
//{
//  std::multimap<std::vector<te::dt::AbstractData*>, int, DataComparator>::iterator it;
//    
//  for(it=d.begin(); it!=d.end(); ++it)
//    te::common::FreeContents(it->first);
//
//  d.clear();
//}

template <class T>
void ClearVector(std::vector<T>& vec)
{
  std::vector<T>().swap(vec);
}

void CleanAbstractData(std::vector<data_pair>& data)
{
  std::vector<data_pair>::iterator it;
    
  for(it=data.begin(); it!=data.end(); ++it)
  {
    te::common::FreeContents(it->second);
    ClearVector(it->second);
  }

  ClearVector(data);
}


struct CleanVectorPtr
{
  void operator() (data_pair_ptr*& data)
  {
    te::common::FreeContents(*data->second);
    delete data->second;
    delete data;
    data = 0;
  }
};

void CleanAbstractDataPtr(std::vector<data_pair_ptr*>& data)
{
  std::for_each(data.begin(), data.end(), CleanVectorPtr());
  ClearVector(data);
}


struct OrderedData
{
  std::vector<te::dt::AbstractData*> m_data;
  size_t m_idx;

  OrderedData()
  {
  }

  OrderedData(const std::vector<te::dt::AbstractData*>& data, size_t idx)
  {
    m_data = data;
    m_idx = idx;
  }

  OrderedData(const OrderedData& other)
  {
    m_data = other.m_data;
    m_idx = other.m_idx;
  }

  ~OrderedData()
  {
    te::common::FreeContents(m_data);
  }

};

inline bool operator< (const OrderedData& lhs, const OrderedData& rhs)
{
  for (size_t i=0; i<lhs.m_data.size(); i++)
  {
    COMPARISON res = CompareAbsData(lhs.m_data[i], rhs.m_data[i]);

    if(res == LESSER)
      return true;
    else if (res == GREATER)
      return false;
  }

  return false;
}


struct FillOrderedDataVector
{
  te::da::DataSet* m_dset;
  const std::vector<int>* m_cols;
  std::vector<size_t>* m_order;
  std::vector<te::dt::AbstractData*> m_value;
  size_t m_i;

  FillOrderedDataVector(te::da::DataSet* dset, const std::vector<int>* cols, std::vector<size_t>* order)
  {
    m_dset = dset;
    m_cols = cols;
    m_order = order;
    m_value.resize(m_cols->size());
    m_i = 0;
  }

  void operator() (OrderedData*& data)
  {
    m_dset->moveNext();

    for(size_t i=0; i<m_cols->size(); i++)
      m_value[i] = (m_dset->isNull((size_t)(*m_cols)[i])) ? 0 : m_dset->getValue((size_t)(*m_cols)[i]).release();

    (*m_order)[m_i] = m_i;

    data = new OrderedData(m_value, m_i);
    m_i++;
  }
};

struct ReorderData
{
  std::vector<size_t>* m_order;
  size_t m_i;

  ReorderData(std::vector<size_t>* order)
  {
    m_order = order;
    m_i = 0;
  }

  void operator() (OrderedData* data)
  {
    (*m_order)[m_i] = data->m_idx;
    m_i++;
  }
};

struct OrderedDataComparator
{
  bool operator() (const OrderedData& lhs, const OrderedData& rhs)
  {
    for (size_t i=0; i<lhs.m_data.size(); i++)
    {
      COMPARISON res = CompareAbsData(lhs.m_data[i], rhs.m_data[i]);

      if(res == LESSER)
        return true;
      else if (res == GREATER)
        return false;
    }

    return false;
  }

};


struct ClearDataVector
{
  void operator() (std::vector<te::dt::AbstractData*>& data)
  {
    te::common::FreeContents(data);
  }
};

struct CleanOrderedData
{
  void operator() (OrderedData*& data)
  {
    delete data;
    data = 0;
  }
};

void ClearOrderedDataVector(std::vector<OrderedData*>& data)
{
  std::for_each(data.begin(), data.end(), CleanOrderedData());
  ClearVector(data);
}

struct FillVectorPtr
{
  te::da::DataSet* m_dset;
//  std::vector<data_pair_ptr*> m_data;
  std::vector<int> m_cols;
  int m_i;

  FillVectorPtr(te::da::DataSet* dset, const std::vector<int>& cols)
  {
    m_dset = dset;
    m_cols = cols;
    m_i = 0;
  }

  void operator() (data_pair_ptr*& data)
  {
    m_dset->moveNext();

    std::vector<te::dt::AbstractData*>* value = new std::vector<te::dt::AbstractData*>(m_cols.size());  

    for(size_t j=0; j<m_cols.size(); j++)
      (*value)[j] = (m_dset->isNull((size_t)m_cols[j])) ? 0 : m_dset->getValue((size_t)m_cols[j]).release();

    data = new data_pair_ptr(m_i, value);
    m_i++;
  }

};

size_t GetRowPosition(const size_t& pos, const std::vector<size_t>& posVec)
{
  for(size_t i=0; i<posVec.size(); i++)
    if(posVec[i] == pos)
      return i;

  throw te::qt::widgets::Exception(TR_QT_WIDGETS("Position not found."));
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

  ClearVector(m_logicalRows);
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

  te::common::TaskProgress task(QObject::tr("Preprocessing primary keys...").toStdString(), te::common::TaskProgress::UNDEFINED, (int)m_logicalRows.size());

  for(size_t i=0; i<setSize; i++)
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
  std::map<te::da::ObjectId*, size_t, ObjectIdComparator>::iterator it;

  for(it=m_PkeysRows.begin(); it!=m_PkeysRows.end(); ++it)
    delete it->first;

  std::map<te::da::ObjectId*, size_t, ObjectIdComparator>().swap(m_PkeysRows);
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

    //if(m_logicalRows[dsPos] != dsPos)
    //{
    //  size_t aux = dsPos;

    //  while(m_logicalRows[aux] != dsPos)
    //    aux++;

    //  dsPos = aux;
    //}

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

  size_t dSize = dset->size();

  if(m_logicalRows.empty())
    m_logicalRows.resize(dSize);

  te::common::TaskProgress task (QObject::tr("Sorting columns...").toStdString(), te::common::TaskProgress::UNDEFINED, (int)dSize);

  boost::ptr_vector<OrderedData> order;
  std::vector<te::dt::AbstractData*> data(cols.size());

//  order.resize(dSize);

//  std::vector<OrderedData, boost::pool_allocator<OrderedData> > order;
//  std::vector< std::vector<te::dt::AbstractData*>, boost::pool_allocator< std::vector<te::dt::AbstractData*> > > data;
////  std::vector<data_pair_ptr*>::iterator data_it;
//
//  order.resize(dSize);
//  data.resize(dSize);
//
  dset->moveBeforeFirst();
  int i=0;

  while(dset->moveNext())
  {
    for(size_t j=0; j<cols.size(); j++)
      data[j] = (dset->isNull((size_t)cols[j])) ? 0 : dset->getValue((size_t)cols[j]).release();

    m_logicalRows[i] = i;

    order.push_back(new OrderedData(data, i));
    i++;
  }

//
//  std::for_each(order.begin(), order.end(), FillOrderedDataVector(dset, &cols, &m_logicalRows));

  order.sort();

  boost::ptr_vector<OrderedData>::iterator it;

  i=0;
  for(it=order.begin(); it!=order.end(); ++it)
    m_logicalRows[i++] = it->m_idx; 

//
////  int i=0;
//  //std::vector<te::dt::AbstractData*> value;
//  //value.resize(cols.size());
//
//  //while (dset->moveNext())
//  //{
//  //  if(!task.isActive())
//  //  {
//  //    ClearVector(m_logicalRows);
//  //    CleanAbstractData(order);
//  //    return;
//  //  }
//
//  //  for(size_t j=0; j<cols.size(); j++)
//  //    value[j] = (dset->isNull((size_t)cols[j])) ? 0 : dset->getValue((size_t)cols[j]).release();
//
//  //  order[i] = data_pair(i, value);
//
//  //  i++;
//
//  //  task.pulse();
//  //}
//
//  std::sort(order.begin(), order.end(), OrderedDataComparator());
//
//  std::for_each(order.begin(), order.end(), ReorderData(&m_logicalRows));
//
//  std::for_each(data.begin(), data.end(), ClearDataVector());
//
//  boost::singleton_pool<boost::pool_allocator_tag, sizeof(OrderedData)>::release_memory();
//  boost::singleton_pool<boost::pool_allocator_tag, sizeof(std::vector<te::dt::AbstractData*>)>::release_memory();
//
//  std::vector<OrderedData, boost::pool_allocator<OrderedData> >().swap(order);

  //task.setCurrentStep(0);
  //task.setTotalSteps((int)dSize);

//  int i=0;

//  for(data_it=order.begin(); data_it!=order.end(); ++data_it)
//  {
  //  if(!task.isActive())
  //  {
  //    ClearVector(m_logicalRows);
  //    CleanAbstractData(order);
  //    return;
  //  }

 //   m_logicalRows[i++] = (*data_it)->first;

  //  task.pulse();
//  }

//  CleanAbstractDataPtr(order);
}

size_t te::qt::widgets::Promoter::map2Row(te::da::ObjectId* oid)
{
  std::map<te::da::ObjectId*, size_t, ObjectIdComparator>::iterator it = m_PkeysRows.find(oid);

  if(it == m_PkeysRows.end())
    throw Exception(TR_QT_WIDGETS("Fail to get position of Object id"));

  return (m_logicalRows.empty() ? it->second : GetRowPosition(it->second, m_logicalRows));
}
