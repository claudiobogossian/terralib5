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
  \file terralib/qt/widgets/layer/explorer/LayerTreeModel.h

  \brief The class that defines the model used in the Qt Model/View architecture.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_EXLORER_INTERNAL_LAYERTREEMODEL_H
#define __TERRALIB_QT_WIDGETS_LAYER_EXLORER_INTERNAL_LAYERTREEMODEL_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"

// STL
#include <vector>

//Qt
#include <QtCore/QAbstractItemModel>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class AbstractLayerTreeItem;

      class TEQTWIDGETSEXPORT LayerTreeModel : public QAbstractItemModel
      {
        Q_OBJECT

        public:

          LayerTreeModel(const std::list<te::map::AbstractLayerPtr>& layers, QObject * parent = 0);

          ~LayerTreeModel();

          bool canFetchMore(const QModelIndex& parent) const;

          int columnCount(const QModelIndex& parent = QModelIndex()) const;

          QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

          bool dropMimeData(const QMimeData* data, Qt::DropAction action,
                            int row, int column, const QModelIndex& parent);

          void fetchMore(const QModelIndex& parent);

          Qt::ItemFlags flags(const QModelIndex& index) const;

          bool hasChildren(const QModelIndex& parent = QModelIndex()) const;

          QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

          //bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());

          QMimeData* mimeData(const QModelIndexList& indexes) const;

          QStringList mimeTypes() const;

          QModelIndex parent(const QModelIndex & index) const;

          int rowCount(const QModelIndex & parent = QModelIndex()) const;

          bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

          Qt::DropActions supportedDropActions() const;

          void setCheckable(const bool checkable);

          bool isCheckable() const;

          void add(const te::map::AbstractLayerPtr& layer);

        private:

          std::vector<AbstractLayerTreeItem*> m_items;
          bool m_checkable;
      };


    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te


#endif // __TERRALIB_QT_WIDGETS_LAYER_EXLORER_INTERNAL_LAYERTREEMODEL_H

