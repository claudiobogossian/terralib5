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
  \file IdentityConstraint.h

  \brief This is the base class for XML Schema elements that are related to identity constraint.
*/

#ifndef __TERRALIB_XSD_INTERNAL_IDENTITYCONSTRAINT_H
#define __TERRALIB_XSD_INTERNAL_IDENTITYCONSTRAINT_H

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
    class Field;
    class Selector;

    /*!
      \class IdentityConstraint

      \brief This is the base class for XML Schema elements that are related to identity constraint.

      Specifically the classes Key, KeyRef and Unique are derived from this base class.

      \sa Key, KeyRef, Unique
    */
    class TEXSDEXPORT IdentityConstraint : public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param id   It specifies a unique ID for this element. It may be a NULL value.
          \param name It specifies the name of this element. It is required a non-NULL value.

          \note The IdentityConstraint object will take the ownership of the id and name pointers.
        */
        IdentityConstraint(std::string* name, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        IdentityConstraint(const IdentityConstraint& rhs);

        /*! \brief Destructor. */
        virtual ~IdentityConstraint();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        IdentityConstraint& operator=(const IdentityConstraint& rhs);

        /*!
          \brief It returns the name of this element.

          \return The name of this element.
        */
        std::string* getName() const;

        /*!
          \brief It returns the Selector element of this element.

          \return The Selector element.
        */
        Selector* getSelector() const;

        /*!
          \brief It returns the list of Fields elements of this element.

          \return The list of Fields.
        */
        const boost::ptr_vector<Field>& getFields() const;

        /*!
          \brief It sets a name for this element.

          \param name A name for this element. Required a non-NULL value.
  
          \note The IdentityConstraint object will take the ownership of the given pointer.
        */
        void setName(std::string* name);

        /*!
          \brief It sets the Selector element of this element.

          \param s It specifies the Selector element.

          \note The IdentityConstraint object will take the ownership of the given pointer.
        */
        void setSelector(Selector* s);

        /*!
          \brief Adds a new field to this element.

          \param f The field that will be added.
          
          \note The IdentityConstraint object will take the ownership of the given pointer.
        */
        void addField(Field* f);

        /*!
          \brief It clones the object.

          \return A clone of the object. The caller will take the ownership.
        */
        virtual IdentityConstraint* clone() const = 0;

      protected:

        std::string* m_name;                 //!< It specifies the name of this element. (Required)
        Selector* m_selector;                //!< It specified the selector element of this. (Required)
        boost::ptr_vector<Field> m_fieldVec; //!< The list of fields. (Required at least one)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_IDENTITYCONSTRAINT_H
