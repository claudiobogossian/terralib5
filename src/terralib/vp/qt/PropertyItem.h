/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/qt/PropertyItem.h

  \brief ???
*/

#ifndef __TERRALIB_VP_QT_INTERNAL_PROPERTYITEM_H
#define __TERRALIB_VP_QT_INTERNAL_PROPERTYITEM_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/layer/explorer/AbstractLayerTreeItem.h"
#include "../core/Config.h"

namespace te
{
  namespace dt { class Property; }

  namespace vp
  {
    class TEVPEXPORT PropertyItem : public te::qt::widgets::AbstractLayerTreeItem
    {
      Q_OBJECT

      public:

        PropertyItem(te::dt::Property* p, te::qt::widgets::AbstractLayerTreeItem* parent);

        ~PropertyItem();

        int columnCount() const;

        QVariant data(int column, int role) const;

        QMenu* getMenu(QWidget* parent = 0) const;

        bool canFetchMore() const;

        Qt::ItemFlags flags() const;

        void fetchMore();

        bool hasChildren() const;

        bool setData(int column, const QVariant& value, int role = Qt::EditRole);

        te::map::AbstractLayerPtr getLayer() const;

        te::dt::Property* getProperty() const;

        void setSelected(bool selected);

        bool isSelected();

      private:

        te::dt::Property* m_property;
        bool m_selected;
    }; 
  }   // end namespace vp
}     // end namespace te

#endif  // __TERRALIB_VP_QT_INTERNAL_PROPERTYITEM_H
