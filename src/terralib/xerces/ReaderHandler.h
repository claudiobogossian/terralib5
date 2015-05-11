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
  \file ReaderHandler.h
   
  \brief This class implements Xerces interface for a handler that receives general document events.
*/

#ifndef __TERRALIB_XERCES_INTERNAL_CONTENTHANDLER_H
#define __TERRALIB_XERCES_INTERNAL_CONTENTHANDLER_H

// TerraLib
#include "../xml/Enums.h"
#include "Config.h"

// STL
#include <vector>

// Xerces-C++
#include <xercesc/sax2/ContentHandler.hpp>

namespace te
{
  namespace xerces
  {
    /*!
      \class ReaderHandler

      \brief This class implements Xerces interface for a handler that receives general document events.

      \sa Reader, ErrorHandler

      \todo Trabalhar com o handler lexico p/ saber se estamos lidando com valores de um elemento ou com um CDATA. Hoje tudo esta como VALUE.
     */
    class TEXERCESEXPORT ReaderHandler : public xercesc::ContentHandler
    {
      public:

        /*! \brief Default constructor. */
        ReaderHandler();

        /*! \brief Destructor. */
        ~ReaderHandler() { }

        void reset();

        /*!
          \brief It receives notification of character data.

          \param chars  The characters from the XML document.
          \param length The number of characters to read from the array.

          \exception SAXException It may throws an exception.
         */
        void characters(const XMLCh* const chars, const XMLSize_t length);

        /*!
          \brief It receives notification of the end of a document.
          
          \exception SAXException It may throws an exception.
         */
        void endDocument();

        /*!
          \brief It receives notification of the end of an element.

          \param uri The   URI of the associated namespace for this element.
          \param localname The local part of the element name.
          \param qname     The QName of this element.

          \exception SAXException It may throws an exception.
         */
        void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname);

        /*!
          \brief It receives notification of ignorable whitespace in element content.

          \param chars  The characters from the XML document.
          \param length The number of characters to read from the array.

          \exception SAXException It may throws an exception.
         */
        void ignorableWhitespace(const XMLCh* const chars, const XMLSize_t length);
        
        /*!
          \brief It receives notification of a processing instruction.

          \param target The processing instruction target.
          \param data   The processing instruction data, or null if none was supplied.

          \exception SAXException It may throws an exception.
         */
        void processingInstruction(const XMLCh* const target, const XMLCh* const data);

        /*!
          \brief It receives an object for locating the origin of SAX document events.

          \param locator An object that can return the location of any SAX document event.
        */
        void setDocumentLocator(const xercesc::Locator* const locator);

        /*!
          \brief It receives notification of the beginning of a document.

          \exception SAXException It may throws an exception.
         */
        void startDocument();

        /*!
          \brief It receives notification of the beginning of an element.

          \param uri The   URI of the associated namespace for this element.
          \param localname The local part of the element name
          \param qname     The QName of this element.
          \param attrs     The attributes attached to the element, if any.

          \exception SAXException It may throws an exception.
         */
        void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const xercesc::Attributes& attrs);

        /*!
          \brief It receives notification of the start of an namespace prefix mapping.

          \param prefix The namespace prefix used.
          \param uri    The namespace URI used.

          \exception SAXException It may throws an exception.
         */
        void startPrefixMapping(const XMLCh* const prefix, const XMLCh* const uri);

        /*!
          \brief It receives notification of the end of an namespace prefix mapping.

          \param prefix The namespace prefix used.

          \exception SAXException It may throws an exception.
         */
        void endPrefixMapping(const XMLCh* const prefix);

        /*!
          \brief It receives notification of a skipped entity.
          
          \param name The name of the skipped entity.

          \exception SAXException It may throws an exception.
         */
        void skippedEntity(const XMLCh* const name);

        /*!
          \brief It return the type of node read by the handler.

          \return The type of node read by the handler.
         */
        te::xml::NodeType getNodeType() const;

        /*!
          \brief TODO

          \return TODO 
         */
        const XMLCh* getElementURI() const { return m_uri; }

        /*!
          \brief TODO

          \return TODO
         */
        const XMLCh* getElementLocalName() const { return m_localname; }

        /*! 
          \brief TODO

          \return TODO
         */
        const XMLCh* getElementQName() const { return m_qname; }

        /*!
          \brief TODO

          \return TODO
         */
        const xercesc::Attributes* getElementAttrs() const { return m_attrs; }

        const XMLCh* getElementValue() const { return m_value; }

        //const XMLCh* getIgnorableWhiteSpace() const { return m_value; }

        XMLSize_t getDataLen() const { return m_len; }

        std::size_t getNumberOfNamespaces() const { return m_nspaces.size(); }

        const std::pair<const XMLCh*, const XMLCh*>& getNamespace(std::size_t i) const { return m_nspaces[i]; }

        void setNodeType(te::xml::NodeType type);

        bool isInContractedForm() const;

        void setInContractedForm(bool d);

      protected:

        const XMLCh* m_uri;                   //!< URI of the associated namespace for the current element.
        const XMLCh* m_localname;             //!< The local part of the current element name.
        const XMLCh* m_qname;                 //!< The QName of the current element.
        const xercesc::Attributes* m_attrs;   //!< The attributes attached to the current element, if any.
        const XMLCh* m_value;                 //!< May be characters from the XML document, ignorable white spaces, CDATA or any other string.
        XMLSize_t m_len;                      //!< The number of characters in the m_value array.
        std::vector<std::pair<const XMLCh*, const XMLCh*> > m_nspaces;  //!< A vector of namespaces (prefix, uri).
        te::xml::NodeType m_nodeType;         //!< The type of node read by the parser.
        bool m_isInContractedForm;            //!< A flag that indicates if the reader handler is on an contracted form element.
    };

  } // end namespace xerces
}   // end namespace te

#endif  // __TERRALIB_XERCES_INTERNAL_CONTENTHANDLER_H

