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
  \file AbstractVisitor.h
   
  \brief Abstract class to represent a visitor. All classes representing a visitor must inherit from this class.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_CONTEXT_ABSTRACT_VISITOR_H
#define __TERRALIB_LAYOUT_INTERNAL_CONTEXT_ABSTRACT_VISITOR_H

// TerraLib
#include "../../ContextItem.h"

namespace te
{
  namespace layout
  {
    class Visitable;

    /*!
	    \brief Abstract class to represent a visitor. All classes representing a visitor must inherit from this class.
	  
	    \ingroup layout
	  */
    class AbstractVisitor
    {
      public:

        /*!
          \brief Constructor
        */ 
        AbstractVisitor();

        /*!
          \brief Destructor
        */ 
        ~AbstractVisitor(void);

        /*!
          \brief Constructor
        */ 
        virtual void visit(Visitable* visitable);

        /*!
          \brief Separate this object from visitable.
        */
        virtual void disassociate();

        /*!
          \brief Return the object to be visited. 

          \param
        */
        virtual Visitable* getVisitable();

      protected:

        /*!
          \brief visit and if necessary redraw
        */
        virtual void visitDependent(ContextItem context) = 0;

        /*!
          \brief Returns context maintaining the drawing context of a MVC component.

          \param context maintaining the drawing context of a MVC component
        */
        virtual ContextItem getContextItem();

        Visitable* m_visitable; //!< object to be visited
    };
  }
}

#endif
