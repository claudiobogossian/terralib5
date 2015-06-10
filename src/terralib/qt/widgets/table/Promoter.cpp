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
#include "Promoter.h"

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../common/Translator.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../datatype/SimpleData.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../Exception.h"
#include "../Config.h"

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

std::vector<std::string> GetColumnsNames(te::da::DataSet* dset, const std::vector<size_t>& colsPositions)
{
  std::vector<std::string> res;
  std::vector<size_t>::const_iterator it;

  for(it=colsPositions.begin(); it!=colsPositions.end(); ++it)
    res.push_back(dset->getPropertyName(*it));

  return res;
}

template <class T>
void ClearVector(std::vector<T>& vec)
{
  std::vector<T>().swap(vec);
}

size_t GetRowPosition(const size_t& pos, const std::vector<size_t>& posVec)
{
  for(size_t i=pos; i<posVec.size(); i++)
    if(posVec[i] == pos)
      return i;

  for(size_t i=0; i<posVec.size(); i++)
    if(posVec[i] == pos)
      return i;

  throw te::qt::widgets::Exception(TE_TR("Position not found."));
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
  size_t setSize = dset->size();

  cleanPreproccessKeys();

  if(m_logicalRows.empty())
    m_logicalRows.resize(setSize);

  dset->moveFirst();

  for(size_t i=0; i<setSize; i++)
  {
    std::string pkey;

    for(size_t aux=0; aux<pkeys.size(); aux++)
    {
      if(dset->isNull(pkeys[aux]) == false)
        pkey += dset->getAsString(pkeys[aux]) + ";";
    }

    m_PkeysRows[pkey] = i;

    m_logicalRows[i] = i;

    dset->moveNext();
  }
}

size_t te::qt::widgets::Promoter::getLogicalRow(const size_t& visualRow)
{
  return m_logicalRows.empty() ? visualRow : m_logicalRows[visualRow];
}

void te::qt::widgets::Promoter::cleanPreproccessKeys()
{
  m_PkeysRows.clear();
}

void te::qt::widgets::Promoter::promote(const te::da::ObjectIdSet* oids)
{
  if(m_logicalRows.empty() || m_PkeysRows.empty() || oids == 0)
    return;

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;

  resetPromotion();

  std::map<size_t, size_t> new_pos;

  for(it = oids->begin(); it != oids->end(); ++it)
  {
    size_t dsPos = map2Row(*it);
    new_pos[dsPos] = m_logicalRows[dsPos];
  }

  std::map<size_t, size_t>::iterator mit;

  size_t pos = 0;

  for(mit = new_pos.begin(); mit != new_pos.end(); ++mit)
  {
    m_logicalRows.erase(m_logicalRows.begin()+mit->first);
    m_logicalRows.insert(m_logicalRows.begin()+pos, mit->second);
    pos++;
  }
}

size_t te::qt::widgets::Promoter::map2Row(te::da::ObjectId* oid)
{
  boost::ptr_vector<te::dt::AbstractData> data = oid->getValue();
  boost::ptr_vector<te::dt::AbstractData>::iterator it_d;

  std::string pkey;

  for(it_d=data.begin(); it_d!=data.end(); ++it_d)
    pkey += it_d->toString() + ";";

  std::map<std::string, size_t>::iterator it = m_PkeysRows.find(pkey);

  if(it == m_PkeysRows.end())
    throw Exception(TE_TR("Fail to get position of Object id"));

  return GetRowPosition(it->second, m_logicalRows);
}
