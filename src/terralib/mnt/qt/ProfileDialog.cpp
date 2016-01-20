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
    m_ui(new Ui::ProfileDialogForm)
{
// add controls
  m_ui->setupUi(this);

// add icons
 // m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("sa-bayesglobal-hint").pixmap(112,48));
 // m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

// connectors
  connect(m_ui->m_rasterLayersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onRasterInputComboBoxChanged(int)));
  
  connect(m_ui->m_dummycheckBox, SIGNAL(toggled(bool)), m_ui->m_dummylineEdit, SLOT(setEnabled(bool)));
  connect(m_ui->m_dummylineEdit, SIGNAL(editingFinished()), this, SLOT(onDummyLineEditEditingFinished()));

  connect(m_ui->m_vectorlayersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onVectorInputComboBoxChanged(int)));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_selectGeometryPushButton, SIGNAL(clicked()), this, SLOT(onSelectGeometryClicked()));
  connect(m_ui->m_clearSelectionPushButton, SIGNAL(clicked()), this, SLOT(onClearSelectionClicked()));

}

te::mnt::ProfileDialog::~ProfileDialog()
{
}

te::map::AbstractLayerPtr te::mnt::ProfileDialog::getLayer()
{
  return m_outputLayer;
}

void te::mnt::ProfileDialog::onInputComboBoxChanged(int index)
{
  /*m_inputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_themeTrajectoryComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_inputLayer = it->get();
      std::auto_ptr<te::da::DataSetType> dsType = m_inputLayer->getSchema();
      std::auto_ptr<te::da::DataSet> inds = m_inputLayer->getData();
      if (dsType->hasGeom())
      {
        QVariant varLayer = m_ui->m_themeTrajectoryComboBox->itemData(index, Qt::UserRole);
        te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

        std::string layerID = m_ui->m_themeTrajectoryComboBox->itemData(index, Qt::UserRole).toString().toStdString();
      }
      if (dsType->hasRaster())
      {
        m_inputType = GRID;
        std::size_t rpos = te::da::GetFirstPropertyPos(inds.get(), te::dt::RASTER_TYPE);
        std::auto_ptr<te::rst::Raster> inputRst(inds->getRaster(rpos).release());
        m_srid = inputRst->getSRID();
        inputRst.release();
      }
      dsType.release();
      break;
    }
    it++;
  }  */
  QVariant varLayer = m_ui->m_vectorlayersComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_inputLayer = layer;

  std::string layerID = m_ui->m_vectorlayersComboBox->itemData(index, Qt::UserRole).toString().toStdString();

}

void te::mnt::ProfileDialog::onSelectGeometryClicked()
{

}
void te::mnt::ProfileDialog::onClearSelectionClicked()
{

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

void te::mnt::ProfileDialog::onRasterInputComboBoxChanged(int index)
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

/*void te::mnt::ProfileDialog::onDummyLineEditEditingFinished()
{
}*/

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
      /*for (std::size_t i = 0; i < props.size(); ++i)
      {
        m_ui->m_attributeComboBox->addItem(QString(props[i]->getName().c_str()), QVariant(props[i]->getName().c_str()));
      }*/
      dsType.release();
    }
    ++it;
  }
}


void te::mnt::ProfileDialog::onOkPushButtonClicked()
{
  //m_ui->m_vectorlayersComboBox->count() == 0
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

    //vector
    te::map::DataSetLayer* indsvectorLayer = dynamic_cast<te::map::DataSetLayer*>(m_vectorinputLayer.get());
    if (!indsvectorLayer)
      throw te::common::Exception(TE_TR("Can not execute this operation on this type of layer!"));

    te::da::DataSourcePtr invectorDataSource = te::da::GetDataSource(indsvectorLayer->getDataSourceId(), true);
    if (!invectorDataSource.get())
      throw te::common::Exception(TE_TR("The selected input data source can not be accessed!"));

    std::string invectorDsetName = indsvectorLayer->getDataSetName();
    std::auto_ptr<te::da::DataSetType> invectorDsetType(invectorDataSource->getDataSetType(invectorDsetName));
    m_srid = m_vectorinputLayer->getSRID();

    //end vector
    
    Profile* profile = new Profile();
    profile->setInput(inrasterDataSource, inDsetName, inDsetType, m_dummy);

    std::auto_ptr<te::rst::Raster> raster = profile->getPrepareRaster();
    te::gm::MultiPoint mpt(0, te::gm::MultiPointZType, m_srid);
    std::string geostype;
    te::gm::MultiLineString isolines(0, te::gm::MultiLineStringZType, m_srid);
    std::vector<te::gm::LineString*> visadas = profile->prepareVector(invectorDsetName, invectorDataSource, geostype);

    // Principal function calling
    std::vector<te::gm::LineString*> profileSet;
    profile->runRasterProfile(raster, visadas, profileSet);

    std::vector<te::color::RGBAColor> color;
    color.push_back(te::color::RGBAColor(255, 0, 0, 255));
    color.push_back(te::color::RGBAColor(255, 0, 255, 255));
    color.push_back(te::color::RGBAColor(0, 255, 255, 255));
    color.push_back(te::color::RGBAColor(37, 127, 0, 255));
    color.push_back(te::color::RGBAColor(0, 0, 255, 255));

    DrawSelected(visadas, color);

    te::mnt::ProfileResultDialog result(m_ui->m_titleLineEdit->text(), m_ui->m_yAxisLineEdit->text(), profileSet, color, this->parentWidget());

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

  for (int v = 0; v < visadas.size(); ++v)
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

