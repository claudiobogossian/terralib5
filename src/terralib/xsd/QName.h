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
  \file QName.h

  \brief A class to deal with XML qualified names.
*/

#ifndef __TERRALIB_XSD_INTERNAL_QNAME_H
#define __TERRALIB_XSD_INTERNAL_QNAME_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace xsd
  {
    /*!
      \class QName

      \brief A class to be used to represent XML qualified names.

      Since URI references can be long and may contain prohibited characters for element/attribute naming, 
      QNames are used to create a mapping between the URI and a namespace prefix. 
      The mapping enables the abbreviation of URIs, therefore it achieves a more convenient way to write XML documents.
      For more details see: http://www.w3.org/TR/REC-xml-names/#ns-qualnames.
    */
    class TEXSDEXPORT QName
    {
      public:

        /*! \brief Constructor. */
        QName();

        /*!
          \brief Constructor.

          \param prefix    The namespace prefix part of the qualified name.
          \param localPart The local part of the qualified name.
        */
        QName(const std::string& prefix, const std::string& localPart);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        QName(const QName& rhs);

        /*! \brief Destructor. */
        ~QName();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        QName& operator=(const QName& rhs);

        /*!
          \brief It returns the qualified name based on prefix and local part.

          \return The qualified name.
        */
        std::string getQualifiedName() const;

         /*!
          \brief It returns the namespace prefix part of the qualified name.

          \return The namespace prefix part of the qualified name.
        */
        const std::string& getPrefix() const;

        /*!
          \brief It returns the local part of the qualified name.

          \return The local part of the qualified name.
        */
        const std::string& getLocalPart() const;

        /*!
          \brief It sets the namespace prefix part of the qualified name.

          \param prefix The namespace prefix part of the qualified name.
        */
        void setPrefix(const std::string& prefix);

        /*!
          \brief It sets the local part of the qualified name.

          \param localPart The local part of the qualified name.
        */
        void setLocalPart(const std::string& localPart);

      private:

        std::string m_prefix;    //!< The namespace prefix part of the qualified name. It must be associated with a namespace URI reference in the namespace declaration.
        std::string m_localPart; //!< The local part of the qualified name.
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_QNAME_H
