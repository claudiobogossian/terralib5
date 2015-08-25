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
 * \file layer/explorer/TreeItem.h
 *
 * \brief Defines an hierarquical structure.
 */

#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_TREEITEM_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_TREEITEM_H

#include "../../Config.h"

// TerraLib
#include "../../../../common/Exception.h"
#include "../../../../common/GenericQueue.h"

// Qt
#include <qnamespace.h>
#include <QString>

// STL
#include <memory>
#include <vector>

// Forward declarations
namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TreeItem;
      typedef te::common::QueueT<TreeItem*> Queue;

      /*!
      * \enum VISIBLE
      *
      * \brief Defines the visibility of an item.
      */
      enum VISIBLE
      {
        NONE,       //!< Not visible.
        TOTALLY,    //!< Visible.
        PARTIALLY   //!< Partially visible.
      };

      /*!
      * \class TreeItem
      *
      * \brief Defines a hierarchical structure.
      *
      * \ingroup widgets
      */
      class TEQTWIDGETSEXPORT TreeItem
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
        * \param type The type of the item.
        */
        TreeItem(const std::string& type);

        /*!
        * \brief Destructor.
        */
        virtual ~TreeItem();
        //@}

        /*!
        * \brief Updates the item parent.
        *
        * \param item The new parent.
        */
        void setParent(TreeItem* item);

        /*!
        * \brief Adds a child to the item. The child is added to the end of the list.
        *
        * \param item The child to be inserted.
        */
        void addChild(TreeItem* item);

        /*!
        * \brief Inserts a child item at the desired position.
        *
        * \param item The child to be inserted.
        *
        * \param pos The position of the child in the children list.
        *
        * \exception If \a pos is greater than the children list size, a te::common::Exception will be raised.
        */
        void insertChild(TreeItem* item, const size_t& pos) throw (te::common::Exception);

        /*!
        * \brief Returns the child located at \a pos.
        *
        * \param pos The position of the desired child at the children list.
        *
        * \return The child item located at \a pos.
        *
        * \exception If \a pos is greater or equal than the children list size, a te::common::Exception will be raised.
        */
        TreeItem* getChild(const size_t& pos) const throw (te::common::Exception);

        /*!
        * \brief Returns the item parent.
        *
        * \return A pointer to the parent.
        */
        TreeItem* getParent() const;

        /*!
        * \brief Removes the child located at \a pos from the children list.
        *
        * \param pos The position of the desired child at the children list.
        *
        * \return The removed item.
        *
        * \exception If \a pos is greater or equal than the children list size, a te::common::Exception will be raised.
        */
        TreeItem* removeChild(const size_t& pos) throw (te::common::Exception);

        /*!
        * \brief Returns the number of children.
        *
        * The \a type, defines the type of children we want to considere in the operation. If an empty string is given as
        * argument, all kinds of children will be considered.
        *
        * \param type The type of children we want to calculate the number.
        *
        * \return Number of children of type \a type.
        */
        size_t getChildrenCount(const std::string& type) const;

        /*!
         * \brief Returns all children of a certain type. 
         * \param[out] items The set of items founded.
         * \param type The type of items that we are searching for.
         */
        void getChildren(std::vector<TreeItem*>& items, const std::string& type) const;

        /*!
        * \brief Tells us if the item has children or not.
        *
        * \return \a True if children list is not empty and false otherwise.
        */
        bool hasChildren() const;

        /*!
        * \brief Returns the label of the item to be presented in a Qt view.
        *
        * \return The item label, to be presented in a Qt view.
        */
        virtual std::string getAsString() const = 0;

        /*!
        * \brief Returns the position of item in its parent's list of children.
        *
        * \return The position of item in its parent's list of children.
        */
        int getPosition();

        /*!
        * \brief Returns the visibilty state of the item.
        *
        * \return The state of visibilty of the item.
        */
        virtual VISIBLE isVisible() const;

        /*!
        * \brief Updates the visibilty state of the item.
        *
        * \param visible The new visibility state.
        *
        * \param updateAncestors Updates also ancestors states.
        *
        * \param updateDescendents Updates also descendents states.
        */
        virtual void setVisible(const VISIBLE& visible, const bool& updateAncestors = false, const bool& updateDescendents = false);

        /*!
        * \brief Returns the type of the item.
        *
        * \return Type of the item.
        */
        std::string getType() const;

        /*!
         * \brief Returns the item tooltip (for information purposes).
         *
         * \return The item tooltip.
        */
        std::string getToolTip() const;

        /*!
        * \brief Removes all children.
        */
        void removeAllChilds();

        /*!
         * \brief Returns the flags to be used by the model.
         *
         * \return Qt flags. By default itens are enabled, editable and selectable. If you want to change the flags reimplement this method
         * in a subclass.
         */
        virtual Qt::ItemFlags flags();

      protected:

        std::string m_type;               //!< Item type.
        std::auto_ptr<Queue> m_children;  //!< Queue of items (children).
        TreeItem* m_parent;               //!< Pointer to the parent.
        std::string m_toolTip;            //!< Tooltip (for information purposes).
      };
    }
  }
}


#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_TREEITEM_H
