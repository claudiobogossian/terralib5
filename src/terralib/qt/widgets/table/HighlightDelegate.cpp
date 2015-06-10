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

#include "HighlightDelegate.h"
#include "Promoter.h"

// TerraLib
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"

bool ToHighlight(te::da::DataSet* dset, const te::da::ObjectIdSet* objs, const int& row)
{
  if(objs == 0)
    return false;

  dset->move(row);

  std::vector<std::string> pNames = objs->getPropertyNames();
  te::da::ObjectId* oId = te::da::GenerateOID(dset, pNames);

  bool res = objs->contains(oId);

  delete oId;

  return res;
}

bool ToHighlight(te::da::DataSet* dset, const std::vector<size_t>& cols, const std::set<std::string>& data, const int& row)
{
  dset->move(row);

  std::string pkey;

  for(size_t i=0; i<cols.size(); i++)
  {
    if(dset->isNull(cols[i]) == false)
      pkey += dset->getAsString(cols[i]) + ";";
  }

  return (data.find(pkey) != data.end());
}

std::string GetOidAsString(const te::da::ObjectId* oid)
{
  boost::ptr_vector<te::dt::AbstractData> data = oid->getValue();
  boost::ptr_vector<te::dt::AbstractData>::iterator it;

  std::string pkey;

  for(it=data.begin(); it!=data.end(); ++it)
    pkey += it->toString() + ";";

  return pkey;
}

std::set<std::string> GetOidsAsString(const te::da::ObjectIdSet* objs)
{
  std::set<std::string> res;

  if(objs != 0)
  {
    std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;

    for(it=objs->begin(); it!=objs->end(); ++it)
      res.insert(GetOidAsString(*it));
  }

  return res;
}

te::qt::widgets::HighlightDelegate::HighlightDelegate(QObject* parent) :
QItemDelegate(parent),
  m_objs(0),
  m_dset(0),
  m_promoter(0)
{
}

te::qt::widgets::HighlightDelegate::~HighlightDelegate()
{
  delete m_objs;
}

void te::qt::widgets::HighlightDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  if(!index.isValid())
    return;

  QStyleOptionViewItem opt = option;

  if(m_objs != 0)
    if(ToHighlight(m_dset, m_objs->getPropertyPos(), m_oids, (int)m_promoter->getLogicalRow(index.row())))
    {
      opt.showDecorationSelected = true;
      opt.state |= QStyle::State_Selected;
      opt.palette.setColor(QPalette::Highlight, m_color);
      opt.palette.setColor(QPalette::HighlightedText, Qt::black);
    }

  QItemDelegate::paint(painter, opt, index);
}


void te::qt::widgets::HighlightDelegate::setColor(const QColor& c)
{
  m_color = c;
}

QColor te::qt::widgets::HighlightDelegate::getColor()
{
  return m_color;
}

void te::qt::widgets::HighlightDelegate::setDataSet(te::da::DataSet* dset)
{
  m_dset = dset;
}

void te::qt::widgets::HighlightDelegate::setObjectIdSet(const te::da::ObjectIdSet* objs)
{
  if(m_objs != 0)
    delete m_objs;

  if(objs == 0)
  {
    m_objs = 0;
    return;
  }

  m_objs = objs->clone();

  m_oids = GetOidsAsString(objs);
}

const te::da::ObjectIdSet* te::qt::widgets::HighlightDelegate::getSelected() const
{
  return m_objs;
}

void te::qt::widgets::HighlightDelegate::setPromoter(Promoter* promoter)
{
  m_promoter = promoter;
}
