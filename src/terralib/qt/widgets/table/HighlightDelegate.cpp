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
#include "Promoter.h"

// TerraLib
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"

bool toHighlight(te::da::DataSet* dset, const te::da::ObjectIdSet* objs, const int& row)
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

te::qt::widgets::HighlightDelegate::HighlightDelegate(QObject* parent) :
QItemDelegate(parent),
  m_objs(0),
  m_dset(0),
  m_promoter(0)
{
}

te::qt::widgets::HighlightDelegate::~HighlightDelegate()
{
}

void te::qt::widgets::HighlightDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  if(!index.isValid())
    return;

  QStyleOptionViewItem opt = option;

  if(toHighlight(m_dset, m_objs, m_promoter->getLogicalRow(index.row())))
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
  m_objs = objs;
}

const te::da::ObjectIdSet* te::qt::widgets::HighlightDelegate::getSelected() const
{
  return m_objs;
}

void te::qt::widgets::HighlightDelegate::setPromoter(Promoter* promoter)
{
  m_promoter = promoter;
}
