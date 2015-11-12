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
\file terralib/mnt/qt/SmoothIsolinesDialog.cpp

\brief A dialog for Smooth Isolines
*/

//terralib
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/DataSetLayer.h"
#include "../../mnt/core/Smooth.h"
#include "../../mnt/core/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/rp/Utils.h"
#include "../../raster.h"
#include "../../srs/SpatialReferenceSystemManager.h"

#include "SmoothIsolinesDialog.h"
#include "ui_SmoothIsolinesDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::mnt::SmoothIsolinesDialog::SmoothIsolinesDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::SmoothIsolinesDialogForm),
  m_layers(std::list<te::map::AbstractLayerPtr>())
{
  // add controls
  m_ui->setupUi(this);

  //signals
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));

  connect(m_ui->m_scalePushButton, SIGNAL(clicked()), this, SLOT(onScalePushButtonClicked()));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::mnt::SmoothIsolinesDialog::~SmoothIsolinesDialog()
{

}

void te::mnt::SmoothIsolinesDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
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
            if (gmType == te::gm::LineStringType || gmType == te::gm::LineStringZType || gmType == te::gm::LineStringMType ||
              gmType == te::gm::LineStringZMType || gmType == te::gm::MultiLineStringType || gmType == te::gm::MultiLineStringZType ||
              gmType == te::gm::MultiLineStringMType || gmType == te::gm::MultiLineStringZMType)
            {
              m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
            }
          }
          dsType.release();
        }
      }
    }
    ++it;
  }
}

void te::mnt::SmoothIsolinesDialog::onInputComboBoxChanged(int index)
{
  m_inputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  m_ui->m_Zlabel->setVisible(false);
  m_ui->m_ZcomboBox->clear();
  m_ui->m_ZcomboBox->setVisible(false);

  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_inputLayer = selectedLayer;
      std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();

      std::auto_ptr<te::gm::GeometryProperty>geomProp(te::da::GetFirstGeomProperty(dsType.get()));
      te::gm::GeomType gmType = geomProp->getGeometryType();
      if (gmType == te::gm::LineStringType || gmType == te::gm::MultiLineStringType)
      {
        m_ui->m_Zlabel->setVisible(true);
        m_ui->m_ZcomboBox->setVisible(true);
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
            m_ui->m_ZcomboBox->addItem(QString(props[i]->getName().c_str()), QVariant(props[i]->getName().c_str()));
            break;
          }
        }
      }
      geomProp.release();
      dsType.release();

      break;
    }
    ++it;
  }
}


void te::mnt::SmoothIsolinesDialog::onScalePushButtonClicked()
{
  m_scale = m_ui->m_scaleLineEdit->text().toDouble();
  m_factor = (m_scale * 0.4) / 1000;
  m_maxdist = m_factor;

  m_ui->m_factorLineEdit->setText(QString::number(m_factor));
  m_ui->m_distanceLineEdit->setText(QString::number(m_maxdist));

}

te::map::AbstractLayerPtr te::mnt::SmoothIsolinesDialog::getLayer()
{
  return m_outputLayer;
}


void te::mnt::SmoothIsolinesDialog::onTargetDatasourceToolButtonPressed()
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

void te::mnt::SmoothIsolinesDialog::onTargetFileToolButtonPressed()
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

void te::mnt::SmoothIsolinesDialog::onHelpPushButtonClicked()
{
}

void te::mnt::SmoothIsolinesDialog::onOkPushButtonClicked()
{

  if (!m_inputLayer.get())
  {
    QMessageBox::information(this, tr("Isolines Smooth"), tr("Select an input layer!"));
    return;
  }

  te::map::DataSetLayer* indsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
  if (!indsLayer)
  {
    QMessageBox::information(this, tr("Isolines Smooth"), tr("Can not execute this operation on this type of layer!"));
    return;
  }

  te::da::DataSourcePtr inDataSource = te::da::GetDataSource(indsLayer->getDataSourceId(), true);
  if (!inDataSource.get())
  {
    QMessageBox::information(this, tr("Isolines Smooth"), tr("The selected input data source can not be accessed!"));
    return;
  }

  std::string inDsetName = indsLayer->getDataSetName();
  std::auto_ptr<te::da::DataSetType> inDsetType(inDataSource->getDataSetType(inDsetName));

  // Checking consistency of output paramenters
  if (m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Isolines Smooth"), tr("Select a repository for the resulting layer."));
    return;
  }

  if (m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Isolines Smooth"), tr("Define a name for the resulting layer."));
    return;
  }
  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();

  std::map<std::string, std::string> outdsinfo;
  boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

  if (m_toFile)
  {
    if (boost::filesystem::exists(uri))
    {
      QMessageBox::information(this, tr("Isolines Smooth"), tr("Output file already exists! Remove it or select a new name and try again."));
      return;
    }

    std::size_t idx = outputdataset.find(".");
    if (idx != std::string::npos)
      outputdataset = outputdataset.substr(0, idx);

    outdsinfo["URI"] = uri.string();
  }

  this->setCursor(Qt::WaitCursor);
  
  Smooth *iso = new te::mnt::Smooth();
  iso->setInput(inDataSource, inDsetName, inDsetType);
  if (m_toFile)
  {
    te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
    dsOGR->setConnectionInfo(outdsinfo);
    dsOGR->open();

    if (dsOGR->dataSetExists(outputdataset))
    {
      QMessageBox::information(this, tr("Isolines Smooth"), tr("There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again."));
      return;
    }

    iso->setOutput(dsOGR, outputdataset);
  }
  else
  {
    te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
    if (!aux)
    {
      QMessageBox::information(this, tr("Isolines Smooth"), tr("The selected output datasource can not be accessed."));
      return;
    }
    if (aux->dataSetExists(outputdataset))
    {
      QMessageBox::information(this, tr("Isolines Smooth"), tr("Dataset already exists. Remove it or select a new name and try again. "));
      return;
    }
    iso->setOutput(aux, outputdataset);

  }

  bool simpl_out = m_ui->m_simploutCheckBox->isChecked();
  std::string attr = m_ui->m_ZcomboBox->currentText().toStdString();

  int srid = m_inputLayer->getSRID();
  iso->setSRID(srid);
  if (srid)
  {
    te::common::UnitOfMeasurePtr unitin = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(srid);
    te::common::UnitOfMeasurePtr unitout = te::common::UnitsOfMeasureManager::getInstance().find("metre");

    if (unitin->getId() != te::common::UOM_Metre)
    {
      convertPlanarToAngle(m_factor, unitout);
      convertPlanarToAngle(m_maxdist, unitout);
    }
  }

  iso->setParams(m_factor, m_maxdist, simpl_out, attr);

  bool result = iso->run();

  delete iso;

  if (m_toFile)
  {
    // let's include the new datasource in the managers
    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);

    te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
    ds->setConnInfo(outdsinfo);
    ds->setTitle(uri.stem().string());
    ds->setAccessDriver("OGR");
    ds->setType("OGR");
    ds->setDescription(uri.string());
    ds->setId(id);

    te::da::DataSourcePtr newds = te::da::DataSourceManager::getInstance().get(id, "OGR", ds->getConnInfo());
    newds->open();
    te::da::DataSourceInfoManager::getInstance().add(ds);
    m_outputDatasource = ds;
  }

  // creating a layer for the result
  te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());

  te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());

  te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
  m_outputLayer = converter(dt);

  this->setCursor(Qt::ArrowCursor);
  accept();
}

void te::mnt::SmoothIsolinesDialog::onCancelPushButtonClicked()
{
  reject();
}

