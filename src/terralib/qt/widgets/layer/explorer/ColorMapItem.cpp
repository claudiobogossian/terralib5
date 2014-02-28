/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/explorer/ColorMapItem.cpp

  \brief A class that represents a color map of rastersymbolizer of a layer in a LayerTreeModel.
*/

// TerraLib
#include "../../../../color/ColorBar.h"
#include "../../../../se/Categorize.h"
#include "../../../../se/ColorMap.h"
#include "../../../../se/Interpolate.h"
#include "../../../../se/InterpolationPoint.h"
#include "../../../../se/Utils.h"
#include "ColorMapSliceItem.h"
#include "ColorMapItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::qt::widgets::ColorMapItem::ColorMapItem(te::se::ColorMap* cm, QObject* parent)
  : AbstractTreeItem(parent),
    m_colorMap(cm),
    m_isCheckable(false),
    m_isChecked(false)
{
}

te::qt::widgets::ColorMapItem::~ColorMapItem()
{
}

int te::qt::widgets::ColorMapItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::ColorMapItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("grouping"));

  if(role == Qt::DisplayRole)
  {
    QString type;

    if(m_colorMap->getCategorize())
    {
      type = tr("Categorization");
    }
    else if(m_colorMap->getInterpolate())
    {
      type = tr("Interpolation");
    }

    QString text = tr("Classification by") + " " + type;
    return QVariant(text);
  }

  if(role == Qt::CheckStateRole && m_isCheckable)
    return QVariant(m_isChecked ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::ColorMapItem::getMenu(QWidget* /*parent*/) const
{
  return 0;
}

bool te::qt::widgets::ColorMapItem::canFetchMore() const
{
  return (m_colorMap->getCategorize() || m_colorMap->getInterpolate()) && children().isEmpty();
}

Qt::ItemFlags te::qt::widgets::ColorMapItem::flags() const
{
  return (m_isCheckable ? (Qt::ItemIsEnabled | Qt::ItemIsUserCheckable) : Qt::ItemIsEnabled);
}

void te::qt::widgets::ColorMapItem::fetchMore()
{
  if(!children().isEmpty())
    return;

  if(m_colorMap->getCategorize())
  {
    std::vector<te::se::ParameterValue*> t = m_colorMap->getCategorize()->getThresholds();
    std::vector<te::se::ParameterValue*> tV = m_colorMap->getCategorize()->getThresholdValues();

    for(std::size_t i = 1; i < tV.size() - 1; ++i)
    {
      double lowerLimit, upperLimit;

      if(i == 0)
      {
        lowerLimit = std::numeric_limits<double>::min();
        upperLimit = te::se::GetDouble(t[i]);
      }
      else if(i == tV.size() - 1)
      {
        lowerLimit = te::se::GetDouble(t[i - 1]);
        upperLimit = std::numeric_limits<double>::max();
      }
      else
      {
        lowerLimit = te::se::GetDouble(t[i - 1]);
        upperLimit = te::se::GetDouble(t[i]);
      }

      std::string colorName = te::se::GetString(tV[i]);
      te::color::RGBAColor color(colorName);
      color.setColor(color.getRed(), color.getGreen(), color.getBlue());

      new ColorMapSliceItem(lowerLimit, upperLimit, color, this);
    }
  }
  else if(m_colorMap->getInterpolate())
  {
    std::vector<te::se::InterpolationPoint*> ip = m_colorMap->getInterpolate()->getInterpolationPoints();

    for(std::size_t i = 0; i < ip.size() - 1; ++i)
    {
      te::se::InterpolationPoint* ipItemInit = ip[i];

      std::string colorName = te::se::GetString(ipItemInit->getValue());
      te::color::RGBAColor colorInit(colorName);
      colorInit.setColor(colorInit.getRed(), colorInit.getGreen(), colorInit.getBlue());
      double lowerLimit = ipItemInit->getData();

      te::se::InterpolationPoint* ipItemEnd = ip[i + 1];

      colorName = te::se::GetString(ipItemEnd->getValue());
      te::color::RGBAColor colorEnd(colorName);
      colorEnd.setColor(colorEnd.getRed(), colorEnd.getGreen(), colorEnd.getBlue());
      double upperLimit = ipItemEnd->getData();

      new ColorMapSliceItem(lowerLimit, upperLimit, colorInit, colorEnd, this);
    }
  }
}

bool te::qt::widgets::ColorMapItem::hasChildren() const
{
  if(m_colorMap->getCategorize())
  {
    return !m_colorMap->getCategorize()->getThresholdValues().empty();
  }

  if(m_colorMap->getInterpolate())
  {
    return !m_colorMap->getInterpolate()->getInterpolationPoints().empty();
  }

  return false;
}

bool te::qt::widgets::ColorMapItem::setData(int /*column*/, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole && m_isCheckable)
  {
    bool ok = false;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt(&ok));
    
    if(!ok)
      return false;

    m_isChecked = (checkState == Qt::Checked ? true : false);

    return true;
  }

  return false;
}

te::map::AbstractLayerPtr te::qt::widgets::ColorMapItem::getLayer() const
{
  return te::map::AbstractLayerPtr(0);
}

const std::string te::qt::widgets::ColorMapItem::getItemType() const
{
  return "COLORMAP_ITEM";
}

void te::qt::widgets::ColorMapItem::setCheckable(bool checkable)
{
  m_isCheckable = false;
}

bool te::qt::widgets::ColorMapItem::getCheckable()
{
  return false;
}
