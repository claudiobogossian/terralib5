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
  \file terralib/qt/widgets/datasource/explorer/DataSetGroupItem.h

  \brief A class used to group a set of dataset items in a DataSourceTreeModel.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_DATASETGROUPITEM_H
#define __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_DATASETGROUPITEM_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "AbstractDataSourceTreeItem.h"

// STL
#include <vector>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DataSetItem;

      class TEQTWIDGETSEXPORT DataSetGroupItem : public AbstractDataSourceTreeItem
      {
        Q_OBJECT

        public:

          DataSetGroupItem(const te::da::DataSourceInfoPtr& ds, AbstractDataSourceTreeItem* parent = 0);

          ~DataSetGroupItem();

          int columnCount() const;

          QVariant data(int column, int role) const;

          QMenu* getMenu(QWidget* parent = 0) const;

          bool canFetchMore() const;

          Qt::ItemFlags flags() const;

          void fetchMore();

          bool hasChildren() const;

          bool setData(const QVariant& value, int role = Qt::EditRole);

          bool isChecked() const;

          void checkState();

          const std::vector<DataSetItem*>& getDataSetItems() const;

        private:

          te::da::DataSourceInfoPtr m_ds;
          bool m_checked;
          mutable bool m_isInvalid;
          std::vector<DataSetItem*> m_items;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_DATASETGROUPITEM_H
