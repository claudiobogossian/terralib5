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
  \file terralib/vp/qt/DataSetLayerItem.h

  \brief The class that represents a dataset layer item in a LayerTreeModel.
*/

#ifndef __TERRALIB_VP_QT_INTERNAL_DATASETLAYERITEM_H
#define __TERRALIB_VP_QT_INTERNAL_DATASETLAYERITEM_H

// TerraLib
#include "../../maptools/DataSetLayer.h"
#include "AbstractLayerTreeItem.h"

namespace te
{
  namespace vp
  {
    class TEVPEXPORT DataSetLayerItem : public AbstractLayerTreeItem
    {
      Q_OBJECT

      public:

        DataSetLayerItem(const te::map::AbstractLayerPtr& l, QObject* parent = 0);

        ~DataSetLayerItem();

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

        te::map::DataSetLayerPtr m_layer;
    }; 
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_VP_QT_INTERNAL_DATASETLAYERITEM_H
