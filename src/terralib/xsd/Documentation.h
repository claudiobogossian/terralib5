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
  \file Documentation.h

  \brief A class that models a documentation element used to enter text comments in annotations.
*/

#ifndef __TERRALIB_XSD_INTERNAL_DOCUMENTATION_H
#define __TERRALIB_XSD_INTERNAL_DOCUMENTATION_H

// TerraLib
#include "AnnotationItem.h"

// STL
#include <string>

namespace te
{
  namespace xsd
  {
    /*!
      \class Documentation

      \brief A class that models a documentation element used to enter text comments in annotations.

      \sa Annotation, AnnotationItem, AppInfo

      \note Parent element: Annotation.
    */
    class TEXSDEXPORT Documentation : public AnnotationItem
    {
      public:

        /*!
          \brief Constructor.

          \param value  The descriptive information. (Required)
          \param source An URI that specifies the source of the information.
          \param lang   The language of the information.          
 
          \note The Documentation object will take the ownership of the pointers source, lang and value.
        */
        Documentation(std::string* value, std::string* source = 0, std::string* lang = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        Documentation(const Documentation& rhs);

        /*! \brief Destructor. */
        ~Documentation();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        Documentation& operator=(const Documentation& rhs);

        /*!
          \brief It returns the language used to code the annotation.

          \return The language used to code the annotation.
        */
        std::string* getLang() const;
        
        /*!
          \brief It returns the URI that specifies the source of the application information.

          \return The URI.
        */
        std::string* getSource() const;

        /*!
          \brief It returns the information associated to the annotation.

          \return The information associated to the annotation.
        */
        std::string* getValue() const;

        /*!
          \brief It sets the information associated to the annotation.

          \param doc A human readable material. This parameter can not be NULL.
  
          \note The Documentation object will take the ownership of the given pointer.
        */
        void setValue(std::string* doc);

        /*!
          \brief It sets the language of the information.

          \param lang The language of the information.
  
          \note The Documentation object will take the ownership of the given pointer.
        */
        void setLang(std::string* lang);

        /*!
          \brief It sets the URI reference that specifies the source of the information.

          \param source The URI.
  
          \note The Documentation object will take the ownership of the given pointer.
        */
        void setSource(std::string* source);

        AnnotationItem* clone() const;

      private:

        std::string* m_value;  //!< A human readable text. (Required)
        std::string* m_source; //!< An URI reference that specifies the source of the information. (Optional)
        std::string* m_lang;   //!< The language of the information. Example: "en". (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_DOCUMENTATION_H
