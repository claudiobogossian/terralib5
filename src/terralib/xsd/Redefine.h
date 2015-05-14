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
  \file Redefine.h

  \brief It models a XML Schema redefine.
*/

#ifndef __TERRALIB_XSD_INTERNAL_REDEFINE_H
#define __TERRALIB_XSD_INTERNAL_REDEFINE_H

// TerraLib
#include "Identifiable.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class Annotation;
    class AttributeGroup;
    class Group;
    class Type;

    /*!
      \class Redefine

      \brief It models a XML Schema redefine.

      \note Parent elements: schema.
    */
    class TEXSDEXPORT Redefine : public Identifiable
    {
      public:        

        /*!
          \brief Constructor.

          \param schemaLocation It specifies the URI to the location of a schema document.
          \param id It specifies a unique ID for the element. It can be a NULL value.
        */
        Redefine(const std::string& schemaLocation, std::string* id = 0);

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
          \brief It returns the list of annotations at redefine element.

          \return The list of annotations at redefine element.
        */
        const boost::ptr_vector<Annotation>& getAnnotations() const;

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

        /*!
          \brief It sets the location of the redefined schema.

          \param schemaLocation The schema location.
        */
        void setSchemaLocation(const std::string& schemaLocation);

        /*!
          \brief It inserts an annotation into the redefine.

          \param ann Some annotation.

          \note The redefine will take the ownership of the given pointer.
        */
        void addAnnotation(Annotation* ann);

        /*!
          \brief It inserts a type into the redefine.

          \param t Some type.

          \note The redefine will take the ownership of the given pointer.
        */
        void addType(Type* t);

        /*!
          \brief It inserts a group into the redefine.

          \param g Some group.

          \note The redefine will take the ownership of the given pointer.
        */
        void addGroup(Group* g);

        /*!
          \brief It inserts an attribute group into the redefine.

          \param ag Some attribute group.

          \note The redefine will take the ownership of the given pointer.
        */
        void addAttributeGroup(AttributeGroup* ag);

      private:

        std::string m_schemaLocation;                          //!< It specifies a URI to the location of a schema document. (Required)
        boost::ptr_vector<Annotation> m_annotationVec;         //!< The list of annotations at redefine element. (Optional)
        boost::ptr_vector<Type> m_typeVec;                     //!< The list of simple and complex types at redefine element. (Optional)
        boost::ptr_vector<Group> m_groupVec;                   //!< The list of group declarations at redefine element. (Optional)
        boost::ptr_vector<AttributeGroup> m_attributeGroupVec; //!< The list of attribute group declarations at redefine element. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_REDEFINE_H

