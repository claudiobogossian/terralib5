/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file MapGridItem.h
   
  \brief Class daughter of te::layout::MapItem that contains a grid that can be viewed on the map.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_GRID_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_MAP_GRID_ITEM_H

// TerraLib
#include "MapItem.h"

class QPainter;

namespace te
{
  namespace layout
  {
    class Observable;
    class ItemController;

    /*!
      \brief Class daughter of te::layout::MapItem that contains a grid that can be viewed on the map.
	  
	    \ingroup layout

	    \sa te::layout::MapItem
	  */
    class MapGridItem : public MapItem
    {
      Q_OBJECT //for slots/signals

      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        MapGridItem( ItemController* controller, Observable* o );

        /*!
          \brief Destructor
         */
        virtual ~MapGridItem();            

      protected:

        /*!
          \brief Reimplemented from MapItem
         */
        virtual void drawSelection( QPainter* painter );
    };
  }
}

#endif
