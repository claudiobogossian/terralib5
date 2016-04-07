/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/core/encoding/CharEncoding.h

  \brief A class for handling character enconding/decoding.

  \author Matheus Cavassan Zaglia
*/


#ifndef __TERRALIB_CORE_ENCODING_CHARENCODING_H
#define __TERRALIB_CORE_ENCODING_CHARENCODING_H

#include "../Config.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace core
  {

    /*!
      \enum Encoding

      \brief Supported character encodings.
    */
    enum class EncodingType
    {
      UTF8,     /*!< UTF-8 encoding.               */
      CP1250,   /*!< CP1250 encoding.              */
      CP1251,   /*!< CP1251 encoding.              */
      CP1252,   /*!< CP1252 encoding.              */
      CP1253,   /*!< CP1253 encoding.              */
      CP1254,   /*!< CP1254 encoding.              */
      CP1257,   /*!< CP1257 encoding.              */
      LATIN1,   /*!< ISO-8859-1 encoding (Latin1). */
      UNKNOWN   /*!< Unknown encoding.             */
    };

    /*!
      \class CharEncoding
     
      \brief A class for handling character enconding/decoding.
     */
    class TEMODULEEXPORT CharEncoding
    {
      public:

        /*!
          \brief Convert a string from a given character encoding to UTF-8.
         
          \param src  String to be encoded in UTF-8.
          \param from String character encoding.
         
          \return String encoded in UTF-8.
         */
        static std::string toUTF8(const std::string& src, EncodingType from);

        /*!
          \brief Convert a string in UTF-8 to another character encoding.

          \param src UTF-8 string.
          \param to  The target character encoding.

          \return String encoded according to new character encoding.
         */
        static std::string fromUTF8(const std::string& src, EncodingType to);

        /*!
          \brief Convert a string from one character encoding to another one.
         
          \param src  String encoded according to "from" encoding type.
          \param from Current string encoding.
          \param to   New encoding for the string.
         
          \return String in a new encoding.
         */
        static std::string convert(const std::string& src,
                                   EncodingType from,
                                   EncodingType to);

      private:

// Not instantiable
        CharEncoding();
      
        ~CharEncoding();

        static const std::map<EncodingType, std::string> EncodingString; //!< An index over character encoding types.
    };
    
  }  // end namespace core
}    // end namespace te

#endif //__TERRALIB_CORE_ENCODING_CHARENCODING_H
