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
  \file terralib/qt/plugins/datasource/wfs/WFSType.cpp

  \brief WFS data source type.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../widgets/layer/selector/DataSetLayerSelector.h"
#include "WFSConnector.h"
#include "WFSType.h"

// STL
#include <cassert>

te::qt::plugins::wfs::WFSType::WFSType()
{
}

te::qt::plugins::wfs::WFSType::~WFSType()
{
}

bool te::qt::plugins::wfs::WFSType::hasDatabaseSupport() const
{
  return false;
}

bool te::qt::plugins::wfs::WFSType::hasFileSupport() const
{
  return false;
}

bool te::qt::plugins::wfs::WFSType::hasRasterSupport() const
{
  return false;
}

bool te::qt::plugins::wfs::WFSType::hasVectorialSupport() const
{
  return false;
}

std::string te::qt::plugins::wfs::WFSType::getName() const
{
  return "WFS";
}

std::string te::qt::plugins::wfs::WFSType::getTitle() const
{
  return TE_TR("Web Feature Service");
}

std::string te::qt::plugins::wfs::WFSType::getDescription() const
{
  return TE_TR("Access to geospatial through a Web Feature Service (WFS)");
}

QWidget* te::qt::plugins::wfs::WFSType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case te::qt::widgets::DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new WFSConnector(parent, f);

    case DataSourceType::WIDGET_LAYER_SELECTOR:
      return new te::qt::widgets::DataSetLayerSelector(parent, f);

    default:
      return 0;
  }
}

QIcon te::qt::plugins::wfs::WFSType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case te::qt::widgets::DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("datasource-wfs");

    case te::qt::widgets::DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("datasource-wfs");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}
