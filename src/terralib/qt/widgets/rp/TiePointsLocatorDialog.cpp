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
  \file terralib/widgets/rp/TiePointsLocatorDialog.cpp

  \brief A dialog used to execute tie points location.
*/


#include "TiePointsLocatorDialog.h"
#include "TiePointsLocatorAdvancedDialog.h"
#include "../Exception.h"

#include <ui_TiePointsLocatorForm.h>

#include <QtCore/QString>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      namespace rp
      {
        TiePointsLocatorDialog::TiePointsLocatorDialog(
          te::rst::Raster const* inRaster1Ptr, te::rst::Raster const* inRaster2Ptr,
          QWidget* parent, Qt::WindowFlags f )
          : QDialog( parent, f ),
            m_inRaster1Ptr( inRaster1Ptr ),
            m_inRaster2Ptr( inRaster2Ptr )
        {
          if( inRaster1Ptr == 0 ) throw te::qt::widgets::Exception( 
            "Invalid raster pointer" );
          if( inRaster2Ptr == 0 ) throw te::qt::widgets::Exception( 
            "Invalid raster pointer" );  
            
          m_uiPtr = new Ui::TiePointsLocatorForm;
          m_uiPtr->setupUi(this);
          
          m_advDialogPtr = new TiePointsLocatorAdvancedDialog( this );
          m_advDialogPtr->m_inputParameters.m_inMaskRaster1Ptr = inRaster1Ptr;
          m_advDialogPtr->m_inputParameters.m_inMaskRaster2Ptr = inRaster2Ptr;
          
          // Signals & slots
          connect(m_uiPtr->m_okPushButton, SIGNAL(clicked()), SLOT(on_okPushButton_clicked()));
          connect(m_uiPtr->m_acquireTiePointsPushButton, SIGNAL(clicked()), SLOT(on_acquireTiePointsPushButton_clicked()));
          connect(m_uiPtr->m_selectAllPushButton, SIGNAL(clicked()), SLOT(on_selectAllPushButton_clicked()));   
          connect(m_uiPtr->m_unselectAllPushButton, SIGNAL(clicked()), SLOT(on_unselectAllPushButton_clicked()));
          connect(m_uiPtr->m_deleteSelectedPushButton, SIGNAL(clicked()), SLOT(on_deleteSelectedPushButton_clicked()));
          connect(m_uiPtr->m_advancedOptionsPushButton, SIGNAL(clicked()), SLOT(on_advancedOptionsPushButton_clicked()));
          
          // fill form
          
          for( unsigned band1Idx = 0 ; band1Idx < inRaster1Ptr->getNumberOfBands() ;
            ++band1Idx )
            m_uiPtr->m_referenceBand1ComboBox->addItem( QString::number( band1Idx ) );
          
          for( unsigned band2Idx = 0 ; band2Idx < inRaster2Ptr->getNumberOfBands() ;
            ++band2Idx )
            m_uiPtr->m_referenceBand1ComboBox->addItem( QString::number( band2Idx ) );  
        }

        TiePointsLocatorDialog::~TiePointsLocatorDialog()
        {
          delete m_uiPtr;
        }


        void TiePointsLocatorDialog::on_okPushButton_clicked()
        {

        }
        
        void TiePointsLocatorDialog::on_acquireTiePointsPushButton_clicked()
        {
        }
        
        void TiePointsLocatorDialog::on_selectAllPushButton_clicked()
        {
        }
        
        void TiePointsLocatorDialog::on_unselectAllPushButton_clicked()
        {
        }
        
        void TiePointsLocatorDialog::on_deleteSelectedPushButton_clicked()
        {
        }
        
        void TiePointsLocatorDialog::on_advancedOptionsPushButton_clicked()
        {
          m_advDialogPtr->exec();
        }
      }
    }
  }
}

