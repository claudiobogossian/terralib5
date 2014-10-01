/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/stmemory/DataSource.h

  \brief Implements a DataSource that contains st memory DataSets indexed by space and time. 
*/

#ifndef __TERRALIB_STMEMORY_INTERNAL_DATASOURCE_H
#define __TERRALIB_STMEMORY_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/query/SQLDialect.h"
#include "Config.h"

// STL
#include <map>
#include <vector>

namespace te { namespace da { class DataSetType; } }   
namespace te { namespace dt { class DateTimePeriod; } }  

namespace te
{
  namespace stmem
  {
// Forward declaration
    class DataSet;
    class Transactor;

    /*!
      \class DataSource

      \brief Implements a DataSource that contains In-Memory DataSets indexed by space and time. 

      \sa te::da::DataSource
    */
    class TESTMEMORYEXPORT DataSource : public te::da::DataSource
    {
      friend class Transactor;

      public:

        /*! \brief Constructor */
        DataSource();

        /*! \brief Destructor */
        ~DataSource();

        /*! \name DataSource inherited methods */
        //@{
        std::string getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        std::auto_ptr<te::da::DataSourceTransactor> getTransactor();

        void open();

        void close();

        bool isOpened() const;

        bool isValid() const;

        const te::da::DataSourceCapabilities& getCapabilities() const;

        const te::da::SQLDialect* getDialect() const;
        //@}

        /*!
          \brief It adds a new DataSet and DataSetType into the DataSource.
              
          \param name The DataSet name.
          \param t    The DataSet Type.
          \param d    The DataSet.

          \note It will take the ownership of the given pointers.
          \note It can throw an Exception if there is already a DataSet with the given name
        */
        void add(const std::string& name, te::da::DataSetType* t, DataSet* d);

        /*!
          \brief It returns a data set with observations whose phenomenon times satisfy a
                 given temporal relation. 

          The possible temporal relations are: 1. AFTER; 
          2. AFTER | EQUALS; 3. BEFORE;  4. BEFORE | EQUALS; 5. DURING; 6. EQUALS 

          \param name The DataSet name.
          \param  dt  A given datetime.
          \param  r   A given temporal relation.
          \param travType The traverse type associated to the returned dataset. 
          \param connected If the returned DataSet is connecet or not.
          
          \return A pointer to a new DataSet that shares the intenal content.

          \note The caller will take the ownership of the returned pointer.
          \note When the temporal relation is DURING, dt must be a time period.
          \note It can throw an Exception when internal errors occur.
        */
         std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                   const te::dt::DateTime* dt, te::dt::TemporalRelation r = te::dt::DURING,
                                                   te::common::TraverseType travType = te::common::FORWARDONLY, 
												                           bool connected = false,
                                                   const te::common::AccessPolicy accessPolicy = te::common::RAccess);

         /*!
          \brief It returns a data set with observations whose observed geometries satisfy a
                 given spatial relation and phenomenon times satisfy a given temporal relation. 

          The possible spatial relations are: INTERSECTS
          The possible temporal relations are: 1. AFTER; 
          2. AFTER | EQUALS; 3. BEFORE;  4. BEFORE | EQUALS; 5. DURING; 6. EQUALS

          \param name         The DataSet name.
          \param geom         A given geometry.
          \param sr           A given spatial relation.
          \param dt           A given datetime.
          \param tr           A given temporal relation.
          \param travType     The traverse type associated to the returned dataset. 
          \param connected    If the returned DataSet is connecet or not.
          
          \return A pointer to a new DataSet that shares the intenal content.

          \note The caller will take the ownership of the returned pointer.
          \note It will NOT take the ownership of the given pointers.
          \note When the temporal relation is DURING, dt must be a time period.
          \note It can throw an Exception when internal errors occur.
        */
        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                  const te::gm::Geometry* geom, te::gm::SpatialRelation sr,
                                                  const te::dt::DateTime* dt, te::dt::TemporalRelation tr = te::dt::DURING,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY, 
												                          bool connected = false,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess);

        /*!
          \brief It returns a data set with observations whose observed geometries satisfy a
                 given spatial relation and phenomenon times satisfy a given temporal relation. 

          The possible spatial relations are: INTERSECTS
          The possible temporal relations are: 1. AFTER; 
          2. AFTER | EQUALS; 3. BEFORE;  4. BEFORE | EQUALS; 5. DURING; 6. EQUALS

          \param name         The DataSet name.
          \param e            A given envelope.
          \param sr           A given spatial relation.
          \param dt           A given datetime.
          \param tr           A given temporal relation.
          \param travType     The traverse type associated to the returned dataset. 
          \param connected    If the returned DataSet is connecet or not.
          
          \return A pointer to a new DataSet that shares the intenal content.

          \note The caller will take the ownership of the returned pointer.
          \note It will NOT take the ownership of the given pointers.
          \note When the temporal relation is DURING, dt must be a time period.
          \note It can throw an Exception when internal errors occur.
        */
        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                  const te::gm::Envelope* e, te::gm::SpatialRelation sr,
                                                  const te::dt::DateTime* dt, te::dt::TemporalRelation tr = te::dt::DURING,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY, 
												                          bool connected = false,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess);

        /*!
          \brief It returns the temporal extent associated to a DataSet.

          \param name         The DataSet name.
          
          \return A pointer to its temporal extent.

          \note The caller will take the ownership of the returned pointer.          
          \note It can throw an Exception when internal errors occur.
        */
        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent(const std::string& name);

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& info);

        std::vector<te::common::CharEncoding> getEncodings(const std::map<std::string, std::string>& dsInfo);

      private:

        std::map<std::string, std::string>                          m_connInfo;    //!< DataSource information.
        std::map<std::string, DataSet* >                            m_datasets;    //!< The set of datasets stored in memory.
        std::map<std::string, te::da::DataSetType* >                m_schemas;     //!< The set of dataset schemas.
        std::size_t                                                 m_maxdatasets; //!< The maximum number of datasets to be handled by the data source.
        bool                                                        m_isOpened;    //!< A flag to control the state of the data source.
        bool                                                        m_deepCopy;    //!< If true each dataset is cloned in the getDataSet method.
        std::auto_ptr<Transactor>                                   m_transactor;  //!< A transactor 

        static te::da::DataSourceCapabilities        sm_capabilities;     //!< The Memory data source capabilities.
        static const te::da::SQLDialect              sm_dialect;          //!< A dummy dialect.
    };

  } // end namespace stmem
}   // end namespace te

#endif  // __TERRALIB_STMEMORY_INTERNAL_DATASOURCE_H

