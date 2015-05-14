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
  \file terralib/dataaccess/datasource/DataSourceFactory.h

  \brief A factory for data sources.
*/

#ifndef __TERRALIB_DATAACCESS_DATASOURCE_INTERNAL_DATASOURCEFACTORY_H
#define __TERRALIB_DATAACCESS_DATASOURCE_INTERNAL_DATASOURCEFACTORY_H

// TerraLib
#include "../../common/Static.h"
#include "../Config.h"
#include "../Exception.h"

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
    class DataSource;

    /*!
      \class DataSourceFactory

      \brief A factory for data sources.

      \ingroup dataaccess

      \sa DataSource
    */
    class TEDATAACCESSEXPORT DataSourceFactory : public te::common::Static
    {
      public:

        typedef boost::function<DataSource* ()> FactoryFnctType;

        static std::auto_ptr<DataSource> make(const std::string& dsType);

        static void add(const std::string& dsType, FactoryFnctType f);

        static void remove(const std::string& dsType);
      
        static bool find(const std::string& dsType);

      private:

        static std::map<std::string, FactoryFnctType> sm_factories;
    };

  }   // end namespace da
}     // end namespace te

#endif  // __TERRALIB_DATAACCESS_DATASOURCE_INTERNAL_DATASOURCEFACTORY_H
