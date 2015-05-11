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
  \file OutsideFactory.h
   
  \brief Factory for creating families of related or dependent widgets (MVC widgets).

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_FACTORY_H 
#define __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_FACTORY_H

// TerraLib
#include "../../../../core/pattern/factory/AbstractOutsideFactory.h"
#include "../../../../core/Config.h"

namespace te
{
  namespace layout
  {

    /*!
	  \brief Factory for creating families of related or dependent widgets (MVC widgets).
	  
	  \ingroup layout

    \sa te::layout::AbstractItemFactory
	  */
    class TELAYOUTEXPORT OutsideFactory : public AbstractOutsideFactory
    {
    public:

      /*!
          \brief Constructor
       */
	    OutsideFactory();

      /*!
          \brief Destructor
       */
	    virtual ~OutsideFactory();

      /*!
          \brief Reimplemented from AbstractItemFactory
       */
	    virtual Observer* make(EnumType* type, OutsideParamsCreate params = OutsideParamsCreate());
    };
  }
}

#endif
