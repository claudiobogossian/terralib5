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
\file terralib/qt/widgets/se/PointPlacementPropertyItem.cpp

\brief A widget used to define the PointPlacement se object.
*/


#include "../../../common/StringUtils.h"
#include "../../../se/AnchorPoint.h"
#include "../../../se/Displacement.h"
#include "../../../se/SvgParameter.h"
#include "../../../se/PointPlacement.h"
#include "../../../se/Utils.h"
#include "../propertybrowser/AbstractPropertyManager.h"
#include "PointPlacementPropertyItem.h"

// STL
#include <cassert>

// QtPropertyBrowser


te::qt::widgets::PointPlacementPropertyItem::PointPlacementPropertyItem(QtTreePropertyBrowser* pb, QColor c) : te::qt::widgets::AbstractPropertyItem(pb, c),
  m_pointPlacement(new te::se::PointPlacement)
{
  //build property browser basic fill
  QtProperty* pointPlacementProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Point Placement"));

  //anchor
  m_anchorProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->addProperty(tr("Anchor"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->setMinimum(m_anchorProperty, QSize(0, 0));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->setMaximum(m_anchorProperty, QSize(100, 100));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->setValue(m_anchorProperty, QSize(50, 50));
  pointPlacementProperty->addSubProperty(m_anchorProperty);

  //displacement
  m_displacementProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->addProperty(tr("Displacement"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->setMinimum(m_displacementProperty, QSize(-100, -100));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->setMaximum(m_displacementProperty, QSize(100, 100));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->setValue(m_displacementProperty, QSize(0, 0));
  pointPlacementProperty->addSubProperty(m_displacementProperty);

  //rotation
  m_rotationProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->addProperty(tr("Rotation"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setMinimum(m_rotationProperty, -180);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setMaximum(m_rotationProperty, 180);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_rotationProperty, 0);
  pointPlacementProperty->addSubProperty(m_rotationProperty);

  addProperty(pointPlacementProperty, tr("Point Placement"), QColor(241, 165, 61));

  m_update = true;
}

te::qt::widgets::PointPlacementPropertyItem::~PointPlacementPropertyItem()
{
  delete m_pointPlacement;
}

void te::qt::widgets::PointPlacementPropertyItem::valueChanged(QtProperty* p, int value)
{
  if (!m_update)
    return;

  if (p == m_rotationProperty)
  {
    m_update = false;

    m_pointPlacement->setRotation(new te::se::ParameterValue(QString::number(value).toStdString()));
    
    emit pointPlacementChanged();
    m_update = true;
  }
}

void te::qt::widgets::PointPlacementPropertyItem::valueChanged(QtProperty* p, const QSize &value)
{
  if (!m_update)
    return;

  if (p == m_displacementProperty)
  {
    m_update = false;

    te::se::Displacement* disp = new te::se::Displacement();
    disp->setDisplacementX(new te::se::ParameterValue(QString::number(value.width()).toStdString()));
    disp->setDisplacementY(new te::se::ParameterValue(QString::number(value.height()).toStdString()));

    m_pointPlacement->setDisplacement(disp);

    emit pointPlacementChanged();
    m_update = true;
  }

  if (p == m_anchorProperty)
  {
    m_update = false;

    double x = value.width() / 100.;
    double y = value.height() / 100.;

    te::se::AnchorPoint* ap = new te::se::AnchorPoint();
    ap->setAnchorPointX(new te::se::ParameterValue(QString::number(x).toStdString()));
    ap->setAnchorPointY(new te::se::ParameterValue(QString::number(y).toStdString()));

    m_pointPlacement->setAnchorPoint(ap);

    emit pointPlacementChanged();
    m_update = true;
  }
}

void te::qt::widgets::PointPlacementPropertyItem::setPointPlacement(const te::se::PointPlacement* pp)
{
  assert(pp);

  // Verifying if this widget can deal with the given fill...
  delete m_pointPlacement;

  m_pointPlacement = dynamic_cast<te::se::PointPlacement*>(pp->clone());

  updateUi();

  m_update = true;
}

te::se::PointPlacement* te::qt::widgets::PointPlacementPropertyItem::getPointPlacement() const
{
  return dynamic_cast<te::se::PointPlacement*>(m_pointPlacement->clone());
}

void te::qt::widgets::PointPlacementPropertyItem::resetPointPlacement()
{
  //reset anchor value
  QSize anchorSize(50, 50);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->setValue(m_anchorProperty, anchorSize);

  //reset displacement value
  QSize dispSize(0, 0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->setValue(m_displacementProperty, dispSize);

  //reset rotation value
  int rotationValue = 0;
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_rotationProperty, rotationValue);
}

void te::qt::widgets::PointPlacementPropertyItem::updateUi()
{
  m_update = true;

  //update anchor
  const te::se::AnchorPoint* ap = m_pointPlacement->getAnchorPoint();

  QSize anchorSize(50, 50);

  if (ap)
  {
    int x = (int)(te::se::GetDouble(ap->getAnchorPointX()) * 100.);
    int y = (int)(te::se::GetDouble(ap->getAnchorPointY()) * 100.);

    anchorSize.setWidth(x);
    anchorSize.setHeight(y);
  }

  te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->setValue(m_anchorProperty, anchorSize);

  //update displacement
  const te::se::Displacement* disp = m_pointPlacement->getDisplacement();

  QSize dispSize (0, 0);

  if (disp)
  {
    dispSize.setWidth(te::se::GetInt(disp->getDisplacementX()));
    dispSize.setHeight(te::se::GetInt(disp->getDisplacementY()));
  }

  te::qt::widgets::AbstractPropertyManager::getInstance().m_sizeManager->setValue(m_displacementProperty, dispSize);

  //update rotation
  int rotationValue = 0;

  if (m_pointPlacement->getRotation())
    rotationValue = te::se::GetInt(m_pointPlacement->getRotation());  

  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_rotationProperty, rotationValue);
  

  m_update = false;
}

