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
  \file terralib/xml/Reader.h
   
  \brief This class models a XML reader object.
*/

#ifndef __TERRALIB_XML_INTERNAL_READER_H
#define __TERRALIB_XML_INTERNAL_READER_H

// TerraLib
#include "Config.h"
#include "Enums.h"

// STL
#include <string>

// Boost
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>

namespace te
{
  namespace xml
  {
    /*!
      \class Reader

      \brief This class models a XML reader object.

      This API is intend to simplify the task of reading XML files. We have
      tried to model it close to the Libxml2 API (see http://xmlsoft.org/xmlreader.html for more information)
      but keeping all Xerces-C++ support capabilities: schema validation and grammar cache.

      \ingroup xml
    */
    class TEXMLEXPORT Reader : public boost::noncopyable
    {
      public:

        /*! \brief Default constructor. */
        Reader() { }

        /*! \brief Virtual destructor. */
        virtual ~Reader() { }

        /*!
          \brief It enables or disables the parser namespace processing.

          \param d If true the internal parser will perform the namespace processing otherwise this will not be done.

          \note By default the reader is set to do namespace processing.
        */
        virtual void setDoNamespaces(bool d) = 0;

        /*!
          \brief It enables or disables the parser schema processing.

          \param d If true the internal parser will perform the schema processing otherwise this will not be done.

          \note If set to true the namespace processing must also be turned on.

          \note By default the reader is set to parser schema.
        */
        virtual void setDoSchema(bool d) = 0;

        /*!
          \brief If true the parser will perform a validation scheme.

          \param d If true the parser will perform a validation scheme.

          \note By default the reader is set to validate the scheme.
        */
        virtual void setValidationScheme(bool d) = 0;

        /*!
          \brief If true the reader will use cached grammar if it exists in the pool.

          \param d If true the reader will use cached grammar if it exists in the pool.

          \note By default the reader is set to use cached grammar.
        */
        virtual void setUseCachedGrammarInParse(bool d) = 0;

        /*!
          \brief If true it caches the grammar in the pool for re-use in subsequent parses.

          \param d If If true it caches the grammar in the pool for re-use in subsequent parses.

          \note By default the reader is set to cache the grammar.
        */
        virtual void setCacheGrammarFromParse(bool d) = 0;

        /*!
          \brief If true the parser will ignore the white space characters.

          \param d If true the parser will ignore the white space characters.

          \note By default the reader is set to consider white spaces.
        */
        virtual void setIgnoreWhiteSpaces(bool d) = 0;

        /*!
          \brief It prepare the given file to be read.

          You must call read to start parsing the XML document.

          \param fileURI A path to a XML file or any URI where it can be found.

          \exception Exception It throws an exception if the file doesn't exist or if the internal parser can not read it.
        */
        virtual void read(const std::string& fileURI) = 0;

        /*!
          \brief It gets the next event to be read.

          \return True if an event was read otherwise false.

          \exception Exception It throws an exception if something goes wrong during the text read.
        */
        virtual bool next() = 0;

        /*!
          \brief It return the type of node read.

          \return The type of node read.
        */
        virtual NodeType getNodeType() const = 0;

        /*!
          \brief It returns the URI of the associated namespace in the case of an element node.

          \return The URI of the associated namespace in the case of an element node.
        */
        virtual std::string getElementURI() const = 0;

        /*!
          \brief It returns the local part of the element name in the case of an element node.

          \return The local part of the element name in the case of an element node.
        */
        virtual std::string getElementLocalName() const = 0;

        /*!
          \brief It returns the qualified name in the case of an element node.

          \return The qualified name in the case of an element node.
        */
        virtual std::string getElementQName() const = 0;

        /*!
          \brief It returns the element data value in the case of VALUE node.

          \return The element data value in the case of VALUE node.
        */
        virtual std::string getElementValue() const = 0;

        /*!
          \brief It returns the element data value in the case of VALUE node.

          \return The element data value in the case of VALUE node.

          \note Just call this method if you know that the element value can be converted to a 32-bit integer.
        */
        virtual boost::int32_t getElementValueAsInt32() const;

        /*!
          \brief It returns the element data value in the case of VALUE node.

          \return The element data value in the case of VALUE node.

          \note Just call this method if you know that the element value can be converted to a double.
        */
        virtual double getElementValueAsDouble() const;

        /*!
          \brief It returns the element data value in the case of VALUE node.

          \return The element data value in the case of VALUE node.

          \note Just call this method if you know that the element value can be converted to a boolean.
        */
        virtual bool getElementValueAsBoolean() const;

        /*!
          \brief It returns the element data value in the case of VALUE node.

          \return The element data value in the case of VALUE node.

          \note Just call this method if you know that the element value can be converted to a bool.
        */
        //virtual bool getElementValueAsBool() const;

        /*!
          \brief It returns the element data value length in the case of VALUE or CDATA node.

          \return The element data value in the case of VALUE or CDATA node.
        */
        virtual std::size_t getElementDataLen() const = 0;

        /*!
          \brief It tells if the element has attributes in the case of an element node.

          \return True if the element has attributes in the case of an element node, otherwise, false.
        */
        virtual bool hasAttrs() const = 0;

        /*!
          \brief It returns the number of attributes in the case of an element node.

          \return The number of attributes in the case of an element node.
        */
        virtual std::size_t getNumberOfAttrs() const = 0;

        /*!
          \brief It returns the attribute value in the case of an element node with valid attributes.

          \param name The attribute name.

          \return The attribute value in the case of an element node with valid attributes.
        */
        virtual std::string getAttr(const std::string& name) const = 0;

        /*!
          \brief It returns the attribute value in the case of an element node with valid attributes.

          \param i The attribute position.

          \return The attribute value in the case of an element node with valid attributes.
        */
        virtual std::string getAttr(std::size_t i) const = 0;

        /*!
          \brief It returns the attribute value in the case of an element node with valid attributes.

          \param name The attribute name.

          \return The attribute value in the case of an element node with valid attributes.

          \note Just call this method if you know that the attribute value can be converted to a 32-bit integer.
        */
        virtual boost::int32_t getAttrAsInt32(const std::string& name) const;

        /*!
          \brief It returns the attribute value in the case of an element node with valid attributes.

          \param i The attribute position.

          \return The attribute value in the case of an element node with valid attributes.

          \note Just call this method if you know that the attribute value can be converted to a 32-bit integer.
        */
        virtual boost::int32_t getAttrAsInt32(std::size_t i) const;

        /*!
          \brief It returns the attribute value in the case of an element node with valid attributes.

          \param i The attribute position.

          \return The attribute value in the case of an element node with valid attributes.

          \note Just call this method if you know that the attribute value can be converted to a 32-bit unsigned integer.
        */
        virtual boost::uint32_t getAttrAsUInt32(std::size_t i) const;

        /*!
          \brief It returns the attribute value in the case of an element node with valid attributes.

          \param name The attribute name.

          \return The attribute value in the case of an element node with valid attributes.

          \note Just call this method if you know that the attribute value can be converted to a 32-bit unsigned integer.
        */
        virtual boost::uint32_t getAttrAsUInt32(const std::string name) const;

        /*!
          \brief It returns the attribute value in the case of an element node with valid attributes.

          \param name The attribute name.

          \return The attribute value in the case of an element node with valid attributes.

          \note Just call this method if you know that the attribute value can be converted to a double.
        */
        virtual double getAttrAsDouble(const std::string& name) const;

        /*!
          \brief It returns the attribute value in the case of an element node with valid attributes.

          \param i The attribute position.

          \return The attribute value in the case of an element node with valid attributes.

          \note Just call this method if you know that the attribute value can be converted to a double.
        */
        virtual double getAttrAsDouble(std::size_t i) const;

        /*!
          \brief It returns the local part of the attribute name for the i-th attribute.

          \param i The attribute position index.

          \return The local part of the attribute name in the case of an element node.
        */
        virtual std::string getAttrLocalName(std::size_t i) const = 0;

        /*!
          \brief It returns the qualified name for the i-th attribute.

          \param i The attribute position index.

          \return The qualified attribute name in the case of an element node.
        */
        virtual std::string getAttrQName(std::size_t i) const = 0;

        /*!
          \brief It returns the attribute URI of the associated namespace in the case of an element node.

          \param i The attribute position index.

          \return The attribute URI of the associated namespace in the case of an element node.
        */
        virtual std::string getAttrURI(std::size_t i) const = 0;

        /*!
          \brief It returns the attribute position.

          \param name The attribute name.

          \return The attribute position.
        */
        virtual std::size_t getAttrPosition(const std::string& name) const = 0;

        virtual std::size_t getNumberOfNamespaces() const = 0;

        virtual void getNamespace(std::size_t i, std::pair<std::string, std::string>& ns) const = 0;

        /*!
          \brief It sets the maximal allowed buffer size used for parsing.

          \param size The maximal allowed buffer size used for parsing.

          \note Default: 65536 bytes (64 kbytes), see the macro TE_XML_READER_MAX_BUFFSIZE.
          \note The default value may be implementation dependent!
        */
        virtual void setInternalBufferSize(const std::size_t size) = 0;
    };

  } // end namespace xml
}   // end namespace te

#endif  // __TERRALIB_XML_INTERNAL_READER_H

