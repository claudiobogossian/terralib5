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
  \file terralib/qt/widgets/plugin/builder/PluginCMakeWriter.cpp

  \brief This class is used to create the cmake file for the new plugin builded.
*/

#include "PluginCMakeWriter.h"
#include "../../../../common/Version.h"

// STL Includes
#include <cstdlib>
#include <iostream>
#include <ctime>

#define CMAKE_FILE_NAME           "CMakeLists.txt"
#define PLUGIN_INFO_XML_FILE_NAME "plugin_info.xml.in"

te::qt::widgets::PluginCMakeWriter::PluginCMakeWriter()
{
}

te::qt::widgets::PluginCMakeWriter::~PluginCMakeWriter()
{
}

void te::qt::widgets::PluginCMakeWriter::createCmakeFile(const std::string& buildPath, const std::string& sourcePath, const std::string& projName, 
                                                         const std::string exportMacro, const std::string& cmakePath, const te::plugin::PluginInfo& pi)
{
  std::ofstream file;

  std::string fileName = buildPath;
              fileName += "/";
              fileName += CMAKE_FILE_NAME;

  file.open(fileName.c_str());

  //insert default header
  insertDefaultHeader(file);

  //cmake version required
  file << "cmake_minimum_required(VERSION 2.8)\n"; 

  //source build tree location
  file << "\n";
  file << "# Controls for the source and build tree locations\n"; 
  file << "set(SRCDIR " + sourcePath + ")\n";
  file << "set(INCLDIR ${SRCDIR}) \n";

  //project name
  file << "\n";
  file << "# Set the project name\n"; 
  file << "set(PROJ_NAME \"" + projName + "\")\n";
  file << "project(${PROJ_NAME})\n";
  file << "set(LIB_NAME ${PROJ_NAME})\n";

  //find packages
  file << "\n";
  file << "# If this plugin has dependencies with any package\n";
  file << "# you should set it here\n";

  findTerralib(file, cmakePath, pi);

  findBoost(file);

  //UI Files
  file << "\n";
  file << "# If this plugin has UI files\n";
  file << "# you should set it here\n"; 
  file << "# qt4_wrap_ui...\n\n";
  
  //add header and sources
  file << "\n";
  file << "# Add files\n"; 
  file << "file(GLOB SRCS ${SRCDIR}/*.cpp)\n";
  file << "file(GLOB HDRS ${INCLDIR}/*.h)\n";

  //win32 definitions
  file << "\n";
  file << "# WIN32 Definitions\n"; 
  file << "if(WIN32)\n";
  file << "  add_definitions(-D_CRT_SECURE_NO_WARNINGS)\n";
  file << "endif(WIN32)\n";

  //general definitions
  file << "\n";
  file << "# General Definitions\n";
  file << "add_definitions(-D" + exportMacro + ")\n";

  //adding extra files
  file << "\n";
  file << "# Append extra files (used in case project has UI forms)\n";
  file << "#source_group(\"Form Files\" FILES ${FORMS})\n";
  file << "#source_group(\"Generated Files\" FILES ${MOC} ${UI})\n";

  //set configure file
  file << "\n";
  file << "# Configure file\n";
  file << "configure_file(\"${CMAKE_CURRENT_SOURCE_DIR}/plugin_info.xml.in\" ";
  file << "\"${CMAKE_CURRENT_BINARY_DIR}/" + projName + ".xml\")\n";

  //lib properties
  file << "\n";
  file << "# Lib properties\n";
  file << "include_directories( ${DEP_INCLUDES} )\n";
  file << "add_library (${PROJ_NAME} SHARED ${HDRS} ${SRCS}) # ${UI} ${MOC}\n";
  file << "set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)\n";
  file << "target_link_libraries(${PROJ_NAME} ${DEP_LIBS})\n";

  file.close();
}

void te::qt::widgets::PluginCMakeWriter::createPluginInfoFile(const std::string& buildPath, const te::plugin::PluginInfo& pi, const te::plugin::Provider& p)
{
  std::ofstream file;

  std::string fileName = buildPath;
              fileName += "/";
              fileName += PLUGIN_INFO_XML_FILE_NAME;

  file.open(fileName.c_str());

  //insert default header
  //insertDefaultXMLHeader(file);

  //insert plugin info
  file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
  file << "<PluginInfo xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n";
  file << "            xmlns:xsd=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
  file << "            xmlns=\"http://www.terralib.org/schemas/plugin\"\n";
  file << "            xsd:schemaLocation=\"http://www.terralib.org/schemas/plugin ${TERRALIB_SCHEMA_LOCATION}/plugin/plugin_info.xsd\"\n";
  file << "            version=\"" + pi.m_version + "\"\n";
  file << "            release=\"" + pi.m_release + "\"\n";
  file << "            engine=\"" + pi.m_engine + "\">\n";
  file << "  <Name>" + pi.m_name + "</Name>\n";
  file << "  <DisplayName>" + pi.m_displayName + "</DisplayName>\n";
  file << "  <Description>" + pi.m_description + "</Description>\n";
  file << "  <TerraLibVersion>" + pi.m_terralibVersion + "</TerraLibVersion>\n";
  file << "  <License xlink:href=\"" + pi.m_licenseURL + "\">" + pi.m_licenseDescription + "</License>\n";
  file << "  <Category>" + pi.m_category + "</Category>\n";
  file << "  <Site xlink:href=\"" + pi.m_site + "\"/>\n";
  file << "  <Provider>\n";
  file << "    <Name>" + p.m_name + "</Name>\n";
  file << "    <Site xlink:href=\"" + p.m_site + "\"/>\n";
  file << "    <Email>" + p.m_email + "</Email>\n";
  file << "  </Provider>\n";

  if(pi.m_requiredPlugins.empty() == false)
  {
    file << "  <RequiredPlugins>\n";
    for(size_t t = 0; t < pi.m_requiredPlugins.size(); ++t)
    {
      file << "    <PluginId>" + pi.m_requiredPlugins[t] + "</PluginId>\n";
    }
    file << "  </RequiredPlugins>\n";
  }

  if(pi.m_requiredModules.empty() == false)
  {
    file << "  <RequiredModules>\n";
    for(size_t t = 0; t < pi.m_requiredModules.size(); ++t)
    {
      file << "    <ModuleId>" + pi.m_requiredModules[t] + "</ModuleId>\n";
    }
    file << "  </RequiredModules>\n";
  }
  
  
  if(pi.m_resources.empty() == false)
  {
    file << "  <Resources>\n";
    for(size_t t = 0; t < pi.m_resources.size(); ++t)
    {
      file << "    <Resource name=\"" + pi.m_resources[t].first + "\" xlink:href=\"" + pi.m_resources[t].second + "\"/>\n";
    }
    file << "  </Resources>\n";
  }
  
  
  if(pi.m_parameters.empty() == false)
  {
    file << "  <Parameters>\n";
    for(size_t t = 0; t < pi.m_parameters.size(); ++t)
    {
      file << "    <Parameter>\n";
      file << "      <Name>" + pi.m_parameters[t].first + "</Name>\n";
      file << "      <Value>" + pi.m_parameters[t].second + "</Value>\n";
      file << "    </Parameter>\n";
    }
    file << "  </Parameters>\n";
  }
  
  
  file << "</PluginInfo>\n";

  file.close();
}

void te::qt::widgets::PluginCMakeWriter::insertDefaultHeader(std::ofstream& stream)
{
  time_t now = time(0);
  char* dt = ctime(&now);
  std::string currentTime = dt;
  std::string fileName = CMAKE_FILE_NAME;
  std::string teVersion = te::common::Version::asString();

  std::string defaultHeader = "";

  defaultHeader += "#######################################################################\n";
  defaultHeader += "## This file was automatically generated by TerraView - Plugin Builder\n";
  defaultHeader += "## TerraLib Version: " + teVersion + " \n";
  defaultHeader += "## File: " + fileName + "\n";
  defaultHeader += "## Date: " + currentTime;
  defaultHeader += "#######################################################################\n";

  stream << defaultHeader.c_str();
  stream << "\n";
}

void te::qt::widgets::PluginCMakeWriter::insertDefaultXMLHeader(std::ofstream& stream)
{
  time_t now = time(0);
  char* dt = ctime(&now);
  std::string currentTime = dt;
  std::string fileName = CMAKE_FILE_NAME;
  std::string teVersion = te::common::Version::asString();

  std::string defaultHeader = "";

  defaultHeader += "<!--###################################################################\n";
  defaultHeader += "##  This file was automatically generated by TerraView - Plugin Builder\n";
  defaultHeader += "##  TerraLib Version: " + teVersion + "\n";
  defaultHeader += "##  File: " + fileName + "\n";
  defaultHeader += "##  Date: " + currentTime;
  defaultHeader += "####################################################################-->\n";

  stream << defaultHeader.c_str();
}

void te::qt::widgets::PluginCMakeWriter::findTerralib(std::ofstream& stream, const std::string& teCmakePath, const te::plugin::PluginInfo& pi)
{
  std::string componentList = "";

  for(size_t t = 0; t < pi.m_requiredModules.size(); ++t)
  {
    int pos = pi.m_requiredModules[t].rfind(".");

    std::string componentName = pi.m_requiredModules[t].substr(pos + 1, pi.m_requiredModules[t].size() - 1);
    componentList += componentName + " ";
  }

  stream << "list (APPEND CMAKE_PREFIX_PATH \"" + teCmakePath + "\")\n";
  stream << "list (APPEND CMAKE_FIND_ROOT_PATH \"" + teCmakePath + "\")\n";

  stream << "find_package(terralib 5 REQUIRED COMPONENTS " + componentList + ")\n";
  stream << "if(terralib_FOUND)\n";
  stream << "  list (APPEND DEP_INCLUDES ${TE_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})\n";
  stream << "  list (APPEND DEP_LIBS ${TE_LIBRARIES})\n";
  stream << "endif()\n";
}

void te::qt::widgets::PluginCMakeWriter::findBoost(std::ofstream& stream)
{
  stream << "find_package(Boost ${_Boost_VERSION} REQUIRED COMPONENTS system filesystem)\n";
  stream << "if(Boost_FOUND)\n";
  stream << "  list (APPEND DEP_INCLUDES ${Boost_INCLUDE_DIRS})\n";
  stream << "endif()\n";
}
