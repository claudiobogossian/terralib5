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
  \file terralib/rp/widgets/se/MixtureModelDialog.cpp

  \brief A dialog used to execute mixture model decomposition.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../rp/MixtureModel.h"
#include "../../../rp/MixtureModelLinearStrategy.h"
#include "../../../rp/MixtureModelPCAStrategy.h"

// GUI
#include "MixtureModelDialog.h"
#include <ui_MixtureModelForm.h>

// QT
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QImage>
#include <QtGui/QMessageBox>
#include <QtGui/QLineEdit>

te::qt::widgets::MixtureModelDialog::MixtureModelDialog(const te::rst::Raster* inputRasterPtr, const std::string& outpuRasterDSType,
                                                        const std::map<std::string, std::string>& outpuRasterInfo, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_inputRasterPtr(inputRasterPtr),
    m_outpuRasterDSType(outpuRasterDSType),
    m_outpuRasterInfo(outpuRasterInfo),
    totalComponents(0)
{
  m_uiPtr = new Ui::MixtureModelForm;

  m_uiPtr->setupUi(this);

// Signals & slots
  connect(m_uiPtr->m_okPushButton, SIGNAL(clicked()), SLOT(on_okPushButton_clicked()));
  connect(m_uiPtr->m_helpPushButton, SIGNAL(clicked()), SLOT(on_selectPixel_clicked())); // ???

// define sensor information
  QStringList sensorsDescriptions;
  sensorsDescriptions.append("sensor_description1");
  sensorsDescriptions.append("sensor_description2");
  sensorsDescriptions.append("sensor_description3");
// initializing the list of bands
  QGridLayout *bandsListLayout = new QGridLayout();
  for(unsigned b = 0 ; b < inputRasterPtr->getNumberOfBands(); b++)
  {
    std::string bname = "Band " + te::common::Convert2String(b);
    QCheckBox* bandCheckBox = new QCheckBox(tr(bname.c_str()));

    QComboBox* sensorDescriptionComboBox = new QComboBox();
    sensorDescriptionComboBox->addItems(sensorsDescriptions);

    bandsListLayout->addWidget(bandCheckBox, b, 0);
    bandsListLayout->addWidget(sensorDescriptionComboBox, b, 1);
  }
  m_uiPtr->m_bandsListGroupBox->setLayout(bandsListLayout);

// initializing the list of components
  QGridLayout *componentsListLayout = new QGridLayout();
  m_uiPtr->m_componentsListGroupBox->setLayout(componentsListLayout);
}

te::qt::widgets::MixtureModelDialog::~MixtureModelDialog()
{
  delete m_uiPtr;
}

bool te::qt::widgets::MixtureModelDialog::getOutputRaster(boost::shared_ptr<te::rst::Raster>& outputRasterPtr)
{
  if(m_outputRasterPtr.get())
  {
    outputRasterPtr = m_outputRasterPtr;

    return true;
  }

  return false;
}

void te::qt::widgets::MixtureModelDialog::on_okPushButton_clicked()
{
  m_outputRasterPtr.reset();

  if (!m_inputRasterPtr)
  {
    QMessageBox::critical(this, "", tr("Invalid input raster"));

    return;
  }

// define classification parameters

// input parameters
  te::rp::MixtureModel::InputParameters algoInputParameters;
  algoInputParameters.m_strategyName = (m_uiPtr->m_mmTypeComboBox->currentText() == "Linear" ? "linear" : "pca");
  algoInputParameters.m_inputRasterPtr = m_inputRasterPtr;

  unsigned selectedBands = 0;
  for (int i = 0; i < (m_uiPtr->m_bandsListGroupBox->layout()->count() - 1); i += 2)
  {
    QCheckBox* bandCheckBox = (QCheckBox*) m_uiPtr->m_bandsListGroupBox->layout()->itemAt(i)->widget();

    if (bandCheckBox->isChecked())
    {
      selectedBands++;

// insert band number
      algoInputParameters.m_inputRasterBands.push_back(i / 2);

// insert sensor description of band
      QComboBox *sensorComboBox = (QComboBox*) m_uiPtr->m_bandsListGroupBox->layout()->itemAt(i + 1)->widget();
      algoInputParameters.m_inputSensorBands.push_back(std::string(sensorComboBox->currentText().toAscii()));
    }
  }

  QGridLayout *componentsListLayout = (QGridLayout*) m_uiPtr->m_componentsListGroupBox->layout();
// insert mixture components
  for (unsigned int i = 0; i < totalComponents; i++)
  {
    QLineEdit* componentLineEdit = (QLineEdit*) componentsListLayout->itemAtPosition(i, 1)->widget();

    for (unsigned int b = 0; b < algoInputParameters.m_inputRasterBands.size(); b++)
    {
      QLineEdit* pixelValueLineEdit = (QLineEdit*) componentsListLayout->itemAtPosition(i, algoInputParameters.m_inputRasterBands[b] + 2)->widget();

      algoInputParameters.m_components[std::string(componentLineEdit->text().toAscii())].push_back(pixelValueLineEdit->text().toDouble());
    }
  }

// link specific parameters with chosen implementation
  te::rp::MixtureModelLinearStrategy::Parameters specificParameters; // ???
  algoInputParameters.setMixtureModelStrategyParams(specificParameters);

// output parameters
  te::rp::MixtureModel::OutputParameters algoOutputParameters;
  algoOutputParameters.m_rInfo = m_outpuRasterInfo;
  algoOutputParameters.m_rType = m_outpuRasterDSType;
  algoOutputParameters.m_normalizeOutput = m_uiPtr->m_normalizeOutputCheckBox->isChecked();
  algoOutputParameters.m_createErrorRaster = m_uiPtr->m_createErrorRasterCheckBox->isChecked();

// execute the algorithm
  te::rp::MixtureModel MixtureModelinstance;

  if(!MixtureModelinstance.initialize(algoInputParameters))
  {
    QMessageBox::critical(this, "", tr("MixtureModel initialization error"));

    return;
  }

  if(!MixtureModelinstance.execute(algoOutputParameters))
  {
    QMessageBox::critical(this, "", tr("MixtureModel execution error"));

    return;
  }

  QMessageBox::information(this, "", tr("Mixture model decomposition ended sucessfully"));
}

void te::qt::widgets::MixtureModelDialog::on_selectPixel_clicked()
{
  QGridLayout *componentsListLayout = (QGridLayout*) m_uiPtr->m_componentsListGroupBox->layout();

// find the selected pixel location for component
  unsigned int selectedColumn = totalComponents * 2; // ???
  unsigned int selectedRow = totalComponents * 5; // ???
  double value;

// add symbol for the selected component
  QLabel* classSymbol = new QLabel("Label");
  componentsListLayout->addWidget(classSymbol, totalComponents, 0);

// add new component information (class name, b0, b1, ...)
  QLineEdit* className = new QLineEdit("Change component name " + QString::number(totalComponents));
  componentsListLayout->addWidget(className, totalComponents, 1);

  for(unsigned b = 0 ; b < m_inputRasterPtr->getNumberOfBands(); b++)
  {
    m_inputRasterPtr->getValue(selectedColumn, selectedRow, value, b);

    QLineEdit* currentPixel = new QLineEdit(QString::number(value));

//    currentPixel->setSize!!!

    componentsListLayout->addWidget(currentPixel, totalComponents, b + 2);
  }
  totalComponents++;
}
