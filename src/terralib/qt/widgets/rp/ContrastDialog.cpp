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
  \file terralib/rp/widgets/se/ContrastDialog.cpp

  \brief A dialog used to execute image contrast enhencement
*/


#include "ContrastDialog.h"

#include "../../../rp/Contrast.h"
#include <ui_ContrastForm.h>

#include <QMessageBox>
#include <QListWidgetItem>

// TerraLib

te::qt::widgets::ContrastDialog::ContrastDialog(
  te::rst::Raster const* inputRasterPtr, const std::string& outpuRasterDSType, 
  const std::map< std::string, std::string >& outpuRasterInfo, 
  QWidget* parent, Qt::WindowFlags f)
  : QDialog( parent, f ),
    m_inputRasterPtr( inputRasterPtr ),
    m_outputRasterIsInputRaster( false ),
    m_outpuRasterDSType( outpuRasterDSType ),
    m_outpuRasterInfo( outpuRasterInfo )
{
  m_uiPtr = new Ui::ContrastForm;
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

te::qt::widgets::ContrastDialog::ContrastDialog(
  te::rst::Raster const* inputRasterPtr,  QWidget* parent, Qt::WindowFlags f)
  : QDialog( parent, f ),
    m_inputRasterPtr( inputRasterPtr ),
    m_outputRasterIsInputRaster( true )
{
  m_uiPtr = new Ui::ContrastForm;
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

te::qt::widgets::ContrastDialog::~ContrastDialog()
{
  delete m_uiPtr;
}

bool te::qt::widgets::ContrastDialog::getOutputRaster( 
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

void te::qt::widgets::ContrastDialog::on_okPushButton_clicked()
{
  m_outputRasterPtr.reset();
  
  if( m_inputRasterPtr )
  {
    // Creating the algorithm parameters
    
    te::rp::Contrast::InputParameters algoInputParams;
    te::rp::Contrast::OutputParameters algoOutputParams;
    
    algoInputParams.m_inRasterPtr = m_inputRasterPtr;
    
    QList<QListWidgetItem *> selectedBands = 
      m_uiPtr->m_inputRasterBandsListWidget->selectedItems();
      
    if( selectedBands.size() > 0 )
    {
      QList<QListWidgetItem *>::const_iterator itBegin = selectedBands.begin();
      QList<QListWidgetItem *>::const_iterator itEnd = selectedBands.end();
      while( itBegin != itEnd )
      {
        algoInputParams.m_inRasterBands.push_back( (*itBegin)->text().toUInt() );
        algoOutputParams.m_outRasterBands.push_back( (*itBegin)->text().toUInt() );
        ++itBegin;
      }
      
      if( m_uiPtr->m_contrastTypeComboBox->currentText() == 
        tr( "Histogram Equalization" ) )
      {
        algoInputParams.m_type = 
          te::rp::Contrast::InputParameters::HistogramEqualizationContrastT;
        algoInputParams.m_hECMaxInput.resize( selectedBands.size(), 
          m_uiPtr->m_hECMaxInputLineEdit->text().toDouble() );
      }
      else if( m_uiPtr->m_contrastTypeComboBox->currentText() == 
        tr( "Linear" ) )
      {
        algoInputParams.m_type = 
          te::rp::Contrast::InputParameters::LinearContrastT;
        algoInputParams.m_lCMinInput.resize( selectedBands.size(),
          m_uiPtr->m_lCMinInputLineEdit->text().toDouble() );
        algoInputParams.m_lCMaxInput.resize( selectedBands.size(),
          m_uiPtr->m_lCMaxInputLineEdit->text().toDouble() );
      }
      else if( m_uiPtr->m_contrastTypeComboBox->currentText() == 
        tr( "Mean and standard deviation" ) )
      {
        algoInputParams.m_type = 
          te::rp::Contrast::InputParameters::SetMeanAndStdContrastT;
        algoInputParams.m_sMASCMeanInput.resize( selectedBands.size(),
          m_uiPtr->m_sMASCMeanInputLineEdit->text().toDouble() );
        algoInputParams.m_sMASCStdInput.resize( selectedBands.size(),
          m_uiPtr->m_sMASCStdInputLineEdit->text().toDouble() );
      }
      
      if( m_outputRasterIsInputRaster )
      {
        algoOutputParams.m_outRasterPtr = (te::rst::Raster*)m_inputRasterPtr;
      }
      else
      {
        algoOutputParams.m_createdOutRasterDSType = m_outpuRasterDSType;
        algoOutputParams.m_createdOutRasterInfo = m_outpuRasterInfo;
      }
      
      // Executing the algorithm
      
      te::rp::Contrast algorithmInstance;
      
      if( algorithmInstance.initialize( algoInputParams ) )
      {
        if( algorithmInstance.execute( algoOutputParams ) )
        {
          if( ! m_outputRasterIsInputRaster )
          {
            m_outputRasterPtr = algoOutputParams.m_createdOutRasterPtr;
          }          
          
          QMessageBox::information(this, "", tr("Contrast enhencement ended sucessfully"));
        }
        else
        {
          QMessageBox::critical(this, "", tr("Contrast enhencement execution error"));
        }
      }
      else
      {
        QMessageBox::critical(this, "", tr("Contrast enhencement initialization error"));
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
