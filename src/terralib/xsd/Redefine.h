/*  Copyright (C) 2010-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file Redefine.h

  \brief It models a XML Schema redefine.
*/

#ifndef __TERRALIB_XSD_INTERNAL_REDEFINE_H
#define __TERRALIB_XSD_INTERNAL_REDEFINE_H

// TerraLib
#include "Annotated.h"
#include "Identifiable.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class AttributeGroup;
    class Group;
    class Type;

    /*!
      \class Redefine

      \brief It models a XML Schema redefine.

      \note Parent elements: schema.
    */
    class TEXSDEXPORT Redefine : public Identifiable, public Annotated
    {
      public:        

        /*!
          \brief Constructor.

          \param id It specifies a unique ID for the element. It can be a NULL value.
          \param schemaLocation It specifies the URI to the location of a schema document.
        */
        Redefine(const std::string& schemaLocation, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Redefine(const Redefine& rhs);

        /*! \brief Destructor. */
        ~Redefine();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Redefine& operator=(const Redefine& rhs);

        /*!
          \brief It returns the location of the redefined schema.

          \return The schema location.
        */
        const std::string& getSchemaLocation() const;

        /*!
          \brief It returns the list of simple and complex types at redefine element.

          \return The list of simple and complex types at redefine element.
        */
        const boost::ptr_vector<Type>& getTypes() const;

        /*!
          \brief It returns the list of group declarations at redefine element.

          \return The list of group declarations at redefine element.
        */
        const boost::ptr_vector<Group>& getGroups() const;
        
        /*!
          \brief It returns the list of attribute group declarations at redefine element.

          \return The list of attribute group declarations at redefine element.
        */
        const boost::ptr_vector<AttributeGroup>& getAttributeGroups() const;

      private:

        std::string m_schemaLocation;                          //!< It specifies a URI to the location of a schema document. (Required)
        boost::ptr_vector<Type> m_typeVec;                     //!< The list of simple and complex types at redefine element. (Optional)
        boost::ptr_vector<Group> m_groupVec;                   //!< The list of group declarations at redefine element. (Optional)
        boost::ptr_vector<AttributeGroup> m_attributeGroupVec; //!< The list of attribute group declarations at redefine element. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_REDEFINE_H

