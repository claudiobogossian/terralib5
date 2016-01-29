/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
\file src/terraMobilePlugin/core/GeoPackageSynchronizer.h

\brief This file is used to Synchronizer operation.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGESYNCHRONIZER_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGESYNCHRONIZER_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"

// STL
#include <memory>


namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terramobile
      {
        /*!
        \class GeoPackageSynchronizer

        \brief This file is used to Synchronizer operation.
        */
        class GeoPackageSynchronizer
        {

        public:

          GeoPackageSynchronizer();

          ~GeoPackageSynchronizer();

        public:

          void setInputParameters(te::da::DataSource* inputDataSource, std::string inputDataSet, te::da::DataSource* outputDataSource, std::string outputDataset);

          void synchronize();

        protected:

          void checkParameters();

        protected:

          te::da::DataSource* m_inputDataSource;
          std::string m_inputDataSet;

          te::da::DataSource* m_outputDataSource;
          std::string m_outputDataset;

        };
      }   // end namespace thirdParty
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_GEOPACKAGESYNCHRONIZER_H

