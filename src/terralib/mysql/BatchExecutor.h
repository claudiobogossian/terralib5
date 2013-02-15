/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mysql/BatchExecutor.h

  \brief Implementation of BatchExecutor for MySQL data access driver.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_BATCHEXECUTOR_H
#define __TERRALIB_MYSQL_INTERNAL_BATCHEXECUTOR_H

// TerraLib
#include "../dataaccess/datasource/BatchExecutor.h"
#include "Config.h"

// STL
#include <vector>

namespace te
{
  namespace mysql
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \class BatchExecutor

      \brief Implementation of BatchExecutor for MySQL data access driver.

      \sa te::da::BatchExecutor
    */
    class TEMYSQLEXPORT BatchExecutor : public te::da::BatchExecutor
    {
      public:

        /*! \brief Constructor. */
        BatchExecutor(DataSourceTransactor* t);

        /*! \brief Destructor. */
        ~BatchExecutor();

        void add(te::da::Query* q);

        void execute();

        void execute(const std::string& uri, const std::map<std::string, std::string>& options);

        void execute(std::istream& istr, const std::map<std::string, std::string>& options);

        te::da::DataSourceTransactor* getTransactor() const;

      protected:

        /*!
          \note MySQL driver extended method.
        */
        void prepareServer(const std::map<std::string, std::string>& options,
                           const std::string& tblName,
                           bool& turnOnFkCheckAtEnd,
                           bool& enableIndexesAtEnd,
                           bool& turnOnUniqueChecksAtEnd,
                           std::string& restoreBulkBufferSizeAtEnd,
                           std::string& restoreKeyBufferSizeAtEnd);

        /*!
          \note MySQL driver extended method.
        */
        void restoreServer(const std::string& tblName,
                           bool& turnOnFkCheckAtEnd,
                           bool& enableIndexesAtEnd,
                           bool& turnOnUniqueChecksAtEnd,
                           std::string& restoreBulkBufferSizeAtEnd,
                           std::string& restoreKeyBufferSizeAtEnd);

        /*!
          \note MySQL driver extended method.
        */
        void execSingle(std::istream& istr);

        /*!
          \note MySQL driver extended method.
        */
        void execMultiple(std::istream& istr, std::size_t ncommands);

        /*!
          \note MySQL driver extended method.
        */
        void execLoadData(const std::string& uri, const std::map<std::string, std::string>& options);

        /*!
          \note MySQL driver extended method.
        */
        void execLote(std::istream& istr, const std::map<std::string, std::string>& options);

        /*!
          \note MySQL driver extended method.

          \todo Melhorar o parser.
        */
        void execMultiLineCommand(std::istream& istr, const std::map<std::string, std::string>& options);

      private:

        DataSourceTransactor* m_t;            //!< The associated transactor.

        std::vector<std::string*> m_commands; //!< Vector of comands.
    };

  } // end namespace mysql
}   // end namespace te

#endif  // __TERRALIB_MYSQL_INTERNAL_BATCHEXECUTOR_H

