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
  \file terralib/qt/af/ApplicationPlugins.h

  \brief A singleton for managing the application plugins.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_APPLICATIONPLUGINS_H
#define __TERRALIB_QT_AF_INTERNAL_APPLICATIONPLUGINS_H

//! TerraLib
#include "../../common/ApplicationSettings.h"
#include "../../common/Singleton.h"
#include "Config.h"

//! Qt
#include <QObject>

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class ApplicationPlugins

        \brief A singleton for managing the application plugins.

        This singleton can be used to keep the list of plugins managed by the application.

        It will look for a plugin configuration file as follows:
        <ul>
        <li>first, searchs for the plugin configuration file name in SystemApplicationSettings</li>
        <li>then it check for this file in the current application dir</li>
        <li>if not foud it looks in the system application dir</li>
        <li>if not found looks in the user folder</li>
        <li>and finally it uses a location defined by an environment variable TERRALIB_DIR_ENVVAR under TERRALIB_CONFIG_DIR</li>
        </ul>

        \ingroup af
      */
      class TEQTAFEXPORT ApplicationPlugins
        : public QObject,
          public te::common::ApplicationSettings,
          public te::common::Singleton<ApplicationPlugins>
      {
        Q_OBJECT

        friend class te::common::Singleton<ApplicationPlugins>;

        public:

          void load();

          void load(const std::string& fileName);

          const std::map<std::string, std::string>& getPluginsFiles() const;

          void addPlugin(const std::string& pluginName, const std::string& pluginFileName);

          void removePlugin(const std::string& pluginName);

          const std::string& getFileName() const;

        public slots:

          void addPlugins(const std::map<std::string, std::string>& plgs);

          void removePlugins(const std::vector<std::string>& plgs);

        protected:

          /*! \brief It initializes the singleton. */
          ApplicationPlugins();

          /*! \brief Destructor. */
          ~ApplicationPlugins();

          std::map<std::string, std::string> m_plugins_files;
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_INTERNAL_APPLICATIONPLUGINS_H
