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
  \file Sequence.h

  \brief This class models a sequence element in a XML Schema.
*/

#ifndef __TERRALIB_XSD_INTERNAL_SEQUENCE_H
#define __TERRALIB_XSD_INTERNAL_SEQUENCE_H

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
    class Any;

    /*!
      \class Sequence

      \brief This class models a sequence element in a XML Schema.
      
      The sequence element specifies that the child elements must appear in a sequence.
      Each child element can occur from 0 to any number of times.

      \note Parent elements: group, choice, sequence, complexType, restriction (both simpleContent and complexContent), extension (both simpleContent and complexContent)

      \todo This class is not preserving the order: (element|group|choice|sequence|any)*. We must work on this later!
    */
    class TEXSDEXPORT Sequence : public Content, public Occurs, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param id It specifies a unique ID for the element. It can be a NULL value.
          \param minOccurs The minimum number of times the element can occur.
          \param maxOccurs The maximum number of times the element can occur.
        */
        Sequence(unsigned int minOccurs = 1, unsigned int maxOccurs = 1, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Sequence(const Sequence& rhs);

        /*! \brief Destructor. */
        ~Sequence();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Sequence& operator=(const Sequence& rhs);

        /*!
          \brief It returns the list of elements of All.

          \return The list of elements.
        */
        const boost::ptr_vector<Element>& getElements() const;

        /*!
          \brief It returns the list of Content elements of Choice.

          \return The list of Content elements: group | choice | sequence.
        */
        const boost::ptr_vector<Content>& getContents() const;
        
        /*!
          \brief It returns the list of any elements of Choice.

          \return The list of any elements. 
        */
        const boost::ptr_vector<Any>& getAnys() const;

        /*!
          \brief It adds an element to this Sequence element.

          \param e The element that will be added.
  
          \note The Sequence object will take the ownership of the given pointer.
        */
        void addElement(Element* e);

        /*!
          \brief It adds a Content element to this Sequence element.

          \param c The appropriate Content element. It can be: group | choice | sequence.
  
          \note The Sequence object will take the ownership of the given pointer.
        */
        void addContent(Content* c);

        /*!
          \brief It adds an any to this Sequence element.

          \param a The any that will be added.
  
          \note The Sequence object will take the ownership of the given pointer.
        */
        void addAny(Any* a);

        Content* clone() const;

      private:

        boost::ptr_vector<Element> m_elementVec;   //!< The list of elements.
        boost::ptr_vector<Content> m_contentVec;   //!< The list of Contents elements (group | choice | sequence).
        boost::ptr_vector<Any> m_anyVec;           //!< The list of any elements.
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_SEQUENCE_H
