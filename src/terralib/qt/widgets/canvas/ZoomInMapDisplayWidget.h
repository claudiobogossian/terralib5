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
  \file terralib/qt/widgets/canvas/ZoomInMapDisplayWidget.h

  \brief This file has the ZoomInMapDisplayWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_ZOOMINMAPDISPLAYWIDGET_H
#define __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_ZOOMINMAPDISPLAYWIDGET_H

// TerraLib
#include "../../../color/RGBAColor.h"
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QSlider>
#include <QWidget>

namespace te
{
  namespace gm { class Envelope; }

  namespace se { class Mark; }

  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;

      /*!
        \class ZoomInMapDisplayWidget

        \brief This class defines the map display ZoomIn, this component is only a specific 
               map that shows the current extent in a larger scale.
      */
      class TEQTWIDGETSEXPORT ZoomInMapDisplayWidget : public QWidget
      {
        Q_OBJECT

        public:

          ZoomInMapDisplayWidget(te::qt::widgets::MapDisplay* mapParent, QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ZoomInMapDisplayWidget();

        public:

          /*!
            \brief This method is used to set the selected layer for mixture model operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void setList(std::list<te::map::AbstractLayerPtr>& layerList, int srid);

          te::gm::Envelope getCurrentExtent();

          void drawCursorPosition(double x, double y);

          void setEnabled(bool status);

        protected slots:

          void onMapDisplayExtentChanged();

          void onParentMapDisplayExtentChanged();

          void onZoomFactorChanged();

        protected:

          te::gm::Envelope calculateExtent(te::gm::Envelope& e);

        private:

          te::qt::widgets::MapDisplay* m_mapDisplay;

          te::qt::widgets::MapDisplay* m_parentMapDisplay;

          te::color::RGBAColor** m_rgbaMark;                  //!< Represents the pattern of cursor point
          te::se::Mark* m_mark;                               //!< Represents the mark of a cursor point

          bool m_itsMe;
          bool m_isEnabled;

          QSlider* m_sliderZoomFactor;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_ZOOMINMAPDISPLAYWIDGET_H

