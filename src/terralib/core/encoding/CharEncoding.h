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
  \file terralib/core/encoding/CharEncoding.h

  \brief A module for character enconding/decoding handling.

  \author Matheus Cavassan Zaglia
*/


#ifndef __TERRALIB_CORE_ENCODING_CHARENCODING_H
#define __TERRALIB_CORE_ENCODING_CHARENCODING_H

//STL
#include <string>
#include <map>
#include <iostream>


namespace te
{
  namespace core
  {
    /*!
    \enum Encoding
    \brief Supported character encodings.
    */
    enum Encoding
    {
      UTF8,                 /*!< UTF-8 encoding.               */
      CP1250,               /*!< CP1250 encoding.              */
      CP1251,               /*!< CP1251 encoding.              */
      CP1252,               /*!< CP1252 encoding.              */
      CP1253,               /*!< CP1253 encoding.              */
      CP1254,               /*!< CP1254 encoding.              */
      CP1257,               /*!< CP1257 encoding.              */
      LATIN1,               /*!< ISO-8859-1 encoding (Latin1). */
      UNKNOWN_CHAR_ENCODING /*!< Unknown encoding. */
    };

    class CharEncoding
    {
      public:

        /*!
         * \brief Encode a string from an encoding to UTF-8.
         * \param src String to be encoded.
         * \param from Current encoding of the string.
         * \return String encoded in UTF-8.
         */
        static std::string encode(const std::string& src, const Encoding &from);

        /*!
         * \brief Decode a string from UTF-8 to another encoding.
         * \param src String to be decoded.
         * \param to New encoding of the string.
         * \return String with the new encoding
         */
        static std::string decode(const std::string& src, const Encoding &to);

        /*!
         * \brief Convert a string from an encoding to another.
         * \param src Source string to be decoded.
         * \param from Current encoding of the string.
         * \param to New encoding for the string.
         * \return String with the new encoding
         */
        static std::string convert(const std::string& src, const Encoding& from, const Encoding& to);

      private:

        CharEncoding();
        ~CharEncoding();

        static const std::map<Encoding, std::string> EncodingString;
    };
  }
}

#endif //__TERRALIB_CORE_ENCODING_CHARENCODING_H
