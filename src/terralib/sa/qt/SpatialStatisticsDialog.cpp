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
  \file terralib/sa/qt/SpatialStatisticsDialog.cpp

  \brief A dialog to calculate spatial statistics from a attribute of a dataset.
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/DataSetLayer.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../core/GPMBuilder.h"
#include "../core/GPMConstructorAdjacencyStrategy.h"
#include "../core/GPMWeightsNoWeightsStrategy.h"
#include "../core/SpatialStatisticsFunctions.h"
#include "../core/SpatialWeightsExchanger.h"
#include "../core/StatisticsFunctions.h"
#include "../core/Utils.h"
#include "../Exception.h"
#include "SpatialStatisticsDialog.h"
#include "ui_SpatialStatisticsDialogForm.h"

// Qt
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QValidator>

// STL
#include <memory>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::sa::SpatialStatisticsDialog::SpatialStatisticsDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::SpatialStatisticsDialogForm)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("sa-spatialstatistics-hint").pixmap(112,48));
  m_ui->m_gpmToolButton->setIcon(QIcon::fromTheme("folder-open"));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

// connectors
  connect(m_ui->m_inputLayerComboBox, SIGNAL(activated(int)), this, SLOT(onInputLayerComboBoxActivated(int)));
  connect(m_ui->m_gpmToolButton, SIGNAL(clicked()), this, SLOT(onGPMToolButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

// help info
  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/sa/sa_spatialstatistics.html");
}

te::sa::SpatialStatisticsDialog::~SpatialStatisticsDialog()
{
}

void te::sa::SpatialStatisticsDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while(it != layers.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

    if(dsLayer && dsType->hasGeom())
      m_ui->m_inputLayerComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }

// fill attributes combo
  if(m_ui->m_inputLayerComboBox->count() > 0)
    onInputLayerComboBoxActivated(0);
}

void te::sa::SpatialStatisticsDialog::onInputLayerComboBoxActivated(int index)
{
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(index, Qt::UserRole);
  
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

  std::vector<te::dt::Property*> propVec = dsType->getProperties();

  m_ui->m_attrIdComboBox->clear();
  m_ui->m_attrLinkComboBox->clear();

  for(std::size_t t = 0; t < propVec.size(); ++t)
  {
    int dataType = propVec[t]->getType();

    if (dataType == te::dt::INT16_TYPE || dataType == te::dt::UINT16_TYPE ||
        dataType == te::dt::INT32_TYPE || dataType == te::dt::UINT32_TYPE ||
        dataType == te::dt::INT64_TYPE || dataType == te::dt::UINT64_TYPE ||
        dataType == te::dt::FLOAT_TYPE || dataType == te::dt::DOUBLE_TYPE)
    {
      m_ui->m_attrIdComboBox->addItem(propVec[t]->getName().c_str(), dataType);
    }

    m_ui->m_attrLinkComboBox->addItem(propVec[t]->getName().c_str(), dataType);
  }
}

void te::sa::SpatialStatisticsDialog::onGPMToolButtonClicked()
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

  m_ui->m_gpmLineEdit->setText(fileName);
}

void te::sa::SpatialStatisticsDialog::onOkPushButtonClicked()
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

  //get selected layer
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(m_ui->m_inputLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());
  
  //get necessary info to calculate statistics
  te::da::DataSourcePtr ds = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

  std::auto_ptr<te::da::DataSetType> dataSetType = l->getSchema();

  std::string attrLink = m_ui->m_attrLinkComboBox->currentText().toStdString();

  std::string attrName = m_ui->m_attrIdComboBox->currentText().toStdString();

  int type = m_ui->m_attrIdComboBox->itemData(m_ui->m_attrIdComboBox->currentIndex()).toInt();

  //associate the selected attribute to the GPM
  int attrIdx = te::sa::AssociateGPMVertexAttribute(gpm.get(), ds.get(), dsLayer->getDataSetName(), attrLink, attrName, type);

  //G Statistics
  if(m_ui->m_localGStatisticsCheckBox->isChecked())
  {
    try
    {
      te::sa::GStatistics(gpm.get(), attrIdx); //this function calculates the g and g* statistics and adds the values as attribute in each vertex from gpm graph.
    }
    catch(...)
    {
      QMessageBox::warning(this, tr("Warning"), tr("Internal error. G Statistics not calculated."));
      return;
    }
  }

  //Local Mean
  if(m_ui->m_localLocalMeanCheckBox->isChecked())
  {
    try
    {
      te::sa::LocalMean(gpm.get(), attrIdx); //this function calculates the local mean statistics and adds the values as attribute in each vertex from gpm graph.
    }
    catch(...)
    {
      QMessageBox::warning(this, tr("Warning"), tr("Internal error. Local Mean not calculated."));
      return;
    }
  }

  //Moran
  if(m_ui->m_globalMoranIndexCheckBox->isChecked() || m_ui->m_localMoranIndex->isChecked())
  {
    try
    {
      te::sa::ZAndWZ(gpm.get(), attrIdx); //this function calculates the standard deviation Z and local mean of the desviation Z (WZ).

      double globalMoranIndex = te::sa::MoranIndex(gpm.get()); //this function calculates the moran index, global and local.

      //set the global moran index value
      if(m_ui->m_globalMoranIndexCheckBox->isChecked())
      {
        m_ui->m_globalMoranIndexValueLineEdit->setText(QString::number(globalMoranIndex));
      }

      //evaluating the significance of the global index.
      if(m_ui->m_globalMoranIndexCheckBox->isChecked() && !m_ui->m_globalEvalNotRadioButton->isChecked())
      {
        int permutValue = 0;

        if(m_ui->m_globalEval99RadioButton->isChecked())
          permutValue = 99;
        else if(m_ui->m_globalEval999RadioButton->isChecked())
          permutValue = 999;

        double globalMoranSignificance = te::sa::GlobalMoranSignificance(gpm.get(), attrIdx, permutValue, globalMoranIndex); //this function calculates the significance of the moran index.

        m_ui->m_globalMoranIndexPValueLineEdit->setText(QString::number(globalMoranSignificance));
      }

      te::sa::BoxMap(gpm.get(), 0); //this function calculates the box map information, needs Z and WZ information... MEAN = 0 ??

      //evaluating the significance of LISA.
      if(m_ui->m_localMoranIndex->isChecked() && !m_ui->m_localEvalNotRadioButton->isChecked())
      {
        int permutValue = 0;

        if(m_ui->m_localEval99RadioButton->isChecked())
          permutValue = 99;
        else if(m_ui->m_localEval999RadioButton->isChecked())
          permutValue = 999;
        else if(m_ui->m_localEval9999RadioButton->isChecked())
          permutValue = 9999;

        te::sa::LisaStatisticalSignificance(gpm.get(), permutValue); //this function calculates the lisa (local moran index) significance, needs Z, Local Moran and Number of Neighbours attributes calculated.

        te::sa::LISAMap(gpm.get(), permutValue); //this function calculates the lisa map, needs LISASig attribute calculated.

        te::sa::MoranMap(gpm.get()); //this function calculates the moran map, needs LISAMap and BoxMAP attributes calculated.
      }
    }
    catch(...)
    {
      QMessageBox::warning(this, tr("Warning"), tr("Internal error. Moran not calculated."));
      return;
    }
  }

  //save spatial statistics calculated into a new dataset
  std::string dataSetName = m_ui->m_newLayerNameLineEdit->text().toStdString();

  std::size_t idx = dataSetName.find(".");
  if (idx != std::string::npos)
        dataSetName=dataSetName.substr(0,idx);

  te::da::DataSourcePtr outputDataSource;

  if(m_toFile)
  {
    //create new data source
    boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

    std::map<std::string, std::string> dsInfo;
    dsInfo["URI"] = uri.string();

    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id_ds = boost::uuids::to_string(u);

    te::da::DataSourceInfoPtr dsInfoPtr(new te::da::DataSourceInfo);
    dsInfoPtr->setConnInfo(dsInfo);
    dsInfoPtr->setTitle(uri.stem().string());
    dsInfoPtr->setAccessDriver("OGR");
    dsInfoPtr->setType("OGR");
    dsInfoPtr->setDescription(uri.string());
    dsInfoPtr->setId(id_ds);

    te::da::DataSourceInfoManager::getInstance().add(dsInfoPtr);

    outputDataSource = te::da::DataSourceManager::getInstance().get(id_ds, "OGR", dsInfoPtr->getConnInfo());
  }
  else
  {
    outputDataSource = te::da::GetDataSource(m_outputDatasource->getId());
  }

  //associate the input geometry attribute to the GPM
  te::gm::GeometryProperty* gmProp = te::da::GetFirstGeomProperty(dataSetType.get());
  int geomIdx = te::sa::AssociateGPMVertexAttribute(gpm.get(), ds.get(), dsLayer->getDataSetName(), attrLink, gmProp->getName(), gmProp->getType(), gmProp->getSRID(), gmProp->getGeometryType());

  try
  {
    gpm->toDataSource(outputDataSource, dataSetName);
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Error saving GPM into data source."));
    return;
  }

  accept();
}

std::auto_ptr<te::sa::GeneralizedProximityMatrix> te::sa::SpatialStatisticsDialog::loadGPM()
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

  std::string attrName = m_ui->m_attrIdComboBox->currentText().toStdString();

  if(m_ui->m_gpmLineEdit->text().isEmpty())
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

void te::sa::SpatialStatisticsDialog::onTargetDatasourceToolButtonPressed()
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

void te::sa::SpatialStatisticsDialog::onTargetFileToolButtonPressed()
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
