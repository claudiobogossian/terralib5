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
\file terralib/mnt/TINGenerationDialog.cpp

\brief A dialog for TIN generation
*/


//terralib
#include "../../common/UnitsOfMeasureManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/DataSetLayer.h"
#include "../../mnt/core/TINGeneration.h"
#include "../../qt/plugins/cellspace/CreateCellularSpaceDialog.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../srs/SpatialReferenceSystemManager.h"

#include "TINGenerationDialog.h"
#include "ui_TINGenerationDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>


te::mnt::TINGenerationDialog::TINGenerationDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::TINGenerationDialogForm),
  m_layers(std::list<te::map::AbstractLayerPtr>())
{
  // add controls
  m_ui->setupUi(this);

  //signals
  connect(m_ui->m_isolinescomboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onIsolinesComboBoxChanged(int)));
  connect(m_ui->m_samplescomboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSamplesComboBoxChanged(int)));

  connect(m_ui->m_scalepushButton, SIGNAL(clicked()), this, SLOT(onScalePushButtonClicked()));

  connect(m_ui->m_yesradioButton, SIGNAL(toggled(bool)), this, SLOT(onYesToggled()));
  connect(m_ui->m_noradioButton, SIGNAL(toggled(bool)), this, SLOT(onNoToggled()));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  m_ui->m_noradioButton->setChecked(true);
  m_hasiso = false;
  m_hassample = false;
  m_ui->m_isolinescomboBox->addItem(QString(""), QVariant(""));
  m_ui->m_samplescomboBox->addItem(QString(""), QVariant(""));

}

te::mnt::TINGenerationDialog::~TINGenerationDialog()
{

}


void te::mnt::TINGenerationDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while (it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if (dsType->hasGeom())
    {
      std::auto_ptr<te::gm::GeometryProperty>geomProp(te::da::GetFirstGeomProperty(dsType.get()));
      te::gm::GeomType gmType = geomProp->getGeometryType();
      switch (gmType)
      {
      case te::gm::PointType:
      case te::gm::PointZType:
      case te::gm::PointMType:
      case te::gm::PointZMType:
      case te::gm::MultiPointType:
      case te::gm::MultiPointZType:
      case te::gm::MultiPointMType:
      case te::gm::MultiPointZMType:
        m_ui->m_samplescomboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
        break;
      case te::gm::LineStringType:
      case te::gm::LineStringZType:
      case te::gm::LineStringMType:
      case te::gm::LineStringZMType:
      case te::gm::MultiLineStringType:
      case te::gm::MultiLineStringZType:
      case te::gm::MultiLineStringMType:
      case te::gm::MultiLineStringZMType:
        m_ui->m_isolinescomboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
        m_ui->m_breaklinecomboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
        break;
      default:
        break;
      }
    }
    dsType.release();
    ++it;
  }
}


void te::mnt::TINGenerationDialog::onIsolinesComboBoxChanged(int index)
{
  m_ui->m_isolinesZcomboBox->clear();
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_isolinescomboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_isolinesLayer = selectedLayer;
      m_hasiso = true;
      std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
      std::vector<te::dt::Property*> props = dsType->getProperties();
      for (std::size_t i = 0; i < props.size(); ++i)
      {
        switch (props[i]->getType())
        {
        case te::dt::FLOAT_TYPE:
        case te::dt::DOUBLE_TYPE:
        case te::dt::INT16_TYPE:
        case te::dt::INT32_TYPE:
        case te::dt::INT64_TYPE:
        case te::dt::UINT16_TYPE:
        case te::dt::UINT32_TYPE:
        case te::dt::UINT64_TYPE:
        case te::dt::NUMERIC_TYPE:
          m_ui->m_isolinesZcomboBox->addItem(QString(props[i]->getName().c_str()), QVariant(props[i]->getName().c_str()));
          break;
        }
      }
      break;
    }
    it++;
  }
}

void te::mnt::TINGenerationDialog::onSamplesComboBoxChanged(int index)
{
  m_ui->m_samplesZcomboBox->clear();
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_samplescomboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_samplesLayer = selectedLayer;
      m_hassample = true;
      std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
      std::vector<te::dt::Property*> props = dsType->getProperties();
      for (std::size_t i = 0; i < props.size(); ++i)
      {
        switch (props[i]->getType())
        {
        case te::dt::FLOAT_TYPE:
        case te::dt::DOUBLE_TYPE:
        case te::dt::INT16_TYPE:
        case te::dt::INT32_TYPE:
        case te::dt::INT64_TYPE:
        case te::dt::UINT16_TYPE:
        case te::dt::UINT32_TYPE:
        case te::dt::UINT64_TYPE:
        case te::dt::NUMERIC_TYPE:
          m_ui->m_samplesZcomboBox->addItem(QString(props[i]->getName().c_str()), QVariant(props[i]->getName().c_str()));
          break;
        }
      }
    }
    it++;
  }
}

void te::mnt::TINGenerationDialog::onScalePushButtonClicked()
{
  m_scale = m_ui->m_scalelineEdit->text().toDouble();
  m_breaktol = m_tol = (m_scale * 0.4) / 1000;
  m_distance = m_tol * 20;
  m_edgeSize = m_tol / 5;

  m_ui->m_minedgelineEdit->setText(QString::number(m_edgeSize));
  m_ui->m_tollineEdit->setText(QString::number(m_tol));
  m_ui->m_breaktollineEdit->setText(QString::number(m_breaktol));
  m_ui->m_distancelineEdit->setText(QString::number(m_distance));

}

void te::mnt::TINGenerationDialog::onYesToggled()
{
  m_ui->m_breaklinelabel->setEnabled(true);
  m_ui->m_breaklinecomboBox->setEnabled(true);
  m_ui->m_breaktollabel->setEnabled(true);
  m_ui->m_breaktollineEdit->setEnabled(true);
}

void te::mnt::TINGenerationDialog::onNoToggled()
{
  m_ui->m_breaklinelabel->setDisabled(true);
  m_ui->m_breaklinecomboBox->setDisabled(true);
  m_ui->m_breaktollabel->setDisabled(true);
  m_ui->m_breaktollineEdit->setDisabled(true);
}

void te::mnt::TINGenerationDialog::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if (dsPtrList.empty())
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;

  m_toFile = false;
}

void te::mnt::TINGenerationDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr("Shapefile (*.shp *.SHP);;"), 0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;

  boost::filesystem::path outfile(fileName.toStdString());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());

  m_toFile = true;
  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

void te::mnt::TINGenerationDialog::onHelpPushButtonClicked()
{
}

void te::mnt::TINGenerationDialog::onOkPushButtonClicked()
{
  int srid;

  try
  {
    te::mnt::TINGeneration *Tin = new te::mnt::TINGeneration();

    // Checking consistency of the input layer where the buffer will executed
    if (m_isolinesLayer.get())
    {
      te::map::DataSetLayer* dsisoLayer = dynamic_cast<te::map::DataSetLayer*>(m_isolinesLayer.get());
      if (!dsisoLayer)
      {
        QMessageBox::information(this, "TIN Generation", "Can not execute this operation on this type of layer.");
        return;
      }

      te::da::DataSourcePtr inDataSource = te::da::GetDataSource(dsisoLayer->getDataSourceId(), true);
      if (!inDataSource.get())
      {
        QMessageBox::information(this, "TIN Generation", "The selected input data source can not be accessed.");
        return;
      }

      std::string inDsetNameiso = dsisoLayer->getDataSetName();
      Tin->setInput(inDataSource, inDsetNameiso, inDataSource->getDataSetType(inDsetNameiso), te::mnt::Isolines);
      srid = dsisoLayer->getSRID();
    }
    if (m_samplesLayer.get())
    {
      te::map::DataSetLayer* dssampleLayer = dynamic_cast<te::map::DataSetLayer*>(m_samplesLayer.get());
      if (!dssampleLayer)
      {
        QMessageBox::information(this, "TIN Generation", "Can not execute this operation on this type of layer.");
        return;
      }

      te::da::DataSourcePtr inDataSource = te::da::GetDataSource(dssampleLayer->getDataSourceId(), true);
      if (!inDataSource.get())
      {
        QMessageBox::information(this, "TIN Generation", "The selected input data source can not be accessed.");
        return;
      }

      std::string inDsetNamesample = dssampleLayer->getDataSetName();
      Tin->setInput(inDataSource, inDsetNamesample, inDataSource->getDataSetType(inDsetNamesample), te::mnt::Samples);
      srid = dssampleLayer->getSRID();
    }

    std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();

    if (m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

      if (boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, "TIN Generation", "Output file already exists. Remove it or select a new name and try again.");
        return;
      }

      std::size_t idx = outputdataset.find(".");
      if (idx != std::string::npos)
        outputdataset = outputdataset.substr(0, idx);

      std::map<std::string, std::string> dsinfo;
      dsinfo["URI"] = uri.string();

      te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
      dsOGR->setConnectionInfo(dsinfo);
      dsOGR->open();

      if (dsOGR->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "TIN Generation", "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.");
        return;
      }
      Tin->setOutput(dsOGR, outputdataset);
    }
    else
    {
      te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
      if (!aux)
      {
        QMessageBox::information(this, "TIN Generation", "The selected output datasource can not be accessed.");
        return;
      }
      if (aux->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "TIN Generation", "Dataset already exists. Remove it or select a new name and try again. ");
        return;
      }
    
      Tin->setOutput(aux, outputdataset);
    }

    Tin->setSRID(srid);

    te::common::UnitOfMeasurePtr unitin = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(srid);
    te::common::UnitOfMeasurePtr unitout = te::common::UnitsOfMeasureManager::getInstance().find("metre");

    if (unitin->getId() != te::common::UOM_Metre)
    {
      convertPlanarToAngle(m_tol, unitout);
      convertPlanarToAngle(m_distance, unitout);
      convertPlanarToAngle(m_edgeSize, unitout);
    }

    Tin->setParams(m_tol, m_distance, m_edgeSize, m_ui->m_isolinesZcomboBox->currentText().toStdString(), m_ui->m_samplesZcomboBox->currentText().toStdString());

    this->setCursor(Qt::WaitCursor);

    bool result = Tin->run();

  }
  catch (const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);
    QMessageBox::information(this, "TIN Generation", e.what());
    return;
  }
  this->setCursor(Qt::ArrowCursor);
  accept();
}

void te::mnt::TINGenerationDialog::onCancelPushButtonClicked()
{
  reject();
}


bool te::mnt::TINGenerationDialog::convertPlanarToAngle(double& val, te::common::UnitOfMeasurePtr unit)
{
  switch (unit->getId())
  {
  case te::common::UOM_Metre:
    val /= 111000;            // 1 degree = 111.000 meters
    break;
  case te::common::UOM_Kilometre:
    val /= 111;               // 1 degree = 111 kilometers
    break;
  case te::common::UOM_Foot:
    val /= 364173.24;        //  1 feet  = 3.28084 meters
    break;
  default:
    return false;
  }
  return true;
}
