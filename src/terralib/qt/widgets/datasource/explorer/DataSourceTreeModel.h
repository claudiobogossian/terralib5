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
  \file terralib/qt/widgets/datasource/explorer/DataSourceTreeModel.h

  \brief ????
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_DATASOURCETREEMODEL_H
#define __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_DATASOURCETREEMODEL_H

// TerraLib
#include "../../Config.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"

// STL
#include <vector>

//Qt
#include <QtCore/QAbstractItemModel>

class QWidget;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declaration
      class AbstractDataSourceTreeItem;

      class TEQTWIDGETSEXPORT DataSourceTreeModel : public QAbstractItemModel
      {
        Q_OBJECT

        public:

          /*!
            \brief This lists all data sources.
          */
          DataSourceTreeModel(const std::vector<te::da::DataSourceInfoPtr>& datasources, QWidget* parent);

          /*!
            \brief This lists all datasets from the data source.
          */
          DataSourceTreeModel(const te::da::DataSourceInfoPtr& ds, QObject* parent = 0);

          ~DataSourceTreeModel();

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

        private:

          std::vector<AbstractDataSourceTreeItem*> m_items;
          bool m_checkable;
      };


    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te


#endif // __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_DATASOURCETREEMODEL_H

