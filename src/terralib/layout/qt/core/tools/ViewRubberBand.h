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

#ifndef __TERRALIB_LAYOUT_INTERNAL_VIEWRUBBER_BAND_H
#define __TERRALIB_LAYOUT_INTERNAL_VIEWRUBBER_BAND_H

// TerraLib
#include "../../../core/Config.h"
#include "../AbstractViewTool.h"

// Qt
#include <QRectF>
#include <QBrush>
#include <QPen>
#include <QPixmap>

namespace te
{
  namespace layout
  {
    // Forward declarations
    class View;

      /*!
        \class ViewRubberBand

        \brief This class implements a concrete tool to geographic pan operation.

        \ingroup widgets
      */
      class TELAYOUTEXPORT ViewRubberBand : public AbstractViewTool
      {
        public:

           /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a rubber band associated with the given map display and with the specified cursor.

            \param display The map display associated with the tool.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
            \note If the given cursor is different of Qt::BlankCursor, it will be setted on map display.
          */
          ViewRubberBand(View* view, QObject* parent = 0);

          /*! \brief Destructor. */
          ~ViewRubberBand();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          bool mousePressEvent(QMouseEvent* e);

          bool mouseMoveEvent(QMouseEvent* e);

          bool mouseReleaseEvent(QMouseEvent* e);

          //@}

        protected:

          QPoint m_origin;  //!< Origin point on mouse pressed.
          QRectF m_rect;    //!< The boundary rectangle managed by the rubber band.
          QPen m_pen;       //!< The pen used to draw the rubber band shape.
          QBrush m_brush;   //!< The brush used to draw the rubber band shape.
          bool m_started;
          QPixmap* m_draft;

      };
  }   // end namespace layout
}     // end namespace te

#endif 
