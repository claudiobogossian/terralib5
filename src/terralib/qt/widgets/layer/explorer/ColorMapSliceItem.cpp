/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/explorer/ColorMapSliceItem.cpp

  \brief A class that represents a grouping item of a color map in a LayerTreeModel.
*/

// TerraLib
#include "../../se/SymbologyPreview.h"
#include "ColorMapSliceItem.h"

// Qt
#include <QMenu>
#include <QPainter>
#include <QWidget>


te::qt::widgets::ColorMapSliceItem::ColorMapSliceItem(double min, double max, te::color::RGBAColor c, QObject* parent)
  : AbstractTreeItem(parent),
    m_min(min),
    m_max(max),
    m_rgbaColor(c),
    m_categorize(true),
    m_interpolate(false),
    m_isCheckable(false),
    m_isChecked(false)
{
}

te::qt::widgets::ColorMapSliceItem::ColorMapSliceItem(double min, double max, te::color::RGBAColor cBegin, te::color::RGBAColor cEnd, QObject* parent)
  : AbstractTreeItem(parent),
    m_min(min),
    m_max(max),
    m_rgbaColorBegin(cBegin),
    m_rgbaColorEnd(cEnd),
    m_categorize(false),
    m_interpolate(true),
    m_isCheckable(false),
    m_isChecked(false)
{
}

te::qt::widgets::ColorMapSliceItem::~ColorMapSliceItem()
{
}

int te::qt::widgets::ColorMapSliceItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::ColorMapSliceItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
  {
    if(m_categorize)
    {
      QPixmap pix(16, 16);
      pix.fill(QColor(m_rgbaColor.getRed(), m_rgbaColor.getGreen(), m_rgbaColor.getBlue()));
      QIcon icon(pix);
      return QVariant(icon);
    }

    if(m_interpolate)
    {
      QPixmap pix(16, 16);
      QPainter p(&pix);
      
      p.fillRect(0,0,8, 16, QColor(m_rgbaColorBegin.getRed(), m_rgbaColorBegin.getGreen(), m_rgbaColorBegin.getBlue()));
      p.fillRect(8,0,8, 16, QColor(m_rgbaColorEnd.getRed(), m_rgbaColorEnd.getGreen(), m_rgbaColorEnd.getBlue()));

      QIcon icon(pix);
      return QVariant(icon);
    }
  }

  if(role == Qt::DisplayRole)
  {
    QString title;

    QString strMin;
    strMin.setNum(m_min);

    QString strMax;
    strMax.setNum(m_max);

    title.append(strMin);
    title.append(" - ");
    title.append(strMax);

    return QVariant(title);
  }

  if(role == Qt::CheckStateRole && m_isCheckable)
    return QVariant(m_isChecked ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::ColorMapSliceItem::getMenu(QWidget* /*parent*/) const
{
  return 0;
}

bool te::qt::widgets::ColorMapSliceItem::canFetchMore() const
{
  return false;
}

Qt::ItemFlags te::qt::widgets::ColorMapSliceItem::flags() const
{
  return (m_isCheckable ? (Qt::ItemIsEnabled | Qt::ItemIsUserCheckable) : Qt::ItemIsEnabled);
}

void te::qt::widgets::ColorMapSliceItem::fetchMore()
{
}

bool te::qt::widgets::ColorMapSliceItem::hasChildren() const
{
  return false;
}

bool te::qt::widgets::ColorMapSliceItem::setData(int /*column*/, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole && m_isCheckable)
  {
    bool ok = false;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt(&ok));
    
    if(!ok)
      return false;

    m_isChecked = (checkState == Qt::Checked ? true : false);
  }

  return false;
}

te::map::AbstractLayerPtr te::qt::widgets::ColorMapSliceItem::getLayer() const
{
  return te::map::AbstractLayerPtr(0);
}

const std::string te::qt::widgets::ColorMapSliceItem::getItemType() const
{
  return "COLORMAP_SLICE_ITEM";
}

void te::qt::widgets::ColorMapSliceItem::setCheckable(bool checkable)
{
  m_isCheckable = false;
}

bool te::qt::widgets::ColorMapSliceItem::getCheckable()
{
  return false;
}
