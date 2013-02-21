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
  \file terralib/qt/af/Utils.h

  \brief Utility routines for the TerraLib Application Framework module.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_UTILS_H
#define __TERRALIB_QT_AF_INTERNAL_UTILS_H

// TerraLib
#include "../../common/Exception.h"
#include "Config.h"

// Forward declarations
class QStringList;

namespace te
{

// Forward declaration
  namespace xml
  {
    class Reader;
    class Writer;
  }

  namespace qt
  {
    namespace af
    {
      class Project;

      TEQTAFEXPORT Project* ReadProject(const std::string& uri);

      TEQTAFEXPORT Project* ReadProject(te::xml::Reader& reader);

      TEQTAFEXPORT void Save(const Project& project, const std::string& uri);

      TEQTAFEXPORT void Save(const Project& project, te::xml::Writer& writer);

      TEQTAFEXPORT void UpdateUserSettingsFile(const QStringList& prjFiles, const QStringList& prjTitles, const std::string& userConfigFile);

      TEQTAFEXPORT void saveDataSourcesFile();
    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_INTERNAL_UTILS_H

