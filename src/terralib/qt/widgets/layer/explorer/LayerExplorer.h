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
  \file terralib/qt/widgets/layer/explorer/LayerExplorer.h

  \brief A widget designed to explore the layers of an application.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_LAYEREXPLORER_H
#define __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_LAYEREXPLORER_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"
#include "LayerTreeView.h"

// STL
#include <list>

// Qt
#include <QtGui/QDockWidget>
#include <QModelIndex>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class AbstractTreeItem;
      class LayerTreeModel;

      /*!
        \class LayerExplorer

        \brief A widget designed to explore the layers of an application.

        \sa LayerTreeView, LayerTreeModel
      */
      class TEQTWIDGETSEXPORT LayerExplorer : public QDockWidget
      {
        Q_OBJECT

        public:

          LayerExplorer(QWidget* parent = 0, Qt::WindowFlags flags = 0);

          ~LayerExplorer();

          LayerTreeView* getTreeView() const;

          LayerTreeModel* getTreeModel() const;

          void set(const std::list<te::map::AbstractLayerPtr>& layers);

          /*!
            \brief It gets the top level layers in the tree model associated to the layer explorer.

            \return The list of the top level layers in the tree model associated to the layer explorer.
          */
          std::list<te::map::AbstractLayerPtr> getTopLayers() const;

          /*!
            \brief It gets all the selected items in the layer explorer.

            \return The list of selected items in the layer explorer.
          */
          std::list<AbstractTreeItem*> getSelectedItems() const;

          /*!
            \brief It gets all the selected layers(single and folders) in the layer explorer.

            \return The list of all the selected layers(single and folders) in the layer explorer.
          */
          std::list<AbstractTreeItem*> getSelectedLayerItems() const;

          /*!
            \brief It gets the single layer items that are selected in the tree view.

            \return The list of single layer items that are selected in the tree view.
          */
          std::list<AbstractTreeItem*> getSelectedSingleLayerItems() const;

          /*!
            \brief It gets the single layers that are selected.

            \return The list of single layers that are selected.
          */
          std::list<te::map::AbstractLayerPtr> getSelectedSingleLayers() const;

          /*!
            \brief It gets the single layers that are selected and visible.

            \return The list of single layers that are selected and visible.
          */
          std::list<te::map::AbstractLayerPtr> getSelectedAndVisibleSingleLayers() const;

          void add(const te::map::AbstractLayerPtr& layer);

          void remove(AbstractTreeItem* item);

          te::qt::widgets::LayerTreeView::ContextMenuType getMenuType(int menuType) const;

          /*!
            \brief It calls the action to a specified menu of a given layer type when a context menu is displayed.

            \param action    The action to be associated to the context menu.
            \param menu      The name of a submenu, using a dot separator notation. If omitted the action will be set on a top menu item.
            \param layerType If omitted the action will be set to all type of layers.
            \param menuType  The type of selection to what this action will be displayed.

            \note The associated LayerTreeView will not take the action ownership.
          */
          void add(QAction* action,
                   const QString& menu = QString(""),
                   const QString& layerType = QString(""),
                   te::qt::widgets::LayerTreeView::ContextMenuType menuType = te::qt::widgets::LayerTreeView::SINGLE_LAYER_SELECTED);

          signals:

            void selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>& selectedLayers);

            void visibilityChanged(const te::map::AbstractLayerPtr& layer);

            void layerOrderChanged();

            void doubleClicked(te::qt::widgets::AbstractTreeItem* item);

          public slots:

          /*!
            \brief It sends out a signal indicating that there were changes in the selection of layers in the tree view.

            \param selectedLayers  The new list of selected layers.
          */
          void onSelectedLayersChanged(const std::list<te::map::AbstractLayerPtr>& selectedLayers);

          /*!
            \brief It sends out a signal indicating that the visibility of the given single layer was changed in the tree view.
          */
          void onVisibilityChanged(const te::map::AbstractLayerPtr& layer);

          /*!
            \brief It sends out a signal indicating that there was a drag and operation on the tree view.
          */
          void onLayerOrderChanged();

          /*!
            \brief It sends out a signal indicating that the given item was double clicked.

            \param item The tree item that was double clicked.
          */
          void onItemDoubleClicked(te::qt::widgets::AbstractTreeItem* item);

          /*!
            \brief It expands the specified model item.

            \param item The model item to be expanded.
          */
          void expand(te::qt::widgets::AbstractTreeItem* item);

          /*!
            \brief It collapses the specified model item.

            \param item The model item to be collapsed.
          */
          void collapse(te::qt::widgets::AbstractTreeItem* item);

        private:

          LayerTreeView*  m_treeView;    //!< The tree view for the layers
          LayerTreeModel* m_treeModel;   //!< the tree model for the layers
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_LAYEREXPLORER_H
