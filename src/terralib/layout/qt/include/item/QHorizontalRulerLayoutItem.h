/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file QHorizontalRulerLayoutItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_QHORIZONTALRULER_LAYOUTITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_QHORIZONTALRULER_LAYOUTITEM_H

// TerraLib
#include "QObjectLayoutItem.h"

// Qt
#include <QGraphicsItem>
#include <QtGui/QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>

namespace te
{
  namespace layout
  {
    class QHorizontalRulerLayoutItem : public QObjectLayoutItem
    {
      public:

        QHorizontalRulerLayoutItem(LayoutItemController* controller = 0, LayoutObservable* o = 0);
        virtual ~QHorizontalRulerLayoutItem();

        virtual void updateObserver(ContextLayoutItem context);
    };
  }
}

#endif