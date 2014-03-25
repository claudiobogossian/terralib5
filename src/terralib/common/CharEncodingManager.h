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
  \file  terralib/common/CharEncodingManager.h

  \brief A singleton for managing the character encodings registered in the system.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_CHARENCODINGMANAGER_H
#define __TERRALIB_COMMON_INTERNAL_CHARENCODINGMANAGER_H

// TerraLib
#include "CharEncoding.h"
#include "Singleton.h"

// STL
#include <map>
#include <vector>

namespace te
{
  namespace common
  {
    /*!
      \class CharEncodingManager

      \brief A singleton for managing the character encodings registered in the system.

      This singleton can be used to keep the list of character encodings supported by the system.
    */
    class TECOMMONEXPORT CharEncodingManager : public Singleton<CharEncodingManager>
    {
      friend class Singleton<CharEncodingManager>;

      public:

        /*!
          \brief It adds the given character encoding category to the list of managed character encodings.

          \param category A character encoding category to be managed by this singleton.

          \note The singleton will take the ownership of the given category.
        */
        //void add(const CharEncoding& cencoding, const std::string& category);

        /*!
          \brief It returns the list of character encoding categories registered in the system.

          \return The list of character encoding categories registered in the system.
        */
        //const std::vector<CharEncoding>& getEncodings(const std::string& category) const;

        /*!
          \brief It returns the list of character encoding categories registered in the system.

          \return The list of character encoding categories registered in the system.
        */
        //const CharEncoding& getEncoding(const std::string& name) const;

        /*!
          \brief It verifies if a character encoding category with the given name is already registered in the system.

          \param name The character encoding category to be evaluated.

          \return True if a character encoding category with the given name is already registered in the system, otherwise returns false.
        */
        //bool existsCategory(const std::string& name) const;

        /*!
          \brief It verifies if a character encoding with the given name is already registered in the system.

          \param name The character encoding to be evaluated.

          \return True if a character encoding with the given name is already registered in the system, otherwise returns false.
        */
        //bool existsCharEncoding(const std::string& name) const;

      protected:

        /*! \brief Singleton constructor is protected. */
        CharEncodingManager();

        /*! \brief Destructor. */
        ~CharEncodingManager();

      private:

        //std::vector<CharEncodingCategory*> m_categories;  //!< The list of supported character encodings organized by categories.
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_CHARENCODINGMANAGER_H
