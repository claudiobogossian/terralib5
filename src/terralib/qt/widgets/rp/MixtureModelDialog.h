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
  \file terralib/qt/widgets/rp/MixtureModelDialog.h

  \brief A dialog used to execute mixture model decomposition.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELDIALOG_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELDIALOG_H

// TerraLib
#include "../Config.h"
#include "../../../color/RGBAColor.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../raster/Raster.h"

// Qt
#include <QDialog>
#include <QPointF>

// STL
#include <map>
#include <string>

// Boost
#include <boost/shared_ptr.hpp>

// Forward user interface declaration
namespace Ui
{
  class MixtureModelForm;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class CoordTracking;
      class MapDisplay;
      class Pan;
      class ZoomWheel;

      /*!
        \class MixtureModelDialogMDEventFilter

        \brief A event filter to handle map display events.
      */
      class TEQTWIDGETSEXPORT MixtureModelDialogMDEventFilter : public QObject
      {
        Q_OBJECT

        public:

          MixtureModelDialogMDEventFilter(te::qt::widgets::MapDisplay* parent);

          ~MixtureModelDialogMDEventFilter();

//overload
          bool eventFilter(QObject* watched, QEvent* event);

        signals:

          /*! This signal is emitted when a keyboar key was pressed. */
          void keyPressedOverMapDisplay( int key );

        protected:

          te::qt::widgets::MapDisplay* m_mDisplay;

      };

      /*!
        \class MixtureModelDialog

        \brief A dialog used to execute mixture model decomposition.
      */
      class TEQTWIDGETSEXPORT MixtureModelDialog : public QDialog
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructs a basic dialog which is a child of parent, with widget flags set to f.

            \param inputRasterPtr      Input raster pointer.
            \param outpuRasterDSType   Output raster data source type (as described in te::rst::RasterFactory).
            \param outpuRasterInfo     The necessary information to create the raster (as described in te::rst::RasterFactory).
            \param parent              Parent widget pointer.
            \param f                   Widget flags.
          */
          MixtureModelDialog(//const te::rst::Raster* inputRasterPtr,
                             const te::map::DataSetLayerPtr& inputLayerPtr,
                             const std::string& outpuRasterDSType,
                             const std::map<std::string, std::string>& outpuRasterInfo,
                             QWidget* parent = 0,
                             Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~MixtureModelDialog();

          /*!
            \brief Returns the output result raster.

            \param outputRasterPtr A pointer to the output result raster

            \return true if OK, false on errors.
          */
          bool getOutputRaster(boost::shared_ptr< te::rst::Raster >& outputRasterPtr);

        protected slots:

          void on_okPushButton_clicked();
          void on_coordTracked_changed(QPointF& coordinate);
          void on_keyPressedOverMapDisplay(int key);
          void on_removeButton_clicked();
          void on_mapDisplay_extentChanged();
          void updateComponentsGrid();

        private:

          Ui::MixtureModelForm* m_uiPtr;                             //!< User interface.
          const te::rst::Raster* m_inputRasterPtr;                   //!< Input raster pointer.
          std::string m_outpuRasterDSType;                           //!< Output raster data source type (as described in te::rst::RasterFactory).
          std::map<std::string, std::string> m_outpuRasterInfo;      //!< The necessary information to create the raster (as described in te::rst::RasterFactory).
          boost::shared_ptr<te::rst::Raster> m_outputRasterPtr;      //!< Output raster pointer.
          int currentColumn;                                         //!< The column position of mouse in map display.
          int currentRow;                                            //!< The row position of mouse in map display.
          te::qt::widgets::MapDisplay* m_mapDisplay;                 //!< The map display to show the input raster.
          te::qt::widgets::Pan* m_panClickEvent;                     //!< Pan click event for map display.
          te::qt::widgets::ZoomWheel* m_zoomScroolEvent;             //!< Zoom event using mouse scrool for map display.
          CoordTracking* m_coordTracking;                            //!< Coordinate tracking for map display.
          MixtureModelDialogMDEventFilter* m_keyboardPressTracking;  //!< The event filter to detect when user press space bar.
          std::map<std::string, std::vector<double> > m_components;  //!< The map of selected components (name -> values).
          std::map<std::string, te::gm::Coord2D> m_coordinates;      //!< The map of selected coordinates (name -> coordinates).
          unsigned int m_maxComponentsInserted;                      //!< The maximum number of components inserted.
          te::color::RGBAColor** m_selectedPointPattern;             //!< The display draw pattern used for selected points.
          // te::color::RGBAColor** m_unselectedPointPattern; //!< The display draw pattern used for unselected points.
          // te::color::RGBAColor** m_tempPointPattern; //!< The display draw pattern used for temporary points.
      };

    }  // end namespace widgets
  }  // end namespace qt
}  // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELDIALOG_H
