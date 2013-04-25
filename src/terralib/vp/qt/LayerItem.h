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
  \file terralib/vp/qt/LayerItem.h

  \brief ????
*/

#ifndef __TERRALIB_VP_QT_INTERNAL_LAYERITEM_H
#define __TERRALIB_VP_QT_INTERNAL_LAYERITEM_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/layer/explorer/AbstractLayerTreeItem.h"
#include "../core/Config.h"

//Qt
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QVariant>

// Forward declaration
class QMenu;
class QWidget;

namespace te
{
  namespace vp
  {
    class TEVPEXPORT LayerItem : public te::qt::widgets::AbstractLayerTreeItem
    {
      Q_OBJECT

      public:

        LayerItem(te::map::AbstractLayerPtr layer, QObject* parent = 0);

        virtual ~LayerItem();

        // Returns the number of columns for the children of the item
        virtual int columnCount() const;

        //Returns the data stored under the given role
        //http://doc.qt.nokia.com/4.7/qt.html#ItemDataRole-enum
        virtual QVariant data(int column, int role) const;

        virtual QMenu* getMenu(QWidget* parent = 0) const;

        virtual bool canFetchMore() const;

        virtual void fetchMore();

        virtual Qt::ItemFlags flags() const;

        virtual bool hasChildren() const;

        virtual bool setData(const QVariant& value, int role = Qt::EditRole);

        virtual te::map::AbstractLayerPtr getLayer() const;

        //virtual AbstractLayerTreeItem* clone(QObject* parent = 0) = 0;

      private:

        te::map::AbstractLayerPtr m_layer;
    };
  }
}

#endif // __TERRALIB_VP_QT_INTERNAL_LAYERITEM_H