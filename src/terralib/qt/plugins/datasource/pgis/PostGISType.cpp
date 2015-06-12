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
  \file terralib/qt/plugins/datasource/pgis/PostGISType.cpp

  \brief PostGIS data source type.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../widgets/layer/selector/DataSetLayerSelector.h"
#include "PostGISConnector.h"
#include "PostGISType.h"

// STL
#include <cassert>

te::qt::plugins::pgis::PostGISType::PostGISType()
{
}

te::qt::plugins::pgis::PostGISType::~PostGISType()
{
}

bool te::qt::plugins::pgis::PostGISType::hasDatabaseSupport() const
{
  return false;
}

bool te::qt::plugins::pgis::PostGISType::hasFileSupport() const
{
  return true;
}

bool te::qt::plugins::pgis::PostGISType::hasRasterSupport() const
{
  return false;
}

bool te::qt::plugins::pgis::PostGISType::hasVectorialSupport() const
{
  return true;
}

std::string te::qt::plugins::pgis::PostGISType::getName() const
{
  return "POSTGIS";
}

std::string te::qt::plugins::pgis::PostGISType::getTitle() const
{
  return TE_TR("PostGIS");
}

std::string te::qt::plugins::pgis::PostGISType::getDescription() const
{
  return TE_TR("Access geospatial data in a PostgreSQL database with the geospatial extension PostGIS");
}

QWidget* te::qt::plugins::pgis::PostGISType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case te::qt::widgets::DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new PostGISConnector(parent, f);

    case DataSourceType::WIDGET_LAYER_SELECTOR:
      return new te::qt::widgets::DataSetLayerSelector(parent, f);

    default:
      return 0;
  }
}

QIcon te::qt::plugins::pgis::PostGISType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case te::qt::widgets::DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("datasource-postgis");

    case te::qt::widgets::DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("datasource-postgis");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}


