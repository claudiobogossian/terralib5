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
  \author Gilberto Ribeiro de Queiroz
*/

#ifndef __TERRALIB_CORE_ENCODING_CHARENCODING_H__
#define __TERRALIB_CORE_ENCODING_CHARENCODING_H__

// TerraLib
#include "../Config.h"

// STL
#include <map>
#include <vector>
#include <string>

namespace te
{
  namespace core
  {

    /*!
      \enum EncodingType

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
      LATIN1    /*!< Latin1 encoding (ISO8859-1).  */
    };

    /*!
      \class CharEncoding

      \brief A class for handling character enconding/decoding.
     */
    class TECOREEXPORT CharEncoding
    {
      public:
        /*!
          \brief Convert a string in UTF-8 to the current locale encoding.

          \param src UTF - 8 string.

          \return String encoded according to new character encoding.
         */
        static std::string fromUTF8(const std::string& src);

        /*!
          \brief Convert a string in UTF-8 to another character encoding.

          \param src UTF-8 string.
          \param to  The target character encoding.
         
          \return String encoded according to new character encoding.
         */
        static std::string fromUTF8(const std::string& src, EncodingType to);

        /*!
          \brief Convert a string from a current locale encoding to UTF-8.

          \param src  String to be encoded in UTF-8.

          \return String encoded in UTF-8.
         */
        static std::string toUTF8(const std::string& src);

        /*!
          \brief Convert a string from a given character encoding to UTF-8.

          \param src  String to be encoded in UTF-8.

          \return String encoded in UTF-8.
         */
        static std::string toUTF8(const std::string& src, EncodingType from);
          
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
        /*!
          \brief Retrive a string from a given character encoding type enum.

          \param et Encoding Type to be retrieved as string.

          \return Encoding Type as string.
         */
        static std::string getEncodingName(EncodingType et);

        /*!
          \brief Retrive an EncodingType from a given character encoding name.

          \param name Encoding name to be retrieved as enum.

          \return Encoding as enum.
         */
        static te::core::EncodingType getEncodingType(const std::string &name);

        /*!
        \brief Retrive a vector of string with all available encoding types name.

        \return A vector of encoding type as string.
        */
        static std::vector<std::string> getEncodingsName();

      private:

// Not instatiable
        CharEncoding();
        ~CharEncoding();
    };
  }  // end namespace core
}    // end namespace te

#endif //__TERRALIB_CORE_ENCODING_CHARENCODING_H__
