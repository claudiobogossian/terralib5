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
  \file terralib/qt/widgets/datasource/connector/wfs/WFSType.cpp

  \brief WFS data source type.
*/

// TerraLib
#include <terralib/common/Translator.h>
//#include "../../../layer/selector/core/DataSetLayerSelector.h"
#include "WFSConnector.h"
#include "WFSType.h"

// STL
#include <cassert>

namespace plg_wfs = qt_af::plugin::wfs;
namespace te_qt = te::qt::widgets;

plg_wfs::WFSType::WFSType()
{
}

plg_wfs::WFSType::~WFSType()
{
}

std::string plg_wfs::WFSType::getName() const
{
  return "WFS";
}

std::string plg_wfs::WFSType::getTitle() const
{
  return AF_TR_WFS("Web Feature Service");
}

std::string plg_wfs::WFSType::getDescription() const
{
  return AF_TR_WFS("Access to geospatial through a Web Feature Service (WFS)");
}

QWidget* plg_wfs::WFSType::getWidget(int widgetType, QWidget* parent, Qt::WindowFlags f) const
{
  switch(widgetType)
  {
    case te_qt::DataSourceType::WIDGET_DATASOURCE_CONNECTOR:
      return new WFSConnector(parent, f);

    //case DataSourceType::WIDGET_LAYER_SELECTOR:
      //return new DataSetLayerSelector(parent, f);

    default:
      return 0;
  }
}

QIcon plg_wfs::WFSType::getIcon(int iconType) const
{
  switch(iconType)
  {
    case te_qt::DataSourceType::ICON_DATASOURCE_SMALL:
      return QIcon::fromTheme("datasource-wfs");

    case te_qt::DataSourceType::ICON_DATASOURCE_CONNECTOR:
      return QIcon::fromTheme("datasource-wfs");

    default:
      return QIcon::fromTheme("unknown-icon");
  }
}


