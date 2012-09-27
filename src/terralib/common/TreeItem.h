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
  \file TreeItem.h

  \brief This abstract class defines the interface to organize items in a tree-oriented way.  
 */

#ifndef __TERRALIB_COMMON_INTERNAL_TREEITEM_H
#define __TERRALIB_COMMON_INTERNAL_TREEITEM_H

// TerraLib
#include "Config.h"
#include "STLUtils.h"

// STL
#include <cassert>
#include <list>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace common
  {
    /*!
      \class TreeItem

      \brief This abstract class describes a basic item to be organized in a tree-oriented way.
      
      A tree item can have several descendants (children) and only one parent.
      This data structure will clear all the memory used by the items.

      \sa Layer, Project
     */
    template<class T> class TreeItem : public boost::noncopyable
    {
      public:

        typedef typename std::list<TreeItem*>::iterator iterator;
        typedef typename std::list<TreeItem*>::const_iterator const_iterator;

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new item having a parent.
          
          Its parent will have this new item attached as its child automatically.

          \param parent The parent item of this new item.

          \note If a parent exists, it will take the ownership of this new item.
         */
        explicit TreeItem(TreeItem* parent);

        /*! \brief Virtual destructor. */
        virtual ~TreeItem();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns a pointer to the parent of this node.

          \return It returns a pointer to the parent node.

          \note You can´t free the memory pointed by the returned pointer; otherwise, the result will be unpredictable.
         */
        TreeItem* getParent() const;        

        /*!
          \brief It returns the n-th child.

          \param i The child index.

          \return It returns a pointer to the specified child node.
            
          \note You can´t free the memory pointed by the returned pointer; otherwise, the result will be unpredictable.
         */
        TreeItem* getChild(std::size_t i) const;

        /*!
          \brief It returns the n-th child.

          \param i The child index.

          \return It returns a pointer to the specified child node.
            
          \note You can´t free the memory pointed by the returned pointer; otherwise, the result will be unpredictable.
         */
        TreeItem* operator[](std::size_t i) const;

        /*!
          \brief It adds the child item to the list of children of this item. If the child item already has a parent,
          it will be disconnected from it and will be attached to this one.

          \param childItem The item to be added as a child of this item.

          \note This item will take the ownership of the pointer.

          \note Never pass a NULL pointer to this method.
         */
        void push_back(TreeItem* childItem);

        /*!
          \brief It adds(appends) a item in the end of the list of children. If the item has a parent,
                 it will be disconnected from it and will be attached to this one.

          \param item The item to be added.

          \note This item will take the ownership of the pointer of the given item.

          \note Never pass a NULL pointer to this method.
         */
        void addChild(TreeItem* item);

        /*!
          \brief It inserts a item in the position. If the item has a parent, 
           it will be disconnected from it and will be attached to this one.

          \param i    The position where the item will be inserted.
          \param item The item to be inserted.

          \note This item will take the ownership of the pointer of the given item.

          \note Never pass a NULL pointer to this method.
         */
        void insertChild(std::size_t pos, TreeItem* item);

        /*!
          \brief It takes the child in the i-th position by disconnecting it from the list of children.

          \param i The position of the item to be taken.

          \return The item taken.

          \note The caller will get the ownership of the node taken.

         */
        TreeItem* takeChild(std::size_t i);

        /*!
          \brief It takes from the given position a certain number of items.
                 The items to be taken will be disconnected from this item.

          \param i     The position from where the items will be taken as children of this item.
          \param count The number of items to be removed as children of this item.

          \return The list of items that was taken.

          \note The caller will get the ownership of the nodes that were taken.

         */
        std::list<TreeItem*> takeChildren(std::size_t i, std::size_t count);        

        /*!
          \brief It replaces the child item at the given position by the new one.
                 The replaced item will be disconnected from this item.

          \param i         The item position where the replace operation will be taken.
          \param childItem The new child item that will replace the item in the i-th position. 

          \return The item that was replaced.

          \note The caller will get the ownership of the replaced item. 
         */
        TreeItem* replace(std::size_t i, TreeItem* childItem);

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

        /*! \brief It disconnects this item from its parent, if it has one.
        
            \note The caller wiil get the ownership of this item. 
        */
        void disconnect();

        /*!
          \brief It returns the index of this item in the list of children of its parent item.

          \return The index of this item in the list of children of its parent item. It returns
                  a negative value (-1), if it has no parent.
         */
        int getIndex() const;

        /*!
          \brief It swaps the position of the given children.

          \param firstChild  One of the children.
          \param secondChild The other child.
         */
        void swap(TreeItem* firstChild, TreeItem* secondChild);

        /*!
          \brief It swaps this item with its sibling in their parent node.

          \param sibling The sibling to swap.
         */
        void swap(TreeItem* sibling);

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
        
        //@}

      protected:

        /** @name Auxiliary Methods
         */
        //@{

        /*!
          \brief It sets the parent of this item to the specified one. Don´t use this method if
                 you are not sure how to handle all the pointers involved in this operation.

          \param parent The item to be set as parent of this item.

          \note Avoid calling this method; instead, use addChild and the constructors.

          \note TreeItem will take the ownership of the pointer.
         */
        void setParent(TreeItem* parent);

        //@}

      protected:

        TreeItem* m_parent;                //!< A pointer to the parent node.
        std::list<TreeItem*> m_children;   //!< Pointers to the items that are children.
    };

    template<class T> inline
    TreeItem<T>::TreeItem(TreeItem<T>* parent)
      : m_parent(parent)
    {
      if(m_parent)
        m_parent->m_children.push_back(this);
    }

    template<class T> inline
    TreeItem<T>::~TreeItem()
    {
      FreeContents(m_children);
    }

    template<class T> inline
    TreeItem<T>* TreeItem<T>::getParent() const
    {
      return m_parent;
    }    

    template<class T> inline
    TreeItem<T>* TreeItem<T>::getChild(std::size_t i) const
    {
      assert(i < m_children.size());

      const_iterator it = begin();
      std::advance(it, i);

      return *it;
    }

    template<class T> inline
    TreeItem<T>* TreeItem<T>::operator[](std::size_t i) const
    {
      return getChild(i);
    }

    template<class T> inline
    void TreeItem<T>::push_back(TreeItem<T>* childItem)
    {
      assert(childItem);

      childItem->setParent(this);
    }

    template<class T> inline
    void TreeItem<T>::addChild(TreeItem<T>* item)
    {
      push_back(item);
    }

    template<class T> inline
    void TreeItem<T>::insertChild(std::size_t pos, TreeItem<T>* item)
    {
      assert(pos <= m_children.size());
      assert(item);

      item->disconnect();

      item->m_parent = this;

      iterator it = begin();
      std::advance(it, pos);

      m_children.insert(it, item); 
    }

    template<class T> inline
    TreeItem<T>* TreeItem<T>::takeChild(std::size_t i)
    {
      assert(i < m_children.size());

      iterator it = begin();

      std::advance(it, i);

      TreeItem* item = *it;
      item->m_parent = 0;

      m_children.erase(it);

      return item;
    } 

    template<class T> inline
    std::list<TreeItem<T>*> TreeItem<T>::takeChildren(std::size_t i, std::size_t count)
    {
      assert(i < m_children.size());

      iterator it, firstIt, lastIt;

      firstIt = begin();
      std::advance(firstIt, i);

      lastIt = firstIt;
      std::advance(lastIt, count);

      std::list<TreeItem*> itemsList;
      for (it = firstIt; it != lastIt; ++it)
      {
        TreeItem* item = *it;
        item->m_parent = 0;
        itemsList.push_back(item);
      }

      m_children.erase(firstIt, lastIt);

      return itemsList;
    }    

    template<class T> inline
    TreeItem<T>* TreeItem<T>::replace(std::size_t i, TreeItem<T>* childItem)
    {
      assert(i < (int)m_children.size());
      assert(childItem);

      TreeItem* replacedItem = getChild(i);
      replacedItem->disconnect();

      insertChild(i, childItem);

      return replacedItem;
    }

    template<class T> inline
    std::size_t TreeItem<T>::getChildrenCount() const
    {
      return m_children.size();
    }

    template<class T> inline
    void TreeItem<T>::getDescendantsCount(std::size_t& count) const
    {
      count += m_children.size();

      const_iterator it;
      for (it = begin(); it != end(); ++it)
      {
        TreeItem* item = *it;
        item->getDescendantsCount(count);
      }
    }

    template<class T> inline
    void TreeItem<T>::disconnect()
    {
      if (m_parent == 0)
        return;

      int index = getIndex();
      if (index < 0)
        return;

      iterator it = m_parent->begin();
      std::advance(it, index);

      m_parent->m_children.erase(it);
      m_parent = 0;
    }

    template<class T> inline
    int TreeItem<T>::getIndex() const
    {
      if (m_parent == 0)
        return -1;

      std::list<TreeItem*>& children = m_parent->m_children;

      const_iterator it;
      int k = 0;

      for (it = children.begin(); it != children.end(); ++it)
      {
        TreeItem* item = *it;

        if (item == this)
          break;

        ++k;
      }  

      return k;
    }

    template<class T> inline
    void TreeItem<T>::swap(TreeItem<T>* firstChild, TreeItem<T>* secondChild)
    {
      assert(firstChild->getParent() == this &&
             secondChild->getParent() == this);

      std::size_t fidx = firstChild->getIndex();
      std::size_t sidx = secondChild->getIndex();

      iterator firstIt = begin();
      std::advance(firstIt, fidx);

      iterator secondIt = begin();
      std::advance(secondIt, sidx);

      *firstIt = secondChild;
      *secondIt = firstChild;
    }

    template<class T> inline
    void TreeItem<T>::swap(TreeItem<T>* sibling)
    {
      if (m_parent)
        m_parent->swap(this, sibling);
    }

    template<class T> inline
    typename TreeItem<T>::const_iterator TreeItem<T>::begin() const
    {
      return m_children.begin();
    }
    
    template<class T> inline
    typename TreeItem<T>::iterator TreeItem<T>::begin()
    {
      return m_children.begin();
    }

    template<class T> inline
    typename TreeItem<T>::const_iterator TreeItem<T>::end() const
    {
      return m_children.end();
    }
    
    template<class T> inline
    typename TreeItem<T>::iterator TreeItem<T>::end()
    {
      return m_children.end();
    }

    template<class T> inline
    void TreeItem<T>::setParent(TreeItem<T>* parent)
    {
      if (m_parent == parent)
        return;

      if (m_parent)
        disconnect();

      m_parent = parent;

      if (parent)
        parent->m_children.push_back(this);
    }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_TREEITEM_H
