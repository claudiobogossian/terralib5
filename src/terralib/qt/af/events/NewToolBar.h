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
  \file NewToolBar.h
  \brief This event signals that an external toolbar was added to the Framework.
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
 */

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_NEWTOOLBAR_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_NEWTOOLBAR_H

// TerraLib
#include "Enums.h"
#include "Event.h"

//! Forward declarations
class QToolBar;

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class NewToolBar
        \copydoc NewToolBar.h
        \ingroup afevents
      */
      class NewToolBar : public Event
      {
        public:

          /*!
            \brief Constructor.
            \param newBar QToolBar to be added to framework.
          */
          NewToolBar(QToolBar* newBar) : 
          Event(te::qt::af::evt::NEW_TOOLBAR),
          m_toolbar(newBar)
          {
          }

          /*!
            \brief Destructor.
          */
          ~NewToolBar()
          {
          }

          QToolBar* m_toolbar;  //!< Pointer to the new QToolBar.
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_EVENTS_INTERNAL_NEWTOOLBAR_H

