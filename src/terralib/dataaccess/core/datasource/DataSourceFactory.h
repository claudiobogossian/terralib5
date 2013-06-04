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
  \file terralib/dataaccess/core/datasource/DataSourceFactory.h

  \brief A factory of data sources.
*/

#ifndef __TERRALIB_DATAACCESS_CORE_DATASOURCE_INTERNAL_DATASOURCEFACTORY_H
#define __TERRALIB_DATAACCESS_CORE_DATASOURCE_INTERNAL_DATASOURCEFACTORY_H

// TerraLib
#include "../../../common/Static.h"
#include "../../Config.h"
#include "../Exception2.h"

// STL
#include <map>
#include <memory>
#include <string>

// Boost
#include <boost/function.hpp>

namespace te
{
  namespace da
  {
    namespace core
    {
      class AbstractDataSource;

      /*!
        \class DataSourceFactory

        \brief A factory of data sources.

        \sa AbstractDataSource
      */
      class TEDATAACCESSEXPORT DataSourceFactory : public te::common::Static
      {
        public:

          typedef boost::function<AbstractDataSource* ()> FactoryFnctType;

          static std::auto_ptr<AbstractDataSource> make(const std::string& dsType) throw(Exception);

          static void add(const std::string& dsType, FactoryFnctType f) throw(Exception);

          static void remove(const std::string& dsType) throw(Exception);

        private:

          static std::map<std::string, FactoryFnctType> sm_factories;
      };

    } // end namespace core
  }   // end namespace da
}     // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_DATASOURCE_INTERNAL_DATASOURCEFACTORY_H

