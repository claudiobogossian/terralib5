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
 * \file LayerItemModel.h
 *
 * \brief Defines a layer item model for Qt5.
 */
#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERITEMMODEL_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERITEMMODEL_H

#include "../../Config.h"

// TerraLib
#include "../../../../maptools/AbstractLayer.h"

// Qt
#include <QAbstractItemModel>
#include <QVariant>

// STL
#include <list>
#include <memory>

// Forward declarations
namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TreeItem;


      /*!
      * \class LayerItemModel
      *
      * \brief Defines an abstract model based on TreeItem objects.
      *
      * Models are used by the item views of the Qt5 that represents some data structure. Its used for manipulate models
      * using a view. This is part of the Model / View framework of the Qt5.
      *
      * \sa TreeItem, te::map::AbstractLayer
      */
      class TEQTWIDGETSEXPORT LayerItemModel: public QAbstractItemModel
      {
        Q_OBJECT

      public:
        /*!
        * \name Instantiation Methods
        *
        * Methods related to creation and destruction of the object.
        */
        //@{

        /*!
        * \brief Constructor.
        *
        * \param parent Model parent (Used by Qt5).
        */
        explicit LayerItemModel(QObject *parent = 0);

        /*!
        * \brief Destructor.
        */
        ~LayerItemModel();
        //@}

        /*!
        * \brief Adds a layer to the model at the end of children list.
        *
        * \param layer The layer to be added.
        *
        * \param idxPath Location where the spatial index must be generated.
        */
        void addLayer(te::map::AbstractLayerPtr layer, const std::string& idxPath = "./");

        void addLayer(te::map::AbstractLayerPtr layer, const QModelIndex& parent, const std::string& idxPath = "./");

        /*!
        * \brief Adds a list of layers to the model at the end of children list.
        *
        * \param layers The layer list to be added.
        *
        * \param idxPath Location where the spatial indexes must be generated.
        */
        void addLayers(const std::list<te::map::AbstractLayerPtr>& layers, const std::string& idxPath = "./");

        void addLayers(const std::list<te::map::AbstractLayerPtr>& layers, const QModelIndex& parent, const std::string& idxPath = "./");

        void setLayers(const std::list<te::map::AbstractLayerPtr>& layers);

        /*!
        * \brief Adds the set of items to the \a parent at the position \a pos.
        *
        * These methods is used when you needs to reparent some items. It is a common operation on the view,
        * like a drag and drop operation, for example. The \a items are removed from its parents and added
        * in the \a parent, as children.
        *
        * \param items The items to be reparent.
        *
        * \param parent The new parent, where the items must be inserted.
        *
        * \param pos The position where the items must be inserted.
        *
        * \note If \a pos is greater than the size of the list of children, items are, then, added to end of the
        * list of children.
        *
        * \note If you want to add items to the root, try to pass "0" as value for \a parent argument.
        */
        void addItems(const std::vector<TreeItem*> items, TreeItem* parent, const int& pos = 0);

        /*!
        * \brief Adds a new folder item to the model.
        *
        * \param name Label of the folder.
        *
        * \param parent Parent item. If this value is "0" the folder is added to the root of the model.
        */
        void addFolder(const std::string& name, TreeItem* parent = 0);

        void addFolder(const std::string& name, const QModelIndex& idx);

        /*!
        * \brief Get all layers contained in the model. Commonly used for persistent pourposes.
        *
        * \return A list with all layers containded in the model.
        */
        std::list<te::map::AbstractLayerPtr> getAllLayers();

        /*!
        * \brief Returns a list with layers that are visible.
        *
        * \return List of visible layers.
        *
        * \note FolderLayers are not contained in the returned list.
        */
        std::list<te::map::AbstractLayerPtr> getVisibleLayers();

        /*!
        * \name Reimplementation of QAbstractItemModel
        *
        * Methods reimplemented of the QAbstractItemModel class.
        */
        //@{
        QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

        Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

        QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

        QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

        int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

        int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

        bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex()) Q_DECL_OVERRIDE;

        Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;

        QStringList mimeTypes() const Q_DECL_OVERRIDE;

        QMimeData* mimeData(const QModelIndexList &indexes) const Q_DECL_OVERRIDE;

        bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

        bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) Q_DECL_OVERRIDE;
        //@}

        /*!
        * \brief Removes the items in \a lst from the model.
        *
        * \param lst List of indexes of the items to be removed.
        */
        void removeItems(const QModelIndexList& lst);

      protected:

        std::auto_ptr<TreeItem> m_root;   //!< The root item of the model.
      };
    }
  }
}

#endif // __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERITEMMODEL_H
