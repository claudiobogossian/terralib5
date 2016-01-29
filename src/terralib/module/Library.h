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
  \file terralib/module/Library.h

  \brief A class for handling shared libraries.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TERRALIB_MODULE_LIBRARY_H__
#define __TERRALIB_MODULE_LIBRARY_H__

// TerraLib
#include "Config.h"

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace module
  {

    /*!
      \class Library

      \brief A class for handling shared libraries (DLLs, SO, DyLibs).

      You can use this class to load any shared library and acces any address
      inside it.

      If the shared library you are attempting to load has dependencies
      the other shared libraries must be in a folder reachable by the operational
      system.

      On Mac OS X the environment variables PATH, DYLD_LIBRARY_PATH,
      DYLD_FALLBACK_LIBRARY_PATH and DYLD_FRAMEWORK_FALLBACK_PATH may be set to
      indicate the list of folders containing shared libraries.

      On Linux the environment variables PATH and LD_LIBRARY_PATH may be used
      to indicate the folders searchable by the operational system.

      On Windows you can set the environment variable PATH.

      Note that this class has a special static method (or class method)
      named addSearchDir that can be used to control the behaviour of
      operational system when loading the shared library dependencies.

      \warning Shared libraries should not be loaded by multiple threads.
     */
    class TEMODULEEXPORT Library : public boost::noncopyable
    {
      public:

        //! Load a new shared library.
        /*!
          The file name may be a relative or absolute path.

          If just the name of the shared library is given the class will look for the library in the operational system
          specific locations.

          \param slib_file_name The library file name. The file name may contain the full library path.
          \param delay_load     If true the client object must call explicitly the load method before trying to access any symbol in the library.

          \exception LibraryLoadException It can throws an exception if delay_load is set to false and the library can not be loaded.
         */
        Library(const std::string& slib_file_name, const bool delay_load = false);

        //! The destructor automatically unloads from memory the shared library if it was not unloaded explicitly.
        ~Library();

        //! Load the shared library to memory.
        /*!
          If the shared library was already loaded this method doesn't perform operations.

          \exception library_load_error If the shared library can not be loaded to memory.

          \note Not thread-safe.
         */
        void load();

        //! Force the unload of the shared library from memory.
        /*!
          If the library wasn't previously loaded or if it was already unloaded
          this method doesn't perform operations.

          \exception library_unload_error If the library can not be
                     unload from memory this method raises an exception.

          \note Not thread-safe.
         */
        void unload();

        //! Return true if the shared library is loaded otherwise return false.
        bool isLoaded() const;

        //! Return the shared library file name as informed in the constructor.
        const std::string& getFileName() const;

        //! Return the address where the given symbol is loaded into memory.
        /*!
          \param symbol The name of the symbol inside the library you are searching for.
                        It may be for instance a function name.

          \return The address where the symbol is loaded.

          \exception library_symbol_not_found_error It throws an exception if it is not possible to locate the given symbol.

          \note Not thread-safe.
         */
        void* getAddress(const char* symbol) const;

        //! Given a shared library name without file extensions, prefixes and nor suffixes it will construct a library name according to the specifc platform.
        static std::string getNativeName(const std::string& name);

        //! Add the informed dir to the path used by the operational system to lookup for shared libraries.
        /*!
          \param dir_name A directory to be added to the lookup of shared libraries.

          \exception library_invalid_search_path_error It throws an exception if the path couldn't be added to the search path.

          \note Not thread-safe.
         */
        static void addSearchDir(const std::string& dir_name);

        //! Comes back the application lookup path to the original state, before any add_search_dir has been called.
        /*!
          \exception library_reset_search_path_error It throws an exception if the path couldn't be reset.

          \note Not thread-safe.
         */
        static void resetSearchPath();

        //! Returns the system lookup path.
        /*!
          \exception library_search_path_error It throws an exception if the path couldn't be reset.

          \note Not thread-safe.

          \todo This method needs a checkup! Instead of
                throwing an exception in windows implementation,
                this methods should take an interative
                loop to allocate more room for the path string!
         */
        static std::string getSearchPath();

      private:

        struct Impl;

        Impl* m_pimpl;
    };

  }  // end namespace module
}    // end namespace te

#endif  // __TERRALIB_MODULE_LIBRARY_H__
