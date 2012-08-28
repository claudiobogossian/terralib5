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
  \file LayerItem.h

  \brief A widget designed to show the information of a Layer in a tree oriented way.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_LAYERITEM_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_LAYERITEM_H

// TerraLib
#include "AbstractTreeItem.h"

//STL
#include <vector>

namespace te
{
// Forward declarations
  namespace map { class AbstractLayer; }

  namespace qt
  {
    namespace widgets
    {
      // Foward declarations
      class LegendItem;

      /*!
        \class LayerItem

        \brief A widget designed to show the information of a Layer in a tree oriented way.
       */
      class TEQTWIDGETSEXPORT LayerItem : public AbstractTreeItem
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It initializes a Layer item with a reference to a layer and an item parent.

            \param refLayer A pointer to the referenced layer.
            \param parent   The parent item or NULL if it is a root item.

            \note The LayerItem will NOT take the ownership of the referenced layer.
                  Be sure not to clear it before dropping off the LayerItem.
           */
          LayerItem(te::map::AbstractLayer* refLayer, QObject* parent = 0);

          /*! Destructor. */
          ~LayerItem();

          //@}

          /*!
            \brief It returns the data stored under the given role for the item referred by this layer.

            This method will be called by the implementation of LayerExplorerModel.

            \param role The role are used by a given view to indicate to this model which type of data it needs.

            \return The data stored under the given role for the item referred by this layer.
           */
          QVariant data(int role) const;

          /*!
            \brief It checks if the item is a layer item.

            \return The default implementation returns true indicating that the item is a layer item.
           */
          bool isLayerItem() const;

          /*!
            \brief It creates a menu to be displayed in the given widget.

            \param parent The parent widget for the menu.

            \return A menu to be displayed in the given widget.
           */
          QMenu* getMenu(QWidget* parent = 0) const;

          /*!
            \brief It sets the legend associated to this layer item.
           */
          void setLegend();
       };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_LAYERITEM_H
