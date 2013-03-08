/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file DataSetTableModel.h

  \brief A table model for a dataset.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATASETTABLEMODEL_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATASETTABLEMODEL_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtCore/QAbstractTableModel>

namespace te
{
  namespace da { class DataSet; }

  namespace map { class DataSetTable; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DataSetTableModel

        \brief A tree view for the data sources of an application.

        \sa DataSetTableView
      */
      class TEQTWIDGETSEXPORT DataSetTableModel : public QAbstractTableModel
      {
        Q_OBJECT

        public:

          DataSetTableModel(te::da::DataSet* dataset, QObject* parent = 0);

          ~DataSetTableModel();

          int columnCount(const QModelIndex& parent = QModelIndex()) const;

          QVariant data(const QModelIndex& index, int role) const;

          QVariant headerData(int section, Qt::Orientation orientation, int role) const;

          /*QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;*/

          int rowCount(const QModelIndex& parent = QModelIndex()) const;

          void setDataSet(te::da::DataSet* dataset);

        private:

          std::auto_ptr<te::map::DataSetTable> m_table;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_DATASETTABLEMODEL_H
