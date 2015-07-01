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
  \file terralib/xml/Enums.h

  \brief Enumerations of XML module.

  \ingroup xml
*/

#ifndef __TERRALIB_XML_INTERNAL_ENUMS_H
#define __TERRALIB_XML_INTERNAL_ENUMS_H

namespace te
{
  namespace xml
  {
    /*!
      \enum NodeType

      \brief The type of node read by XML reader.
     */
    typedef enum NodeType
    {    
      UNKNOWN = 0,                /*!< If the parser has read something not known.          */
      START_DOCUMENT = 1,         /*!< Signals that the start of document was reached.      */
      END_DOCUMENT = 2,           /*!< Signals that the end of document was reached.        */
      START_ELEMENT = 3,          /*!< The parser has reached the element startint tag.     */
      END_ELEMENT = 4,            /*!< The parser has reached the element closing tag.      */
      VALUE = 5,                  /*!< The parser has reached the text value of an element. */
      CDATA = 6,                  /*!< The parser has read a CDATA.                         */
      IGNORABLE_WHITESPACE = 7,   /*!< The parser has read ignorable whitespaces.           */
      WHITESPACE = 8,             /*!< The parser has read whitespaces.                     */
      PROCESSING_INSTRUCTION = 9, /*!< The parser has read a processing instruction.        */ 
      START_PREFIX_MAPPING = 10,  /*!< The parser is signaling the start of prefix mapping. */
      END_PREFIX_MAPPING = 11,    /*!< The parser is signaling the end of prefix mapping.   */
      SKIPPED_ENTITY = 12         /*!< The parser is signaling an skipped entity.           */
    } NodeType;

  } // end namespace xml
}   // end namespace te

#endif  // __TERRALIB_XML_INTERNAL_ENUMS_H

