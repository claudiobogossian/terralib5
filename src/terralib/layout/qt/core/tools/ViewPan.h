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

#ifndef __TERRALIB_LAYOUT_INTERNAL_VIEWPAN_H
#define __TERRALIB_LAYOUT_INTERNAL_VIEWPAN_H

// TerraLib
#include "../../../core/Config.h"
#include "../AbstractViewTool.h"

namespace te
{
  namespace layout
  {
    // Forward declarations
    class View;

      /*!
        \class Pan

        \brief This class implements a concrete tool to geographic pan operation.

        \ingroup widgets
      */
      class TELAYOUTEXPORT ViewPan : public AbstractViewTool
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a pan tool associated with the given map display and with the specified cursors.

            \param display The map display associated with the tool.
            \param cursor The default tool cursor.
            \param actionCursor An optional cursor to be used during the pan user action. If Qt::BlankCursor, it will be NOT modified.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
            \note If the given cursor is different of Qt::BlankCursor, it will be setted on map display.
          */
          ViewPan(View* view, const QCursor& cursor, const QCursor& actionCursor = Qt::BlankCursor, QObject* parent = 0);

          /*! \brief Destructor. */
          ~ViewPan();

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

          bool m_panStarted;      //!< Flag that indicates if pan operation was started.
          QPoint m_origin;        //!< Origin point on mouse pressed.
          QPoint m_delta;         //!< Difference between pressed point and destination point on mouse move.
          QCursor m_actionCursor; //!< An optional cursor to be used during the pan user action.

      };
  }   // end namespace layout
}     // end namespace te

#endif 
