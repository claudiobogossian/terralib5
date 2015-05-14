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
  \file terralib/qt/widgets/tools/PointPicker.h

  \brief This class implements a concrete tool to geographic coordinate picker on mouse click operation.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_POINTPICKER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_POINTPICKER_H

// TerraLib
#include "../Config.h"
#include "AbstractTool.h"

// Forward declarations
class QPointF;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
    class MapDisplay;

      /*!
        \class PointPicker

        \brief This class implements a concrete tool to geographic coordinate picker on mouse click operation.
      */
      class TEQTWIDGETSEXPORT PointPicker : public AbstractTool
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a coordinate tracking tool associated with the given map display.

            \param display The map display associated with the tool.
            \param cursor The default tool cursor.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
          */
          PointPicker(MapDisplay* display, const QCursor& cursor, QObject* parent = 0);

          /*! \brief Destructor. */
          ~PointPicker();

          //@}

          /** @name AbstractTool Methods
           *  Methods related with tool behavior.
           */
          //@{

          bool mouseReleaseEvent(QMouseEvent* e);

          //@}

        signals:

          /*! This signal is emitted when a coordinate was picked. */
          void pointPicked(QPointF& coordinate);
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_POINTPICKER_H
