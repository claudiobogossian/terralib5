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

#include "HighlightDelegate.h"

// TerraLib
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"

bool toHighlight(te::da::DataSet* dset, te::da::ObjectIdSet* objs, const int& row)
{
  dset->move(row);

  std::vector<std::string> pNames = objs->getPropertyNames();
  te::da::ObjectId* oId = te::da::GenerateOID(dset, pNames);

  bool res = objs->contains(oId);

  delete oId;

  return res;
}

te::da::ObjectId* GetOId(te::da::DataSet* dset, const std::vector<std::string>& pNames, const int& row)
{
  dset->move(row);

  return te::da::GenerateOID(dset, pNames);
}

te::da::ObjectIdSet* GetOIDSet(te::da::DataSet* dset, te::da::ObjectIdSet* oidSet, const int& initRow, const int& finalRow)
{
  te::da::ObjectIdSet* oIds = new te::da::ObjectIdSet;
  std::vector<std::string> pnames = oidSet->getPropertyNames();
  std::vector<size_t> ppos = oidSet->getPropertyPos();
  std::vector<int> ptypes = oidSet->getPropertyTypes();

  for(size_t i=0; i<pnames.size(); i++)
    oIds->addProperty(pnames[i], ppos[i], ptypes[i]);

  for(int i=initRow; i<=finalRow; i++)
    oIds->add(GetOId(dset, pnames, i));
 
  return oIds;
}

te::qt::widgets::HighlightDelegate::HighlightDelegate(QObject* parent) :
QItemDelegate(parent),
  m_objs(0),
  m_dset(0)
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

  if(toHighlight(m_dset, m_objs, index.row()))
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

void te::qt::widgets::HighlightDelegate::addObjects(te::da::ObjectIdSet* oIds)
{
  assert(m_objs);

  m_objs->Union(oIds);
}

void te::qt::widgets::HighlightDelegate::addObject(te::da::ObjectId* oId)
{
  assert(m_objs);
  assert(oId);

  if(m_objs->contains(oId))
  {
    m_objs->remove(oId);
    delete oId;
  }
  else
    m_objs->add(oId);
}

void te::qt::widgets::HighlightDelegate::addObject(const int& row)
{
  assert(m_objs);
  assert(m_dset);

  m_dset->move(row);

  te::da::ObjectId* id = te::da::GenerateOID(m_dset, m_objs->getPropertyNames());
  addObject(id);
}

void te::qt::widgets::HighlightDelegate::addObjects(const int& initRow, const int& endRow)
{
  assert(m_objs);
  assert(m_dset);

  te::da::ObjectIdSet* oIds = GetOIDSet(m_dset, m_objs, initRow, endRow); 

  addObjects(oIds);
}

void te::qt::widgets::HighlightDelegate::setDataSet(te::da::DataSet* dset)
{
  m_dset = dset;
}

void te::qt::widgets::HighlightDelegate::setObjectIdSet(te::da::ObjectIdSet* objs)
{
  m_objs = objs;
}

void te::qt::widgets::HighlightDelegate::clearSelected()
{
  m_objs->clear();
}
