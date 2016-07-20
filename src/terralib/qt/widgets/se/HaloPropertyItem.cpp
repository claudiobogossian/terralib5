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
\file terralib/qt/widgets/se/HaloPropertyItem.cpp

  \brief A widget used to define the halo se object.
*/


#include "../../../common/StringUtils.h"
#include "../../../se/SvgParameter.h"
#include "../../../se/Fill.h"
#include "../../../se/Halo.h"
#include "../../../se/Utils.h"
#include "../propertybrowser/AbstractPropertyManager.h"
#include "BasicFillPropertyItem.h"
#include "HaloPropertyItem.h"

// STL
#include <cassert>

// QtPropertyBrowser


te::qt::widgets::HaloPropertyItem::HaloPropertyItem(QtTreePropertyBrowser* pb, QColor c) : te::qt::widgets::AbstractPropertyItem(pb, c),
  m_halo(new te::se::Halo)
{
  //build property browser basic fill
  QtProperty* haloProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Halo Properties"));

  //label
  QStringList emptyList;
  m_radiusProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->addProperty(tr("Radius"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setMinimum(m_radiusProperty, 0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setMaximum(m_radiusProperty, 99);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(m_radiusProperty, 8);
  haloProperty->addSubProperty(m_radiusProperty);

  addProperty(haloProperty, tr("Halo Properties"), QColor(152, 252, 36));

  m_bf = new te::qt::widgets::BasicFillPropertyItem(m_propertyBrowser, QColor(152, 252, 36));

  connect(m_bf, SIGNAL(fillChanged()), SLOT(onFillChanged()));

  // Setups initial fill
  m_halo->setFill(m_bf->getFill());

  m_update = true;
}

te::qt::widgets::HaloPropertyItem::~HaloPropertyItem()
{
  delete m_halo;
}

void te::qt::widgets::HaloPropertyItem::valueChanged(QtProperty* p, int value)
{
  if (!m_update)
    return;

  if (p == m_radiusProperty)
  {
    m_update = false;

    m_halo->setRadius(new te::se::ParameterValue(QString::number(value).toStdString()));
    
    emit haloChanged();
    m_update = true;
  }
}

void te::qt::widgets::HaloPropertyItem::onFillChanged()
{
  m_halo->setFill(m_bf->getFill());

  emit haloChanged();
}

void te::qt::widgets::HaloPropertyItem::setHalo(const te::se::Halo* halo)
{
  assert(halo);

  // Verifying if this widget can deal with the given fill...
  delete m_halo;

  m_halo = dynamic_cast<te::se::Halo*>(halo->clone());

  updateUi();
  m_update = true;
}

te::se::Halo* te::qt::widgets::HaloPropertyItem::getHalo() const
{
  return dynamic_cast<te::se::Halo*>(m_halo->clone());
}

void te::qt::widgets::HaloPropertyItem::resetHalo()
{
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(m_radiusProperty, 8);
}

void te::qt::widgets::HaloPropertyItem::updateUi()
{
  m_update = true;

  //update radius
  int radius = te::se::GetInt(m_halo->getRadius());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(m_radiusProperty, radius);

  //update fill
  if (m_halo->getFill())
    m_bf->setFill(m_halo->getFill());

  m_update = false;
}

