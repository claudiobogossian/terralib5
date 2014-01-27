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
  \file terralib/qt/widgets/rp/RasterNavigatorWidget.h

  \brief This file has the RasterNavigatorWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERNAVIGATORWIDGET_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERNAVIGATORWIDGET_H

// TerraLib
#include "../../../geometry/Polygon.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/Enums.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWidget>
#include <QtGui/QComboBox>

namespace Ui { class RasterNavigatorWidgetForm; }

namespace te
{
  namespace gm  { class Envelope; }
  namespace rst { class Raster; }
  namespace se  { class RasterSymbolizer; }

  namespace qt
  {
    namespace widgets
    {
      class AbstractTool;
      class EyeBirdMapDisplayWidget;
      class MapDisplay;
      class Pan;
      class ZoomInMapDisplayWidget;
      class ZoomWheel;

      /*!
        \class RasterNavigatorWidget

        \brief This class is used to navigate over a DataSetLayer (having a raster representation)
           and given a set of tools, such as, zoom in, zoom out, pan, recompose.
        Two new tools as created for raster interaction:
        - point clicked
        - geom definition

        \sa RasterFactory
      */
      class TEQTWIDGETSEXPORT RasterNavigatorWidget : public QWidget
      {
        Q_OBJECT

        public:

          RasterNavigatorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~RasterNavigatorWidget();

        public:

          /*!
            \brief This method is used to set the selected layer
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void set(te::map::AbstractLayerPtr layer, bool setFullScaleBox = false);

          void setVectorial(te::map::AbstractLayerPtr layer);

          void removeVectorial();

          te::gm::Envelope getCurrentExtent();

          te::qt::widgets::MapDisplay* getDisplay();

          te::rst::Raster* getExtentRaster(bool fullScale = false);

          void drawRaster(te::rst::Raster* rst, te::se::Style* style = 0);

          void showAsPreview(bool asPreview);

          void hideColorCompositionTool(bool hide);

          void hideEditionTools(bool hide);

          void hidePickerTool(bool hide);

          void hideGeomTool(bool hide);

          void hideInfoTool(bool hide);

          void hideExtraDisplaysTool(bool hide);

          void setSelectionMode(bool mode);

        protected slots:

          void onGeomAquired(te::gm::Polygon* poly);

          void onPointPicked(QPointF& point);

          void onCoordTrackedChanged(QPointF& coordinate);

          void onMapDisplayExtentChanged();

          void onZoomAreaToggled(bool checked);

          void onZoomOutToggled(bool checked);

          void onPanToggled(bool checked);

          void onPointPickerToggled(bool checked);
          
          void onGeomToggled(bool checked);

          void onReadPixelToggled(bool checked);

          void onExtraDisplaysToggled(bool checked);

          void onRecomposeClicked();

          void onRedComboBoxActivated(int index);

          void onGreenComboBoxActivated(int index);

          void onBlueComboBoxActivated(int index);

          void onMonoComboBoxActivated(int index);

          void onMonoToolClicked(bool flag);

          void onRedToolClicked(bool flag);

          void onGreenToolClicked(bool flag);

          void onBlueToolClicked(bool flag);

          void onCompositionToolClicked(bool flag);

          void onPreviewClicked();

        signals:

          void previewClicked();

          void mapDisplayExtentChanged();

          void pointPicked(double x, double y);

          void geomAquired(te::gm::Polygon* poly);

        protected:

          void setCurrentTool(te::qt::widgets::AbstractTool* tool);

          void listBands();

          void getCompositionInfo();

          void setComboBoxText(QComboBox* cb, std::string value);

        private:

          std::auto_ptr<Ui::RasterNavigatorWidgetForm> m_ui;

          te::map::AbstractLayerPtr m_layer;
          te::se::RasterSymbolizer* m_symbolizer;

          te::qt::widgets::AbstractTool* m_tool;
          te::qt::widgets::MapDisplay* m_mapDisplay;
          te::qt::widgets::ZoomInMapDisplayWidget* m_zoomInMapDisplay;
          te::qt::widgets::EyeBirdMapDisplayWidget* m_eyeBirdMapDisplay;

          te::qt::widgets::Pan* m_panTool;                          //!< Pan tool (used in preview mode)
          te::qt::widgets::ZoomWheel* m_zoomTool;                   //!< Zoom tool (used in preview mode)

          int m_currentColumn;                                       //!< The column position of mouse in map display.
          int m_currentRow;                                          //!< The row position of mouse in map display.

          te::map::Visibility m_visibility;

          QCursor m_pointCursor;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_RASTERNAVIGATORWIDGET_H

