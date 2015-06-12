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
  \file terralib/qt/widgets/plugin/builder/PluginCMakeWriter.h

  \brief This class is used to create the cmake file for the new plugin builded.
*/

#ifndef __TERRALIB_QT_WIDGETS_PLUGIN_BUILDER_INTERNAL_PLUGINCMAKEWRITER_H
#define __TERRALIB_QT_WIDGETS_PLUGIN_BUILDER_INTERNAL_PLUGINCMAKEWRITER_H

// TerraLib
#include "../../Config.h"
#include "../../../../plugin/PluginInfo.h"
#include "../../../../plugin/Provider.h"

// STL Include
#include <fstream>
#include <string>
#include <vector>

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

      class TEQTWIDGETSEXPORT PluginCMakeWriter
      {
        public:

          /* Default Constructor */
          PluginCMakeWriter();

          /* Destructor */
          ~PluginCMakeWriter();

        public:

          /* Build Cmake File */
          void createCmakeFile(const std::string& buildPath, const std::string& sourcePath, const std::string& projName, 
                               const std::string exportMacro, const std::string& cmakePath, const te::plugin::PluginInfo& pi);

          /* Build XML Plugin Information File */
          void createPluginInfoFile(const std::string& buildPath, const te::plugin::PluginInfo& pi, const te::plugin::Provider& p);

        protected:

          void insertDefaultHeader(std::ofstream& stream);
          
          void insertDefaultXMLHeader(std::ofstream& stream);

          void findTerralib(std::ofstream& stream, const std::string& teCmakePath, const te::plugin::PluginInfo& pi);

          void findBoost(std::ofstream& stream);
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PLUGIN_BUILDER_INTERNAL_PLUGINCMAKEWRITER_H
