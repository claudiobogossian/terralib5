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
  \file terralib/mysql/DataSetTypePersistence.h

  \brief DataSetTypePersistence implementation for MySQL data access driver.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_DATASETTYPEPERSISTENCE_H
#define __TERRALIB_MYSQL_INTERNAL_DATASETTYPEPERSISTENCE_H

// TerraLib
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "Config.h"

namespace te
{
  namespace mysql
  {
// Forward declaration
    class DataSourceTransactor;    
    
    /*!
      \class DataSetTypePersistence

      \brief An abstract class responsible for persisting a dataset type definition in a data source.

      \sa te::da::DataSetTypePersistence
    */
    class TEMYSQLEXPORT DataSetTypePersistence : public te::da::DataSetTypePersistence
    {
      public:

        DataSetTypePersistence(DataSourceTransactor* parent);

        ~DataSetTypePersistence();

        void create(te::da::DataSetType* dt, const std::map<std::string, std::string>& options);

        void clone(const std::string& datasetName, const std::string& newDatasetName, const std::map<std::string, std::string>& options);

        void drop(te::da::DataSetType* dt);

        void drop(const std::string& datasetName);

        void rename(te::da::DataSetType* dt, const std::string& newName);
   
        void add(te::da::DataSetType* dt, te::dt::Property* p);

        void drop(te::dt::Property* p);

        void rename(te::dt::Property* p, const std::string& newName); 

        void update(te::dt::Property* oldP, te::dt::Property* newP);
        
        void add(te::da::DataSetType* dt, te::da::PrimaryKey* pk); 
        
        void drop(te::da::PrimaryKey* pk);
        
        void add(te::da::DataSetType* dt, te::da::UniqueKey* uk); 
        
        void drop(te::da::UniqueKey* uk);
        
        void add(te::da::DataSetType* dt, te::da::Index* index, const std::map<std::string, std::string>& options); 
        
        void drop(te::da::Index* index);

        void add(te::da::DataSetType* dt, te::da::ForeignKey* fk);

        void drop(te::da::ForeignKey* fk);

        void add(te::da::DataSetType* dt, te::da::CheckConstraint* cc);
   
        void drop(te::da::CheckConstraint* cc);
   
        void create(te::da::Sequence* sequence);
   
        void drop(te::da::Sequence* sequence);

        te::da::DataSourceTransactor* getTransactor() const;

      protected:

        /*!
          \note MySQL driver extended method.
        */
        void checkNotNull(te::da::DataSetType* dt, te::da::Index* index);

        /*!
          \note MySQL driver extended method.
        */
        void add(te::da::DataSetType* dt, te::da::PrimaryKey* pk, const bool refresh); 

        /*!
          \note MySQL driver extended method.
        */
        void add(te::da::DataSetType* dt, te::da::UniqueKey* uk, const bool refresh); 

        /*!
          \note MySQL driver extended method.
        */
        void add(te::da::DataSetType* dt, te::da::Index* index, const std::map<std::string, std::string>& options, const bool refresh); 

        /*!
          \note MySQL driver extended method.
        */
        void createRaster(te::da::DataSetType* dt, const std::map<std::string, std::string>& options);

      private:

        DataSourceTransactor* m_t;  //!< The associated data source transactor.

    };

  } // end namespace mysql
}   // end namespace te

#endif  // __TERRALIB_MYSQL_INTERNAL_DATASETTYPEPERSISTENCE_H

