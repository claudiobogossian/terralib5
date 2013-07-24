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
  \file terralib/dataaccess/datasource/DataSourceFactory.h

  \brief This is the abstract factory for DataSource.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASOURCEFACTORY_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASOURCEFACTORY_H

// TerraLib
#include "../../common/AbstractFactory.h"
#include "DataSource.h"

namespace te
{
  namespace da
  {   

    /*!
      \class DataSourceFactory

      \brief This is the abstract factory for DataSource.

      This is an abstract factory, so you will need to call the static method make
      as follow:

      \code
      DataSource* ds = DataSourceFactory::make("POSTGIS");

      try
      {
        std::string dsInfo("host=localhost&port=5432&dbname=bdgeo&user=gribeiro&password=pppppp&connect_timeout=4");
        
        ds->open(dsInfo);

        ...
      }
      catch(const te::da::Exception& e)
      {
        ...
      }

      \endcode

      or

      \code
      DataSource* ds = DataSourceFactory::make("POSTGIS");

      try
      {
        std::map<std::string, std::string> dsInfo;      
        dsInfo["user"] = "postgres";
        dsInfo["password"] = "mypasswd";
        dsInfo["dbname"] = "citiesdb";
        dsInfo["port"] = "5432";
        
        ds->open(dsInfo);

        ...
      }
      catch(const te::da::Exception& e)
      {
        ...
      }
      \endcode

      \sa DataSource
    */
    class TEDATAACCESSEXPORT DataSourceFactory : public te::common::AbstractFactory<te::da::DataSource, std::string>
    {
      public:

        static te::da::DataSource* make(const std::string& dsType);

        static te::da::DataSource* open(const std::string& dsType, const std::map<std::string, std::string>& dsInfo);

        /*!
          \brief Virtual destructor.

          \note It will automatically unregister the factory from the dictionary.
        */
        virtual ~DataSourceFactory() {}

        /*!
          \brief It returns the type of data source that the factory knows how to create.

          \return The type of data source that the factory knows how to create.
        */
        virtual const std::string& getType() const = 0;

        /*!
          \brief It returns the list of data source connection parameters.

          \param params A vector to output the list of data source connection parameters.
        */
        virtual void getConnectionParameters(std::vector<std::string>& params) const = 0;

      protected:

        /*!
          \brief It creates the factory.

          The key of a DataSourceFactory is a string.

          \param factoryKey The key that identifies the factory.
        */
        DataSourceFactory(const std::string& factoryKey);
    };    

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCEFACTORY_H
