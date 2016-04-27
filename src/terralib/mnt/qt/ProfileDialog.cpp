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

#include "../../edit/qt/tools/CreateLineTool.h"
#include "../../qt/af/events/MapEvents.h"
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
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/canvas/MapDisplay.h"
#include "../../raster.h"
#include "../../statistics/core/Utils.h"

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
    m_tool(0),
    m_app(0)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_selectPushButton->setIcon(QIcon::fromTheme("mnt-profile-select"));
  m_ui->m_addPointMousePushButton->setIcon(QIcon::fromTheme("mnt-profile-line create"));
  m_ui->m_addPointKeyPushButton->setIcon(QIcon::fromTheme("mnt-profile-point create XY"));
  m_ui->m_changePointPushButton->setIcon(QIcon::fromTheme("mnt-profile-vertex move"));
  m_ui->m_addPointPushButton->setIcon(QIcon::fromTheme("mnt-profile-vertex create"));
  m_ui->m_deletePointPushButton->setIcon(QIcon::fromTheme("mnt-profile-vertex delete"));
  m_ui->m_deletePathPushButton->setIcon(QIcon::fromTheme("mnt-profile-delete"));
  m_ui->m_invertPushButton->setIcon(QIcon::fromTheme("mnt-profile-invert"));

// connectors
  connect(m_ui->m_rasterLayersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));
  
  connect(m_ui->m_dummycheckBox, SIGNAL(toggled(bool)), m_ui->m_dummylineEdit, SLOT(setEnabled(bool)));

  connect(m_ui->m_editionradioButton, SIGNAL(toggled(bool)), this, SLOT(oneditionEnabled(bool)));
  connect(m_ui->m_selectionradioButton, SIGNAL(toggled(bool)), this, SLOT(onselectionEnabled(bool)));

  connect(m_ui->m_vectorlayersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onVectorInputComboBoxChanged(int)));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));

  connect(m_ui->m_selectPushButton, SIGNAL(toggled(bool)), SLOT(onselectLineToggled(bool)));
  connect(m_ui->m_addPointMousePushButton, SIGNAL(toggled(bool)), SLOT(onaddPointMouseToggled(bool)));
  connect(m_ui->m_addPointKeyPushButton, SIGNAL(toggled(bool)), SLOT(onaddPointKeyToggled(bool)));
  connect(m_ui->m_changePointPushButton, SIGNAL(toggled(bool)), SLOT(onchangePointToggled(bool)));
  connect(m_ui->m_addPointPushButton, SIGNAL(toggled(bool)), SLOT(onaddPointToggled(bool)));
  connect(m_ui->m_deletePointPushButton, SIGNAL(toggled(bool)), SLOT(ondeletePointToggled(bool)));
  connect(m_ui->m_deletePathPushButton, SIGNAL(toggled(bool)), SLOT(ondeletePathToggled(bool)));
  connect(m_ui->m_invertPushButton, SIGNAL(toggled(bool)), SLOT(oninvertToggled(bool)));

  m_ui->m_selectionradioButton->setChecked(true);
  m_ui->m_trajectorystackedWidget->setCurrentIndex(1);
}

te::mnt::ProfileDialog::~ProfileDialog()
{
  if (m_tool)
  {
    m_app->getMapDisplay()->removeEventFilter(m_tool);
    delete m_tool;
  }
}

te::map::AbstractLayerPtr te::mnt::ProfileDialog::getLayer()
{
  return m_outputLayer;
}

void te::mnt::ProfileDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

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
            //te::gm::GeomType gmType = geomProp->getGeometryType();
            /*if (gmType == te::gm::PolygonType || gmType == te::gm::MultiPolygonType || gmType == te::gm::PolyhedralSurfaceType ||
              gmType == te::gm::PolygonZType || gmType == te::gm::MultiPolygonZType || gmType == te::gm::PolyhedralSurfaceZType ||
              gmType == te::gm::PolygonMType || gmType == te::gm::MultiPolygonMType || gmType == te::gm::PolyhedralSurfaceMType ||
              gmType == te::gm::PolygonZMType || gmType == te::gm::MultiPolygonZMType || gmType == te::gm::PolyhedralSurfaceZMType
              )
            {
              m_ui->m_vectorlayersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
            }*/
            m_ui->m_vectorlayersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));

            geomProp.release();
          }
          if (dsType->hasRaster())
          {
            m_ui->m_rasterLayersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
          }
          dsType.release();
        }
      }
    }
    ++it;
  }
}

void te::mnt::ProfileDialog::onInputComboBoxChanged(int index)
{
  m_rasterinputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_rasterLayersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_rasterinputLayer = it->get();
      std::auto_ptr<te::da::DataSetType> dsType = m_rasterinputLayer->getSchema();
      te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(dsType.get());
      te::map::DataSetLayer* indsLayer = dynamic_cast<te::map::DataSetLayer*>(m_rasterinputLayer.get());
      te::da::DataSourcePtr inDataSource = te::da::GetDataSource(indsLayer->getDataSourceId(), true);
      std::auto_ptr<te::da::DataSet> dsRaster = inDataSource->getDataSet(indsLayer->getDataSetName());
      std::auto_ptr<te::rst::Raster> in_raster = dsRaster->getRaster(rasterProp->getName());
      m_ui->m_dummylineEdit->setText(QString::number(in_raster->getBand(0)->getProperty()->m_noDataValue));
      in_raster.release();
      dsRaster.release();
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
  m_srid = m_app->getMapDisplay()->getSRID();

  m_ui->m_addPointMousePushButton->toggled(true);

}


void  te::mnt::ProfileDialog::onselectionEnabled(bool checked)
{
  if (!checked)
    return;

  m_ui->m_trajectorystackedWidget->setCurrentIndex(1);
}


void te::mnt::ProfileDialog::onVectorInputComboBoxChanged(int index)
{
  m_vectorinputLayer = 0;
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_vectorlayersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_vectorinputLayer = it->get();

      std::auto_ptr<te::da::DataSetType> dsType = m_vectorinputLayer->getSchema();
      std::vector<te::dt::Property*> props = dsType->getProperties();
      dsType.release();
    }
    ++it;
  }
}

void te::mnt::ProfileDialog::onselectLineToggled(bool checked)
{
  if (!checked)
    return;
  if (m_tool)
    delete m_tool;

  m_tool = new te::mnt::ProfileTools(m_app->getMapDisplay(), LINE_SELECT);
  ((te::mnt::ProfileTools*)m_tool)->setLines(m_visadas);
  m_app->getMapDisplay()->installEventFilter(m_tool);

}

void te::mnt::ProfileDialog::onaddPointMouseToggled(bool checked)
{
  if (!checked)
    return;

  if (!m_app)
    return;

  if (m_tool)
    delete m_tool;

  m_tool = new te::edit::CreateLineTool(m_app->getMapDisplay(), m_rasterinputLayer, Qt::ArrowCursor, 0);
  m_app->getMapDisplay()->installEventFilter(m_tool);

  connect(m_tool, SIGNAL(geometriesEdited()), SLOT(onGeometriesChanged()));

}

void te::mnt::ProfileDialog::onaddPointKeyToggled(bool checked)
{
  if (!checked)
    return;

 // m_tool->setType(NONE);
}

void te::mnt::ProfileDialog::onchangePointToggled(bool checked)
{
  if (!checked)
    return;

 // m_tool->setType(VERTEX_MOVE);
}

void te::mnt::ProfileDialog::onaddPointToggled(bool checked)
{
  if (!checked)
    return;

 // m_tool->setType(VERTEX_ADD);
}

void te::mnt::ProfileDialog::ondeletePointToggled(bool checked)
{
  if (!checked)
    return;

 // m_tool->setType(VERTEX_DELETE);
}

void te::mnt::ProfileDialog::ondeletePathToggled(bool checked)
{
  if (!checked)
    return;

 // m_tool->setType(NONE);

}

void te::mnt::ProfileDialog::oninvertToggled(bool checked)
{
  if (!checked)
    return;
 // m_tool->setType(NONE);
}

void te::mnt::ProfileDialog::onGeometriesChanged()
{
  te::edit::CreateLineTool *ct= dynamic_cast<te::edit::CreateLineTool*>(m_tool);
  te::gm::LineString *ls = dynamic_cast<te::gm::LineString*>(ct->buildLine());
  m_visadas.push_back(ls);
}

void te::mnt::ProfileDialog::onOkPushButtonClicked()
{
  try
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //raster
    if (!m_rasterinputLayer.get() || !m_vectorinputLayer.get())
      throw te::common::Exception(TE_TR("Select an input layer!"));

    if (m_rasterinputLayer->getSRID() != m_vectorinputLayer->getSRID())
      throw te::common::Exception(TE_TR("Can not execute this operation with different SRIDs geometries!"));

    m_dummy = m_ui->m_dummylineEdit->text().toDouble();

    te::map::DataSetLayer* indsrasterLayer = dynamic_cast<te::map::DataSetLayer*>(m_rasterinputLayer.get());
    if (!indsrasterLayer)
      throw te::common::Exception(TE_TR("Can not execute this operation on this type of layer!"));

    te::da::DataSourcePtr inrasterDataSource = te::da::GetDataSource(indsrasterLayer->getDataSourceId(), true);
    if (!inrasterDataSource.get())
      throw te::common::Exception(TE_TR("The selected input data source can not be accessed!"));

    std::string inDsetName = indsrasterLayer->getDataSetName();
    std::auto_ptr<te::da::DataSetType> inDsetType(inrasterDataSource->getDataSetType(inDsetName));
    // end raster

    Profile* profile = new Profile();
    profile->setInput(inrasterDataSource, inDsetName, inDsetType, m_dummy);
    std::auto_ptr<te::rst::Raster> raster = profile->getPrepareRaster();
    std::string geostype;

    //vector
    if (m_ui->m_selectionradioButton->isChecked())
    {
      te::map::DataSetLayer* indsvectorLayer = dynamic_cast<te::map::DataSetLayer*>(m_vectorinputLayer.get());
      if (!indsvectorLayer)
        throw te::common::Exception(TE_TR("Can not execute this operation on this type of layer!"));

      te::da::DataSourcePtr invectorDataSource = te::da::GetDataSource(indsvectorLayer->getDataSourceId(), true);
      if (!invectorDataSource.get())
        throw te::common::Exception(TE_TR("The selected input data source can not be accessed!"));

      std::string invectorDsetName = indsvectorLayer->getDataSetName();
      std::auto_ptr<te::da::DataSetType> invectorDsetType(invectorDataSource->getDataSetType(invectorDsetName));
      m_srid = m_vectorinputLayer->getSRID();
      m_visadas = profile->prepareVector(invectorDsetName, invectorDataSource, geostype);
    }

    //end vector

//    te::gm::MultiPoint mpt(0, te::gm::MultiPointZType, m_srid);
//    te::gm::MultiLineString isolines(0, te::gm::MultiLineStringZType, m_srid);

    // Principal function calling
    std::vector<te::gm::LineString*> profileSet;
    profile->runRasterProfile(raster, m_visadas, profileSet);

    std::vector<te::color::RGBAColor> color;
    color.push_back(te::color::RGBAColor(255, 0, 0, 255));
    color.push_back(te::color::RGBAColor(255, 0, 255, 255));
    color.push_back(te::color::RGBAColor(0, 255, 255, 255));
    color.push_back(te::color::RGBAColor(37, 127, 0, 255));
    color.push_back(te::color::RGBAColor(0, 0, 255, 255));

    DrawSelected(m_visadas, color);

    te::mnt::ProfileResultDialog result(m_ui->m_titleLineEdit->text(), m_ui->m_yAxisLineEdit->text(), profileSet, color, this->parentWidget());

    QApplication::restoreOverrideCursor();
    if (result.exec() != QDialog::Accepted)
    {
      return;
    }

   //result = profile->run(raster);
 }
  catch (const std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, tr("Profile "), e.what());
    return;
  }

  QApplication::restoreOverrideCursor();
  accept();
  
}

void te::mnt::ProfileDialog::DrawSelected(const std::vector<te::gm::LineString*> visadas, const std::vector<te::color::RGBAColor>color)
{
  te::qt::af::BaseApplication* window = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());
  const te::gm::Envelope& displayExtent = window->getMapDisplay()->getExtent();
  te::qt::widgets::Canvas canvas(window->getMapDisplay()->getDisplayPixmap());
  canvas.setWindow(displayExtent.m_llx, displayExtent.m_lly, displayExtent.m_urx, displayExtent.m_ury);
  canvas.setLineWidth(3);
  canvas.setLineDashStyle(te::map::SolidLine);

  bool needRemap = false;

  if ((m_srid != TE_UNKNOWN_SRS) && (window->getMapDisplay()->getSRID() != TE_UNKNOWN_SRS) && (m_srid != window->getMapDisplay()->getSRID()))
    needRemap = true;

  for (unsigned int v = 0; v < visadas.size(); ++v)
  {
    canvas.setLineColor(color[v%color.size()]);
    if (needRemap)
    {
      visadas[v]->setSRID(m_srid);
      visadas[v]->transform(window->getMapDisplay()->getSRID());
    }
    canvas.draw(visadas[v]);
  }
  window->getMapDisplay()->repaint();
}

