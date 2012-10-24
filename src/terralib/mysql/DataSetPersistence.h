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
  \file terralib/mysql/DataSetPersistence.h

  \brief DataSetPersistence implementation for MySQL data access driver.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_DATASETPERSISTENCE_H
#define __TERRALIB_MYSQL_INTERNAL_DATASETPERSISTENCE_H

// TerraLib
#include "../dataaccess/dataset/DataSetPersistence.h"
#include "Config.h"

namespace te
{
  namespace mysql
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \brief DataSetPersistence implementation for MySQL data access driver.

      \sa te::da::DataSetPersistence
    */
    class TEMYSQLEXPORT DataSetPersistence : public te::da::DataSetPersistence
    {
      public:

        DataSetPersistence(DataSourceTransactor* parent);

        ~DataSetPersistence();

        void create(te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& options, std::size_t limit = 0);

        void remove(const te::da::DataSetType* dt);

        void remove(const std::string& datasetName);
       
        void remove(const te::da::DataSetType* dt, te::da::DataSet* d, std::size_t limit = 0);

        void remove(const te::da::DataSetType* dt, te::da::DataSetItem* item);

        void add(const te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& options, std::size_t limit = 0);

        void add(const te::da::DataSetType* dt, te::da::DataSetItem* item);

        void update(const te::da::DataSetType* dt,
                    te::da::DataSet* dataset,
                    const std::vector<te::dt::Property*>& properties,
                    const std::map<std::string, std::string>& options,
                    std::size_t limit = 0);

        void update(const te::da::DataSetType* dt,
                    te::da::DataSet* oldD,
                    te::da::DataSet* newD,
                    const std::vector<te::dt::Property*>& properties,
                    std::size_t limit = 0);

        void update(const te::da::DataSetType* dt,
                    te::da::DataSetItem* item,
                    const std::vector<te::dt::Property*>& properties);

        void update(const te::da::DataSetType* dt,
                    te::da::DataSetItem* oldItem,
                    te::da::DataSetItem* newItem,
                    const std::vector<te::dt::Property*>& properties);

        te::da::DataSourceTransactor* getTransactor() const;

      private:

        // insere 1-linha nao preparada por item
        void add1(const te::da::DataSetType* dt, te::da::DataSet* d);

        // usa transacao + insere 1-linha nao preparada por item
        void add2(const te::da::DataSetType* dt, te::da::DataSet* d);

        // insere 1-linha preparada por item
        void add3(const te::da::DataSetType* dt, te::da::DataSet* d);

        // usa transacao + insere 1-linha preparada por item
        void add4(const te::da::DataSetType* dt, te::da::DataSet* d);

        // insere usando multiplas linhas
        void add5(const te::da::DataSetType* dt, te::da::DataSet* d);

        // usa transacao + insere usando multiplas linhas
        void add6(const te::da::DataSetType* dt, te::da::DataSet* d);

        // insercao multipla em uma linha
        void add7(const te::da::DataSetType* dt, te::da::DataSet* d);

        // usa transacao + insercao multipla em uma linha
        void add8(const te::da::DataSetType* dt, te::da::DataSet* d);

        // gera um arquivo e usa LOAD do cliente p/ servidor
        void add9(const te::da::DataSetType* dt, te::da::DataSet* d);

        // gera um arquivo e usa LOAD no proprio servidor
        void add10(const te::da::DataSetType* dt, te::da::DataSet* d);

        void update1(const te::da::DataSetType* dt,
                     te::da::DataSet* dataset,
                     const std::vector<te::dt::Property*>& properties);

        void update2(const te::da::DataSetType* dt,
                     te::da::DataSet* dataset,
                     const std::vector<te::dt::Property*>& properties);

      private:

        DataSourceTransactor* m_t;
    };

  } // end namespace mysql
}   // end namespace te

#endif  // __TERRALIB_MYSQL_INTERNAL_DATASETPERSISTENCE_H

