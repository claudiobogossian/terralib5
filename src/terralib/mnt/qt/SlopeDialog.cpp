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
#include "../../core/filesystem/FileSystem.h"
#include "../../core/translator/Translator.h"
#include "../../common/progress/ProgressManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/DataSetLayer.h"
#include "../../mnt/core/Slope.h"
#include "../../mnt/core/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/rp/Utils.h"
#include "../../qt/widgets/srs/SRSManagerDialog.h"
#include "../../raster.h"
#include "../../srs/SpatialReferenceSystemManager.h"

#include "LayerSearchDialog.h"
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
  connect(m_ui->m_layerSearchToolButton, SIGNAL(clicked()), this, SLOT(onInputLayerToolButtonClicked()));
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));

  connect(m_ui->m_dummycheckBox, SIGNAL(toggled(bool)), m_ui->m_dummylineEdit, SLOT(setEnabled(bool)));

  connect(m_ui->m_sloperadioButton, SIGNAL(toggled(bool)), this, SLOT(onSlopeToggled()));
  connect(m_ui->m_aspectradioButton, SIGNAL(toggled(bool)), this, SLOT(onAspectToggled()));

  connect(m_ui->m_resXLineEdit, SIGNAL(editingFinished()), this, SLOT(onResXLineEditEditingFinished()));
  connect(m_ui->m_resYLineEdit, SIGNAL(editingFinished()), this, SLOT(onResYLineEditEditingFinished()));
  connect(m_ui->m_dimCLineEdit, SIGNAL(editingFinished()), this, SLOT(onDimCLineEditEditingFinished()));
  connect(m_ui->m_dimLLineEdit, SIGNAL(editingFinished()), this, SLOT(onDimLLineEditEditingFinished()));

  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  m_ui->m_srsToolButton->setIcon(QIcon::fromTheme("srs"));
  connect(m_ui->m_srsToolButton, SIGNAL(clicked()), this, SLOT(onSrsToolButtonClicked()));

  m_outsrid = 0;

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
          std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
          mntType type = getMNTType(dsType.get());

          if (type == TIN || type == GRID)
            m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));

          dsType.release();
        }
      }
    }
    ++it;
  }
}

void te::mnt::SlopeDialog::onInputLayerToolButtonClicked()
{
  LayerSearchDialog search(this->parentWidget());
  search.setLayers(m_layers);
  QList<mntType> types;
  types << TIN << GRID;
  search.setActive(types);

  if (search.exec() != QDialog::Accepted)
  {
    return;
  }

  m_ui->m_layersComboBox->setCurrentText(search.getLayer().get()->getTitle().c_str());

}

void te::mnt::SlopeDialog::onInputComboBoxChanged(int index)
{
  m_inputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toUtf8().data();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_inputLayer = it->get();

      setSRID(m_inputLayer->getSRID());

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

void te::mnt::SlopeDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr("TIFF (*.tif *.TIF);;"), 0);

  if (fileName.isEmpty())
    return;


  try
  {
    te::core::FileSystem::remove(fileName.toUtf8().data());
  }
  catch (const std::exception& e)
  {
    QMessageBox::information(this, tr("Slope"), e.what());
    return;
  }

  boost::filesystem::path outfile(fileName.toUtf8().data());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());

  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

void te::mnt::SlopeDialog::onHelpPushButtonClicked()
{
}

void te::mnt::SlopeDialog::onOkPushButtonClicked()
{

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  try
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (!m_inputLayer.get())
      throw te::common::Exception(TE_TR("Select an input layer!"));

    te::map::DataSetLayer* indsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
    if (!indsLayer)
      throw te::common::Exception(TE_TR("Can not execute this operation on this type of layer!"));

    te::da::DataSourcePtr inDataSource = te::da::GetDataSource(indsLayer->getDataSourceId(), true);
    if (!inDataSource.get())
      throw te::common::Exception(TE_TR("The selected input data source can not be accessed!"));

    std::string inDsetName = indsLayer->getDataSetName();
    std::auto_ptr<te::da::DataSetType> inDsetType(inDataSource->getDataSetType(inDsetName));

    // Checking consistency of output paramenters
    if (m_ui->m_repositoryLineEdit->text().isEmpty())
      throw te::common::Exception(TE_TR("Select a repository for the resulting layer."));

    if (m_ui->m_newLayerNameLineEdit->text().isEmpty())
      throw te::common::Exception(TE_TR("Define a name for the resulting layer."));

    std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toUtf8().data();

    std::map<std::string, std::string> outdsinfo;
    boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toUtf8().data());

    if (te::core::FileSystem::exists(uri.string()))
      throw te::common::Exception(TE_TR("Output file already exists! Remove it or select a new name and try again."));

    std::size_t idx = outputdataset.find(".");
    if (idx != std::string::npos)
      outputdataset = outputdataset.substr(0, idx);

    outdsinfo["URI"] = uri.string();

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

    decl->setParams(m_ui->m_resXLineEdit->text().toDouble(), m_ui->m_resYLineEdit->text().toDouble(), grad, slope, m_outsrid, dummy);

    decl->run();

    delete decl;

    m_outputLayer = te::qt::widgets::createLayer("GDAL", outdsinfo);
  }
  catch (te::common::Exception& e)
  {
    QApplication::restoreOverrideCursor();
    te::common::ProgressManager::getInstance().removeViewer(id);
    QMessageBox::information(this, tr("Slope "), e.what());
    return;
  }

  QApplication::restoreOverrideCursor();
  te::common::ProgressManager::getInstance().removeViewer(id);
  accept();

}

void te::mnt::SlopeDialog::onCancelPushButtonClicked()
{
  reject();
}


void te::mnt::SlopeDialog::onSrsToolButtonClicked()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if (srsDialog.exec() == QDialog::Rejected)
    return;

  int newSRID = srsDialog.getSelectedSRS().first;

  setSRID(newSRID);

}

void te::mnt::SlopeDialog::setSRID(int newSRID)
{
  if (newSRID <= 0)
  {
    m_ui->m_resSRIDLabel->setText("No SRS defined");
  }
  else
  {
    std::string name = te::srs::SpatialReferenceSystemManager::getInstance().getName(newSRID);
    if (name.size())
      m_ui->m_resSRIDLabel->setText(name.c_str());
    else
      m_ui->m_resSRIDLabel->setText(QString("%1").arg(newSRID));
  }
  m_outsrid = newSRID;

}
