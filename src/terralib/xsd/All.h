/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file All.h

  \brief This class models an XML Schema all element.
*/

#ifndef __TERRALIB_XSD_INTERNAL_ALL_H
#define __TERRALIB_XSD_INTERNAL_ALL_H

// TerraLib
#include "Annotated.h"
#include "Content.h"
#include "Identifiable.h"
#include "Occurs.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class Element;

    /*!
      \class All

      \brief This class models an XML Schema all element.
      
      The all element specifies that the child elements can appear in any order and that each child element can occur zero or one time.

      \note Parent elements: group, complexType, restriction (both simpleContent and complexContent), extension (both simpleContent and complexContent).
    */
    class TEXSDEXPORT All : public Content, public Occurs, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param id It specifies a unique ID for the element. It can be a NULL value.
          \param minOccurs The minimum number of times the element can occur.
          \param maxOccurs The maximum number of times the element can occur.
        */
        All(unsigned int minOccurs = 1, unsigned int maxOccurs = 1, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        All(const All& rhs);

        /*! \brief Destructor. */
        ~All();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        All& operator=(const All& rhs);

        /*!
          \brief It returns the list of elements of All.

          \return The list of elements.
        */
        const boost::ptr_vector<Element>& getElements() const;

        /*!
          \brief It adds an element to this All element.

          \param e The element that will be added.
  
          \note The All object will take the ownership of the given pointer.
        */
        void addElement(Element* e);

        Content* clone() const;

      private:

        boost::ptr_vector<Element> m_elementVec; //!< Lis of elements. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_ALL_H
