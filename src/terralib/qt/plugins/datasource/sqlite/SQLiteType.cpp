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
  \file terralib/qt/widgets/datasource/connector/sqlite/SQLiteType.cpp

  \brief SQLite data source type.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../widgets/layer/selector/DataSetLayerSelector.h"
#include "SQLiteConnector.h"
#include "SQLiteType.h"

// STL
#include <cassert>

te::qt::plugins::sqlite::SQLiteType::SQLiteType()
{
}

te::qt::plugins::sqlite::SQLiteType::~SQLiteType()
{
}

std::string te::qt::plugins::sqlite::SQLiteType::getName() const
{
  return "SQLITE";
}

std::string te::qt::plugins::sqlite::SQLiteType::getTitle() const
{
  return TR_QT_WIDGETS("SQLite");
}

std::string te::qt::plugins::sqlite::SQLiteType::getDescription() const
{
  return TR_QT_WIDGETS("Access geospatial data stored in SQLite database");
}

QWidget* te::qt::plugins::sqlite::SQLiteType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new SQLiteConnector(parent, f);

    case DataSourceType::WIDGET_LAYER_SELECTOR:
      return new te::qt::widgets::DataSetLayerSelector(parent, f);

    default:
      return 0;
  }
}

QIcon te::qt::plugins::sqlite::SQLiteType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("datasource-sqlite");

    case DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("datasource-sqlite");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}


