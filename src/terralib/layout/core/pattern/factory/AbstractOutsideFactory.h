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
  \file AbstractOutsideFactory.h
   
  \brief Abstract Factory provide an interface for creating families of related or dependent graphic widgets (MVC widgets) without specifying their concrete classes.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_OUTSIDE_FACTORY_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_OUTSIDE_FACTORY_H

// TerraLib
#include "OutsideParamsCreate.h"
#include "../../enum/EnumType.h"

namespace te
{
  namespace layout
  {
		class AbstractOutsideView;

    /*!
	  \brief Abstract Factory provide an interface for creating families of related or dependent graphic widgets (MVC widgets) without specifying their concrete classes.
	  
	  \ingroup layout
	  */
    class AbstractOutsideFactory 
    {
    public:

      /*!
          \brief Constructor
       */
      virtual ~AbstractOutsideFactory(void) {}

      /*!
          \brief Method that builds and returns a new widget (MVC widget) of the type defined as parameter.

          \param type type of the new object
          \param params parameters to creation new object. Ex.: "Model" and "Controller" of the new object (MVC widget).
       */
			virtual AbstractOutsideView* make(EnumType* type, OutsideParamsCreate params = OutsideParamsCreate()) = 0;
    };
  }
}

#endif
