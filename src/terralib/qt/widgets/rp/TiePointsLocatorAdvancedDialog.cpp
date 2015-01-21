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
  \file terralib/widgets/rp/TiePointsLocatorAdvancedDialog.cpp

  \brief A dialog used to execute tie points location advanced options.
*/


#include "TiePointsLocatorAdvancedDialog.h"
#include "../Exception.h"
#include "../../../geometry/GTFactory.h"

#include <ui_TiePointsLocatorAdvancedForm.h>

#include <QtCore/QString>
#include <QCheckBox>
#include <QComboBox>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      TiePointsLocatorAdvancedDialog::TiePointsLocatorAdvancedDialog(
        QWidget* parent, Qt::WindowFlags f )
        : QDialog( parent, f )
      {
        m_uiPtr = new Ui::TiePointsLocatorAdvancedForm;
        m_uiPtr->setupUi(this);
        
        // Signals & slots
        connect(m_uiPtr->m_okPushButton, SIGNAL(clicked()), this, SLOT(on_okPushButton_clicked()));   
        
        // fill form
        

      }

      TiePointsLocatorAdvancedDialog::~TiePointsLocatorAdvancedDialog()
      {
        delete m_uiPtr;
      }

      void TiePointsLocatorAdvancedDialog::showEvent( QShowEvent* )
      {
        m_uiPtr->m_enableGeometryFilterCheckBox->setChecked( 
          m_inputParameters.m_enableGeometryFilter );
          
        m_uiPtr->m_enableMultiThreadCheckBox->setChecked(
          m_inputParameters.m_enableMultiThread);
          
        switch( m_inputParameters.m_interesPointsLocationStrategy )
        {
          case te::rp::TiePointsLocator::InputParameters::SurfStrategyT :
          {
            m_uiPtr->m_interesPointsLocationStrategyComboBox->setCurrentIndex(
              m_uiPtr->m_interesPointsLocationStrategyComboBox->findText( "Surf" ) );
            break;
          }
          default:
          {
            m_uiPtr->m_interesPointsLocationStrategyComboBox->setCurrentIndex(
              m_uiPtr->m_interesPointsLocationStrategyComboBox->findText( "Moravec" ) );
          }
        }
        
        te::gm::GTFactory::dictionary_type::const_iterator gtItB = 
          te::gm::GTFactory::getDictionary().begin();
        const te::gm::GTFactory::dictionary_type::const_iterator gtItE = 
          te::gm::GTFactory::getDictionary().end();
        while( gtItB != gtItE )
        {
          m_uiPtr->m_geomTransfNameComboBox->addItem( QString( gtItB->first.c_str() ) );
          ++gtItB;
        }
        m_uiPtr->m_geomTransfNameComboBox->setCurrentIndex(
          m_uiPtr->m_geomTransfNameComboBox->findText( 
          m_inputParameters.m_geomTransfName.c_str() ) );        
        
        m_uiPtr->m_geometryFilterAssuranceLineEdit->setText( QString::number( 
          m_inputParameters.m_geometryFilterAssurance ) );
        
        m_uiPtr->m_geomTransfMaxErrorLineEdit->setText( QString::number(
          m_inputParameters.m_geomTransfMaxError ) );
          
        switch( m_inputParameters.m_interpMethod )
        {
          case te::rst::Bilinear :
          {
            m_uiPtr->m_interpMethodComboBox->setCurrentIndex(
              m_uiPtr->m_interpMethodComboBox->findText( "Bilinear" ) );
            break;
          }
          case te::rst::Bicubic :
          {
            m_uiPtr->m_interpMethodComboBox->setCurrentIndex(
              m_uiPtr->m_interpMethodComboBox->findText( "Bicubic" ) );
            break;
          }
          default:
          {
            m_uiPtr->m_interpMethodComboBox->setCurrentIndex(
              m_uiPtr->m_interpMethodComboBox->findText( "NearestNeighbor" ) );
          }
        } 
          
        m_uiPtr->m_maxTiePointsLineEdit->setText( QString::number(
          m_inputParameters.m_maxTiePoints ) );
          
        m_uiPtr->m_correlationWindowWidthLineEdit->setText( QString::number(
          m_inputParameters.m_moravecCorrelationWindowWidth ) );
          
        m_uiPtr->m_gaussianFilterIterationsLineEdit->setText( QString::number(
          m_inputParameters.m_moravecNoiseFilterIterations ) );
          
        m_uiPtr->m_minAbsCorrelationLineEdit->setText( QString::number(
          m_inputParameters.m_moravecMinAbsCorrelation ) );
          
        m_uiPtr->m_moravecWindowWidthLineEdit->setText( QString::number(
          m_inputParameters.m_moravecWindowWidth ) );
          
        m_uiPtr->m_maxNormEuclideanDistLineEdit->setText( QString::number(
          m_inputParameters.m_surfMaxNormEuclideanDist ) );
          
        m_uiPtr->m_octavesNumberLineEdit->setText( QString::number(
          m_inputParameters.m_surfOctavesNumber ) );
          
        m_uiPtr->m_scalesNumberLineEdit->setText( QString::number(
          m_inputParameters.m_surfScalesNumber ) );
      }

      void TiePointsLocatorAdvancedDialog::on_okPushButton_clicked()
      {
        m_inputParameters.m_enableGeometryFilter = 
          m_uiPtr->m_enableGeometryFilterCheckBox->isChecked();
          
        m_inputParameters.m_enableMultiThread = 
          m_uiPtr->m_enableMultiThreadCheckBox->isChecked();
          
        if( m_uiPtr->m_interesPointsLocationStrategyComboBox->currentText() == 
          "Surf" )
        {
          m_inputParameters.m_interesPointsLocationStrategy =
            te::rp::TiePointsLocator::InputParameters::SurfStrategyT;
        }
        else
        {
          m_inputParameters.m_interesPointsLocationStrategy =
            te::rp::TiePointsLocator::InputParameters::MoravecStrategyT;
        }
        
        m_inputParameters.m_geomTransfName = 
          m_uiPtr->m_geomTransfNameComboBox->currentText().toStdString();
          
        m_inputParameters.m_geometryFilterAssurance = 
          m_uiPtr->m_geometryFilterAssuranceLineEdit->text().toDouble();
          
        m_inputParameters.m_geomTransfMaxError = 
          m_uiPtr->m_geomTransfMaxErrorLineEdit->text().toDouble();
          
        if( m_uiPtr->m_interpMethodComboBox->currentText() == "Bilinear" )
        {
          m_inputParameters.m_interpMethod = te::rst::Bilinear;
        }
        else if( m_uiPtr->m_interpMethodComboBox->currentText() == "Bicubic" )
        {
          m_inputParameters.m_interpMethod = te::rst::Bicubic;
        }
        else
        {
          m_inputParameters.m_interpMethod = te::rst::NearestNeighbor;
        }
        
        m_inputParameters.m_maxTiePoints =  
          m_uiPtr->m_maxTiePointsLineEdit->text().toUInt();
          
        m_inputParameters.m_moravecCorrelationWindowWidth = 
          m_uiPtr->m_correlationWindowWidthLineEdit->text().toUInt();
          
        m_inputParameters.m_moravecNoiseFilterIterations = 
          m_uiPtr->m_gaussianFilterIterationsLineEdit->text().toUInt();
          
        m_inputParameters.m_moravecMinAbsCorrelation = 
          m_uiPtr->m_minAbsCorrelationLineEdit->text().toDouble();
          
        m_inputParameters.m_moravecWindowWidth =
           m_uiPtr->m_moravecWindowWidthLineEdit->text().toUInt();
           
        m_inputParameters.m_surfMaxNormEuclideanDist =
          m_uiPtr->m_maxNormEuclideanDistLineEdit->text().toDouble();
          
        m_inputParameters.m_surfOctavesNumber = 
          m_uiPtr->m_octavesNumberLineEdit->text().toUInt();
          
        m_inputParameters.m_surfScalesNumber =
          m_uiPtr->m_scalesNumberLineEdit->text().toUInt();
          
        hide();
      }
    }
  }
}

