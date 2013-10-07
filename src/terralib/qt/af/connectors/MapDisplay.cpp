/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
#include "../../widgets/layer/explorer/LayerExplorer.h"
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
#include "LayerExplorer.h"
#include "MapDisplay.h"

// Qt
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMessageBox>

// STL
#include <cassert>
#include <utility>

#define EXTENT_STACK_SIZE 5

te::qt::af::MapDisplay::MapDisplay(te::qt::widgets::MapDisplay* display, te::qt::af::LayerExplorer* explorer)
  : QObject(display),
    m_display(display),
    m_explorer(explorer),
    m_tool(0),
    m_menu(0),
    m_currentExtent(-1)
{
  // CoordTracking tool
  te::qt::widgets::CoordTracking* coordTracking = new te::qt::widgets::CoordTracking(m_display, this);
  connect(coordTracking, SIGNAL(coordTracked(QPointF&)), SLOT(onCoordTracked(QPointF&)));
  m_display->installEventFilter(coordTracking);

  // Zoom Wheel tool
  m_display->installEventFilter(new te::qt::widgets::ZoomWheel(m_display, 2.0, this));

  // Signals & slots
  connect(m_display, SIGNAL(drawLayersFinished(const QMap<QString, QString>&)), SLOT(onDrawLayersFinished(const QMap<QString, QString>&)));
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));

  // Gets the popup menu
  m_menu = ApplicationController::getInstance().findMenu("Map");

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

te::qt::widgets::MapDisplay* te::qt::af::MapDisplay::getDisplay()
{
  return m_display;
}

te::qt::af::LayerExplorer* te::qt::af::MapDisplay::getLayerExplorer() const
{
  return m_explorer;
}

bool te::qt::af::MapDisplay::eventFilter(QObject* /*watched*/, QEvent* e)
{
  switch(e->type())
  {
    case QEvent::ContextMenu:
      if(m_menu)
        m_menu->exec(static_cast<QContextMenuEvent*>(e)->globalPos());
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

  std::list<te::map::AbstractLayerPtr> visibleLayers;
  te::map::GetVisibleLayers(layers, visibleLayers);

  configSRS(visibleLayers);

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

void te::qt::af::MapDisplay::nextExtent()
{
  if(m_currentExtent != EXTENT_STACK_SIZE-1 && m_extentStack.size() != m_currentExtent+1)
  {
    m_currentExtent += 1;
    m_display->setExtent(m_extentStack[m_currentExtent]);
  }
}

void te::qt::af::MapDisplay::previousExtent()
{
  if(m_currentExtent > 0)
  {
    m_currentExtent -= 1;
    m_display->setExtent(m_extentStack[m_currentExtent]);
  }
}

void te::qt::af::MapDisplay::fit(const std::list<te::map::AbstractLayerPtr>& layers)
{
  std::list<te::map::AbstractLayerPtr> visibleLayers;
  te::map::GetVisibleLayers(layers, visibleLayers);

  configSRS(visibleLayers);

  te::gm::Envelope displayExtent = te::map::GetExtent(layers, m_display->getSRID(), true);

  m_display->setExtent(displayExtent, false);

  m_display->setLayerList(layers);

  m_display->refresh();
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

  // Draw the layers selection
  //drawLayersSelection(ApplicationController::getInstance().getProject()->getLayers());
  drawLayersSelection(m_explorer->getExplorer()->getSelectedAndVisibleLayers());
}

void te::qt::af::MapDisplay::onApplicationTriggered(te::qt::af::evt::Event* e)
{
  switch(e->m_id)
  {
    case te::qt::af::evt::PROJECT_ADDED:
      clear();
    break;

    case te::qt::af::evt::LAYER_SELECTED_OBJECTS_CHANGED:
    {
      //te::qt::af::evt::LayerSelectedObjectsChanged* LayerSelectedObjectsChanged = static_cast<te::qt::af::evt::LayerSelectedObjectsChanged*>(e);

      QPixmap* content = m_display->getDisplayPixmap();
      content->fill(Qt::transparent);

      QPainter painter(content);
      painter.drawPixmap(0, 0, m_lastDisplayContent);
      painter.end();

      //drawLayersSelection(ApplicationController::getInstance().getProject()->getLayers());
      drawLayersSelection(m_explorer->getExplorer()->getSelectedAndVisibleLayers());
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

void te::qt::af::MapDisplay::drawLayersSelection(const std::list<te::map::AbstractLayerPtr>& layers)
{
  if(layers.empty())
    return;

  std::list<te::map::AbstractLayerPtr>::const_iterator it;
  for(it = layers.begin(); it != layers.end(); ++it)
      drawLayerSelection(*it);

  m_display->repaint();
}

//void te::qt::af::MapDisplay::drawLayersSelection(const std::list<te::map::AbstractLayerPtr>& layers)
//{
//  std::list<te::map::AbstractLayerPtr>::const_iterator it;
//  for(it = layers.begin(); it != layers.end(); ++it)
//  {
//    te::map::AbstractLayerPtr layer = it->get();
//    if(layer->getType() == "FOLDERLAYER")
//    {
//      const std::list<te::common::TreeItemPtr>& items = layer->getChildren();
//
//      std::list<te::map::AbstractLayerPtr> childrenLayers;
//
//      std::list<te::common::TreeItemPtr>::const_iterator layerIt;
//      for(layerIt = items.begin(); layerIt != items.end(); ++layerIt)
//        childrenLayers.push_back(static_cast<te::map::AbstractLayer*>(layerIt->get()));
//
//      drawLayersSelection(childrenLayers);
//    }
//    else
//      drawLayerSelection(layer);
//  }
//
//  m_display->repaint();
//}

void te::qt::af::MapDisplay::drawLayerSelection(te::map::AbstractLayerPtr layer)
{
  assert(layer.get());

  //if(layer->getVisibility() != te::map::VISIBLE)
  //  return;

  const te::da::ObjectIdSet* oids = layer->getSelected();
  if(oids == 0 || oids->size() == 0)
    return;

  bool needRemap = false;

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (m_display->getSRID() != TE_UNKNOWN_SRS) && (layer->getSRID() != m_display->getSRID()))
    needRemap = true;

  // Try to retrieve the layer selection
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
}

void te::qt::af::MapDisplay::onExtentChanged()
{
  if(!m_extentStack.empty() && m_display->getExtent().equals(m_extentStack[m_currentExtent]))
    return;

  if(m_currentExtent != EXTENT_STACK_SIZE)
  {
    std::vector<te::gm::Envelope> aux;

    if(m_currentExtent == -1)
    {
      aux.push_back(m_display->getExtent());
      m_extentStack = aux;
      m_currentExtent += 1;
    }
    else
    {
      for(std::size_t i = 0; i <= m_currentExtent; ++i)
      {
        aux.push_back(m_extentStack[i]);
      }
      aux.push_back(m_display->getExtent());

      m_extentStack = aux;
      m_currentExtent += 1;
    }
  }
  else
  {
    std::vector<te::gm::Envelope> aux;
    for(std::size_t i = 0; i <= m_currentExtent; ++i)
    {
      if(i == 0)
        continue;

      aux.push_back(m_extentStack[i]);
    }
    aux.push_back(m_display->getExtent());

    m_extentStack = aux;
    m_currentExtent = EXTENT_STACK_SIZE;
  }
}

void te::qt::af::MapDisplay::configSRS(const std::list<te::map::AbstractLayerPtr>& layers)
{
  if(layers.size() == 1 && (*layers.begin())->getSRID() ==  TE_UNKNOWN_SRS && m_display->getSRID() != TE_UNKNOWN_SRS)
  {
    const te::map::AbstractLayerPtr& layer = *layers.begin();

    m_display->setSRID(TE_UNKNOWN_SRS, false);

    std::pair<int, std::string> srid(layer->getSRID(), "EPSG");
    te::qt::af::evt::MapSRIDChanged mapSRIDChanged(srid);
    ApplicationController::getInstance().broadcast(&mapSRIDChanged);
  }
  else if(m_display->getSRID() == TE_UNKNOWN_SRS)
  {
    std::list<te::map::AbstractLayerPtr>::const_iterator it;

    for(it = layers.begin(); it != layers.end(); ++it)
    {
      const te::map::AbstractLayerPtr& layer = *it;

      if(layer->getSRID() == TE_UNKNOWN_SRS)
        continue;

      m_display->setSRID(layer->getSRID(), false);

      std::pair<int, std::string> srid(layer->getSRID(), "EPSG");
      te::qt::af::evt::MapSRIDChanged mapSRIDChanged(srid);
      ApplicationController::getInstance().broadcast(&mapSRIDChanged);

      break;
    }
  }
}
