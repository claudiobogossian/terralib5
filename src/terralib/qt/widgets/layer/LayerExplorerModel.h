/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file LayerExplorerModel.h

  \brief It provides an implementation for an item model classes based on te::qt::widgets::TreeItem.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_LAYEREXPLORERMODEL_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_LAYEREXPLORERMODEL_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtCore/QAbstractItemModel>

// STL
#include <vector>

namespace te
{
// Forward declarations
  namespace map { class AbstractLayer; }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class AbstractLayerItem;

      /*!
        \class LayerExplorerModel

        \brief A Layer explorer.
       */
      class TEQTWIDGETSEXPORT LayerExplorerModel : public QAbstractItemModel
      {
        Q_OBJECT

        public:

          /*!
            \brief It constructs an item model with the given parent.

            \param layer  The layer to be explored. It must be the root of a set of layers.
            \param parent The parent object.
           */
          LayerExplorerModel(te::map::AbstractLayer* layer, QObject* parent = 0);

          /*! Destructor */
          ~LayerExplorerModel();

          /*!
            \brief It returns the index of the item in the model specified by the given row, column and parent index.

            It will call createIndex() to generate model indexes that other components
            can use to refer to items in the model.

            \param row     The item row.
            \param column  The column item.
            \param parent  The parent index of the item.

            \return The item index.
           */
          QModelIndex index(int row, int column, const QModelIndex& parent) const;

          /*!
            \brief It returns the item parent of the given index, or QModelIndex(), if it has no parent.

            \param index The item index.

            \return The parent index of the item with the given index, or QModelIndex(), if it has no parent.
           */
          QModelIndex parent(const QModelIndex& index) const;

          /*!
            \brief It returns the number of rows of the given parent.
            
            \param parent The parent index.

            \return The number of rows of the given parent.
           */
          int rowCount(const QModelIndex& parent) const;

          /*!
            \brief It returns the number of columns for the children of the given parent.

            \param parent The parent index.

            \return The number of columns for the children of the given parent.
           */
          int columnCount(const QModelIndex& parent) const;

          /*!
            \brief It returns the data stored under the given role for the item referred to by the index.

            \param index The item index.
            \param role  The role that is used by the view to indicate to the model which type of data it needs.

            \return The data stored under the given role for the item referred to by the index.
           */
          QVariant data(const QModelIndex& index, int role) const;

          /*!
            \brief It sets the role data for the item at index to value.

            \param index The item index.
            \param value The value to be assigned to the item.
            \param role  The role used.

            \return It returns true if successful; otherwise, it returns false.

            \note The dataChanged() signal is emitted if the data was successfully set.
           */
          bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

          /*!
            \brief It returns the data for the given role and section in the header with the specified orientation.
                   For horizontal headers, the section number corresponds to the column number of items shown beneath it.
                   For vertical headers, the section number typically to the row number of items shown alongside it.

            \param section      The column or row number.
            \param orientation  It specifies if the header is horizontal or vertical.
            \param role         The role used.

            \return The data for the given role and section in the header with the specified orientation.
           */
          QVariant headerData(int section, Qt::Orientation orientation, int role) const;

          /*!
            \brief It returns the item flags for the given index.

            \param index The item index.

            \return The item flags for the given index.
           */
          Qt::ItemFlags flags(const QModelIndex& index) const;

          /*!
            \brief It returns the drop actions supported by this model.

            \return The drop actions supported by this model.
           */
          Qt::DropActions supportedDropActions() const;

          /*!
            \brief It returns a list of MIME types that can be used to describe a list of model indexes.

            \return The list of MIME types that can be used to describe a list of model indexes.
           */          
          QStringList mimeTypes() const;

          /*!
            \brief It returns an object that contains serialized items of data corresponding to the list of indexes specified.
                   The formats used to describe the encoded data is obtained from the mimeTypes() function.

            \param indexes The list of indexes that was dragged.

            \return An object containing serialized items of data corresponding to the list of indexes specified.
                    If the list of indexes is empty, or there are no supported MIME types,
                    0 is returned rather than a serialized empty list.                 
           */     
          QMimeData* mimeData(const QModelIndexList& indexes) const;

          /*!
            \brief It handles the data supplied by a drag and drop operation that ended with the given action.

            \param data   The object containing serialized items of data.
            \param action The action associated by the drag and drop operation.
            \param row    The item row in the model where the operation ended.
            \param column The item column in the model where the operation ended.
            \param parent The item parent in the model where the operation ended.

            \return It returns true if the data and action can be handled by the model; otherwise, it returns false.                 
           */
          bool dropMimeData(const QMimeData* data, Qt::DropAction action, 
                            int row, int column, const QModelIndex& parent);

          /*!
            \brief It inserts "count" rows into the model before the given row.
                   The items in the new row will be children of the item represented by the parent model index.

            \param row The rows will be inserted before this row.
            \param count The number of rows to be inserted.
            \param parent The parent of the inserted rows.

            \return It returns true if the rows were successfully inserted; otherwise, it returns false.              
           */
          bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());

          /*!
            \brief It remove "count" rows into the model before the given row.

            \param row The initial row.
            \param count The number of rows to be removed.
            \param parent The parent of the removed rows.

            \return It returns true if the rows were successfully removed; otherwise, it returns false.              
           */
          bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

          /*!
            \brief Get the item associated to the given index.

            \param index The item index.

            \return It returns the item associated to the given index.              
           */
          te::qt::widgets::AbstractLayerItem* getItem(const QModelIndex& index);

          /*! \brief It resets the model to its original state in any attached views. */
          void resetModel();

          void setItemsToBeInserted(std::vector<te::qt::widgets::AbstractLayerItem*> items); //Lauro

        signals:

          void dragDropEnded(const QModelIndex& dragIndex, const QModelIndex& dropIndex);

        protected:

         /*!
            \brief It emits the dataChanged signal for the descendants indexes of the given index.

            \param index The index whose descendants will have the dataChanged signal emitted.
          */
          void dataChangedForDescendantsIndexes(const QModelIndex& index);

        private:

          te::qt::widgets::AbstractLayerItem* m_rootItem;            //!< The pointer to the root item.
          mutable QModelIndex m_dragIndex;                           //!< The index of the dragged item
          bool m_dndOperation;                                       //!< Flag indicating drag and drop operation.

          std::vector<te::qt::widgets::AbstractLayerItem*> m_childItemsToBeInserted;  //!< A vector of the children items to be inserted.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_LAYEREXPLORERMODEL_H
