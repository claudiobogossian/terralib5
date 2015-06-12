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
  \file terralib/vp/qt/LegendGroupItem.h

  \brief A class that represents a group of legends in a LayerTreeModel.
*/

#ifndef __TERRALIB_VP_QT_INTERNAL_LEGENDGROUPITEM_H
#define __TERRALIB_VP_QT_INTERNAL_LEGENDGROUPITEM_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/layer/explorer/AbstractTreeItem.h"
#include "../Config.h"

namespace te
{
  namespace dt { class Property; }

  namespace se { class Rule; }

  namespace vp
  {
    class TEVPEXPORT LegendGroupItem : public te::qt::widgets::AbstractTreeItem
    {
      Q_OBJECT

      public:

        LegendGroupItem(std::vector<te::se::Rule*> rules, AbstractTreeItem* parent);

        ~LegendGroupItem();

        int columnCount() const;

        QVariant data(int column, int role) const;

        QMenu* getMenu(QWidget* parent = 0) const;

        bool canFetchMore() const;

        Qt::ItemFlags flags() const;

        void fetchMore();

        bool hasChildren() const;

        bool setData(int column, const QVariant& value, int role = Qt::EditRole);

        te::map::AbstractLayerPtr getLayer() const;

        std::vector<te::qt::widgets::AbstractTreeItem*> getItems() const;

        const std::string getItemType() const;

      private:

        std::vector<te::qt::widgets::AbstractTreeItem*> m_items;
    }; 
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_QT_INTERNAL_LEGENDGROUPITEM_H
