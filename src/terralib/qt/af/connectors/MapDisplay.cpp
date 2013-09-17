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
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Utils.h"
#include "../../../maptools/Utils.h"
#include "../../../srs/Config.h"
#include "../../widgets/canvas/Canvas.h"
#include "../../widgets/canvas/MapDisplay.h"
#include "../../widgets/tools/AbstractTool.h"
#include "../../widgets/tools/ZoomWheel.h"
#include "../../widgets/tools/CoordTracking.h"
#include "../../widgets/Utils.h"
#include "../events/LayerEvents.h"
#include "../events/MapEvents.h"
#include "../events/ProjectEvents.h"
#include "../events/ToolEvents.h"
#include "../ApplicationController.h"
#include "../Project.h"
#include "../Utils.h"
#include "MapDisplay.h"

// Qt
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMessageBox>

// STL
#include <cassert>
#include <utility>

te::qt::af::MapDisplay::MapDisplay(te::qt::widgets::MapDisplay* display)
  : QObject(display),
    m_display(display),
    m_tool(0)
{
  // CoordTracking tool
  te::qt::widgets::CoordTracking* coordTracking = new te::qt::widgets::CoordTracking(m_display, this);
  connect(coordTracking, SIGNAL(coordTracked(QPointF&)), SLOT(onCoordTracked(QPointF&)));
  m_display->installEventFilter(coordTracking);

  // Zoom Wheel tool
  m_display->installEventFilter(new te::qt::widgets::ZoomWheel(m_display, 2.0, this));

  // Signals & slots
  connect(m_display, SIGNAL(drawLayersFinished(const QMap<QString, QString>&)), SLOT(onDrawLayersFinished(const QMap<QString, QString>&)));

  // Build the popup menu
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.SRID"));
  m_menu.addSeparator();
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Draw"));
  m_menu.addSeparator();
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Zoom In"));
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Zoom Out"));
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Pan"));
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Zoom Extent")); 
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Previous Extent"));
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Next Extent"));
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Info"));
  m_menu.addSeparator();
  m_menu.addAction(ApplicationController::getInstance().findAction("Map.Stop Draw"));

  // To show popup menu
  m_display->installEventFilter(this);

  // Config the default SRS
  m_display->setSRID(ApplicationController::getInstance().getDefaultSRID(), false);
  
  // Getting default display color
  m_display->setBackgroundColor(te::qt::af::GetDefaultDisplayColorFromSettings());
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

  if(m_display->getSRID() == TE_UNKNOWN_SRS)
  {
    for(it = layers.begin(); it != layers.end(); ++it)
    {
      const te::map::AbstractLayerPtr& layer = *it;

      if(!layer->getVisibility() || layer->getSRID() == TE_UNKNOWN_SRS)
        continue;

      m_display->setSRID(layer->getSRID(), false);

      std::pair<int, std::string> srid(layer->getSRID(), "EPSG");
      te::qt::af::evt::MapSRIDChanged mapSRIDChagned(srid);
      ApplicationController::getInstance().broadcast(&mapSRIDChagned);

      break;
    }
  }

  if(!m_display->getExtent().isValid())
  {
    te::gm::Envelope displayExtent = te::map::GetExtent(layers, m_display->getSRID(), true);
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

void te::qt::af::MapDisplay::onCoordTracked(QPointF& coordinate)
{
  te::qt::af::evt::CoordinateTracked e(coordinate.x(), coordinate.y());
  ApplicationController::getInstance().broadcast(&e);
}

void te::qt::af::MapDisplay::onDrawLayersFinished(const QMap<QString, QString>& /*errors*/)
{
  // Stores the clean pixmap!
  m_lastDisplayContent = QPixmap(*m_display->getDisplayPixmap());

  // TODO!!!
  drawLayerSelection((ApplicationController::getInstance().getProject()->getLayers().begin())->get());
}

void te::qt::af::MapDisplay::onApplicationTriggered(te::qt::af::evt::Event* e)
{
  switch(e->m_id)
  {
    case te::qt::af::evt::PROJECT_ADDED:
      clear();
    break;

    case te::qt::af::evt::LAYER_SELECTION_CHANGED:
    {
      te::qt::af::evt::LayerSelectionChanged* layerSelectionChanged = static_cast<te::qt::af::evt::LayerSelectionChanged*>(e);

      QPixmap* content = m_display->getDisplayPixmap();
      content->fill(Qt::transparent);

      QPainter painter(content);
      painter.drawPixmap(0, 0, m_lastDisplayContent);
      painter.end();

      drawLayerSelection(layerSelectionChanged->m_layer);
    }
    break;

    case te::qt::af::evt::LAYER_VISIBILITY_CHANGED:
    {
      // TODO
    }
    break;

    case te::qt::af::evt::MAP_COLOR_CHANGED:
    {
      te::qt::af::evt::MapColorChanged* mapColorChanged = static_cast<te::qt::af::evt::MapColorChanged*>(e);
      m_display->setBackgroundColor(mapColorChanged->m_color);
      m_display->refresh();
    }
    break;

    default:
      return;
  }
}

void te::qt::af::MapDisplay::drawLayerSelection(te::map::AbstractLayer* layer)
{
  assert(layer);

  if(layer->getVisibility() != te::map::VISIBLE)
    return;

  const te::da::ObjectIdSet* oids = layer->getSelected();
  if(oids == 0 || oids->size() == 0)
  {
    m_display->repaint();
    return;
  }

  bool needRemap = false;

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (m_display->getSRID() != TE_UNKNOWN_SRS) && (layer->getSRID() != m_display->getSRID()))
    needRemap = true;

  // Try retrieves the layer selection
  std::auto_ptr<te::da::DataSet> selected;
  try
  {
    selected = layer->getData(oids);
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The layer selection cannot be drawn. Details:") + " %1.").arg(e.what()));
    return;
  }

  std::size_t gpos = te::da::GetFirstPropertyPos(selected.get(), te::dt::GEOMETRY_TYPE);

  QPixmap* content = m_display->getDisplayPixmap();

  const te::gm::Envelope& displayExtent = m_display->getExtent();

  te::qt::widgets::Canvas canvas(content);
  canvas.setWindow(displayExtent.m_llx, displayExtent.m_lly, displayExtent.m_urx, displayExtent.m_ury);

  te::gm::GeomType currentGeomType = te::gm::UnknownGeometryType;

  while(selected->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> g(selected->getGeometry(gpos));

    if(needRemap)
    {
      g->setSRID(layer->getSRID());
      g->transform(m_display->getSRID());
    }

    if(currentGeomType != g->getGeomTypeId())
    {
      currentGeomType = g->getGeomTypeId();
      te::qt::widgets::Config2DrawLayerSelection(&canvas, ApplicationController::getInstance().getSelectionColor(), currentGeomType);
    }

    canvas.draw(g.get());
  }

  m_display->repaint();
}
