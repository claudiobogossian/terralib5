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
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../edit/Feature.h"
#include "../../../edit/Repository.h"
#include "../../../edit/qt/tools/CreateLineTool.h"
#include "../../../edit/qt/tools/CreatePolygonTool.h"
#include "../../../edit/qt/tools/MoveGeometryTool.h"
#include "../../../edit/qt/tools/VertexTool.h"
#include "../../../edit/qt/tools/AggregateAreaTool.h"
#include "../../../edit/qt/tools/SubtractAreaTool.h"
#include "../../../edit/qt/tools/DeleteGeometryTool.h"
#include "../../../edit/qt/tools/MergeGeometriesTool.h"
#include "../../../edit/qt/SnapOptionsDialog.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/GeometryCollection.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../memory/DataSet.h"
#include "../../../memory/DataSetItem.h"
#include "../../../qt/af/events/LayerEvents.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
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
    m_snapOptionsAction(0),
    m_deleteGeometryToolAction(0),
    m_aggregateAreaToolAction(0),
    m_subtractAreaToolAction(0),
    m_undoToolAction(0),
    m_redoToolAction(0),
    m_undoView(0)
{
    initialize();
}

te::qt::plugins::edit::ToolBar::~ToolBar()
{
  delete m_toolBar;
  delete m_editionManager;
  delete m_undoView;
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

  // Initialize the Edition Manager
  m_editionManager = new te::edit::EditionManager();

  initializeActions();
}

void te::qt::plugins::edit::ToolBar::initializeActions()
{
  // Enable Edition Mode
  createAction(m_editAction, tr("Turn on/off edition mode"), "edit-enable",true, true,"edit_enable",SLOT(onEditActivated(bool)));
  
  m_toolBar->addAction(m_editAction);

  // Save
  createAction(m_saveAction, tr("Save edition"), "edit-save", false, false,"save_edition",SLOT(onSaveActivated()));
  m_toolBar->addAction(m_saveAction);

  // Undo/Redo
  te::qt::af::evt::GetMapDisplay e;
  te::qt::af::ApplicationController::getInstance().broadcast(&e);

  QUndoStack* undoStack = m_editionManager->getUndoStack();

  if (undoStack)
  {
    m_undoToolAction = undoStack->createUndoAction(this, tr("&Undo"));
    m_undoToolAction->setShortcuts(QKeySequence::Undo);
    m_undoToolAction->setObjectName("redo");
    m_undoToolAction->setIcon(QIcon::fromTheme("edit-undo"));
    m_undoToolAction->setToolTip("Undo Action");

    m_redoToolAction = undoStack->createRedoAction(this, tr("&Redo"));
    m_redoToolAction->setShortcuts(QKeySequence::Redo);
    m_redoToolAction->setObjectName("undo");
    m_redoToolAction->setIcon(QIcon::fromTheme("edit-redo"));
    m_redoToolAction->setToolTip("Redo Action");
  }

  m_toolBar->addAction(m_undoToolAction);
  m_toolBar->addAction(m_redoToolAction);

  m_toolBar->addSeparator();

  createAction(m_vertexToolAction, tr("Vertex Tool - Move, add and remove"), "edit-vertex-tool", true, false,"vertex_tool", SLOT(onVertexToolActivated(bool)));
  createAction(m_createPolygonToolAction, tr("Create Polygon"), "edit-create-polygon", true, false,"create_polygon",SLOT(onCreatePolygonToolActivated(bool)));
  //createAction(m_createLineToolAction, tr("Create Line"), "layout-drawline", true, false,"create_line", SLOT(onCreateLineToolActivated(bool))); //edit-create-line
  createAction(m_moveGeometryToolAction, tr("Move Geometry"), "edit-move-geometry", true, false,"move_geometry",SLOT(onMoveGeometryToolActivated(bool)));
  createAction(m_aggregateAreaToolAction, tr("Aggregate Area"), "vector-processing-aggregation", true, false,"aggregate_area", SLOT(onAggregateAreaToolActivated(bool)));
  createAction(m_subtractAreaToolAction, tr("Subtract Area"), "vector-processing-subtraction", true, false,"subtract_area",SLOT(onSubtractAreaToolActivated(bool)));
  createAction(m_deleteGeometryToolAction, tr("Delete Geometry"), "edit_delete", true, false,"delete_geometry",SLOT(onDeleteGeometryToolActivated(bool)));
  createAction(m_mergeGeometriesToolAction, tr("Merge Geometries"), "edition_mergeGeometries", true, false,"merge_geometries",SLOT(onMergeGeometriesToolActivated(bool)));

  // Get the action group of map tools.
  QActionGroup* toolsGroup = te::qt::af::ApplicationController::getInstance().findActionGroup("Map.ToolsGroup");
  assert(toolsGroup);

  // Adding the new tools
  toolsGroup->addAction(m_vertexToolAction);
  toolsGroup->addAction(m_createPolygonToolAction);
  //toolsGroup->addAction(m_createLineToolAction);
  toolsGroup->addAction(m_moveGeometryToolAction);
  toolsGroup->addAction(m_aggregateAreaToolAction);
  toolsGroup->addAction(m_subtractAreaToolAction);
  toolsGroup->addAction(m_deleteGeometryToolAction);
  toolsGroup->addAction(m_mergeGeometriesToolAction);

  // Grouping...
  m_tools.push_back(m_vertexToolAction);
  m_tools.push_back(m_createPolygonToolAction);
  //m_tools.push_back(m_createLineToolAction);
  m_tools.push_back(m_moveGeometryToolAction);
  m_tools.push_back(m_aggregateAreaToolAction);
  m_tools.push_back(m_subtractAreaToolAction);
  m_tools.push_back(m_deleteGeometryToolAction);
  m_tools.push_back(m_mergeGeometriesToolAction);
  
  // Adding tools to toolbar
  for(int i = 0; i < m_tools.size(); ++i)
    m_toolBar->addAction(m_tools[i]);

  // Snap
  createAction(m_snapOptionsAction, tr("Snap Options"), "edit_snap", false, false,"snap_option",SLOT(onSnapOptionsActivated()));
  m_toolBar->addSeparator();
  m_toolBar->addAction(m_snapOptionsAction);

}

void te::qt::plugins::edit::ToolBar::createAction(QAction*& action, const QString& tooltip, const QString& icon, bool checkable, bool enabled, const QString& objName, const char* member)
{
  action = new QAction(this);
  action->setIcon(QIcon::fromTheme(icon));
  action->setToolTip(tooltip);
  action->setCheckable(checkable);
  action->setEnabled(enabled);
  action->setObjectName(objName);
  connect(action, SIGNAL(triggered(bool)), this, member);
}

void te::qt::plugins::edit::ToolBar::onEditActivated(bool checked)
{
  m_saveAction->setEnabled(checked);

  for(int i = 0; i < m_tools.size(); ++i)
    m_tools[i]->setEnabled(checked);

  m_snapOptionsAction->setEnabled(checked);

  createUndoView(checked);
}

void te::qt::plugins::edit::ToolBar::onSaveActivated()
{
  try
  {
    std::map<std::string, te::edit::Repository*> repositories = m_editionManager->m_repository->getRepositories();

    std::map<std::string, te::edit::Repository*>::iterator it;

    for (it = repositories.begin(); it != repositories.end(); ++it) // for each repository
    {
      // The current repository
      te::edit::Repository* repo = it->second;
      assert(repo);

      // Retrieve the layer associated with the current repository
      te::map::AbstractLayerPtr layer = getLayer(it->first);
      assert(layer.get());

      // For while, use DataSetLayer to get the DataSource
      te::map::DataSetLayer* dslayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());
      assert(dslayer);

      te::da::DataSourcePtr dsource = te::da::GetDataSource(dslayer->getDataSourceId(), true);
      assert(dsource.get());

      // Get the layer schema
      std::auto_ptr<te::map::LayerSchema> schema(layer->getSchema());
      assert(schema.get());

      // Get the property names that compose the object id
      std::vector<std::string> oidPropertyNames;
      te::da::GetOIDPropertyNames(schema.get(), oidPropertyNames);
         
      // Get the edited geometries
      const std::vector<te::edit::Feature*>& features = repo->getAllFeatures();

      //if not have any geometry
      if (features.size() == 0)
        return;

      // Build the DataSet that will be used to update
      std::auto_ptr<te::mem::DataSet> createds(new te::mem::DataSet(schema.get()));
      std::auto_ptr<te::mem::DataSet> updateds(new te::mem::DataSet(schema.get()));
      std::auto_ptr<te::mem::DataSet> removeds(new te::mem::DataSet(schema.get()));

      // Get the geometry property position
      std::size_t gpos = te::da::GetFirstSpatialPropertyPos(createds.get());
      assert(gpos != std::string::npos);

      // Get the geometry type
      std::auto_ptr<te::da::DataSetType> dt = layer->getSchema();
      te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dt.get());

      for (std::size_t i = 0; i < features.size(); ++i) // for each edited feature
      {

          // Create the new item
          te::mem::DataSetItem* item = new te::mem::DataSetItem(createds.get());

          // Get the object id
          te::da::ObjectId* oid = features[i]->getId();
          assert(oid);

          const boost::ptr_vector<te::dt::AbstractData>& values = oid->getValue();
          assert(values.size() == oidPropertyNames.size());

          // Get the edited geometry 
          te::gm::Geometry* geom = features[i]->getGeometry();
          assert(geom);

          // Fill the new item                    
          for (std::size_t j = 0; j < values.size(); ++j)
            item->setValue(oidPropertyNames[j], values[j].clone());

          // Set the geometry type
          if (geomProp->getGeometryType() == te::gm::MultiPolygonType &&
            features[i]->getGeometry()->getGeomTypeId() == te::gm::PolygonType)
          {
            std::auto_ptr<te::gm::GeometryCollection> gc(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, layer->getSRID()));
            gc->add(geom);
            item->setGeometry(gpos, gc.release());
          }
          else
          {
            item->setGeometry(gpos, static_cast<te::gm::Geometry*>(geom->clone()));
          }

          std::map<std::string, std::size_t>::iterator it;

          for (it = m_editionManager->m_operation.begin(); it != m_editionManager->m_operation.end(); ++it)
          {
            if (oid->getValueAsString() == it->first)
            {
              switch (it->second)
              {
                case EDT_CREATE:
                  createds->add(item);
                  break;

                case EDT_UPDATE:
                  updateds->add(item);
                  break;

                case EDT_REMOVE:
                  removeds->add(item);
                  break;
              }
            }
          }

      }

      std::set<int> gproperty;
      gproperty.insert(gpos);

      te::da::ObjectIdSet* currentOidsCreated = new te::da::ObjectIdSet();
      te::da::ObjectIdSet* currentOidsUpdated = new te::da::ObjectIdSet();
      te::da::ObjectIdSet* currentOidsRemoved = new te::da::ObjectIdSet();

      if (createds->size())
      {
        currentOidsCreated = te::da::GenerateOIDSet(createds.get(), schema.get());

        createds->moveBeforeFirst();

        std::map<std::string, std::string> options;

        dsource->add(dslayer->getDataSetName(), createds.get(), options, 0);
      }

      if (updateds->size())
      {
        std::vector<std::set<int> > properties;
        for (std::size_t i = 0; i < updateds->size(); ++i){
          properties.push_back(gproperty);
        }

        std::vector<std::size_t> oidPropertyPosition;
        for (std::size_t i = 0; i < oidPropertyNames.size(); ++i)
          oidPropertyPosition.push_back(te::da::GetPropertyPos(updateds.get(), oidPropertyNames[i]));

        currentOidsUpdated = te::da::GenerateOIDSet(updateds.get(), schema.get());

        updateds->moveBeforeFirst();

        dsource->update(dslayer->getDataSetName(), updateds.get(), properties, oidPropertyPosition);

      }

      if (removeds->size())
      {
        currentOidsRemoved = te::da::GenerateOIDSet(removeds.get(), schema.get());

        removeds->moveBeforeFirst();

        dsource->remove(dslayer->getDataSetName(), currentOidsRemoved);

      }

      repo->clear();

      layer->clearSelected();

      if (currentOidsCreated->size())
      {
        layer->select(currentOidsCreated->clone());
      }
        
      if (currentOidsUpdated->size())
      {
        layer->select(currentOidsUpdated->clone());
      }

      emit layerSelectedObjectsChanged(layer);

    }

    // repaint and clear
    te::qt::af::evt::GetMapDisplay e;
    te::qt::af::ApplicationController::getInstance().broadcast(&e);
    
    e.m_display->getDisplay()->refresh();

    m_editionManager->getUndoStack()->clear();

    }
    catch(te::common::Exception& e)
    {
      QMessageBox::critical(0, tr("TerraLib Edit Qt Plugin"), e.what());
      return;
    }
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

  te::edit::VertexTool* tool = new te::edit::VertexTool(m_editionManager, e.m_display->getDisplay(), layer, 0);
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

  te::edit::CreatePolygonTool* tool = new te::edit::CreatePolygonTool(m_editionManager, e.m_display->getDisplay(), layer, Qt::ArrowCursor, 0);
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

  te::edit::CreateLineTool* tool = new te::edit::CreateLineTool(m_editionManager, e.m_display->getDisplay(), layer, Qt::ArrowCursor, 0);
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

  te::edit::MoveGeometryTool* tool = new te::edit::MoveGeometryTool(m_editionManager, e.m_display->getDisplay(), layer, 0);
  e.m_display->setCurrentTool(tool);

}

void te::qt::plugins::edit::ToolBar::onSnapOptionsActivated()
{
  te::edit::SnapOptionsDialog options(m_toolBar);
  options.setLayers(te::qt::af::ApplicationController::getInstance().getProject()->getAllLayers(false));
  options.exec();
}


void te::qt::plugins::edit::ToolBar::onAggregateAreaToolActivated(bool checked)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if (layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    return;
  }

  if (layer->getSelected() == 0)
  {
    QMessageBox::critical(0, tr("Error"), QString(tr("To aggregate area, you must select exactly 1 polygon!")));
    return;
  }

  if (layer->getSelected()->size() != 1)
  {
    QMessageBox::critical(0, tr("Error"), QString(tr("To aggregate area, you must select exactly 1 polygon!")));
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  te::qt::af::ApplicationController::getInstance().broadcast(&e);

  assert(e.m_display);

  te::edit::AggregateAreaTool* tool = new te::edit::AggregateAreaTool(m_editionManager, e.m_display->getDisplay(), layer, 0);
  e.m_display->setCurrentTool(tool);

}

void te::qt::plugins::edit::ToolBar::onSubtractAreaToolActivated(bool checked)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if (layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    return;
  }

  if (layer->getSelected() == 0)
  {
    QMessageBox::critical(0, tr("Error"), QString(tr("To subtract area, you must select exactly 1 polygon!")));
    return;
  }

  if (layer->getSelected()->size() != 1)
  {
    QMessageBox::critical(0, tr("Error"), QString(tr("To subtract area, you must select exactly 1 polygon!")));
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  te::qt::af::ApplicationController::getInstance().broadcast(&e);

  assert(e.m_display);

  te::edit::SubtractAreaTool* tool = new te::edit::SubtractAreaTool(m_editionManager,e.m_display->getDisplay(), layer, 0);
  e.m_display->setCurrentTool(tool);

}


void te::qt::plugins::edit::ToolBar::onDeleteGeometryToolActivated(bool checked)
{
  try
  {
    te::map::AbstractLayerPtr layer = getSelectedLayer();

    m_deleteGeometryToolAction->setChecked(false);

    if (layer.get() == 0)
    {
      QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
      return;
    }

    if (layer->getSelected() == 0)
    {
      QMessageBox::critical(0, tr("Error"), QString(tr("To delete geometry, you must select a polygon!")));
      return;
    }

    if (layer->getSelected()->size() != 1)
    {
      QMessageBox::critical(0, tr("Error"), QString(tr("To delete geometry, you must select a polygon!")));
      return;
    }

    m_deleteGeometryToolAction->setChecked(true);

    te::qt::af::evt::GetMapDisplay e;
    te::qt::af::ApplicationController::getInstance().broadcast(&e);

    assert(e.m_display);

    te::edit::DeleteGeometryTool* tool = new te::edit::DeleteGeometryTool(m_editionManager, e.m_display->getDisplay(), layer, 0);
    e.m_display->setCurrentTool(tool);

  }
  catch (te::common::Exception& e)
  {
    QMessageBox::critical(0, tr("TerraLib Edit Qt Plugin"), e.what());
    return;
  }

}

void te::qt::plugins::edit::ToolBar::onMergeGeometriesToolActivated(bool checked)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if (layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    return;
  }

  if (layer->getSelected() == 0)
  {
    QMessageBox::critical(0, tr("Error"), QString(tr("To use this tool, you must select at least two geometries!")));
    return;
  }

  if (layer->getSelected()->size() < 2)
  {
    QMessageBox::critical(0, tr("Error"), QString(tr("To use this tool, you must select at least two geometries!")));
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  te::qt::af::ApplicationController::getInstance().broadcast(&e);

  assert(e.m_display);

  te::edit::MergeGeometriesTool* tool = new te::edit::MergeGeometriesTool(m_editionManager, e.m_display->getDisplay(), layer, Qt::ArrowCursor, 0 );
  e.m_display->setCurrentTool(tool);

}


void te::qt::plugins::edit::ToolBar::createUndoView(bool checked)
{
  if (!checked)
    return;

  m_undoView = new QUndoView(m_editionManager->getUndoStack());
  m_undoView->setWindowTitle(tr("Edition List"));
  m_undoView->setFixedSize(QSize(300, 300));
  m_undoView->show();
  m_undoView->setAttribute(Qt::WA_QuitOnClose, false);

}

