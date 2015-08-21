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
  \file ObjectItem.h
   
   \brief Abstract class that represents a graphic item. This object is of type QGraphicsObject. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OBJECT_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_OBJECT_ITEM_H

// Qt
#include <QGraphicsObject>
#include <QPixmap>
#include <QVariant>
#include <QPainter>

// TerraLib
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../../geometry/Envelope.h"
#include "../../core/enum/AbstractType.h"
#include "../../../color/RGBAColor.h"
#include "../../core/Config.h"
#include "ParentItem.h"

class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;
class QStyleOptionGraphicsItem;
class QWidget;

namespace te
{
  namespace layout
  {
    /*!
    \brief Abstract class that represents a graphic item. This object is of type QGraphicsObject.
    
    \ingroup layout

    \sa te::layout::ItemObserver
  */
    class TELAYOUTEXPORT ObjectItem : public ParentItem<QGraphicsObject>
    {
      Q_OBJECT //for slots/signals

      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        ObjectItem(ItemController* controller = 0, Observable* o = 0, bool invertedMatrix = false);

        /*!
          \brief Destructor
        */
        virtual ~ObjectItem();
    };
  }
}

#endif
