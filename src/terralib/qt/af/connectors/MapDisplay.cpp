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
  \file terralib/qt/af/connectors/MapDisplay.cpp

  \brief A connector of the te::qt::widgets::MapDisplay class to the Application Framework.
*/

// TerraLib
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Utils.h"
#include "../../../srs/Config.h"
#include "../../widgets/canvas/MapDisplay.h"
#include "../../widgets/tools/AbstractTool.h"
#include "../../widgets/tools/ZoomWheel.h"
#include "../../widgets/tools/CoordTracking.h"
#include "../events/ProjectEvents.h"
#include "../ApplicationController.h"
#include "../Project.h"
#include "MapDisplay.h"

// Qt
#include <QtGui/QContextMenuEvent>

te::qt::af::MapDisplay::MapDisplay(te::qt::widgets::MapDisplay* display)
  : QObject(display),
    m_display(display),
    m_tool(0)
{
  // CoordTracking tool
  te::qt::widgets::CoordTracking* coordTracking = new te::qt::widgets::CoordTracking(m_display, this);
  m_display->installEventFilter(coordTracking);

  // Zoom Wheel tool
  m_display->installEventFilter(new te::qt::widgets::ZoomWheel(m_display, 2.0, this));

  // Build the popup menu
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Draw"));
  m_menu.addSeparator();
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Zoom In"));
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Zoom Out"));
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Zoom Area"));
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Pan"));
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Zoom Extent")); 
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Previous Extent"));
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Next Extent"));
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Info"));
  m_menu.addSeparator();
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Stop Draw"));

  // To show popup menu
  m_display->installEventFilter(this);
}

te::qt::af::MapDisplay::~MapDisplay()
{
  delete m_tool;
}

te::qt::widgets::MapDisplay*  te::qt::af::MapDisplay::getDisplay()
{
  return m_display;
}

bool te::qt::af::MapDisplay::eventFilter(QObject* /*watched*/, QEvent* e)
{
  switch(e->type())
  {
    case QEvent::ContextMenu:
      m_menu.exec(static_cast<QContextMenuEvent*>(e)->globalPos());
    break;
          
    default:
    break;
  }

  return false;
}

void te::qt::af::MapDisplay::draw(const std::list<te::map::AbstractLayerPtr>& layers)
{
  if(layers.empty())
    return;

  std::list<te::map::AbstractLayerPtr>::const_iterator it;

  /* It adjusts the map display SRID. 
     This code try find the first valid SRID. Need review! */
  if(m_display->getSRID() == TE_UNKNOWN_SRS)
  {
    for(it = layers.begin(); it != layers.end(); ++it)
    {
      const te::map::AbstractLayerPtr& layer = *it;

      if(layer->getSRID() == TE_UNKNOWN_SRS)
        continue;

      m_display->setSRID(layer->getSRID(), false);

      break;
    }
  }

  /* It adjusts the map display extent.
     This code calculates an extent that covers all visible layers. Need review! */
  if(!m_display->getExtent().isValid())
  {
    te::gm::Envelope displayExtent;
    for(it = layers.begin(); it != layers.end(); ++it)
    {
      const te::map::AbstractLayerPtr& layer = *it;
      if(layer->getVisibility() == te::map::NOT_VISIBLE)
        continue;

      te::gm::Envelope e(layer->getExtent());

      if((layer->getSRID() != TE_UNKNOWN_SRS) && (m_display->getSRID() != TE_UNKNOWN_SRS))
        e.transform(layer->getSRID(), m_display->getSRID());

      displayExtent.Union(e);
    }
    m_display->setExtent(displayExtent, false);
  }
  
  m_display->setLayerList(layers);
  m_display->refresh();
}

void te::qt::af::MapDisplay::clear()
{
  std::list<te::map::AbstractLayerPtr> empty;
  m_display->setLayerList(empty);
  m_display->refresh();
}

void te::qt::af::MapDisplay::setCurrentTool(te::qt::widgets::AbstractTool* tool)
{
  delete m_tool;
  m_tool = tool;

  m_display->installEventFilter(m_tool);
}

void te::qt::af::MapDisplay::onApplicationTriggered(te::qt::af::evt::Event* e)
{
  switch (e->m_id)
  {
    case te::qt::af::evt::PROJECT_ADDED:
      clear();
    break;
  }
}
