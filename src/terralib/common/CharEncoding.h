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
  \file terralib/common/CharEncoding.h

  \brief A class that describes a character encoding.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_CHARENCODING_H
#define __TERRALIB_COMMON_INTERNAL_CHARENCODING_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace common
  {
    /*!
      \class CharEncoding

      \brief A class that describes a charset (character encoding).

      As described in Wikipedia (http://en.wikipedia.org/wiki/Character_encoding),
      the terms character encoding, character set (charset), and sometimes character map or code page
      are used interchangeably.

      \sa CharEncodingCategory, CharEncodingManager
    */
    class TECOMMONEXPORT CharEncoding
    {
      public:

        /*!
          \brief Constructor.

          \param name        The character encoding.
          \param description A brief description about the character encoding.
        */
        CharEncoding(const std::string& name, const std::string& description);

        /*! \brief Destructor. */
        ~CharEncoding();

        /*!
          \brief It returns the charset name.

          \return The charset name.
        */
        const std::string& getName() const;

        /*!
          \brief It returns a brief description about the charset.

          \return A brief description about the charset.
        */
        const std::string& getDescription() const;

      private:

        std::string m_name;         //!< Character encoding name.
        std::string m_description;  //!< A brief description about the character encoding.
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_CHARENCODING_H
