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
  \file Group.h

  \brief This class models a group element in a XML Schema.
*/

#ifndef __TERRALIB_XSD_INTERNAL_GROUP_H
#define __TERRALIB_XSD_INTERNAL_GROUP_H

// TerraLib
#include "Annotated.h"
#include "Content.h"
#include "Identifiable.h"
#include "Occurs.h"

namespace te
{
  namespace xsd
  {
// Forward declarations
    class Content;
    class QName;

    /*!
      \class Group

      \brief This class models a group element in a XML Schema.
      
      The group element is used to define a group of elements to be used in complex type definitions.

      \note Parent elements: schema, choice, sequence, complexType, restriction (both simpleContent and complexContent), extension (both simpleContent and complexContent).
    */
    class TEXSDEXPORT Group : public Content, public Occurs, public Identifiable, public Annotated
    {
      public:

        /*!
          \brief Constructor.
          
          \param id It specifies a unique ID for the element. It can be a NULL value.
          \param minOccurs The minimum number of times the element can occur.
          \param maxOccurs The maximum number of times the element can occur.
         */
        Group(unsigned int minOccurs = 1, unsigned int maxOccurs = 1, Annotation* ann = 0, std::string* id = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement this method!
        */
        Group(const Group& rhs);

        /*! \brief Destructor. */
        ~Group();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.

          \todo Implement this method!
        */
        Group& operator=(const Group& rhs);

        /*!
          \brief It returns the name of the group.

          \return The name of the group.
        */
        std::string* getName() const;

        /*!
          \brief It returns the reference to a name of another group.

          \return The reference to a name of another group.
        */
        QName* getRef() const;

        /*!
          \brief It returns the content of this group.

          \return The content of this group.
        */
        Content* getContent() const;

        /*!
          \brief It sets a name for the group.

          \param name The group name.
  
          \note Setting a name will turn to NULL the reference property.
          \note The Group object will take the ownership of the given pointer.
        */
        void setName(std::string* name);

        /*!
          \brief It sets a reference to a name of another group.

          \param ref A reference to a name of another group.
  
          \note Setting a reference property will turn to NULL the name.
          \note The Group object will take the ownership of the given pointer.
        */
        void setRef(QName* ref);

        /*!
          \brief It sets the group content. It can be one of: all | choice | sequence.

          \param c The appropriate content to group.
  
          \note The Group object will take the ownership of the given pointer.
        */
        void setContent(Content* c);

        Content* clone() const;

      private:

        std::string* m_name;    //!< It specifies a name for the group. (Optional)
        QName* m_ref;           //!< It refers to the name of another group. Name and ref attributes cannot both be present.
        Content* m_content;     //!< It can be one of: all | choice | sequence. (Optional)        
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_GROUP_H
