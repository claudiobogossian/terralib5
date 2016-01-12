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
\file terralib/mnt/qt/VolumeDialog.cpp

\brief A dialog for Calculate Volume
*/

//terralib
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/DataSetLayer.h"
#include "../../mnt/core/Volume.h"
#include "../../qt/af/ApplicationController.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/rp/Utils.h"
#include "../../raster.h"

#include "VolumeDialog.h"
#include "VolumeResultDialog.h"
#include "ui_VolumeDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

te::mnt::VolumeDialog::VolumeDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::VolumeDialogForm),
  m_layers(std::list<te::map::AbstractLayerPtr>())
{
  // add controls
  m_ui->setupUi(this);

  //signals
  connect(m_ui->m_rasterlayersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onRasterInputComboBoxChanged(int)));

  connect(m_ui->m_dummycheckBox, SIGNAL(toggled(bool)), m_ui->m_dummylineEdit, SLOT(setEnabled(bool)));
  connect(m_ui->m_dummylineEdit, SIGNAL(editingFinished()), this, SLOT(onDummyLineEditEditingFinished()));

  connect(m_ui->m_vectorlayersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onVectorInputComboBoxChanged(int)));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::mnt::VolumeDialog::~VolumeDialog()
{

}

void te::mnt::VolumeDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
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
            te::gm::GeomType gmType = geomProp->getGeometryType();
            if (gmType == te::gm::PolygonType || gmType == te::gm::MultiPolygonType || gmType == te::gm::PolyhedralSurfaceType ||
              gmType == te::gm::PolygonZType || gmType == te::gm::MultiPolygonZType || gmType == te::gm::PolyhedralSurfaceZType ||
              gmType == te::gm::PolygonMType || gmType == te::gm::MultiPolygonMType || gmType == te::gm::PolyhedralSurfaceMType ||
              gmType == te::gm::PolygonZMType || gmType == te::gm::MultiPolygonZMType || gmType == te::gm::PolyhedralSurfaceZMType
              )
            {
              m_ui->m_vectorlayersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
            }
            geomProp.release();
          }
          if (dsType->hasRaster())
          {
            m_ui->m_rasterlayersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
          }
          dsType.release();
        }
      }
    }
    ++it;
  }
}

void te::mnt::VolumeDialog::onRasterInputComboBoxChanged(int index)
{
  m_rasterinputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_rasterlayersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
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

void te::mnt::VolumeDialog::onDummyLineEditEditingFinished()
{
}

void te::mnt::VolumeDialog::onVectorInputComboBoxChanged(int index)
{
  m_vectorinputLayer = 0;
  m_ui->m_attributeComboBox->clear();
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_vectorlayersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_vectorinputLayer = it->get();

      std::auto_ptr<te::da::DataSetType> dsType = m_vectorinputLayer->getSchema();
      std::vector<te::dt::Property*> props = dsType->getProperties();
      for (std::size_t i = 0; i < props.size(); ++i)
      {
        m_ui->m_attributeComboBox->addItem(QString(props[i]->getName().c_str()), QVariant(props[i]->getName().c_str()));
      }
      dsType.release();
    }
    ++it;
  }
}

void te::mnt::VolumeDialog::onHelpPushButtonClicked()
{
}

void te::mnt::VolumeDialog::onOkPushButtonClicked()
{

  if (!m_rasterinputLayer.get() || !m_vectorinputLayer.get())
  {
    QMessageBox::information(this, tr("Volume"), tr("Select an input layer!"));
    return;
  }

  if (m_rasterinputLayer->getSRID() != m_vectorinputLayer->getSRID())
  {
    QMessageBox::information(this, tr("Volume"), tr("Can not execute this operation with different SRIDs geometries!"));
    return;
  }

  te::map::DataSetLayer* indsrasterLayer = dynamic_cast<te::map::DataSetLayer*>(m_rasterinputLayer.get());
  if (!indsrasterLayer)
  {
    QMessageBox::information(this, tr("Volume"), tr("Can not execute this operation on this type of layer!"));
    return;
  }

  te::da::DataSourcePtr inrasterDataSource = te::da::GetDataSource(indsrasterLayer->getDataSourceId(), true);
  if (!inrasterDataSource.get())
  {
    QMessageBox::information(this, tr("Volume"), tr("The selected input data source can not be accessed!"));
    return;
  }

  std::string inrasterDsetName = indsrasterLayer->getDataSetName();
  std::auto_ptr<te::da::DataSetType> inrasterDsetType(inrasterDataSource->getDataSetType(inrasterDsetName));


  const te::da::ObjectIdSet* OidSet = 0;
  if (m_ui->m_selectCheckBox->isChecked())
  {
    OidSet = m_vectorinputLayer->getSelected();
    if (!OidSet)
    {
      QMessageBox::information(this, "Intersection", "Select the layer objects to perform the intersection operation.");
      return;
    }
  }

  te::map::DataSetLayer* indsvectorLayer = dynamic_cast<te::map::DataSetLayer*>(m_vectorinputLayer.get());
  if (!indsvectorLayer)
  {
    QMessageBox::information(this, tr("Volume"), tr("Can not execute this operation on this type of layer!"));
    return;
  }

  te::da::DataSourcePtr invectorDataSource = te::da::GetDataSource(indsvectorLayer->getDataSourceId(), true);
  if (!invectorDataSource.get())
  {
    QMessageBox::information(this, tr("Volume"), tr("The selected input data source can not be accessed!"));
    return;
  }

  std::string invectorDsetName = indsvectorLayer->getDataSetName();
  std::auto_ptr<te::da::DataSetType> invectorDsetType(invectorDataSource->getDataSetType(invectorDsetName));
  
  Volume *calvol = new te::mnt::Volume();

  std::string attr = m_ui->m_attributeComboBox->currentText().toStdString();

  calvol->setSRID(m_vectorinputLayer->getSRID());
  calvol->setInput(inrasterDataSource, inrasterDsetName, inrasterDsetType, invectorDataSource, invectorDsetName, invectorDsetType, OidSet);
  calvol->setParams(m_ui->m_quotaLineEdit->text().toDouble(), attr, m_ui->m_dummylineEdit->text().toDouble());

  this->setCursor(Qt::WaitCursor);
  if (calvol->run())
  { 
    this->setCursor(Qt::ArrowCursor);
    std::vector<std::string> polyvec;
    std::vector<std::string> cortevec;
    std::vector<std::string> aterrovec;
    std::vector<std::string> areavec;
    std::vector<std::string> iquotavec;
    calvol->getResults(polyvec, cortevec, aterrovec, areavec, iquotavec);

    te::mnt::VolumeResultDialog result(polyvec,
                                       cortevec,
                                       aterrovec,
                                       areavec,
                                       iquotavec,
                                       attr,
                                       this->parentWidget());

    if (result.exec() != QDialog::Accepted)
      return;

  }

  delete calvol;

  this->setCursor(Qt::ArrowCursor);

  accept();

}

void te::mnt::VolumeDialog::onCancelPushButtonClicked()
{
  reject();
}
