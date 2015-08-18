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
  \file terralib/qt/plugins/edit/ToolBar.cpp

  \brief The main toolbar of TerraLib Edit Qt plugin.
*/

// Terralib
#include "../../../common/Exception.h"
#include "../../../common/Translator.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../edit/Feature.h"
#include "../../../edit/Repository.h"
#include "../../../edit/RepositoryManager.h"
#include "../../../edit/qt/tools/CreateLineTool.h"
#include "../../../edit/qt/tools/CreatePolygonTool.h"
#include "../../../edit/qt/tools/MoveGeometryTool.h"
#include "../../../edit/qt/tools/VertexTool.h"
#include "../../../edit/qt/SnapOptionsDialog.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../memory/DataSet.h"
#include "../../../memory/DataSetItem.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/events/MapEvents.h"
#include "../../af/ApplicationController.h"
#include "../../af/Project.h"
#include "ToolBar.h"

// Qt
#include <QActionGroup>
#include <QMessageBox>

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

// STL
#include <cassert>
#include <list>
#include <vector>

te::qt::plugins::edit::ToolBar::ToolBar()
  : m_toolBar(0),
    m_editAction(0),
    m_saveAction(0),
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

te::map::AbstractLayerPtr te::qt::plugins::edit::ToolBar::getLayer(const std::string& id)
{
  std::list<te::map::AbstractLayerPtr> layers = te::qt::af::ApplicationController::getInstance().getProject()->getSingleLayers();

  std::list<te::map::AbstractLayerPtr>::iterator it;
  for(it = layers.begin(); it != layers.end(); ++it)
  {
    if((*it)->getId() == id)
      return *it;
  }

  throw te::common::Exception(TE_TR("Could not retrieve the layer."));
}

void te::qt::plugins::edit::ToolBar::initialize()
{
  // Create the main toolbar
  m_toolBar = new QToolBar;

  initializeActions();
}

void te::qt::plugins::edit::ToolBar::initializeActions()
{
  // Enable Edition Mode
  createAction(m_editAction, tr("Turn on/off edition mode"), "edit-enable", true, true,  SLOT(onEditActivated(bool)));
  m_toolBar->addAction(m_editAction);

  // Save
  createAction(m_saveAction, tr("Save edition"), "edit-save", false, false,  SLOT(onSaveActivated()));
  m_toolBar->addAction(m_saveAction);

  m_toolBar->addSeparator();

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
  m_saveAction->setEnabled(checked);

  for(int i = 0; i < m_tools.size(); ++i)
    m_tools[i]->setEnabled(checked);

  m_snapOptionsAction->setEnabled(checked);
}

void te::qt::plugins::edit::ToolBar::onSaveActivated()
{
  try
  {
    std::map<std::string, te::edit::Repository*> repositories = te::edit::RepositoryManager::getInstance().getRepositories();

    std::map<std::string, te::edit::Repository*>::iterator it;
    for(it = repositories.begin(); it != repositories.end(); ++it) // for each repository
    {
      // The current repository
      te::edit::Repository* repo = it->second;
      assert(repo);

      // Retrieve the layer associated with the current repository
      te::map::AbstractLayerPtr layer = getLayer(it->first);
      assert(layer.get());

      // Get the layer schema
      std::auto_ptr<te::map::LayerSchema> schema(layer->getSchema());
      assert(schema.get());

      // Get the property names that compose the object id
      std::vector<std::string> oidPropertyNames;
      te::da::GetOIDPropertyNames(schema.get(), oidPropertyNames);

      // Get the edited geometries
      const std::vector<te::edit::Feature*>& features = repo->getAllFeatures();

      // Build the DataSet that will be used to update
      std::auto_ptr<te::mem::DataSet> memds(new te::mem::DataSet(schema.get()));

      // Get the geometry property position
      std::size_t gpos = te::da::GetFirstSpatialPropertyPos(memds.get());
      assert(gpos != std::string::npos);

      for(std::size_t i = 0; i < features.size(); ++i) // for each edited feature
      {
        // Create the new item
        te::mem::DataSetItem* item = new te::mem::DataSetItem(memds.get());

        // Get the object id
        te::da::ObjectId* oid = features[i]->getId();
        assert(oid);

        const boost::ptr_vector<te::dt::AbstractData>& values = oid->getValue();
        assert(values.size() == oidPropertyNames.size());

        // Get the edited geometry
        te::gm::Geometry* geom = features[i]->getGeometry();
        assert(geom);

        // Fill the new item
        for(std::size_t j = 0; j < values.size(); ++j)
          item->setValue(oidPropertyNames[j], values[j].clone());

        item->setGeometry(gpos, static_cast<te::gm::Geometry*>(geom->clone()));

        memds->add(item);
      }

      // For while, use DataSetLayer to get the DataSource
      te::map::DataSetLayer* dslayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());
      assert(dslayer);

      te::da::DataSourcePtr ds = te::da::GetDataSource(dslayer->getDataSourceId(), true);
      assert(ds.get());

      std::set<int> gproperty;
      gproperty.insert((const int)gpos);

      std::vector<std::set<int> > properties;
      for(std::size_t i = 0; i < memds->size(); ++i)
        properties.push_back(gproperty);

      std::vector<std::size_t> oidPropertyPosition;
      for(std::size_t i = 0; i < oidPropertyNames.size(); ++i)
        oidPropertyPosition.push_back(te::da::GetPropertyPos(memds.get(), oidPropertyNames[i]));

      ds->update(dslayer->getDataSetName(), memds.get(), properties, oidPropertyPosition);

      repo->clear();
    }
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::critical(0, tr("TerraLib Edit Qt Plugin"), e.what());
    return;
  }
}

void te::qt::plugins::edit::ToolBar::onVertexToolActivated(bool)
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

void te::qt::plugins::edit::ToolBar::onCreatePolygonToolActivated(bool)
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

void te::qt::plugins::edit::ToolBar::onCreateLineToolActivated(bool)
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

void te::qt::plugins::edit::ToolBar::onMoveGeometryToolActivated(bool)
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
  te::edit::SnapOptionsDialog options(m_toolBar);
  options.setLayers(te::qt::af::ApplicationController::getInstance().getProject()->getAllLayers(false));
  options.exec();
}
