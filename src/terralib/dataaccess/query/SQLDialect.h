/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/query/SQLDialect.h

  \brief It represents the SQL query dialect accepted by a given data source.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_SQLDIALECT_H
#define __TERRALIB_DATAACCESS_INTERNAL_SQLDIALECT_H

// TerraLib
#include "../Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace da
  {
// Forward declarations
    class SQLFunctionEncoder;

    /*!
      \class SQLDialect
      
      \brief It represents the SQL query dialect accepted by a given data source.

      This class is target for the portability of TerraLib...

      \ingroup dataaccess
    */
    class TEDATAACCESSEXPORT SQLDialect : public boost::noncopyable
    {
      public:

        /*! \brief Default constructor. */
        SQLDialect() {}

        /*! \brief Destructor. */
        ~SQLDialect();

        /*!
          \brief It adds a new encoder for a given function.

          \param funcName The function name.
          \param encoder  The SQL dialect encoder for the given function.

          \exception Exception It thorws an exception if a given function is already registered.

          \note The SQLDialect will take the ownership of the given encoder.
        */
        void insert(const std::string& funcName,
                    SQLFunctionEncoder* encoder);

        /*!
          \brief It searches for an encoder for the given function.

          \param funcName The function name.

          \return An encoder or NULL if none is found.
        */
        const SQLFunctionEncoder* find(const std::string& funcName) const;

        /*!
          \brief It gets the all registered names from  registed functions.

          \return A vector with TerraLib general name for SQL functions.
        */
        std::vector<std::string> getRegisteredNames() const;

      private:

        std::map<std::string, SQLFunctionEncoder*> m_regFunctions; //!< A map from the TerraLib general name to a given SQL formatter.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_SQLDIALECT_H

