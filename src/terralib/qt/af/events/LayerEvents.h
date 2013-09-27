/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file LayerEvents.h

  \brief Contains a list of the layer events.
*/

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_LAYEREVENTS_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_LAYEREVENTS_H

#include "../../../maptools/Enums.h"
#include "../../../maptools/AbstractLayer.h"
#include "Event.h"
#include "Enums.h"

namespace te
{
  namespace qt
  {
    namespace af
    {
      namespace evt
      {
        /*!
          \struct LayerAdded

          \brief This event signals that a new layer was created.

          \ingroup afevents
         */
        struct LayerAdded : public Event
        {
          /*!
            \brief Constructor.

            \param newLayer Layer added.
          */
          LayerAdded(te::map::AbstractLayerPtr newlayer)
            : Event(LAYER_ADDED),
              m_layer(newlayer)
          {
          }

          te::map::AbstractLayerPtr m_layer;  //!< Layer added.
        };

        /*!
          \struct LayerRemoved

          \brief This event signals that a layer was removed.

          \ingroup afevents
         */
        struct LayerRemoved : public Event
        {
          /*!
            \brief Constructor.

            \param layer Layer removed.
          */
          LayerRemoved(te::map::AbstractLayerPtr layer)
            : Event(LAYER_REMOVED),
              m_layer(layer)
          {
          }

          te::map::AbstractLayerPtr m_layer;  //!< Layer removed.
        };

        /*!
          \struct LayerVisibilityChanged

          \brief This event signals that a the layer visibility changed.

          \ingroup afevents
        */
        struct LayerVisibilityChanged : public Event
        {
          /*! 
            \brief Constructor.

            \param layer The layer whose visibility has changed.

            \param visibility The new visibility state: VISIBLE, NOT_VISIBLE, PARTIALLY_VISIBLE.
          */
          LayerVisibilityChanged(te::map::AbstractLayerPtr layer, te::map::Visibility visibility)
            : Event(LAYER_VISIBILITY_CHANGED),
              m_layer(layer),
              m_visibility(visibility)
          {
          }

          te::map::AbstractLayerPtr m_layer;     //!< Layer whose visibility has changed.
          te::map::Visibility m_visibility;      //!< Layer visibility.
        };

        /*!
          \struct LayerSelected

          \brief This event indicates that the layer has been selected.
        
          \ingroup afevents
        */
        struct LayerSelected : public Event
        {
          /*!
            \brief Construtor.

            \param layer The new selected layer.
          */
          LayerSelected(te::map::AbstractLayerPtr layer)
            : Event(LAYER_SELECTED),
              m_layer(layer)
          {
          }
  
          te::map::AbstractLayerPtr m_layer;  //!< Pointer to the selected layer.
        };

        /*!
          \struct LayerSelectedObjectsChanged

          \brief This event indicates that the selected objects of the layer were changed.
        */
        struct LayerSelectedObjectsChanged : public Event
        {
          /*!
            \brief Constructor

            \param layer The layer whose selected objects were changed.
          */
          LayerSelectedObjectsChanged(te::map::AbstractLayerPtr layer)
            : Event(LAYER_SELECTED_OBJECTS_CHANGED),
              m_layer(layer)
          {
          }

          te::map::AbstractLayerPtr m_layer; //!< Layer whose selected objects were changed.
        };

        /*!
          \struct LayersChanged
          
          \brief This event signals that the layout of layers has changed.
        */
        struct LayersChanged : public Event
        {
          /*!
            \brief Constructor.

            \param layers The layers.
          */
          LayersChanged(const std::vector<te::map::AbstractLayerPtr>& layers)
            : Event(LAYERS_CHANGED),
              m_layers(layers)
          {
          }

          std::vector<te::map::AbstractLayerPtr> m_layers; //!< Vector of layers.
        };

        /*!
          \struct LayerStyleSelected

          \brief This event indicates that the layer style was selected on the layer explorer.
        
          \ingroup afevents
        */
        struct LayerStyleSelected : public Event
        {
          /*!
            \brief Construtor.

            \param layer The layer that has the selected style.
          */
          LayerStyleSelected(te::map::AbstractLayerPtr layer) :
            Event(LAYER_STYLE_SELECTED),
            m_layer(layer)
          {
          }
  
          te::map::AbstractLayerPtr m_layer;  //!< Layer whose style was selected.
        };
      }
    }
  }
}

#endif //__TERRALIB_QT_AF_EVENTS_INTERNAL_LAYEREVENTS_H
