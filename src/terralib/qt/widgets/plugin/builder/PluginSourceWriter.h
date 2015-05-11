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
  \file terralib/qt/widgets/plugin/builder/PluginSourceWriter.h

  \brief This class is used to create the source files for the new plugin builded.
*/

#ifndef __TERRALIB_QT_WIDGETS_PLUGIN_BUILDER_INTERNAL_PLUGINSOURCEWRITER_H
#define __TERRALIB_QT_WIDGETS_PLUGIN_BUILDER_INTERNAL_PLUGINSOURCEWRITER_H

// TerraLib
#include "../../Config.h"

// STL Include
#include <fstream>
#include <string>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class PluginCMakeWriter

        \brief This class is used to create the cmake file for the new plugin builded.
      */

      class TEQTWIDGETSEXPORT PluginSourceWriter
      {
        public:

          /* Default Constructor */
          PluginSourceWriter();

          /* Destructor */
          ~PluginSourceWriter();

        public:

          /* Build Header File */
          void createHeaderFile(const std::string& sourcePath, const std::string& nameSpace);

          /* Build Cpp File */
          void createCppFile(const std::string& sourcePath, const std::string& nameSpace, const std::string projName);

          /* Build Config File */
          void createConfigFile(const std::string& sourcePath, const std::string& nameSpace, const std::string exportMacro, const std::string projName);

        protected:

          void insertDefaultHeader(std::ofstream& stream, const std::string& fileName);

          void insertTerraLibHeader(std::ofstream& stream);

          void insertFileInfo(std::ofstream& stream, const std::string& fileName);

          void insertIfDefInfo(std::ofstream& stream, const std::string& nameSpace, const std::string& fileName);

          void insertEndIfInfo(std::ofstream& stream, const std::string& nameSpace, const std::string& fileName);

          std::string getExportMacro(const std::string& nameSpace);
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PLUGIN_BUILDER_INTERNAL_PLUGINSOURCEWRITER_H
