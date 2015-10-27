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
#include "../../mnt/core/Slope.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/rp/Utils.h"
#include "../../raster.h"
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
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
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
    ++it;
  }
}

void te::mnt::SmoothIsolinesDialog::onInputComboBoxChanged(int index)
{
  m_inputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      break;
    }
    ++it;
  }
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
    QMessageBox::information(this, tr("Smooth"), tr("Select an input layer!"));
    return;
  }

  te::map::DataSetLayer* indsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
  if (!indsLayer)
  {
    QMessageBox::information(this, tr("Smooth"), tr("Can not execute this operation on this type of layer!"));
    return;
  }

  te::da::DataSourcePtr inDataSource = te::da::GetDataSource(indsLayer->getDataSourceId(), true);
  if (!inDataSource.get())
  {
    QMessageBox::information(this, tr("Smooth"), tr("The selected input data source can not be accessed!"));
    return;
  }

  std::string inDsetName = indsLayer->getDataSetName();
  std::auto_ptr<te::da::DataSetType> inDsetType(inDataSource->getDataSetType(inDsetName));

  // Checking consistency of output paramenters
  if (m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Slope"), tr("Select a repository for the resulting layer."));
    return;
  }

  if (m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Smooth"), tr("Define a name for the resulting layer."));
    return;
  }
  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();

  std::map<std::string, std::string> outdsinfo;
  boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

  if (m_toFile)
  {
    if (boost::filesystem::exists(uri))
    {
      QMessageBox::information(this, tr("Smooth"), tr("Output file already exists! Remove it or select a new name and try again."));
      return;
    }

    std::size_t idx = outputdataset.find(".");
    if (idx != std::string::npos)
      outputdataset = outputdataset.substr(0, idx);

    outdsinfo["URI"] = uri.string();
  }

  this->setCursor(Qt::WaitCursor);


  m_outputLayer = te::qt::widgets::createLayer("OGR", outdsinfo);

  this->setCursor(Qt::ArrowCursor);
  accept();

}

void te::mnt::SmoothIsolinesDialog::onCancelPushButtonClicked()
{
  reject();
}

