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
 * \file LayerViewDelegate.h
 *
 * \brief A class for rendering icons for layer and folder itens.
 */
#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERVIEWDELEGATE_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERVIEWDELEGATE_H

#include "../../Config.h"

// TerraLib
#include "../../../../common/Decorator.h"

// Qt
#include <QIcon>
#include <QStyledItemDelegate>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT LayerViewDelegate: public te::common::Decorator<QStyledItemDelegate>
      {
        Q_OBJECT

      public:

        LayerViewDelegate(QStyledItemDelegate* decorated, QObject* parent = 0);

        ~LayerViewDelegate();

        void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

      protected:

        QIcon getIcon(const QModelIndex & index) const;

        QIcon m_layer;
        QIcon m_folder;
        QIcon m_colorMap;
        QIcon m_chart;
        QIcon m_queryLayer;
        QIcon m_tabularLayer;
        QIcon m_invalidLayer;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERVIEWDELEGATE_H
