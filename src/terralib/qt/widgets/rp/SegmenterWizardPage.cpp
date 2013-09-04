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
#include "RasterNavigatorWidget.h"
#include "SegmenterWizardPage.h"
#include "ui_SegmenterWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QMessageBox>

// STL
#include <memory>


te::qt::widgets::SegmenterWizardPage::SegmenterWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::SegmenterWizardPageForm),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  fillSegmenterTypes();

//build form
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_frame);
  m_navigator.reset( new te::qt::widgets::RasterNavigatorWidget(m_ui->m_frame));
  m_navigator->showAsPreview(true);
  displayLayout->addWidget(m_navigator.get());
  displayLayout->setContentsMargins(0,0,0,0);

//configure page
  this->setTitle(tr("Segmenter"));
  this->setSubTitle(tr("Select the type of segmenter and set their specific parameters."));

//connects
  connect(m_ui->m_strategyTypeComboBox, SIGNAL(activated(int)), this, SLOT(onStrategyTypeComboBoxActivated(int)));
}

te::qt::widgets::SegmenterWizardPage::~SegmenterWizardPage()
{
}

void te::qt::widgets::SegmenterWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  std::list<te::map::AbstractLayerPtr> list;

  list.push_back(m_layer);

  m_navigator->set(m_layer);

  listBands();
}

te::rp::Segmenter::InputParameters te::qt::widgets::SegmenterWizardPage::getInputParams()
{
  te::rp::Segmenter::InputParameters algoInputParams;

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  //set segmenter parameters
  algoInputParams.m_inputRasterPtr = inputRst.release();

  int nBands = m_ui->m_bandTableWidget->rowCount();

  for(int i = 0; i < nBands; ++i)
  {
    QCheckBox* checkBox = (QCheckBox*)m_ui->m_bandTableWidget->cellWidget(i, 0);
    
    if(checkBox->isChecked())
    {
      algoInputParams.m_inputRasterBands.push_back(i);
    }
  }

  int index = m_ui->m_strategyTypeComboBox->currentIndex();

  std::string strategyName = m_ui->m_strategyTypeComboBox->itemData(index).toString().toStdString();

  if(strategyName == "RegionGrowing")
  {
    te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
    strategyParameters.m_minSegmentSize = m_ui->m_minimumSegmentSizeRGLineEdit->text().toUInt();
    strategyParameters.m_segmentsSimilarityThreshold = m_ui->m_thresholdRGDoubleSpinBox->value();
    strategyParameters.m_segmentFeatures = te::rp::SegmenterRegionGrowingStrategy::Parameters::MeanFeaturesType;

    algoInputParams.m_strategyName = "RegionGrowing";
    algoInputParams.setSegStrategyParams( strategyParameters );
  }
  else if(strategyName == "Baatz")
  {
    te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
    strategyParameters.m_minSegmentSize = m_ui->m_minimumSegmentSizeRGLineEdit->text().toUInt();
    strategyParameters.m_segmentsSimilarityThreshold = m_ui->m_thresholdBaatzDoubleSpinBox->value();
    strategyParameters.m_segmentFeatures = te::rp::SegmenterRegionGrowingStrategy::Parameters::BaatzFeaturesType;

    for(int i = 0; i < nBands; ++i)
    {
      QCheckBox* checkBox = (QCheckBox*)m_ui->m_bandTableWidget->cellWidget(i, 0);
    
      if(checkBox->isChecked())
      {
        QDoubleSpinBox* spinBox = (QDoubleSpinBox*)m_ui->m_bandTableWidget->cellWidget(i, 1);

        strategyParameters.m_bandsWeights.push_back(spinBox->value());
      }
    }

    strategyParameters.m_colorWeight = m_ui->m_colorWeightBaatzDoubleSpinBox->value();
    strategyParameters.m_compactnessWeight = m_ui->m_compactnessWeightBaatzDoubleSpinBox->value();

    algoInputParams.m_strategyName = "RegionGrowing";
    algoInputParams.setSegStrategyParams( strategyParameters );
  }

  return algoInputParams;
}

void te::qt::widgets::SegmenterWizardPage::onStrategyTypeComboBoxActivated(int index)
{
  std::string strategyName = m_ui->m_strategyTypeComboBox->itemData(index).toString().toStdString();

  if(strategyName == "RegionGrowing")
  {
    QStringList list;
    list.append(tr("Band"));

    m_ui->m_bandTableWidget->setColumnCount(1);
    m_ui->m_bandTableWidget->setHorizontalHeaderLabels(list);
  }
  else if(strategyName == "Baatz")
  {
    QStringList list;
    list.append(tr("Band"));
    list.append(tr("Weight"));

    m_ui->m_bandTableWidget->setColumnCount(2);
    m_ui->m_bandTableWidget->setHorizontalHeaderLabels(list);

    int nBands = m_ui->m_bandTableWidget->rowCount();

    for(int i = 0; i < nBands; ++i)
    {
      QDoubleSpinBox* spinBox = new QDoubleSpinBox(m_ui->m_bandTableWidget);
      spinBox->setMinimum(0.0);
      spinBox->setMaximum(1.0);
      spinBox->setSingleStep(0.1);
      spinBox->setDecimals(4);
      spinBox->setValue(0.3333);

      m_ui->m_bandTableWidget->setCellWidget(i, 1, spinBox);
    }
  }

  m_ui->m_bandTableWidget->resizeColumnsToContents();
  m_ui->m_bandTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

void te::qt::widgets::SegmenterWizardPage::fillSegmenterTypes()
{
  m_ui->m_strategyTypeComboBox->clear();

  m_ui->m_strategyTypeComboBox->addItem(tr("Region Growing"), "RegionGrowing");
  m_ui->m_strategyTypeComboBox->addItem(tr("Baatz"), "Baatz");
}

void te::qt::widgets::SegmenterWizardPage::listBands()
{
  assert(m_layer.get());

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
         m_ui->m_bandTableWidget->setRowCount(0);

        // initializing the list of bands
        for(unsigned b = 0 ; b < inputRst->getNumberOfBands(); b++)
        {
          int newrow = m_ui->m_bandTableWidget->rowCount();
          m_ui->m_bandTableWidget->insertRow(newrow);

          QString bName(tr("Band "));
          bName.append(QString::number(b));
        
          QCheckBox* bandCheckBox = new QCheckBox(bName, this);

          m_ui->m_bandTableWidget->setCellWidget(newrow, 0, bandCheckBox);
        }
      }
    }
  }
}
