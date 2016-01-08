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
\file terralib/mnt/qt/SlopenDialog.cpp

\brief A dialog for Slope Grid generation
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
#include "SlopeDialog.h"
#include "ui_SlopeDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::mnt::SlopeDialog::SlopeDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::SlopeDialogForm),
  m_layers(std::list<te::map::AbstractLayerPtr>())
{
  // add controls
  m_ui->setupUi(this);

  m_ui->m_dimLLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_dimCLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_resXLineEdit->setValidator(new QDoubleValidator(0, 100, 4, this));
  m_ui->m_resYLineEdit->setValidator(new QDoubleValidator(0, 100, 4, this));

  //signals
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));

  connect(m_ui->m_dummycheckBox, SIGNAL(toggled(bool)), m_ui->m_dummylineEdit, SLOT(setEnabled(bool)));

  connect(m_ui->m_sloperadioButton, SIGNAL(toggled(bool)), this, SLOT(onSlopeToggled()));
  connect(m_ui->m_aspectradioButton, SIGNAL(toggled(bool)), this, SLOT(onAspectToggled()));

  connect(m_ui->m_resXLineEdit, SIGNAL(editingFinished()), this, SLOT(onResXLineEditEditingFinished()));
  connect(m_ui->m_resYLineEdit, SIGNAL(editingFinished()), this, SLOT(onResYLineEditEditingFinished()));
  connect(m_ui->m_dimCLineEdit, SIGNAL(editingFinished()), this, SLOT(onDimCLineEditEditingFinished()));
  connect(m_ui->m_dimLLineEdit, SIGNAL(editingFinished()), this, SLOT(onDimLLineEditEditingFinished()));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::mnt::SlopeDialog::~SlopeDialog()
{

}

void te::mnt::SlopeDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
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
            if (gmType == te::gm::TINType || gmType == te::gm::MultiPolygonType || gmType == te::gm::PolyhedralSurfaceType ||
              gmType == te::gm::TINZType || gmType == te::gm::MultiPolygonZType || gmType == te::gm::PolyhedralSurfaceZType)//TIN
            {
              m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
            }
          }
          if (dsType->hasRaster()) //GRID
          {
            m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
          }
          dsType.release();
        }
      }
    }
    ++it;
  }
}

void te::mnt::SlopeDialog::onInputComboBoxChanged(int index)
{
  m_inputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_inputLayer = it->get();
      std::auto_ptr<te::da::DataSetType> dsType = m_inputLayer->getSchema();
      if (dsType->hasRaster()) //GRID
      {
        te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(dsType.get());
        te::map::DataSetLayer* indsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
        te::da::DataSourcePtr inDataSource = te::da::GetDataSource(indsLayer->getDataSourceId(), true);
        std::auto_ptr<te::da::DataSet> dsRaster = inDataSource->getDataSet(indsLayer->getDataSetName());
        std::auto_ptr<te::rst::Raster> in_raster = dsRaster->getRaster(rasterProp->getName());
        m_ui->m_resXLineEdit->setText(QString::number(in_raster->getResolutionX()));
        m_ui->m_resYLineEdit->setText(QString::number(in_raster->getResolutionY()));
        m_ui->m_dimCLineEdit->setText(QString::number(in_raster->getNumberOfColumns()));
        m_ui->m_dimLLineEdit->setText(QString::number(in_raster->getNumberOfRows()));
        m_ui->m_resXLineEdit->setEnabled(false);
        m_ui->m_resYLineEdit->setEnabled(false);
        m_ui->m_dimCLineEdit->setEnabled(false);
        m_ui->m_dimLLineEdit->setEnabled(false);
        m_ui->m_dummycheckBox->setVisible(true);
        m_ui->m_dummylineEdit->setVisible(true);
        m_ui->m_dummylineEdit->setText(QString::number(in_raster->getBand(0)->getProperty()->m_noDataValue));
        in_raster.release();
        dsRaster.release();
        m_inputType = GRID;
      }
      else
      {
        m_ui->m_dummycheckBox->setVisible(false);
        m_ui->m_dummylineEdit->setVisible(false);
        m_ui->m_resXLineEdit->setEnabled(true);
        m_ui->m_resYLineEdit->setEnabled(true);
        m_ui->m_dimCLineEdit->setEnabled(true);
        m_ui->m_dimLLineEdit->setEnabled(true);
        m_inputType = TIN;
      }
      break;
    }
    ++it;
  }
}

te::map::AbstractLayerPtr te::mnt::SlopeDialog::getLayer()
{
  return m_outputLayer;
}

void te::mnt::SlopeDialog::onSlopeToggled()
{
  m_ui->m_degreesradioButton->setEnabled(true);
  m_ui->m_percentradioButton->setEnabled(true);
}

void te::mnt::SlopeDialog::onAspectToggled()
{
  m_ui->m_degreesradioButton->setDisabled(true);
  m_ui->m_percentradioButton->setDisabled(true);
}


void te::mnt::SlopeDialog::onResXLineEditEditingFinished()
{
  double resX = m_ui->m_resXLineEdit->text().toDouble();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("Slope"), tr("Invalid envelope!"));
    return;
  }

  int maxCols = (int)ceil((env.m_urx - env.m_llx) / resX);

  m_ui->m_dimCLineEdit->setText(QString::number(maxCols));
}

void te::mnt::SlopeDialog::onResYLineEditEditingFinished()
{
  double resY = m_ui->m_resYLineEdit->text().toDouble();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("Slope"), tr("Invalid envelope!"));
    return;
  }

  int maxRows = (int)ceil((env.m_ury - env.m_lly) / resY);

  m_ui->m_dimLLineEdit->setText(QString::number(maxRows));
}

void te::mnt::SlopeDialog::onDimLLineEditEditingFinished()
{
  int cols = m_ui->m_dimCLineEdit->text().toInt();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("Slope"), tr("Invalid envelope!"));
    return;
  }

  double resX = (env.m_urx - env.m_llx) / cols;

  m_ui->m_resXLineEdit->setText(QString::number(resX));
}

void te::mnt::SlopeDialog::onDimCLineEditEditingFinished()
{
  int rows = m_ui->m_dimLLineEdit->text().toInt();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("Slope"), tr("Invalid envelope!"));
    return;
  }

  double resY = (env.m_ury - env.m_lly) / rows;

  m_ui->m_resYLineEdit->setText(QString::number(resY));
}


void te::mnt::SlopeDialog::onTargetDatasourceToolButtonPressed()
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

void te::mnt::SlopeDialog::onTargetFileToolButtonPressed()
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

void te::mnt::SlopeDialog::onHelpPushButtonClicked()
{
}

void te::mnt::SlopeDialog::onOkPushButtonClicked()
{

  if (!m_inputLayer.get())
  {
    QMessageBox::information(this, tr("Slope"), tr("Select an input layer!"));
    return;
  }

  te::map::DataSetLayer* indsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
  if (!indsLayer)
  {
    QMessageBox::information(this, tr("Slope"), tr("Can not execute this operation on this type of layer!"));
    return;
  }

  te::da::DataSourcePtr inDataSource = te::da::GetDataSource(indsLayer->getDataSourceId(), true);
  if (!inDataSource.get())
  {
    QMessageBox::information(this, tr("Slope"), tr("The selected input data source can not be accessed!"));
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
    QMessageBox::information(this, tr("Slope"), tr("Define a name for the resulting layer."));
    return;
  }
  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();

  std::map<std::string, std::string> outdsinfo;
  boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

  if (m_toFile)
  {
    if (boost::filesystem::exists(uri))
    {
      QMessageBox::information(this, tr("Slope"), tr("Output file already exists! Remove it or select a new name and try again."));
      return;
    }

    std::size_t idx = outputdataset.find(".");
    if (idx != std::string::npos)
      outputdataset = outputdataset.substr(0, idx);

    outdsinfo["URI"] = uri.string();
  }

  this->setCursor(Qt::WaitCursor);

  te::mnt::Slope *decl = new te::mnt::Slope();
  decl->setInput(inDataSource, inDsetName, inDataSource->getDataSetType(inDsetName));
  decl->setOutput(outdsinfo);
  char grad, slope;
  if (m_ui->m_aspectradioButton->isChecked())
    grad = 'a';
  else
    grad = 's';
  if (m_ui->m_degreesradioButton->isChecked())
    slope = 'g';
  else
    slope = 'p';

  double dummy = m_ui->m_dummylineEdit->text().toDouble();

  decl->setParams(m_ui->m_resXLineEdit->text().toDouble(), m_ui->m_resYLineEdit->text().toDouble(), grad, slope, m_inputLayer->getSRID(), dummy);

  decl->run();

  delete decl;

  m_outputLayer = te::qt::widgets::createLayer("GDAL", outdsinfo);

  this->setCursor(Qt::ArrowCursor);
  accept();

}

void te::mnt::SlopeDialog::onCancelPushButtonClicked()
{
  reject();
}

