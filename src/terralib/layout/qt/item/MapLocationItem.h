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
  \file MapLocationItem.h
   
   \brief This class is a proxy MapDisplay. This makes it possible to add a MapDisplay as item of a scene. 
   This object is of type QGraphicsProxyWidget. He have a directly interaction by user. 
   His transformation matrix is inverted, that is, the inverse of the matrix of the scene, so its coordinate system is screen (pixel), 
   but its position in the scene remains in millimeters.
   This is also son of ItemObserver, so it can become observer of a model (Observable). "View" part of MVC component.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_LOCATION_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_MAP_LOCATION_ITEM_H

// TerraLib
#include "MapItem.h"
#include "../../core/Config.h"

namespace te
{
  namespace layout
  {
    /*!
    \brief This class is a proxy MapDisplay. This makes it possible to add a MapDisplay as item of a scene. 
    This object is of type QGraphicsProxyWidget. He have a directly interaction by user. 
    His transformation matrix is inverted, that is, the inverse of the matrix of the scene, so its coordinate system is screen (pixel), 
    but its position in the scene remains in millimeters.
    Drawing starting point is llx, lly.
    He is also the son of ItemObserver, so it can become observer of a model (Observable). "View" part of MVC component.
    
    \ingroup layout

    \sa te::layout::ItemObserver
  */
    class TELAYOUTEXPORT MapLocationItem : public MapItem
    {
      Q_OBJECT //for slots/signals

      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        MapLocationItem( AbstractItemController* controller, AbstractItemModel* model );

        /*!
          \brief Destructor
        */ 
        virtual ~MapLocationItem();
    };
  }
}

#endif
