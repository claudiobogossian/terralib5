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
  \file terralib/qt/widgets/tools/ExtentAcquire.h

  \brief This class implements a concrete tool to define a boundary rectangle.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_EXTENTACQUIRE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_EXTENTACQUIRE_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../Config.h"
#include "RubberBand.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
    class MapDisplay;

      /*!
        \class ExtentAcquire

        \brief This class implements a concrete tool to define a boundary rectangle.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT ExtentAcquire : public RubberBand
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It constructs a extent acquire tool associated with the given map display and with the specified cursor.

            \param display The map display associated with the tool.
            \param cursor The tool cursor.
            \param parent The tool's parent.

            \note The tool will NOT take the ownership of the given pointers.
            \note If the given cursor is different of Qt::BlankCursor, it will be setted on map display.
          */
          ExtentAcquire(MapDisplay* display, const QCursor& cursor, QObject* parent = 0);

          /*! \brief Destructor. */
          ~ExtentAcquire();

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
          ExtentAcquire(const ExtentAcquire& rhs);

          /*!
            \brief Assignment operator not allowed.

            \param rhs The right-hand-side copy that would be used to copy from.

            \return A reference to this object.
          */
          ExtentAcquire& operator=(const ExtentAcquire& rhs);

        signals:

          /*! This signal is emitted when a envelope was acquired. */
          void extentAcquired(te::gm::Envelope env);

          //@}

        private:

          bool m_started; //!< Flag that indicates if operation was started.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_EXTENTACQUIRE_H
