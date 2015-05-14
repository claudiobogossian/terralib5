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
  \file terralib/qt/widgets/datasource/explorer/PrimaryKeyItem.h

  \brief A class that represents a primary key in a TreeModel.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_PRIMARYKEYITEM_H
#define __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_PRIMARYKEYITEM_H

// TerraLib
#include "AbstractDataSourceTreeItem.h"

namespace te
{
  namespace da { class PrimaryKey; }

  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT PrimaryKeyItem : public AbstractDataSourceTreeItem
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param pk     The primary-key to be showned. If it is associated to a dataset the item will not take its ownership, otherwise, it will.
            \param parent The parent item in the tree or NULL if it is a root item.
          */
          PrimaryKeyItem(te::da::PrimaryKey* pk, AbstractDataSourceTreeItem* parent);

          ~PrimaryKeyItem();

          int columnCount() const;

          QVariant data(int column, int role) const;

          QMenu* getMenu(QWidget* parent = 0) const;

          bool canFetchMore() const;

          Qt::ItemFlags flags() const;

          void fetchMore();

          bool hasChildren() const;

          bool setData(const QVariant& value, int role = Qt::EditRole);

          te::da::PrimaryKey* getPrimaryKey() const;

        private:

          te::da::PrimaryKey* m_pk;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASOURCE_EXPLORER_INTERNAL_PRIMARYKEYITEM_H
