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
 * \file LayerItemView.h
 *
 * \brief Defines a layer item view for Qt5.
 */
#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERITEMVIEW_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERITEMVIEW_H

#include "../../Config.h"

// TerraLib
#include "../../../../maptools/AbstractLayer.h"

// Qt
#include <QTreeView>

// STL
#include <list>

// Forward declarations
class QAction;
class QStyledItemDelegate;

namespace te
{
  namespace map
  {
    class Chart;
  }

  namespace qt
  {
    namespace widgets
    {
      class LayerItemModel;
      class LayerViewMenuManager;
      class TreeItem;

      /*!
      * \class LayerItemView
      *
      * \brief A specialization of QTreeView for manipulate layers.
      *
      * This is a component used to manipulate layers in a visual way. You can add, remove, reparent items,
      * changes its visibility, add folders and some other operations over a set of layers. It can be feeded
      * by the LayersSelector or projects of TerraLib-5.
      *
      * \ingroup widgets
      *
      * \sa LayerItemModel, LayerViewDelegate, LayerViewMenuManager
      */
      class TEQTWIDGETSEXPORT LayerItemView: public QTreeView
      {
        Q_OBJECT

      public:

        /** @name Initializer Methods
        *  Methods related to instantiation and destruction.
        */
        //@{

        /*!
        * \brief Constructor.
        * \param parent Used by Qt for memory release.
        */
        LayerItemView(QWidget* parent = 0);

        /*!
        * \brief Destructor.
        */
        ~LayerItemView();
        //@}

        /** @name Tree manipulation Methods
        *  Methods related insertions and recovery of layers in the tree.
        */
        //@{

        /*!
        * \brief Add the layers to the model
        * \param layers The list of layers to be added.
        * \param idx The index of the parent item wich the must be insert.
        * \param idxPath Unused.
        */
        void addLayers(const std::list<te::map::AbstractLayerPtr>& layers, const QModelIndex& idx, const std::string& idxPath = "./");

        /*!
        * \brief Sets the list of layers. Old layers in the tree are then removed.
        * \param layers The new list of layers.
        */
        void setLayers(const std::list<te::map::AbstractLayerPtr>& layers);

        /*!
        * \brief Returs all layers in the tree including folders. 
        *
        * This method must be called when we need to store the list of layers.
        * \return List of layers in the tree.
        */
        std::list<te::map::AbstractLayerPtr> getAllLayers() const;

        /*!
        * \brief Returns just the visible layers. No folder layers are returned.
        * \return List of visible layers.
        */
        std::list<te::map::AbstractLayerPtr> getVisibleLayers() const;

        /*!
        * \brief Returns a list of TreeItem that are selected.
        * \return The list of selected TreeItem.
        */
        std::list<te::qt::widgets::TreeItem*> getSelectedItems() const;

        /*!
        * \brief Adds a folder layer to the model.
        * \param name Name of the folder.
        * \param idx Index of the parent, where the new folder must be inserted in.
        */
        void addFolder(const std::string& name, const QModelIndex& idx);

        /*!
        * \brief Updates the chart item of the element.
        * \param idx Index of the layer to add chart.
        * \note idx must contain a Layer or the operation will fail.
        */
        void updateChart(const QModelIndex& idx);

        /*!
        * \brief Updates the grouping item of the element.
        * \param idx Index of the layer to add grouping.
        * \note idx must contain a Layer or the operation will fail.
        */
        void updateGrouping(const QModelIndex& idx);

        /*!
         * \brief updateLegend
         * \param l
         */
        void updateLegend(te::map::AbstractLayer* l);
        //@}

        /** @name Popup Menu manipulation Methods
        *  Methods related to manipulation of the popup menus.
        */
        //@{

        /*!
        * \brief Adds the action to the popup menu presented when there is no layers selected.
        * \param act Action to be added.
        */
        void addNoLayerAction(QAction* act);

        /*!
        * \brief Adds the action to the popup menu presented for all kinds of selected items.
        * \param act Action to be added.
        */
        void addAllLayerAction(QAction* act);

        /*!
        * \brief Adds the action to the popup menu presented when the selected layer has vector representation.
        * \param act Action to be added.
        */
        void addVectorLayerAction(QAction* act);

        /*!
        * \brief Adds the action to the popup menu presented when the selected layer has raster representation.
        * \param act Action to be added.
        */
        void addRasterLayerAction(QAction* act);

        /*!
        * \brief Adds the action to the popup menu presented when the selected item is a folder.
        * \param act Action to be added.
        */
        void addFolderLayerAction(QAction* act);

        /*!
        * \brief Adds the action to the popup menu presented when there is multiple items selected.
        * \param act Action to be added.
        */
        void addMultipleSelectionAction(QAction* act);

        /*!
        * \brief Adds the action to the popup menu presented when the selected layer is invalid.
        * \param act Action to be added.
        */
        void addInvalidLayerAction(QAction* act);
        //@}

        /** @name Remove Methods
        *  Methods related to layers removal.
        */
        //@{

        /*!
        * \brief Removes the items in the list.
        * \param idxs List of indexes of items to be removed.
        */
        void removeItems(const QModelIndexList& idxs);

        /*!
        * \brief Removes the items that are selected.
        */
        void removeSelectedItems();
        //@}

        /** @name Tree Customization Methods
        *  Methods used to change the tree behavior.
        *
        * These methods can be used for change the Tree presentation and popup menus. 
        * You can used delegates to change presentation behavior, and use QObject to handle popup menus.
        */
        //@{

        /*!
         * \brief Removes the delegate from the tree.
         * \param d Delegate to be removed.
         *
         * \note The tree does NOT TAKE the ownership of the pointer.
         */
        void removeDelegate(QStyledItemDelegate* d);

        /*!
         * \brief Updates the popup event handler.
         *
         * This is used for change the popup menus. If you want to change the popup management try to use
         * this function. You can change all menu handling or a specific type of item.
         *
         * \param obj The new menu event handler.
         *
         * \note The tree does NOT TAKE the ownership of the pointer.
         */
        void setMenuEventHandler(QObject* obj);

        /*!
         * \brief Rerturns the current popup handler being used.
         * \return Current menu event handler.
         */
        QObject* getMenuEventHandler() const;

        /*!
         * \brief Removes the menu handler.
         *
         * Removes the menu handler from the tree.
         *
         * \param handler The handler to be removed.
         *
         * \note The tree does NOT TAKE free memory of handler object.
         */
        void removeMenuEventHandler(QObject* handler);
        //@}

      Q_SIGNALS:

        /** @name Qt signals
        *  
        * Signals emitted by LayerItemView
        */
        //@{

        /*!
         * \brief Emited when user double clicks over an AbstractLayer.
         * \param layer The layer that receives mouse double click event.
         */
        void doubleClicked(te::map::AbstractLayerPtr layer);

        /*!
         * \brief Emited when some item changes its visibility.
         */
        void visibilityChanged();

        /*!
         * \brief Emited when the selection changes.
         * \param layers The current selected layers.
         */
        void selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>& layers);

        /*!
         * \brief Emited when the order of the layers has changed.
         */
        void layerOrderChanged();
        //@}

      protected:

        /** @name Qt Reimplemented Methods
        *
        * Methods that are reimplemented of the QTreeView class.
        */
        //@{
        void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

        void dropEvent(QDropEvent * event);

        void mouseDoubleClickEvent(QMouseEvent* event);
        //@}

        LayerItemModel* m_model;          //!< Model to be used.
        LayerViewMenuManager* m_mnuMger;  //!< Default popup handler.
        QObject* m_outterFilter;          //!< Popup handler defined outside the tree.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERITEMVIEW_H
