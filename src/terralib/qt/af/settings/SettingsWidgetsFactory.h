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
  \file ProjectWidget.h

  \brief A frame for setting project options.
*/
#ifndef __TERRALIB_QT_AF_SETTINGSWIDGETSFACTORY_H
#define __TERRALIB_QT_AF_SETTINGSWIDGETSFACTORY_H

#include "../../../common/AbstractFactory.h"

// Forward declarations
class AbstractSettingWidget;

namespace te
{
  namespace qt
  {
    namespace af
    {
      class SettingsWidgetsFactory : public te::common::AbstractFactory<AbstractSettingWidget, std::string>
      {
        public:
          
          static AbstractSettingWidget* make(const std::string& cwType, QWidget* parent = 0);

          ~SettingsWidgetsFactory();

        protected:

          SettingsWidgetsFactory(const std::string& fKey);

          virtual AbstractSettingWidget* create(QWidget* parent = 0) = 0;
      };

      inline AbstractSettingWidget* SettingsWidgetsFactory::make(const std::string& cwType, QWidget* parent)
      {
        te::common::AbstractFactory<AbstractSettingWidget, std::string>::dictionary_type& d = te::common::AbstractFactory<AbstractSettingWidget, std::string>::getDictionary();

        SettingsWidgetsFactory* f = static_cast<SettingsWidgetsFactory*>(d.find(cwType));

        if(f == 0)
          throw QObject::tr("Fail to find QWidget factory");

        return f->create(parent);
      }

      inline SettingsWidgetsFactory::~SettingsWidgetsFactory()
      {
      }

      inline SettingsWidgetsFactory::SettingsWidgetsFactory(const std::string& fKey) :
      te::common::AbstractFactory<AbstractSettingWidget, std::string>(fKey)
      {
      }
    }
  }
}

#endif //__TERRALIB_QT_AF_SETTINGSWIDGETSFACTORY_H
