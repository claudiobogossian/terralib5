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
  \file MovingItemGroup.h
   
   \brief Class that represents the grouping of objects of moving QGraphicsItems, MVC components.
   Its used to move 2...n QGraphicsItems that has inverted QMatrix.
   Its coordinate system is the same of scene (millimeters). 
   This is also son of ItemGroup, ItemObserver and ObjectItem, so it can become observer of a model (Observable). 

  \ingroup layout
*/

#ifndef	__TERRALIB_LAYOUT_INTERNAL_MOVINGITEMGROUP_H
#define __TERRALIB_LAYOUT_INTERNAL_MOVINGITEMGROUP_H

//TerraLib
#include "../../core/Config.h"
#include "ItemGroup.h"

namespace te
{
  namespace layout
  {

    /*!
      \file MovingItemGroup.h
   
       \brief Class that represents the grouping of objects of moving QGraphicsItems, MVC components.
       Its used to move 2...n QGraphicsItems that has inverted QMatrix.
       Its coordinate system is the same of scene (millimeters). 
       This is also son of ItemGroup, ItemObserver and ObjectItem, so it can become observer of a model (Observable). 

      \ingroup layout
    */
    class TELAYOUTEXPORT MovingItemGroup : public ItemGroup
    {
    public:

      MovingItemGroup(te::layout::ItemController* controller, te::layout::Observable* o);

      virtual ~MovingItemGroup();

      virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    };
  }
}

#endif

