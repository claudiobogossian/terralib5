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
  \file terralib/qt/af/connectors/LayoutEditor.cpp

  \brief A connector of the te::qt::widgets::MultiThreadMapDisplay used in te::qt::widgets::DataFrame class to the Application Framework.
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
#include "../../widgets/layout/LayoutEditor.h"
#include "../../widgets/layout/DataFrame.h"
#include "../../widgets/canvas/MultiThreadMapDisplay.h"
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
#include "LayoutEditor.h"

// Qt
#include <QContextMenuEvent>
#include <QMessageBox>

// STL
#include <cassert>
#include <utility>

#define EXTENT_STACK_SIZE 5

te::qt::af::LayoutEditor::LayoutEditor(te::qt::widgets::LayoutEditor* le)
  : QObject(),
    m_layoutEditor(le),
    m_tool(0)
{
  //// Zoom Wheel tool
  //m_mapDisplay->installEventFilter(new te::qt::widgets::ZoomWheel(m_mapDisplay, 2.0, this));
 
  // Signals & slots
  //connect((QObject*)m_layoutEditor, SIGNAL(drawLayersFinished(const QMap<QString, QString>&)), SLOT(onDrawLayersFinished(const QMap<QString, QString>&)));

  ApplicationController::getInstance().addListener(this);
}

te::qt::af::LayoutEditor::~LayoutEditor()
{
  delete m_layoutEditor;
  delete m_tool;
}

te::qt::widgets::LayoutEditor*  te::qt::af::LayoutEditor::getLayoutEditor()
{
  return m_layoutEditor;
}

void te::qt::af::LayoutEditor::setCurrentTool(te::qt::widgets::AbstractTool* tool)
{
  //delete m_tool;
  //m_tool = tool;

  //m_layoutEditor->installEventFilter(m_tool);
}

void te::qt::af::LayoutEditor::onApplicationTriggered(te::qt::af::evt::Event* e)
{
  switch(e->m_id)
  {
    case te::qt::af::evt::LAYER_SELECTED_OBJECTS_CHANGED:
    {
      QColor selColor = ApplicationController::getInstance().getSelectionColor();
      m_layoutEditor->setSelectionColor(selColor);
      m_layoutEditor->drawLayersSelection();
    }
    break;

    case te::qt::af::evt::MAP_COLOR_CHANGED:
    {
      te::qt::af::evt::MapColorChanged* mapColorChanged = static_cast<te::qt::af::evt::MapColorChanged*>(e);

      m_layoutEditor->setDisplayBackgroundColor(mapColorChanged->m_color);
    }
    break;

    case te::qt::af::evt::DRAW_BUTTON_CLICKED:
    {
      QColor selColor = ApplicationController::getInstance().getSelectionColor();
      m_layoutEditor->setSelectionColor(selColor);
      m_layoutEditor->drawButtonClicked();
    }
    break;

    case te::qt::af::evt::ZOOM_IN_BUTTON_TOGGLED:
    {
      m_layoutEditor->setMouseMode(1);
    }
    break;

    case te::qt::af::evt::ZOOM_OUT_BUTTON_TOGGLED:
    {
      m_layoutEditor->setMouseMode(2);
    }
    break;

    case te::qt::af::evt::PAN_BUTTON_TOGGLED:
    {
      m_layoutEditor->setMouseMode(3);
    }
    break;

    case te::qt::af::evt::SELECTION_BUTTON_TOGGLED:
    {
      m_layoutEditor->setMouseMode(0);
    }
    break;

    default:
      return;
  }
}

//void te::qt::af::LayoutEditor::drawLayerSelection(te::map::AbstractLayer* layer)
//{
  //assert(layer);

  //if(layer->getVisibility() != te::map::VISIBLE)
  //  return;

  //const te::da::ObjectIdSet* oids = layer->getSelected();
  //if(oids == 0 || oids->size() == 0)
  //{
  //  m_mapDisplay->repaint();
  //  return;
  //}

  //bool needRemap = false;

  //if((layer->getSRID() != TE_UNKNOWN_SRS) && (m_mapDisplay->getSRID() != TE_UNKNOWN_SRS) && (layer->getSRID() != m_mapDisplay->getSRID()))
  //  needRemap = true;

  //// Try retrieves the layer selection
  //std::auto_ptr<te::da::DataSet> selected;
  //try
  //{
  //  selected = layer->getData(oids);
  //}
  //catch(std::exception& e)
  //{
  //  QMessageBox::critical(m_mapDisplay, tr("Error"), QString(tr("The layer selection cannot be drawn. Details:") + " %1.").arg(e.what()));
  //  return;
  //}

  //std::size_t gpos = te::da::GetFirstPropertyPos(selected.get(), te::dt::GEOMETRY_TYPE);

  //QPixmap* content = m_mapDisplay->getDisplayPixmap();

  //const te::gm::Envelope& displayExtent = m_mapDisplay->getExtent();

  //te::qt::widgets::Canvas canvas(content);
  //canvas.setWindow(displayExtent.m_llx, displayExtent.m_lly, displayExtent.m_urx, displayExtent.m_ury);

  //te::gm::GeomType currentGeomType = te::gm::UnknownGeometryType;

  //while(selected->moveNext())
  //{
  //  std::auto_ptr<te::gm::Geometry> g(selected->getGeometry(gpos));

  //  if(needRemap)
  //  {
  //    g->setSRID(layer->getSRID());
  //    g->transform(m_mapDisplay->getSRID());
  //  }

  //  if(currentGeomType != g->getGeomTypeId())
  //  {
  //    currentGeomType = g->getGeomTypeId();
  //    te::qt::widgets::Config2DrawLayerSelection(&canvas, ApplicationController::getInstance().getSelectionColor(), currentGeomType);
  //  }

  //  canvas.draw(g.get());
  //}

  //m_mapDisplay->repaint();
//}

//void te::qt::af::LayoutEditor::onDrawLayersFinished(const QMap<QString, QString>& /*errors*/)
//{
  //// Stores the clean pixmap!
  //m_lastDisplayContent = QPixmap(*m_mapDisplay->getDisplayPixmap());

  //// TODO!!!
  //drawLayerSelection((ApplicationController::getInstance().getProject()->getLayers().begin())->get());
//}
