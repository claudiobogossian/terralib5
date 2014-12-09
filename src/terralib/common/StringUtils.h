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
  \file terralib/common/StringUtils.h

  \brief Utility functions for dealing with strings.

  \ingroup common
*/

#ifndef __TERRALIB_COMMON_INTERNAL_STRINGUTILS_H
#define __TERRALIB_COMMON_INTERNAL_STRINGUTILS_H

// STL
#include <cstdio>
#include <cstring>
#include <limits>
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/cstdint.hpp>

namespace te
{
  namespace common
  {
    /*!
      \brief It converts a short integer value to a string.

      \return The integer value converted to a string.
     */
    inline std::string Convert2String(boost::int16_t value)
    {
      char name[std::numeric_limits<boost::int16_t>::digits10 + 2];
      sprintf(name, "%hd", value);
      return name;
    }
    
    /*!
      \brief It converts a unsigned short integer value to a string.

      \return The integer value converted to a string.
     */
    inline std::string Convert2String(boost::uint16_t value)
    {
      char name[std::numeric_limits<boost::uint16_t>::digits10 + 2];
      sprintf(name, "%hu", value);
      return name;
    }        
    
    /*!
      \brief It converts an integer value to a string.

      \return The integer value converted to a string.
     */
    inline std::string Convert2String(boost::int32_t value)
    {
      char name[std::numeric_limits<boost::int32_t>::digits10 + 2];
      sprintf(name, "%d", value);
      return name;
    }

    /*!
      \brief It converts an unsigned integer value to a string.

      \return The unsigned integer value converted to a string.
     */
    inline std::string Convert2String(boost::uint32_t value)
    {
      char name[std::numeric_limits<boost::uint32_t>::digits10 + 2];
      sprintf(name, "%u", value);
      return name;
    }
    
    /*!
      \brief It converts a long value to a string.

      \return The long value converted to a string.
     */
    inline std::string Convert2String(boost::int64_t value)
    {
      char name[std::numeric_limits<boost::int64_t>::digits10 + 2];
      sprintf(name, "%lli", value);
      return name;
    }    

    /*!
      \brief It converts a unsigned long value to a string.

      \return The unsigned long value converted to a string.
     */
    inline std::string Convert2String(boost::uint64_t value)
    {
      char name[std::numeric_limits<boost::uint64_t>::digits10 + 2];
      sprintf(name, "%llu", value);
      return name;
    }

    /*!
      \brief It converts a double value to a string.

      \return The double value converted to a string.
     */
    inline std::string Convert2String(float value)
    {
      char name[std::numeric_limits<float>::digits10 + std::numeric_limits<float>::digits + 4];
      sprintf(name, "%f", value);
      return name;
    }

    /*!
      \brief It converts a double value to a string.

      \return The double value converted to a string.
     */
    inline std::string Convert2String(double value)
    {
      char name[std::numeric_limits<double>::digits10 + std::numeric_limits<double>::digits + 4];
      sprintf(name, "%e", value);
      return name;
    }

    /*!
      \brief It converts a double value to a string with a precision.

      \return The double value converted with the given precision to a string.

      \warning Max allowed precision is 20.
     */
    inline std::string Convert2String(double value, int precision)
    {
      char name[256];
      sprintf(name, "%.*f", precision, value );
      return name;
    }

    /*!
      \brief It converts a string to upper case.

      \param value The string value to be converted.

      \return The converted string.
     */  
    inline std::string Convert2UCase(const std::string& value)
    {
      size_t size = value.size();

      std::string aux(size, ' ');

      for(size_t i = 0; i < size; ++i)
        aux[i] = ((value[i] >= 97) && (value[i] <= 122)) ? (value[i] - 32) : value[i];

      return aux;
    }  

    /*!
      \brief It converts a string to upper case in place (it doesn't allocate an auxiliar buffer).

      \param value The string value to be converted.

      \return The converted string.
     */  
    inline void Convert2UCaseInPlace(std::string& value)
    {
      size_t size = value.size();

      for(size_t i = 0; i < size; ++i)
        value[i] = ((value[i] >= 97) && (value[i] <= 122)) ? (value[i] - 32) : value[i];
    }

    /*!
      \brief It converts a string to lower case.

      \param value The string value to be converted.

      \return The converted string.
     */  
    inline std::string Convert2LCase(const std::string& value)
    {
      size_t size = value.size();

      std::string aux(size, ' ');

      for(size_t i = 0; i < size; ++i)
        aux[i] = ((value[i] >= 65) && (value[i] <= 90)) ? (value[i] + 32) : value[i];

      return aux;
    }  

    /*!
      \brief It tokenizes a given string with a delimiter of your own choice.

      \param str        The string to be tokenize.
      \param tokens     The output vector with the tokens.
      \param delimiters The character delimiters.
     */
    inline void Tokenize(const std::string& str,
                         std::vector<std::string>& tokens,
                         const std::string& delimiters = " ")
    {
// skip delimiters at beginning
      size_t lastPos = str.find_first_not_of(delimiters, 0);

// find first "non-delimiter"
      size_t pos = str.find_first_of(delimiters, lastPos);

      while((std::string::npos != pos) ||
            (std::string::npos != lastPos))
      {
// found a token, add it to the vector
        tokens.push_back(str.substr(lastPos, pos - lastPos));

// skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);

// find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
      }
    }

    /*!
      \brief It extracts a key-value map from a string.
      
      \param kvpStr       A string with key-value-pairs to be splitted.
      \param kvp          A map to output the pairs: (parameter-name, parameter-value).
      \param kvpDelimiter The character used to delimit the key-value-pairs.
      \param kvDelimiter  The character used to delimit a key from its value.
      \param toUpper      If true the key will be converted to upper-case. The value case will not be touched.

      \pre The kvpStr must already be cleaned if necessary (see CleanQueryString function if needed).
     */
    inline void ExtractKVP(const std::string& kvpStr,
                           std::map<std::string, std::string>& kvp,
                           const std::string& kvpDelimiter = "&",
                           const std::string& kvDelimiter = "=",
                           bool toUpper = false)
    {
      if(kvpStr.empty())
        return;

// split query string
      std::vector<std::string> tokens;

      Tokenize(kvpStr, tokens, kvpDelimiter);

// for each token let's find a kvp
      size_t size = tokens.size();
  
      for(size_t i = 0; i < size; ++i)
      {
        std::vector<std::string> kv;

        Tokenize(tokens[i], kv, kvDelimiter);

        //if(kvp.empty())
        //  continue;

        if(toUpper)
          Convert2UCaseInPlace(kv[0]);

        if(!kv[0].empty())
          kvp[kv[0]] = "";

        if(kv.size() > 1)
          kvp[kv[0]] = kv[1];

        if(kv.size() > 2)  // if we have >2 '=' ? concat
        {
          std::string aux("");

          for(size_t k = 1; k < kv.size(); ++k)
            aux += kv[k] + "=";

          size_t pos = aux.size() - 1;

          kvp[kv[0]] = aux.erase(pos, 1);
        }
      }
    }

    /*!
      \brief It converts the C++ string to a C-string.

      \param s The C++ string to be converted to a char*.

      \return A C-string. The caller will take the ownership of the returned string and must use 'delete [] pointer' to release it when necessary.
     */
    inline char* CreateCString(const std::string& s)
    {
      char* cs = new char[s.length() + 1];
      strncpy(cs, s.c_str(), s.length() + 1);
      return cs;
    }

    /*!
      \brief It converts the C++ vector of string pointers to a C array os strings.

      \param vs The C++ vector of string pointers to be converted to a char**.

      \return A C array os strings NULL terminated. The caller will take the ownership of the returned array.
     */
    inline char** CreateCStringArray(const std::vector<std::string*>& vs)
    {
      std::size_t ns = vs.size() ;

      char** as = new char*[ns + 1];

      as[ns] = 0;

      for(std::size_t i = 0; i< ns; ++i)
      {
        as[i] = new char[vs[i]->length() + 1];

        strncpy(as[i], vs[i]->c_str(), vs[i]->length() + 1);
      }
      
      return as;
    }

    /*!
      \brief It converts the C++ vector of string pointers to a C array os strings.

      \param vs The C++ vector of string pointers to be converted to a char**.

      \return A C array os strings NULL terminated. The caller will take the ownership of the returned array.
     */
    inline char** CreateCStringArray(const std::vector<std::string>& vs)
    {
      std::size_t ns = vs.size() ;

      char** as = new char*[ns + 1];

      as[ns] = 0;

      for(std::size_t i = 0; i< ns; ++i)
      {
        as[i] = new char[vs[i].length() + 1];

        strncpy(as[i], vs[i].c_str(), vs[i].length() + 1);
      }
      
      return as;
    }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_STRINGUTILS_H

