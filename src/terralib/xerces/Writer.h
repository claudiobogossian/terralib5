/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Writer.h
   
  \brief A class that models a XML writer object built on top of Xerces-C++.
*/

#ifndef __TERRALIB_XERCES_INTERNAL_WRITER_H
#define __TERRALIB_XERCES_INTERNAL_WRITER_H

// TerraLib
#include "../xml/AbstractWriter.h"
#include "Config.h"

// STL
#include <string>
#include <vector>

// Xerces-C++
#include <xercesc/util/XercesDefs.hpp>

// Forward declaration
XERCES_CPP_NAMESPACE_BEGIN
class DOMDocument;
class DOMElement;
class DOMImplementation;
XERCES_CPP_NAMESPACE_END

namespace te
{
  namespace xerces
  {
// Forward declaration

    /*!
      \class AbstractWriter

      \brief A class that models a XML writer object built on top of Xerces-C++
    */
    class TEXERCESEXPORT Writer : public te::xml::AbstractWriter
    {
      public:

        /*! \brief Default constructor. */
        Writer();

        /*! \brief Destructor. */
        ~Writer();

        void writeStartDocument(const std::string& encoding, const std::string& standalone);

        void writeStartElement(const std::string& qName);

        void writeElement(const std::string& qName, const std::string& value);

        void writeElement(const std::string& qName, const double& value);

        void writeElement(const std::string& qName, boost::int32_t value);

        void writeElement(const std::string& qName, boost::uint32_t value);

        void writeElement(const std::string& qName, boost::int64_t value);

        void writeElement(const std::string& qName, boost::uint64_t value);

        void writeAttribute(const std::string& attName, const std::string& value);

        void writeAttribute(const std::string& attName, const double& value);

        void writeAttribute(const std::string& attName, boost::int32_t value);

        void writeAttribute(const std::string& attName, boost::uint32_t value);

        void writeAttribute(const std::string& attName, boost::int64_t value);

        void writeAttribute(const std::string& attName, boost::uint64_t value);

        void writeValue(const std::string& value);

        void writeValue(const double& value);

        void writeValue(boost::int32_t value);

        void writeValue(boost::uint32_t value);

        void writeValue(boost::int64_t value);

        void writeValue(boost::uint64_t value);

        void writeEndElement(const std::string& qName);

        void writeToFile();

        void setRootNamespaceURI(const std::string& uri);

      private:

        /*!
          \brief Adds a XML value to the last inserted element.

          \param value Element value. (Input)
        */
        void addText(const std::string& qValue);

      private:

        XERCES_CPP_NAMESPACE_QUALIFIER DOMImplementation* m_impl; //!< DOM element - implementation.
        XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* m_doc;        //!< DOM element - document.

        std::vector<XERCES_CPP_NAMESPACE_QUALIFIER DOMElement*> m_elementSet; //!< Vector that contains all the elements (nodes).

        std::size_t m_indice;     //!< Index to build the ElementSet vector.
        std::size_t m_topIndice;  //!< Index of the parent node.

        std::vector<std::size_t> m_topElementSet; //!< This vector gives the parent node of each element.
        std::vector<std::string> m_topElementNamesSet; //!< This vector gives the parent node of each element.

        std::string m_encoding;
        bool m_standalone;
    };

  } // end namespace xerces
}   // end namespace te

#endif  // __TERRALIB_XERCES_INTERNAL_WRITER_H

