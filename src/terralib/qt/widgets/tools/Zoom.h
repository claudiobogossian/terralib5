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
  \file terralib/qt/widgets/tools/Zoom.h

  \brief This is a utility class to geographic zoom operation.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ZOOM_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ZOOM_H

// TerraLib
#include "../Config.h"
#include "AbstractTool.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
    class MapDisplay;

      /*!
        \class Zoom

        \brief This is a utility class to geographic zoom operation.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT Zoom : public AbstractTool
      {
        public:

          /*!
            \enum ZoomType

            \brief Defines the zoom type.
          */
          enum ZoomType
          {
            In, /*!< Performs the zoom in operation.  */
            Out /*!< Performs the zoom out operation. */
          };

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a zoom associated with the given map display.

            \param display The map display associated with the zoom operation.

            \note The class will NOT take the ownership of the given pointer.
          */
          Zoom(MapDisplay* display, const double& zoomFactor = 2.0, const ZoomType& type = In, QObject* parent = 0);

          /*! \brief Destructor. */
          virtual ~Zoom();

          //@}

          /*!
            \brief Sets the zoom operation type.

            \param type The zoom operation type - ZoomIn | ZoomOut;
          */
          void setZoomType(const ZoomType& type);

          /*!
            \brief Performs the zoom operation on map display, considering the zoom factor, zoom type and the given point.

            \param point A point in world coordinates.
            \param centralize If the reference point is given, its possible to choose between centering this reference point or keeping it in the same position that it was.

            \note If the given point is not null, the new extent will be centered on this point. Otherwise, the current center will be kepped.
          */
          void applyZoom(const QPointF& point = QPointF(), bool centralize = true);

        protected:

          double m_zoomFactor;   //!< Factor used to zoom. i.e. A factor value of 2.0 (default) will generate a new extent twice (%) bigger or smaller, depending of ZoomType.
          ZoomType m_zoomType;   //!< Enum that indicates the zoom type.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_ZOOM_H
