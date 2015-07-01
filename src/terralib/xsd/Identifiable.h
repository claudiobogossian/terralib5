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
  \file Identifiable.h

  \brief A base class for XSD classes that must provide a unique ID property.
*/

#ifndef __TERRALIB_XSD_INTERNAL_IDENTIFIABLE_H
#define __TERRALIB_XSD_INTERNAL_IDENTIFIABLE_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace xsd
  {
    /*!
      \class Identifiable

      \brief A base class for XSD classes that must provide a unique ID property.

      \sa Annotation, Attribute, Schema
    */
    class TEXSDEXPORT Identifiable
    {
      public:

        /*!
          \brief it sets the id.

          \param id The id to be associated to the object. It will take the ownership of the given string.
        */
        void setId(std::string* id);

        /*! \brief It returns a pointer (may be NULL) to the object id. */
        std::string* getId() const;

      protected:

        /*!
          \brief Constructor.
          
          \param id The unique ID for the XSD element.

          \note The Identifiable object will take the ownership of the informed pointer.
        */
        Identifiable(std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        Identifiable(const Identifiable& rhs);

        /*! \brief Protected non-virtual destructor. */
        ~Identifiable();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        Identifiable& operator=(const Identifiable& rhs);

      private:
        
        std::string* m_id; //!< It specifies a unique identifier for the element. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_IDENTIFIABLE_H
