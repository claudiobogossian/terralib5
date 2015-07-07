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
  \file terralib/qt/widgets/rp/SegmenterWizardPage.cpp

  \brief This file defines a class for a Segmenter Wizard page.
*/

// TerraLib
#include "../../../common/progress/ProgressManager.h"
#include "../../../common/StringUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/Segmenter.h"
#include "../../../rp/SegmenterRegionGrowingBaatzStrategy.h"
#include "../../../rp/SegmenterRegionGrowingMeanStrategy.h"
#include "../progress/ProgressViewerDialog.h"
#include "RasterNavigatorWidget.h"
#include "SegmenterWizardPage.h"
#include "ui_SegmenterWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QCheckBox>
#include <QIntValidator>
#include <QMessageBox>

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
  m_navigator->showAsPreview(true, false);
  m_navigator->hideColorCompositionTool(true);
  displayLayout->addWidget(m_navigator.get());
  displayLayout->setContentsMargins(0,0,0,0);

//configure page
  this->setTitle(tr("Segmenter"));
  this->setSubTitle(tr("Select the type of segmenter and set their specific parameters."));

  QIntValidator* intValRG = new QIntValidator(this);
  intValRG->setBottom(0);
  m_ui->m_minimumSegmentSizeRGLineEdit->setValidator(intValRG);

  QIntValidator* intValB = new QIntValidator(this);
  intValB->setBottom(0);
  m_ui->m_minimumSegmentSizeRGLineEdit_2->setValidator(intValB);

  m_ui->m_noDataValueLineEdit->setValidator(new QDoubleValidator(this));
  
  te::rp::SegmenterRegionGrowingBaatzStrategy::Parameters regGrowStrategyParameters;
  m_ui->m_minimumSegmentSizeRGLineEdit->setText( QString::number( regGrowStrategyParameters.m_minSegmentSize ) );
  m_ui->m_minimumSegmentSizeRGLineEdit_2->setText( QString::number( regGrowStrategyParameters.m_minSegmentSize ) );
  m_ui->m_thresholdRGDoubleSpinBox->setValue( regGrowStrategyParameters.m_segmentsSimilarityThreshold );
  m_ui->m_thresholdBaatzDoubleSpinBox->setValue( regGrowStrategyParameters.m_segmentsSimilarityThreshold );
  m_ui->m_colorWeightBaatzDoubleSpinBox->setValue( regGrowStrategyParameters.m_colorWeight );
  m_ui->m_compactnessWeightBaatzDoubleSpinBox->setValue( regGrowStrategyParameters.m_compactnessWeight );

//connects
  connect(m_ui->m_strategyTypeComboBox, SIGNAL(activated(int)), this, SLOT(onStrategyTypeComboBoxActivated(int)));
  connect(m_navigator.get(), SIGNAL(previewClicked()), this, SLOT(apply()));
}

te::qt::widgets::SegmenterWizardPage::~SegmenterWizardPage()
{
}

bool te::qt::widgets::SegmenterWizardPage::isComplete() const
{
  int nBands = m_ui->m_bandTableWidget->rowCount();

  for(int i = 0; i < nBands; ++i)
  {
    QCheckBox* checkBox = (QCheckBox*)m_ui->m_bandTableWidget->cellWidget(i, 0);
    
    if(checkBox->isChecked())
    {
      return true;
    }
  }

  return false;
}

void te::qt::widgets::SegmenterWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  std::list<te::map::AbstractLayerPtr> list;

  list.push_back(m_layer);

  m_navigator->set(m_layer, true);

  listBands();
}

te::map::AbstractLayerPtr te::qt::widgets::SegmenterWizardPage::get()
{
  return m_layer;
}

te::rp::Segmenter::InputParameters te::qt::widgets::SegmenterWizardPage::getInputParams()
{
  te::rp::Segmenter::InputParameters algoInputParams;

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  m_inputRst.reset(ds->getRaster(rpos).release());

  //set segmenter parameters
  algoInputParams.m_inputRasterPtr = m_inputRst.get();

  int nBands = m_ui->m_bandTableWidget->rowCount();

  for(int i = 0; i < nBands; ++i)
  {
    QCheckBox* checkBox = (QCheckBox*)m_ui->m_bandTableWidget->cellWidget(i, 0);
    
    if(checkBox->isChecked())
    {
      algoInputParams.m_inputRasterBands.push_back(i);
      
      if(m_ui->m_noDataValueCheckBox->isChecked() && !m_ui->m_noDataValueLineEdit->text().isEmpty())
      {
        algoInputParams.m_inputRasterNoDataValues.push_back(m_ui->m_noDataValueLineEdit->text().toDouble());
      }      
    }
  }

  int index = m_ui->m_strategyTypeComboBox->currentIndex();

  std::string strategyName = m_ui->m_strategyTypeComboBox->itemData(index).toString().toStdString();

  if(strategyName == "RegionGrowingMean")
  {
    te::rp::SegmenterRegionGrowingMeanStrategy::Parameters strategyParameters;
    strategyParameters.m_minSegmentSize = m_ui->m_minimumSegmentSizeRGLineEdit->text().toUInt();
    strategyParameters.m_segmentsSimilarityThreshold = m_ui->m_thresholdRGDoubleSpinBox->value();
    strategyParameters.m_enableLocalMutualBestFitting = m_ui->m_localMutualBestFittingCheckBox->isChecked();
    strategyParameters.m_enableSameIterationMerges = m_ui->m_sameIterationMergeCheckBox->isChecked();

    algoInputParams.m_strategyName = "RegionGrowingMean";
    algoInputParams.setSegStrategyParams( strategyParameters );
  }
  else if(strategyName == "RegionGrowingBaatz")
  {
    te::rp::SegmenterRegionGrowingBaatzStrategy::Parameters strategyParameters;
    strategyParameters.m_minSegmentSize = m_ui->m_minimumSegmentSizeRGLineEdit->text().toUInt();
    strategyParameters.m_segmentsSimilarityThreshold = m_ui->m_thresholdBaatzDoubleSpinBox->value();
    strategyParameters.m_enableLocalMutualBestFitting = m_ui->m_localMutualBestFittingCheckBox->isChecked();
    strategyParameters.m_enableSameIterationMerges = m_ui->m_sameIterationMergeCheckBox->isChecked();    

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

    algoInputParams.m_strategyName = "RegionGrowingBaatz";
    algoInputParams.setSegStrategyParams( strategyParameters );
  }

  return algoInputParams;
}

void te::qt::widgets::SegmenterWizardPage::onStrategyTypeComboBoxActivated(int index)
{
  std::string strategyName = m_ui->m_strategyTypeComboBox->itemData(index).toString().toStdString();

  if(strategyName == "RegionGrowingMean")
  {
    QStringList list;
    list.append(tr("Band"));

    m_ui->m_bandTableWidget->setColumnCount(1);
    m_ui->m_bandTableWidget->setHorizontalHeaderLabels(list);
  }
  else if(strategyName == "RegionGrowingBaatz")
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
  
#if (QT_VERSION >= 0x050000)
  m_ui->m_bandTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
  m_ui->m_bandTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
}

void te::qt::widgets::SegmenterWizardPage::apply()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

//get preview raster
  te::rst::Raster* inputRst = m_navigator->getExtentRaster(true);

  //set segmenters parameters
  te::rp::Segmenter::InputParameters algoInputParams = getInputParams();

  algoInputParams.m_inputRasterPtr = inputRst;
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_enableBlockProcessing = false;

  te::rp::Segmenter::OutputParameters algoOutputParams;

  std::map<std::string, std::string> rinfo;
  rinfo["MEM_RASTER_NROWS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfRows());
  rinfo["MEM_RASTER_NCOLS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfColumns());
  rinfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>(inputRst->getBandDataType(0));
  rinfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfBands());

  algoOutputParams.m_rType = "MEM";
  algoOutputParams.m_rInfo = rinfo;

  //run contrast
  te::rp::Segmenter algorithmInstance;

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  try
  {
    if(algorithmInstance.initialize(algoInputParams))
    {
      if(algorithmInstance.execute(algoOutputParams))
      {
        std::auto_ptr<te::rst::Raster> rst = algoOutputParams.m_outputRasterPtr;

        m_navigator->drawRaster(rst.get());
      }
    }
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Constrast error."));
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  //delete input raster dataset
  delete inputRst;
}

void te::qt::widgets::SegmenterWizardPage::fillSegmenterTypes()
{
  m_ui->m_strategyTypeComboBox->clear();

  m_ui->m_strategyTypeComboBox->addItem(tr("Region Growing Mean"), "RegionGrowingMean");
  m_ui->m_strategyTypeComboBox->addItem(tr("Region Growing Baatz"), "RegionGrowingBaatz");
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
          bandCheckBox->setChecked( true );

          m_ui->m_bandTableWidget->setCellWidget(newrow, 0, bandCheckBox);
        }
      }
    }
  }
}
