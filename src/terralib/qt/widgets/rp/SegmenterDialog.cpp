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
  \file terralib/rp/widgets/se/SegmenterDialog.cpp

  \brief A dialog used to execute image segmentation
*/


#include "SegmenterDialog.h"

#include "../../../rp/Segmenter.h"
#include "../../../rp/SegmenterRegionGrowingStrategy.h"
#include <ui_SegmenterForm.h>

#include <QMessageBox>
#include <QListWidgetItem>

// TerraLib

te::qt::widgets::SegmenterDialog::SegmenterDialog(
  te::rst::Raster const* inputRasterPtr, const std::string& outpuRasterDSType, 
  const std::map< std::string, std::string >& outpuRasterInfo, 
  QWidget* parent, Qt::WindowFlags f)
  : QDialog( parent, f ),
    m_inputRasterPtr( inputRasterPtr ),
    m_outpuRasterDSType( outpuRasterDSType ),
    m_outpuRasterInfo( outpuRasterInfo )
{
  m_uiPtr = new Ui::SegmenterForm;
  m_uiPtr->setupUi(this);
  
  // Signals & slots
  connect(m_uiPtr->m_okPushButton, SIGNAL(clicked()), SLOT(on_okPushButton_clicked()));   
  
  // initializing the input raster bands combo
  
  for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx < 
    inputRasterPtr->getNumberOfBands() ; ++inRasterBandsIdx )
  {
    m_uiPtr->m_inputRasterBandsListWidget->addItem( 
      QString::number( inRasterBandsIdx ) );
  }  
}

te::qt::widgets::SegmenterDialog::~SegmenterDialog()
{
  delete m_uiPtr;
}

bool te::qt::widgets::SegmenterDialog::getOutputRaster( 
  boost::shared_ptr< te::rst::Raster >& outputRasterPtr )
{
  if( m_outputRasterPtr.get() )
  {
    outputRasterPtr = m_outputRasterPtr;
    return true;
  }
  else
  {
    return false;
  }
}

void te::qt::widgets::SegmenterDialog::on_okPushButton_clicked()
{
  m_outputRasterPtr.reset();
  
  if( m_inputRasterPtr )
  {
    // Creating the algorithm parameters
    
    te::rp::Segmenter::InputParameters algoInputParams;
    
    algoInputParams.m_inputRasterPtr = m_inputRasterPtr;
    
    QList<QListWidgetItem *> selectedBands = 
      m_uiPtr->m_inputRasterBandsListWidget->selectedItems();
      
    if( selectedBands.size() > 0 )
    {
      QList<QListWidgetItem *>::const_iterator itBegin = selectedBands.begin();
      QList<QListWidgetItem *>::const_iterator itEnd = selectedBands.end();
      while( itBegin != itEnd )
      {
        algoInputParams.m_inputRasterBands.push_back( (*itBegin)->text().toUInt() );
        ++itBegin;
      }
        
      algoInputParams.m_enableThreadedProcessing = 
        m_uiPtr->m_enableThreadedProcessingcheckBox->isChecked();
      algoInputParams.m_maxSegThreads = m_uiPtr->m_maximumThreadsNumberLineEdit->text().toUInt();
      algoInputParams.m_enableBlockProcessing = 
        m_uiPtr->m_enableBlockProcessingcheckBox->isChecked();
      algoInputParams.m_enableBlockMerging = 
        m_uiPtr->m_enableBlockMergingCheckBox->isChecked();
      algoInputParams.m_maxBlockSize = m_uiPtr->m_maximumBlockSizeLineEdit->text().toUInt();
      
      
      algoInputParams.m_strategyName = m_uiPtr->m_segmenterStrategyComboBox->currentText().toStdString();
      
      if( algoInputParams.m_strategyName == "RegionGrowing" )
      {
        te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
        strategyParameters.m_minSegmentSize = 
          m_uiPtr->m_minimumSegmentSizeRGLineEdit->text().toUInt();
        strategyParameters.m_segmentsSimilarityThreshold = 
          m_uiPtr->m_segmentsSimilarityThresholdRGLineEdit->text().toDouble();
        algoInputParams.setSegStrategyParams( strategyParameters );
      }
      
      te::rp::Segmenter::OutputParameters algoOutputParams;
      algoOutputParams.m_rInfo = m_outpuRasterInfo;
      algoOutputParams.m_rType = m_outpuRasterDSType;
      
      // Executing the algorithm
      
      te::rp::Segmenter algorithmInstance;
      
      if( algorithmInstance.initialize( algoInputParams ) )
      {
        if( algorithmInstance.execute( algoOutputParams ) )
        {
          m_outputRasterPtr = algoOutputParams.m_outputRasterPtr;
          QMessageBox::information(this, "", tr("Segmentation ended sucessfully"));
        }
        else
        {
          QMessageBox::critical(this, "", tr("Segmentation execution error"));
        }
      }
      else
      {
        QMessageBox::critical(this, "", tr("Segmentation initialization error"));
      }
    }
    else
    {
      QMessageBox::critical(this, "", tr("Invalid number of bands"));
    }
  }
  else
  {
    QMessageBox::critical(this, "", tr("Invalid input raster"));
  }
}
