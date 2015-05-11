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
  \file terralib/qt/plugins/datasource/gdal/GDALType.cpp

  \brief GDAL data source type.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../widgets/layer/selector/DataSetLayerSelector.h"
//#include "../../../widgets/layer/selector/HiddenDataSetLayerSelector.h"
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

bool te::qt::plugins::gdal::GDALType::hasDatabaseSupport() const
{
  return false;
}

bool te::qt::plugins::gdal::GDALType::hasFileSupport() const
{
  return true;
}

bool te::qt::plugins::gdal::GDALType::hasRasterSupport() const
{
  return true;
}

bool te::qt::plugins::gdal::GDALType::hasVectorialSupport() const
{
  return false;
}

std::string te::qt::plugins::gdal::GDALType::getName() const
{
  return "GDAL";
}

std::string te::qt::plugins::gdal::GDALType::getTitle() const
{
  return TE_TR("Raster files");
}

std::string te::qt::plugins::gdal::GDALType::getDescription() const
{
  return TE_TR("Access to geospatial data through the Geospatial Data Abstraction Library (GDAL)");
}

QWidget* te::qt::plugins::gdal::GDALType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case te::qt::widgets::DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new GDALConnector(parent, f);

    case DataSourceType::WIDGET_LAYER_SELECTOR:
      return new te::qt::widgets::DataSetLayerSelector(parent, f);
      //return new te::qt::widgets::HiddenDataSetLayerSelector(parent, f);

    default:
      return 0;
  }
}

QIcon te::qt::plugins::gdal::GDALType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case te::qt::widgets::DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("file-raster");

    case te::qt::widgets::DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("file-raster");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}


