/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

This file is part of the TerraLib - a Framework for building GIS enabled applications.

TerraLib is free software: you can redistribute it and/or 

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
#include "../../core/filesystem/FileSystem.h"
#include "../../core/translator/Translator.h"
#include "../../common/progress/ProgressManager.h"
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
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/rp/Utils.h"
#include "../../qt/widgets/srs/SRSManagerDialog.h"
#include "../../raster.h"
#include "../../srs/SpatialReferenceSystemManager.h"

#include "LayerSearchDialog.h"
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
  connect(m_ui->m_layerSearchToolButton, SIGNAL(clicked()), this, SLOT(onInputLayerToolButtonClicked()));
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));

  connect(m_ui->m_scalePushButton, SIGNAL(clicked()), this, SLOT(onScalePushButtonClicked()));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  m_ui->m_srsToolButton->setIcon(QIcon::fromTheme("srs"));
  connect(m_ui->m_srsToolButton, SIGNAL(clicked()), this, SLOT(onSrsToolButtonClicked()));

  m_outsrid = 0;
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
        mntType type = getMNTType(dsType.get());

        if (type == ISOLINE)
          m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));

        dsType.release();
      }
    }
    ++it;
  }
}

void te::mnt::SmoothIsolinesDialog::onInputLayerToolButtonClicked()
{
  LayerSearchDialog search(this->parentWidget());
  search.setLayers(m_layers);
  QList<mntType> types;
  types.append(ISOLINE);
  search.setActive(types);

  if (search.exec() != QDialog::Accepted)
  {
    return;
  }

  m_ui->m_layersComboBox->setCurrentText(search.getLayer().get()->getTitle().c_str());

}

void te::mnt::SmoothIsolinesDialog::onInputComboBoxChanged(int index)
{
  m_inputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toUtf8().data();

  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_inputLayer = selectedLayer;
      
      setSRID(m_inputLayer->getSRID());

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
    QString(), tr("Shapefile (*.shp *.SHP);;"), 0);

  if (fileName.isEmpty())
    return;

  try
  {
    te::core::FileSystem::remove(fileName.toUtf8().data());
  }
  catch (const std::exception& e)
  {
    QMessageBox::information(this, tr("Smooth Isolines"), e.what());
    return;
  }

  boost::filesystem::path outfile(fileName.toUtf8().data());
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
    std::string outdsinfo ("File://");
    boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toUtf8().data());

    if (m_toFile)
    {
      if (te::core::FileSystem::exists(uri.string()))
        throw te::common::Exception(TE_TR("Output file already exists! Remove it or select a new name and try again."));

      std::size_t idx = outputdataset.find(".");
      if (idx != std::string::npos)
        outputdataset = outputdataset.substr(0, idx);

      outdsinfo += uri.string();
    }

    Smooth *iso = new te::mnt::Smooth();
    iso->setInput(inDataSource, inDsetName, inDsetType);
    if (m_toFile)
    {
      te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR", outdsinfo).release());
      dsOGR->open();

      if (dsOGR->dataSetExists(outputdataset))
        throw te::common::Exception(TE_TR("There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again."));

      iso->setOutput(dsOGR, outputdataset);
    }
    else
    {
      te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
      if (!aux)
        throw te::common::Exception(TE_TR("The selected output datasource can not be accessed."));

      if (aux->dataSetExists(outputdataset))
        throw te::common::Exception(TE_TR("Dataset already exists. Remove it or select a new name and try again. "));

      iso->setOutput(aux, outputdataset);
    }

    bool simpl_out = m_ui->m_simploutCheckBox->isChecked();

    iso->setSRID(m_outsrid);
    if (m_outsrid)
    {
      te::common::UnitOfMeasurePtr unitin = te::srs::SpatialReferenceSystemManager::getInstance().getUnit((unsigned)m_outsrid);
      if (unitin.get())
      {
        te::common::UnitOfMeasurePtr unitout = te::common::UnitsOfMeasureManager::getInstance().find("metre");

        if (unitin->getId() != te::common::UOM_Metre)
        {
          convertPlanarToAngle(m_factor, unitout);
          convertPlanarToAngle(m_maxdist, unitout);
        }
      }
    }

    iso->setParams(m_factor, m_maxdist, simpl_out);

    bool result = iso->run();

    delete iso;

    if (result)
    {
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
    }
  }
  catch (const std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    te::common::ProgressManager::getInstance().removeViewer(id);
    QMessageBox::information(this, tr("Smooth Isolines "), e.what());
    return;
  }
  
  QApplication::restoreOverrideCursor();
  te::common::ProgressManager::getInstance().removeViewer(id);
  accept();

}

void te::mnt::SmoothIsolinesDialog::onCancelPushButtonClicked()
{
  reject();
}


void te::mnt::SmoothIsolinesDialog::onSrsToolButtonClicked()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if (srsDialog.exec() == QDialog::Rejected)
    return;

  int newSRID = srsDialog.getSelectedSRS().first;

  setSRID(newSRID);

}

void te::mnt::SmoothIsolinesDialog::setSRID(int newSRID)
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
