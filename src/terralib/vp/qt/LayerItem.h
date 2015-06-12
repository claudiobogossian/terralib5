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
  \file terralib/vp/qt/LayerItem.h

  \brief A class that represents a Layer in a LayerTreeModel.
*/

#ifndef __TERRALIB_VP_QT_INTERNAL_LAYERITEM_H
#define __TERRALIB_VP_QT_INTERNAL_LAYERITEM_H

// TerraLib
#include "../../qt/widgets/layer/explorer/AbstractTreeItem.h"
#include "../Config.h"

namespace te
{
  namespace vp
  {
    class PropertyGroupItem;
    class LegendGroupItem;

    class TEVPEXPORT LayerItem : public te::qt::widgets::AbstractTreeItem
    {
      Q_OBJECT

      public:

        LayerItem(te::map::AbstractLayerPtr layer, QObject* parent = 0);

        ~LayerItem();

        int columnCount() const;

        QVariant data(int column, int role) const;

        QMenu* getMenu(QWidget* parent = 0) const;

        bool canFetchMore() const;

        void fetchMore();

        Qt::ItemFlags flags() const;

        bool hasChildren() const;

        bool setData(int column, const QVariant& value, int role = Qt::EditRole);

        te::map::AbstractLayerPtr getLayer() const;

        /*!
          \brief Set the Layer as selected.

          \param selected True or False to Layer select state.
        */
        void isSelected(bool selected);

        /*!
          \brief Check if the Layer is selected.

          \return True if the Layer is selected.
        */
        bool isSelected();

        /*!
          \brief Get the selected Properties of the Layer

          \return Vector of Properties.
        */
        std::vector<te::dt::Property*> getSelected();
      
        const std::string getItemType() const;

      private:

        std::vector<te::qt::widgets::AbstractTreeItem*> m_items;
        te::map::AbstractLayerPtr m_layer;  //!< Terralib Layer of the LayerItem.
        te::vp::PropertyGroupItem* m_propertyGroup;
        te::vp::LegendGroupItem* m_legendGroup;
        bool m_selected;                    //!< Layer select state.
        bool m_onlySelecteds;               //!< Only features selected.
        bool m_OnlyLegend;
    };
  }
}

#endif // __TERRALIB_VP_QT_INTERNAL_LAYERITEM_H

