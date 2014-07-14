/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*! 
  \file terralib/qt/af/connectors/TimeSliderWidget.cpp

  \brief A connector of the te::qt::widgets::TimeSliderWidget used in the Application Framework.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Utils.h"
#include "../../../maptools/Utils.h"
#include "../../../srs/Config.h"
#include "../../../st/maptools/TrajectoryDataSetLayer.h"
#include "../../../st/core/trajectory/TrajectoryDataSet.h"
#include "../../widgets/st/TimeSliderWidget.h"
#include "../../widgets/Utils.h"
#include "../events/LayerEvents.h"
#include "../events/MapEvents.h"
#include "../events/ProjectEvents.h"
#include "../events/ToolEvents.h"
#include "../ApplicationController.h"
#include "../Project.h"
#include "../Utils.h"
#include "TimeSliderWidget.h"

// Qt
#include <QContextMenuEvent>
#include <QMessageBox>

// STL
#include <cassert>
#include <utility>

te::qt::af::TimeSliderWidget::TimeSliderWidget(te::qt::widgets::TimeSliderWidget* t)
  : QObject(),
    m_timeSliderWidget(t)
{
  ApplicationController::getInstance().addListener(this);
}

te::qt::af::TimeSliderWidget::~TimeSliderWidget()
{
  ApplicationController::getInstance().removeListener(this);
  delete m_timeSliderWidget;
}

te::qt::widgets::TimeSliderWidget*  te::qt::af::TimeSliderWidget::getTimeSliderWidget()
{
  return m_timeSliderWidget;
}

void te::qt::af::TimeSliderWidget::onApplicationTriggered(te::qt::af::evt::Event* e)
{
  switch(e->m_id)
  {
    case te::qt::af::evt::APP_CLOSED:
    {
      if(m_timeSliderWidget)
        m_timeSliderWidget->deleteMe();
    }
    break;

    case te::qt::af::evt::LAYER_ADDED:
      {
        te::qt::af::evt::LayerAdded* ev = static_cast<te::qt::af::evt::LayerAdded*>(e);

        if(ev->m_layer->getType() == "TRAJECTORYDATASETLAYER")
        {
          te::st::TrajectoryDataSet* test = dynamic_cast<te::st::TrajectoryDataSetLayer*>(ev->m_layer.get())->getTrajectoryDataset().release();
          m_timeSliderWidget->addTrajectory(QString::fromStdString(test->getId()), "C:/Users/andre.oliveira/Funcate/Projetos/Fontes/Terralib/source/resources/images/terralib.png", test);
        }
      }
    break;
    default:
      return;
  }
}

