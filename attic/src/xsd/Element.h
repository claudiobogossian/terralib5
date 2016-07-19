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
  \file Element.h

  \brief This class models the element of a XML Schema.
*/

#ifndef __TERRALIB_XSD_INTERNAL_ELEMENT_H
#define __TERRALIB_XSD_INTERNAL_ELEMENT_H

// TerraLib
#include "Config.h"
#include "Annotated.h"
#include "Enums.h"
#include "Identifiable.h"
#include "Occurs.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace xsd
  {
// Forward declarations
    class IdentityConstraint;
    class QName;
    class Type;
    class Unique;

    /*!
      \class Element

      \brief This class models the element of a XML Schema.

      \note Parent elements: schema, choice, all, sequence, group.
    */
    class TEXSDEXPORT Element : public Occurs, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.

          \param id It specifies a unique ID for the element. It can be a NULL value.
        */
        Element(unsigned int minOccurs = 1, unsigned int maxOccurs = 1, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Element(const Element& rhs);

        /*!
          \brief Destructor.
        
          \todo Implement!
        */
        ~Element();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement!
        */
        Element& operator=(const Element& rhs);
           
        /*!
          \brief It returns the name of a built-in data type, or the name of a simpleType or complexType element.

          \return The name of a built-in data type, or the name of a simpleType or complexType element.
        */
        QName* getType() const;

        /*!
          \brief It returns the default value for the element.

          \return The default value for the element.
        */
        std::string* getDefaultValue() const;

        /*!
          \brief It returns the fixed value for the element.

          \return The fixed value for the element.
        */
        std::string* getFixedValue() const;

        /*!
          \brief If the element is "nillable" or not.

          \return True if the element is "nillable" and false if not.
        */
        bool isNillable();

        /*!
          \brief If the element is "abstract" or not.

          \return True if the element is "abstract" and false if not.
        */
        bool isAbstract();

        /*!
          \brief It returns the block value for the element. It prevents an element with a specified type of derivation from being used in place of this element.

          \return The block value. It can be: #all | list of (extension|restriction).
        */
        int getBlock();

        /*!
          \brief It sets the element type.

          \param t The element type.

          \note The element will take the ownership of the given type.
        */
        void setType(QName* t); 

        /*!
          \brief It sets the default value for the element.

          \param v The element default value.

          \note The element will take the ownership of the given value.
        */
        void setDefaultValue(std::string* v);

        /*!
          \brief It sets a fixed value for the element.

          \param v A fixed value for the element.

          \note The element will take the ownership of the given value.
        */
        void setFixedValue(std::string* v);

        /*!
          \brief It specifies whether an explicit null value can be assigned to the element.         

          \param v True enables an instance of the element to have the null attribute set to true.
        */
        void setAsNillable(bool v);

        /*!
          \brief It specifies whether the element can be used in an instance document.

          \param v True indicates that the element cannot appear in the instance document. Instead, another element whose substitutionGroup attribute contains the qualified name (QName) of this element must appear in this element's place.
        */
        void setAsAbstract(bool v);

        /*!
          \brief It prevents an element with a specified type of derivation from being used in place of this element.

          \param v It can be: #all|list of (extension|restriction).
        */
        void setBlock(int v);

        /*!
          \brief It returns the element name or NULL if it doesn't have one.

          \return The element name or NULL if it doesn't have one.
        */
        std::string* getName() const;

        /*!
          \brief It returns the reference to a name of another element.

          \return The reference to a name of another element.
        */
        QName* getRef() const;

        /*!
          \brief It sets the element name.

          \param name The element name.

          \note The element will take the ownership of the given name.
          \note Setting a name will turn to NULL the reference property.
        */
        void setName(std::string* name);

        /*!
          \brief It sets a reference to a name of another element.

          \param ref A reference to a name of another element.
  
          \note Setting a reference property will turn to NULL the name.
          \note The LocalElement object will take the ownership of the given pointer.
        */
        void setRef(QName* ref);

        /*!
          \brief It sets the substitution group.

          \param g The substitution group.

          \note The element will take the ownership of the given substitution group.
         */
        void setSubstitutionGroup(QName* g);

        /*!
          \brief It sets the default value of the final attribute on the element element.

          \param v It can be: #all|list of (extension|restriction).
         */
        void setFinal(int v);

        /*!
          \brief It sets the content type of the element.

          \param t The content type of the element.

          \note The element will take the ownership of the given type.
        */
        void setContentType(Type* t);

        /*!
          \brief It adds an IdentityConstraint to the element.

          \param ic The IdentityConstraint that will be added.

          \note The element will take the ownership of the given constraint.
        */
        void addIdentityConstraint(IdentityConstraint* ic);

      private:

        std::string* m_name;        //!< It specifies a name for the element. (Optional)
        QName* m_ref;               //!< It refers to the name of another element. The ref attribute can include a namespace prefix.
        QName* m_type;              //!< It specifies either the name of a built-in data type, or the name of a simpleType or complexType element. (Optional)
        QName* m_substitutionGroup; //!< It specifies the element that can be substituted with this element.
        std::string* m_default;     //!< It specifies a default value for the element (can only be used if the element's content is a simple type or text only).
        std::string* m_fixed;       //!< It ensures that the elements are set to particular value.
        Form m_form;                //!< Here, "unqualified" indicates that this attribute is not required to be qualified with the namespace prefix. "qualified" indicates that this attribute must be qualified with the namespace prefix. The default value is the value of the elementFormDefault attribute of the schema element. (Optional)
        bool m_nillable;            //!< It specifies whether an explicit null value can be assigned to the element. True enables an instance of the element to have the null attribute set to true. The null attribute is defined as part of the XML Schema namespace for instances. Default is false.
        bool m_abstract;            //!< It specifies whether the element can be used in an instance document. True indicates that the element cannot appear in the instance document. Instead, another element whose substitutionGroup attribute contains the qualified name (QName) of this element must appear in this element's place. Default is false.
        int m_block;                //!< It prevents an element with a specified type of derivation from being used in place of this element. (Optional)
        int m_final;                //!< It sets the default value of the final attribute on the element element.
        Type* m_contentType;        //!< A content of this element. A simpleType or complexType element can be present as a child only if both the ref and type attributes are not present.
        boost::ptr_vector<IdentityConstraint> m_identityConstraintVec; //!< The list of elements related to identity constraint - (key | keyref | unique)*. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_ELEMENT_H

