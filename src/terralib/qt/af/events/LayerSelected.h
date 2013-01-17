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
  \file LayerSelected.h

  \brief This event signals that the selected layer on layer explorer has changed.
 */

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_LAYERSELECTED_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_LAYERSELECTED_H

// TerraLib
#include "Enums.h"
#include "Event.h"

namespace te
{
  //! Forward declarations
  namespace map
  {
    class AbstractLayer;
  }

  namespace qt
  {
    namespace af
    {
      /*!
        \class LayerSelected

        \brief This event signals that the selected layer on layer explorer has changed.
        
        \ingroup afevents
      */
      class LayerSelected : public Event
      {
        public:

          /*!
            \brief Constructor.
            \param layer The selected layer.
          */
          LayerSelected(te::map::AbstractLayer* layer) : 
          Event(te::qt::af::evt::LAYER_SELECTED),
          m_layer(layer)
          {
          }

          /*!
            \brief Destructor.
          */
          ~LayerSelected()
          {
          }

          te::map::AbstractLayer* m_layer;  //!< Pointer to the selected layer.
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_EVENTS_INTERNAL_LAYERSELECTED_H

