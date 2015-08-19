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
 * \file LayerItem.h
 *
 * \brief Defines a layer item.
 */
#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERITEM_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERITEM_H

#include "TreeItem.h"

// TerraLib
#include "../../../../maptools/AbstractLayer.h"

// STL
#include <memory>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
      * \class LayerItem
      *
      * \brief An item that contains a te::map::AbstractLayerPtr.
      */
      class TEQTWIDGETSEXPORT LayerItem: public TreeItem
      {
      public:
        /*!
        * \name Instantiation Methods
        *
        * Methods related to creation and destruction of the object.
        */
        //@{

        /*!
        * \brief Constructor.
        *
        * \param layer The layer of the item.
        */
        LayerItem(te::map::AbstractLayerPtr layer);

        /*!
        * \brief Destructor.
        */
        ~LayerItem();
        //@}

        /*!
        * \name Reimplementation of TreeItem
        *
        * Methods reimplemented of the TreeItem class.
        */
        //@{
        std::string getAsString() const;

        VISIBLE isVisible() const;

        void setVisible(const VISIBLE& visible, const bool& updateAncestors = false, const bool& updateDescendents = false);

        Qt::ItemFlags flags();
        //@}

        /*!
        * \brief Returns the layer contained in the item.
        *
        * \return The layer contained at the item.
        */
        te::map::AbstractLayerPtr getLayer() const;

        void updateChart();

        int updateGrouping();

      protected:
        te::map::AbstractLayerPtr m_layer;   //!< The asbtract layer.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERITEM_H
