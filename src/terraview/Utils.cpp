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
  \file terraview/Utils.cpp

  \brief Utility routines for the TerraView Application Framework module.
*/

// Boost
#include <boost/foreach.hpp> // Boost => don't change this include order, otherwise you may have compiling problems! 

// TerraLib
#include <terralib/Version.h>
#include <terralib/common/BoostUtils.h>
#include <terralib/common/PlatformUtils.h>
#include <terralib/xml/AbstractWriter.h>
#include <terralib/xml/AbstractWriterFactory.h>
#include <terralib/xml/Reader.h>
#include <terralib/xml/ReaderFactory.h>
#include "Utils.h"

// STL
#include <cassert>
#include <fstream>
#include <memory>

// Boost
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <QApplication>
#include <QSettings>


void GetProjectInformationsFromSettings(QString& defaultAuthor, int& maxSaved)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("projects");
  defaultAuthor = sett.value("author_name").toString();
  maxSaved = sett.value("recents_history_size").toInt();
  sett.endGroup();
}

void SaveProjectInformationsOnSettings(const QString& defaultAuthor, const int& maxSaved)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("projects");
  sett.setValue("author_name", defaultAuthor);
  sett.setValue("recents_history_size", maxSaved);
  sett.endGroup();
}

void SaveOpenLastProjectOnSettings(bool openLast)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.setValue("projects/openLastDataSource", openLast);
}

bool GetOpenLastProjectFromSettings()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  QVariant variant = sett.value("projects/openLastDataSource");

  // If the option was never edited
  if(variant.isNull() || !variant.isValid())
    return true;

  return variant.toBool();
}

void WriteDefaultProjectFile(const QString& fileName)
{
  boost::property_tree::ptree p;

  std::string schema_location = te::common::FindInTerraLibPath("share/terralib/schemas/terralib/qt/af/project.xsd");

  //Header
  p.add("Project.<xmlattr>.xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance");
  p.add("Project.<xmlattr>.xmlns:te_map", "http://www.terralib.org/schemas/maptools");
  p.add("Project.<xmlattr>.xmlns:te_qt_af", "http://www.terralib.org/schemas/qt/af");
  p.add("Project.<xmlattr>.xmlns", "http://www.terralib.org/schemas/qt/af");
  p.add("Project.<xmlattr>.xsd:schemaLocation", "http://www.terralib.org/schemas/qt/af " + schema_location);
  p.add("Project.<xmlattr>.version", TERRALIB_VERSION_STRING);

  //Contents
  p.add("Project.Title", "Default project");
  p.add("Project.Author", "");
  p.add("Project.ComponentList", "");
  p.add("Project.te_map:LayerList", "");

  //Store file
#if BOOST_VERSION > 105600
  boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
#else
  boost::property_tree::xml_writer_settings<char> settings('\t', 1);
#endif
  boost::property_tree::write_xml(fileName.toStdString(), p, std::locale(), settings);
}

void CreateDefaultSettings()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("toolbars");

  sett.beginGroup("File Tool Bar");
  sett.setValue("name", "File Tool Bar");
  sett.beginWriteArray("Actions");
  sett.setArrayIndex(0);
  sett.setValue("action", "File.New Project");
  sett.setArrayIndex(1);
  sett.setValue("action", "File.Open Project");
  sett.setArrayIndex(2);
  sett.setValue("action", "File.Save Project");
  sett.setArrayIndex(3);
  sett.setValue("action", "");
  sett.setArrayIndex(4);
  sett.setValue("action", "Project.New Folder");
  sett.setArrayIndex(5);
  sett.setValue("action", "Project.Add Layer.All Sources");
  sett.endArray();
  sett.endGroup();

  sett.beginGroup("View Tool Bar");
  sett.setValue("name", "View Tool Bar");
  sett.beginWriteArray("Actions");
  sett.setArrayIndex(0);
  sett.setValue("action", "View.Layer Explorer");
  sett.setArrayIndex(1);
  sett.setValue("action", "View.Map Display");
  sett.setArrayIndex(2);
  sett.setValue("action", "View.Data Table");
  sett.setArrayIndex(3);
  sett.setValue("action", "View.Style Explorer");
  sett.endArray();
  sett.endGroup();

  sett.beginGroup("Map Tool Bar");
  sett.setValue("name", "Map Tool Bar");
  sett.beginWriteArray("Actions");
  sett.setArrayIndex(0);
  sett.setValue("action", "Map.Draw");
  sett.setArrayIndex(1);
  sett.setValue("action", "Map.Previous Extent");
  sett.setArrayIndex(2);
  sett.setValue("action", "Map.Next Extent");
  sett.setArrayIndex(3);
  sett.setValue("action", "Map.Zoom Extent");
  sett.setArrayIndex(4);
  sett.setValue("action", "");
  sett.setArrayIndex(5);
  sett.setValue("action", "Map.Zoom In");
  sett.setArrayIndex(6);
  sett.setValue("action", "Map.Zoom Out");
  sett.setArrayIndex(7);
  sett.setValue("action", "Map.Pan");
  sett.setArrayIndex(8);
  sett.setValue("action", "");
  sett.setArrayIndex(9);
  sett.setValue("action", "Map.Info");
  sett.setArrayIndex(10);
  sett.setValue("action", "Map.Selection");
  sett.endArray();
  sett.endGroup();

  sett.endGroup();

  sett.beginGroup("projects");

  sett.setValue("author_name", "");
  sett.setValue("recents_history_size", "8");

  sett.endGroup();
}

