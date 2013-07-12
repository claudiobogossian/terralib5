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

/*!
  \file terralib/qt/widgets/canvas/EyeBirdMapDisplayWidget.cpp

  \brief This file has the EyeBirdMapDisplayWidget class.
*/

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../canvas/MapDisplay.h"
#include "../tools/PanExtent.h"
#include "EyeBirdMapDisplayWidget.h"

// Qt
#include <QtGui/QGridLayout>


#define PATTERN_SIZE 14

te::qt::widgets::EyeBirdMapDisplayWidget::EyeBirdMapDisplayWidget(te::qt::widgets::MapDisplay* mapParent, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f), m_parentMapDisplay(mapParent), m_itsMe(false), m_isEnabled(true)
{
//build form
  QGridLayout* displayLayout = new QGridLayout(this);
  m_mapDisplay = new te::qt::widgets::MapDisplay(this->size(), this);
  displayLayout->addWidget(m_mapDisplay);
  displayLayout->setContentsMargins(0,0,0,0);

  m_panExtent = new te::qt::widgets::PanExtent(m_mapDisplay, Qt::OpenHandCursor, Qt::ClosedHandCursor);

  connect(m_panExtent, SIGNAL(extentMoved(te::gm::Envelope)), this, SLOT(onExtentMoved(te::gm::Envelope)));
  connect(m_parentMapDisplay, SIGNAL(extentChanged()), this, SLOT(onParentMapDisplayExtentChanged()));

  m_mapDisplay->installEventFilter(m_panExtent);
}

te::qt::widgets::EyeBirdMapDisplayWidget::~EyeBirdMapDisplayWidget()
{
  delete m_panExtent;
}

void te::qt::widgets::EyeBirdMapDisplayWidget::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  std::list<te::map::AbstractLayerPtr> list;

  list.push_back(m_layer);

  te::gm::Envelope inputExt = m_layer->getExtent();

  m_mapDisplay->setMouseTracking(true);
  m_mapDisplay->setLayerList(list);
  m_mapDisplay->setSRID(m_layer->getSRID(), false);
  m_mapDisplay->setExtent(inputExt, m_isEnabled);

  if(m_isEnabled)
    m_panExtent->setCurrentExtent(inputExt);
}

void te::qt::widgets::EyeBirdMapDisplayWidget::recompose()
{
  if(!m_isEnabled)
    return;

  te::gm::Envelope inputExt = m_layer->getExtent();
  m_mapDisplay->setExtent(inputExt, true);
  m_panExtent->setCurrentExtent(inputExt);
}

void te::qt::widgets::EyeBirdMapDisplayWidget::setEnabled(bool status)
{
  m_isEnabled = status;

  recompose();
}

void te::qt::widgets::EyeBirdMapDisplayWidget::onExtentMoved(te::gm::Envelope e)
{
  m_itsMe = true;
  m_parentMapDisplay->setExtent(e);
  m_itsMe = false;
}

void te::qt::widgets::EyeBirdMapDisplayWidget::onParentMapDisplayExtentChanged()
{
  if(m_itsMe == false)
  {
    te::gm::Envelope e = m_parentMapDisplay->getExtent();

    if(e.isValid() && m_isEnabled)
      m_panExtent->setCurrentExtent(e);
  }
}
