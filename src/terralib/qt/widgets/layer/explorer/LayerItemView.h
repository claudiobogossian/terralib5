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

namespace te
{
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
      * \sa LayerItemModel, LayerViewDelegate, LayerViewMenuManager
      */
      class TEQTWIDGETSEXPORT LayerItemView: public QTreeView
      {
        Q_OBJECT

      public:

        /*!
        * \brief 
        * \param parent
        */
        LayerItemView(QWidget* parent = 0);

        /*!
        * \brief
        */
        ~LayerItemView();

        /*!
        * \brief 
        * \param layers
        * \param idx
        * \param idxPath
        */
        void addLayers(const std::list<te::map::AbstractLayerPtr>& layers, const QModelIndex& idx, const std::string& idxPath = "./");

        void setLayers(const std::list<te::map::AbstractLayerPtr>& layers);

        /*!
        * \brief 
        * \return
        */
        std::list<te::map::AbstractLayerPtr> getAllLayers() const;

        /*!
        * \brief 
        * \return
        */
        std::list<te::map::AbstractLayerPtr> getVisibleLayers() const;

        /*!
        * \brief 
        * \return
        */
        std::list<te::qt::widgets::TreeItem*> getSelectedItems() const;

        /*!
        * \brief 
        * \param name
        */
        void addFolder(const std::string& name, const QModelIndex& idx);

        /*!
        * \brief
        * \param
        */
        void addNoLayerAction(QAction* act);

        /*!
        * \brief
        * \param
        */
        void addAllLayerAction(QAction* act);

        /*!
        * \brief
        * \param
        */
        void addVectorLayerAction(QAction* act);

        /*!
        * \brief
        * \param
        */
        void addRasterLayerAction(QAction* act);

        /*!
        * \brief
        * \param
        */
        void addFolderLayerAction(QAction* act);

        /*!
        * \brief
        * \param
        */
        void addMultipleSelectionAction(QAction* act);

        /*!
        * \brief
        * \param
        */
        void removeItems(const QModelIndexList& idxs);

        /*!
        * \brief
        * \param
        */
        void removeSelectedItems();

      protected:

        LayerItemModel* m_model;          //!<
        LayerViewMenuManager* m_mnuMger;  //!<
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERITEMVIEW_H
