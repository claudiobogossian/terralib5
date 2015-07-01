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
  \file terralib/qt/widgets/datasource/connector/core/DataSourceTypeManager.h

  \brief A singleton to keep all the registered data source types.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_DATASOURCETYPEMANAGER_H
#define __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_DATASOURCETYPEMANAGER_H

// TerraLib
#include "../../../../common/Singleton.h"
#include "../../Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/ptr_container/ptr_map.hpp>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DataSourceType;

      /*!
        \class DataSourceTypeManager

        \brief A singleton to keep all the registered data source types.

        \ingroup widgets

        \todo Derivar DataSourceTypeManager e DataSourceType a partir de QObject e emitir sinais!
      */
      class TEQTWIDGETSEXPORT DataSourceTypeManager : public te::common::Singleton<DataSourceTypeManager>
      {
        friend class te::common::Singleton<DataSourceTypeManager>;

        public:

          typedef std::map<std::string, DataSourceType*>::const_iterator const_iterator;
          typedef std::map<std::string, DataSourceType*>::iterator iterator;

          const DataSourceType* get(const std::string& name) const;

          void add(DataSourceType* dst);

          void remove(const std::string& name);

          std::size_t size() const;

          const_iterator begin() const;

          iterator begin();

          const_iterator end() const;

          iterator end();

        protected:

          DataSourceTypeManager();

          ~DataSourceTypeManager();

        private:

          std::map<std::string, DataSourceType*> m_datasourceTypeMap;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASOURCE_CONNECTOR_CORE_INTERNAL_DATASOURCETYPEMANAGER_H
