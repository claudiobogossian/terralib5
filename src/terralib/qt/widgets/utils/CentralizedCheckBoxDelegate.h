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
  \file CentralizedCheckBoxDelegate.h

  \brief Just use a delegate to render checkboxes centralized on a view.
*/
#ifndef __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_CENTRALIZEDCHECKBOXDELEGATE_H
#define __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_CENTRALIZEDCHECKBOXDELEGATE_H

#include "../Config.h"

// Qt
#include <QStyledItemDelegate>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT CentralizedCheckBoxDelegate : public QStyledItemDelegate
      {
        public:

          CentralizedCheckBoxDelegate(QObject* parent=0);

          ~CentralizedCheckBoxDelegate();

          void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

          bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_UTILS_INTERNAL_CENTRALIZEDCHECKBOXDELEGATE_H
