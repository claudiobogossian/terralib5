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
  \file StyleChanged.h
  \brief Event emitted the style of some group has changed.
  \details 
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
*/

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_STYLECHANGED_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_STYLECHANGED_H

//! TerraLib
#include "Enums.h"
#include "Event.h"

//! Forward declarations
class QColor;

namespace te
{
  namespace map
  {
    class Layer;
  }

  namespace qt
  {
    namespace af
    {
      /*!
        \class StyleChanged
        \copydoc StyleChanged.h
        \ingroup afevents
      */
      class StyleChanged : public Event
      {
      public:

        /*!
          \brief Constructor.
          \param layer The layer wich the group is associated to.
          \param group The identifier of the group of highlight.
          \param color Pointer to the color selected.
        */
        StyleChanged(te::map::Layer* layer, const int& group, const QColor* color) :
        Event(te::qt::af::evt::STYLE_CHANGED),
        m_group(group),
        m_color(color)
        {
        }

        /*!
          \brief Destructor.
        */
        ~StyleChanged()
        {
        }

        int m_group;              //!< Identifier of the group.
        const QColor* m_color;    //!< New color selected.
        te::map::Layer* m_layer;  //!< Layer to be updated the color.
      };
    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_EVENTS_INTERNAL_STYLECHANGED_H

