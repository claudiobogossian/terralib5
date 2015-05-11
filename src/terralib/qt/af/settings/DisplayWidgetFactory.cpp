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
  \file terralib/qt/af/settings/DisplayWidgetFactory.cpp

  \brief A factory to build the display frame object.
*/

#include "DisplayWidget.h"
#include "DisplayWidgetFactory.h"

te::qt::af::DisplayWidgetFactory* te::qt::af::DisplayWidgetFactory::sm_factory(0);

void te::qt::af::DisplayWidgetFactory::initialize()
{
  finalize();
  sm_factory = new DisplayWidgetFactory;
}

void te::qt::af::DisplayWidgetFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::af::DisplayWidgetFactory::~DisplayWidgetFactory()
{
}

te::qt::af::AbstractSettingWidget* te::qt::af::DisplayWidgetFactory::build()
{
  return new DisplayWidget;
}

te::qt::af::DisplayWidgetFactory::DisplayWidgetFactory() : 
te::qt::af::SettingsWidgetsFactory(QObject::tr("Display").toStdString())
{

}

te::qt::af::AbstractSettingWidget* te::qt::af::DisplayWidgetFactory::create(QWidget* parent)
{
  return new DisplayWidget(parent);
}