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
  \file PointGeometries.h
  \brief Event emitted when geometries were pointed.
  \details 
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
*/

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_POINTGEOMETRIES_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_POINTGEOMETRIES_H

// TerraLib
#include "Enums.h"
#include "Event.h"

namespace te
{
  namespace gm
  {
    //! Forward declarations
    class Geometry;
  }

  namespace map
  {
    class Layer;
  }

  namespace qt
  {
    namespace af
    {
      /*!
        \class PointGeometries
        \copydoc PointGeometries.h
        \ingroup afevents
      */
      class PointGeometries : public Event
      {
      public:

        /*!
          \brief Constructor.
          \param layer The layer that contains the pointed geometries.
          \param geoms A map of geometries, relating identifiers to geometries.
        */
        PointGeometries(te::map::Layer* layer, const std::map<std::string, te::gm::Geometry*>* geoms) :
        Event(te::qt::af::evt::POINT_GEOMETRIES),
        m_geoms(geoms),
        m_layer(layer)
        {
        }

        /*!
          \brief Destructor.
        */
        ~PointGeometries()
        {
        }

        const std::map<std::string, te::gm::Geometry*>* m_geoms;  //!< Pointer to a map of geometries.
        te::map::Layer* m_layer;                                  //!< Layer that contains the geometries.
      };
    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_EVENTS_INTERNAL_POINTGEOMETRIES_H

