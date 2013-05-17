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
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../datatype/SimpleData.h"
#include "../../../dataaccess/utils/Utils.h"


//struct AbstractDataCompare
//{
//  bool operator() (const std::vector<te::dt::AbstractData*>& lhs, const std::vector<te::dt::AbstractData*>& rhs) const
//  {
//    for (size_t i=0; i<lhs.size(); i++)
//    {
//      switch(lhs[i]->getTypeCode())
//      {
//      }
////      te::dt::SimpleData* l = dynamic_cast<te::dt::SimpleData*>(lhs[i])
//    }
//      //if(*rhs[i] < *lhs[i])
//      //  return false;
//
//    return true;
//  }
//};


std::vector<std::string> GetColumnsNames(te::da::DataSet* dset, const std::vector<size_t>& colsPositions)
{
  std::vector<std::string> res;
  std::vector<size_t>::const_iterator it;

  for(it=colsPositions.begin(); it!=colsPositions.end(); ++it)
    res.push_back(dset->getPropertyName(*it));

  return res;
}

te::qt::widgets::Promoter::Promoter() :
m_enabled(false)
{
}

te::qt::widgets::Promoter::~Promoter()
{
  cleanPreproccessKeys();
}

bool te::qt::widgets::Promoter::isPromotionEnabled()
{
  return m_enabled;
}

void te::qt::widgets::Promoter::resetPromotion()
{
  for(size_t i=0; i<m_logicalRows.size(); i++)
    m_logicalRows[i] = i;
}

void te::qt::widgets::Promoter::preProcessKeys(te::da::DataSet* dset, const std::vector<size_t>& pkeys)
{
  size_t setSize = dset->size();
  std::vector<std::string> colsNames = GetColumnsNames(dset, pkeys);

  cleanPreproccessKeys();

  m_logicalRows.resize(setSize);

  dset->moveFirst();

  for(size_t i=0; i<setSize; i++)
  {
    te::da::ObjectId* obj = te::da::GenerateOID(dset, colsNames);

    dset->moveNext();

    m_PkeysRows[obj] = i;
    m_logicalRows[i] = i;
  }

  m_enabled = true;
}

size_t te::qt::widgets::Promoter::getLogicalRow(const size_t& visualRow)
{
  return m_logicalRows[visualRow];
}

void te::qt::widgets::Promoter::cleanPreproccessKeys()
{
  m_logicalRows.clear();
  m_PkeysRows.clear();

  m_enabled = false;
}

void te::qt::widgets::Promoter::promote(const std::vector<te::da::ObjectId*>& oids)
{
  std::vector<te::da::ObjectId*>::const_iterator it;
  size_t pos=0;

  resetPromotion();

  for(it=oids.begin(); it!=oids.end(); ++it)
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

  m_logicalRows.resize(dset->size());

  std::map<std::string, int> order;
  std::map<std::string, int>::iterator m_it;
  std::vector<int>::const_iterator it;

  dset->moveBeforeFirst();

  int i=0;

  while (dset->moveNext())
  {
    std::string value;

    for(it=cols.begin(); it !=cols.end(); ++it)
      value += dset->getAsString(*it, 5);

    order[value] = i++;
  }

  i=0;
  for(m_it=order.begin(); m_it!=order.end(); ++m_it)
    m_logicalRows[i++] = m_it->second;
}

size_t te::qt::widgets::Promoter::map2Row(te::da::ObjectId* oid)
{
  std::map<te::da::ObjectId*, size_t, te::common::LessCmp<te::da::ObjectId*> >::iterator it = m_PkeysRows.find(oid);

  return it->second;
}
