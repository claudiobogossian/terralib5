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
  \file terralib/qt/plugins/datasource/shp/ShapeFileType.cpp

  \brief ShapeFile data source type.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../widgets/layer/selector/HiddenDataSetLayerSelector.h"
#include "ShapeFileConnector.h"
//#include "ShapeFileSelector.h"
#include "ShapeFileType.h"

// STL
#include <cassert>

te::qt::plugins::shp::ShapeFileType::ShapeFileType()
{
}

te::qt::plugins::shp::ShapeFileType::~ShapeFileType()
{
}

std::string te::qt::plugins::shp::ShapeFileType::getName() const
{
  return "SHAPEFILE";
}

std::string te::qt::plugins::shp::ShapeFileType::getTitle() const
{
  return TR_QT_WIDGETS("Shapefile");
}

std::string te::qt::plugins::shp::ShapeFileType::getDescription() const
{
  return TR_QT_WIDGETS("Quick access to ESRI shapefiles");
}

QWidget* te::qt::plugins::shp::ShapeFileType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new ShapeFileConnector(parent, f);

    case DataSourceType::WIDGET_LAYER_SELECTOR:
      return new te::qt::widgets::HiddenDataSetLayerSelector(parent, f);  //return new ShapeFileSelector(parent, f);

    default:
      return 0;
  }
}

QIcon te::qt::plugins::shp::ShapeFileType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("datasource-shapefile");

    case DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("datasource-shapefile");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}


