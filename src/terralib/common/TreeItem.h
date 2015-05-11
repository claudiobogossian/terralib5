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
  \file terralib/common/TreeItem.h

  \brief A base class for data organized as a tree.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_COUNTEDTREEITEM_H
#define __TERRALIB_COMMON_INTERNAL_COUNTEDTREEITEM_H

// TerraLib
#include "Config.h"
#include "Counted.h"

// STL
#include <list>

// Boost
#include <boost/intrusive_ptr.hpp>

namespace te
{
  namespace common
  {
// Forward declaration
    class TreeItem;

    typedef boost::intrusive_ptr<TreeItem> TreeItemPtr;

    /*!
      \class TreeItem

      \brief This abstract class describes a basic item to be organized in a tree-oriented way.

      A tree item can have several descendants (children) and only one parent.
      This data structure will clear all the memory used by the items.

      This implementation is based in the counted pattern, allowing to share the ownership of the items.

      \ingroup common

      \sa AbstractLayer
    */
    class TECOMMONEXPORT TreeItem : public Counted
    {
      public:

        typedef std::list<TreeItemPtr>::iterator iterator;
        typedef std::list<TreeItemPtr>::const_iterator const_iterator;

        /*!
          \brief It initializes a new item having a parent.

          Its parent will have this new item attached as its child automatically.

          \param parent The parent item of this new item.
        */
        explicit TreeItem(TreeItem* parent = 0);

        /*!
          \brief It initializes a new item having a parent.

          Its parent will have this new item attached as its child automatically.

          \param parent The parent item of this new item.
        */
        explicit TreeItem(const TreeItemPtr& parent);

        /*! \brief Virtual destructor. */
        virtual ~TreeItem();

        /*!
          \brief It returns true if the item has descendants.

          \return True if the item has descendants.
        */
        bool hasChildren() const;

        /*!
          \brief It returns a pointer to the parent of this node.

          \return It returns a pointer to the parent node.

          \note You can´t free the memory pointed by the returned pointer; otherwise, the result will be unpredictable.
        */
        TreeItem* getParent() const;

        /*!
          \brief It returns the children of this tree item.

          \return It returns the children list of this tree item.
        */
        const std::list<TreeItemPtr>& getChildren() const;

        /*!
          \brief It returns the n-th child.

          \param i The child index.

          \return It returns a pointer to the specified child node.
        */
        const TreeItemPtr& getChild(std::size_t i) const;

        /*!
          \brief It returns the n-th child.

          \param i The child index.

          \return It returns a pointer to the specified child node.
        */
        const TreeItemPtr& operator[](std::size_t i) const;

        /*!
          \brief It adds (appends) the item to the end of the children's list.

          If the child item already has a parent,
          it will be disconnected from it and will be attached to this one.

          \param childItem The item to be added as a child of this item.

          \pos The children's list will be increased with one element.
        */
        void add(const TreeItemPtr& childItem);

        /*!
          \brief It inserts an item in the informed position.

          If the item has a parent,
          it will be disconnected from it and will be attached to this one.

          \param i    The position where the item will be inserted.
          \param item The item to be inserted.

          \pos The children's list will be increased with one element.
        */
        void insert(std::size_t i, const TreeItemPtr& childItem);

        /*!
          \brief It removes the i-th child.

          \param i The position of the item to be removed.

          \return The removed item.

          \pos The children's list will be decreased by one element.
        */
        TreeItemPtr remove(std::size_t i);

        /*!
          \brief It removes from the given position a certain number of items as children of this item.
                 The items to be removed will be disconnected from this item.

          \param i     The position from where the items will be removed.
          \param count The number of items to be removed (> 0).

          \return The list of removed items.
        */
        std::list<TreeItemPtr> remove(std::size_t i, std::size_t count);

        /*!
          \brief It replaces the child item at the given position by the new one.
                 The replaced item will be disconnected from this item.

          \param i         The item position where the replace operation will be taken.
          \param childItem The new child item that will replace the item in the i-th position. 

          \return The item that was replaced.
        */
        TreeItemPtr replace(std::size_t i, const TreeItemPtr& childItem);

        /*!
          \brief It checks if the given layer is sibling of this one.
          
          \param layer The layer to be compared to.

          \return It returns true if the layers are siblings.
        */
        bool isSibling(const TreeItem* item) const;

        /*!
          \brief It returns the number of children of this node.

          \return The number of direct descendants of this node.
        */
        std::size_t getChildrenCount() const;

        /*!
          \brief It returns the number of nodes that descends from this node.

          \param count The number of nodes that descends from this node.

          \note It will return the number of nodes in a sub-tree. So, if you want to count
                how many elements there are in a tree, just call this method for the root item.

          \note The count parameter must be initialized with zero.
        */
        void getDescendantsCount(std::size_t& count) const;

        /*! \brief It disconnects this item from its parent, if it has one. */
        void disconnect();

        /*!
          \brief It returns the index of this item in the list of children of its parent item.

          \return The index of this item in the list of children of its parent item. It returns
                  npos, if it has no parent.
        */
        std::size_t getIndex() const;

        /*!
          \brief It swaps the position of the given children.

          \param firstChild  One of the children.
          \param secondChild The other child.
        */
        void swap(const TreeItemPtr& firstChild, const TreeItemPtr& secondChild);

        /*!
          \brief It swaps this item with its sibling in their parent node.

          \param sibling The sibling to swap.
        */
        void swap(const TreeItemPtr& sibling);

        /*!
          \brief It returns the constant iterator associated to the first child of this item.

          \return The constant iterator associated to the first child of this item.
        */
        const_iterator begin() const;

        /*!
          \brief It returns the iterator associated to the first child of this item.

          \return The iterator associated to the first child of this item.
        */
        iterator begin();

        /*!
          \brief It returns the constant iterator that refers to one past the end of the children of this item.

          \return The constant iterator that refers to one past the end of the children of this item.
        */
        const_iterator end() const;

        /*!
          \brief It returns the iterator that refers to one past the children of this item.

          \return The iterator that refers to one past the children of this item.
        */
        iterator end();

      protected:

        /*!
          \brief It sets the parent of this item to the specified one. Don't use this method if
                 you are not sure how to handle all the pointers involved in this operation.

          \param parent The item to be set as parent of this item.

          \note Avoid calling this method; instead, use add or the constructors.
        */
        void setParent(TreeItem* parent);

      public:

        static const std::size_t npos;  //!< A value to indicate no match.

      protected:

        TreeItem* m_parent;                  //!< A pointer to the parent node.
        std::list<TreeItemPtr> m_children;   //!< Pointers to the items that are children.
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_COUNTEDTREEITEM_H
