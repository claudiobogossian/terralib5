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
  \file MapLocationModel.h
   
   \brief Class that represents a "Model" part of Map MVC component.  
   Its coordinate system is the same of scene (millimeters). 
   This is also son of ItemModelObservable, so it can become observable, and son of Visitable, so it can become visitable.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_LOCATION_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_MAP_LOCATION_MODEL_H

// TerraLib
#include "MapModel.h"
#include "../core/Config.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace layout
  {
    /*!
    \brief Class that represents a "Model" part of Map MVC component.  
    Its coordinate system is the same of scene (millimeters). 
    This is also son of ItemModelObservable, so it can become observable, and son of Visitable, so it can become visitable.
      	  
	    \ingroup layout

      \sa te::layout::ItemModelObservable , te::layout::Visitable
	  */
    class TELAYOUTEXPORT MapLocationModel : public MapModel
    {
      public:

        /*!
          \brief Constructor
        */
        MapLocationModel();

        /*!
          \brief Destructor
        */ 
        virtual ~MapLocationModel();
    };
  }
}

#endif 
