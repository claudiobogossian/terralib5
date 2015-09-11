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
//#include "../../af/connectors/InterfaceController.h"
#include "../../af/ApplicationController.h"
#include "../../../qt/af/events/LayerEvents.h"
//#include "../../af/Project.h"
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
    te::qt::af::AppCtrlSingleton::getInstance().removeListener(m_timeSliderWidget);
    delete m_timeSliderWidget;
  }
  m_menu->removeAction(m_action);
  delete m_action;
}

void te::qt::plugins::st::TimeSliderWidgetAction::onActionActivated(bool checked)
{
  if (m_timeSliderWidget == 0)
  {
    //Create the time slider
    QWidget* mainWindow = te::qt::af::AppCtrlSingleton::getInstance().getMainWindow();
    te::qt::af::BaseApplication* ba = (te::qt::af::BaseApplication*)mainWindow;
    te::qt::widgets::MapDisplay* display = ba->getMapDisplay();
    m_timeSliderWidget = new te::qt::widgets::TimeSliderWidget(display, display);
    connect(m_timeSliderWidget, SIGNAL(deleteTimeSliderWidget()), this, SLOT(onDeleteTimeSliderWidget()));
//    connect(ba, SIGNAL(applicationClose()), this, SLOT(onDeleteTimeSliderWidget())); alguem j� providencia isso mesmo sem ter este coonect
    te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::BOTH);
  }

  if (m_timeSliderWidget->isHidden())
    m_timeSliderWidget->showWidget(true);
  else
    m_timeSliderWidget->showWidget(false);
}

void te::qt::plugins::st::TimeSliderWidgetAction::onDeleteTimeSliderWidget()
{
  if(m_timeSliderWidget)
    delete m_timeSliderWidget;
  m_timeSliderWidget = 0;
}

void te::qt::plugins::st::TimeSliderWidgetAction::onApplicationTriggered(te::qt::af::evt::Event* e)
{
  switch (e->m_id)
  {
  case te::qt::af::evt::LAYER_VISIBILITY_CHANGED:
  {
    int a = 0;
  }
  break;

  case te::qt::af::evt::LAYER_ADDED:
  {
    te::qt::af::evt::LayerSelected* ev = static_cast<te::qt::af::evt::LayerSelected*>(e);
    te::map::AbstractLayerPtr layer = static_cast<te::map::AbstractLayerPtr>(ev->m_layer);
    if (layer->getType() == "TRAJECTORYDATASETLAYER")
      m_timeSliderWidget->layerAdded(layer);
  }
  break;

  case te::qt::af::evt::LAYER_REMOVED:
  {
    te::qt::af::evt::LayerRemoved* ev = static_cast<te::qt::af::evt::LayerRemoved*>(e);
    std::list<te::map::AbstractLayerPtr> layers = ev->m_layers;
    std::list<te::map::AbstractLayerPtr>::iterator it;

    for (it = layers.begin(); it != layers.end(); ++it)
    {
      if ((*it)->getType() == "TRAJECTORYDATASETLAYER")
        m_timeSliderWidget->layerRemoved(*it);
    }
  }
  break;

  case te::qt::af::evt::LAYER_SELECTED:
  {
    int a = 0;
  }
  break;

  case te::qt::af::evt::LAYER_CHANGED:
  {
    int a = 0;
  }
  break;

  default:
    return;
  }
}

