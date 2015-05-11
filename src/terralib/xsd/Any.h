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
  \file Any.h

  \brief It models the XML Schema any element.
*/

#ifndef __TERRALIB_XSD_INTERNAL_ANY_H
#define __TERRALIB_XSD_INTERNAL_ANY_H

// TerraLib
#include "Annotated.h"
#include "Enums.h"
#include "Identifiable.h"
#include "Occurs.h"

// STL 
#include <string>

namespace te
{
  namespace xsd
  {
    /*!
      \class Any

      \brief It models the XML Schema any element.
      
      The any element can be used to extend the XML document with elements not specified by the schema.
    */
    class TEXSDEXPORT Any : public Occurs, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param ann An annotation.
          \param id  It specifies a unique ID for the element.

          \note The Any object will take the ownership of the annotation and id.
        */
        Any(Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        Any(const Any& rhs);

        /*! \brief Destructor. */
        ~Any();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        Any& operator=(const Any& rhs);

        /*!
          \brief It returns the namespaces containing the elements that can be used.

          \return The namespaces containing the elements that can be used.
        */
        std::string* getNamespace() const;

        /*!
          \brief It returns how the XML processor should handle validation against the elements specified by this any element.

          \return The mode how the XML processor should handle validation: STRICT | LAX | SKIP.
        */
        ProcessContents getProcessContent() const;

        /*!
          \brief It sets the namespaces containing the elements that can be used.

          \param ns The namespaces that will be used.
  
          \note The Any object will take the ownership of the given pointer.
        */
        void setNamespace(std::string* ns);

        /*!
          \brief It sets how the XML processor should handle validation against the elements specified by this any element.

          \param pc It specifies how the XML processor should handle validation: STRICT | LAX | SKIP.
        */
        void setProcessContents(ProcessContents pc);

      private:

        std::string* m_namespace;     //!< It specifies the namespaces containing the elements that can be used. (Optional)
        ProcessContents m_pContents;  //!< It specifies how the XML processor should handle validation against the elements specified by this any element. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_ANY_H
