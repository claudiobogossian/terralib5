/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/tools/ZoomArea.h

  \brief This class implements a concrete tool to geographic zoom in operation using a boundary rectangle.
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_VIEWZOOM_AREA_BAND_H
#define __TERRALIB_LAYOUT_INTERNAL_VIEWZOOM_AREA_BAND_H

// TerraLib
#include "ViewRubberBand.h"

namespace te
{
  namespace layout
  {
// Forward declarations
    class View;

      /*!
        \class ZoomArea

        \brief This class implements a concrete tool to geographic zoom in operation using a boundary rectangle.

        \ingroup widgets
      */
      class TELAYOUTEXPORT ViewZoomArea : public ViewRubberBand
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a zoom area tool associated with the given map display and with the specified cursor.

            \param display The map display associated with the tool.
            \param cursor The tool cursor.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
            \note If the given cursor is different of Qt::BlankCursor, it will be setted on map display.
          */
          ViewZoomArea(View* view, const QCursor& cursor, QObject* parent = 0);

          /*! \brief Destructor. */
          ~ViewZoomArea();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          bool mousePressEvent(QMouseEvent* e);

          bool mouseMoveEvent(QMouseEvent* e);

          bool mouseReleaseEvent(QMouseEvent* e);

          //@}

        private:

          /** @name Copy Constructor and Assignment Operator
           *  Copy constructor and assignment operator not allowed.
           */
          //@{

          /*!
            \brief Copy constructor not allowed.

            \param rhs The right-hand-side copy that would be used to copy from.
          */
          ViewZoomArea(const ViewZoomArea& rhs);

          /*!
            \brief Assignment operator not allowed.

            \param rhs The right-hand-side copy that would be used to copy from.

            \return A reference to this object.
          */
          ViewZoomArea& operator=(const ViewZoomArea& rhs);

          //@}

        private:

          bool m_zoomStarted; //!< Flag that indicates if zoom area operation was started.
      };

  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_ZOOMAREA_H
