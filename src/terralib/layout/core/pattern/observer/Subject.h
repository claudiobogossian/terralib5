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
  \file Observable.h
   
  \brief Abstract class to represent an observable. "Model" part of MVC component. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SUBJECT_H
#define __TERRALIB_LAYOUT_INTERNAL_SUBJECT_H

// TerraLib
#include "../../Config.h"

// STL
#include <vector>

namespace te
{
  namespace layout
  {
    class Observer; 

    /*!
      \brief Abstract class to represent an observable. "Model" part of MVC component. 
    
      \ingroup layout
    */
    class TELAYOUTEXPORT Subject 
    {
      public:

        /*!
          \brief Destructor
        */ 
        virtual ~Subject();

        /*!
          \brief Adds the specified observer to the set of observers for this object.
          \param o specified observer
          \return true if add, false otherwise
        */
        virtual bool attach(Observer* observer);

        /*!
          \brief Removes an observer from the set of observers of this object.
          \param o specified observer
          \return true if remove, false otherwise
        */
        virtual bool detach(Observer* observer);

      protected:

        /*!
          \brief Constructor
        */ 
        Subject();

        /*!
          \brief Notifies all set of observers that the state of model changed

          \param context maintaining the drawing context of a MVC component.
        */
        virtual void notify();

    private:

      std::vector<Observer*> m_observers;

    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_SUBJECT_H
