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
  \file terralib/qt/widgets/layer/explorer/TimeSeriesLayerItem.h

  \brief The class that represents a TimeSeries layer item in a LayerTreeModel.
*/

#ifndef __TE_QT_PLUGINS_ST_INTERNAL_TIMESERIESLAYERITEM_H
#define __TE_QT_PLUGINS_ST_INTERNAL_TIMESERIESLAYERITEM_H

// TerraLib
#include "../../../st/maptools/TimeSeriesDataSetLayer.h"
#include "../../../qt/widgets/layer/explorer/AbstractTreeItem.h"

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace st
        {
        class TimeSeriesLayerItem : public te::qt::widgets::AbstractTreeItem
        {
          Q_OBJECT

          public:

            TimeSeriesLayerItem(const te::map::AbstractLayerPtr& l, QObject* parent = 0);

            ~TimeSeriesLayerItem();

            int columnCount() const;

            QVariant data(int column, int role) const;

            QMenu* getMenu(QWidget* parent = 0) const;

            bool canFetchMore() const;

            Qt::ItemFlags flags() const;

            void fetchMore();

            bool hasChildren() const;

            bool setData(int column, const QVariant& value, int role = Qt::EditRole);

            te::map::AbstractLayerPtr getLayer() const;

            const std::string getItemType() const;

          private:

            bool hasGroupingItem() const;

            bool hasChartItem() const;

          private:

            te::st::TimeSeriesDataSetLayerPtr m_layer;
        }; 
      } // end namespace st
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif  // __TE_QT_PLUGINS_ST_INTERNAL_TIMESERIESLAYERITEM_H
