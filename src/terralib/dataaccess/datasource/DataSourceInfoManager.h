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
  \file terralib/dataaccess/datasource/DataSourceInfoManager.h

  \brief A singleton to keep all the registered data sources.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASOURCEINFOMANAGER_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASOURCEINFOMANAGER_H

// TerraLib
#include "../../common/Singleton.h"
#include "DataSourceInfo.h"

// STL
#include <map>
#include <string>
#include <vector>

namespace te
{
  namespace da
  {
      /*!
        \class DataSourceInfoManager

        \brief A singleton to keep all the registered data sources.

        \ingroup dataaccess
      */
      class TEDATAACCESSEXPORT DataSourceInfoManager : public te::common::Singleton<DataSourceInfoManager>
      {
        friend class te::common::Singleton<DataSourceInfoManager>;

        public:

          typedef std::map<std::string, DataSourceInfoPtr>::const_iterator const_iterator;
          typedef std::map<std::string, DataSourceInfoPtr>::iterator iterator;

          DataSourceInfoPtr get(const std::string& id) const;

          bool add(const DataSourceInfoPtr& ds);

          void remove(const std::string& id);

          void removeByType(const std::string& dsTypeName);

          void getByType(const std::string& dsTypeName, std::vector<DataSourceInfoPtr>& datasources) const;

          DataSourceInfoPtr getByConnInfo(std::string connInfo);

          std::size_t size() const;

          const_iterator begin() const;

          iterator begin();

          const_iterator end() const;

          iterator end();

        protected:

          DataSourceInfoManager();

          ~DataSourceInfoManager();

        private:

          std::map<std::string, DataSourceInfoPtr> m_datasources;
      }; 

  }   // end namespace da
}     // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCEINFOMANAGER_H
