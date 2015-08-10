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
  \file terralib/qt/af/settings/ProjectWidgetFactory.h

  \brief A factory to build the project frame object.
*/

#ifndef __TERRALIB_QT_AF_PROJECTWIDGETFACTORY_H
#define __TERRALIB_QT_AF_PROJECTWIDGETFACTORY_H

// TerraLib
#include "../Config.h"
#include <terralib/qt/af/settings/AbstractSettingWidget.h>
#include <terralib/qt/af/settings/SettingsWidgetsFactory.h>

// Forward declarations
class QWidget;

/*!
  \class ProjectWidgetFactory

  \brief A factory to build the project frame object.
*/
class ProjectWidgetFactory : public te::qt::af::SettingsWidgetsFactory
{
  public:

    static void initialize();

    static void finalize();

    ~ProjectWidgetFactory();

  protected:        

    te::qt::af::AbstractSettingWidget* build();

    ProjectWidgetFactory();

    virtual te::qt::af::AbstractSettingWidget* create(QWidget* parent = 0);

  private:

    static ProjectWidgetFactory* sm_factory; 
};

#endif // __TERRALIB_QT_AF_PROJECTWIDGETFACTORY_H