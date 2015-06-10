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
  \file Selector.h

  \brief It models the selector element of an XML Schema.
*/

#ifndef __TERRALIB_XSD_INTERNAL_SELECTOR_H
#define __TERRALIB_XSD_INTERNAL_SELECTOR_H

// TerraLib
#include "Annotated.h"
#include "Identifiable.h"

namespace te
{
  namespace xsd
  {
    /*!
      \class Selector

      \brief It models the selector element of an XML Schema.

      \note Parent elements: key, keyref, unique.
    */
    class TEXSDEXPORT Selector : public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param id    It specifies a unique ID for the element. It may be a NULL value.
          \param xpath It identifies a single element or attribute whose content or value is used for the constraint. It is required a non-NULL value.

          \note The Selector object will take the ownership of the given pointers.
        */
        Selector(std::string* xpath, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Selector(const Selector& rhs);

        /*! \brief Destructor. */
        ~Selector();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Selector& operator=(const Selector& rhs);

        /*!
          \brief It returns the xpath expression of the Selector element.

          \return The xpath expression.
        */
        std::string* getXPath() const;

        /*!
          \brief It sets the xpath expression of the Selector element.

          \param xpath It specifies the xpath expression of the Selector element.

          \note The Selector object will take the ownership of the given pointer.
        */
        void setXPath(std::string* xpath);

      private:

        std::string* m_xpath;  //!< It specifies an XPath expression, relative to the element being declared, that identifies the child elements to which the identity constraint applies. (Required)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_SELECTOR_H
