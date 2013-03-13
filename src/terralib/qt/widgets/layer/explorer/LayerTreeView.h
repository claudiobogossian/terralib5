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

// Qt
#include <QtGui/QTreeView>

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
      class TEQTWIDGETSEXPORT LayerTreeView : public QTreeView
      {
        Q_OBJECT

        public:

          LayerTreeView(QWidget* parent = 0);

          ~LayerTreeView();

          std::list<AbstractLayerTreeItem*> getSelectedItems() const;

        signals:

          void activated(te::qt::widgets::AbstractLayerTreeItem* item);

          void clicked(te::qt::widgets::AbstractLayerTreeItem* item);

          void doubleClicked(te::qt::widgets::AbstractLayerTreeItem* item);

          void entered(te::qt::widgets::AbstractLayerTreeItem* item);

          void pressed(te::qt::widgets::AbstractLayerTreeItem* item);

          void toggled(te::qt::widgets::AbstractLayerTreeItem* item, bool checked);

        public slots:

          void add(const te::map::AbstractLayerPtr& layer);

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
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_EXPLORER_INTERNAL_LAYERTREEVIEW_H
