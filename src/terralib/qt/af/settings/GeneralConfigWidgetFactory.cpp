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
  \file terralib/qt/af/settings/GeneralConfigWidgetFactory.cpp

  \brief A factory to build the general config frame object.
*/

#include "GeneralConfigWidget.h"
#include "GeneralConfigWidgetFactory.h"

te::qt::af::GeneralConfigWidgetFactory* te::qt::af::GeneralConfigWidgetFactory::sm_factory(0);

void te::qt::af::GeneralConfigWidgetFactory::initialize()
{
  finalize();
  sm_factory = new GeneralConfigWidgetFactory;
}

void te::qt::af::GeneralConfigWidgetFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::af::GeneralConfigWidgetFactory::~GeneralConfigWidgetFactory()
{
}

te::qt::af::AbstractSettingWidget* te::qt::af::GeneralConfigWidgetFactory::build()
{
  return new GeneralConfigWidget;
}

te::qt::af::GeneralConfigWidgetFactory::GeneralConfigWidgetFactory() : 
te::qt::af::SettingsWidgetsFactory(QObject::tr("General configurations").toStdString())
{

}

te::qt::af::AbstractSettingWidget* te::qt::af::GeneralConfigWidgetFactory::create(QWidget* parent)
{
  return new GeneralConfigWidget(parent);
}