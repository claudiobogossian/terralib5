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
  \file terralib/qt/widgets/layer/explorer/LayerTreeView.h

  \brief A tree view for the layers of an application.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_LAYERTREEVIEW_H
#define __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_LAYERTREEVIEW_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"

// STL
#include <list>

// Boost
#include <boost/noncopyable.hpp>

// Qt
#include <QTreeView>

// Forward declaration
class QAction;
class QString;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class AbstractTreeItem;

      /*!
        \class LayerTreeView

        \brief A tree view for the layers of an application.

        \ingroup widgets

        \sa LayerExplorer, LayerTreeModel
      */
      class TEQTWIDGETSEXPORT LayerTreeView : public QTreeView, public boost::noncopyable
      {
        Q_OBJECT

        public:

          enum ContextMenuSelectionType
          {
            NO_ITEM_SELECTED,
            UNIQUE_ITEM_SELECTED,
            MULTIPLE_ITEMS_SELECTED
          };

          /*! \brief Constructor */
          LayerTreeView(QWidget* parent = 0);

          /*! \brief Destructor */
          ~LayerTreeView();

          /*!
            \brief It gets the items that are selected in the tree view.

            \return The list of items that are selected in the tree view.
          */
          std::list<AbstractTreeItem*> getSelectedItems() const;

          /*!
            \brief It gets the single and folder layer items that are selected in the tree view.

            \return The list of single and folder layer items that are selected in the tree view.
          */
          std::list<AbstractTreeItem*> getSelectedLayerItems() const;

          /*!
            \brief It gets the single layer items that are selected in the tree view.

            \return The list of single layer items that are selected in the tree view.
          */
          std::list<AbstractTreeItem*> getSelectedSingleLayerItems() const;

          /*!
            \brief It gets the single layers that are selected in the view.

            \return The list of single layers that are selected in the view.
          */
          std::list<te::map::AbstractLayerPtr> getSelectedSingleLayers() const;

          /*!
            \brief It gets the single layers that are selected and visible in the tree view.

            \return The list of single layers that are selected and visible in the tree view.
          */
          std::list<te::map::AbstractLayerPtr> getSelectedAndVisibleSingleLayers() const;

          /*!
            \brief It adds the action to a specified menu of a given item type when a context menu is displayed.

            \param action                The action to be associated to the context menu.
            \param menu                  The name of a submenu, using a dot separator notation. If omitted, the action will be set on a top menu item.
            \param itemType              If omitted the action will be set to all item types.
            \param menuSelectionType     The selection type of the context menu.

            \note LayerTreeView will not take the ownership of the action.
          */
          void add(QAction* action,
                   const std::string& menu = "",
                   const std::string& itemType = "",
                   ContextMenuSelectionType menuSelectionType = te::qt::widgets::LayerTreeView::UNIQUE_ITEM_SELECTED);

          /*!
            \brief It removes the action from the list of context menu.

            \param action The action to be removed from the context menu.

            \note The LayerTreeView will not destroy the action, it will only detach it from the widget.
          */
          void remove(QAction* action);

        public slots:

          void onSelectedLayersChanged(const QItemSelection& selected, const QItemSelection& deselected);

        signals:

          void activated(te::qt::widgets::AbstractTreeItem* item);

          void clicked(te::qt::widgets::AbstractTreeItem* item);

          void doubleClicked(te::qt::widgets::AbstractTreeItem* item);

          void entered(te::qt::widgets::AbstractTreeItem* item);

          void pressed(te::qt::widgets::AbstractTreeItem* item);

          void visibilityChanged(const te::map::AbstractLayerPtr& layer);

          void selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>& selectedLayers);

        protected:

          void dragEnterEvent(QDragEnterEvent* e);

          void dragMoveEvent(QDragMoveEvent* e);

          void dragLeaveEvent(QDragLeaveEvent* e);

          void dropEvent(QDropEvent* e);

          void contextMenuEvent(QContextMenuEvent* e);

        protected slots:

          void onItemActivated(const QModelIndex& index);

          void onItemClicked(const QModelIndex& index);

          void onItemDoubleClicked(const QModelIndex& index);

          void onItemEntered(const QModelIndex& index);

          void onItemPressed(const QModelIndex& index);

        private:

          class Impl;

          Impl* m_pImpl;  //!< The tree view implementation.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_LAYERTREEVIEW_H
