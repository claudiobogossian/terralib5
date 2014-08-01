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
#include "../../dataaccess/utils/Utils.h"
#include "../../maptools/DataSetLayer.h"
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

// connectors
  connect(m_ui->m_inputLayerComboBox, SIGNAL(activated(int)), this, SLOT(onInputLayerComboBoxActivated(int)));
  connect(m_ui->m_gpmToolButton, SIGNAL(clicked()), this, SLOT(onGPMToolButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));

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

  std::auto_ptr<te::da::DataSet> dataSet = l->getData();

  std::auto_ptr<te::da::DataSetType> dataSetType = l->getSchema();

  std::string attrLink = dataSetType->getPrimaryKey()->getName();

  std::string attrName = m_ui->m_attrIdComboBox->currentText().toStdString();

  int type = m_ui->m_attrIdComboBox->itemData(m_ui->m_attrIdComboBox->currentIndex()).toInt();

  //associate the selected attribute to the GPM
  int attrIdx = te::sa::AssociateGPMVertexAttribute(ds.get(), dsLayer->getDataSetName(), attrLink, attrName, type, gpm.get());

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

  //update input dataset with spatial statistics calculated

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
