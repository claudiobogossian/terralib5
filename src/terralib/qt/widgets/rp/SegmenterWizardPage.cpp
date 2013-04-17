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
  \file terralib/qt/widgets/rp/SegmenterWizardPage.cpp

  \brief This file defines a class for a Segmenter Wizard page.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/Segmenter.h"
#include "../../../rp/SegmenterRegionGrowingStrategy.h"
#include "SegmenterWizardPage.h"
#include "ui_SegmenterWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QtGui/QMessageBox>

te::qt::widgets::SegmenterWizardPage::SegmenterWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::SegmenterWizardPageForm),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  fillSegmenterTypes();

//configure page
  this->setTitle(tr("Segmenter"));
  this->setSubTitle(tr("Select the type of segmenter and set their specific parameters."));
}

te::qt::widgets::SegmenterWizardPage::~SegmenterWizardPage()
{
}

void te::qt::widgets::SegmenterWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  listBands();
}

te::rp::Segmenter::InputParameters te::qt::widgets::SegmenterWizardPage::getInputParams()
{
  te::rp::Segmenter::InputParameters algoInputParams;

  //get input raster
  te::da::DataSet* ds = m_layer->getData();
  te::rst::Raster* inputRst = ds->getRaster();

  //set segmenter parameters
  algoInputParams.m_inputRasterPtr = inputRst;

  QList<QListWidgetItem *> selectedBands = m_ui->m_inputRasterBandsListWidget->selectedItems();
  QList<QListWidgetItem *>::const_iterator itBegin = selectedBands.begin();
  QList<QListWidgetItem *>::const_iterator itEnd = selectedBands.end();
  while( itBegin != itEnd )
  {
    algoInputParams.m_inputRasterBands.push_back( (*itBegin)->text().toUInt() );
    ++itBegin;
  }

  algoInputParams.m_enableThreadedProcessing = m_ui->m_enableThreadedProcessingcheckBox->isChecked();
  algoInputParams.m_maxSegThreads = m_ui->m_maximumThreadsNumberLineEdit->text().toUInt();
  algoInputParams.m_enableBlockProcessing = m_ui->m_enableBlockProcessingcheckBox->isChecked();
  algoInputParams.m_enableBlockMerging = m_ui->m_enableBlockMergingCheckBox->isChecked();
  algoInputParams.m_maxBlockSize = m_ui->m_maximumBlockSizeLineEdit->text().toUInt();
  algoInputParams.m_strategyName = m_ui->m_strategyTypeComboBox->currentText().toStdString();

  if( algoInputParams.m_strategyName == "RegionGrowing" )
  {
    te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
    strategyParameters.m_minSegmentSize = m_ui->m_minimumSegmentSizeRGLineEdit->text().toUInt();
    strategyParameters.m_segmentsSimilarityThreshold = m_ui->m_segmentsSimilarityThresholdRGLineEdit->text().toDouble();
    strategyParameters.m_segmentFeatures = te::rp::SegmenterRegionGrowingStrategy::Parameters::MeanFeaturesType;   //NOT IN INTERFACE
    algoInputParams.setSegStrategyParams( strategyParameters );
  }

  delete ds;

  return algoInputParams;
}

void te::qt::widgets::SegmenterWizardPage::fillSegmenterTypes()
{
  m_ui->m_strategyTypeComboBox->clear();

  m_ui->m_strategyTypeComboBox->addItem(tr("RegionGrowing"));
}

void te::qt::widgets::SegmenterWizardPage::listBands()
{
  assert(m_layer.get());

  //get input raster
  te::da::DataSet* ds = m_layer->getData();

  if(ds)
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds, te::dt::RASTER_TYPE);

    te::rst::Raster* inputRst = ds->getRaster();

    if(inputRst)
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
        m_ui->m_inputRasterBandsListWidget->addItem(QString::number(i));
      }
    }
  }

  delete ds;
}
