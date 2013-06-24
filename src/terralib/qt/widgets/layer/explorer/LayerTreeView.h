/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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
#include <QtGui/QTreeView>

// Forward declaration
class QAction;
class QString;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class AbstractLayerTreeItem;

      /*!
        \class LayerTreeView

        \brief A tree view for the layers of an application.

        \sa LayerExplorer, LayerTreeViewModel
      */
      class TEQTWIDGETSEXPORT LayerTreeView : public QTreeView, public boost::noncopyable
      {
        Q_OBJECT

        public:

          enum ContextMenuType
          {
            ALL_SELECTION_TYPES,
            NO_LAYER_SELECTED,
            SINGLE_LAYER_SELECTED,
            MULTIPLE_LAYERS_SELECTED
          };

          LayerTreeView(QWidget* parent = 0);

          ~LayerTreeView();

          std::list<AbstractLayerTreeItem*> getSelectedItems() const;

          QModelIndexList getSelectedIndexes() const;

        signals:

          void activated(te::qt::widgets::AbstractLayerTreeItem* item);

          void clicked(te::qt::widgets::AbstractLayerTreeItem* item);

          void doubleClicked(te::qt::widgets::AbstractLayerTreeItem* item);

          void entered(te::qt::widgets::AbstractLayerTreeItem* item);

          void pressed(te::qt::widgets::AbstractLayerTreeItem* item);

          void visibilityChanged(te::qt::widgets::AbstractLayerTreeItem* item);

        public slots:

          void add(const te::map::AbstractLayerPtr& layer);

          void remove(const QModelIndex& indexes);

        public:

          /*!
            \brief It adds the action to a specified menu of a given layer type when a context menu is displayed.

            \param action    The action to be associated to the context menu.
            \param menu      The name of a submenu, using a dot separator notation. If omitted the action will be set on a top menu item.
            \param layerType If omitted the action will be set to all type of layers.
            \param menuType  The type of selection to what this action will be displayed.

            \note LayerTreeView will not take the action ownership.
          */
          void add(QAction* action,
                   const QString& menu = QString(""),
                   const QString& layerType = QString(""),
                   ContextMenuType menuType = SINGLE_LAYER_SELECTED);

          /*!
            \brief It removes the action from the list of context menu.

            \param action The action to be removed from the context menu.

            \note LayerTreeView will not destroy the action, it will only detach it from widget.
          */
          void remove(QAction* action);

        protected slots:

          void itemActivated(const QModelIndex & index);

          void itemClicked(const QModelIndex & index);

          void itemDoubleClicked(const QModelIndex & index);

          void itemEntered(const QModelIndex & index);

          void itemPressed(const QModelIndex & index);

        protected:

          void dragEnterEvent(QDragEnterEvent* e);

          void dragMoveEvent(QDragMoveEvent* e);

          void dragLeaveEvent(QDragLeaveEvent* e);

          void dropEvent(QDropEvent* e);

          void contextMenuEvent(QContextMenuEvent* e);

        private:

          class Impl;

          Impl* m_pImpl;  //!< The tree view implementation.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_LAYERTREEVIEW_H
