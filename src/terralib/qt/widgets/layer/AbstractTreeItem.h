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
  \file AbstractTreeItem.h

  \brief A widget designed to show the information of a tree item.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTTREEITEM_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTTREEITEM_H

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
  namespace map
  {
    class AbstractLayer; 
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class AbstractTreeItem

        \brief A widget designed to show the information of a tree item.

        \todo As classes do lado do Qt devem ser notificadas de qualquer alteracao no modelo em memoria refenciado por ela.
              Talvez, uma solucao seja usar o padrao Observer para as classes te::map::AbstractLayer e te::qt::widgets::AbstractLayer.
       */
      class TEQTWIDGETSEXPORT AbstractTreeItem : public QObject
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It initializes a tree item with its parent item.

            \param parent   The parent item or NULL if it is a root item.
           */
          AbstractTreeItem(QObject* parent = 0);

          /*! Virtual destructor. */
          virtual ~AbstractTreeItem();

          //@}

          /** @name Qt AbstractTreeItem General Methods
           *  General methods for a AbstractTreeItem.
           */
          //@{

          /*!
            \brief It returns the layer associated to this AbstractTreeItem.

            \return The layer associated to this AbstractTreeItem.
           */
          te::map::AbstractLayer* getRefLayer() const;

          /*!
            \brief It returns the children list of this tree item.

            \return The list of children of this tree item.
           */
          const QList<QObject*>& getChildren() const;

          /*!
            \brief It returns the child item in the given position.

            \param pos The position of the item to be retrieved.

            \return The child item in the given position.
           */
          te::qt::widgets::AbstractTreeItem* getChild(int pos);

          /*!
            \brief It returns the index position of this item as child of the parent item.

            \return The index position of this item as child of the parent item.

            \note Use this method only if this item has a parent, otherwise, it will return an invalid index.
           */
          int getIndex() const;

          /*!
            \brief It removes the tree item in the row-th position and returns to the caller the associated item in the memory.

            \param row   The item position of the tree item to be removed.

            \return The associated item in the memory
            \param count The number of items to be removed as children of this item.
           */
          //te::qt::widgets::AbstractTreeItem* takeChild(int row);

          /*!
            \brief It removes the tree item in the given position.

            \param row The position of the item relative to its parent.

            \return It returns the reference layer associated to the removed item.

            \note The caller takes the ownership of the returned reference layer.
           */
          te::map::AbstractLayer* removeChild(int row);

          /*!
            \brief It removes the given number of items as children of this item from the given position.

            \param i     The position from which the list of items will be removed as children of this item.
            \param count The number of items to be removed as children of this item.
           */
          bool removeChildren(int i, int count);

          /*!
            \brief It inserts an item in the given position.

            \param pos   The position where the item will be inserted.
            \param item  The item to be added.
           */
          void insertChild(int i, AbstractTreeItem* item);

          /*!
            \brief It returns the child row of the given item.

            \param treeItem  The tree item

            \return The row of the layer item
           */
          int getChildRow(AbstractTreeItem* treeItem);

          /*!
            \brief It checks if the item is a layer item.

            \return The default implementation returns true indicating that the item is a layer item.
           */
          virtual bool isLayerItem() const;

          /*!
            \brief It checks if the item is a legend item.

            \return The default implementation returns false indicating that the item is not a legend item.
           */
          virtual bool isLegendItem() const;

          //@}

          /** @name Pure Virtual Methods
           *  Methods that subclasses must implement.
           */
          //@{

          /*!
            \brief It returns the data stored under the given role for this item tree.

            This method will be called by the implementation of LayerExplorerModel.

            \param role The role are used by a given view to indicate to this model which type of data it needs.

            \return The data stored under the given role for this tree item.
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
          void append(AbstractTreeItem* item);

        protected:
      
          te::map::AbstractLayer* m_refLayer;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTTREEITEM_H
