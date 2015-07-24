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
  \file LayerEvents.h

  \brief Contains a list of the layer events.
*/

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_LAYEREVENTS_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_LAYEREVENTS_H

#include "../../../maptools/Enums.h"
#include "../../../maptools/AbstractLayer.h"
//#include "../../widgets/layer/explorer/AbstractTreeItem.h"
#include "Event.h"
#include "Enums.h"

// Qt
#include <QAction>
#include <QColor>

// STL
#include <memory>

namespace te
{
// Forward declaration
  namespace da
  {
    class DataSet;
  }

  namespace gm
  {
    class Envelope;
  }

  namespace qt
  {
    namespace af
    {
      namespace evt
      {
        /*!
          \struct LayerAdded

          \brief This event signals that a new layer was created.
         */
        struct LayerAdded : public Event
        {
          /*!
            \brief Constructor.

            \param layer       The layer to be added.
            \param parentLayer  The parent layer where the new layer will be added.
          */
          LayerAdded(const te::map::AbstractLayerPtr& layer, const te::map::AbstractLayerPtr& parentLayer = te::map::AbstractLayerPtr())
            : Event(LAYER_ADDED),
              m_layer(layer),
              m_parentLayer(parentLayer)
          {
          }

          te::map::AbstractLayerPtr m_layer;        //!< Tha layer to be added.
          te::map::AbstractLayerPtr m_parentLayer;  //!< The parent layer where the layer will be added.
        };

        /*!
          \struct ItemOfLayerRemoved

          \brief This event signals that a item belonging to a layer will be removed from the layer explorer.
         */
        //struct ItemOfLayerRemoved : public Event
        //{
        //  /*!
        //    \brief Constructor.

        //    \param item Item belonging to a layer to be removed from the layer explorer.
        //  */
        //  ItemOfLayerRemoved(te::qt::widgets::AbstractTreeItem* item)
        //    : Event(ITEM_OF_LAYER_REMOVED),
        //      m_item(item)
        //  {
        //  }

        //  te::qt::widgets::AbstractTreeItem* m_item;  //!< Item of the layer to be removed.
        //};

        /*!
          \struct LayerRemoved

          \brief This event signals that a layer is to be removed from the layer explorer.
         */
        struct LayerRemoved : public Event
        {
          /*!
            \brief Constructor.

            \param layer Layer to be removed.
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

          \brief This event signals that the layer visibility has changed.
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
          LayerSelectedObjectsChanged(te::map::AbstractLayerPtr layer, te::gm::Envelope* e = 0)
            : Event(LAYER_SELECTED_OBJECTS_CHANGED),
              m_layer(layer),
              m_envelope(e)
          {
          }

          te::map::AbstractLayerPtr m_layer; //!< Layer whose selected objects were changed.
          te::gm::Envelope* m_envelope;      //!< The box of the last selected object.
        };

        /*!
          \struct HighlightLayerObjects

          \brief This event indicates that the objects of the given layer must be highlighted.
        */
        struct HighlightLayerObjects : public Event
        {
          /*!
            \brief Constructor

            \param layer   The layer whose objects must be highlighted.
            \param dataset The dataset that represents the objects that must be highlighted.
          */
          HighlightLayerObjects(te::map::AbstractLayerPtr layer, te::da::DataSet* dataset, const QColor& color)
            : Event(HIGHLIGHT_LAYER_OBJECTS),
              m_layer(layer),
              m_dataset(dataset),
              m_color(color)
          {
            assert(m_dataset);
          }

          te::map::AbstractLayerPtr m_layer; //!< The layer whose objects must be highlighted.
          te::da::DataSet* m_dataset;        //!< The dataset that represents the objects that must be highlighted.
          QColor m_color;                    //!< The color used to highlight.
        };

        /*!
          \struct LayerStyleSelected

          \brief This event indicates that the layer style was selected on the layer explorer.
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

        /*!
          \struct LayerPopUpAddAction

          \brief This event is used to add a action in a layer tree pop up
        */
        struct LayerPopUpAddAction : public Event
        {
          /*!
            \brief Construtor.

            \param action             The action to be added to the context menu.
            \param itemType           The type of the selected item.
            \param menuSelectionType  The selection type of the context menu
          */
          LayerPopUpAddAction(QAction* action, std::string itemType, int menuSelectionType)
            : Event(LAYER_POPUP_ADD_ACTION),
              m_action(action),
              m_itemType(itemType),
              m_menuSelectionType(menuSelectionType)
          {
          }
  
          QAction* m_action;           //!< The action to be added in pop up menu
          std::string m_itemType;      //!< The type of the selected item
          int m_menuSelectionType;     //!< The selection type of the context menu
        };

        /*!
          \struct LayerPopUpRemoveAction

          \brief This event is used to remove a action in a layer tree pop up
        */
        struct LayerPopUpRemoveAction : public Event
        {
          /*!
            \brief Construtor.

            \param action The action to be removed.
          */
          LayerPopUpRemoveAction(QAction* action)
            : Event(LAYER_POPUP_REMOVE_ACTION),
              m_action(action)
          {
          }
  
          QAction* m_action;              //!< Action to be removed in the pop up menu
        };

        /*!
          \struct GetLayerSelected

          \brief This event is used to get a single layer selected in layer tree
        */
        struct GetLayerSelected : public Event
        {
          /*!
            \brief Construtor.

            \param layer The layer that has the selected style.
          */
          GetLayerSelected() : Event(GET_LAYER_SELECTED)
          {
          }
  
          te::map::AbstractLayerPtr m_layer;  //!< Layer selected.
        };
      }
    }
  }
}

#endif //__TERRALIB_QT_AF_EVENTS_INTERNAL_LAYEREVENTS_H
