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
  \file terralib/common/Library.h

  \brief A class for handling shared libraries.
 */

#ifndef __TERRALIB_COMMON_INTERNAL_LIBRARY_H
#define __TERRALIB_COMMON_INTERNAL_LIBRARY_H

// TerraLib
#include "Config.h"
#include "Exception.h"

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace common
  {
    /*!
      \class Library

      \brief A class for handling shared libraries (DLLs, SO, DyLibs).

      \note If the library you are trying to load has dependencies
            on other shared libraries then these are automatically
            loaded by the underlying operational system. The dependencies
            must be in a folder where the system can search for these dependencies.

      \note This class is based on pimpl idiom.

      \ingroup common
    */
    class TECOMMONEXPORT Library : public boost::noncopyable
    {
      public:

        /*!
          \brief Loads a new library specified by the file name.

          File name may be a relative or absolute path. If just the name of the
          library is given the class will look for the library in the operational system
          specific locations.

          \param fileName  The library file name. The file name may contain the full library path.
          \param delayLoad If true the client object must call explicitly the load method before trying to access any symbol in the library.

          \exception Exception It throws an exception if delayLoad is true and the library can not be loaded.
        */
        Library(const std::string& fileName, bool delayLoad = false) throw(Exception);

        /*! \brief The destructor will automatically unload the library from memory. */
        ~Library();

        /*!
          \brief It loads the shared library to memory.

          If the library was already loaded this method doesn't perform operations.

          \exception Exception If the library can not be load to memory this method raises an exception.

          \note Not thread-safe.
        */
        void load() throw(Exception);

        /*!
          \brief It forces the unload of the shared library from memory.

          If the library wasn't previously loaded or if it was already unloaded this method doesn't perform operations.

          \exception Exception If the library can not be unload from memory this method raises an exception.

          \note Not thread-safe.
        */
        void unload() throw(Exception);

        /*!
          \brief It returns true if the shared library is loaded otherwise return false.

          \return True if the shared library is loaded otherwise returns false.

          \note Thread-safe.
        */
        bool isLoaded() const throw();

        /*!
          \brief It returns the library file name as informed in the constructor.

          \return The library file name as informed in the constructor.

          \note Thread-safe.
        */
        const std::string& getFileName() const throw();

        /*!
          \brief It returns the address where the given symbol is loaded into memory.

          \param symbol The name of the symbol inside the library you are searching for. It may be for instance a function name.

          \return The address where the symbol is loaded.

          \exception Exception It throws an exception if it is not possible to locate the given symbol.

          \note Thread-safe.
        */
        void* getAddress(const std::string& symbol) const throw(Exception);
       
        /*!
          \brief Given a library name without file extensions, prefixes and nor suffixes it will construct a library name according to the specifc platform.

          \return The library file name according to the specifc platform.

          \note Thread-safe.
        */
        static std::string getNativeName(const std::string& name) throw();

        /*!
          \brief It adds the informed dir to the path used by the operational system to lookup for shared libraries.

          \param d A directory to be added to the lookup of shared libraries.

          \exception Exception It throws an exception if the path couldn't be added to the search path.

          \note Not thread-safe.
        */
        static void addSearchDir(const std::string& d) throw(Exception);

        /*!
          \brief It comes back the application lookupo path to the original state, before any addPath has been called.

          \exception Exception It throws an exception if the search path couldn't be reset.

          \note Not thread-safe.
        */
        static void resetSearchPath() throw(Exception);

        /*!
          \brief It returns the system lookup path.

          \return The system lookup path.

          \exception Exception It throws an exception if the search path couldn't be reset.

          \todo This method needs a checkup! Instead of
                throwing an exception in windows implementation,
                this methods should take an interative
                loop to allocate more room for the path string!

          \note Thread-safe.
        */
        static std::string getSearchPath() throw(Exception);

      private:

        class Impl;

        Impl* m_pImpl;  //!< A pointer to the real implementation.
    };

    typedef boost::shared_ptr<Library> LibraryPtr;

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_LIBRARY_H

