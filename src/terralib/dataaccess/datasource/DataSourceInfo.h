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
  \file terralib/dataaccess/datasource/DataSourceInfo.h

  \brief A conteiner class for keeping information about a data source.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASOURCEINFO_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASOURCEINFO_H

// TerraLib
#include "../Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace da
  {
      /*!
        \class DataSourceInfo

        \brief A class that represents a data source component.

        This class splits the concept of data source from data access drivers.

        For example, at application level one can have a data source named File that uses
        diferent drivers to access data. It will depend on user preferences and dialogs used to create
        the data source.

        \ingroup dataaccess
      */
      class TEDATAACCESSEXPORT DataSourceInfo
      {
        public:

          DataSourceInfo();

          ~DataSourceInfo();

          const std::string& getId() const;

          void setId(const std::string& id);

          const std::string& getType() const;

          void setType(const std::string& dsType);

          const std::string& getAccessDriver() const;

          void setAccessDriver(const std::string& d);

          const std::string& getTitle() const;

          void setTitle(const std::string& title);

          const std::string& getDescription() const;

          void setDescription(const std::string& d);

          const std::map<std::string, std::string>& getConnInfo() const;

          std::map<std::string, std::string>& getConnInfo();

          std::string getConnInfoAsString();

          void setConnInfo(const std::map<std::string, std::string>& conninfo);

          bool operator<(const DataSourceInfo& rhs) const;

        private:

          std::string m_id;
          std::string m_dsType;
          std::string m_accessDriver;
          std::string m_title;
          std::string m_description;
          std::map<std::string, std::string> m_conninfo;
      }; 

      typedef boost::shared_ptr<DataSourceInfo> DataSourceInfoPtr;

  }// end namespace da
}  // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCEINFO_H
