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
  \file Reader.h
   
  \brief A class that models a XML reader object built on top of Xerces-C++.
*/

#ifndef __TERRALIB_XERCES_INTERNAL_READER_H
#define __TERRALIB_XERCES_INTERNAL_READER_H

// TerraLib
#include "../xml/Reader.h"
#include "Config.h"

// STL
#include <string>

// Xerces-C++
#include <xercesc/util/XercesDefs.hpp>

// Forward declaration
XERCES_CPP_NAMESPACE_BEGIN
class Attributes;
class SAX2XMLReader;
class XMLPScanToken;
XERCES_CPP_NAMESPACE_END

namespace te
{
  namespace xerces
  {
// Forward declaration
    class ErrorHandler;
    class ReaderHandler;

    /*!
      \class Reader

      \brief A class that models a XML reader object built on top of Xerces-C++

      This API is intend to simplify the task of reading XML files. We have
      tried to model it close to the Libxml2 API (see http://xmlsoft.org/xmlreader.html for more information)
      but keeping all Xerces-C++ support capabilities: schema validation and grammar cache.

      If you prefer to use the SAX or DOM parsers, please, look
      the Xerces-C++ documentation available at http://xerces.apache.org/xerces-c.
    */
    class TEXERCESEXPORT Reader : public te::xml::Reader
    {
      public:

        /*! \brief Default constructor. */
        Reader();

        /*! \brief Destructor. */
        ~Reader();

        void setDoNamespaces(bool d);

        void setDoSchema(bool d);

        void setValidationScheme(bool d);

        void setUseCachedGrammarInParse(bool d);

        void setCacheGrammarFromParse(bool d);

        void setIgnoreWhiteSpaces(bool d);

        void read(const std::string& fileURI);

        bool next();

        te::xml::NodeType getNodeType() const;

        std::string getElementURI() const;

        std::string getElementLocalName() const;

        std::string getElementQName() const;

        std::string getElementValue() const;

        std::size_t getElementDataLen() const;

        bool hasAttrs() const;

        std::size_t getNumberOfAttrs() const;

        std::string getAttr(const std::string& name) const;

        std::string getAttr(std::size_t i) const;

        std::string getAttrLocalName(std::size_t i) const;

        std::string getAttrQName(std::size_t i) const;

        std::string getAttrURI(std::size_t i) const;

        std::size_t getAttrPosition(const std::string& name) const;

        std::size_t getNumberOfNamespaces() const;

        void getNamespace(std::size_t i, std::pair<std::string, std::string>& ns) const;

        void setInternalBufferSize(const std::size_t size);

        /*!
          \brief It resets the parser.

          \exception Exception It throws an exception if it is not possible to reset the parser.

          \note Xerces extended method.
        */
        void reset();

      private:

        xercesc::SAX2XMLReader* m_parser;   //!< A pointer to the parser used by the reader.
        ReaderHandler* m_readerH;           //!< A pointer to a content handler.
        ErrorHandler* m_errH;               //!< A pointer to an error handler.
        xercesc::XMLPScanToken* m_token;    //!< This keeps the parser state.
        bool m_ignoreWhiteSpaces;           //!< A flag that indicates if the parser should ignore white spaces.
    };

  } // end namespace xerces
}   // end namespace te

#endif  // __TERRALIB_XERCES_INTERNAL_READER_H

