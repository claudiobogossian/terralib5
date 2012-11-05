/*  Copyright (C) 2010-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file Choice.h

  \brief This class models the XML Schema choice element.
*/

#ifndef __TERRALIB_XSD_INTERNAL_CHOICE_H
#define __TERRALIB_XSD_INTERNAL_CHOICE_H

// TerraLib
#include "Annotated.h"
#include "Content.h"
#include "Identifiable.h"
#include "Occurs.h"

// STL
#include <vector>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class Any;
    class Element;

    /*!
      \class Choice

      \brief This class models the XML Schema choice element.
      
      The choice element allows only one of the elements contained in the <choice> declaration to be present within the containing element.

      \note Parent elements: group, choice, sequence, complexType, restriction (both simpleContent and complexContent), extension (both simpleContent and complexContent).

      \todo This class is not preserving the order: (element|group|choice|sequence|any)*. We must work on this later!
    */
    class TEXSDEXPORT Choice : public Content, public Occurs, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param id It specifies a unique ID for the element. It can be a NULL value.
          \param minOccurs The minimum number of times the element can occur.
          \param maxOccurs The maximum number of times the element can occur.
        */
        Choice(unsigned int minOccurs = 1, unsigned int maxOccurs = 1, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo We must implement this method.
        */
        Choice(const Choice& rhs);

        /*! \brief Destructor. */
        ~Choice();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo We must implement this method.
        */
        Choice& operator=(const Choice& rhs);

        /*!
          \brief It returns the list of elements of Choice.

          \return The list of elements.
        */
        std::vector<Element*>* getElements() const;

        /*!
          \brief It returns the list of Content elements of Choice.

          \return The list of Content elements: group | choice | sequence.
        */
        std::vector<Content*>* getContents() const;
        
        /*!
          \brief It returns the list of any elements of Choice.

          \return The list of any elements. 
        */
        std::vector<Any*>* getAnys() const;

        /*!
          \brief It adds an element to this Choice element.

          \param e The element that will be added.
  
          \note The Choice object will take the ownership of the given pointer.
        */
        void addElement(Element* e);

        /*!
          \brief It adds a Content element to this Choice element.

          \param c The appropriate Content element. It can be: group | choice | sequence.
  
          \note The Choice object will take the ownership of the given pointer.
        */
        void addContent(Content* c);

        /*!
          \brief It adds an any to this Choice element.

          \param a The any that will be added.
  
          \note The Choice object will take the ownership of the given pointer.
        */
        void addAny(Any* a);

        Content* clone() const;

      private:

        std::vector<Element*>* m_elements;   //!< The list of elements.       
        std::vector<Content*>* m_contents;   //!< The list of Contents elements (group | choice | sequence).
        std::vector<Any*>* m_anys;           //!< The list of any elements.
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_CHOICE_)

