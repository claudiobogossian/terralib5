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
  \file terralib/vp/qt/LayerTreeModel.h

  \brief The class that defines the model used in the Qt Model/View architecture
*/

#ifndef __TERRALIB_VP_QT_INTERNAL_LAYERTREEMODEL_H
#define __TERRALIB_VP_QT_INTERNAL_LAYERTREEMODEL_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/layer/explorer/AbstractTreeItem.h"
#include "../Config.h"

// STL
#include <vector>

//Qt
#include <QtCore/QAbstractItemModel>

namespace te
{
  namespace vp
  {
    class TEVPEXPORT LayerTreeModel : public QAbstractItemModel
    {
      Q_OBJECT

      public:

        /*!
          \brief It constructs an item model with the given parent.

          \param layers The layers which will be associated to the model indexes.
          \param parent The parent object.
          */
        LayerTreeModel(const std::list<te::map::AbstractLayerPtr>& layers, bool singleSelection = false, QObject * parent = 0);

        /*! Destructor */
        ~LayerTreeModel();

        /*!
          \brief It fetches more data for parent.

          \param parent The object used to fetch more data.

          \return It returns true if there is more data available for parent.
        */
        bool canFetchMore(const QModelIndex& parent) const;

        /*!
          \brief It returns the number of columns for the children of the given parent.

          \param parent The parent index.

          \return The number of columns for the children of the given parent.
        */
        int columnCount(const QModelIndex& parent = QModelIndex()) const;

        /*!
          \brief It returns the data stored under the given role for the item referred to by the index.

          \param index The item index.
          \param role  The role that is used by the view to indicate to the model which type of data it needs.

          \return The data stored under the given role for the item referred to by the index.
        */
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

        /*!
          \brief It fetches any available data for the items with the parent specified by the parent index.

          \param parent The object used to fetch available data.
        */
        void fetchMore(const QModelIndex& parent);

        /*!
          \brief It returns the item flags for the given index.

          \param index The item index.

          \return The item flags for the given index.
        */
        Qt::ItemFlags flags(const QModelIndex& index) const;

        /*!
          \brief It checks if the given index has children.

          \param parent The item index.

          \return It returns true if the given index has children.
        */
        bool hasChildren(const QModelIndex& parent = QModelIndex()) const;

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
          \brief It returns the number of rows of the given parent.
            
          \param parent The parent index.

          \return The number of rows of the given parent.
        */
        int rowCount(const QModelIndex & parent = QModelIndex()) const;

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
          \brief Returns the data for the given role and section in the header with the specified orientation.

          \param section      The section number corresponds to the column number.
          \param orientation  This type is used to signify an object's orientation.
          \param role         The role used.

          \return The data for the given role and section in the header with the specified orientation.
        */
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        /*!
          \brief It get the Layer selected and the Properties selected of the Layer

          \return It returns a map of Layers end Properties selected.
        */
        std::map<te::map::AbstractLayerPtr, std::vector<te::dt::Property*> > getSelected();

        void setLayerList(const std::list<te::map::AbstractLayerPtr>& layers);

      private:

        std::vector<te::qt::widgets::AbstractTreeItem*> m_items;
        bool m_singleSelection;  //!< If is single selection or multiselection
    };
  }   // end namespace vp
}     // end namespace te


#endif // __TERRALIB_VP_QT_INTERNAL_LAYERTREEMODEL_H

