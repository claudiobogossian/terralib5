/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A Free and Open Source GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*!
  \file terralib/qt/widgets/canvas/FrameData.h

  \brief The main class of FrameData.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATA_FRAME_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATA_FRAME_H

// TerraLib
#include "../Config.h"
#include "../canvas/MultiThreadMapDisplay.h"
#include "Frame.h"

// Qt
#include <QRect>
#include <QRectF>
#include <QPixmap>
#include <QWidget>

class QMenu;
class QAction;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class LayoutEditor;
      class GeographicGridFrame;
      class UTMGridFrame;
      class GraphicScaleFrame;
      /*!
        \class DataFrame

        \brief A DataFrame ....

      */
      class TEQTWIDGETSEXPORT DataFrame : public Frame
      {
        Q_OBJECT
        
        public:
          DataFrame(const QRectF& frameRect, te::qt::widgets::LayoutEditor* me, Qt::WindowFlags f = Qt::Window);
          DataFrame(const DataFrame& rhs);

          virtual ~DataFrame();
          DataFrame& operator=(const DataFrame& rhs);

          te::qt::widgets::MultiThreadMapDisplay* getMapDisplay();
          void getLayerList(te::map::AbstractLayerPtr, std::list<te::map::AbstractLayerPtr>&);
          QRectF getDataRect();
          void setDataRect(QRectF& r);
          void adjust();
          void adjustWidgetToFrameRect(const QRectF& r);
          double getScale();
          void setScale(double v);
          void sendEventToChildren(bool);
          bool eventFilter(QObject*, QEvent*);
          te::map::AbstractLayer* getData();
          void setData(te::map::AbstractLayerPtr d, int nsrid = TE_UNKNOWN_SRS, QRectF r = QRectF());
          bool transformEnvelope(te::gm::Envelope& e, int oldsrid, int newsrid);
          double getDataUnitToMilimeter();
          void findDataUnitToMilimeter(const te::gm::Envelope& e, int srid);
          void draw();
          void setCursor();
          void rubberBand();
          void getSelectionPoint(const QPoint& p);
          QPoint getCenterSelected();
          void verifyConstraints();
          void showSelectionPoints();
          void hideSelectionPoints();
          void toolTip(const QPoint&, const QString&);
          QPoint getGlobalPositionCenter();
          void setDataChanged(bool);
          void drawButtonClicked();
          QPixmap* getLastDisplayContent();
          QPixmap* getPixmap();
          void setMagneticDeclination(double angle);
          double getMagneticDeclination();

          void hide();
          void show();
          void lower();
          void createUTMGrid();
          void removeUTMGrid();
          void createGeographicGrid();
          void removeGeographicGrid();
          void createGraphicScale();
          void removeGraphicScale();
          void magneticDeclination();
          te::qt::widgets::GeographicGridFrame* getGeoGridFrame();
          te::qt::widgets::UTMGridFrame* getUTMGridFrame();
          te::qt::widgets::GraphicScaleFrame* getGraphicScaleFrame();
          //void createNorthOrientation();
          //void removeNorthOrientation();
          //void createLegend();
          //void removeLegend();

          void setSelectionColor(QColor selColor);
          void drawLayerSelection();

        protected slots:
          void onDrawLayersFinished(const QMap<QString, QString>& errors);

        private:
          te::qt::widgets::MultiThreadMapDisplay* m_mapDisplay;
          QPixmap m_lastDisplayContent;
          double m_scale;
          double m_dataUnitToMilimeter; // conversion factor to milimeter
          te::qt::widgets::GeographicGridFrame* m_geoGridFrame;
          te::qt::widgets::UTMGridFrame* m_UTMGridFrame;
          te::qt::widgets::GraphicScaleFrame* m_graphicScaleFrame;

          bool m_undo;
          QRectF m_dataRect;
          bool m_dataChanged;
          te::map::AbstractLayer* m_data;
          std::list<te::map::AbstractLayerPtr> m_visibleLayers;
          QColor m_selectionColor;
          int m_mouseTask; // 0=none, 1=zoomin, 2=zoomout, 3=pan (sobre o dado)

          //te::qt::widgets::NorthOrientationFrame* m_northFrame;

          //te::qt::widgets::LegendFrame* m_lagendFrame;

          QMenu *m_menu;
          QAction *m_createUTMGridAction;
          QAction *m_removeUTMGridAction;
          QAction *m_createGeographicGridAction;
          QAction *m_removeGeographicGridAction;
          QAction *m_createGraphicScaleAction;
          QAction *m_removeGraphicScaleAction;
          QAction *m_magneticDeclinationAction;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DATA_FRAME_H
