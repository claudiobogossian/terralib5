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
  \file terralib/vp/qt/FolderLayerItem.h

  \brief The class that represents a folder layer item in a LayerTreeModel.
*/

#ifndef __TERRALIB_VP_QT_INTERNAL_FOLDERLAYERITEM_H
#define __TERRALIB_VP_QT_INTERNAL_FOLDERLAYERITEM_H

// TerraLib
#include "../../maptools/FolderLayer.h"
#include "AbstractLayerTreeItem.h"

namespace te
{
  namespace vp
  {
    class TEVPEXPORT FolderLayerItem : public AbstractLayerTreeItem
    {
      Q_OBJECT

      public:

        FolderLayerItem(const te::map::AbstractLayerPtr& l, QObject* parent = 0);

        ~FolderLayerItem();

        int columnCount() const;

        QVariant data(int column, int role) const;

        QMenu* getMenu(QWidget* parent = 0) const;

        bool canFetchMore() const;

        Qt::ItemFlags flags() const;

        void fetchMore();

        bool hasChildren() const;

        bool setData(const QVariant& value, int role = Qt::EditRole);

        te::map::AbstractLayerPtr getLayer() const;

        //AbstractLayerTreeItem* clone(QObject* parent = 0);

      private:

        te::map::FolderLayerPtr m_layer;
    };
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_QT_INTERNAL_FOLDERLAYERITEM_H
