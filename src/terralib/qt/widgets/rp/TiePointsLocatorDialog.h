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
  \file terralib/qt/widgets/rp/TiePointsLocatorDialog.h

  \brief A dialog used to execute tie points location.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTSLOCATORDIALOG_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTSLOCATORDIALOG_H

// TerraLib
#include "../Config.h"
#include "../../../raster/Raster.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../geometry/GTParameters.h"
#include "../../../color/RGBAColor.h"

// Qt
#include <QDialog>
#include <QtCore/QObject>
#include <QtCore/QPoint>

// STL

#include <vector>
#include <map>

// Forward user interface declaration
namespace Ui
{
  class TiePointsLocatorForm;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
      class ZoomLeftAndRightClick;
      class CoordTracking;
      class TiePointsLocatorAdvancedDialog;
      
      /*!
        \class TiePointsLocatorDialogMDEventFilter

        \brief A event filter to handle map display events.
      */
      class TEQTWIDGETSEXPORT TiePointsLocatorDialogMDEventFilter : public QObject
      {
        Q_OBJECT
        
        public:
          

          TiePointsLocatorDialogMDEventFilter( te::qt::widgets::MapDisplay* parent );
          
          ~TiePointsLocatorDialogMDEventFilter();
          
          //overload
          bool eventFilter( QObject * watched, QEvent * event );
          
        signals:
          
          /*! This signal is emitted when a keyboar key was pressed. */
          void keyPressed( int key );     
          
        protected :
          
          te::qt::widgets::MapDisplay* m_mDisplay;
          
      };        
      
      /*!
        \class TiePointsLocatorDialog

        \brief A dialog used to execute tie points location.
      */
      class TEQTWIDGETSEXPORT TiePointsLocatorDialog : public QDialog
      {
        Q_OBJECT
        
        public:

          /*! \brief Constructs a basic dialog which is a child of parent, with widget flags set to f. 
          
              \param inRaster1Ptr Input raster layer 1 pointer.
              
              \param inRaster1Ptr Input raster layer 2 pointer.
              
              \param parent Parent widget pointer.
              
              \param f Widget flags.
          */
          TiePointsLocatorDialog(
            const te::map::DataSetLayerPtr& inLayer1Ptr,
            const te::map::DataSetLayerPtr& inLayer2Ptr,
            QWidget* parent = 0, 
            Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~TiePointsLocatorDialog();
          
          /*! 
            \brief Get the current acquired tie-points. 
            \param tiePoints The current acquired tie-points.
          */
          void getTiePoints( std::vector< te::gm::GTParameters::TiePoint >&
            tiePoints ) const;
          
        protected slots:

          void on_okPushButton_clicked();
          void on_autoAcquireTiePointsPushButton_clicked();
          void on_selectAllPushButton_clicked();
          void on_unselectAllPushButton_clicked();
          void on_deleteSelectedPushButton_clicked();
          void on_advancedOptionsPushButton_clicked();
          void on_addPushButton_clicked();
          void on_mapDisplay1_keyPressed( int key );
          void on_mapDisplay2_keyPressed( int key );
          void on_mapDisplay1_coordTracked(QPointF &coordinate);
          void on_mapDisplay2_coordTracked(QPointF &coordinate);
          void on_tiePointsTableWidget_itemSelectionChanged();
          void on_mapDisplay1_extentChanged();
          void on_mapDisplay2_extentChanged();

        protected:

          /*! \class TiePointData Tie Point data. */
          class TiePointData
          {
            public :
              
              /*! \enum TiePointAcquisitionType Tie point acquisition type. */
              enum TiePointAcquisitionType { 
                InvalidAcquisitionT,  //!< Invalid acquisition type.
                ManualAcquisitionT, //!< Manual acquisition type.
                AutomaticAcquisitionT //!< Automatic acquisition type.
              };              
              
              TiePointAcquisitionType m_acqType; //!< Acquisition type.
              te::gm::GTParameters::TiePoint m_tiePoint; //!< Tie point coordinates.
              
              TiePointData();
              TiePointData( const TiePointData& other );
              ~TiePointData();
              const TiePointData& operator=( const TiePointData& other );
          };
          
          typedef std::map< unsigned int, TiePointData >  TPContainerT; //!< Tie-pints container type definition.
          
          te::color::RGBAColor** m_selectedPointPattern; //!< The display draw pattern used for selected points.
          te::color::RGBAColor** m_unselectedPointPattern; //!< The display draw pattern used for unselected points.
          te::color::RGBAColor** m_tempPointPattern; //!< The display draw pattern used for temporary points.
          Ui::TiePointsLocatorForm* m_uiPtr; //! User interface.
          TiePointsLocatorAdvancedDialog* m_advDialogPtr; //!< Advanced options dialog.
          te::qt::widgets::MapDisplay* m_mapDisplay1; //!< Map display 1.
          te::qt::widgets::MapDisplay* m_mapDisplay2; //!< Map display 2.
          te::map::DataSetLayerPtr m_inLayer1Ptr; //! Input data set layer 1 pointer.
          te::map::DataSetLayerPtr m_inLayer2Ptr; //! Input data set layer 2 pointer.
          te::rst::Raster* m_raster1;             //! Input raster 1 pointer.
          te::rst::Raster* m_raster2;             //! Input raster 2 pointer.
          te::qt::widgets::ZoomLeftAndRightClick* m_zoomClickEvent1; //!< Zoom click event (map display 1).
          te::qt::widgets::ZoomLeftAndRightClick* m_zoomClickEvent2; //!< Zoom click event (map display 2).
          CoordTracking* m_coordTracking1; //!< Coord tracking (map display 1);
          CoordTracking* m_coordTracking2; //!< Coord tracking (map display 2);
          te::gm::GTParameters::TiePoint m_lastTrackedTiePoint; //!< The last mouse tracked tie-point by analysing the mouse move over the map areas.
          te::gm::GTParameters::TiePoint m_lastSelectedTiePoint; //!< The last mouse tracked tie-point by pressing any key over the map areas.
          bool m_lastSelectedTiePointHasFirstOk; //!< true if the last selected tie-point has the first part set;
          TiePointsLocatorDialogMDEventFilter* m_mDEventFilter1; //!< Map display 1 event filter.
          TiePointsLocatorDialogMDEventFilter* m_mDEventFilter2; //!< Map display 2 event filter.
          TPContainerT m_tiePoints; //!< Internal tie-points container.
          unsigned int m_lastInsertedTPID; //!< A ID counter for new tie pointes inserted into m_tiePoints;
          
          /*! \brief Uptate the tie-points table widget. */
          void tiePointsTableUpdate();
          
          /*! \brief Uptate the current transformation information widgets. */
          void transformationInfoUpdate();
          
          /*! \brief Refresh map display 1. */
          void refreshMapDisplay1();
          
          /*! \brief Refresh map display 2. */
          void refreshMapDisplay2();            
      }; 
    };
  };
};



#endif
