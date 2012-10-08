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
  \file terralib/qt/widgets/datasource/connector/core/DataSourceManager.h

  \brief A singleton to keep all the registered data sources.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_DATASOURCEMANAGER_H
#define __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_DATASOURCEMANAGER_H

// TerraLib
#include "../../../../common/Singleton.h"
#include "DataSource.h"

// STL
#include <map>
#include <string>
#include <vector>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DataSourceManager

        \brief A singleton to keep all the registered data sources.

        \todo Derivar DataSourceManager e DataSource a partir de QObject e emitir sinais como no caso do antigo DataSourceInfo!
      */
      class TEQTWIDGETSEXPORT DataSourceManager : public te::common::Singleton<DataSourceManager>
      {
        friend class te::common::Singleton<DataSourceManager>;

        public:

          typedef std::map<std::string, DataSourcePtr>::const_iterator const_iterator;
          typedef std::map<std::string, DataSourcePtr>::iterator iterator;

          DataSourcePtr get(const std::string& id) const;

          void add(const DataSourcePtr& ds);

          void remove(const std::string& id);

          void getByType(const std::string& dsTypeName, std::vector<DataSourcePtr>& datasources) const;

          std::size_t size() const;

          const_iterator begin() const;

          iterator begin();

          const_iterator end() const;

          iterator end();

        protected:

          DataSourceManager();

          ~DataSourceManager();

        private:

          std::map<std::string, DataSourcePtr> m_datasources;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_DATASOURCEMANAGER_H
