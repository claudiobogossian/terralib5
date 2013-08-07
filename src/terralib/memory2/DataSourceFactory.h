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
  \file terralib/memory/DataSourceFactory.h

  \brief This is the data source concrete factory for the In-Memory Data Access driver.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_DATASOURCEFACTORY_H
#define __TERRALIB_MEMORY_INTERNAL_DATASOURCEFACTORY_H

// TerraLib
#include "../dataaccess2/datasource/DataSourceFactory.h"
#include "Config.h"

namespace te
{
  namespace mem
  {
    /*!
      \class DataSourceFactory

      \brief This is the data source concrete factory for the In-Memory Data Access driver.

      \sa te::da::DataSourceFactory
    */
    class TEMEMORYEXPORT DataSourceFactory : public te::da::DataSourceFactory
    {
      public:

        ~DataSourceFactory();

        const std::string& getType() const;

        void getConnectionParameters(std::vector<std::string>& params) const;

      protected:        

        DataSourceFactory();

        te::da::DataSource* build();

      private:

        static DataSourceFactory sm_factory; //!< The global In-Memory data source factory.
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_DATASOURCEFACTORY_H
