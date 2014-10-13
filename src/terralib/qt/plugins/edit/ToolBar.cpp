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
#include "../../../edit/qt/tools/MoveGeometryTool.h"
#include "../../../edit/qt/tools/VertexTool.h"
#include "../../../edit/qt/SnapOptionsDialog.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/events/MapEvents.h"
#include "../../af/ApplicationController.h"
#include "../../af/Project.h"
#include "ToolBar.h"

// Qt
#include <QActionGroup>
#include <QMessageBox>

// STL
#include <cassert>

te::qt::plugins::edit::ToolBar::ToolBar()
  : m_toolBar(0),
    m_editAction(0),
    m_vertexToolAction(0),
    m_createPolygonToolAction(0),
    m_createLineToolAction(0),
    m_moveGeometryToolAction(0),
    m_snapOptionsAction(0)
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

  initializeActions();
}

void te::qt::plugins::edit::ToolBar::initializeActions()
{
  createAction(m_editAction, tr("Turn on/off edition mode"), "edit-enable", true, true,  SLOT(onEditActivated(bool)));
  m_toolBar->addAction(m_editAction);

  // Tools
  createAction(m_vertexToolAction, tr("Vertex Tool - Move, add and remove"), "edit-vertex-tool", true, false,  SLOT(onVertexToolActivated(bool)));
  createAction(m_createPolygonToolAction, tr("Create Polygon"), "edit-create-polygon", true, false,  SLOT(onCreatePolygonToolActivated(bool)));
  createAction(m_createLineToolAction, tr("Create Line"), "edit-create-line", true, false,  SLOT(onCreateLineToolActivated(bool)));
  createAction(m_moveGeometryToolAction, tr("Move Geometry"), "edit-move-geometry", true, false,  SLOT(onMoveGeometryToolActivated(bool)));

  // Get the action group of map tools.
  QActionGroup* toolsGroup = te::qt::af::ApplicationController::getInstance().findActionGroup("Map.ToolsGroup");
  assert(toolsGroup);

  // Adding the new tools
  toolsGroup->addAction(m_vertexToolAction);
  toolsGroup->addAction(m_createPolygonToolAction);
  toolsGroup->addAction(m_createLineToolAction);
  toolsGroup->addAction(m_moveGeometryToolAction);

  // Grouping...
  m_tools.push_back(m_vertexToolAction);
  m_tools.push_back(m_createPolygonToolAction);
  m_tools.push_back(m_createLineToolAction);
  m_tools.push_back(m_moveGeometryToolAction);

  // Adding tools to toolbar
  for(int i = 0; i < m_tools.size(); ++i)
    m_toolBar->addAction(m_tools[i]);

  // Snap
  createAction(m_snapOptionsAction, tr("Snap Options"), "edit-snap", false, false,  SLOT(onSnapOptionsActivated()));
  m_toolBar->addSeparator();
  m_toolBar->addAction(m_snapOptionsAction);
}

void te::qt::plugins::edit::ToolBar::createAction(QAction*& action, const QString& tooltip, const QString& icon, bool checkable, bool enabled, const char* member)
{
  action = new QAction(this);
  action->setIcon(QIcon::fromTheme(icon));
  action->setToolTip(tooltip);
  action->setCheckable(checkable);
  action->setEnabled(enabled);
  connect(action, SIGNAL(triggered(bool)), this, member);
}

void te::qt::plugins::edit::ToolBar::onEditActivated(bool checked)
{
  for(int i = 0; i < m_tools.size(); ++i)
    m_tools[i]->setEnabled(checked);

  m_snapOptionsAction->setEnabled(checked);
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
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if(layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  te::qt::af::ApplicationController::getInstance().broadcast(&e);

  assert(e.m_display);

  te::edit::CreatePolygonTool* tool = new te::edit::CreatePolygonTool(e.m_display->getDisplay(), layer, Qt::ArrowCursor, 0);
  e.m_display->setCurrentTool(tool);
}

void te::qt::plugins::edit::ToolBar::onCreateLineToolActivated(bool checked)
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

  te::edit::CreateLineTool* tool = new te::edit::CreateLineTool(e.m_display->getDisplay(), layer, Qt::ArrowCursor, 0);
  e.m_display->setCurrentTool(tool);
}

void te::qt::plugins::edit::ToolBar::onMoveGeometryToolActivated(bool checked)
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

  te::edit::MoveGeometryTool* tool = new te::edit::MoveGeometryTool(e.m_display->getDisplay(), layer, 0);
  e.m_display->setCurrentTool(tool);
}

void te::qt::plugins::edit::ToolBar::onSnapOptionsActivated()
{
  te::edit::SnapOptionsWidget options(m_toolBar);
  options.setLayers(te::qt::af::ApplicationController::getInstance().getProject()->getAllLayers(false));
  options.exec();
}
