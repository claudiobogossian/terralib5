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
  \file terralib/qt/widgets/datasource/explorer/DataSetItem.h

  \brief A class that represents a dataset in a TreeModel.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_DATASETITEM_H
#define __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_DATASETITEM_H

// TerraLib
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "AbstractDataSourceTreeItem.h"

namespace te
{
  namespace da { class DataSource; } 
  
  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT DataSetItem : public AbstractDataSourceTreeItem
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param dt     The dataset type to be showned. If it is associated to a data source catalog the item will not take its ownership, otherwise, it will.
            \param ds     If the dataset type is not associated to a data source catalog you can explicitly inform a data source to be used to query more information about the dataset.
            \param parent The parent item in the tree or NULL if it is a root item.
          */
          DataSetItem(const te::da::DataSetTypePtr& dt, const std::string& geomPropertyName, te::da::DataSource* ds = 0, AbstractDataSourceTreeItem* parent = 0);

          ~DataSetItem();

          int columnCount() const;

          QVariant data(int column, int role) const;

          QMenu* getMenu(QWidget* parent = 0) const;

          bool canFetchMore() const;

          Qt::ItemFlags flags() const;

          void fetchMore();

          bool hasChildren() const;

          bool setData(const QVariant& value, int role = Qt::EditRole);

          const te::da::DataSetTypePtr& getDataSet() const;

          const std::string& getGeomPropertyName() const;

          te::da::DataSource* getDataSource() const;

          bool isChecked() const;

        private:

          te::da::DataSetTypePtr m_dataset;
          std::string m_geomPropertyName;
          te::da::DataSource* m_datasource;
          bool m_checked;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_DATASETITEM_H
