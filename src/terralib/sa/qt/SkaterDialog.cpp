/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sa/qt/SkaterDialog.cpp

  \brief A dialog for Spatial 'K'luster Analysis by Tree Edge Removal.
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/DataSetLayer.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../core/GPMBuilder.h"
#include "../core/GPMConstructorAdjacencyStrategy.h"
#include "../core/GPMWeightsNoWeightsStrategy.h"
#include "../core/SkaterOperation.h"
#include "../core/SkaterParams.h"
#include "../core/SpatialWeightsExchanger.h"
#include "../core/Utils.h"
#include "../Exception.h"
#include "SkaterDialog.h"
#include "Utils.h"
#include "ui_SkaterDialogForm.h"

// Qt
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QMessageBox>
#include <QValidator>

// STL
#include <memory>

// Boost
#include <boost/filesystem.hpp>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::sa::SkaterDialog::SkaterDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::SkaterDialogForm)
{
// add controls
  m_ui->setupUi(this);

// add double list widget
  m_doubleListWidget = new te::qt::widgets::DoubleListWidget(m_ui->m_widget);
  m_doubleListWidget->setLeftLabel("Layer Attributes");
  m_doubleListWidget->setRightLabel("Selected Attributes");
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->addWidget(m_doubleListWidget);
  layout->setContentsMargins(0, 0, 0, 0);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("sa-skater-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

// connectors
  connect(m_ui->m_inputLayerComboBox, SIGNAL(activated(int)), this, SLOT(onInputLayerComboBoxActivated(int)));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));
  connect(m_ui->m_gpmToolButton, SIGNAL(clicked()), this, SLOT(onGPMToolButtonClicked()));

// help info
  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/sa/sa_skater.html");
}

te::sa::SkaterDialog::~SkaterDialog()
{
}

void te::sa::SkaterDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while(it != layers.end())
  {
    te::map::AbstractLayerPtr l = *it;

    if(l->isValid())
    {
      std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();
      te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

      if(dsLayer && dsType->hasGeom())
        m_ui->m_inputLayerComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));
    }

    ++it;
  }

// fill attributes combo
  if(m_ui->m_inputLayerComboBox->count() > 0)
    onInputLayerComboBoxActivated(0);
}

te::map::AbstractLayerPtr te::sa::SkaterDialog::getOutputLayer()
{
  return m_outputLayer;
}

void te::sa::SkaterDialog::onInputLayerComboBoxActivated(int index)
{
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(index, Qt::UserRole);
  
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

  std::vector<te::dt::Property*> propVec = dsType->getProperties();

  m_ui->m_popComboBox->clear();
  m_ui->m_attrLinkComboBox->clear();

  m_doubleListWidget->clearInputValues();
  m_doubleListWidget->clearOutputValues();

  std::vector<std::string> vec;

  for(std::size_t t = 0; t < propVec.size(); ++t)
  {
    int dataType = propVec[t]->getType();

    if (dataType == te::dt::INT16_TYPE || dataType == te::dt::UINT16_TYPE ||
        dataType == te::dt::INT32_TYPE || dataType == te::dt::UINT32_TYPE ||
        dataType == te::dt::INT64_TYPE || dataType == te::dt::UINT64_TYPE ||
        dataType == te::dt::FLOAT_TYPE || dataType == te::dt::DOUBLE_TYPE)
    {
      m_ui->m_popComboBox->addItem(propVec[t]->getName().c_str(), dataType);

      vec.push_back(propVec[t]->getName());
    }

    m_ui->m_attrLinkComboBox->addItem(propVec[t]->getName().c_str(), dataType);
  }

  std::sort(vec.begin(), vec.end());

  m_doubleListWidget->setInputValues(vec);
}

void te::sa::SkaterDialog::onOkPushButtonClicked()
{
  // check input parameters
  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Warning"), tr("Define a repository for the result."));
    return;
  }
       
  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Warning"), tr("Define a name for the resulting layer."));
    return;
  }

  if(!m_ui->m_clustersCheckBox->isChecked() && !m_ui->m_popCheckBox->isChecked())
  {
    QMessageBox::information(this, tr("Warning"), tr("Select the aggregation type."));
    return;
  }

  if(m_ui->m_clustersCheckBox->isChecked())
  {
    if(m_ui->m_nClustersLineEdit->text().isEmpty())
    {
      QMessageBox::information(this, tr("Warning"), tr("Number of clusters not defined."));
      return;
    }
  }
  
  if(m_ui->m_popCheckBox->isChecked())
  {
    if(m_ui->m_minPopLineEdit->text().isEmpty())
    {
      QMessageBox::information(this, tr("Warning"), tr("Minimum population not defined."));
      return;
    }
  }

  std::vector<std::string> attrs = m_doubleListWidget->getOutputValues();

  if(attrs.empty())
  {
    QMessageBox::information(this, tr("Warning"), tr("Select the attributes."));
    return;
  }

  //get GPM
  std::auto_ptr<te::sa::GeneralizedProximityMatrix> gpm;

  try
  {
    gpm = loadGPM();
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Internal error. GPM not loaded."));
    return;
  }

  if(!gpm.get())
    return;

//get selected layer
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(m_ui->m_inputLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  std::auto_ptr<te::da::DataSetType> dataSetType = l->getSchema();
  std::auto_ptr<te::da::DataSet> dataSet = l->getData();

//create datasource to save the output information
  std::string dataSetName = m_ui->m_newLayerNameLineEdit->text().toStdString();

  std::size_t idx = dataSetName.find(".");
  if (idx != std::string::npos)
        dataSetName=dataSetName.substr(0,idx);

  te::da::DataSourcePtr outputDataSource;

  if(m_toFile)
  {
    outputDataSource = te::sa::CreateOGRDataSource(m_ui->m_repositoryLineEdit->text().toStdString());
  }
  else
  {
    outputDataSource = te::da::GetDataSource(m_outputDatasource->getId());
  }

  //run skater
  te::sa::SkaterInputParams* inParams = new te::sa::SkaterInputParams();

  inParams->m_ds = dataSet;
  inParams->m_dsType = dataSetType;
  inParams->m_gpm = gpm;
  inParams->m_gpmAttrLink = m_ui->m_attrLinkComboBox->currentText().toStdString();

  if(m_ui->m_clustersCheckBox->isChecked() && m_ui->m_popCheckBox->isChecked())
  {
    inParams->m_aggregType = te::sa::Both;
    inParams->m_nClusters = (std::size_t)m_ui->m_nClustersLineEdit->text().toInt();
    inParams->m_minPop = (std::size_t)m_ui->m_minPopLineEdit->text().toInt();
    inParams->m_attrPop = m_ui->m_popComboBox->currentText().toStdString();
  }
  else if(m_ui->m_clustersCheckBox->isChecked())
  {
    inParams->m_aggregType = te::sa::Clusters;
    inParams->m_nClusters = (std::size_t)m_ui->m_nClustersLineEdit->text().toInt();
  }
  else if(m_ui->m_popCheckBox->isChecked())
  {
    inParams->m_aggregType = te::sa::Population;
    inParams->m_minPop = (std::size_t)m_ui->m_minPopLineEdit->text().toInt();
    inParams->m_attrPop = m_ui->m_popComboBox->currentText().toStdString();
  }

  inParams->m_attrs = attrs;

  te::sa::SkaterOutputParams* outParams = new te::sa::SkaterOutputParams();

  outParams->m_dataSource = outputDataSource;
  outParams->m_outputDataSetName = dataSetName;

  int nClasses;

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
    te::sa::SkaterOperation op;

    op.setParameters(inParams, outParams);

    op.execute();

    nClasses = op.getNumberOfClasses();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Warning"), e.what());

    QApplication::restoreOverrideCursor();

    te::common::ProgressManager::getInstance().removeViewer(id);

    return;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Internal Error. Skater not calculated."));

    QApplication::restoreOverrideCursor();

    te::common::ProgressManager::getInstance().removeViewer(id);

    return;
  }

  QApplication::restoreOverrideCursor();

  te::common::ProgressManager::getInstance().removeViewer(id);

  //create layer
  m_outputLayer = te::sa::CreateLayer(outputDataSource, dataSetName);

  //create legend
  te::sa::CreateSkaterGrouping(m_outputLayer, nClasses);

  accept();
}

void te::sa::SkaterDialog::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);

  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.size() <= 0)
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;
  
  m_toFile = false;
}

void te::sa::SkaterDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();
  
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("Shapefile (*.shp *.SHP);;"),0, QFileDialog::DontConfirmOverwrite);
  
  if (fileName.isEmpty())
    return;
  
  boost::filesystem::path outfile(fileName.toStdString());

  m_ui->m_repositoryLineEdit->setText(outfile.string().c_str());

  m_ui->m_newLayerNameLineEdit->setText(outfile.leaf().string().c_str());

  m_ui->m_newLayerNameLineEdit->setEnabled(false);
  
  m_toFile = true;
}

void te::sa::SkaterDialog::onGPMToolButtonClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Generalized Proximity Matrix File"), "", tr("GAL Files (*.gal *.GAL);; GWT Files (*.gwt *.GWT)"));

  if(fileName.isEmpty())
    return;

  //get selected layer
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(m_ui->m_inputLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

  //check if the selected gpm is valid for selected layer
  std::string path = fileName.toStdString();
  std::string dataSetName = "";
  std::string attrName = "";

  te::sa::SpatialWeightsExchanger::getSpatialWeightsFileInfo(path, dataSetName, attrName);

  if(dsLayer->getDataSetName() != dataSetName)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Invalid GPM file for selected layer."));
    return;
  }

  if(m_ui->m_attrLinkComboBox->currentText().toStdString() != attrName)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Invalid GPM file for selected Attr Link."));
    return;
  }

  m_ui->m_gpmLineEdit->setText(fileName);
}

std::auto_ptr<te::sa::GeneralizedProximityMatrix> te::sa::SkaterDialog::loadGPM()
{
  std::auto_ptr<te::sa::GeneralizedProximityMatrix> gpm;

  //get selected layer
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(m_ui->m_inputLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  if(!l.get())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Invalid selected layer."));
    return gpm;
  }

  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

  //get gpm necessary parameters
  te::da::DataSourcePtr ds = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

  std::string dataSetName = dsLayer->getDataSetName();

  if(!m_ui->m_gpmGroupBox->isChecked())
  {
    //create gpm
    if(QMessageBox::question(this, tr("Spatial Analysis"), tr("GPM not selected. Create default GPM?"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
      return gpm;

    //get attrlink
    std::auto_ptr<te::da::DataSetType> dsType = dsLayer->getSchema();
    std::string attrLink = dsType->getPrimaryKey()->getName();

    //create default gpm
    te::sa::GPMConstructorAbstractStrategy* constructor = new te::sa::GPMConstructorAdjacencyStrategy(true);
    te::sa::GPMWeightsAbstractStrategy* weights = new te::sa::GPMWeightsNoWeightsStrategy(true);

    te::sa::GPMBuilder builder(constructor, weights);
    
    builder.setGPMInfo(ds, dataSetName, attrLink);

    gpm = builder.build();
  }
  else
  {
    if(m_ui->m_gpmLineEdit->text().isEmpty())
    {
      QMessageBox::warning(this, tr("Warning"), tr("GPM File not selected."));
      return gpm;
    }

    //load gpm
    QFileInfo file(m_ui->m_gpmLineEdit->text());

    std::string extension = file.suffix().toStdString();

    te::sa::SpatialWeightsExchanger swe;

    if(extension == "gal" || extension == "GAL")
    {
      gpm.reset(swe.importFromGAL(m_ui->m_gpmLineEdit->text().toStdString(), ds.get()));
    }
    else if(extension == "gwt" || extension == "GWT")
    {
      gpm.reset(swe.importFromGWT(m_ui->m_gpmLineEdit->text().toStdString(), ds.get()));
    }
  }

  return gpm;
}
