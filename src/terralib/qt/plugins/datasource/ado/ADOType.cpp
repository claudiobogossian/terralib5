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
  \file terralib/qt/widgets/datasource/connector/ado/ADOType.cpp

  \brief ADO data source type.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../widgets/layer/selector/DataSetLayerSelector.h"
#include "ADOConnector.h"
#include "ADOType.h"

// STL
#include <cassert>

te::qt::plugins::ado::ADOType::ADOType()
{
}

te::qt::plugins::ado::ADOType::~ADOType()
{
}

bool te::qt::plugins::ado::ADOType::hasDatabaseSupport() const
{
  return true;
}

bool te::qt::plugins::ado::ADOType::hasFileSupport() const
{
  return false;
}

bool te::qt::plugins::ado::ADOType::hasRasterSupport() const
{
  return false;
}

bool te::qt::plugins::ado::ADOType::hasVectorialSupport() const
{
  return true;
}

std::string te::qt::plugins::ado::ADOType::getName() const
{
  return "ADO";
}

std::string te::qt::plugins::ado::ADOType::getTitle() const
{
  // Friendly Name
  return TE_TR("Microsoft Access");
}

std::string te::qt::plugins::ado::ADOType::getDescription() const
{
  return TE_TR("Access geospatial data stored in ADO database");
}

QWidget* te::qt::plugins::ado::ADOType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new ADOConnector(parent, f);

    case DataSourceType::WIDGET_LAYER_SELECTOR:
      return new te::qt::widgets::DataSetLayerSelector(parent, f);

    default:
      return 0;
  }
}

QIcon te::qt::plugins::ado::ADOType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("datasource-ado");

    case DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("datasource-ado");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}


