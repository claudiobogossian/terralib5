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
  \file BuildGraphicsItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_BUILD_GRAPHICS_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_BUILD_GRAPHICS_ITEM_H

//TerraLib
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/Coord2D.h"
#include "EnumMode.h"
#include "Properties.h"

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class BuildGraphicsItem 
    {
      public:

        BuildGraphicsItem();
        virtual ~BuildGraphicsItem();
        
        QGraphicsItem* rebuildItem(te::layout::Properties* props);
        QGraphicsItem* createItem(te::layout::LayoutMode mode, const te::gm::Coord2D& coordinate);

      protected:

        QGraphicsItem* createPaper();
        QGraphicsItem* createMap();
        QGraphicsItem* createMapGrid();
        QGraphicsItem* createText();
        QGraphicsItem* createRectangle();
	      QGraphicsItem* createLegend();

        te::gm::Coord2D findCoordinate(te::layout::Properties* props);

        int findZValue(te::layout::Properties* props);

      protected:

        te::gm::Coord2D m_coord;
        te::layout::Properties* m_props;
        int m_zValue;
    };
  }
}

#endif