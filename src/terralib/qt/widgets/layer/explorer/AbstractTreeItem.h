/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/explorer/AbstractTreeItem.h

  \brief The class that represents an item in a LayerTreeModel.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_ABSTRACTTREEITEM_H
#define __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_ABSTRACTTREEITEM_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"

//Qt
#include <QModelIndex>
#include <QObject>
#include <QVariant>

// Forward declaration
class QMenu;
class QWidget;

// STL
#include <vector>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class AbstractTreeItem

        \brief The class that represents an item in a LayerTreeModel.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT AbstractTreeItem : public QObject
      {
        Q_OBJECT

        public:

          AbstractTreeItem(QObject* parent = 0);

          virtual ~AbstractTreeItem();

          std::vector<te::qt::widgets::AbstractTreeItem*> getDescendants();

          std::vector<te::qt::widgets::AbstractTreeItem*> getAncestors();

          // Return the number of columns for the children of the item
          virtual int columnCount() const = 0;

          // Return the data stored under the given role
          virtual QVariant data(int column, int role) const = 0;

          virtual QMenu* getMenu(QWidget* parent = 0) const = 0;

          virtual bool canFetchMore() const = 0;

          virtual void fetchMore() = 0;

          virtual Qt::ItemFlags flags() const = 0;

          virtual bool hasChildren() const = 0;

          virtual bool setData(int column, const QVariant& value, int role = Qt::EditRole) = 0;

          virtual te::map::AbstractLayerPtr getLayer() const = 0;

          /*!
            \brief It returns the item type.

            \return The item type.
          */
          virtual const std::string getItemType() const = 0;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_ABSTRACTTREEITEM_H
