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


//Terralib
#include "RasterSlicingWizardPage.h"
#include "ui_RasterSlicingWizardPageForm.h"

// QT
#include <QComboBox>
#include <QString>

te::qt::widgets::RasterSlicingWizardPage::RasterSlicingWizardPage(QWidget *parent) :
  QWizardPage(parent)
{
  m_ui.reset(new Ui::RasterSlicingWizardPageForm);

  m_ui->setupUi(this);

//configure page
  this->setTitle(tr("RasterSlicing"));
  this->setSubTitle(tr("Define the raster slicing parameters."));
}

te::qt::widgets::RasterSlicingWizardPage::~RasterSlicingWizardPage()
{
}

void te::qt::widgets::RasterSlicingWizardPage::setBandsNumber( const unsigned int bandsNumber )
{
  m_ui->_m_bandComboBox->clear();
  for( unsigned int band = 0 ; band < bandsNumber ; ++band )
  {
    m_ui->_m_bandComboBox->addItem( QString::number( band ) );
  }
}

void te::qt::widgets::RasterSlicingWizardPage::getParameters( 
  unsigned int& inputRasterBand, bool& createPaletteRaster,
  unsigned int& slicesNumber, bool& eqHistogram )
{
  inputRasterBand = (unsigned int)m_ui->_m_bandComboBox->currentText().toUInt();
  createPaletteRaster = m_ui->m_createPaletteRasterCheckBox->isChecked();
  slicesNumber = (unsigned int)m_ui->m_slicesNumberSpinBox->value();
  eqHistogram = m_ui->m_equalizeHistogramCheckBox->isChecked();  
}


