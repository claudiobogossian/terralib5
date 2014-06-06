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
  \file terralib/qt/plugins/layout/TimeSliderWidgetAction.cpp

  \brief This file defines the TimeSliderWidgetAction class
*/

// Terralib
#include "../../../qt/widgets/st/TimeSliderWidget.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../af/ApplicationController.h"
#include "../../af/BaseApplication.h"
#include "../../af/connectors/MapDisplay.h"
#include "../../af/Project.h"
#include "TimeSliderWidgetAction.h"

te::qt::plugins::st::TimeSliderWidgetAction::TimeSliderWidgetAction(QMenu* menu)
  : te::qt::plugins::st::AbstractAction(menu),
  m_timeSliderWidget(0)
{
  createAction(tr("Time Slider...").toStdString());
}

te::qt::plugins::st::TimeSliderWidgetAction::~TimeSliderWidgetAction()
{
  if(m_timeSliderWidget)
  {
    te::qt::af::ApplicationController::getInstance().removeListener(m_timeSliderWidget);
    delete m_timeSliderWidget;
  }
}

void te::qt::plugins::st::TimeSliderWidgetAction::onActionActivated(bool checked)
{
  if(m_timeSliderWidget == 0)
  {
    QWidget* parent = te::qt::af::ApplicationController::getInstance().getMainWindow();
    te::qt::af::BaseApplication* ba = (te::qt::af::BaseApplication*)parent;
    te::qt::widgets::MapDisplay* display = ba->getDisplay()->getDisplay();
    if(display->getExtent().isValid() == false)
    {
      // WGS84
      te::gm::Envelope e(-180, -180, 360, 360);
      display->setExtent(e);
      display->setSRID(4326);
    }
    te::qt::widgets::TimeSliderWidget* ts = new te::qt::widgets::TimeSliderWidget(display, parent, Qt::Window);
    ts->show();
    ts->move(200, 200);
    connect(ts, SIGNAL(deleteTimeSliderWidget()), this, SLOT(onDeleteTimeSliderWidget()));
    connect(ba, SIGNAL(applicationClose()), this, SLOT(onDeleteTimeSliderWidget()));
    m_timeSliderWidget = new te::qt::af::TimeSliderWidget(ts);
  }
  else
  {
    if(m_timeSliderWidget->getTimeSliderWidget()->isMinimized())
      m_timeSliderWidget->getTimeSliderWidget()->showNormal();
    else
      m_timeSliderWidget->getTimeSliderWidget()->show();
  }
}

void te::qt::plugins::st::TimeSliderWidgetAction::onDeleteTimeSliderWidget()
{
  if(m_timeSliderWidget)
    delete m_timeSliderWidget;
  m_timeSliderWidget = 0;
}
