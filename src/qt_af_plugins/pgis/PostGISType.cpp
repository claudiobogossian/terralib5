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
  \file PostGISType.cpp

  \brief PostGIS data source type.
*/

//! TerraLib
#include <terralib/common/Translator.h>
//#include "../../../layer/selector/core/DataSetLayerSelector.h"
#include "PostGISConnector.h"
#include "PostGISType.h"

// STL
#include <cassert>

namespace te_qt = te::qt::widgets;
namespace plg_pgis = qt_af::plugin::pgis;

plg_pgis::PostGISType::PostGISType()
{
}

plg_pgis::PostGISType::~PostGISType()
{
}

std::string plg_pgis::PostGISType::getName() const
{
  return "POSTGIS";
}

std::string plg_pgis::PostGISType::getTitle() const
{
  return TR_QT_WIDGETS("PostGIS");
}

std::string plg_pgis::PostGISType::getDescription() const
{
  return TR_QT_WIDGETS("Access geospatial data in a PostgreSQL database with the geospatial extension PostGIS");
}

QWidget* plg_pgis::PostGISType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case te_qt::DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new PostGISConnector(parent, f);

//    case DataSourceType::WIDGET_LAYER_SELECTOR:
//      return new DataSetLayerSelector(parent, f);

    default:
      return 0;
  }
}

QIcon plg_pgis::PostGISType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case te_qt::DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("datasource-postgis");

    case te_qt::DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("datasource-postgis");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}


