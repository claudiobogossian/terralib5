/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,DataSource
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
#include "../../../core/translator/Translator.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype/SimpleData.h"
#include "../../../edit/Feature.h"
#include "../../../edit/Repository.h"
#include "../../../edit/RepositoryManager.h"
#include "../../../edit/qt/core/UndoStackManager.h"
#include "../../../edit/qt/tools/AggregateAreaTool.h"
#include "../../../edit/qt/tools/CreateLineTool.h"
#include "../../../edit/qt/tools/CreatePointTool.h"
#include "../../../edit/qt/tools/CreatePolygonTool.h"
#include "../../../edit/qt/tools/DeleteGeometryTool.h"
#include "../../../edit/qt/tools/DeletePartTool.h"
#include "../../../edit/qt/tools/EditInfoTool.h"
#include "../../../edit/qt/tools/MergeGeometriesTool.h"
#include "../../../edit/qt/tools/MoveGeometryTool.h"
#include "../../../edit/qt/tools/SplitPolygonTool.h"
#include "../../../edit/qt/tools/SubtractAreaTool.h"
#include "../../../edit/qt/tools/VertexTool.h"
#include "../../../edit/qt/SnapOptionsDialog.h"
#include "../../../edit/qt/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../memory/DataSet.h"
#include "../../../memory/DataSetItem.h"
#include "../../widgets/canvas/MapDisplay.h"
#include "../../widgets/canvas/MultiThreadMapDisplay.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/events/MapEvents.h"

#include "ToolBar.h"
#include "Stasher.h"

// Qt
#include <QActionGroup>
#include <QApplication>
#include <QMessageBox>
#include <QUndoCommand>

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

// STL
#include <algorithm>
#include <cassert>
#include <list>
#include <memory>
#include <vector>

te::qt::plugins::edit::ToolBar::ToolBar(QObject* parent):
QObject(parent),
  m_toolBar(0),
  m_editAction(0),
  m_saveAction(0),
  m_clearEditionAction(0),
  m_vertexToolAction(0),
  m_createPolygonToolAction(0),
  m_createLineToolAction(0),
  m_moveGeometryToolAction(0),
  m_snapOptionsAction(0),
  m_deleteGeometryToolAction(0),
  m_aggregateAreaToolAction(0),
  m_subtractAreaToolAction(0),
  m_featureAttributesAction(0),
  m_splitPolygonToolAction(0),
  m_mergeGeometriesToolAction(0),
  m_createPointToolAction(0),
  m_deletePartToolAction(0),
  m_undoToolAction(0),
  m_redoToolAction(0),
  m_undoView(0),
  m_currentTool(0),
  m_usingStash(false),
  m_layerIsStashed(true),
  m_isEnabled(false)
{
  initialize();
}

te::qt::plugins::edit::ToolBar::~ToolBar()
{
  if(m_currentTool != 0)
  {
    if(m_currentTool->isInUse())
    {
      te::qt::af::evt::GetMapDisplay e;
      emit triggered(&e);

      e.m_display->getDisplay()->setCurrentTool(0);
    }
    else
      delete m_currentTool;
  }

  delete m_toolBar;
  delete m_undoView;
}

QToolBar* te::qt::plugins::edit::ToolBar::get() const
{
  return m_toolBar;
}

void te::qt::plugins::edit::ToolBar::updateLayer(te::map::AbstractLayer* layer, const bool& stashed)
{
  if (!m_editAction->isChecked())
    return;

  if(layer == 0 || layer->getSRID() == 0)
  {
    for (int i = 0; i < m_tools.size(); ++i)
      m_tools[i]->setEnabled(false);

    m_editAction->setChecked(false);

    if (layer->getSRID() == 0)
      QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("SRID invalid."));

    return;
  }
  else
  {
    m_toolBar->setEnabled(true);
    enableActionsByGeomType(m_tools,true);
  }

  m_layerIsStashed = stashed;

  if(m_currentTool != 0)
    m_currentTool->setLayer(layer);

  if(stashed && te::edit::RepositoryManager::getInstance().getRepository(layer->getTitle()) == 0)
  {
    std::map<std::string, int> ops;
    std::map<std::string, te::gm::Geometry*> gms;

    GetStashedGeometries(layer, gms, ops);

    for(std::map<std::string, te::gm::Geometry*>::iterator it = gms.begin(); it != gms.end(); ++it)
    {
      bool ok;
      QString id = QString::fromUtf8(it->first.c_str());
      te::da::ObjectId* oid = new te::da::ObjectId;

      int v = id.toInt(&ok);

      if(!ok)
      {
        te::dt::String* data = new te::dt::String(it->first);
        oid->addValue(data);
      }
      else
      {
        te::dt::Int32* data = new te::dt::Int32(v);
        oid->addValue(data);
      }

      te::edit::RepositoryManager::getInstance().addGeometry(layer->getId(), oid, it->second, (te::edit::OperationType)ops[it->first]);
    }
  }
}

te::map::AbstractLayerPtr te::qt::plugins::edit::ToolBar::getSelectedLayer()
{
  te::qt::af::evt::GetLayerSelected e;
  emit triggered(&e);

  te::map::AbstractLayerPtr layer = e.m_layer;

  return layer;
}

te::map::AbstractLayerPtr te::qt::plugins::edit::ToolBar::getLayer(const std::string& id)
{
  te::qt::af::evt::GetAvailableLayers evt;
  emit triggered(&evt);

  std::list<te::map::AbstractLayerPtr>::iterator it;
  for(it = evt.m_layers.begin(); it != evt.m_layers.end(); ++it)
  {
    if((*it)->getId() == id)
      return *it;
  }

  throw te::common::Exception(TE_TR("Could not retrieve the layer."));
}

bool te::qt::plugins::edit::ToolBar::dataSrcIsPrepared(const te::map::AbstractLayerPtr& layer)
{
  te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(layer.get()->getDataSourceId());

  if (info->getType() != "POSTGIS" && info->getType() != "OGR")
  {
    m_toolBar->setEnabled(false);
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Under Development to this data source: ") + QString(info->getType().c_str()));
    return false;
  }

  return true;
}

void te::qt::plugins::edit::ToolBar::initialize()
{
  // Create the main toolbar
  m_toolBar = new QToolBar;

  initializeActions();

  m_saveAction->setEnabled(true);

  for (int i = 0; i < m_tools.size(); ++i)
    m_tools[i]->setEnabled(true);

  m_snapOptionsAction->setEnabled(true);

  enableActionsByGeomType(m_tools, false);

}

void te::qt::plugins::edit::ToolBar::initializeActions()
{
  // Enable Edition Mode
  createAction(m_editAction, tr("Turn on/off edition mode"), QString("edit-enable"), true, true, "edit_enable", SLOT(onEditActivated(bool)));
  m_toolBar->addAction(m_editAction);

  m_toolBar->addSeparator();

  // Save
  createAction(m_saveAction, tr("Save edition"), "edit-save", false, false, "save_edition", SLOT(onSaveActivated()));
  m_toolBar->addAction(m_saveAction);

  // Cancel all Edition
  createAction(m_clearEditionAction, tr("Cancel all edition [ESC]"), "clearEdition", false, false, "cancel_edition", SLOT(onResetVisualizationToolActivated(bool)));
  m_clearEditionAction->setShortcut(Qt::Key_Escape);
  m_toolBar->addAction(m_clearEditionAction);

  // Undo/Redo
  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);
  QUndoStack* undoStack = te::edit::UndoStackManager::getInstance().getUndoStack();

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

  // Tools
  createAction(m_createPolygonToolAction, tr("Create Polygon"), "edit-create-polygon", true, false, "create_polygon", SLOT(onCreatePolygonToolActivated(bool)));
  createAction(m_createLineToolAction, tr("Create Line"), "layout-drawline", true, false,"create_line", SLOT(onCreateLineToolActivated(bool)));
  createAction(m_createPointToolAction, tr("Create Point"), "edit-create-point", true, false, "create_point", SLOT(onCreatePointToolActivated(bool)));
  createAction(m_moveGeometryToolAction, tr("Move Geometry"), "edit-move-geometry", true, false, "move_geometry", SLOT(onMoveGeometryToolActivated(bool)));
  createAction(m_vertexToolAction, tr("Vertex Tool - Move, \nAdd [Double Click] and \nRemove [Shift+Click]"), "edit-vertex-tool", true, false, "vertex_tool", SLOT(onVertexToolActivated(bool)));
  createAction(m_aggregateAreaToolAction, tr("Aggregate Area"), "edit-aggregateGeometry", true, false, "aggregate_area", SLOT(onAggregateAreaToolActivated(bool)));
  createAction(m_subtractAreaToolAction, tr("Subtract Area"), "edit-subtractGeometry", true, false, "subtract_area", SLOT(onSubtractAreaToolActivated(bool)));
  createAction(m_mergeGeometriesToolAction, tr("Merge Geometries"), "edition_mergeGeometries", true, true, "merge_geometries", SLOT(onMergeGeometriesToolActivated(bool)));
  createAction(m_splitPolygonToolAction, tr("Split Polygon"), "edit-cut", true, true, "split_polygon", SLOT(onSplitPolygonToolActivated(bool)));
  createAction(m_featureAttributesAction, tr("Feature Attributes"), "edit-Info", true, true, "feature_attributes", SLOT(onFeatureAttributesActivated(bool)));
  createAction(m_deleteGeometryToolAction, tr("Delete Geometry"), "edit-deletetool", true, false, "delete_geometry", SLOT(onDeleteGeometryToolActivated(bool)));
  createAction(m_deletePartToolAction, tr("Delete Part"), "edit-deleteparttool", true, false, "delete_part", SLOT(onDeletePartToolActivated(bool)));
  createAction(m_snapOptionsAction, tr("Snap Options"), "edit_snap", false, false, "snap_option", SLOT(onSnapOptionsActivated()));

  m_toolBar->addAction(m_saveAction);
  m_toolBar->addSeparator();
  m_toolBar->addAction(m_undoToolAction);
  m_toolBar->addAction(m_redoToolAction);
  m_toolBar->addSeparator();
  m_toolBar->addAction(m_createPolygonToolAction);
  m_toolBar->addAction(m_createLineToolAction);
  m_toolBar->addAction(m_createPointToolAction);
  m_toolBar->addSeparator();
  m_toolBar->addAction(m_moveGeometryToolAction);
  m_toolBar->addAction(m_vertexToolAction);
  m_toolBar->addSeparator();
  m_toolBar->addAction(m_aggregateAreaToolAction);
  m_toolBar->addAction(m_subtractAreaToolAction);
  m_toolBar->addAction(m_mergeGeometriesToolAction);
  m_toolBar->addSeparator();
  m_toolBar->addAction(m_splitPolygonToolAction);
  m_toolBar->addAction(m_featureAttributesAction);
  m_toolBar->addSeparator();
  m_toolBar->addAction(m_deleteGeometryToolAction);
  m_toolBar->addAction(m_deletePartToolAction);
  m_toolBar->addSeparator();
  m_toolBar->addAction(m_snapOptionsAction);

  // Get the action group of map tools.
  QActionGroup* toolsGroup = te::qt::af::AppCtrlSingleton::getInstance().findActionGroup("Map.ToolsGroup");
  assert(toolsGroup);

  // Adding the new tools
  toolsGroup->addAction(m_vertexToolAction);
  toolsGroup->addAction(m_createPolygonToolAction);
  toolsGroup->addAction(m_createLineToolAction);
  toolsGroup->addAction(m_createPointToolAction);
  toolsGroup->addAction(m_moveGeometryToolAction);
  toolsGroup->addAction(m_aggregateAreaToolAction);
  toolsGroup->addAction(m_subtractAreaToolAction);
  toolsGroup->addAction(m_deleteGeometryToolAction);
  toolsGroup->addAction(m_featureAttributesAction);
  toolsGroup->addAction(m_splitPolygonToolAction);
  toolsGroup->addAction(m_mergeGeometriesToolAction);
  toolsGroup->addAction(m_deletePartToolAction);

  // Grouping...
  m_tools.push_back(m_saveAction);
  m_tools.push_back(m_clearEditionAction);
  m_tools.push_back(m_createPolygonToolAction);
  m_tools.push_back(m_createLineToolAction);
  m_tools.push_back(m_createPointToolAction);
  m_tools.push_back(m_vertexToolAction);
  m_tools.push_back(m_moveGeometryToolAction);
  m_tools.push_back(m_splitPolygonToolAction);
  m_tools.push_back(m_aggregateAreaToolAction);
  m_tools.push_back(m_subtractAreaToolAction);
  m_tools.push_back(m_mergeGeometriesToolAction);
  m_tools.push_back(m_featureAttributesAction);
  m_tools.push_back(m_deleteGeometryToolAction);
  m_tools.push_back(m_deletePartToolAction);
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
  m_isEnabled = checked;

  enableActionsByGeomType(m_tools, m_isEnabled);

  enableCurrentTool(m_isEnabled);

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  if (e.m_display == 0)
    return;

  if (!m_isEnabled)
  {
    QPixmap* draft = e.m_display->getDisplay()->getDraftPixmap();
    draft->fill(Qt::transparent);

    // Clear the repositories
    std::map<std::string, te::edit::Repository*> repositories = te::edit::RepositoryManager::getInstance().getRepositories();

    std::map<std::string, te::edit::Repository*>::const_iterator it;
    for (it = repositories.begin(); it != repositories.end(); ++it)
    {
      if (it->second)
        it->second->clear();
    }

    // Clear the undo stack
    te::edit::UndoStackManager::getInstance().getUndoStack()->clear();
  }

  e.m_display->getDisplay()->repaint();
}

void te::qt::plugins::edit::ToolBar::onSaveActivated()
{
  std::auto_ptr<te::da::DataSourceTransactor> t;

  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if (layer.get() == 0)
    return;

  if(m_usingStash && !m_layerIsStashed)
  {
    std::map<std::string, te::edit::Repository*> repositories = te::edit::RepositoryManager::getInstance().getRepositories();

    std::map<std::string, te::edit::Repository*>::iterator it;

    std::map<std::string, int> ops;

    // Retrieve the layer associated with the current repository
    layer = getSelectedLayer();
    assert(layer.get());

    // The current repository
    te::edit::Repository* repo = repositories[layer->getId()];
    assert(repo);

    // Get the edited geometries
    const std::vector<te::edit::Feature*>& features = repo->getAllFeatures();

    std::map<std::string, te::gm::Geometry*> gs;

    for(std::vector<te::edit::Feature*>::const_iterator fIt = features.begin(); fIt != features.end(); ++fIt)
    {
      te::da::ObjectId* oid = (*fIt)->getId();
      assert(oid);

      // Get the edited geometry
      te::gm::Geometry* geom = (*fIt)->getGeometry();
      assert(geom);

      std::string sOid = oid->getValueAsString();

      gs[sOid] = geom;

      ops[sOid] = (int)(*fIt)->getOperationType();
    }

    StashGeometries(layer.get(), gs, ops);

    emit stashed(layer.get());

    m_layerIsStashed = true;

    return; 
  }

  try
  {

    std::map<std::string, te::edit::Repository*> repositories = te::edit::RepositoryManager::getInstance().getRepositories();

    std::map<std::string, te::edit::Repository*>::iterator it = repositories.find(layer->getId());

    if(it != repositories.end())
    {
      // The current repository
      te::edit::Repository* repo = it->second;
      assert(repo);

      // Retrieve the layer associated with the current repository
      layer = getLayer(it->first);
      assert(layer.get());

      // The data source is it prepared?
      if (!dataSrcIsPrepared(layer))
        return;

      // Get the data souce info
      te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(layer->getDataSourceId());
      assert(info.get());

      te::da::DataSourcePtr dsource = te::da::GetDataSource(layer->getDataSourceId(), true);
      assert(dsource.get());

      // Start the transactor
      t = dsource->getTransactor();

      // Get the layer schema
      std::auto_ptr<te::map::LayerSchema> schema(layer->getSchema());
      assert(schema.get());

      // Get the property names that compose the object id
      std::vector<std::string> oidPropertyNames;
      te::da::GetOIDPropertyNames(schema.get(), oidPropertyNames);

      // Get the edited geometries
      const std::vector<te::edit::Feature*>& features = repo->getAllFeatures();

      // Build the DataSet that will be used to update
      std::map<te::edit::OperationType, te::mem::DataSet* > operationds;

      for (std::size_t i = 0; i <= te::edit::GEOMETRY_UPDATE_ATTRIBUTES; ++i)
        operationds[te::edit::OperationType(i)] = new te::mem::DataSet(schema.get());

      // Get the geometry property position
      std::size_t gpos = te::da::GetFirstSpatialPropertyPos(operationds[te::edit::GEOMETRY_CREATE]);
      assert(gpos != std::string::npos);

      // Get the geometry type
      std::auto_ptr<te::da::DataSetType> dt = layer->getSchema();

      // Get the envelope of layer
      te::gm::Envelope env(layer->getExtent());

      std::map<te::edit::OperationType, std::set<int> > propertiesPos;

      t->begin();

      for (std::size_t i = 0; i < features.size(); ++i) // for each edited feature
      {
        if (features[i]->getOperationType() > te::edit::GEOMETRY_UPDATE_ATTRIBUTES)
          continue;

        // Create the new item
        te::mem::DataSetItem* item = new te::mem::DataSetItem(operationds[te::edit::GEOMETRY_CREATE]);

        // Get the object id
        te::da::ObjectId* oid = features[i]->getId();
        assert(oid);

        const boost::ptr_vector<te::dt::AbstractData>& values = oid->getValue();
        assert(values.size() == oidPropertyNames.size());

        // Fill the new item
        for (std::size_t j = 0; j < values.size(); ++j)
          item->setValue(oidPropertyNames[j], values[j].clone());

        // Get the edited geometry
        te::gm::Geometry* geom = te::edit::ConvertGeomType(layer, te::gm::Validate(features[i]->getGeometry()));
        assert(geom);

        // Set the geometry type
        item->setGeometry(gpos, static_cast<te::gm::Geometry*>(geom->clone()));

        switch (features[i]->getOperationType())
        {
          case te::edit::GEOMETRY_CREATE:
          {
            te::mem::DataSet* tempDs = new te::mem::DataSet(schema.get());

            tempDs->add(item);

            // used to not insert the pk
            for (std::size_t j = 0; j < oidPropertyNames.size(); ++j)
            {
              std::size_t pos = te::da::GetPropertyPos(operationds[te::edit::GEOMETRY_CREATE], oidPropertyNames[j]);
              tempDs->drop(pos);
            }

            std::map<std::string, std::string> options;

            t->add(layer->getDataSetName(), tempDs, options);

            boost::int64_t id = t->getLastGeneratedId();

            for (std::size_t j = 0; j < values.size(); ++j)
              item->setValue(oidPropertyNames[j], new te::dt::SimpleData<int, te::dt::INT32_TYPE>((int)(id)));

            operationds[te::edit::GEOMETRY_CREATE]->add(item);

            env.Union(*geom->getMBR());
          }
            break;

          case te::edit::GEOMETRY_UPDATE:

            propertiesPos[te::edit::GEOMETRY_UPDATE].insert((int)gpos);

            operationds[te::edit::GEOMETRY_UPDATE]->add(item);
            break;

          case te::edit::GEOMETRY_DELETE:

            operationds[te::edit::GEOMETRY_DELETE]->add(item);
            break;

          case te::edit::GEOMETRY_UPDATE_ATTRIBUTES:
          {
            std::vector<std::size_t> objIdIdx;
            te::da::GetOIDPropertyPos(layer->getSchema().get(), objIdIdx);

            for (std::map<std::size_t, te::dt::AbstractData*>::const_iterator it = features[i]->getData().begin(); it != features[i]->getData().end(); ++it)
            {
              if (std::find(objIdIdx.begin(), objIdIdx.end(), (int)it->first) == objIdIdx.end())
              {
                item->setValue(it->first, it->second);
                propertiesPos[te::edit::GEOMETRY_UPDATE_ATTRIBUTES].insert((int)it->first);
              }
            }

            propertiesPos[te::edit::GEOMETRY_UPDATE_ATTRIBUTES].insert((int)gpos);

            operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES]->add(item);
          }
            break;

          default:
            break;
        }
      }

      std::map<te::edit::OperationType, te::da::ObjectIdSet* > currentOids;

      if (operationds[te::edit::GEOMETRY_CREATE]->size() > 0)
      {
        currentOids[te::edit::GEOMETRY_CREATE] = te::da::GenerateOIDSet(operationds[te::edit::GEOMETRY_CREATE], schema.get());

        operationds[te::edit::GEOMETRY_CREATE]->moveBeforeFirst();
      }

      if (operationds[te::edit::GEOMETRY_UPDATE]->size() > 0)
      {
        std::vector<std::set<int> > properties;
        for (std::size_t i = 0; i < operationds[te::edit::GEOMETRY_UPDATE]->size(); ++i)
          properties.push_back(propertiesPos[te::edit::GEOMETRY_UPDATE]);

        std::vector<std::size_t> oidPropertyPosition;
        for (std::size_t i = 0; i < oidPropertyNames.size(); ++i)
          oidPropertyPosition.push_back(te::da::GetPropertyPos(operationds[te::edit::GEOMETRY_UPDATE], oidPropertyNames[i]));

        currentOids[te::edit::GEOMETRY_UPDATE] = te::da::GenerateOIDSet(operationds[te::edit::GEOMETRY_UPDATE], schema.get());

        operationds[te::edit::GEOMETRY_UPDATE]->moveBeforeFirst();

        t->update(layer->getDataSetName(), operationds[te::edit::GEOMETRY_UPDATE], properties, oidPropertyPosition);
      }

      if (operationds[te::edit::GEOMETRY_DELETE]->size() > 0)
      {
        currentOids[te::edit::GEOMETRY_DELETE] = te::da::GenerateOIDSet(operationds[te::edit::GEOMETRY_DELETE], schema.get());

        operationds[te::edit::GEOMETRY_DELETE]->moveBeforeFirst();

        t->remove(layer->getDataSetName(), currentOids[te::edit::GEOMETRY_DELETE]);

      }

      if (operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES]->size() > 0)
      {
        std::vector<std::set<int> > properties;
        for (std::size_t i = 0; i < operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES]->size(); ++i)
          properties.push_back(propertiesPos[te::edit::GEOMETRY_UPDATE_ATTRIBUTES]);

        std::vector<std::size_t> oidPropertyPosition;
        for (std::size_t i = 0; i < oidPropertyNames.size(); ++i)
          oidPropertyPosition.push_back(te::da::GetPropertyPos(operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES], oidPropertyNames[i]));

        currentOids[te::edit::GEOMETRY_UPDATE_ATTRIBUTES] = te::da::GenerateOIDSet(operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES], schema.get());

        operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES]->moveBeforeFirst();

        t->update(layer->getDataSetName(), operationds[te::edit::GEOMETRY_UPDATE_ATTRIBUTES], properties, oidPropertyPosition);
      }

      env.Union(*operationds[te::edit::GEOMETRY_UPDATE]->getExtent(gpos).get());

      layer->setExtent(env);

      // Commit the transaction
      t->commit();

      // Clear the repository
      repo->clear();

      // Clear the undo stack
      te::edit::UndoStackManager::getInstance().getUndoStack()->clear();
    }

    layer->clearSelected();

    // repaint and clear
    te::qt::af::evt::GetMapDisplay e;
    emit triggered(&e);

    //update layer
    te::qt::widgets::MultiThreadMapDisplay* mtmp = dynamic_cast<te::qt::widgets::MultiThreadMapDisplay*>(e.m_display->getDisplay());
    if (mtmp)
      mtmp->updateLayer(layer);
    else
      e.m_display->getDisplay()->refresh(true);

    m_layerIsStashed = false;

    m_currentTool->resetVisualizationTool();
  }
  catch(te::common::Exception& ex)
  {
    t->rollBack();
    QMessageBox::critical(0, tr("TerraLib Edit Qt Plugin"), ex.what());
    return;
  }

  emit stashed(layer.get());
}

void te::qt::plugins::edit::ToolBar::onVertexToolActivated(bool)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if(layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    m_vertexToolAction->setChecked(false);
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  setCurrentTool(new te::edit::VertexTool(e.m_display->getDisplay(), layer, this), e.m_display);
}

void te::qt::plugins::edit::ToolBar::onCreatePolygonToolActivated(bool)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if(layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    m_createPolygonToolAction->setChecked(false);
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  setCurrentTool(new te::edit::CreatePolygonTool(e.m_display->getDisplay(), layer, Qt::ArrowCursor, te::edit::mouseDoubleClick, this), e.m_display);
}

void te::qt::plugins::edit::ToolBar::onCreateLineToolActivated(bool)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if(layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    m_createLineToolAction->setCheckable(false);
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  setCurrentTool(new te::edit::CreateLineTool(e.m_display->getDisplay(), layer, Qt::ArrowCursor, this), e.m_display);
}

void te::qt::plugins::edit::ToolBar::onMoveGeometryToolActivated(bool)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if(layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    m_moveGeometryToolAction->setChecked(false);
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  setCurrentTool(new te::edit::MoveGeometryTool(e.m_display->getDisplay(), layer, te::edit::mouseReleaseRightClick, this), e.m_display);
}

void te::qt::plugins::edit::ToolBar::onSnapOptionsActivated()
{
  te::edit::SnapOptionsDialog options(m_toolBar);

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  te::qt::af::evt::GetAvailableLayers evt;
  emit triggered(&evt);

  options.setLayers(evt.m_layers);
  options.setMapDisplay(e.m_display->getDisplay());

  options.exec();
}

void te::qt::plugins::edit::ToolBar::onFeatureAttributesActivated(bool)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if (layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    m_featureAttributesAction->setChecked(false);
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  setCurrentTool(new te::edit::EditInfoTool(e.m_display->getDisplay(), layer, this), e.m_display);

}

void te::qt::plugins::edit::ToolBar::onAggregateAreaToolActivated(bool)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if(layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    m_aggregateAreaToolAction->setChecked(false);
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  setCurrentTool(new te::edit::AggregateAreaTool(e.m_display->getDisplay(), layer, te::edit::mouseDoubleClick, this), e.m_display);
}

void te::qt::plugins::edit::ToolBar::onSubtractAreaToolActivated(bool)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if(layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    m_subtractAreaToolAction->setChecked(false);
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  setCurrentTool(new te::edit::SubtractAreaTool(e.m_display->getDisplay(), layer, te::edit::mouseDoubleClick, this), e.m_display);
}

void te::qt::plugins::edit::ToolBar::onDeleteGeometryToolActivated(bool)
{
  try
  {
    te::map::AbstractLayerPtr layer = getSelectedLayer();

    if(layer.get() == 0)
    {
      QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
      m_deleteGeometryToolAction->setChecked(false);
      return;
    }

    te::qt::af::evt::GetMapDisplay e;
    emit triggered(&e);

    assert(e.m_display);

    setCurrentTool(new te::edit::DeleteGeometryTool(e.m_display->getDisplay(), layer, this), e.m_display);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::critical(0, tr("TerraLib Edit Qt Plugin"), e.what());
    return;
  }
}

void te::qt::plugins::edit::ToolBar::onMergeGeometriesToolActivated(bool)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if(layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    return;
  }

  if(layer->getSelected() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a geometry first!"));
    return;
  }

  if(layer->getSelected()->size() < 2)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("To use this tool, you must select at least two geometries!"));
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  setCurrentTool(new te::edit::MergeGeometriesTool(e.m_display->getDisplay(), layer, Qt::ArrowCursor, this), e.m_display);
}

void te::qt::plugins::edit::ToolBar::onSplitPolygonToolActivated(bool)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if (layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    return;
  }

  if (layer->getSelected() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a geometry first!"));
    return;
  }

  if (!layer->getSelected()->size())
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("To use this tool, you must select at least one geometry!"));
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  setCurrentTool(new te::edit::SplitPolygonTool(e.m_display->getDisplay(), layer, te::edit::mouseDoubleClick, this), e.m_display);
}

void te::qt::plugins::edit::ToolBar::onResetVisualizationToolActivated(bool /*checked*/)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if (layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    return;
  }

  // Clear the repository
  te::edit::Repository* repo = te::edit::RepositoryManager::getInstance().getRepository(layer->getId());

  if (repo)
    repo->clear();

  // Reset visualization tool
  if (m_currentTool)
    m_currentTool->resetVisualizationTool();

  // Clear the undo stack
  te::edit::UndoStackManager& stack = te::edit::UndoStackManager::getInstance();
  stack.reset();

  // Repaint
  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  if (e.m_display == 0)
    return;

  e.m_display->getDisplay()->getDraftPixmap()->fill(Qt::transparent);
  e.m_display->getDisplay()->repaint();

}

void te::qt::plugins::edit::ToolBar::onCreatePointToolActivated(bool /*checked*/)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if (layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    m_createPolygonToolAction->setChecked(false);
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  setCurrentTool(new te::edit::CreatePointTool(e.m_display->getDisplay(), layer, Qt::ArrowCursor, this), e.m_display);
}

void te::qt::plugins::edit::ToolBar::onDeletePartToolActivated(bool /*checked*/)
{
  te::map::AbstractLayerPtr layer = getSelectedLayer();
  if (layer.get() == 0)
  {
    QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
    m_deletePartToolAction->setChecked(false);
    return;
  }

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  assert(e.m_display);

  setCurrentTool(new te::edit::DeletePartTool(e.m_display->getDisplay(), layer, this), e.m_display);
}

void te::qt::plugins::edit::ToolBar::onCreateUndoViewActivated(bool /*checked*/)
{
  try
  {
    te::map::AbstractLayerPtr layer = getSelectedLayer();

    if (layer.get() == 0)
    {
      QMessageBox::information(0, tr("TerraLib Edit Qt Plugin"), tr("Select a layer first!"));
      return;
    }

    te::qt::af::evt::GetMapDisplay e;
    emit triggered(&e);

    assert(e.m_display);

    if (m_undoView == 0)
    {
      m_undoView = new QUndoView(te::edit::UndoStackManager::getInstance().getUndoStack(), this->get());
      m_undoView->setWindowTitle(tr("Edition List"));
      m_undoView->setFixedSize(QSize(300, 300));
      m_undoView->show();
      m_undoView->setAttribute(Qt::WA_QuitOnClose, true);
    }

  }
  catch (te::common::Exception& e)
  {
    QMessageBox::critical(0, tr("TerraLib Edit Qt Plugin"), e.what());
    return;
  }
}

void te::qt::plugins::edit::ToolBar::onToolDeleted()
{
  m_currentTool = 0;
}

void te::qt::plugins::edit::ToolBar::enableCurrentTool(const bool& enable)
{
  if(m_currentTool == 0)
    return;

  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  if(e.m_display == 0)
    return;

  if(enable)
    e.m_display->getDisplay()->setCurrentTool(m_currentTool);
  else
  {
    e.m_display->getDisplay()->setCursor(Qt::ArrowCursor);
    e.m_display->getDisplay()->setCurrentTool(0, false);
  }

  m_currentTool->setInUse(enable);
}

void te::qt::plugins::edit::ToolBar::setCurrentTool(te::edit::GeometriesUpdateTool* tool, te::qt::af::MapDisplay* display)
{
  display->getDisplay()->setCurrentTool(tool);

  m_currentTool = tool;

  connect(m_currentTool, SIGNAL(geometriesEdited()), SIGNAL(geometriesEdited()));
  connect(m_currentTool, SIGNAL(toolDeleted()), SLOT(onToolDeleted()));

}

void te::qt::plugins::edit::ToolBar::enableActionsByGeomType(QList<QAction*> acts, const bool& enable)
{
  std::size_t geomType = 0;
  te::map::AbstractLayerPtr layer = getSelectedLayer();

  for(QList<QAction*>::iterator it = acts.begin(); it != acts.end(); ++it)
      (*it)->setEnabled(enable);

  if (enable)
  {
    if (layer.get() == 0 || layer->getSRID() == 0)
      return;

    // Get the geometry type of layer
    std::auto_ptr<te::da::DataSetType> dt = layer->getSchema();
    te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dt.get());

    if (geomProp == 0)
    {
      for (QList<QAction*>::iterator it = acts.begin(); it != acts.end(); ++it)
        (*it)->setEnabled(false);

      return;
    }

    switch (geomProp->getGeometryType())
    {
      case te::gm::MultiPolygonType:
      case te::gm::PolygonType:
        geomType = 1;
        break;

      case te::gm::MultiLineStringType:
      case te::gm::LineStringType:
        geomType = 2;
        break;

      case te::gm::MultiPointType:
      case te::gm::PointType:
        geomType = 3;
        break;

      default:
        geomType = -1;
    }

    if (geomType == -1)
    {
      for (QList<QAction*>::iterator it = acts.begin(); it != acts.end(); ++it)
        (*it)->setEnabled(false);

      return;
    }

    m_createPolygonToolAction->setEnabled(geomType == 1 ? true : false);
    m_aggregateAreaToolAction->setEnabled(geomType == 1 ? true : false);
    m_subtractAreaToolAction->setEnabled(geomType == 1 ? true : false);
    m_mergeGeometriesToolAction->setEnabled(geomType == 1 ? true : false);
    m_splitPolygonToolAction->setEnabled(geomType == 1 ? true : false);
    m_createLineToolAction->setEnabled(geomType == 2 ? true : false);
    m_vertexToolAction->setEnabled(geomType == 1 || geomType == 2 ? true : false);
    m_moveGeometryToolAction->setEnabled(geomType >= 1 && geomType <= 3 ? true : false);
    m_deleteGeometryToolAction->setEnabled(geomType >= 1 && geomType <= 3 ? true : false);
    m_createPointToolAction->setEnabled(geomType == 3 ? true : false);
  }

}
