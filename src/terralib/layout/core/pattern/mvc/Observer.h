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
  \file Observer.h
   
  \brief Abstract class to represent an observer. "View" part of MVC component.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OBSERVER_H
#define __TERRALIB_LAYOUT_INTERNAL_OBSERVER_H

//TerraLib
#include "../../ContextItem.h"
#include "../../Config.h"

namespace te
{
  namespace layout
  {
    class Properties;
    class ItemController;
    class Observable;

    /*!
      \brief Abstract class to represent an observer. "View" part of MVC component.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT Observer
    {
    public:

      /*!
          \brief Destructor
       */ 
	    virtual ~Observer(void) {}

      /*!
          \brief This method is called when a change has occurred in the state of the observable.
            Reimplement this function in a ItemObserver subclass to provide the item's updateObserver implementation.

          \param context maintaining the drawing context of a MVC component.
       */ 
	    virtual void updateObserver(ContextItem context) = 0;
    };
  }
}

#endif
