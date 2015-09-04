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
 * \file layer/explorer/GroupingItem.h
 *
 * \brief A class that represents a grouping of a layer in a LayerTreeModel.
 */
#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_GROUPINGITEM_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_GROUPINGITEM_H

#include "TreeItem.h"

namespace te
{
  namespace map
  {
    class Grouping;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
      * \class GroupingItem
      *
      * \brief A class that represents a grouping of a layer in a LayerTreeModel.
      *
      * \note The type of the item is "GROUPING".
      *
      * \ingroup widgets
      */
      class TEQTWIDGETSEXPORT GroupingItem: public TreeItem
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
        * \param g The grouping to be used.
        */
        GroupingItem(te::map::Grouping* g);

        /*!
        * \brief Destructor.
        */
        ~GroupingItem();
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

      protected:

        std::string m_label;            //!< Label to be presented on the Qt view.
        te::map::Grouping* m_grouping;  //!< Grouping being used.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_GROUPINGITEM_H
