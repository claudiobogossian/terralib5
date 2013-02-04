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
  \file UpdateLayerVisibility.h

  \brief This event signals that a the layer visibility changed.
 */

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_UPDATELAYERVISIBILITY_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_UPDATELAYERVISIBILITY_H

// TerraLib
#include "Enums.h"
#include "Event.h"

namespace te
{
  //! Forward declarations
  namespace map
  {
    class Layer;
  }

  namespace qt
  {
    namespace af
    {
      /*!
        \class UpdateLayerVisibilty
        
        \brief This event signals that a the layer visibility changed.
        
        \ingroup afevents
      */
      class UpdateLayerVisibilty : public Event
      {
        public:

          /*!
            \brief Constructor.
            \param layer The layer wich the visibility changes.
            \param visible Layer visibility.
          */
          UpdateLayerVisibilty(te::map::Layer* layer, const bool& visible) : 
          Event(te::qt::af::evt::UPDATE_LAYER_VISIBILTY),
          m_layer(layer),
          m_visible(visible)
          {
          }

          /*!
            \brief Destructor.
          */
          ~UpdateLayerVisibilty()
          {
          }

          te::map::Layer* m_layer;  //!< Pointer to the layer.
          bool m_visible;           //!< Visibility.
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_EVENTS_INTERNAL_UPDATELAYERVISIBILITY_H

