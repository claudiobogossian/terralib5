/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file DataGridModel.h

  \brief It provides an interface for the data grid model associated to te::qt::widgets::DataGridView.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATAGRIDMODEL_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATAGRIDMODEL_H

// TerraLib
#include "../Config.h"

//Qt
#include <QtCore/QAbstractTableModel>

namespace te
{
  namespace map { class DataGridOperation; }

  namespace da
  {
    class DataSet;
    class DataSetType;
  }


  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DataGridModel

        \brief This class provides a standard interface for a model that represents their data as a data grid.
       */
      class TEQTWIDGETSEXPORT DataGridModel : public QAbstractTableModel
      {
        Q_OBJECT

        public:

          /*!
            \brief It constructs a data grid model with the given parent widget.

            \param dt          The dataset type associated to the dataset.
            \param dataSet     The dataset whose contents will be modeled.
            \param dataGridOp  The class that represents the operations that can be made on the data grid.
            \param parent      The parent object.
           */
          DataGridModel(te::da::DataSetType* dt, te::da::DataSet* dataSet,
                        te::map::DataGridOperation* dataGridOp = 0, QObject* parent = 0);

          /*!
            \brief It constructs a data grid model with the given parent widget.

            \param dataGridOp  The class that represents the operations that can be made on the data grid.
            \param parent      The parent object.
           */
          DataGridModel(te::map::DataGridOperation* dataGridOp = 0, QObject* parent = 0);

          /*!
            \brief It initializes the data grid model with the data that will be modeled.

            \param dt          The dataset type associated to the dataset.
            \param dataSet     The dataset whose contents will be modeled.
            */
          void init(te::da::DataSetType* dt, te::da::DataSet* dataSet);

          /*! \brief Destructor. */
          ~DataGridModel();

          /*!
            \brief It sets the data set type.
            
            \param dt The data set type.
           */
          void setDataSetType(te::da::DataSetType* dt) { m_dataSetType = dt; }

          /*!
            \brief It sets the data set.
            
            \param dataSet The data set.
           */
          void setDataSet(te::da::DataSet* dataSet) { m_dataSet = dataSet; }

          /*!
            \brief It returns the number of rows under the given parent.

            \param parent The parent index.

            \return The number of rows under the given parent.
           */
          int rowCount(const QModelIndex& parent) const;

          /*!
            \brief It returns the number of clumns under the given parent.

            \param parent The parent index.

            \return The number of columns under the given parent.
           */
          int columnCount(const QModelIndex& parent) const;

          /*!
            \brief It returns the data stored under the given role for the item referred to by the index.

            \param index The item index.
            \param role  The data type.

            \return The data stored under the given data type (role) for the item given by its index.
           */
          QVariant data(const QModelIndex& index, int role) const;

          /*!
            \brief It sets the role data to value for the item at index.

            \param index The item index.
            \param role  The data type.

            \return True, if successful; otherwise, it returns false.

            \note The dataChanged() signal should be emitted if the data was successfully set.
           */
          bool setData(const QModelIndex& index, const QVariant& value, int role);

          /*!
            \brief It returns the data for the given role and section in the header with the specified orientation.

            \param section     For horizontal headers, the section is a column; for vertical headers, the section is a row.
            \param orientation Qt::Horizontal or Qt::Vertical.
            \param role        The data type.

            \return The data for the given role and section in the header with the specified orientation.
           */
          QVariant headerData(int section, Qt::Orientation orientation, int role) const;

          /*!
            \brief It returns the item flags for the given index.

            \param index The item index.

            \return The item flags for the given index.

            \note The flags can be combined, so the item can be enabled and selected, for example.
           */
          Qt::ItemFlags flags(const QModelIndex& index) const;

          /*!
            \brief It resets the model to its original state in any attached views.
           */
          void resetModel();

          /*!
            \brief It returns the data grid operation.

            \return The data grid operation.
           */
          te::map::DataGridOperation* getDataGridOperation() { return m_dataGridOp; }

         /*!
            \brief It sets the data grid operation.

            \param dataGridOpThe data grid operation.
           */
          void setDataGridOperation(te::map::DataGridOperation* dataGridOp) { m_dataGridOp = dataGridOp; } 

        private:
          te::da::DataSetType* m_dataSetType;         //!< The data set whose data will be the data grid contents.
          te::da::DataSet*     m_dataSet;             //!< The data set whose data will be the data grid contents.
          te::map::DataGridOperation* m_dataGridOp;   //!< The data grid associated to the model.
      };
    }
  }
}

#endif
