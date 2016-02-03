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
  \file terralib/module/LibraryManager.h

  \brief A singleton that can be used to observe the available shared libraries in the system.

  \author Gilberto Ribeiro de Queiroz
 */
 
#ifndef __TERRALIB_MODULE_LIBRARYMANAGER_H__
#define __TERRALIB_MODULE_LIBRARYMANAGER_H__

// TerraLib
#include "Config.h"

// STL
#include <memory>
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace module
  {
// Forward declaration
    class Library;

    //! A singleton that can be used to observe the available libraries in the system.
    /*!
      This singleton doesn't control the libraries lifetime, it just make smart
      references to them. These references will be automatically removed
      when a library goes out of scope (or been destroyed). Actually it works
      like an observer of known libraries.

      \note This class is based on pimpl idiom.
     */
    class TEMODULEEXPORT LibraryManager : public boost::noncopyable
    {
      public:

        //! Add a new library to be managed.
        /*!
          This implementation will keep the library in the manager
          while there is at least someone pointing to it.
          As soon as no one keeps a reference to it the
          manager will be communicated and the library will not be available
          anymore.

          \param id An identifier used to search for the library in successive lookups.
          \param l  The library to be managed.
         */
        void insert(const std::string& id, const std::shared_ptr<Library>& l);

        //! Return a null pointer if a library doesnt't exist.
        /*!
          \param name The library name.

          \return A pointer to an already loaded library or null.
        */
        std::shared_ptr<Library> get(const std::string& id);

        //! Return a reference to the singleton.
        static LibraryManager& instance();

      protected:

        /*! \brief Consructor. */
        LibraryManager();

        /*! \brief Destructor. */
        ~LibraryManager();

      private:

        struct Impl;

        Impl* m_pimpl;
    };

  }  // end namespace plugin
}    // end namespace te

#endif  // __TERRALIB_MODULE_LIBRARYMANAGER_H__
