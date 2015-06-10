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

    /*!
      \brief It build two string vectors with special upper case characters and the his respective normal characters

      \param vecUpperIn  The vector with upper case special characters.
      \param vecUpperOut The vector with upper case normal characters.

    */
    inline void GetAccentuatedUpperVector(std::vector<std::string> & vecUpperIn, std::vector<std::string> & vecUpperOut)
    {
      static std::vector<std::string> upperIn;
      static std::vector<std::string> upperOut;

      if(upperIn.empty() == true || upperOut.empty() == true)
      {
        upperIn.clear();
        upperOut.clear();

        upperIn.push_back("Á");
        upperIn.push_back("É");
        upperIn.push_back("Í");
        upperIn.push_back("Ó");
        upperIn.push_back("Ú");
        upperIn.push_back("À");
        upperIn.push_back("È");
        upperIn.push_back("Ì");
        upperIn.push_back("Ò");
        upperIn.push_back("Ù");
        upperIn.push_back("Ä");
        upperIn.push_back("Ë");
        upperIn.push_back("Ï");
        upperIn.push_back("Ö");
        upperIn.push_back("Ü");
        upperIn.push_back("Â");
        upperIn.push_back("Ê");
        upperIn.push_back("Î");
        upperIn.push_back("Ô");
        upperIn.push_back("Û");
        upperIn.push_back("Ã");
        upperIn.push_back("Õ");
        upperIn.push_back("Ç");


        upperOut.push_back("A");
        upperOut.push_back("E");
        upperOut.push_back("I");
        upperOut.push_back("O");
        upperOut.push_back("U");
        upperOut.push_back("A");
        upperOut.push_back("E");
        upperOut.push_back("I");
        upperOut.push_back("O");
        upperOut.push_back("U");
        upperOut.push_back("A");
        upperOut.push_back("E");
        upperOut.push_back("I");
        upperOut.push_back("O");
        upperOut.push_back("U");
        upperOut.push_back("A");
        upperOut.push_back("E");
        upperOut.push_back("I");
        upperOut.push_back("O");
        upperOut.push_back("U");
        upperOut.push_back("A");
        upperOut.push_back("O");
        upperOut.push_back("C");
      }

      vecUpperIn = upperIn;
      vecUpperOut = upperOut;

    }

    /*!
      \brief It build two string vectors with special lower case characters and the his respective normal characters

      \param vecUpperIn  The vector with lower case special characters.
      \param vecUpperOut The vector with lower case normal characters.

    */
    inline void GetAccentuatedLowerVector(std::vector<std::string> & vecLowerIn, std::vector<std::string> & vecLowerOut)
    {
      static std::vector<std::string> lowerIn;
      static std::vector<std::string> lowerOut;

      if(lowerIn.empty() == true || lowerOut.empty() == true)
      {
        lowerIn.clear();
        lowerOut.clear();

        lowerIn.push_back("á");
        lowerIn.push_back("é");
        lowerIn.push_back("í");
        lowerIn.push_back("ó");
        lowerIn.push_back("ú");
        lowerIn.push_back("à");
        lowerIn.push_back("è");
        lowerIn.push_back("ì");
        lowerIn.push_back("ò");
        lowerIn.push_back("ù");
        lowerIn.push_back("ä");
        lowerIn.push_back("ë");
        lowerIn.push_back("ï");
        lowerIn.push_back("ö");
        lowerIn.push_back("ü");
        lowerIn.push_back("â");
        lowerIn.push_back("ê");
        lowerIn.push_back("î");
        lowerIn.push_back("ô");
        lowerIn.push_back("û");
        lowerIn.push_back("ã");
        lowerIn.push_back("õ");

        std::string str = "ç";
        lowerIn.push_back(str);

        lowerOut.push_back("a");
        lowerOut.push_back("e");
        lowerOut.push_back("i");
        lowerOut.push_back("o");
        lowerOut.push_back("u");
        lowerOut.push_back("a");
        lowerOut.push_back("e");
        lowerOut.push_back("i");
        lowerOut.push_back("o");
        lowerOut.push_back("u");
        lowerOut.push_back("a");
        lowerOut.push_back("e");
        lowerOut.push_back("i");
        lowerOut.push_back("o");
        lowerOut.push_back("u");
        lowerOut.push_back("a");
        lowerOut.push_back("e");
        lowerOut.push_back("i");
        lowerOut.push_back("o");
        lowerOut.push_back("u");
        lowerOut.push_back("a");
        lowerOut.push_back("o");
        lowerOut.push_back("c");
      }

      vecLowerIn = lowerIn;
      vecLowerOut = lowerOut;
    }

    /*!
      \brief It build two string vectors with special characters and the his respective normal characters

      \param especialIn  The vector with special characters.
      \param especialOut The vector with normal characters.

    */
    inline void GetEspecialCharsFixVector(std::vector<std::string> & especialIn, std::vector<std::string> & especialOut)
    {
      especialIn.push_back("ª");
      especialIn.push_back("º");
      especialIn.push_back("¹");
      especialIn.push_back("²");
      especialIn.push_back("³");

      especialOut.push_back("a");
      especialOut.push_back("o");
      especialOut.push_back("1");
      especialOut.push_back("2");
      especialOut.push_back("3");
    }

    /*!
      \brief It replace special characters of a string.

      \param str     The string that will be verify.
      \param changed Boolean that records whether there was a change.

    */
    inline std::string ReplaceSpecialChars ( const std::string& str, bool& changed)
    {

      changed = false;

      std::vector<std::string> upperIn;
      std::vector<std::string> upperOut;
      std::vector<std::string> lowerIn;
      std::vector<std::string> lowerOut;
      std::vector<std::string> especialIn;
      std::vector<std::string> especialOut;

      GetAccentuatedUpperVector(upperIn, upperOut);
      GetAccentuatedLowerVector(lowerIn, lowerOut);
      GetEspecialCharsFixVector(especialIn, especialOut);


      std::string outputStr = str;
      for(unsigned int i = 0; i < outputStr.size(); ++i)
      {
        std::string value = "";
        value += outputStr[i];
        for(unsigned int j = 0; j < upperIn.size(); ++j)
        {
          if(outputStr[i] == upperIn[j][0])
          {
            outputStr[i] = upperOut[j][0];
            changed = true;
            break;
          }
        }

        for(unsigned int j = 0; j < lowerIn.size(); ++j)
        {
          if(value == lowerIn[j])
          {
            outputStr[i] = lowerOut[j][0];
            changed = true;
            break;
          }
        }

        for(unsigned int j = 0; j < especialIn.size(); ++j)
        {
          if(outputStr[i] == especialIn[j][0])
          {
            outputStr[i] = especialOut[j][0];
            changed = true;
            break;
          }
        } 

      }

      return outputStr;

    }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_STRINGUTILS_H

