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
  \file terralib/qt/plugins/layout/TimeSliderWidgetAction.cpp

  \brief This file defines the TimeSliderWidgetAction class
*/

// Terralib
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../af/ApplicationController.h"
#include "../../af/BaseApplication.h"
#include "../../af/connectors/MapDisplay.h"
#include "../../af/connectors/InterfaceController.h"
#include "../../af/Project.h"
#include "TimeSliderWidgetAction.h"

te::qt::plugins::st::TimeSliderWidgetAction::TimeSliderWidgetAction(QMenu* menu)
  : te::qt::plugins::st::AbstractAction(menu),
  m_timeSliderWidget(0)
{
  createAction(tr("Time Slider...").toStdString());

  //GEnerating the slider
  QWidget* mainWindow = te::qt::af::ApplicationController::getInstance().getMainWindow();
  te::qt::af::BaseApplication* ba = (te::qt::af::BaseApplication*)mainWindow;
  te::qt::widgets::MapDisplay* display = ba->getDisplay()->getDisplay();
  //if(display->getExtent().isValid() == false)
  //{
  //  // WGS84
  //  te::gm::Envelope e(-180, -180, 360, 360);
  //  display->setExtent(e);
  //  display->setSRID(4326);
  //}
  m_timeSliderWidget = new te::qt::widgets::TimeSliderWidget(display, display);
  connect(m_timeSliderWidget, SIGNAL(deleteTimeSliderWidget()), this, SLOT(onDeleteTimeSliderWidget()));
  connect(ba, SIGNAL(applicationClose()), this, SLOT(onDeleteTimeSliderWidget()));

  ba->getInterfaceController()->addInterface(m_timeSliderWidget);
  m_timeSliderWidget->hide();
}

te::qt::plugins::st::TimeSliderWidgetAction::~TimeSliderWidgetAction()
{
  if(m_timeSliderWidget)
  {
    QWidget* mainWindow = te::qt::af::ApplicationController::getInstance().getMainWindow();
    te::qt::af::BaseApplication* ba = (te::qt::af::BaseApplication*)mainWindow;

    ba->getInterfaceController()->removeInteface(m_timeSliderWidget);
    delete m_timeSliderWidget;
  }
  m_menu->removeAction(m_action);
  delete m_action;
}

void te::qt::plugins::st::TimeSliderWidgetAction::onActionActivated(bool checked)
{
  if(m_timeSliderWidget->isHidden())
  {
    m_timeSliderWidget->show();
  }
  else
  {
    m_timeSliderWidget->hide();
  }
}

void te::qt::plugins::st::TimeSliderWidgetAction::onDeleteTimeSliderWidget()
{
  if(m_timeSliderWidget)
    delete m_timeSliderWidget;
  m_timeSliderWidget = 0;
}
