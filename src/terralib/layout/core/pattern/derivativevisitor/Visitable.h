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
  \file Visitable.h
   
  \brief Class to represent a visitable. All classes representing a visitable must inherit from this class.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_VISITABLE_H  
#define __TERRALIB_LAYOUT_INTERNAL_VISITABLE_H

// STL
#include <set>

namespace te
{
  namespace layout
  {
    class AbstractVisitor;

    /*!
	    \brief Class to represent a visitable. All classes representing a visitable must inherit from this class.
	  
	    \ingroup layout
	  */
    class Visitable
    {
      public:
        
        /*!
          \brief Destructor
        */ 
        ~Visitable(void)
        {
          unacceptAllVisitor();
        }

        /*!
          \brief Adds the specified visitor to the set of visitors for this object.

          \param visitor specified visitor
        */
        virtual void acceptVisitor(AbstractVisitor* visitor);

        /*!
          \brief Removes a visitor from the set of visitors of this object. Dissociate the visitor of this object.

          \param visitor specified visitor
        */
        virtual void unacceptVisitor(AbstractVisitor* visitor);

        /*!
          \brief Removes all visitors from the set of visitors of this object. Dissociate the visitors of this object.
        */
        virtual void unacceptAllVisitor();

      protected:

        /*!
          \brief Notifies all set of visitors that the state of this object changed
        */
        virtual void updateVisitors();

      protected:

        std::set<AbstractVisitor*>	m_visitors; //!< set of visitors of this object
    };
  }
}
#endif
