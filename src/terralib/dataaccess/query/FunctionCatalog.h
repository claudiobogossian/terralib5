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
  \file terralib/dataaccess/query/FunctionCatalog.h

  \brief A FunctionCatalog can be used to keep track of registered functions.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONCATALOG_H
#define __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONCATALOG_H

// TerraLib
#include "../Config.h"

// STL
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace da
  {
// Forward declarations
    class FunctionDefn;

    /*!
      \class FunctionCatalog
      
      \brief A FunctionCatalog can be used to keep track of registered functions.

      \sa FunctionDefn, FunctionCatalogManager
    */
    class TEDATAACCESSEXPORT FunctionCatalog : public boost::noncopyable
    {
      public:

        /*!
          \brief Constructor.

          \param name The parameter name.
          \param t    The parameter type.
        */
        FunctionCatalog() {}

        /*! \brief Destructor. */
        ~FunctionCatalog();

        /*!
          \brief It inserts the function into the catalog.

          \param fdfn The function definition. The catalog will take the ownership of the given pointer.
        */
        void insert(FunctionDefn* fdfn);

        /*!
          \brief It finds the function with the given name.

          \param functionName The function name you are looking information for.

          \return The list of functions with the given name. Remember, a function name may be overloaded. If none is found, return NULL.

          \note Don't delete the returned pointer, it belongs to the catalog!
        */
        std::vector<FunctionDefn*>* find(const std::string& functionName) const;

      private:

        std::map<std::string, std::vector<FunctionDefn*>* > m_fMap;  //!< Function map: function-name -> std::vector<FunctionDef*>*.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONCATALOG_H


