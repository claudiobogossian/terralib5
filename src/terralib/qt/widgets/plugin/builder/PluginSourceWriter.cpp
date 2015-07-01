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
  \file terralib/qt/widgets/plugin/builder/PluginSourceWriter.cpp

  \brief This class is used to create the source files for the new plugin builded.
*/

#include "PluginSourceWriter.h"
#include "../../../../common/Version.h"
#include "../../../../common/StringUtils.h"

// STL Includes
#include <cstdlib>
#include <iostream>
#include <ctime>

#define PLUGIN_H_FILE_NAME       "Plugin.h"
#define PLUGIN_CPP_FILE_NAME     "Plugin.cpp"
#define PLUGIN_CONFIG_FILE_NAME  "Config.h"

te::qt::widgets::PluginSourceWriter::PluginSourceWriter()
{
}

te::qt::widgets::PluginSourceWriter::~PluginSourceWriter()
{
}

void te::qt::widgets::PluginSourceWriter::createHeaderFile(const std::string& sourcePath, const std::string& nameSpace)
{
  std::ofstream file;

  std::string fileName = sourcePath;
              fileName += "/";
              fileName += PLUGIN_H_FILE_NAME;

  file.open(fileName.c_str());

  //insert default header
  insertDefaultHeader(file, PLUGIN_H_FILE_NAME);

  //insert terralib header
  insertTerraLibHeader(file);

  //insert file info
  insertFileInfo(file, PLUGIN_H_FILE_NAME);

  //insert ifdef
  insertIfDefInfo(file, nameSpace, PLUGIN_H_FILE_NAME);

  //add includes
  file << "// TerraLib Includes\n";
  file << "#include \"Config.h\"\n";
  file << "#include <terralib/plugin/Plugin.h>\n\n";

  //define class
  file << "namespace te\n";
  file << "{\n";
  file << "  namespace qt\n";
  file << "  {\n";
  file << "    namespace plugins\n";
  file << "    {\n";
  file << "      namespace " + nameSpace +"\n";
  file << "      {\n";
  file << "        class Plugin : public te::plugin::Plugin\n";
  file << "        {\n";
  file << "          public:\n";
  file << "\n";
  file << "            Plugin(const te::plugin::PluginInfo& pluginInfo);\n";
  file << "\n";
  file << "            ~Plugin();\n";
  file << "\n";
  file << "            void startup();\n";
  file << "\n";
  file << "            void shutdown();\n";
  file << "        };\n";
  file << "\n";
  file << "      } // end namespace " + nameSpace + "\n";
  file << "    }   // end namespace plugins\n";
  file << "  }     // end namespace qt\n";
  file << "}       // end namespace te\n";

  file << "\nPLUGIN_CALL_BACK_DECLARATION(" + getExportMacro(nameSpace) + ")\n";

  //insert endif
  insertEndIfInfo(file, nameSpace, PLUGIN_H_FILE_NAME);

  file.close();
}

void te::qt::widgets::PluginSourceWriter::createCppFile(const std::string& sourcePath, const std::string& nameSpace, const std::string projName)
{
  std::ofstream file;

  std::string fileName = sourcePath;
              fileName += "/";
              fileName += PLUGIN_CPP_FILE_NAME;

  file.open(fileName.c_str());

  //insert default header
  insertDefaultHeader(file, PLUGIN_CPP_FILE_NAME);

  //insert terralib header
  insertTerraLibHeader(file);

  //insert file info
  insertFileInfo(file, PLUGIN_CPP_FILE_NAME);

  //add includes
  file << "// TerraLib Includes\n";
  file << "#include \"Plugin.h\"\n\n";
  file << "#include <terralib/common/Config.h>\n";
  file << "#include <terralib/common/Logger.h>\n";
  file << "#include <terralib/common/Translator.h>\n\n";
  
  //constructor
  file << "te::qt::plugins::" + nameSpace + "::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)\n";
  file << "  : te::plugin::Plugin(pluginInfo)\n";
  file << "{\n";
  file << "}\n\n";

  //destructor
  file << "te::qt::plugins::" + nameSpace + "::Plugin::~Plugin()\n";
  file << "{\n";
  file << "}\n\n";

  std::string nsUpperCase = te::common::Convert2UCase(nameSpace);

  //startup
  file << "void te::qt::plugins::" + nameSpace + "::Plugin::startup()\n";
  file << "{\n";
  file << "  if(m_initialized)\n";
  file << "    return;\n";
  file << "\n";
  file << "// it initializes the Translator support\n";
  file << "  TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_TEXT_DOMAIN, TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_TEXT_DOMAIN_DIR, \"UTF-8\");\n";
  file << "\n";
  file << "  TE_LOG_TRACE(TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "(\"Plugin " + projName + " startup!\"));\n";
  file << "\n";
  file << "  m_initialized = true;\n";
  file << "}\n\n";

  //shutdown
  file << "void te::qt::plugins::" + nameSpace + "::Plugin::shutdown()\n";
  file << "{\n";
  file << "  if(!m_initialized)\n";
  file << "    return;\n";
  file << "\n";
  file << "  TE_LOG_TRACE(TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "(\"Plugin " + projName + " shutdown!\"));\n";
  file << "\n";
  file << "  m_initialized = false;\n";
  file << "}\n\n";


  file << "PLUGIN_CALL_BACK_IMPL(te::qt::plugins::" + nameSpace + "::Plugin)\n";

  file.close();
}

void te::qt::widgets::PluginSourceWriter::createConfigFile(const std::string& sourcePath, const std::string& nameSpace, const std::string exportMacro, const std::string projName)
{
  std::ofstream file;

  std::string fileName = sourcePath;
              fileName += "/";
              fileName += PLUGIN_CONFIG_FILE_NAME;

  file.open(fileName.c_str());

  //insert default header
  insertDefaultHeader(file, PLUGIN_CONFIG_FILE_NAME);

  //insert terralib header
  insertTerraLibHeader(file);

  //insert file info
  insertFileInfo(file, PLUGIN_CONFIG_FILE_NAME);

  //insert ifdef
  insertIfDefInfo(file, nameSpace, PLUGIN_CONFIG_FILE_NAME);

  std::string nsUpperCase = te::common::Convert2UCase(nameSpace);

  //plugin name
  file << "/*!\n";
  file << "  \\def TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_PLUGIN_NAME\n";
  file << "\n";
  file << "  \\brief It contains the plugin name.\n";
  file << "*/\n";
  file << "#define TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_PLUGIN_NAME \"" + projName + "\"\n\n";

  //text domain
  file << "/*!\n";
  file << "  \\def TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_TEXT_DOMAIN\n";
  file << "\n";
  file << "  \\brief It contains the name of the text domain used in the translation of messages in this plugin.\n";
  file << "*/\n";
  file << "#define TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_TEXT_DOMAIN \"" + projName + "\"\n\n";

  //translation catalog dir
  file << "/*!\n";
  file << "  \\def TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_TEXT_DOMAIN_DIR\n";
  file << "\n";
  file << "  \\brief It contains the translation catalog directory.\n";
  file << "*/\n";
  file << "#define TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_TEXT_DOMAIN_DIR \"locale\"\n\n";

  //translation macro
  file << "/*!\n";
  file << "  \\def TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "\n";
  file << "\n";
  file << "  \\brief It marks a string in order to get translated.\n";
  file << "*/\n";
  file << "#define TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "(message) TR(message, TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_TEXT_DOMAIN)\n\n";

  //export information
  file << "/*!\n";
  file << "  \\def " + getExportMacro(nameSpace) + "\n";
  file << "\n";
  file << "  \\brief You can use this macro in order to export/import classes and functions from all plug-ins files.\n";
  file << "\n";
  file << "  \\note To compile plug-ins in Windows, remember to insert " + exportMacro + " into the project's list of defines.\n";
  file << "*/\n";
  file << "#ifdef WIN32\n";
  file << "  #ifdef " + exportMacro + "\n";
  file << "    #define " + getExportMacro(nameSpace) + "  __declspec(dllexport)   // export DLL information\n";
  file << "  #else\n";
  file << "    #define " + getExportMacro(nameSpace) + "  __declspec(dllimport)   // import DLL information\n";
  file << "  #endif \n";
  file << "#else\n";
  file << "  #define " + getExportMacro(nameSpace) + "\n";
  file << "#endif\n\n";

  //insert endif
  insertEndIfInfo(file, nameSpace, PLUGIN_CONFIG_FILE_NAME);

  file.close();
}

void te::qt::widgets::PluginSourceWriter::insertDefaultHeader(std::ofstream& stream, const std::string& fileName)
{
  time_t now = time(0);
  char* dt = ctime(&now);
  std::string currentTime = dt;
  std::string teVersion = te::common::Version::asString();

  std::string defaultHeader = "";

  defaultHeader += "/**********************************************************************\n";
  defaultHeader += "** This file was automatically generated by TerraView - Plugin Builder\n";
  defaultHeader += "** TerraLib Version: " + teVersion + " \n";
  defaultHeader += "** File: " + fileName + "\n";
  defaultHeader += "** Date: " + currentTime;
  defaultHeader += "**********************************************************************/\n";

  stream << defaultHeader.c_str();
  stream << "\n";
}

void te::qt::widgets::PluginSourceWriter::insertTerraLibHeader(std::ofstream& stream)
{
  std::string defaultHeader = "";

  defaultHeader += "/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.\n";
  defaultHeader += "\n";
  defaultHeader += "    This file is part of the TerraLib - a Framework for building GIS enabled applications.\n";
  defaultHeader += "\n";
  defaultHeader += "    TerraLib is free software: you can redistribute it and/or modify\n";
  defaultHeader += "    it under the terms of the GNU Lesser General Public License as published by\n";
  defaultHeader += "    the Free Software Foundation, either version 3 of the License,\n";
  defaultHeader += "    or (at your option) any later version.\n";
  defaultHeader += "\n";
  defaultHeader += "    TerraLib is distributed in the hope that it will be useful,\n";
  defaultHeader += "    but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
  defaultHeader += "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n";
  defaultHeader += "    GNU Lesser General Public License for more details.\n";
  defaultHeader += "\n";
  defaultHeader += "    You should have received a copy of the GNU Lesser General Public License\n";
  defaultHeader += "    along with TerraLib. See COPYING. If not, write to\n";
  defaultHeader += "    TerraLib Team at <terralib-team@terralib.org>.\n";
  defaultHeader += "*/\n";

  stream << defaultHeader.c_str();
  stream << "\n";
}

void te::qt::widgets::PluginSourceWriter::insertFileInfo(std::ofstream& stream, const std::string& fileName)
{
  std::string info = "";

  info += "/*!\n";
  info += "  \\file " + fileName +"\n";
  info += "\n";
  info += "  \\brief ...\n";
  info += "*/\n";

  stream << info.c_str();
  stream << "\n";
}

void te::qt::widgets::PluginSourceWriter::insertIfDefInfo(std::ofstream& stream, const std::string& nameSpace, const std::string& fileName)
{
  std::string str = "";

  std::string nsUpperCase = te::common::Convert2UCase(nameSpace);

  int pos = fileName.find(".");
  std::string body = fileName.substr(0, pos);
  std::string flUPpperCase = te::common::Convert2UCase(body);

  str += "#ifndef __TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_INTERNAL_" + flUPpperCase + "_H\n";
  str += "#define __TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_INTERNAL_" + flUPpperCase + "_H\n";

  stream << str.c_str();
  stream << "\n";
}

void te::qt::widgets::PluginSourceWriter::insertEndIfInfo(std::ofstream& stream, const std::string& nameSpace, const std::string& fileName)
{
  std::string str = "";

  std::string nsUpperCase = te::common::Convert2UCase(nameSpace);

  int pos = fileName.find(".");
  std::string body = fileName.substr(0, pos);
  std::string flUPpperCase = te::common::Convert2UCase(body);

  str += "#endif //__TE_QT_PLUGIN_BUILDER_" + nsUpperCase + "_INTERNAL_" + flUPpperCase + "_H\n";
  
  stream << "\n";
  stream << str.c_str();
  stream << "\n";
}

std::string te::qt::widgets::PluginSourceWriter::getExportMacro(const std::string& nameSpace)
{
  std::string nsUpperCase = te::common::Convert2UCase(nameSpace);

  std::string tag = "TEQTPLUGIN" + nsUpperCase + "EXPORT";

  return tag;
}
