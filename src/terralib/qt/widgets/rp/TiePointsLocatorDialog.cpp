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
  \file terralib/widgets/rp/TiePointsLocatorDialog.cpp

  \brief A dialog used to execute tie points location.
*/

#include "TiePointsLocatorDialog.h"
#include "TiePointsLocatorAdvancedDialog.h"
#include "../Exception.h"
#include "../canvas/MapDisplay.h"
#include "../canvas/Canvas.h"
#include "../../widgets/tools/ZoomLeftAndRightClick.h"
#include "../../widgets/tools/CoordTracking.h"
#include "../../../raster/Grid.h"
#include "../../../geometry/GTFactory.h"
#include "../../../geometry/Point.h"
#include "../../../geometry/Envelope.h"
#include "../../../maptools/Utils.h"
#include "../../../rp/TiePointsLocator.h"

#include <ui_TiePointsLocatorForm.h>

#include <QtCore/QString>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QColor>

#define TPLDIALOG_P_PATTERN_W 11

namespace te
{
  namespace qt
  {
    namespace widgets
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
            emit( keyPressed( ((QKeyEvent*)event)->key() ) );
          }
        }
          
        return false;
      }        
      
      TiePointsLocatorDialog::TiePointData::TiePointData()
      : m_acqType( InvalidAcquisitionT )
      {
      }
      
      TiePointsLocatorDialog::TiePointData::TiePointData( 
        const TiePointsLocatorDialog::TiePointData& other )
      {
        operator=( other );
      }      
      
      TiePointsLocatorDialog::TiePointData::~TiePointData()
      {
      }
      
      const TiePointsLocatorDialog::TiePointData& TiePointsLocatorDialog::TiePointData::operator=( 
        const TiePointsLocatorDialog::TiePointData& other )
      {
        m_acqType = other.m_acqType;
        m_tiePoint = other.m_tiePoint;
        return other;
      }
      
      TiePointsLocatorDialog::TiePointsLocatorDialog(
        const te::map::DataSetLayerPtr& inLayer1Ptr,
        const te::map::DataSetLayerPtr& inLayer2Ptr,
        QWidget* parent, Qt::WindowFlags f )
        : QDialog( parent, f ),
          m_inLayer1Ptr( inLayer1Ptr ),
          m_inLayer2Ptr( inLayer2Ptr ),
          m_lastSelectedTiePointHasFirstOk( false ),
          m_lastInsertedTPID( 0 )
      {
        if( inLayer1Ptr.get() == 0 ) throw te::qt::widgets::Exception( 
          "Invalid data set layer pointer" );
        if( inLayer1Ptr.get() == 0 ) throw te::qt::widgets::Exception( 
          "Invalid data set layer pointer" );
          
        m_uiPtr = new Ui::TiePointsLocatorForm;
        m_uiPtr->setupUi(this);
        
        m_raster1 = te::map::GetRaster(m_inLayer1Ptr.get());
        m_raster2 = te::map::GetRaster(m_inLayer2Ptr.get());

        m_advDialogPtr = new TiePointsLocatorAdvancedDialog( this );
        m_advDialogPtr->m_inputParameters.m_inMaskRaster1Ptr = m_raster1;
        m_advDialogPtr->m_inputParameters.m_inMaskRaster2Ptr = m_raster2;
        
        QGridLayout* gridLayout1 = new QGridLayout( m_uiPtr->m_image1Frame );
        QGridLayout* gridLayout2 = new QGridLayout( m_uiPtr->m_image2Frame );
        
        // map display 1
        
        m_mapDisplay1 = new te::qt::widgets::MapDisplay( 
          m_uiPtr->m_image1Frame->size(),  m_uiPtr->m_image1Frame );
        m_mapDisplay1->setMouseTracking ( true );
        gridLayout1->addWidget( m_mapDisplay1 );  
        m_mapDisplay1->setResizePolicy(te::qt::widgets::MapDisplay::Center);
        std::list< te::map::AbstractLayerPtr >  layerList1;  
        layerList1.push_back( m_inLayer1Ptr );
        m_mapDisplay1->setLayerList( layerList1 );
        m_mapDisplay1->setSRID(m_raster1->getSRID());
        te::gm::Envelope displayExtent1(*(m_raster1->getExtent()));
        m_mapDisplay1->setExtent( displayExtent1 );
        
        // map display 2
          
        m_mapDisplay2 = new te::qt::widgets::MapDisplay( 
          m_uiPtr->m_image2Frame->size(),  m_uiPtr->m_image2Frame );
        m_mapDisplay2->setMouseTracking ( true );
        gridLayout2->addWidget( m_mapDisplay2 );  
        m_mapDisplay2->setResizePolicy(te::qt::widgets::MapDisplay::Center);
        std::list< te::map::AbstractLayerPtr >  layerList2;  
        layerList2.push_back( m_inLayer2Ptr );
        m_mapDisplay2->setLayerList( layerList2 );
        m_mapDisplay2->setSRID(m_raster2->getSRID());
        te::gm::Envelope displayExtent2(*(m_raster2->getExtent()));
        m_mapDisplay1->setExtent( displayExtent2 );
        
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
        connect(m_coordTracking1, SIGNAL(coordTracked(QPointF&)), this, SLOT(on_mapDisplay1_coordTracked(QPointF&)));
        connect(m_coordTracking2, SIGNAL(coordTracked(QPointF&)), this, SLOT(on_mapDisplay2_coordTracked(QPointF&)));
        connect(m_mDEventFilter1, SIGNAL(keyPressed(int)), this, SLOT(on_mapDisplay1_keyPressed(int)));
        connect(m_mDEventFilter2, SIGNAL(keyPressed(int)), this, SLOT(on_mapDisplay2_keyPressed(int)));
        connect(m_uiPtr->m_tiePointsTableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(on_tiePointsTableWidget_itemSelectionChanged()));
        connect(m_mapDisplay1, SIGNAL(extentChanged()), this, SLOT(on_mapDisplay1_extentChanged()));
        connect(m_mapDisplay2, SIGNAL(extentChanged()), this, SLOT(on_mapDisplay2_extentChanged()));
        
        // fill form
        
        for( unsigned band1Idx = 0 ; band1Idx < m_raster1->getNumberOfBands() ;
          ++band1Idx )
          m_uiPtr->m_referenceBand1ComboBox->addItem( QString::number( band1Idx ) );
        
        for( unsigned band2Idx = 0 ; band2Idx < m_raster2->getNumberOfBands() ;
          ++band2Idx )
          m_uiPtr->m_referenceBand2ComboBox->addItem( QString::number( band2Idx ) );
        
        // create points draw patterns
        
        m_selectedPointPattern = new te::color::RGBAColor*[ TPLDIALOG_P_PATTERN_W ];
        m_unselectedPointPattern = new te::color::RGBAColor*[ TPLDIALOG_P_PATTERN_W ];
        m_tempPointPattern = new te::color::RGBAColor*[ TPLDIALOG_P_PATTERN_W ];
        
        for( unsigned int line = 0 ; line < TPLDIALOG_P_PATTERN_W ; ++line )
        {
          m_selectedPointPattern[ line ] = new te::color::RGBAColor[ TPLDIALOG_P_PATTERN_W ];
          m_unselectedPointPattern[ line ] = new te::color::RGBAColor[ TPLDIALOG_P_PATTERN_W ];
          m_tempPointPattern[ line ] = new te::color::RGBAColor[ TPLDIALOG_P_PATTERN_W ];
          
          for( unsigned int col = 0 ; col < TPLDIALOG_P_PATTERN_W ; ++col )
          {
            if( ( line == col ) || ( line == ( TPLDIALOG_P_PATTERN_W - col - 1 ) ) )
            {
              m_selectedPointPattern[ line ][ col ].setColor( 255, 0, 0, TE_OPAQUE );
              m_unselectedPointPattern[ line ][ col ].setColor( 0, 255, 0, TE_OPAQUE );
              m_tempPointPattern[ line ][ col ].setColor( 0, 0, 255, TE_OPAQUE );
            }
            else
            {
              m_selectedPointPattern[ line ][ col ].setColor( 0, 0, 0, TE_TRANSPARENT  );
              m_unselectedPointPattern[ line ][ col ].setColor( 0, 0, 0, TE_TRANSPARENT  );
              m_tempPointPattern[ line ][ col ].setColor( 0, 0, 0, TE_TRANSPARENT  );
            }
          }
        }
      }

      TiePointsLocatorDialog::~TiePointsLocatorDialog()
      {
        // assuring that the keyboard is released from the map displays
        
        m_mapDisplay1->releaseKeyboard();
        m_mapDisplay2->releaseKeyboard();

        delete m_raster1;
        delete m_raster2;
        
        // delete points draw patterns
        
        for( unsigned int line = 0 ; line < TPLDIALOG_P_PATTERN_W ; ++line )
        {
          delete[] m_selectedPointPattern[ line ];
          delete[] m_unselectedPointPattern[ line ];
          delete[] m_tempPointPattern[ line ];
        }
        
        delete[] m_selectedPointPattern;
        delete[] m_unselectedPointPattern;
        delete[] m_tempPointPattern;
        
        // detroying the UI
        
        delete m_uiPtr;
      }
      
      void TiePointsLocatorDialog::getTiePoints( std::vector< te::gm::GTParameters::TiePoint >&
        tiePoints ) const
      {
        tiePoints.clear();
        
        TPContainerT::const_iterator itB = m_tiePoints.begin();
        const TPContainerT::const_iterator itE = m_tiePoints.end();
        tiePoints.reserve( m_tiePoints.size() );
        
        while( itB != itE )
        {
          tiePoints.push_back( itB->second.m_tiePoint );
          ++itB;
        }
      }

      void TiePointsLocatorDialog::on_okPushButton_clicked()
      {
        close();
      }
      
      void TiePointsLocatorDialog::on_autoAcquireTiePointsPushButton_clicked()
      {
        // creating the algorithm parameters
        
        te::rp::TiePointsLocator::InputParameters inputParams = 
          m_advDialogPtr->m_inputParameters;
          
        inputParams.m_inRaster1Ptr = m_raster1;
        inputParams.m_inRaster2Ptr = m_raster2;
          
        te::gm::Envelope auxEnvelope1( m_mapDisplay1->getExtent() );
        double r1LLX = 0;
        double r1LLY = 0;
        double r1URX = 0;
        double r1URY = 0;
        inputParams.m_inRaster1Ptr->getGrid()->geoToGrid(
          auxEnvelope1.m_llx, auxEnvelope1.m_lly, r1LLX, r1LLY );
        inputParams.m_inRaster1Ptr->getGrid()->geoToGrid(
          auxEnvelope1.m_urx, auxEnvelope1.m_ury, r1URX, r1URY );
        inputParams.m_raster1TargetAreaColStart = (unsigned int)std::max( 0.0,
          r1LLX );
        inputParams.m_raster1TargetAreaLineStart = (unsigned int)std::max( 0.0,
          r1URY );
        inputParams.m_raster1TargetAreaWidth = ( (unsigned int)std::min(
          (double)inputParams.m_inRaster1Ptr->getNumberOfColumns(),
          r1URX ) ) - inputParams.m_raster1TargetAreaColStart + 1;
        inputParams.m_raster1TargetAreaHeight = ( (unsigned int)std::min(
          (double)inputParams.m_inRaster1Ptr->getNumberOfRows(),
          r1LLY ) ) - inputParams.m_raster1TargetAreaColStart + 1;
          
        te::gm::Envelope auxEnvelope2( m_mapDisplay2->getExtent() );
        double r2LLX = 0;
        double r2LLY = 0;
        double r2URX = 0;
        double r2URY = 0;
        inputParams.m_inRaster2Ptr->getGrid()->geoToGrid(
          auxEnvelope2.m_llx, auxEnvelope2.m_lly, r2LLX, r2LLY );
        inputParams.m_inRaster2Ptr->getGrid()->geoToGrid(
          auxEnvelope2.m_urx, auxEnvelope2.m_ury, r2URX, r2URY );
        inputParams.m_raster2TargetAreaColStart = (unsigned int)std::max( 0.0,
          r2LLX );
        inputParams.m_raster2TargetAreaLineStart = (unsigned int)std::max( 0.0,
          r2URY );
        inputParams.m_raster2TargetAreaWidth = ( (unsigned int)std::min(
          (double)inputParams.m_inRaster2Ptr->getNumberOfColumns(),
          r2URX ) ) - inputParams.m_raster2TargetAreaColStart + 1;
        inputParams.m_raster2TargetAreaHeight = ( (unsigned int)std::min(
          (double)inputParams.m_inRaster2Ptr->getNumberOfRows(),
          r2LLY ) ) - inputParams.m_raster2TargetAreaColStart + 1;
          
        inputParams.m_inRaster1Bands.push_back( 
          m_uiPtr->m_referenceBand1ComboBox->currentText().toUInt() );
        inputParams.m_inRaster2Bands.push_back( 
          m_uiPtr->m_referenceBand2ComboBox->currentText().toUInt() );
          
        te::rp::TiePointsLocator::OutputParameters outputParams;
        
        // Looking for manual inserted tie-points for an initial estimate
        
        unsigned int manualTPNumber = 0;
        
        TPContainerT::const_iterator itB = m_tiePoints.begin();
        const TPContainerT::const_iterator itE = m_tiePoints.end();
        double coordDiffX = 0;
        double coordDiffY = 0;
        
        while( itB != itE )
        {
          if( itB->second.m_acqType == TiePointData::ManualAcquisitionT )
          {
            coordDiffX = itB->second.m_tiePoint.first.x -
              itB->second.m_tiePoint.second.x;
            coordDiffY = itB->second.m_tiePoint.first.y -
              itB->second.m_tiePoint.second.y;              
//             inputParams.m_maxR1ToR2Offset += std::max( inputParams.m_maxR1ToR2Offset,
//               (unsigned int)std::ceil( std::sqrt( ( coordDiffX * coordDiffX ) + 
//               ( coordDiffY * coordDiffY ) ) ) );
            ++manualTPNumber;
          }
          
          ++itB;
        }
        
//         if( inputParams.m_maxR1ToR2Offset > 0 )
//         {
//           inputParams.m_maxR1ToR2Offset /= manualTPNumber;
//           inputParams.m_maxR1ToR2Offset += ( ( inputParams.m_maxR1ToR2Offset * 10 )
//             / 100 );
//         }
        
        // Executing the algorithm
        
        te::rp::TiePointsLocator algorithmInstance;          
        
        if( algorithmInstance.initialize( inputParams ) )
        {
          if( algorithmInstance.execute( outputParams ) )
          {
            const unsigned int tpsNmb = (unsigned int)
              outputParams.m_tiePoints.size();
              
            if( tpsNmb )
            {
              TiePointData auxTpData;
              auxTpData.m_acqType = TiePointData::AutomaticAcquisitionT;
              
              for( unsigned int tpIdx = 0 ; tpIdx < tpsNmb ; ++tpIdx )
              {
                ++m_lastInsertedTPID;
                auxTpData.m_tiePoint = outputParams.m_tiePoints[ tpIdx ];
                m_tiePoints[ m_lastInsertedTPID ] = auxTpData;
              }
              
              tiePointsTableUpdate();
            }
          }
        }
      }
      
      void TiePointsLocatorDialog::on_selectAllPushButton_clicked()
      {
        m_uiPtr->m_tiePointsTableWidget->selectAll();
      }
      
      void TiePointsLocatorDialog::on_unselectAllPushButton_clicked()
      {
        m_uiPtr->m_tiePointsTableWidget->clearSelection();
      }
      
      void TiePointsLocatorDialog::on_deleteSelectedPushButton_clicked()
      {
        const int rowCount = 
          m_uiPtr->m_tiePointsTableWidget->rowCount();
        QTableWidgetItem* itemPtr = 0;
        unsigned int tpID = 0;
        TPContainerT::iterator deletionIt;
          
        for( int row = 0 ; row < rowCount ; ++row )
        {
          itemPtr = m_uiPtr->m_tiePointsTableWidget->item( row, 0 );
          
          if( itemPtr->isSelected() )
          {
            tpID = itemPtr->text().toUInt();
            deletionIt = m_tiePoints.find( tpID );
            assert( deletionIt != m_tiePoints.end() );
            m_tiePoints.erase( deletionIt );              
          }
        }
        
        if( m_tiePoints.empty() ) 
        {
          m_lastInsertedTPID = 0;
        }
        
        tiePointsTableUpdate();
      }
      
      void TiePointsLocatorDialog::on_advancedOptionsPushButton_clicked()
      {
        m_advDialogPtr->exec();
      }
      
      void TiePointsLocatorDialog::on_addPushButton_clicked()
      {
        if( ( ! m_uiPtr->m_x1LineEdit->text().isEmpty() ) &&
          ( ! m_uiPtr->m_y1LineEdit->text().isEmpty() ) &&
          ( ! m_uiPtr->m_x2LineEdit->text().isEmpty() ) &&
          ( ! m_uiPtr->m_y2LineEdit->text().isEmpty() ) )
        {
          TiePointData auxTpData;
          auxTpData.m_acqType = TiePointData::ManualAcquisitionT;          
          auxTpData.m_tiePoint.first.x = m_uiPtr->m_x1LineEdit->text().toDouble();
          auxTpData.m_tiePoint.first.y = m_uiPtr->m_y1LineEdit->text().toDouble();
          auxTpData.m_tiePoint.second.x = m_uiPtr->m_x2LineEdit->text().toDouble();
          auxTpData.m_tiePoint.second.y = m_uiPtr->m_y2LineEdit->text().toDouble();
          
          ++m_lastInsertedTPID;
          m_tiePoints[ m_lastInsertedTPID ] = auxTpData;
          
          tiePointsTableUpdate();
        }
      }
      
      void TiePointsLocatorDialog::on_mapDisplay1_keyPressed( int key )
      {
        m_lastSelectedTiePoint.first = m_lastTrackedTiePoint.first;
        m_lastSelectedTiePointHasFirstOk = true;
        
        refreshMapDisplay1();
      }    
      
      void TiePointsLocatorDialog::on_mapDisplay2_keyPressed( int key )
      {
        if( m_lastSelectedTiePointHasFirstOk )
        {
          m_lastSelectedTiePointHasFirstOk = false;
          
          m_lastSelectedTiePoint.second = m_lastTrackedTiePoint.second;
          
          TiePointData auxTpData;
          auxTpData.m_acqType = TiePointData::ManualAcquisitionT;
          auxTpData.m_tiePoint = m_lastSelectedTiePoint;
          
          ++m_lastInsertedTPID;
          m_tiePoints[ m_lastInsertedTPID ] = auxTpData;
          
          tiePointsTableUpdate();
        }
      }
      
      void TiePointsLocatorDialog::on_mapDisplay1_coordTracked( QPointF& coordinate )
      {
        m_lastTrackedTiePoint.first = m_raster1->getGrid()->geoToGrid( 
          (double)coordinate.rx(), (double)coordinate.ry() );
        
        m_uiPtr->m_currentImage1LineLineEdit->setText( QString::number( 
          m_lastTrackedTiePoint.first.y ) );
        m_uiPtr->m_currentImage1ColumnLineEdit->setText( QString::number( 
          m_lastTrackedTiePoint.first.x ) );            
      }          
      
      void TiePointsLocatorDialog::on_mapDisplay2_coordTracked( QPointF& coordinate )
      {
        m_lastTrackedTiePoint.second = m_raster2->getGrid()->geoToGrid( 
          (double)coordinate.rx(), (double)coordinate.ry() );
        
        m_uiPtr->m_currentImage2LineLineEdit->setText( QString::number( 
          m_lastTrackedTiePoint.second.y ) );
        m_uiPtr->m_currentImage2ColumnLineEdit->setText( QString::number( 
          m_lastTrackedTiePoint.second.x ) );
      }     
      
      void TiePointsLocatorDialog::tiePointsTableUpdate()
      {
        // building the geometric transformation
        
        te::gm::GTParameters transParams;
                  
        TPContainerT::const_iterator tPIt = m_tiePoints.begin();
        const TPContainerT::const_iterator tPItEnd = m_tiePoints.end(); 
        
        while( tPIt != tPItEnd )   
        {
          transParams.m_tiePoints.push_back( tPIt->second.m_tiePoint );
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
        
        m_uiPtr->m_tiePointsTableWidget->blockSignals( true );
        
        const int rowCount = m_uiPtr->m_tiePointsTableWidget->rowCount();
          
        for( int row = rowCount - 1 ; row >= 0 ; --row )
        {
          m_uiPtr->m_tiePointsTableWidget->removeRow( row );
        }          
        
        if( ! m_tiePoints.empty() )
        {
          tPIt = m_tiePoints.begin();
          int currentRow = 0;
          m_uiPtr->m_tiePointsTableWidget->setSortingEnabled( false );
          double currTPError = 0;
          QTableWidgetItem* newItemPtr = 0;
          
          while( tPIt != tPItEnd )
          {
            const te::gm::GTParameters::TiePoint& currTP = tPIt->second.m_tiePoint;
            currTPError = transfPtr.get() ? transfPtr->getDirectMappingError( 
              currTP ) : 0.0;
              
            currentRow = m_uiPtr->m_tiePointsTableWidget->rowCount();
              
            m_uiPtr->m_tiePointsTableWidget->insertRow( currentRow );
            
            newItemPtr = new QTableWidgetItem( QString::number( tPIt->first ) );
            newItemPtr->setFlags(  Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 0, newItemPtr );
            
            newItemPtr = new QTableWidgetItem( QString::number( currTPError ) );
            newItemPtr->setFlags(  Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 1, newItemPtr );
            
            if( tPIt->second.m_acqType == TiePointData::ManualAcquisitionT )
            {
              newItemPtr = new QTableWidgetItem( QString( tr( "Manual" ) ) );
            }
            else
            {
              newItemPtr = new QTableWidgetItem( QString( tr( "Automatic" ) ) );
            }
            newItemPtr->setFlags(  Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 2, newItemPtr );
            
            newItemPtr = new QTableWidgetItem( QString::number( currTP.first.x ) );
            newItemPtr->setFlags(  Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 3, newItemPtr );          
            
            newItemPtr = new QTableWidgetItem( QString::number( currTP.first.y ) );
            newItemPtr->setFlags(  Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 4, newItemPtr );      
            
            newItemPtr = new QTableWidgetItem( QString::number( currTP.second.x ) );
            newItemPtr->setFlags(  Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 5, newItemPtr );              
            
            newItemPtr = new QTableWidgetItem( QString::number( currTP.second.y ) );
            newItemPtr->setFlags(  Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            m_uiPtr->m_tiePointsTableWidget->setItem( currentRow, 6, newItemPtr );              
            
            ++tPIt;
          }
          
          m_uiPtr->m_tiePointsTableWidget->setSortingEnabled( true );
          m_uiPtr->m_tiePointsTableWidget->sortByColumn( 1, Qt::DescendingOrder );
        }
        
        m_uiPtr->m_tiePointsTableWidget->blockSignals( false );
        
        transformationInfoUpdate();
        refreshMapDisplay1();
        refreshMapDisplay2();
      }
      
      void TiePointsLocatorDialog::transformationInfoUpdate()
      {
        // creating the transformations parameters
        
        te::gm::GTParameters transParamsAllTP;
        te::gm::GTParameters transParamsSelectedTP;
        te::gm::GTParameters transParamsUnselectedTP;
        
        const int rowCount = m_uiPtr->m_tiePointsTableWidget->rowCount();
        QTableWidgetItem* itemPtr = 0;
        TPContainerT::const_iterator it;
          
        for( int row = 0 ; row < rowCount ; ++row )
        {
          itemPtr = m_uiPtr->m_tiePointsTableWidget->item( row, 0 );
          it = m_tiePoints.find( itemPtr->text().toUInt() );
          assert( it != m_tiePoints.end() );
          
          const te::gm::GTParameters::TiePoint& tiePoint = it->second.m_tiePoint;
          
          if( itemPtr->isSelected() )
          {
            transParamsSelectedTP.m_tiePoints.push_back( tiePoint );
          }
          else
          {
            transParamsUnselectedTP.m_tiePoints.push_back( tiePoint );
          }
          
          transParamsAllTP.m_tiePoints.push_back( tiePoint );
        }
        
        // instantiating the transformations
        
        std::auto_ptr< te::gm::GeometricTransformation > transfAllTPPtr( 
          te::gm::GTFactory::make( m_advDialogPtr->m_inputParameters.m_geomTransfName ) );
        if( transfAllTPPtr.get() ) 
        {
          if( ! transfAllTPPtr->initialize( transParamsAllTP ) )
            transfAllTPPtr.reset();
        }          
        
        std::auto_ptr< te::gm::GeometricTransformation > transfSelectedTPPtr( 
          te::gm::GTFactory::make( m_advDialogPtr->m_inputParameters.m_geomTransfName ) );
        if( transfSelectedTPPtr.get() ) 
        {
          if( ! transfSelectedTPPtr->initialize( transParamsSelectedTP ) )
            transfSelectedTPPtr.reset();
        }     
        
        std::auto_ptr< te::gm::GeometricTransformation > transfUnselectedTPPtr( 
          te::gm::GTFactory::make( m_advDialogPtr->m_inputParameters.m_geomTransfName ) );
        if( transfUnselectedTPPtr.get() ) 
        {
          if( ! transfUnselectedTPPtr->initialize( transParamsUnselectedTP ) )
            transfUnselectedTPPtr.reset();
        }     
        
        // updating widgets
        
        m_uiPtr->m_tiePointsNumberLineEdit->setText( QString::number( 
          m_tiePoints.size() ) );
        
        if( transfAllTPPtr.get() )
          m_uiPtr->m_transformationRMSEAllLineEdit->setText( QString::number( 
          transfAllTPPtr->getDirectMapRMSE() ) );
        else
          m_uiPtr->m_transformationRMSEAllLineEdit->setText("N/A");
        
        if( transfSelectedTPPtr.get() )
          m_uiPtr->m_transformationRMSESelectedLineEdit->setText( QString::number( 
          transfSelectedTPPtr->getDirectMapRMSE() ) );
        else
          m_uiPtr->m_transformationRMSESelectedLineEdit->setText("N/A");   
        
        if( transfUnselectedTPPtr.get() )
          m_uiPtr->m_transformationRMSEunselectedLineEdit->setText( QString::number( 
          transfUnselectedTPPtr->getDirectMapRMSE() ) );
        else
          m_uiPtr->m_transformationRMSEunselectedLineEdit->setText("N/A");     
      }
      
      void TiePointsLocatorDialog::refreshMapDisplay1()
      {
        te::gm::Envelope auxEnvelope( m_mapDisplay1->getExtent() );
        m_mapDisplay1->setExtent( auxEnvelope );
      }
      
      void TiePointsLocatorDialog::refreshMapDisplay2()
      {
        te::gm::Envelope auxEnvelope( m_mapDisplay2->getExtent() );
        m_mapDisplay2->setExtent( auxEnvelope );
      }
      
      void TiePointsLocatorDialog::on_tiePointsTableWidget_itemSelectionChanged()
      {
        transformationInfoUpdate();
        refreshMapDisplay1();
        refreshMapDisplay2();
      }
      
      void TiePointsLocatorDialog::on_mapDisplay1_extentChanged()
      {
        const te::gm::Envelope& mapDisplayExtent = 
          m_mapDisplay1->getExtent();
          
        m_mapDisplay1->getDraftPixmap()->fill( QColor( 0, 0, 0, 0 ) );
          
        te::qt::widgets::Canvas canvasInstance( m_mapDisplay1->getDraftPixmap() );
        canvasInstance.setWindow( mapDisplayExtent.m_llx,
          mapDisplayExtent.m_lly, mapDisplayExtent.m_urx,
          mapDisplayExtent.m_ury );
        
        // Drawing the colected points
        
        const int rowCount = 
          m_uiPtr->m_tiePointsTableWidget->rowCount();
        QTableWidgetItem* itemPtr = 0;
        unsigned int tpID = 0;
        TPContainerT::iterator tiePointsIt;
        te::gm::Point auxPoint;
        te::gm::Coord2D auxCoord2D;
          
        for( int row = 0 ; row < rowCount ; ++row )
        {
          itemPtr = m_uiPtr->m_tiePointsTableWidget->item( row, 0 );
          
          if( itemPtr->isSelected() )
          {
            canvasInstance.setPointPattern( m_selectedPointPattern, 
              TPLDIALOG_P_PATTERN_W, TPLDIALOG_P_PATTERN_W );
          }
          else
          {
            canvasInstance.setPointPattern( m_unselectedPointPattern, 
              TPLDIALOG_P_PATTERN_W, TPLDIALOG_P_PATTERN_W );
          }
          
          tpID = itemPtr->text().toUInt();
          
          tiePointsIt = m_tiePoints.find( tpID );
          assert( tiePointsIt != m_tiePoints.end() );
          
          m_raster1->getGrid()->gridToGeo( 
            tiePointsIt->second.m_tiePoint.first.x, tiePointsIt->second.m_tiePoint.first.y,
            auxCoord2D.x, auxCoord2D.y );
            
          auxPoint.setX( auxCoord2D.x );
          auxPoint.setY( auxCoord2D.y );
          
          canvasInstance.draw( &auxPoint );            
        }     
        
        // Drawing the selected temporary point
        
        if( m_lastSelectedTiePointHasFirstOk )
        {
          m_raster1->getGrid()->gridToGeo( 
            m_lastSelectedTiePoint.first.x, m_lastSelectedTiePoint.first.y,
            auxCoord2D.x, auxCoord2D.y );
            
          auxPoint.setX( auxCoord2D.x );
          auxPoint.setY( auxCoord2D.y );
        
          canvasInstance.setPointPattern( m_tempPointPattern, 
            TPLDIALOG_P_PATTERN_W, TPLDIALOG_P_PATTERN_W );            
          
          canvasInstance.draw( &auxPoint );  
        }
      }

      void TiePointsLocatorDialog::on_mapDisplay2_extentChanged()
      {
        const te::gm::Envelope& mapDisplayExtent = 
          m_mapDisplay2->getExtent();
          
        m_mapDisplay2->getDraftPixmap()->fill( QColor( 0, 0, 0, 0 ) );
          
        te::qt::widgets::Canvas canvasInstance( m_mapDisplay2->getDraftPixmap() );
        canvasInstance.setWindow( mapDisplayExtent.m_llx,
          mapDisplayExtent.m_lly, mapDisplayExtent.m_urx,
          mapDisplayExtent.m_ury );          
        
        // Drawing the colected points
        
        const int rowCount = 
          m_uiPtr->m_tiePointsTableWidget->rowCount();
        QTableWidgetItem* itemPtr = 0;
        unsigned int tpID = 0;
        TPContainerT::iterator tiePointsIt;
        te::gm::Point auxPoint;
        te::gm::Coord2D auxCoord2D;
          
        for( int row = 0 ; row < rowCount ; ++row )
        {
          itemPtr = m_uiPtr->m_tiePointsTableWidget->item( row, 0 );
          
          if( itemPtr->isSelected() )
          {
            canvasInstance.setPointPattern( m_selectedPointPattern, 
              TPLDIALOG_P_PATTERN_W, TPLDIALOG_P_PATTERN_W );
          }
          else
          {
            canvasInstance.setPointPattern( m_unselectedPointPattern, 
              TPLDIALOG_P_PATTERN_W, TPLDIALOG_P_PATTERN_W );
          }
          
          tpID = itemPtr->text().toUInt();
          
          tiePointsIt = m_tiePoints.find( tpID );
          assert( tiePointsIt != m_tiePoints.end() );
          
          m_raster2->getGrid()->gridToGeo( 
            tiePointsIt->second.m_tiePoint.second.x, tiePointsIt->second.m_tiePoint.second.y,
            auxCoord2D.x, auxCoord2D.y );
            
          auxPoint.setX( auxCoord2D.x );
          auxPoint.setY( auxCoord2D.y );
          
          canvasInstance.draw( &auxPoint );            
        }    
      }
    } // namespace qt
  } // namespace qt
} // namespace te

