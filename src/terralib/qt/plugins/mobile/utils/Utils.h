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
  \file terralib/qt/plugins/mobile/Utils.h

  \brief This file contains a set of utility functions used to export data to a geopackage file
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_UTILS_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_UTILS_H

//TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../Config.h"

namespace te
{
  //Forward declaration
  namespace da { class DataSource; }
  namespace qt
  {
    namespace plugins
    {
      namespace terramobile
      {
        std::auto_ptr<te::da::DataSource> createGeopackage(std::string gpkgName);

        void exportToGPKG(te::map::AbstractLayerPtr layer, te::da::DataSource* dsGPKG, std::string outFileName, const te::gm::Envelope extent);

        void queryGPKG(std::string query, te::da::DataSource* dsGPKG);
      }   // end namespace terramobile
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te

#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_UTILS_H