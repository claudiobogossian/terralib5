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
  \file terralib/qt/widgets/datasource/connector/ogr/OGRType.cpp

  \brief OGR data source type.
*/

// TerraLib
#include <terralib/common/Translator.h>
//#include "../../../layer/selector/core/DataSetLayerSelector.h"
#include "OGRConnector.h"
#include "OGRType.h"

// STL
#include <cassert>

namespace plg_ogr = qt_af::plugin::ogr;
namespace te_qt = te::qt::widgets;

plg_ogr::OGRType::OGRType()
{
}

plg_ogr::OGRType::~OGRType()
{
}

std::string plg_ogr::OGRType::getName() const
{
  return "OGR";
}

std::string plg_ogr::OGRType::getTitle() const
{
  return AF_TR_OGR("OGR");
}

std::string plg_ogr::OGRType::getDescription() const
{
  return AF_TR_OGR("Access to simple feature data through OGR (GDAL/OGR)");
}

QWidget* plg_ogr::OGRType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case te_qt::DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new OGRConnector(parent, f);

//    case DataSourceType::WIDGET_LAYER_SELECTOR:
//      return new DataSetLayerSelector(parent, f);

    default:
      return 0;
  }
}

QIcon plg_ogr::OGRType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case te_qt::DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("datasource-gdal");

    case te_qt::DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("datasource-gdal");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}


