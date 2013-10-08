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
      class LayerTreeView;

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
            \brief It gets all the top level layers in the layer explorer, including the top level folder layers.

            \return List of all the top level layers in the layer explorer, including the top level folder layers.
          */
          std::list<te::map::AbstractLayerPtr> getAllTopLevelLayers() const;

          /*!
            \brief It gets all the layers in the layer explorer, including the folder layers.

            \return List of all the layers in the layer explorer, including the folder layers.
          */
          std::list<te::map::AbstractLayerPtr> getAllLayers() const;

          /*!
            \brief It gets the layers in the layer explorer, not including the folder layers.

            \return List of layers in the layer explorer, not including the folder layers.
          */
          std::list<te::map::AbstractLayerPtr> getLayers() const;

          /*!
            \brief It gets the layers in the layer explorer that are visible, not including the folder layers.

            \return List of layers in the layer explorer that are visible, not including the folder layers.
          */
          std::list<te::map::AbstractLayerPtr> getVisibleLayers() const;

          /*!
            \brief It gets all the selected items in the layer explorer.

            \return The list of selected items in the layer explorer.
          */
          std::list<AbstractTreeItem*> getSelectedItems() const;

          /*!
            \brief It gets the selected layers in the layer explorer, not including the selected folder layers.

            \return The list of selected layers in the layer explorer, not including the selected folder layers.
          */
          std::list<te::map::AbstractLayerPtr> getSelectedLayers() const;

          /*!
            \brief It gets the layers in the layer explorer that are selected and visible,
                   not including the selected and visible folder layers.

            \return List of layers in the view that are selected and visible,
                    not including selected and visible folder layers.
          */
          std::list<te::map::AbstractLayerPtr> getSelectedAndVisibleLayers() const;

        public slots:

          void add(const te::map::AbstractLayerPtr& layer);

          void remove(AbstractTreeItem* item);

        //protected:

        //  void dragEnterEvent(QDragEnterEvent* e);

        //  void dragMoveEvent(QDragMoveEvent* e);

        //  void dragLeaveEvent(QDragLeaveEvent* e);

        //  void dropEvent(QDropEvent* e);

        private:

          LayerTreeView* m_treeView;
          LayerTreeModel* m_treeModel;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_LAYEREXPLORER_H
