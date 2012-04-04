/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file DataSourceFactory.h

  \brief This is the concrete implementation of a factory that builds data sources managed by the GDAL Library.
*/

#ifndef __TERRALIB_GDAL_INTERNAL_DATASOURCEFACTORY_H
#define __TERRALIB_GDAL_INTERNAL_DATASOURCEFACTORY_H

// TerraLib
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "Config.h"

namespace te
{
  namespace gdal
  {
    /*!
      \class DataSourceFactory

      \brief This is the concrete implementation of a factory that builds data sources managed by the GDAL Library.

      \sa te::da::DataSourceFactory, te::da::DataSource
     */
    class TEGDALEXPORT DataSourceFactory : public te::da::DataSourceFactory
    {
      public:

        /*!
          \brief Destructor.

          \note It will automatically unregister the factory from the dictionary.
        */
        ~DataSourceFactory();

        /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory DataSourceFactory. */
        static void initialize();

        /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory DataSourceFactory. */
        static void finalize();

        const std::string& getType() const;

        void getConnectionParameters(std::vector<std::string>& params) const;

      protected: 
      
        DataSourceFactory();

        te::da::DataSource* build();
    
      private:

        static DataSourceFactory* sm_factory; //!< A pointer to the global GDAL datasource factory.
    };

  } // end namespace gdal
}   // end namespace te

#endif  // __TERRALIB_GDAL_INTERNAL_DATASOURCEFACTORY_H


