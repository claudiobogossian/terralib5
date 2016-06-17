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
  \file terralib/mnt/qt/ProfileDialog.cpp

  \brief .
*/
//TerraLib

#include "../../common/Logger.h"
#include "../../edit/qt/Renderer.h"
#include "../../edit/qt/tools/CreateLineTool.h"
#include "../../edit/qt/tools/VertexTool.h"
#include "../../edit/Feature.h"
#include "../../edit/Repository.h"
#include "../../edit/RepositoryManager.h"
#include "../../qt/af/events/MapEvents.h"
#include "../../qt/widgets/Utils.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../mnt/core/Profile.h"
#include "../../qt/af/ApplicationController.h"
#include "../../qt/af/BaseApplication.h"
#include "../../qt/af/connectors/MapDisplay.h"
#include "../../qt/widgets/canvas/Canvas.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/explorer/LayerItemView.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/canvas/MapDisplay.h"
#include "../../raster.h"
#include "../../statistics/core/Utils.h"

#include "LayerSearchDialog.h"
#include "ProfileDialog.h"
#include "ProfileResultDialog.h"
#include "ui_ProfileDialogForm.h"

// Qt
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QValidator>

// STL
#include <memory>

// Boost
#include <boost/filesystem.hpp>



Q_DECLARE_METATYPE(te::map::AbstractLayerPtr)

te::mnt::ProfileDialog::ProfileDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ProfileDialogForm),
    m_tooltype(ToolNone),
    m_tool(0),
    m_app(0)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_addPointMousePushButton->setIcon(QIcon::fromTheme("mnt-profile-line create"));
  m_ui->m_changePointPushButton->setIcon(QIcon::fromTheme("mnt-profile-vertex move"));
  m_ui->m_addPointPushButton->setIcon(QIcon::fromTheme("mnt-profile-vertex create"));
  m_ui->m_deletePointPushButton->setIcon(QIcon::fromTheme("mnt-profile-vertex delete"));
  m_ui->m_deletePathPushButton->setIcon(QIcon::fromTheme("mnt-profile-delete"));
  m_ui->m_invertPushButton->setIcon(QIcon::fromTheme("mnt-profile-invert"));

// connectors
  connect(m_ui->m_layerSearchToolButton, SIGNAL(clicked()), this, SLOT(onInputLayerToolButtonClicked()));
  connect(m_ui->m_inputLayersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));
  
  connect(m_ui->m_dummycheckBox, SIGNAL(toggled(bool)), m_ui->m_dummylineEdit, SLOT(setEnabled(bool)));

  connect(m_ui->m_editionradioButton, SIGNAL(toggled(bool)), this, SLOT(oneditionEnabled(bool)));
  connect(m_ui->m_selectionradioButton, SIGNAL(toggled(bool)), this, SLOT(onselectionEnabled(bool)));

  connect(m_ui->m_vectorlayersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onVectorInputComboBoxChanged(int)));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));

  connect(m_ui->m_addPointMousePushButton, SIGNAL(toggled(bool)), SLOT(onaddPointMouseToggled(bool)));
  connect(m_ui->m_changePointPushButton, SIGNAL(toggled(bool)), SLOT(onchangePointToggled(bool)));
  connect(m_ui->m_addPointPushButton, SIGNAL(toggled(bool)), SLOT(onaddPointToggled(bool)));
  connect(m_ui->m_deletePointPushButton, SIGNAL(toggled(bool)), SLOT(ondeletePointToggled(bool)));
  connect(m_ui->m_deletePathPushButton, SIGNAL(toggled(bool)), SLOT(ondeletePathToggled(bool)));
  connect(m_ui->m_invertPushButton, SIGNAL(toggled(bool)), SLOT(oninvertToggled(bool)));

  m_color.push_back(te::color::RGBAColor(255, 0, 0, 255));
  m_color.push_back(te::color::RGBAColor(255, 0, 255, 255));
  m_color.push_back(te::color::RGBAColor(0, 255, 255, 255));
  m_color.push_back(te::color::RGBAColor(37, 127, 0, 255));
  m_color.push_back(te::color::RGBAColor(0, 0, 255, 255));

  m_vertex = true;

  setDefaultInterface();
}

te::mnt::ProfileDialog::~ProfileDialog()
{
}

te::map::AbstractLayerPtr te::mnt::ProfileDialog::getLayer()
{
  return m_outputLayer;
}

void te::mnt::ProfileDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  m_ui->m_vectorlayersComboBox->clear();
  m_ui->m_inputLayersComboBox->clear();

  m_ui->m_vectorlayersComboBox->addItem(QString(""), QVariant(""));

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while (it != m_layers.end())
  {
    if (it->get())
    {
      if (it->get()->isValid())
      {
        std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
        if (dsType.get())
        {
          if (dsType->hasGeom())
          {
            std::auto_ptr<te::gm::GeometryProperty>geomProp(te::da::GetFirstGeomProperty(dsType.get()));
            te::gm::GeomType gmType = geomProp->getGeometryType();
            switch (gmType)
            {
              case te::gm::LineStringType:
              case te::gm::MultiLineStringType:
                m_ui->m_vectorlayersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
                break;
              case te::gm::LineStringZType:
              case te::gm::MultiLineStringZType:
              case te::gm::LineStringMType:
              case te::gm::MultiLineStringMType:
              case te::gm::LineStringZMType:
              case te::gm::MultiLineStringZMType:
                m_ui->m_vectorlayersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
              case te::gm::TINType:
              case te::gm::MultiPolygonType:
              case te::gm::PolyhedralSurfaceType:
              case te::gm::PolygonType:
              case te::gm::TINZType:
              case te::gm::MultiPolygonZType:
              case te::gm::PolyhedralSurfaceZType:
              case te::gm::PolygonZType:
              case te::gm::GeometryType:
                m_ui->m_inputLayersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
                break;
              default:
                break;
            }

            geomProp.release();
          }
          if (dsType->hasRaster())
          m_ui->m_inputLayersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
          dsType.release();
        }
      }
    }
    ++it;
  }

  setDefaultInterface();
}

void te::mnt::ProfileDialog::setDefaultInterface()
{
  m_ui->m_selectionradioButton->setChecked(true);
  m_ui->m_trajectorystackedWidget->setCurrentIndex(1);
}

void te::mnt::ProfileDialog::release()
{
  m_visadas.clear();
  DrawSelected(m_visadas, 1, false);
  if (m_tool)
  {
    m_app->getMapDisplay()->setCurrentTool(0);
    m_tool = 0;
  }
  m_tooltype = ToolNone;

  if (m_inputLayer)
  {
    te::edit::Repository* repository = te::edit::RepositoryManager::getInstance().getRepository(m_inputLayer->getId());
    if (repository)
      repository->clear();
  }

  if (m_app)
    connect(m_app->getMapDisplay(), SIGNAL(drawLayersFinished(const QMap<QString, QString>&)), SLOT(onDrawLayersFinished(const QMap<QString, QString>&)));

  te::qt::af::BaseApplication* window = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());
  window->onDrawTriggered();
 
  setDefaultInterface();

}

void te::mnt::ProfileDialog::onInputLayerToolButtonClicked()
{
}

void te::mnt::ProfileDialog::onInputComboBoxChanged(int index)
{
  m_inputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_inputLayersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_inputLayer = it->get();
      std::auto_ptr<te::da::DataSetType> dsType = m_inputLayer->getSchema();
      te::map::DataSetLayer* indsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
      te::da::DataSourcePtr inDataSource = te::da::GetDataSource(indsLayer->getDataSourceId(), true);
      std::auto_ptr<te::da::DataSet> dsRaster = inDataSource->getDataSet(indsLayer->getDataSetName());
      if (dsType->hasRaster()) //GRID
      {
        te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(dsType.get());
        std::auto_ptr<te::rst::Raster> in_raster = dsRaster->getRaster(rasterProp->getName());
        m_ui->m_dummylineEdit->setText(QString::number(in_raster->getBand(0)->getProperty()->m_noDataValue));
        m_tol = in_raster->getResolutionX() * 3;
        in_raster.release();
        dsRaster.release();
      }
      m_srid = m_inputLayer->getSRID();
      dsType.release();
    }
    ++it;
  }

}

void  te::mnt::ProfileDialog::oneditionEnabled(bool checked)
{
  if (!checked)
    return;

  m_ui->m_trajectorystackedWidget->setCurrentIndex(0);

  m_app = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());
  m_app->onSelectionToggled(false);
  m_app->onPanToggled(false);
  m_app->onZoomInToggled(false);
  m_app->onZoomOutToggled(false);

  connect(m_app->getMapDisplay(), SIGNAL(drawLayersFinished(const QMap<QString, QString>&)), SLOT(onDrawTrajectories(const QMap<QString, QString>&)));

  m_ui->m_addPointMousePushButton->toggled(true);

  m_ui->m_vectorlayersComboBox->setCurrentIndex(0);
  m_visadas.clear();
}

void  te::mnt::ProfileDialog::onselectionEnabled(bool checked)
{
  if (!checked)
    return;
  
  release();

  m_ui->m_trajectorystackedWidget->setCurrentIndex(1);
  m_ui->m_vectorlayersComboBox->setCurrentIndex(0);
}


void te::mnt::ProfileDialog::onVectorInputComboBoxChanged(int index)
{
  m_trajectoryLayer = 0;
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_vectorlayersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_trajectoryLayer = it->get();
      if (m_trajectoryLayer == m_inputLayer)
      {
        m_ui->m_vectorlayersComboBox->setCurrentIndex(0);
        m_trajectoryLayer = 0;
        throw te::common::Exception(TE_TR("Trajectory layer must be different from input layer!"));
      }
    }
    ++it;
  }
}

void te::mnt::ProfileDialog::onaddPointMouseToggled(bool checked)
{
  if (!checked && m_tool)
    return;

  if (!m_app)
    return;

  if (!m_inputLayer)
    return;

  m_app->getLayerExplorer()->clearSelection();

  if (m_tooltype != CreateLine || !m_tool)
  {
    if (m_inputLayer->getSRID() != m_app->getMapDisplay()->getSRID())
    {
      QMessageBox::information(this, tr("Profile "), tr("SRID Input Layer different from SRID Display"));
      return;
    }
    m_tooltype = CreateLine;
    m_app->getMapDisplay()->setCurrentTool(0);

    m_tool = new te::edit::CreateLineTool(m_app->getMapDisplay(), m_inputLayer, Qt::ArrowCursor, 0);
    m_app->getMapDisplay()->setCurrentTool(m_tool);

    connect(m_tool, SIGNAL(geometriesEdited()), SLOT(onGeometriesChanged()));
    connect(m_tool, SIGNAL(toolDeleted()), SLOT(onToolDeleted()));

    m_vertex = true;
  }

  m_ui->m_addPointMousePushButton->setChecked(true);
  m_ui->m_changePointPushButton->setChecked(false);
  m_ui->m_addPointPushButton->setChecked(false);
  m_ui->m_deletePointPushButton->setChecked(false);
  m_ui->m_deletePathPushButton->setChecked(false);
  m_ui->m_invertPushButton->setChecked(false);
  DrawSelected(m_visadas, 1, m_vertex);
}

void te::mnt::ProfileDialog::onchangePointToggled(bool checked)
{
  if (!checked && m_tool)
    return;

  if (!m_inputLayer)
    return;

  if (!setVertexEdition())
    return;

  ((te::mnt::ProfileTools*)m_tool)->setType(VERTEX_MOVE);

  m_vertex = true;
  m_ui->m_changePointPushButton->setChecked(true);
  m_ui->m_addPointMousePushButton->setChecked(false);
  m_ui->m_addPointPushButton->setChecked(false);
  m_ui->m_deletePointPushButton->setChecked(false);
  m_ui->m_deletePathPushButton->setChecked(false);
  m_ui->m_invertPushButton->setChecked(false);
  DrawSelected(m_visadas, 1, m_vertex);
}

void te::mnt::ProfileDialog::onaddPointToggled(bool checked)
{
  if (!checked && m_tool)
    return;

  if (!m_inputLayer)
    return;

  if (!setVertexEdition())
    return;

  ((te::mnt::ProfileTools*)m_tool)->setType(VERTEX_ADD);

  m_vertex = true;
  m_ui->m_addPointPushButton->setChecked(true);
  m_ui->m_addPointMousePushButton->setChecked(false);
  m_ui->m_changePointPushButton->setChecked(false);
  m_ui->m_deletePointPushButton->setChecked(false);
  m_ui->m_deletePathPushButton->setChecked(false);
  m_ui->m_invertPushButton->setChecked(false);
  DrawSelected(m_visadas, 1, m_vertex);
}

void te::mnt::ProfileDialog::ondeletePointToggled(bool checked)
{
  if (!checked && m_tool)
    return;

  if (!m_inputLayer)
    return;

  if (!setVertexEdition())
    return;

  ((te::mnt::ProfileTools*)m_tool)->setType(VERTEX_DELETE);

  m_vertex = true;
  m_ui->m_deletePointPushButton->setChecked(true);
  m_ui->m_addPointMousePushButton->setChecked(false);
  m_ui->m_changePointPushButton->setChecked(false);
  m_ui->m_addPointPushButton->setChecked(false);
  m_ui->m_deletePathPushButton->setChecked(false);
  m_ui->m_invertPushButton->setChecked(false);
  DrawSelected(m_visadas, 1, m_vertex);
}

void te::mnt::ProfileDialog::ondeletePathToggled(bool checked)
{
  if (!checked && m_tool)
    return;

  if (!m_inputLayer)
    return;

  if (!setVertexEdition())
    return;

  ((te::mnt::ProfileTools*)m_tool)->setType(LINE_DELETE);
  connect(m_tool, SIGNAL(geometriesEdited()), SLOT(onGeometriesChanged()));
  connect(m_tool, SIGNAL(toolDeleted()), SLOT(onToolDeleted()));

  m_app->getMapDisplay()->setCursor(Qt::ArrowCursor);
  m_vertex = false;
  m_ui->m_deletePathPushButton->setChecked(true);
  m_ui->m_addPointMousePushButton->setChecked(false);
  m_ui->m_changePointPushButton->setChecked(false);
  m_ui->m_addPointPushButton->setChecked(false);
  m_ui->m_deletePointPushButton->setChecked(false);
  m_ui->m_invertPushButton->setChecked(false);
  DrawSelected(m_visadas, 1, m_vertex);
}

void te::mnt::ProfileDialog::oninvertToggled(bool checked)
{
  if (!checked && m_tool)
    return;
 
  if (!m_inputLayer)
    return;

  if (!setVertexEdition())
    return;
  
  ((te::mnt::ProfileTools*)m_tool)->setType(LINE_INVERT);
  connect(m_tool, SIGNAL(geometriesEdited()), SLOT(onGeometriesChanged()));

  m_vertex = false;
  m_ui->m_invertPushButton->setChecked(true);
  m_ui->m_addPointMousePushButton->setChecked(false);
  m_ui->m_changePointPushButton->setChecked(false);
  m_ui->m_addPointPushButton->setChecked(false);
  m_ui->m_deletePointPushButton->setChecked(false);
  m_ui->m_deletePathPushButton->setChecked(false);
  DrawSelected(m_visadas, 1, m_vertex);
}

void te::mnt::ProfileDialog::onDrawTrajectories(const QMap<QString, QString>&)
{
  DrawSelected(m_visadas, 1, m_vertex, false);

}

void te::mnt::ProfileDialog::onToolDeleted()
{
  m_ui->m_addPointMousePushButton->setChecked(false);
  m_ui->m_changePointPushButton->setChecked(false);
  m_ui->m_addPointPushButton->setChecked(false);
  m_ui->m_deletePointPushButton->setChecked(false);
  m_ui->m_deletePathPushButton->setChecked(false);
  m_ui->m_invertPushButton->setChecked(false);
  m_tool = 0;
}

void te::mnt::ProfileDialog::onGeometriesChanged()
{
  if (!m_inputLayer)
    return;

  if (m_tooltype == CreateLine)
  {
    te::edit::CreateLineTool *ct = dynamic_cast<te::edit::CreateLineTool*>(m_tool);
    te::gm::LineString *ls = dynamic_cast<te::gm::LineString*>(ct->buildLine());
    if (ls->getNPoints() < 2)
  {
      delete ls;
      return;
        }

    m_visadas.push_back(ls);
  }
  else
  {
    if (((te::mnt::ProfileTools*)m_tool)->getType() != te::mnt::VERTEX_ADD)
    {
      te::edit::Repository* repository = te::edit::RepositoryManager::getInstance().getRepository(m_inputLayer->getId());
      if (repository)
        repository->clear();

      if (((te::mnt::ProfileTools*)m_tool)->getType() == te::mnt::LINE_DELETE)
        m_visadas = ((te::mnt::ProfileTools*)m_tool)->getLines();
      else
      ((te::mnt::ProfileTools*)m_tool)->setLines(m_visadas);
    }
  }

  testGeometries();

  DrawSelected(m_visadas, 1, m_vertex);

}

void te::mnt::ProfileDialog::onOkPushButtonClicked()
{
  te::map::Visibility visibility;
  if (m_trajectoryLayer)
    visibility = m_trajectoryLayer->getVisibility();

  try
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (!m_tool)
    {
      if (!m_inputLayer)
        throw te::common::Exception(TE_TR("Select an input layer!"));

      if (m_trajectoryLayer)
      {
        if (m_inputLayer->getSRID() != m_trajectoryLayer->getSRID())
            throw te::common::Exception(TE_TR("Can not execute this operation with different SRIDs geometries!"));
      }
    }

    m_dummy = m_ui->m_dummylineEdit->text().toDouble();

    te::map::DataSetLayer* indsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
    if (!indsLayer)
      throw te::common::Exception(TE_TR("Can not execute this operation on this type of layer!"));

    te::da::DataSourcePtr inDataSource = te::da::GetDataSource(indsLayer->getDataSourceId(), true);
    if (!inDataSource.get())
      throw te::common::Exception(TE_TR("The selected input data source can not be accessed!"));

    std::string inDsetName = indsLayer->getDataSetName();

    std::auto_ptr<te::da::DataSet> inDset = inDataSource->getDataSet(inDsetName);
    std::size_t geo_pos = te::da::GetFirstPropertyPos(inDset.get(), te::dt::GEOMETRY_TYPE);
    inDset->moveFirst();
    std::auto_ptr<te::gm::Geometry> gin = inDset->getGeometry(geo_pos);
    if (gin.get())
      if (!gin.get()->is3D())
      {
        std::stringstream msg;
        msg << "'" << inDsetName << "' " << TE_TR("without 3D information!");
        throw te::common::Exception(msg.str());
      }

    std::auto_ptr<te::da::DataSetType> inDsetType(inDataSource->getDataSetType(inDsetName));

    Profile* profile = new Profile();
    profile->setInput(inDataSource, inDsetName, inDsetType, m_dummy);
    profile->setSRID(m_inputLayer->getSRID());

    std::string geostype;

    if (m_ui->m_selectionradioButton->isChecked())
    {
      if (!m_trajectoryLayer)
        throw te::common::Exception(TE_TR("Select an vector layer!"));
      std::auto_ptr<te::da::DataSet> inDset;

      if (m_ui->m_selectCheckBox->isChecked())
      {
        const te::da::ObjectIdSet* objSet = m_trajectoryLayer->getSelected();
        inDset = m_trajectoryLayer->getData(objSet);
      }
      else
        inDset = te::da::GetDataSource(m_trajectoryLayer->getDataSourceId(), true)->getDataSet(m_trajectoryLayer->getDataSetName());

      std::size_t geo_pos = te::da::GetFirstPropertyPos(inDset.get(), te::dt::GEOMETRY_TYPE);

      inDset->moveBeforeFirst();

      m_visadas.clear();

      while (inDset->moveNext())
      {
        std::auto_ptr<te::gm::Geometry> gin = inDset->getGeometry(geo_pos);
        gin->setSRID(m_srid);
        geostype = gin.get()->getGeometryType();

        if (geostype == "LineString")
        {
          te::gm::LineString *l = dynamic_cast<te::gm::LineString*>(gin.get()->clone());
          m_visadas.push_back(l);
        }
        if (geostype == "MultiLineString")
        {
          te::gm::MultiLineString *g = dynamic_cast<te::gm::MultiLineString*>(gin.get());
          std::size_t np = g->getNumGeometries();
          for (std::size_t i = 0; i < np; ++i)
          {
            te::gm::LineString *l = dynamic_cast<te::gm::LineString*>(g->getGeometryN(i)->clone());
            m_visadas.push_back(l);
          }
        }
      }

      if (visibility == te::map::VISIBLE)
        m_trajectoryLayer->setVisibility(te::map::NOT_VISIBLE);
    }
    else
    {
      if (!m_visadas.size())
        throw te::common::Exception(TE_TR("No trajectory selected!"));
    }

    std::string msg = "Profile - Start.";
#ifdef TERRALIB_LOGGER_ENABLED
    te::common::Logger::logDebug("mnt", msg.c_str());
#endif
    // Principal function calling
    std::vector<te::gm::LineString*> profileSet;
    std::auto_ptr<te::da::DataSetType> dsType = m_inputLayer->getSchema();
    if (dsType->hasRaster())
    {
      profile->runRasterProfile(m_visadas, profileSet);
    }
    if (dsType->hasGeom())
    {
      std::auto_ptr<te::gm::GeometryProperty>geomProp(te::da::GetFirstGeomProperty(dsType.get()));
      te::gm::GeomType gmType = geomProp->getGeometryType();
      if (gmType == te::gm::LineStringType || gmType == te::gm::MultiLineStringType ||
        gmType == te::gm::LineStringZType || gmType == te::gm::MultiLineStringZType ||
        gmType == te::gm::LineStringMType || gmType == te::gm::MultiLineStringMType ||
        gmType == te::gm::LineStringZMType || gmType == te::gm::MultiLineStringZMType)
        {
          profile->runIsolinesProfile(m_visadas, profileSet);
        }
      else if (gmType == te::gm::TINType || gmType == te::gm::MultiPolygonType || gmType == te::gm::PolyhedralSurfaceType || gmType == te::gm::PolygonType ||
        gmType == te::gm::TINZType || gmType == te::gm::MultiPolygonZType || gmType == te::gm::PolyhedralSurfaceZType || gmType == te::gm::PolygonZType ||
        gmType == te::gm::GeometryType)//TIN
        {
          profile->runTINProfile(m_visadas, profileSet);
        }
        else
          throw te::common::Exception(TE_TR("Input layer type is invalid!"));
    }

    msg = "Profile - End.";
#ifdef TERRALIB_LOGGER_ENABLED
    te::common::Logger::logDebug("mnt", msg.c_str());
#endif

    DrawSelected(m_visadas, 2, false);

    dsType.release();

    if (!profileSet.size())
      throw te::common::Exception(TE_TR("No profile calculated!"));

    te::mnt::ProfileResultDialog result(m_ui->m_titleLineEdit->text(), m_ui->m_yAxisLineEdit->text(), profileSet, m_color, this->parentWidget());

    QApplication::restoreOverrideCursor();
    if (result.exec() != QDialog::Accepted)
    {
      if (m_trajectoryLayer)
      {
        m_trajectoryLayer->setVisibility(visibility);
        m_visadas.clear();
      }

      DrawSelected(m_visadas, 1, false);

      return;
    }
  }
  catch (const std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, tr("Profile "), e.what());

    if (m_trajectoryLayer)
      m_trajectoryLayer->setVisibility(visibility);
    release();

    return;
  }

  if (m_trajectoryLayer)
    m_trajectoryLayer->setVisibility(visibility);
  release();

  QApplication::restoreOverrideCursor();
  accept();

}

void te::mnt::ProfileDialog::DrawSelected(const std::vector<te::gm::LineString*> visadas, int width, bool vertex, bool draw)
{
  te::qt::af::BaseApplication* window = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());
  if (draw)
  window->onDrawTriggered();

  const te::gm::Envelope& displayExtent = window->getMapDisplay()->getExtent();
  te::qt::widgets::Canvas canvas(window->getMapDisplay()->getDisplayPixmap());
  if (!canvas.getDevice())
    return;

  canvas.setWindow(displayExtent.m_llx, displayExtent.m_lly, displayExtent.m_urx, displayExtent.m_ury);
  canvas.setLineWidth(width);
  canvas.setLineDashStyle(te::map::SolidLine);
  te::qt::widgets::Config2DrawPoints(&canvas, "circle", 8, Qt::red, Qt::red, 1);

  for (unsigned int v = 0; v < visadas.size(); ++v)
  {
    canvas.setLineColor(m_color[v%m_color.size()]);
    canvas.setPointColor(m_color[v%m_color.size()]);
    if (visadas[v]->getSRID() != window->getMapDisplay()->getSRID())
    {
#ifdef TERRALIB_LOGGER_ENABLED
      te::common::Logger::logDebug("mnt", tr("SRID InputLayer different from SRID Display").toLatin1().data());
#endif
     }
    canvas.draw(visadas[v]);
    if (vertex)
    {
      for (size_t p = 0; p < visadas[v]->getNPoints(); p++)
      {
        std::auto_ptr<te::gm::Point> point(visadas[v]->getPointN(p));
        canvas.draw(point.get());
      }
    }

  }
  window->getMapDisplay()->repaint();
  if (m_app)
    m_app->getMapDisplay()->setCursor(Qt::ArrowCursor);

}

void  te::mnt::ProfileDialog::testGeometries()
{
  bool changed = false;
  for (size_t i = 0; i < m_visadas.size(); i++)
  {
    if (m_visadas[i]->getNPoints() < 2)
    {
      m_visadas.erase(m_visadas.begin() + i);
      i--;
      changed = true;
    }
  }
  if (changed)
    ((te::mnt::ProfileTools*)m_tool)->setLines(m_visadas);

}

bool te::mnt::ProfileDialog::setVertexEdition()
{
  if (!m_visadas.size()){
    QMessageBox::information(this, tr("Profile "), TE_TR("No trajectories!"));
    return false;
  }

  if (m_tooltype != EditVertex || !m_tool)
  {
    m_tooltype = EditVertex;

    m_app->getMapDisplay()->setCurrentTool(0);

    m_tool = new te::mnt::ProfileTools(m_app->getMapDisplay(), m_inputLayer);
    connect(m_tool, SIGNAL(geometriesEdited()), SLOT(onGeometriesChanged()));
    connect(m_tool, SIGNAL(toolDeleted()), SLOT(onToolDeleted()));

    m_app->getMapDisplay()->setCurrentTool(m_tool);
    ((te::mnt::ProfileTools*)m_tool)->setLines(m_visadas);
  }

  m_app->getMapDisplay()->setCursor(Qt::ArrowCursor);
  return true;
}

