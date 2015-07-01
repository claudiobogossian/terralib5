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
  \file terralib/qt/widgets/layer/explorer/GroupingItem.h

  \brief A class that represents a grouping of a layer in a LayerTreeModel.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_GROUPINGITEM_H
#define __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_GROUPINGITEM_H

// TerraLib
#include "AbstractTreeItem.h"

namespace te
{
// Forward declaration
  namespace map { class Grouping; }

  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT GroupingItem : public AbstractTreeItem
      {
        Q_OBJECT

        public:

          GroupingItem(te::map::Grouping* grouping, QObject* parent = 0);

          ~GroupingItem();

          int columnCount() const;

          QVariant data(int column, int role) const;

          QMenu* getMenu(QWidget* parent = 0) const;

          bool canFetchMore() const;

          Qt::ItemFlags flags() const;

          void fetchMore();

          bool hasChildren() const;

          bool setData(int column, const QVariant& value, int role = Qt::EditRole);

          te::map::AbstractLayerPtr getLayer() const;

        /*!
          \brief It returns the item type: "GROUPING_ITEM".

          \return The item type: "GROUPING_ITEM".
        */
          const std::string getItemType() const;

          void setCheckable(bool checkable);

          bool getCheckable();

        private:

          te::map::Grouping* m_grouping;
          bool m_isCheckable;
          bool m_isChecked;

      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_GROUPINGITEM_H
