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
  \file terralib/qt/widgets/dataset/explorer/DataSetTreeModel.h

  \brief A simple model for datasets belonging to a data source.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASET_EXPLORER_INTERNAL_DATASETTREEMODEL_H
#define __TERRALIB_QT_WIDGETS_DATASET_EXPLORER_INTERNAL_DATASETTREEMODEL_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../Config.h"

// Qt
#include <QtCore/QAbstractItemModel>

class QWidget;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DataSetGroupItem;

      class TEQTWIDGETSEXPORT DataSetTreeModel : public QAbstractItemModel
      {
        Q_OBJECT

        public:

          DataSetTreeModel(const te::da::DataSourceInfoPtr& datasource, QWidget* parent = 0);

          ~DataSetTreeModel();

          bool canFetchMore(const QModelIndex& parent) const;

          int columnCount(const QModelIndex& parent = QModelIndex()) const;

          QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

          void fetchMore(const QModelIndex& parent);

          Qt::ItemFlags flags(const QModelIndex& index) const;

          bool hasChildren(const QModelIndex& parent = QModelIndex()) const;

          QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

          QModelIndex parent(const QModelIndex & index) const;

          int rowCount(const QModelIndex & parent = QModelIndex()) const;

          bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

          void setCheckable(const bool checkable);

          bool isCheckable() const;

          void setForceCatalogCache(const bool force);

          bool forceCatalogCache() const;

        protected:

          /*!
            \brief It emits the dataChanged signal for the descendants indexes of the given index.

            \param parent The item index whose descendants will have the dataChanged signal emitted.
          */
          void emitDataChangedForDescendants(const QModelIndex& parent);

          /*!
            \brief It emits the dataChanged signal for the indexes that are ancestors of the given index.

            \param index The item index whose ancestors will have the dataChanged signal emitted.
          */
          void emitDataChangedForAncestors(const QModelIndex& index);


        private:

          DataSetGroupItem* m_datasets; //!< A container for the datasets of the input data source.
          bool m_checkable;             //!< If true the model allows for checkable items.
          bool m_forceCatalogCache;     //!< If true every information looked in the data source catalog is also cahed in the data source catalog.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_WIDGETS_DATASET_EXPLORER_INTERNAL_DATASETTREEMODEL_H
