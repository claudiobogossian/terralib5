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
  \file terralib/qt/plugins/edit/ToolBar.cpp

  \brief The main toolbar of TerraLib Edit Qt plugin.
*/

// Terralib
#include "../../../edit/qt/tools/CreateLineTool.h"
#include "../../../edit/qt/tools/CreatePolygonTool.h"
#include "../../../edit/qt/tools/VertexTool.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/events/MapEvents.h"
#include "../../af/ApplicationController.h"
#include "../../af/Project.h"
#include "ToolBar.h"

// Qt
#include <QMessageBox>

// STL
#include <cassert>

te::qt::plugins::edit::ToolBar::ToolBar()
  : m_toolBar(0),
    m_vertexToolAction(0),
    m_createPolygonToolAction(0),
    m_createLineToolAction(0)
{
  initialize();
}

te::qt::plugins::edit::ToolBar::~ToolBar()
{
  delete m_toolBar;
}

QToolBar* te::qt::plugins::edit::ToolBar::get() const
{
  return m_toolBar;
}

te::map::AbstractLayerPtr te::qt::plugins::edit::ToolBar::getSelectedLayer()
{
  te::qt::af::evt::GetLayerSelected e;
  te::qt::af::ApplicationController::getInstance().broadcast(&e);

  te::map::AbstractLayerPtr layer = e.m_layer;

  return layer;
}

void te::qt::plugins::edit::ToolBar::initialize()
{
  // Create the main toolbar
  m_toolBar = new QToolBar;

  // VertexTool action
  m_vertexToolAction = m_toolBar->addAction("Vertex Tool", this, SLOT(onVertexToolActivated(bool)));

  // CreatePolygonTool action
  m_createPolygonToolAction = m_toolBar->addAction("Create Polygon", this, SLOT(onCreatePolygonToolActivated(bool)));

  // CreateLineTool action
  m_createLineToolAction = m_toolBar->addAction("Create Line", this, SLOT(onCreateLineToolActivated(bool)));
}

void te::qt::plugins::edit::ToolBar::onVertexToolActivated(bool checked)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if(layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  te::qt::af::ApplicationController::getInstance().broadcast(&e);

  assert(e.m_display);

  te::edit::VertexTool* tool = new te::edit::VertexTool(e.m_display->getDisplay(), layer, 0);
  e.m_display->setCurrentTool(tool);
}

void te::qt::plugins::edit::ToolBar::onCreatePolygonToolActivated(bool checked)
{
  te::qt::af::evt::GetMapDisplay e;
  te::qt::af::ApplicationController::getInstance().broadcast(&e);

  assert(e.m_display);

  te::edit::CreatePolygonTool* tool = new te::edit::CreatePolygonTool(e.m_display->getDisplay(), Qt::ArrowCursor, 0);
  e.m_display->setCurrentTool(tool);
}

void te::qt::plugins::edit::ToolBar::onCreateLineToolActivated(bool checked)
{
  te::qt::af::evt::GetMapDisplay e;
  te::qt::af::ApplicationController::getInstance().broadcast(&e);

  assert(e.m_display);

  te::edit::CreateLineTool* tool = new te::edit::CreateLineTool(e.m_display->getDisplay(), Qt::ArrowCursor, 0);
  e.m_display->setCurrentTool(tool);
}
