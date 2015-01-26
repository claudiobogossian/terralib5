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
  \file ScaleItem.h
   
   \brief Class that represents a graphic scale of a map.
   Its coordinate system is the same of scene (millimeters). 
   This is also the son of ItemObserver and ObjectItem, so it can become observer of a model (Observable). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SCALE_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_SCALE_ITEM_H

// TerraLib
#include "ObjectItem.h"

namespace te
{
  namespace layout
  {
    class Observable;

    /*!
    \brief Class that represents a graphic scale of a map.
        Its coordinate system is the same of scene (millimeters). 
        This is also the son of ItemObserver and ObjectItem, so it can become observer of a model (Observable).  
	  
	    \ingroup layout

	    \sa te::layout::ObjectItem
	  */
    class ScaleItem : public ObjectItem
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        ScaleItem( ItemController* controller, Observable* o );

        /*!
          \brief Destructor
         */
        virtual ~ScaleItem();
        
        /*!
          \brief Reimplemented from ObjectItem
         */
        virtual void updateObserver(ContextItem context);
    };
  }
}

#endif
