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
  \file terralib/sa/qt/KernelRatioDialog.cpp

  \brief A dialog to calculate the kernel ratio of a datasets.
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../maptools/DataSetLayer.h"
#include "../core/KernelRatioOperation.h"
#include "../Exception.h"
#include "KernelRatioDialog.h"
#include "ui_KernelRatioDialogForm.h"

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

te::sa::KernelRatioDialog::KernelRatioDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::KernelRatioDialogForm)
{
// add controls
  m_ui->setupUi(this);

  m_ui->m_nColsLineEdit->setValidator(new QIntValidator(this));

  fillKernelParameters();

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("sa-kernelratio-hint").pixmap(112,48));

// connectors
  connect(m_ui->m_inputLayerComboBox, SIGNAL(activated(int)), this, SLOT(onInputLayerComboBoxActivated(int)));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

// help info
  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/sa/sa_kernelratio.html");
}

te::sa::KernelRatioDialog::~KernelRatioDialog()
{
}

void te::sa::KernelRatioDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while(it != layers.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

    if(dsLayer && dsType->hasGeom())
    {
      m_ui->m_inputLayerComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));
    }

    ++it;
  }

// fill attributes combo A and B
  if(m_ui->m_inputLayerComboBox->count() > 0)
    onInputLayerComboBoxActivated(0);
}

void te::sa::KernelRatioDialog::fillKernelParameters()
{
  //function
  m_ui->m_functionComboBox->clear();
  
  m_ui->m_functionComboBox->addItem("Quartic", QVariant(te::sa::Quartic));
  m_ui->m_functionComboBox->addItem("Normal", QVariant(te::sa::Normal));
  m_ui->m_functionComboBox->addItem("Triangular", QVariant(te::sa::Triangular));
  m_ui->m_functionComboBox->addItem("Uniform", QVariant(te::sa::Uniform));
  m_ui->m_functionComboBox->addItem("Negative Exponential", QVariant(te::sa::Negative_Exp));

  //estimation
  m_ui->m_estimationComboBox->clear();
  
  m_ui->m_estimationComboBox->addItem("Density", QVariant(te::sa::Density));
  m_ui->m_estimationComboBox->addItem("Spatial Moving Average", QVariant(te::sa::Spatial_Moving_Average));
  m_ui->m_estimationComboBox->addItem("Probability", QVariant(te::sa::Probability));

  //combination
  m_ui->m_combinationComboBox->clear();
  
  m_ui->m_combinationComboBox->addItem("Ratio", QVariant(te::sa::Ratio));
  m_ui->m_combinationComboBox->addItem("Log Ratio", QVariant(te::sa::Log_Ratio));
  m_ui->m_combinationComboBox->addItem("Absolute Difference", QVariant(te::sa::Abs_Difference));
  m_ui->m_combinationComboBox->addItem("Relative Difference", QVariant(te::sa::Relative_Difference));
  m_ui->m_combinationComboBox->addItem("Absolute Sum", QVariant(te::sa::Abs_Sum));
  m_ui->m_combinationComboBox->addItem("Relative Sum", QVariant(te::sa::Relative_Sum));
}

void te::sa::KernelRatioDialog::onInputLayerComboBoxActivated(int index)
{
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(index, Qt::UserRole);
  
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

  std::vector<te::dt::Property*> propVec = dsType->getProperties();

  m_ui->m_attrLayerComboBoxA->clear();
  m_ui->m_attrLayerComboBoxB->clear();

  for(std::size_t t = 0; t < propVec.size(); ++t)
  {
    int dataType = propVec[t]->getType();

    if (dataType == te::dt::INT16_TYPE || dataType == te::dt::UINT16_TYPE ||
        dataType == te::dt::INT32_TYPE || dataType == te::dt::UINT32_TYPE ||
        dataType == te::dt::INT64_TYPE || dataType == te::dt::UINT64_TYPE ||
        dataType == te::dt::FLOAT_TYPE || dataType == te::dt::DOUBLE_TYPE)
    {
      m_ui->m_attrLayerComboBoxA->addItem(propVec[t]->getName().c_str(), dataType);
      m_ui->m_attrLayerComboBoxB->addItem(propVec[t]->getName().c_str(), dataType);
    }
  }
}

void te::sa::KernelRatioDialog::onOkPushButtonClicked()
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

  //get selected layer A
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(m_ui->m_inputLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

  //set input kernel parameters
  te::sa::KernelInputParams* kInParams = new te::sa::KernelInputParams();

  kInParams->m_isRatioKernel = true;

  kInParams->m_functionType = (te::sa::KernelFunctionType) m_ui->m_functionComboBox->itemData(m_ui->m_functionComboBox->currentIndex()).toInt();
  kInParams->m_estimationType = (te::sa::KernelEstimationType) m_ui->m_estimationComboBox->itemData(m_ui->m_estimationComboBox->currentIndex()).toInt();
  kInParams->m_combinationType = (te::sa::KernelCombinationType) m_ui->m_combinationComboBox->itemData(m_ui->m_combinationComboBox->currentIndex()).toInt();

  kInParams->m_useAdaptativeRadius = m_ui->m_useAdaptRadiusCheckBox->isChecked();
  
  if(!m_ui->m_useAdaptRadiusCheckBox->isChecked())
  {
    kInParams->m_radiusPercentValue = m_ui->m_radiusSpinBox->value();
  }

  if(m_ui->m_useAttrLayerCheckBoxA->isChecked())
  {
    kInParams->m_intensityAttrName = m_ui->m_attrLayerComboBoxA->currentText().toStdString();
  }

  if(m_ui->m_useAttrLayerCheckBoxB->isChecked())
  {
    kInParams->m_intensityAttrName2 = m_ui->m_attrLayerComboBoxB->currentText().toStdString();
  }

  kInParams->m_ds = l->getData();
  kInParams->m_dsType = l->getSchema();

  //set output kernel parameters
  te::sa::KernelOutputParams* kOutParams = new te::sa::KernelOutputParams();

  kOutParams->m_outputPath = m_ui->m_repositoryLineEdit->text().toStdString();
  kOutParams->m_outputDataSetName = m_ui->m_newLayerNameLineEdit->text().toStdString();

  if(m_ui->m_gridRadioButton->isChecked())
  {
    if(m_ui->m_nColsLineEdit->text().isEmpty())
    {
      QMessageBox::information(this, tr("Warning"), tr("Define the number of columns to the output grid."));
      return;
    }

    kOutParams->m_storageType = te::sa::Grid;
    kOutParams->m_nCols = m_ui->m_nColsLineEdit->text().toInt();
  }
  else if(m_ui->m_attrRadioButton->isChecked())
  {
    if(m_ui->m_attrNameLineEdit->text().isEmpty())
    {
      QMessageBox::information(this, tr("Warning"), tr("Define the attribute name to the output data."));
      return;
    }

    kOutParams->m_storageType = te::sa::Attribute;
    kOutParams->m_outputAttrName = m_ui->m_attrNameLineEdit->text().toStdString();
  }

  //execute kernel
  try
  {
    te::sa::KernelRatioOperation op;

    op.setParameters(kInParams, kOutParams);

    op.execute();
  }
  catch(...)
  {
    QMessageBox::information(this, tr("Warning"), tr("Kernel Map internal error."));
    return;
  }

  accept();
}

void te::sa::KernelRatioDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();
  
  QString fileName = QFileDialog::getExistingDirectory(this, tr("Set output location..."));
  
  if (fileName.isEmpty())
    return;

  m_ui->m_repositoryLineEdit->setText(fileName);
}
