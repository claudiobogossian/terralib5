/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/table/DataSetTableModel.h

  \brief A model based on te::da::DataSet.
*/

#ifndef __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEMODEL_H
#define __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEMODEL_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtCore/QAbstractTableModel>

// STL
#include <vector>

namespace te
{
  // Forward declarations
  namespace da
  {
    class DataSet;
    class ObjectId;
  }

  namespace qt
  {
    namespace widgets
    {
      // Forward declaration
      class Promoter;

      /*!
        \class DataSetTableModel

        \brief A table model representing a te::da::DataSet.
      */
      class TEQTWIDGETSEXPORT DataSetTableModel : public QAbstractTableModel
      {
        public:

          /*!
            \brief Constructor.
          */
          DataSetTableModel(QObject* parent = 0);

          /*! 
            \brief Virtual destructor. 
          */
          virtual ~DataSetTableModel();

          /*!
            \brief Updates the data being used. 

            \param dset The new data set to be used.

            \note This method DOES TAKE the ownership of \a dset.
          */
          void setDataSet(te::da::DataSet* dset);

          /*!
            \brief Sets the columns used as pkeys, for presentation purposes.

            \param pkeys Positions of the columns that form the primary key.
          */
          void setPkeysColumns(const std::vector<size_t>& pkeys);

          /*!
            \brief Enables or disables promotion fo the rows.

            \param enable True for enable promotion, false for disable it.

            \note This operation consumes time and memory resources, because the all primary keys must be processed.
          */
          void setPromotionEnable(const bool& enable);

          /*!
            \brief Promotes the rows identified by \a oids.

            \parama oids The identifiers of the rows to be promoted.
          */
          void promote(const std::vector<te::da::ObjectId*>& oids);

          /*!
            \brief Returns true if promotion is enabled, false otherwise.

            \return True if promotion is enabled and false otherwise.
          */
          bool isPromotionEnabled();

          /*!
            \brief Returns the pointer to the promoter being used.

            \return Pointer of the promoter being used.

            \note The caller of this method DOES NOT take the ownership of the pointer.
          */
          Promoter* getPromoter();

          void showOIdsVisible(const bool& visible);

          /*!
            \name QAbstractTableModel re-implementation methods.

            \brief Re-implementation of QAbstractTableModel methods.
          */
          //@{

          int rowCount(const QModelIndex & parent) const;

          int columnCount(const QModelIndex & parent) const;

          QVariant data(const QModelIndex & index, int role) const;

          QVariant headerData(int section, Qt::Orientation orientation, int role) const;

          Qt::ItemFlags flags(const QModelIndex & index) const;

          bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
          //@}

        protected:

          te::da::DataSet* m_dataset;         //!< The dataset being used.

          mutable int m_currentRow;           //!< An internal row pointer.

          std::vector<size_t> m_pkeysColumns; //!< Primary key columns.

          Promoter* m_promoter;               //!< Promoter to be used.

          bool m_OIdsVisible;                 //!< Oids icon visibility.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEMODEL_H
