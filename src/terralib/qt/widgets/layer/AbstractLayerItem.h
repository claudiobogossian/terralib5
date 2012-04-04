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
  \file AbstractLayerItem.h

  \brief A widget designed to show the information of a given Layer in a tree oriented way.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTLAYERITEM_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTLAYERITEM_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QVariant>

// Forward declaration
class QMenu;
class QWidget;

namespace te
{
// Forward declarations
  namespace map { class AbstractLayer; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class AbstractLayerItem

        \brief A widget designed to show the information of a given Layer in a tree oriented way.

        \todo As classes de layer do lado Qt devem ser notificadas de qualquer alteracao no modelo em memoria refenciado por ela.
              Talvez, uma solucao seja usar o padrao Observer para as classes te::map::AbstractLayer e te::qt::widgets::AbstractLayer.
       */
      class TEQTWIDGETSEXPORT AbstractLayerItem : public QObject
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It initializes a Layer item with a reference to a layer and an item parent.

            \param parent   The parent item or NULL if it is a root item.

            \note The AbstractLayerItem will NOT take the ownership of the referenced layer.
                  Be sure not to clear it before dropping off the AbstractLayerItem.
           */
          AbstractLayerItem(te::map::AbstractLayer* refLayer, QObject* parent = 0);

          /*! Virtual destructor. */
          virtual ~AbstractLayerItem();

          //@}

          /** @name Qt AbstractLayerItem General Methods
           *  General methods for a AbstractLayerItem.
           */
          //@{

          /*!
            \brief It returns the layer associated to this AbstractLayerItem.

            \return The layer associated to this AbstractLayerItem.
           */
          te::map::AbstractLayer* getRefLayer() const;

          /*!
            \brief It returns the children list of this tree item.

            \return The list of children of this tree item.
           */
          const QList<te::qt::widgets::AbstractLayerItem*>& getChildren() const;

          /*!
            \brief It returns the child item in the given position.

            \param pos The position of the item to be retrieved.

            \return The child item in the given position.
           */
          te::qt::widgets::AbstractLayerItem* getChild(int pos);

          /*!
            \brief It returns the index position of this item as child of the parent item.

            \return The index position of this item as child of the parent item.

            \note Use this method only if this item has a parent, otherwise, it will return an invalid index.
           */
          int getIndex() const;

          /*!
            \brief It removes the given number of items as children of this item from the given position.

            \param i     The position from which the list of items will be removed as children of this item.
            \param count The number of items to be removed as children of this item.
           */
          bool removeChildren(int i, int count);

          /*!
            \brief It adds an item in the given position.

            \param i          The position where the item will be added.
            \param layerItem  The layer item to be added.
           */
          void addChild(int i, AbstractLayerItem* layerItem);

          /*!
            \brief It moves a child from a position to another one.

            \param fromPos  The position the child item should be moved from.
            \param toPos    The position where the child item should be moved to.
           */
          void moveChild(int fromPos, int toPos);

          /*!
            \brief It returns the child row of the given item.

            \param layeItem  The layer item

            \return The row of the layer item
           */
          int getChildRow(AbstractLayerItem* layerItem);

          //@}

          /** @name Pure Virtual Methods
           *  Methods that subclasses must implement.
           */
          //@{

          /*!
            \brief It returns the data stored under the given role for the item referred by this layer.

            This method will be called by the implementation of LayerExplorerModel.

            \param role The role are used by a given view to indicate to this model which type of data it needs.

            \return The data stored under the given role for the item referred by this layer.
           */
          virtual QVariant data(int role) const = 0;

          /*!
            \brief It creates a menu to be displayed in the given widget.

            \param parent The parent widget for the menu.

            \return A menu to be displayed in the given widget.
           */
          virtual QMenu* getMenu(QWidget* parent = 0) const = 0;

          //@}

        protected:

          /*!
            \brief It appends an item at the end of the children list of the tree item.

            \param item The item to be appended.
           */
          void append(AbstractLayerItem* item);

        protected:
      
          te::map::AbstractLayer* m_refLayer;  //!< A pointer to the referenced layer.
          QList<te::qt::widgets::AbstractLayerItem*> m_children; //!< internal list of children.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTLAYERITEM_H
