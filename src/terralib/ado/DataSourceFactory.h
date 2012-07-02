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
  \file terralib/ado/DataSourceFactory.h

  \brief This is the concrete factory for the TerraLib ADO Data Access driver.
*/

#ifndef __TERRALIB_ADO_INTERNAL_DATASOURCEFACTORY_H
#define __TERRALIB_ADO_INTERNAL_DATASOURCEFACTORY_H

// TerraLib
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "Config.h"

namespace te
{
  namespace ado
  {
    /*!
      \class DataSourceFactory

      \brief This is the concrete factory for ADO.

      \sa te::da::DataSourceFactory
    */
    class TEADOEXPORT DataSourceFactory : public te::da::DataSourceFactory
    {
      public:

        const std::string& getType() const;

        void getConnectionParameters(std::vector<std::string>& params) const;

        ~DataSourceFactory();

        /*! \brief It initializes the factory: it will be registered in the abstract factory DataSourceFactory. */
        static void initialize();

        /*! \brief It finalizes the factory: it will be destroyed and will be unregistered from the abstract factory DataSourceFactory. */
        static void finalize();

      protected:        

        te::da::DataSource* build();

        DataSourceFactory();

      private:

        static DataSourceFactory* sm_factory; //!< A pointer to the global ADO data source factory.

    };

  } // end namespace ado
}   // end namespace te

#endif  // __TERRALIB_ADO_INTERNAL_DATASOURCEFACTORY_H

