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
  \file PointModel.h
   
   \brief Class that represents a "Model" part of Point MVC component. 
      Its coordinate system is the same of scene (millimeters). 
      This is also son of ItemModelObservable, so it can become observable.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_POINT_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_POINT_MODEL_H

// TerraLib
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/ContextItem.h"

namespace te
{
  namespace layout
  {
    /*!
      \brief Class that represents a "Model" part of Point MVC component. 
          Its coordinate system is the same of scene (millimeters). 
          He is also the son of ItemModelObservable, so it can become observable.
      	  
	    \ingroup layout

      \sa te::layout::ItemModelObservable
	  */
    class PointModel : public ItemModelObservable
    {
      public:

        /*!
          \brief Constructor
        */
        PointModel();

        /*!
          \brief Destructor
        */ 
        virtual ~PointModel();

        /*!
          \brief Reimplemented from ItemModelObservable
         */
        virtual void draw( ContextItem context );
    };
  }
}

#endif 
