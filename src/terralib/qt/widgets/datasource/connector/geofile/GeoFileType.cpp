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
  \file terralib/qt/widgets/datasource/connector/geofile/GeoFileType.cpp

  \brief File based data source type.
*/

// TerraLib
#include "../../../../../common/Translator.h"
//#include "../../../layer/selector/core/DataSetLayerSelector.h"
#include "GeoFileConnector.h"
#include "GeoFileType.h"

// STL
#include <cassert>

te::qt::widgets::GeoFileType::GeoFileType()
{
}

te::qt::widgets::GeoFileType::~GeoFileType()
{
}

std::string te::qt::widgets::GeoFileType::getName() const
{
  return "GEOFILE";
}

std::string te::qt::widgets::GeoFileType::getTitle() const
{
  return TR_QT_WIDGETS("Geospatial Data");
}

std::string te::qt::widgets::GeoFileType::getDescription() const
{
  return TR_QT_WIDGETS("Access local or network files with geo-spatial data");
}

QWidget* te::qt::widgets::GeoFileType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new GeoFileConnector(parent, f);

//    case DataSourceType::WIDGET_LAYER_SELECTOR:
//      return new DataSetLayerSelector(parent, f);

    default:
      return 0;
  }
}

QIcon te::qt::widgets::GeoFileType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("datasource-geofile");

    case DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("datasource-geofile");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}


