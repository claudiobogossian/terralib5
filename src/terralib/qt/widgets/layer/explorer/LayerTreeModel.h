/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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

  \brief This class defines the model used in the Qt Model/View architecture for the tree of layers.
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
      class AbstractTreeItem;

      class TEQTWIDGETSEXPORT LayerTreeModel : public QAbstractItemModel
      {
        Q_OBJECT

        public:

          /*!
            \brief It constructs a layer tree model with the given parent.

            \param parent The parent object of the layer tree model.
          */
          LayerTreeModel(QObject* parent = 0);

          /*!
            \brief It constructs a layer tree model with the given parent.

            \param layers The layers that will be associated to the layer items of the layer tree model.
            \param parent The parent object of the layer tree model.
          */
          LayerTreeModel(const std::list<te::map::AbstractLayerPtr>& layers, QObject* parent = 0);

          /*! Destructor */
          ~LayerTreeModel();

          /*!
            \brief It resets the model and associates the new top level items to the given top level layers.

            \param layers The list of top level layers that will be associated to the new top level items.
          */
          void set(const std::list<te::map::AbstractLayerPtr>& layers);

          /*!
            \brief It gets the top level layers of the model.

            \param layers The top level layers of the model.
          */
          const std::vector<te::map::AbstractLayerPtr>& getTopLayers() const;

          /*!
            \brief It gets the top level layer items of the model.

            \param layers The top level layer items of the model.
          */
          const std::vector<te::qt::widgets::AbstractTreeItem*>& getTopLayerItems() const;

          /*!
            \brief It fetches more data for the given parent.

            \param parent The object used to fetch more data.

            \return It returns true if there is more data available for parent.
          */
          bool canFetchMore(const QModelIndex& parent) const;

          /*!
            \brief It fetches any available data for the items with the parent specified by the parent index.

            \param parent The object used to fetch available data.
          */
          void fetchMore(const QModelIndex& parent);

          /*!
            \brief It returns the number of columns for the children of the given parent.

            \param parent The parent index.

            \return The number of columns for the children of the given parent.
          */
          int columnCount(const QModelIndex& parent = QModelIndex()) const;

          /*!
            \brief It returns the number of rows of the given parent.
            
            \param parent The parent index.

            \return The number of rows of the given parent.
          */
          int rowCount(const QModelIndex & parent = QModelIndex()) const;

          /*!
            \brief It returns the index of the item in the model specified by the given row, column and parent index.

            It will call createIndex() to generate model indexes that other components
            can use to refer to items in the model.

            \param row     The item row.
            \param column  The item column.
            \param parent  The parent index of the item.

            \return The item index.
          */
          QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

          /*!
            \brief It returns the item parent of the given index, or QModelIndex(), if it has no parent.

            \param index The item index.

            \return The parent index of the item with the given index, or QModelIndex(), if it has no parent.
          */
          QModelIndex parent(const QModelIndex & index) const;

          /*!
            \brief It returns the item flags for the given index.

            \param index The item index.

            \return The item flags for the given index.
          */
          Qt::ItemFlags flags(const QModelIndex& index) const;

          /*!
            \brief It returns the data stored under the given role for the item referred to by the index.

            \param index The item index.
            \param role  The role that is used by the view to indicate to the model which type of data it needs.

            \return The data stored under the given role for the item referred to by the index.
          */
          QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

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
            \brief It checks if the given index has children.

            \param parent The item index.

            \return It returns true if the given index has children.
          */
          bool hasChildren(const QModelIndex& parent = QModelIndex()) const;

          /*!
            \brief It returns a list of MIME types that can be used to describe a list of model indexes.

            \return The list of MIME types that can be used to describe a list of model indexes.
          */
          QStringList mimeTypes() const;

          /*!
            \brief It returns the drop actions supported by this model.

            \return The drop actions supported by this model.
          */
          Qt::DropActions supportedDropActions() const;
        
#if (QT_VERSION >= 0x050000)
          Qt::DropActions supportedDragActions() const;
#endif

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
            \brief It inserts a determined number of rows given by the "count" parameter,
                   starting with the given row under the parent "parent" from the model.

            \param row    The initial row.
            \param count  The number of rows to be inserted.
            \param parent The parent of the rows to be inserted.

            \return True, if the rows were successfully inserted; otherwise, it returns false.
          */
          bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());

          /*!
            \brief It removes a determined number of rows given by the "count" parameter,
                   starting with the given row under parent "parent" from the model.

            \param row    The initial row.
            \param count  The number of rows to be removed.
            \param parent The parent of the rows to be removed.

            \return It returns true if the rows were successfully removed; otherwise, it returns false.
          */
          bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

          /*!
            \brief It sets the model items as checkable or not.

            \param checkable Flag that indicates if the model items are checkable.
           */
          void setCheckable(const bool checkable);

          /*!
            \brief It verifies if the model items are checkable or not.

            \return It returns true if the model items are checkable.
          */
          bool isCheckable() const;

          /*!
            \brief It gets the index that is associated to the given item.

            \param item The item whose index will be returned.

            \return The index that is associated to the item.
          */
          QModelIndex getIndex(AbstractTreeItem* item);

          /*!
            \brief It gets the layer item that is associated to the given layer.

            \param layer The layer whose the corresponding layer item will be returned.

            \return The layer item associated to the layer.
          */
          te::qt::widgets::AbstractTreeItem* getLayerItem(const te::map::AbstractLayerPtr& layer);

          /*!
            \brief It adds a item layer to the list of item layers of the parent layer item.
                   If the parent layer is not specified, the item layer is added as a top layer item.

            \param layer       The layer that will be associated to the item layer to be added.
            \param parentItem  The parent item where the layer item will be added.
          */
          void add(const te::map::AbstractLayerPtr& layer, AbstractTreeItem* parentItem = 0);

          /*!
            \brief It removes a item from the model.

            \param item The item to be removed from the model.

            \return True, if the item was successfully removed; otherwise, it returns false.
          */
          bool remove(AbstractTreeItem* item);

        signals:

          void visibilityChanged(const te::map::AbstractLayerPtr& layer);
          //void expandItem(te::qt::widgets::AbstractTreeItem* item);
          void layerOrderChanged();

        protected:

          /*!
            \brief It removes the given layers from the list of children of their parents.

            \param layers The layers that will be removed from the list of children of their parents.
          */
          void removeLayerFromParentChildrenList(std::vector<te::map::AbstractLayerPtr>& layers);

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

          std::vector<AbstractTreeItem*> m_items;                    //!< The list of top level items.
          std::vector<te::map::AbstractLayerPtr> m_layers;           //!< The list of top level layers.
          std::vector<te::map::AbstractLayerPtr> m_insertingLayers;  //!< The list of layers to be inserted.
          bool m_checkable;                                          //!< It informs if the model is checkable.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_WIDGETS_LAYER_EXLORER_INTERNAL_LAYERTREEMODEL_H
