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
  \file Utils.h
   
  \brief This file contains several utilities functions for dealing with Xerces in TerraLib.
*/

#ifndef __TERRALIB_XERCES_INTERNAL_UTILS_H
#define __TERRALIB_XERCES_INTERNAL_UTILS_H

// TerraLib
#include "Config.h"

// Xerces-C++
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLString.hpp>

// STL
#include <string>

namespace te
{
  namespace xerces
  {
    /*!
      \brief It return true if the two strings are identical, otherwise, it returns false.

      \param s1 The first string to compare.
      \param s2 The second string to compare.

      \return True if the two strings are identical, otherwise, it returns false.

      \note This function is intend to be inlined and just convert
            the XMLCh string to a char* and then compare the two strings.
    */
    inline bool Equals(const XMLCh* const s1, const char* const s2)
    {
      char* s1Aux = xercesc::XMLString::transcode(s1);

      if(strcmp(s1Aux, s2) == 0)
      {
        xercesc::XMLString::release(&s1Aux);
        return true;
      }
      else
      {
        xercesc::XMLString::release(&s1Aux);
        return false;
      }
    }

    /*!
      \brief It return true if the two strings are different, otherwise, it returns false.

      \param s1 The first string to compare.
      \param s2 The second string to compare.

      \return True if the two strings are different, otherwise, it returns false.

      \note This function is intend to be inlined and just convert
            the XMLCh string to a char* and then compare the two strings.
    */
    inline bool NotEquals(const XMLCh* const s1, const char* const s2)
    {
      char* s1Aux = xercesc::XMLString::transcode(s1);

      if(strcmp(s1Aux, s2) == 0)
      {
        xercesc::XMLString::release(&s1Aux);
        return false;
      }
      else
      {
        xercesc::XMLString::release(&s1Aux);
        return true;
      }
    }

    /*!
      \brief It converts the XML string to an integer value.

      \param value An XML string containing a valid int value.

      \return An int value.
    */
    inline int ToInt(const XMLCh* value)
    {
      char* sAux = xercesc::XMLString::transcode(value);
      int iVal = atoi(sAux);
      xercesc::XMLString::release(&sAux);
      return iVal;
    }

    /*!
      \brief It converts the XML string to a standard C++ string.

      \param value An XML string containing a valid string value.

      \return An int value.

      \note If the pointer is NULL this method will return an empty string.
    */
    inline std::string ToString(const XMLCh* const value)
    {
      if(value == 0)
        return std::string("");

      char* sAux = xercesc::XMLString::transcode(value);
      std::string s(sAux);
      return s;
    }

    /*!
      \brief It returns the given attribute value.

      \param attrs   The attributes attached to an element, if any.
      \param attName The name of the attribute we are looking for its value.

      \return The attribute value or NULL if it is not found.

      \note The caller will take the ownership of the returned pointer. Use
            xercesc::XMLString::release(&val) in order to release the returned value.
    */
    inline char* GetAttributeVal(const xercesc::Attributes& attrs, const XMLCh* const attName)
    {
      const XMLSize_t nattributes = attrs.getLength();

      for(XMLSize_t i = 0; i < nattributes; ++i)
        if(xercesc::XMLString::equals(attrs.getLocalName(i), attName))
          return xercesc::XMLString::transcode(attrs.getValue(i));

      return 0;
    }

    /*!
      \brief It reads the XML file using the DOM API.

      \param xmlFile     The XML file to be read.
      \param doNamespace If true the namespace processing will be done.
      \param doSchema    If true the parser will attempt to use the XML schema.
      \param valid       If true the parser will valid the XML file against the schema.

      \return The read dcument.

      \exception Exception If the parser can not read the file it will raise an exception.
    */
    TEXERCESEXPORT xercesc::DOMDocument* readXML(const std::string& xmlFile,
                                                 const bool doNamespace = false,
                                                 const bool doSchema = false,
                                                 const bool valid = false);

  } // end namespace xerces
}   // end namespace te

#endif  // __TERRALIB_XERCES_INTERNAL_UTILS_H

