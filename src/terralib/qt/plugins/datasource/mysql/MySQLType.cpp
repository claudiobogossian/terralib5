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
  \file terralib/qt/plugins/datasource/mysql/MySQLType.cpp

  \brief MySQL data source type.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../widgets/layer/selector/DataSetLayerSelector.h"
#include "MySQLConnector.h"
#include "MySQLType.h"

// STL
#include <cassert>

te::qt::plugins::mysql::MySQLType::MySQLType()
{
}

te::qt::plugins::mysql::MySQLType::~MySQLType()
{
}

std::string te::qt::plugins::mysql::MySQLType::getName() const
{
  return "MYSQL";
}

std::string te::qt::plugins::mysql::MySQLType::getTitle() const
{
  return TR_QT_WIDGETS("MySQL");
}

std::string te::qt::plugins::mysql::MySQLType::getDescription() const
{
  return TR_QT_WIDGETS("Access geospatial data in a MySQL database with the geospatial extension MySQL");
}

QWidget* te::qt::plugins::mysql::MySQLType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case te::qt::widgets::DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new MySQLConnector(parent, f);

    case DataSourceType::WIDGET_LAYER_SELECTOR:
      return new te::qt::widgets::DataSetLayerSelector(parent, f);

    default:
      return 0;
  }
}

QIcon te::qt::plugins::mysql::MySQLType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case te::qt::widgets::DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("datasource-mysql");

    case te::qt::widgets::DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("datasource-mysql");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}


