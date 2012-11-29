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
#include "../../../maptools/RasterLayer.h"
#include "../../../geometry/GTParameters.h"

// Qt
#include <QtGui/QDialog>
#include <QtCore/QObject>
#include <QtCore/QPoint>

// STL

#include <vector>

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
      
      namespace rp
      {
        class TiePointsLocatorAdvancedDialog;
        
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
              te::map::RasterLayer const* inRasterLayer1Ptr,
              te::map::RasterLayer const* inRasterLayer2Ptr,
              QWidget* parent = 0, 
              Qt::WindowFlags f = 0);

            /*! \brief Destructor. */
            ~TiePointsLocatorDialog();
            
            // overload
            void keyPressEvent ( QKeyEvent * event );

          protected slots:

            void on_okPushButton_clicked();
            void on_autoAcquireTiePointsPushButton_clicked();
            void on_selectAllPushButton_clicked();
            void on_unselectAllPushButton_clicked();
            void on_deleteSelectedPushButton_clicked();
            void on_advancedOptionsPushButton_clicked();
            void on_addPushButton_clicked();
            void on_insertKeyPressed1( QPointF& coordinate );
            void on_insertKeyPressed2( QPointF& coordinate );
            void on_coordTracked1(QPointF &coordinate);
            void on_coordTracked2(QPointF &coordinate);

          private:

            Ui::TiePointsLocatorForm* m_uiPtr; //! User interface.
            TiePointsLocatorAdvancedDialog* m_advDialogPtr; //!< Advanced options dialog.
            te::qt::widgets::MapDisplay* m_mapDisplay1; //!< Map display 1.
            te::qt::widgets::MapDisplay* m_mapDisplay2; //!< Map display 2.
            te::map::RasterLayer const* m_inRasterLayer1Ptr; //! Input raster layer 1 pointer.
            te::map::RasterLayer const* m_inRasterLayer2Ptr; //! Input raster layer 2 pointer.
            te::qt::widgets::ZoomLeftAndRightClick* m_zoomClickEvent1; //!< Zoom click event (map display 1).
            te::qt::widgets::ZoomLeftAndRightClick* m_zoomClickEvent2; //!< Zoom click event (map display 2).
            CoordTracking* m_coordTracking1; //!< Coord tracking (map display 1);
            CoordTracking* m_coordTracking2; //!< Coord tracking (map display 2);
            te::gm::GTParameters::TiePoint m_lastTrackedTiePoint; //!< The last mouse tracked tie-point;
        }; 
      };
    };
  };
};



#endif
