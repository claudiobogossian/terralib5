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
  \file terralib/common/LibraryManager.h

  \brief A singleton that can be used to observe the available libraries in the system.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_LIBRARYMANAGER_H
#define __TERRALIB_COMMON_INTERNAL_LIBRARYMANAGER_H

// TerraLib
#include "Config.h"
#include "Library.h"
#include "Singleton.h"

// STL
#include <string>


namespace te
{
  namespace common
  {
    /*!
      \class LibraryManager

      \brief A singleton that can be used to observe the available libraries in the system.

      \note This singleton doesn't control the libraries lifetime, it
            just make smart references to them that will be automatically removed
            when a library goes out of scope (or been destroyed). Actually it works
            like an observer of known libraries.

      \note This class is based on pimpl idiom.

      \ingroup common
    */
    class TECOMMONEXPORT LibraryManager : public Singleton<LibraryManager>
    {
      friend class Singleton<LibraryManager>;

      public:

        /*!
          \brief It adds a new Library to be managed.

          This implementaion will keep the library in the manager
          while there is at least someone pointing to it.
          As soon as no one keeps a reference to it the
          manager will be communicated and the library will not be available
          anymore.

          \param id An identifier used to search for the library in successive lookups.
          \param l  The library to be managed.
        */
        void add(const std::string& id, const LibraryPtr& l) throw();

        /*!
          \brief Returns a null pointer if a library doesnt't exist.

          \param name The library name.

          \return A pointer to an already loaded library or null.
        */
        LibraryPtr find(const std::string& name) throw();

      protected:

        /*! \brief Consructor. */
        LibraryManager();

        /*! \brief Destructor. */
        ~LibraryManager();

      private:

        class Impl;

        Impl* m_pImpl;  //!< A pointer to the real implementation.
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_LIBRARYMANAGER_H

