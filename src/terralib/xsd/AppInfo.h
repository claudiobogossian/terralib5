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
  \file AppInfo.h

  \brief This class models an object that specifies information to be used by applications.
*/

#ifndef __TERRALIB_XSD_INTERNAL_APPINFO_H
#define __TERRALIB_XSD_INTERNAL_APPINFO_H

// TerraLib
#include "AnnotationItem.h"

// STL
#include <string>

namespace te
{
  namespace xsd
  {
    /*!
      \class AppInfo

      \brief This class models an object that specifies information to be used by applications.

      \sa Annotation, AnnotationItem, Documentation

      \note Parent element: Annotation.
    */
    class TEXSDEXPORT AppInfo : public AnnotationItem
    {
      public:

        /*!
          \brief Constructor.
          
          \param source A URI reference that specifies the source of the application information.
          \param value  Any descriptive information.

          \note The AppInfo object will take the ownership of the source and value pointers.
        */
        AppInfo(std::string* source, std::string* value);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        AppInfo(const AppInfo& rhs);

        /*! \brief Destructor. */
        ~AppInfo();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        AppInfo& operator=(const AppInfo& rhs);

        /*!
          \brief It sets the information to be used by the application.

          \param value A machine readable material.

          \note The AppInfo object will take the ownership of the given pointer.
        */
        void setValue(std::string* value);

        /*!
          \brief It sets the URI that specifies the source of the information.

          \param info An URI.
        */
        void setSource(std::string* source);

        /*!
          \brief It returns the information associated to the annotation.

          \return The information associated to the annotation.
        */
        std::string* getValue() const;

        /*!
          \brief It returns the URI that specifies the source of the information.

          \return The URI.
        */
        std::string* getSource() const;

        AnnotationItem* clone() const;

      private:

        std::string* m_source;   //!< An URI that specifies the source of the application information. (Optional)
        std::string* m_value;    //!< An information to be used by the application. (Required)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_APPINFO_H
