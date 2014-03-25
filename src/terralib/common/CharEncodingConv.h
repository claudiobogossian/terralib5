/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/CharEncodingConv.h

  \brief A class that deals with character encoding/decoding.
 */

#ifndef __TERRALIB_COMMON_INTERNAL_CHARENCODINGCONV_H
#define __TERRALIB_COMMON_INTERNAL_CHARENCODINGCONV_H

// TerraLib
#include "Config.h"
#if 0;//TE_CHARENCODING_ENABLED

// STL
#include <string>

// Forward declarations
extern "C"
{
  typedef void* iconv_t;
}

namespace te
{
  namespace common
  {
    /*!
      \class CharEncodingConv

      \brief This class deals with character encoding/decoding.
     */
    class TECOMMONEXPORT CharEncodingConv
    {
      public:

        /*!
          \brief It prepare sthe conversion from a charset code to another one.

          \fromCode The name of the origen charset of the string to be converted to destiny charset.
          \toCode   The name of the destiny charset of the string to be converted.
         */
        CharEncodingConv(const std::string& fromCode, const std::string& toCode);

        /*! \brief Destructor. */
        ~CharEncodingConv();

        /*!
          \brief It converts the source string to a target charset.

          \param src The string in a source charset.

          \return The string converted to the destiny charset.

          \exception Exception It throws an exception if the conversion can not be done.

          \note This method tries to cache the internal encoder/decoder machine.

          \note Once  an exception is thrown the converter object can not be used anymore because it can be in an state that can not be used to convert text anymore.
         */
        std::string conv(const std::string& src);

        /*!
          \brief An static method that converts the source string to a target charset.

          \param src      The string in a source charset.
          \param fromCode The name of the origen charset of the string to be converted to destiny charset.
          \param toCode   The name of the destiny charset of the string to be converted.

          \return The string converted to the destiny charset.

          \exception Exception It throws an exception if the conversion can not be done.

          \note This method doesn't cache the internal encoder/decoder machine.
         */
        static std::string convert(const std::string& src, const std::string& fromCode, const std::string& toCode);

      private:

        /*! \brief Copy constructor not allowed! */
        CharEncodingConv(const CharEncodingConv& rhs);

        /*! \brief Assignment operator not allowed! */
        CharEncodingConv& operator=(const CharEncodingConv& rhs);

      private:

        const std::string m_fromCode; //!< The origin charset.
        const std::string m_toCode;   //!< The target charset.
        iconv_t m_cd;                 //!< The iconv state machine used to convert string from one character to another one.
    };

  } // end namespace common
}   // end namespace te

#endif // TE_CHARENCODING_ENABLED

#endif  // __TERRALIB_COMMON_INTERNAL_CHARENCODINGCONV_H
