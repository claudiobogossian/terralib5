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
\file terralib/mnt/qt/MNTGenerationDialog.cpp

\brief A dialog for Retangular Grid generation
*/

//terralib
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/DataSetLayer.h"
#include "../../mnt/core/TINCalculateGrid.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/rp/Utils.h"

#include "MNTGenerationDialog.h"
#include "ui_MNTGenerationDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::mnt::MNTGenerationDialog::MNTGenerationDialog(QWidget* parent , Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::MNTGenerationDialogForm),
  m_layers(std::list<te::map::AbstractLayerPtr>())
{
  // add controls
  m_ui->setupUi(this);

  //signals
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));

  m_ui->m_dimLLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_dimCLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_resXLineEdit->setValidator(new QDoubleValidator(0, 100, 4, this));
  m_ui->m_resYLineEdit->setValidator(new QDoubleValidator(0, 100, 4, this));

  connect(m_ui->m_resXLineEdit, SIGNAL(editingFinished()), this, SLOT(onResXLineEditEditingFinished()));
  connect(m_ui->m_resYLineEdit, SIGNAL(editingFinished()), this, SLOT(onResYLineEditEditingFinished()));
  connect(m_ui->m_dimCLineEdit, SIGNAL(editingFinished()), this, SLOT(onDimCLineEditEditingFinished()));
  connect(m_ui->m_dimLLineEdit, SIGNAL(editingFinished()), this, SLOT(onDimLLineEditEditingFinished()));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::mnt::MNTGenerationDialog::~MNTGenerationDialog()
{
}

void te::mnt::MNTGenerationDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while (it != m_layers.end())
  {
    m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

te::map::AbstractLayerPtr te::mnt::MNTGenerationDialog::getLayer()
{
  return m_outputLayer;
}

void te::mnt::MNTGenerationDialog::onInputComboBoxChanged(int index)
{
  m_ui->m_interpolatorComboBox->clear();
  m_inputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_inputLayer = it->get();
      std::auto_ptr<te::da::DataSetType> dsType = m_inputLayer->getSchema();
      if (dsType->hasGeom())
      {
        std::auto_ptr<te::gm::GeometryProperty>geomProp(te::da::GetFirstGeomProperty(dsType.get()));

        te::gm::GeomType gmType = geomProp->getGeometryType();
        if (gmType == te::gm::PointType || gmType == te::gm::MultiPointType ||
          gmType == te::gm::PointZType || gmType == te::gm::MultiPointZType ||
          gmType == te::gm::LineStringType || gmType == te::gm::MultiLineStringType ||
          gmType == te::gm::LineStringZType || gmType == te::gm::MultiLineStringZType) //samples
        {
          m_inputType = SAMPLE;
          m_ui->m_interpolatorComboBox->addItem("Weighted Avg./Z Value/Quadrant");
          m_ui->m_interpolatorComboBox->addItem("Weighted Average/Quadrant");
          m_ui->m_interpolatorComboBox->addItem("Weighted Average");
          m_ui->m_interpolatorComboBox->addItem("Simple Average");
          m_ui->m_interpolatorComboBox->addItem("Nearest Neighbor");
          m_ui->m_interpolatorComboBox->addItem("Bilinear Spline");
          m_ui->m_interpolatorComboBox->addItem("Bicubic Spline;");
          m_ui->m_interpolatorComboBox->addItem("Mitasova Spline");
        }
        if (gmType == te::gm::TINType || gmType == te::gm::MultiPolygonType || gmType == te::gm::PolyhedralSurfaceType ||
          gmType == te::gm::TINZType || gmType == te::gm::MultiPolygonZType || gmType == te::gm::PolyhedralSurfaceZType)//TIN
        {
          m_inputType = TIN;
          m_ui->m_interpolatorComboBox->addItem("Linear");
          m_ui->m_interpolatorComboBox->addItem("Quintic without breaklines");
          m_ui->m_interpolatorComboBox->addItem("Quintic with breaklines");
        }
      }
      if (dsType->hasRaster()) //GRID
      {
        m_inputType = GRID;
        m_ui->m_interpolatorComboBox->addItem("Bilinear");
        m_ui->m_interpolatorComboBox->addItem("Bicubic");

      }
      dsType.release();
      break;
    }
    it++;
  }
}

void te::mnt::MNTGenerationDialog::onResXLineEditEditingFinished()
{
  double resX = m_ui->m_resXLineEdit->text().toDouble();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("DTM Generation"), tr("Invalid envelope!"));
    return;
  }

  int maxCols = (int)ceil((env.m_urx - env.m_llx) / resX);

  m_ui->m_dimCLineEdit->setText(QString::number(maxCols));
}

void te::mnt::MNTGenerationDialog::onResYLineEditEditingFinished()
{
  double resY = m_ui->m_resYLineEdit->text().toDouble();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("DTM Generation"), tr("Invalid envelope!"));
    return;
  }

  int maxRows = (int)ceil((env.m_ury - env.m_lly) / resY);

  m_ui->m_dimLLineEdit->setText(QString::number(maxRows));
}

void te::mnt::MNTGenerationDialog::onDimLLineEditEditingFinished()
{
  int cols = m_ui->m_dimCLineEdit->text().toInt();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("DTM Generation"), tr("Invalid envelope!"));
    return;
  }

  double resX = (env.m_urx - env.m_llx) / cols;

  m_ui->m_resXLineEdit->setText(QString::number(resX));
}

void te::mnt::MNTGenerationDialog::onDimCLineEditEditingFinished()
{
  int rows = m_ui->m_dimLLineEdit->text().toInt();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("DTM Generation"), tr("Invalid envelope!"));
    return;
  }

  double resY = (env.m_ury - env.m_lly) / rows;

  m_ui->m_resYLineEdit->setText(QString::number(resY));
}

void te::mnt::MNTGenerationDialog::onTargetDatasourceToolButtonPressed()
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

void te::mnt::MNTGenerationDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr("TIFF (*.tif *.TIF);;"), 0, QFileDialog::DontConfirmOverwrite);

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

void te::mnt::MNTGenerationDialog::onHelpPushButtonClicked()
{}

void te::mnt::MNTGenerationDialog::onOkPushButtonClicked()
{
  int srid;

  if (!m_inputLayer.get())
  {
    QMessageBox::information(this, tr("DTM Generation"), tr("Select an input layer."));
    return;
  }

  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
  if (!dsLayer)
  {
    QMessageBox::information(this, tr("DTM Generation"), tr("Can not execute this operation on this type of layer."));
    return;
  }

  te::da::DataSourcePtr inDataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
  if (!inDataSource.get())
  {
    QMessageBox::information(this, tr("DTM Generation"), tr("The selected input data source can not be accessed."));
    return;
  }

  std::map<std::string, std::string> dsinfo;

  try
  {
    std::string inDsetName = dsLayer->getDataSetName();
    srid = dsLayer->getSRID();

    std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();
    if (outputdataset.empty())
    {
      QMessageBox::information(this, tr("DTM Generation"), tr("The selected output datasource is empty."));
      return;
    }

    boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

    if (boost::filesystem::exists(uri))
    {
      QMessageBox::information(this, tr("DTM Generation"), tr("Output file already exists. Remove it or select a new name and try again."));
      return;
    }

    std::size_t idx = outputdataset.find(".");
    if (idx != std::string::npos)
      outputdataset = outputdataset.substr(0, idx);

    dsinfo["URI"] = uri.string();

    te::mnt::TINCalculateGrid *grid = new te::mnt::TINCalculateGrid();

    switch (m_inputType)
    {
      case SAMPLE:
        break;
      case TIN:
      {
        int inter_i = m_ui->m_interpolatorComboBox->currentIndex();
        switch (inter_i)
        {
        case 0: //Linear
          m_inter = Linear;
          break;
        case 1: //Quintico
          m_inter = Quintico;
            break;
        case 2: //Quintico breakline
          m_inter = QuinticoBrkLine;
        }

        grid->setInput(inDataSource, inDsetName, inDataSource->getDataSetType(inDsetName));
        grid->setOutput(dsinfo);
        grid->setParams(m_ui->m_resXLineEdit->text().toDouble(), m_ui->m_resYLineEdit->text().toDouble(), m_inter);
        grid->setSRID(srid);

        grid->run();

        delete grid;
        break;
      }
      case GRID:
        break;
      default:
        break;
    }

  }
  catch (const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);
    QMessageBox::information(this, "DTM Generation", e.what());
    return;
  }

  m_outputLayer = te::qt::widgets::createLayer("GDAL", dsinfo);

  this->setCursor(Qt::ArrowCursor);
  accept();

}

void te::mnt::MNTGenerationDialog::onCancelPushButtonClicked()
{
  reject();
}

