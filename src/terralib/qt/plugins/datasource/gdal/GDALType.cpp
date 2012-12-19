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
  \file terralib/qt/plugins/datasource/gdal/GDALType.cpp

  \brief GDAL data source type.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "GDALConnector.h"
#include "GDALType.h"

// STL
#include <cassert>

te::qt::plugins::gdal::GDALType::GDALType()
{
}

te::qt::plugins::gdal::GDALType::~GDALType()
{
}

std::string te::qt::plugins::gdal::GDALType::getName() const
{
  return "GDAL";
}

std::string te::qt::plugins::gdal::GDALType::getTitle() const
{
  return TR_QT_WIDGETS("GDAL");
}

std::string te::qt::plugins::gdal::GDALType::getDescription() const
{
  return TR_QT_WIDGETS("Access to geospatial data through the Geospatial Data Abstraction Library (GDAL)");
}

QWidget* te::qt::plugins::gdal::GDALType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case te::qt::widgets::DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new GDALConnector(parent, f);

//    case DataSourceType::WIDGET_LAYER_SELECTOR:
//      return new DataSetLayerSelector(parent, f);

    default:
      return 0;
  }
}

QIcon te::qt::plugins::gdal::GDALType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case te::qt::widgets::DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("datasource-gdal");

    case te::qt::widgets::DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("datasource-gdal");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}


