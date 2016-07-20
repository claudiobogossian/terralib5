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
  \file terralib/sqlite/BatchExecutor.h

  \brief Implementation of the BatchExecutor class for the TerraLib SQLite Data Access driver.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_BATCHEXECUTOR_H
#define __TERRALIB_SQLITE_INTERNAL_BATCHEXECUTOR_H

// TerraLib
#include "../dataaccess/datasource/BatchExecutor.h"

// STL
#include <vector>

namespace te
{
  namespace sqlite
  {
    class DataSourceTransactor;

    class BatchExecutor : public te::da::BatchExecutor
    {
      public:

        BatchExecutor(DataSourceTransactor* t);

        ~BatchExecutor();

        void add(te::da::Query* q);

        void execute();

        void execute(const std::string& uri, const std::map<std::string, std::string>& options);

        void execute(std::istream& istr, const std::map<std::string, std::string>& options);

        te::da::DataSourceTransactor* getTransactor() const;

        void add(std::string* sql);

      private:

        DataSourceTransactor* m_t;

        std::vector<std::string*> m_commands;
    };

  } // end namespace sqlite
}   // end namespace te


#endif  // __TERRALIB_SQLITE_INTERNAL_BATCHEXECUTOR_H


