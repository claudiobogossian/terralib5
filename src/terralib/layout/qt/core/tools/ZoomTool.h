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

#ifndef __TERRALIB_LAYOUT_INTERNAL_ZOOM_TOOL_H
#define __TERRALIB_LAYOUT_INTERNAL_ZOOM_TOOL_H

// TerraLib
#include "../../../core/Config.h"
#include "AbstractLayoutTool.h"

namespace te
{
  namespace layout
  {
    // Forward declarations
    class View;

      /*!
        \class ZoomTool

        \brief This class implements a concrete tool to geographic zoom operation.

        \ingroup widgets
      */
    class TELAYOUTEXPORT ZoomTool : public AbstractLayoutTool
    {
      public:

        /*!
          \enum ViewZoomType

          \brief Defines the zoom type.
        */
        enum ZoomType
        {
          ZoomIn, /*!< Performs the zoom in operation.  */
          ZoomOut /*!< Performs the zoom out operation. */
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
        ZoomTool(View* view, const double& zoomFactor = 2.0, const ZoomType& type = ZoomIn, QObject* parent = 0);

        /*! \brief Destructor. */
        virtual ~ZoomTool();

        //@}

        /*!
          \brief Sets the zoom operation type.

          \param type The zoom operation type - ZoomIn | ZoomOut;
        */
        void setZoomType(const ZoomType& type);

        /*!
          \brief Performs the zoom operation on map display, considering the zoom factor, zoom type and the given point.

          \param point A point in world coordinates.

          \note If the given point is not null, the new extent will be centered on this point. Otherwise, the current center will be kepped.
        */
        void applyZoom(const QPointF& point = QPointF());

      protected:

        double m_zoomFactor;   //!< Factor used to zoom. i.e. A factor value of 2.0 (default) will generate a new extent twice (%) bigger or smaller, depending of ZoomType.
        ZoomType m_zoomType;   //!< Enum that indicates the zoom type.
    };
  }   // end namespace layout
}     // end namespace te

#endif 
