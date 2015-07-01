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
  \file terralib/rp/widgets/se/ClassifierDialog.cpp

  \brief A dialog used to execute image classification.
*/

// TerraLib
#include "../../../rp/Classifier.h"
#include "../../../rp/ClassifierISOSegStrategy.h"

// GUI
#include "ClassifierDialog.h"
#include <ui_ClassifierForm.h>

// QT
#include <QListWidgetItem>
#include <QMessageBox>

te::qt::widgets::ClassifierDialog::ClassifierDialog(const te::rst::Raster* inputRasterPtr, std::vector<te::gm::Polygon*> inputPolygons,
                                                    const std::string& outpuRasterDSType, const std::map<std::string, std::string>& outpuRasterInfo,
                                                    QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_inputRasterPtr(inputRasterPtr),
    m_inputPolygons(inputPolygons),
    m_outpuRasterDSType(outpuRasterDSType),
    m_outpuRasterInfo(outpuRasterInfo)
{
  m_uiPtr = new Ui::ClassifierForm;

  m_uiPtr->setupUi(this);

// Signals & slots
  connect(m_uiPtr->m_okPushButton, SIGNAL(clicked()), SLOT(on_okPushButton_clicked()));

// initializing the input raster bands combo
  for(unsigned b = 0 ; b < inputRasterPtr->getNumberOfBands(); b++)
    m_uiPtr->m_inputRasterBandsListWidget->addItem(QString::number(b));

  m_uiPtr->m_helpPushButton->setPageReference("widgets/rp/classifier.html");
}

te::qt::widgets::ClassifierDialog::~ClassifierDialog()
{
  delete m_uiPtr;
}

bool te::qt::widgets::ClassifierDialog::getOutputRaster(boost::shared_ptr<te::rst::Raster>& outputRasterPtr)
{
  if(m_outputRasterPtr.get())
  {
    outputRasterPtr = m_outputRasterPtr;

    return true;
  }

  return false;
}

void te::qt::widgets::ClassifierDialog::on_okPushButton_clicked()
{
  m_outputRasterPtr.reset();

  if (!m_inputRasterPtr)
  {
    QMessageBox::critical(this, "", tr("Invalid input raster"));

    return;
  }

// define classification parameters

// input parameters
  te::rp::Classifier::InputParameters algoInputParameters;
  algoInputParameters.m_strategyName = "isoseg";
  algoInputParameters.m_inputPolygons = m_inputPolygons;
  algoInputParameters.m_inputRasterPtr = m_inputRasterPtr;

  QList<QListWidgetItem*> selectedBands = m_uiPtr->m_inputRasterBandsListWidget->selectedItems();

  if(selectedBands.size() <= 0)
  {
    QMessageBox::critical(this, "", tr("Invalid number of bands"));

    return;
  }

  QList<QListWidgetItem*>::const_iterator it = selectedBands.begin();
  QList<QListWidgetItem*>::const_iterator itend = selectedBands.end();

  while(it != itend)
  {
    algoInputParameters.m_inputRasterBands.push_back((*it)->text().toUInt());

    ++it;
  }

// link specific parameters with chosen implementation
  te::rp::ClassifierISOSegStrategy::Parameters classifierparameters;
  classifierparameters.m_acceptanceThreshold = m_uiPtr->m_acceptanceThresholdComboBox->currentText().toDouble();

  algoInputParameters.setClassifierStrategyParams(classifierparameters);

// output parameters
  te::rp::Classifier::OutputParameters algoOutputParameters;
  algoOutputParameters.m_rInfo = m_outpuRasterInfo;
  algoOutputParameters.m_rType = m_outpuRasterDSType;

// execute the algorithm
  te::rp::Classifier classifierinstance;

  if(!classifierinstance.initialize(algoInputParameters))
  {
    QMessageBox::critical(this, "", tr("Classifier initialization error"));

    return;
  }
  if(!classifierinstance.execute(algoOutputParameters))
  {
    QMessageBox::critical(this, "", tr("Classifier execution error"));

    return;
  }

  QMessageBox::information(this, "", tr("Classification ended sucessfully"));

}
