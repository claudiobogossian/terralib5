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
#include "../canvas/MapDisplay.h"
#include "../../widgets/tools/ZoomLeftAndRightClick.h"
#include "../../widgets/tools/CoordTracking.h"
#include "../../../raster/Grid.h"
#include "../../../geometry/GTFactory.h"

#include <ui_TiePointsLocatorForm.h>

#include <QtCore/QString>
#include <QtGui/QGridLayout>
#include <QtGui/QKeyEvent>
#include <QtGui/QLineEdit>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      namespace rp
      {
        TiePointsLocatorDialogMDEventFilter::TiePointsLocatorDialogMDEventFilter(
          te::qt::widgets::MapDisplay* parent )
          : QObject( parent ), m_mDisplay( parent )
        {
        }
        
        TiePointsLocatorDialogMDEventFilter::~TiePointsLocatorDialogMDEventFilter()
        {
        }
        
        bool TiePointsLocatorDialogMDEventFilter::eventFilter( QObject * watched, 
          QEvent * event )
        {
          if( event->type() == QEvent::Enter )
          {  
            m_mDisplay->grabKeyboard();
          }
          else if( event->type() == QEvent::Leave )
          {  
            m_mDisplay->releaseKeyboard();
          }          
          else if( event->type() == QEvent::KeyPress )
          {
            if( m_mDisplay->underMouse() )
            {
              emit( keyPressedOverMapDisplay( ((QKeyEvent*)event)->key() ) );
            }
          }
            
          return false;
        }        
        
        TiePointsLocatorDialog::TiePointsLocatorDialog(
          te::map::RasterLayer const* inRasterLayer1Ptr,
          te::map::RasterLayer const* inRasterLayer2Ptr,
          QWidget* parent, Qt::WindowFlags f )
          : QDialog( parent, f ),
            m_inRasterLayer1Ptr( inRasterLayer1Ptr ),
            m_inRasterLayer2Ptr( inRasterLayer2Ptr ),
            m_lastSelectedTiePointHasFirstOk( false )
        {
          if( inRasterLayer1Ptr == 0 ) throw te::qt::widgets::Exception( 
            "Invalid raster layer pointer" );
          if( inRasterLayer2Ptr == 0 ) throw te::qt::widgets::Exception( 
            "Invalid raster layer pointer" );  
            
          m_uiPtr = new Ui::TiePointsLocatorForm;
          m_uiPtr->setupUi(this);
          
          m_advDialogPtr = new TiePointsLocatorAdvancedDialog( this );
          m_advDialogPtr->m_inputParameters.m_inMaskRaster1Ptr = m_inRasterLayer1Ptr->getRaster();
          m_advDialogPtr->m_inputParameters.m_inMaskRaster2Ptr = m_inRasterLayer2Ptr->getRaster();
          
          QGridLayout* gridLayout1 = new QGridLayout( m_uiPtr->m_image1Frame );
          QGridLayout* gridLayout2 = new QGridLayout( m_uiPtr->m_image2Frame );
          
          // map display 1
          
          m_mapDisplay1 = new te::qt::widgets::MapDisplay( 
            m_uiPtr->m_image1Frame->size(),  m_uiPtr->m_image1Frame );
          m_mapDisplay1->setMouseTracking ( true );
          gridLayout1->addWidget( m_mapDisplay1 );  
          m_mapDisplay1->setResizePolicy(te::qt::widgets::MapDisplay::Center);
          std::list< te::map::AbstractLayer* >  layerList1;  
          layerList1.push_back( (te::map::RasterLayer*)inRasterLayer1Ptr );
          m_mapDisplay1->setLayerList( layerList1 );
          m_mapDisplay1->setSRID(m_inRasterLayer1Ptr->getRaster()->getSRID());
          m_mapDisplay1->setExtent( *(m_inRasterLayer1Ptr->getRaster()->getExtent()) );
          
          // map display 2
            
          m_mapDisplay2 = new te::qt::widgets::MapDisplay( 
            m_uiPtr->m_image2Frame->size(),  m_uiPtr->m_image2Frame );
          m_mapDisplay2->setMouseTracking ( true );
          gridLayout2->addWidget( m_mapDisplay2 );  
          m_mapDisplay2->setResizePolicy(te::qt::widgets::MapDisplay::Center);
          std::list< te::map::AbstractLayer* >  layerList2;  
          layerList2.push_back( (te::map::RasterLayer*)inRasterLayer2Ptr );
          m_mapDisplay2->setLayerList( layerList2 );
          m_mapDisplay2->setSRID(m_inRasterLayer2Ptr->getRaster()->getSRID());
          m_mapDisplay2->setExtent( *(m_inRasterLayer2Ptr->getRaster()->getExtent()) );       
          
          // Events
          
          m_zoomClickEvent1 = new te::qt::widgets::ZoomLeftAndRightClick( m_mapDisplay1, 1.5, m_mapDisplay1 );
          m_zoomClickEvent2 = new te::qt::widgets::ZoomLeftAndRightClick( m_mapDisplay2, 1.5, m_mapDisplay2 );
          
          m_coordTracking1 = new te::qt::widgets::CoordTracking( m_mapDisplay1, m_mapDisplay1 );
          m_coordTracking2 = new te::qt::widgets::CoordTracking( m_mapDisplay2, m_mapDisplay2 );
          
          m_mDEventFilter1 = new TiePointsLocatorDialogMDEventFilter( m_mapDisplay1 );
          m_mDEventFilter2 = new TiePointsLocatorDialogMDEventFilter( m_mapDisplay2 );
                    
          m_mapDisplay1->installEventFilter( m_zoomClickEvent1 );
          m_mapDisplay2->installEventFilter( m_zoomClickEvent2 );
          
          m_mapDisplay1->installEventFilter( m_coordTracking1 );
          m_mapDisplay2->installEventFilter( m_coordTracking2 );          
          
          m_mapDisplay1->installEventFilter( m_mDEventFilter1 );
          m_mapDisplay2->installEventFilter( m_mDEventFilter2 );
          
          
          // Signals & slots
          
          connect(m_uiPtr->m_okPushButton, SIGNAL(clicked()), this, SLOT(on_okPushButton_clicked()));
          connect(m_uiPtr->m_autoAcquireTiePointsPushButton, SIGNAL(clicked()), this, SLOT(on_autoAcquireTiePointsPushButton_clicked()));
          connect(m_uiPtr->m_selectAllPushButton, SIGNAL(clicked()), this, SLOT(on_selectAllPushButton_clicked()));   
          connect(m_uiPtr->m_unselectAllPushButton, SIGNAL(clicked()), this, SLOT(on_unselectAllPushButton_clicked()));
          connect(m_uiPtr->m_deleteSelectedPushButton, SIGNAL(clicked()), this, SLOT(on_deleteSelectedPushButton_clicked()));
          connect(m_uiPtr->m_advancedOptionsPushButton, SIGNAL(clicked()), this, SLOT(on_advancedOptionsPushButton_clicked()));
          connect(m_uiPtr->m_addPushButton, SIGNAL(clicked()), this, SLOT(on_addPushButton_clicked()));
          connect(m_coordTracking1, SIGNAL(coordTracked(QPointF&)), this, SLOT(on_coordTracked1(QPointF&)));
          connect(m_coordTracking2, SIGNAL(coordTracked(QPointF&)), this, SLOT(on_coordTracked2(QPointF&)));
          connect(m_mDEventFilter1, SIGNAL(keyPressedOverMapDisplay(int)), this, SLOT(on_keyPressedOverMapDisplay1(int)));
          connect(m_mDEventFilter2, SIGNAL(keyPressedOverMapDisplay(int)), this, SLOT(on_keyPressedOverMapDisplay2(int)));
          
          // fill form
          
          for( unsigned band1Idx = 0 ; band1Idx < m_inRasterLayer1Ptr->getRaster()->getNumberOfBands() ;
            ++band1Idx )
            m_uiPtr->m_referenceBand1ComboBox->addItem( QString::number( band1Idx ) );
          
          for( unsigned band2Idx = 0 ; band2Idx < m_inRasterLayer2Ptr->getRaster()->getNumberOfBands() ;
            ++band2Idx )
            m_uiPtr->m_referenceBand2ComboBox->addItem( QString::number( band2Idx ) );  
        }

        TiePointsLocatorDialog::~TiePointsLocatorDialog()
        {
          m_mapDisplay1->releaseKeyboard();
          m_mapDisplay2->releaseKeyboard();
          
          delete m_uiPtr;
        }

        void TiePointsLocatorDialog::on_okPushButton_clicked()
        {

        }
        
        void TiePointsLocatorDialog::on_autoAcquireTiePointsPushButton_clicked()
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
        
        void TiePointsLocatorDialog::on_addPushButton_clicked()
        {
          
        }
        
        void TiePointsLocatorDialog::on_keyPressedOverMapDisplay1( int key )
        {
          m_lastSelectedTiePoint.first = m_lastTrackedTiePoint.first;
          m_lastSelectedTiePointHasFirstOk = true;
        }    
        
        void TiePointsLocatorDialog::on_keyPressedOverMapDisplay2( int key )
        {
          if( m_lastSelectedTiePointHasFirstOk )
          {
            m_lastSelectedTiePointHasFirstOk = false;
            m_lastSelectedTiePoint.second = m_lastTrackedTiePoint.second;
            
            unsigned int tpID = ( (unsigned int)m_tiePoints.size() ) + 1;
            
            m_tiePoints[ tpID ] = m_lastSelectedTiePoint;
            
            update();
          }
        }        
        
        
        void TiePointsLocatorDialog::on_coordTracked1( QPointF& coordinate )
        {
          m_lastTrackedTiePoint.first = m_inRasterLayer1Ptr->getRaster()->getGrid()->geoToGrid( 
            (double)coordinate.rx(), (double)coordinate.ry() );
          
          m_uiPtr->m_currentImage1LineLineEdit->setText( QString::number( 
            m_lastTrackedTiePoint.first.y ) );
          m_uiPtr->m_currentImage1ColumnLineEdit->setText( QString::number( 
            m_lastTrackedTiePoint.first.x ) );            
        }          
        
        void TiePointsLocatorDialog::on_coordTracked2( QPointF& coordinate )
        {
          m_lastTrackedTiePoint.second = m_inRasterLayer1Ptr->getRaster()->getGrid()->geoToGrid( 
            (double)coordinate.rx(), (double)coordinate.ry() );
          
          m_uiPtr->m_currentImage2LineLineEdit->setText( QString::number( 
            m_lastTrackedTiePoint.second.y ) );
          m_uiPtr->m_currentImage2ColumnLineEdit->setText( QString::number( 
            m_lastTrackedTiePoint.second.x ) );
        }     
        
        void TiePointsLocatorDialog::update()
        {
          // building the geometric transformations
          
          te::gm::GTParameters transParams;
                    
          TPContainerT::const_iterator tPIt = m_tiePoints.begin();
          const TPContainerT::const_iterator tPItEnd = m_tiePoints.end(); 
          
          while( tPIt != tPItEnd )   
          {
            transParams.m_tiePoints.push_back( tPIt->second );
            ++tPIt;
          }
          
          std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
            te::gm::GTFactory::make( m_advDialogPtr->m_inputParameters.m_geomTransfName ) );
            
          if( transfPtr.get() ) 
          {
            if( ! transfPtr->initialize( transParams ) )
              transfPtr.reset();
          }
          
          // updating the tie points table
          
          tPIt = m_tiePoints.begin();
          unsigned int currentRow = 0;
          m_uiPtr->m_tiePointsTableWidget->clearContents ();
          m_uiPtr->m_tiePointsTableWidget->setRowCount( (int)m_tiePoints.size() );
          double currTPError = 0;
          
          while( tPIt != tPItEnd )
          {
            const te::gm::GTParameters::TiePoint& currTP = tPIt->second;
            currTPError = transfPtr.get() ? transfPtr->getDirectMappingError( 
              currTP ) : 0.0;
            
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 0, new
              QTableWidgetItem( QString::number( tPIt->first ) ) );
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 1, new
              QTableWidgetItem( QString::number( currTPError ) ) );              
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 2, new
              QTableWidgetItem( QString::number( currTP.first.x ) ) );              
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 3, new
              QTableWidgetItem( QString::number( currTP.first.y ) ) );              
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 4, new
              QTableWidgetItem( QString::number( currTP.second.x ) ) );              
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 5, new
              QTableWidgetItem( QString::number( currTP.second.y ) ) );              
            
            ++currentRow;
            ++tPIt;
          }
        }

        
      }
    }
  }
}

