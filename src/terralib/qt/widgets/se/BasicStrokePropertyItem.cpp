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

/*!
  \file terralib/qt/widgets/se/BasicStrokePropertyItem.cpp

  \brief A widget used to define the basic fill se object.
*/

// TerraLib
#include "../../../raster/Utils.h"
#include "../../../se/Config.h"
#include "../../../se/Stroke.h"
#include "../../../se/SvgParameter.h"
#include "../../../se/Utils.h"
#include "../../../maptools/Utils.h"
#include "../../../maptools/Enums.h"
#include "../propertybrowser/AbstractPropertyManager.h"
#include "BasicStrokePropertyItem.h"

// Qt
#include <QPainter>
#include <QIcon>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantPropertyManager>

// STL
#include <cassert>


te::qt::widgets::BasicStrokePropertyItem::BasicStrokePropertyItem(QtTreePropertyBrowser* pb, QColor c) : te::qt::widgets::AbstractPropertyItem(pb, c),
  m_stroke(new te::se::Stroke)
{
  //build property browser basic stroke
  QtProperty* basicStrokeProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Basic Stroke"));

  //color
  m_colorProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_colorManager->addProperty(tr("Color"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_colorManager->setValue(m_colorProperty, QColor(TE_SE_DEFAULT_FILL_BASIC_COLOR));
  basicStrokeProperty->addSubProperty(m_colorProperty);
  
  //opacity
  m_opacityProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->addProperty(tr("Opacity"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_opacityProperty, 100);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setMinimum(m_opacityProperty, 0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setMaximum(m_opacityProperty, 100);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setSingleStep(m_opacityProperty, 10);
  basicStrokeProperty->addSubProperty(m_opacityProperty);

  //width
  m_widthProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Width"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_widthProperty, 1);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setMinimum(m_widthProperty, 0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setMaximum(m_widthProperty, 100);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setSingleStep(m_widthProperty, 1);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setDecimals(m_widthProperty, 2);
  basicStrokeProperty->addSubProperty(m_widthProperty);

  //dash
  m_dashProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->addProperty(tr("Dash"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setEnumNames(m_dashProperty, getDashIconsNames());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setEnumIcons(m_dashProperty, getDashIcons());
  basicStrokeProperty->addSubProperty(m_dashProperty);

  //join
  m_joinProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->addProperty(tr("Join"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setEnumNames(m_joinProperty, getJoinNames());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setEnumIcons(m_joinProperty, getJoinIcons());
  basicStrokeProperty->addSubProperty(m_joinProperty);

  //cap
  m_capProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->addProperty(tr("Cap"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setEnumNames(m_capProperty, getCapNames());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setEnumIcons(m_capProperty, getCapIcons());
  basicStrokeProperty->addSubProperty(m_capProperty);

  addProperty(basicStrokeProperty, tr("Basic Stroke"), QColor(175, 255, 175));

  m_update = true;
}

te::qt::widgets::BasicStrokePropertyItem::~BasicStrokePropertyItem()
{
  delete m_stroke;
}

void te::qt::widgets::BasicStrokePropertyItem::setStroke(const te::se::Stroke* stroke)
{
  assert(stroke);

  delete m_stroke;
  
  m_stroke = stroke->clone();

  updateUi();
}

te::se::Stroke* te::qt::widgets::BasicStrokePropertyItem::getStroke() const
{
  return m_stroke->clone();
}

void te::qt::widgets::BasicStrokePropertyItem::updateUi()
{
  // Color
  te::color::RGBAColor rgba(TE_SE_DEFAULT_STROKE_BASIC_COLOR, TE_OPAQUE);
  te::se::GetColor(m_stroke, rgba);
  m_color = QColor(rgba.getRgba());
  m_color.setAlpha(rgba.getAlpha());
  updateUiStrokeColor();

  // Opacity
  int opacity = te::rst::Round((m_color.alpha() / 255.) * 100);

  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_opacityProperty, opacity);

  // Width
  const te::se::SvgParameter* width = m_stroke->getWidth();
  if(width)
    te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_widthProperty, te::se::GetDouble(width));

  // Dash style
  const te::se::SvgParameter* dasharray = m_stroke->getDashArray();
  if(dasharray)
  {
    std::string str = te::se::GetString(dasharray);
    for(size_t t = 0; t < m_dashes.size(); ++t)
    {
      if(str == m_dashes[t])
      {
        te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setValue(m_dashProperty, t);
        break;
      }
    }
  }
  else
  {
    te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setValue(m_dashProperty, 0);
  }

  // Join Style
  const te::se::SvgParameter* join = m_stroke->getLineJoin();
  if(join)
  {
    std::string joinStr = te::se::GetString(join);

    std::map<int, std::string>::iterator it = m_joinMap.begin();

    while(it != m_joinMap.end())
    {
      if(it->second == joinStr)
      {
        te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setValue(m_joinProperty, it->first);
      }

      ++it;
    }
  }
  else
  {
    te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setValue(m_joinProperty, 0); //TE_SE_MITRE_JOIN
  }

  // Cap Style
  const te::se::SvgParameter* cap = m_stroke->getLineCap();
  if(cap)
  {
    std::string capStr = te::se::GetString(cap);

    std::map<int, std::string>::iterator it = m_capMap.begin();

    while(it != m_capMap.end())
    {
      if(it->second == capStr)
      {
        te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setValue(m_capProperty, it->first);
      }

      ++it;
    }
  }
  else
  {
    te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setValue(m_capProperty, 0); //TE_SE_BUTT_CAP
  }
}

void te::qt::widgets::BasicStrokePropertyItem::updateUiStrokeColor()
{
  te::qt::widgets::AbstractPropertyManager::getInstance().m_colorManager->setValue(m_colorProperty, m_color);
}

void te::qt::widgets::BasicStrokePropertyItem::valueChanged(QtProperty *p, int value)
{
  if(!m_update)
    return;

  m_update = false;

  if(p == m_opacityProperty)
  {
    double opacity = value / 100.;

    int alpha = te::rst::Round(opacity * 255);

    m_color.setAlpha(alpha);
    updateUiStrokeColor();

    // Updating stroke opacity
    m_stroke->setOpacity(QString::number(opacity, 'g', 2).toStdString());
    emit strokeChanged();
  }
  else if(p == m_dashProperty)
  {
    if(value == -1)
      return;

    std::string pattern = m_dashes[value];
    m_stroke->setDashArray(pattern);
    emit strokeChanged();
  }
  else if(p == m_joinProperty)
  {
    m_stroke->setLineJoin(m_joinMap[value]);
    
    emit strokeChanged();
  }
  else if(p == m_capProperty)
  {
    m_stroke->setLineCap(m_capMap[value]);
    
    emit strokeChanged();
  }

  m_update = true;
}

void te::qt::widgets::BasicStrokePropertyItem::valueChanged(QtProperty *p, double value)
{
  if(!m_update)
    return;

  m_update = false;

  if(p == m_widthProperty)
  {
    QString str;
    str.setNum(value);

    m_stroke->setWidth(str.toStdString());
    emit strokeChanged();
  }

  m_update = true;
}

void te::qt::widgets::BasicStrokePropertyItem::valueChanged(QtProperty *p, const QColor &value)
{
  if(!m_update)
    return;

  m_update = false;

  if(p == m_colorProperty)
  {
    // The new stroke color
    m_color.setRgb(value.red(), value.green(), value.blue(), value.alpha());

    int opacity = (value.alpha() / 255) * 100;

    te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_opacityProperty, opacity);

    updateUiStrokeColor();

    // Updating stroke color
    m_stroke->setColor(m_color.name().toStdString());
    m_stroke->setOpacity(QString::number(opacity, 'g', 2).toStdString());
    emit strokeChanged();
  }

  m_update = true;
}

QMap<int, QIcon> te::qt::widgets::BasicStrokePropertyItem::getDashIcons()
{
  QMap<int, QIcon> map;

  // Dash graphical representation size
  QSize size(32, 16);

  // Line that will be draw with the dashe styles
  QLine line(0, size.height() * 0.5, size.width(), size.height() * 0.5);

  // Setuping...
  QPen pen;
  pen.setWidth(2);

  for(size_t t = 0; t < m_dashes.size(); ++t)
  {
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

    std::string pattern = m_dashes[t];
    std::vector<double> dasharray;
    te::map::GetDashStyle(pattern, dasharray);
      
    QVector<qreal> qdasharray = QVector<qreal>::fromStdVector(dasharray);

    pen.setDashPattern(qdasharray);
    painter.setPen(pen);
    painter.drawLine(line);

    QIcon icon(pixmap);

    map[t] = icon;
  }

  return map;
}

QMap<int, QIcon> te::qt::widgets::BasicStrokePropertyItem::getJoinIcons()
{
  QMap<int, QIcon> map;

  map[0] = QIcon::fromTheme("stroke-join-miter").pixmap(16,16);
  map[1] = QIcon::fromTheme("stroke-join-round").pixmap(16,16);
  map[2] = QIcon::fromTheme("stroke-join-bevel").pixmap(16,16);

  return map;
}

QMap<int, QIcon> te::qt::widgets::BasicStrokePropertyItem::getCapIcons()
{
  QMap<int, QIcon> map;

  map[0] = QIcon::fromTheme("stroke-cap-butt").pixmap(16,16);
  map[1] = QIcon::fromTheme("stroke-cap-round").pixmap(16,16);
  map[2] = QIcon::fromTheme("stroke-cap-square").pixmap(16,16);

  return map;
}

QStringList te::qt::widgets::BasicStrokePropertyItem::getDashIconsNames()
{
  m_dashes.push_back("");
  m_dashes.push_back("1 2");
  m_dashes.push_back("4 2");
  m_dashes.push_back("4 2 1 2");
  m_dashes.push_back("4 2 1 2 1 2");

  QStringList l;

  for(size_t t = 0; t < m_dashes.size(); ++t)
  {
     l << m_dashes[t].c_str();
  }

  return l;
}

QStringList te::qt::widgets::BasicStrokePropertyItem::getJoinNames()
{
  m_joinMap.insert(std::map<int, std::string>::value_type(0, TE_SE_MITRE_JOIN));
  m_joinMap.insert(std::map<int, std::string>::value_type(1, TE_SE_ROUND_JOIN));
  m_joinMap.insert(std::map<int, std::string>::value_type(2, TE_SE_BEVEL_JOIN));

  QStringList l;

  l << m_joinMap[0].c_str();
  l << m_joinMap[1].c_str();
  l << m_joinMap[2].c_str();

  return l;
}

QStringList te::qt::widgets::BasicStrokePropertyItem::getCapNames()
{
  m_capMap.insert(std::map<int, std::string>::value_type(0, TE_SE_BUTT_CAP));
  m_capMap.insert(std::map<int, std::string>::value_type(1, TE_SE_ROUND_CAP));
  m_capMap.insert(std::map<int, std::string>::value_type(2, TE_SE_SQUARE_CAP));

  QStringList l;

  l << m_capMap[0].c_str();
  l << m_capMap[1].c_str();
  l << m_capMap[2].c_str();

  return l;
}
