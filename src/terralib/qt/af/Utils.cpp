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
  \file terralib/qt/af/Utils.cpp

  \brief Utility routines for the TerraLib Application Framework module.
*/

// TerraLib
#include "../../common/UserApplicationSettings.h"
#include "../../dataaccess/serialization/xml/Serializer.h"
#include "../../maptools/AbstractLayer.h"
#include "../../plugin/PluginManager.h"
#include "../../plugin/PluginInfo.h"
#include "../../serialization/maptools/Layer.h"
#include "../../xml/Reader.h"
#include "../../xml/ReaderFactory.h"
#include "../../xml/Writer.h"
#include "ApplicationController.h"
#include "ApplicationPlugins.h"
#include "Exception.h"
#include "Project.h"
#include "Utils.h"

// STL
#include <cassert>
#include <fstream>
#include <memory>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>

//Qt
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QToolBar>

te::qt::af::Project* te::qt::af::ReadProject(const std::string& uri)
{
  boost::filesystem::path furi(uri);
  
  if (!boost::filesystem::exists(furi) || !boost::filesystem::is_regular_file(furi))   
    throw Exception((boost::format(TR_QT_AF("Could not read project file: %1%.")) % uri).str());
  
  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());
  
  xmlReader->read(uri);
  
  if(!xmlReader->next())
    throw Exception((boost::format(TR_QT_AF("Could not read project information in the file: %1%.")) % uri).str());
  
  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw Exception((boost::format(TR_QT_AF("Error reading the document %1%, the start element wasn't found.")) % uri).str());
  
  if(xmlReader->getElementLocalName() != "Project")
    throw Exception((boost::format(TR_QT_AF("The first tag in the document %1% is not 'Project'.")) % uri).str());
  
  Project* proj = ReadProject(*xmlReader);
  
  proj->setFileName(uri);
  
  return proj;

}

te::qt::af::Project* te::qt::af::ReadProject(te::xml::Reader& reader)
{
  std::auto_ptr<Project> project(new Project);

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Title");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  project->setTitle(reader.getElementValue());
  reader.next(); // End element

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Author");

  reader.next();

  if(reader.getNodeType() == te::xml::VALUE)
  {
    project->setAuthor(reader.getElementValue());
    reader.next(); // End element
  }

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ComponentList");
  reader.next(); // End element
  reader.next(); // next after </ComponentList>

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "LayerList");

  reader.next();

  const te::serialize::Layer& lserial = te::serialize::Layer::getInstance();

  while((reader.getNodeType() != te::xml::END_ELEMENT) &&
        (reader.getElementLocalName() != "LayerList"))
  {
    te::map::AbstractLayerPtr layer(lserial.read(reader));

    if(layer.get() == 0)
      break;

    project->add(layer);
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  assert(reader.getElementLocalName() == "LayerList");

  reader.next();
  assert((reader.getNodeType() == te::xml::END_ELEMENT) || (reader.getNodeType() == te::xml::END_DOCUMENT));
  assert(reader.getElementLocalName() == "Project");

  project->projectChanged(false);

  return project.release();
}

void te::qt::af::Save(const te::qt::af::Project& project, const std::string& uri)
{
  std::ofstream fout(uri.c_str(), std::ios_base::trunc);

  te::xml::Writer w(fout);

  Save(project, w);

  fout.close();
}

void te::qt::af::Save(const te::qt::af::Project& project, te::xml::Writer& writer)
{
  writer.writeStartDocument("UTF-8", "no");

  writer.writeStartElement("Project");

  QDir schemaLocation(TE_SCHEMA_LOCATION);

  writer.writeAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance");
  writer.writeAttribute("xmlns:te_da", "http://www.terralib.org/schemas/dataaccess");
  writer.writeAttribute("xmlns:te_map", "http://www.terralib.org/schemas/maptools");
  writer.writeAttribute("xmlns:te_qt_af", "http://www.terralib.org/schemas/common/af");

  writer.writeAttribute("xmlns:se", "http://www.opengis.net/se");
  writer.writeAttribute("xmlns:ogc", "http://www.opengis.net/ogc");
  writer.writeAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");

  writer.writeAttribute("xmlns", "http://www.terralib.org/schemas/qt/af");
  writer.writeAttribute("xsd:schemaLocation", "http://www.terralib.org/schemas/qt/af " + schemaLocation.absolutePath().toStdString() + "/qt/af/project.xsd");
  writer.writeAttribute("version", TERRALIB_STRING_VERSION);

  writer.writeElement("Title", project.getTitle());
  writer.writeElement("Author", project.getAuthor());

  writer.writeStartElement("ComponentList");
  writer.writeEndElement("ComponentList");

  writer.writeStartElement("te_map:LayerList");

  const te::serialize::Layer& lserial = te::serialize::Layer::getInstance();

  for(std::list<te::map::AbstractLayerPtr>::const_iterator it = project.getLayers().begin();
      it != project.getLayers().end();
      ++it)
    lserial.write(it->get(), writer);

  writer.writeEndElement("te_map:LayerList");

  writer.writeEndElement("Project");
}

void te::qt::af::UpdateUserSettings(const QStringList& prjFiles, const QStringList& prjTitles, const std::string& userConfigFile)
{
  // Recent projects
  //----------------
  if(prjFiles.empty())
    return;

  boost::property_tree::ptree& p = te::common::UserApplicationSettings::getInstance().getAllSettings();

  p.get_child("UserSettings.MostRecentProject.<xmlattr>.xlink:href").put_value(prjFiles.at(0).toStdString());
  p.get_child("UserSettings.MostRecentProject.<xmlattr>.title").put_value(prjTitles.at(0).toStdString());

  if(prjFiles.size() > 1)
  {
    boost::property_tree::ptree recPrjs;

    for(int i=1; i<prjFiles.size(); i++)
    {
      boost::property_tree::ptree prj;

      prj.add("<xmlattr>.xlink:href", prjFiles.at(i).toStdString());
      prj.add("<xmlattr>.title", prjTitles.at(i).toStdString());

      recPrjs.add_child("Project", prj);
    }

    p.put_child("UserSettings.RecentProjects", recPrjs);
  }

  //Enabled plugins
  //----------------
  boost::property_tree::ptree plgs;
  std::vector<std::string> plugins;
  std::vector<std::string>::iterator it;
  te::plugin::PluginManager::getInstance().getPlugins(plugins);

  for(it=plugins.begin(); it!=plugins.end(); ++it)
    if(te::plugin::PluginManager::getInstance().isLoaded(*it))
    {
      boost::property_tree::ptree plg;
      plg.put_value(*it);
      plgs.add_child("Plugin", plg);
    }

  p.put_child("UserSettings.EnabledPlugins", plgs);

  te::common::UserApplicationSettings::getInstance().changed();
}

void te::qt::af::SaveDataSourcesFile()
{
  std::string fileName = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.DataSourcesFile");

  if(fileName.empty())
    return;

  te::serialize::xml::Save(fileName);
}

void te::qt::af::UpdateApplicationPlugins()
{
  ApplicationPlugins::getInstance().getAllSettings().get_child("Plugins").erase("Plugin");
  boost::property_tree::ptree& p = ApplicationPlugins::getInstance().getAllSettings();

  std::vector<std::string> plugins;
  std::vector<std::string>::iterator it;
  te::plugin::PluginManager::getInstance().getPlugins(plugins);

  for(it=plugins.begin(); it!=plugins.end(); ++it)
  {
    const te::plugin::PluginInfo& info = te::plugin::PluginManager::getInstance().getPlugin(*it);
    boost::property_tree::ptree plg;

    std::string plgFileName = info.m_folder + "/" + info.m_name + ".teplg";

    plg.add("Name", info.m_name);
    plg.add("Path.<xmlattr>.xlink:href", plgFileName);

    p.add_child("Plugins.Plugin", plg);
  }

  // Store the file.
  boost::property_tree::xml_writer_settings<char> settings('\t', 1);
  boost::property_tree::write_xml(ApplicationPlugins::getInstance().getFileName(), p, std::locale(), settings);
}

void AddToolbarAndActions(QToolBar* bar, QSettings& sett)
{
  sett.beginGroup(bar->objectName());

  sett.setValue("name", bar->objectName());

  sett.beginWriteArray("Actions");

  QList<QAction*> acts = bar->actions();

  for(int i=0; i<acts.size(); i++)
  {
    sett.setArrayIndex(i);
    sett.setValue("action", acts.at(i)->objectName());
  }

  sett.endArray();

  sett.endGroup();
}

void te::qt::af::UpdateToolBarsInTheSettings()
{
  std::vector<QToolBar*> bars = te::qt::af::ApplicationController::getInstance().getToolBars();
  std::vector<QToolBar*>::const_iterator it;
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("toolbars");

  for (it = bars.begin(); it != bars.end(); ++it)
  {
    QToolBar* bar = *it;

    sett.remove(bar->objectName());

    AddToolbarAndActions(bar, sett);
  }

  sett.endGroup();
}

void te::qt::af::AddToolBarToSettings(QToolBar* bar)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("toolbars");

  AddToolbarAndActions(bar, sett);

  sett.endGroup();
}

void te::qt::af::RemoveToolBarFromSettings(QToolBar* bar)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("toolbars");
  sett.remove(bar->objectName());
  sett.endGroup();
}

std::vector<QToolBar*> te::qt::af::ReadToolBarsFromSettings(QWidget* barsParent)
{
  std::vector<QToolBar*> bars;

  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("toolbars");
  QStringList lst = sett.childGroups();

  QStringList::iterator it;

  for(it=lst.begin(); it != lst.end(); ++it)
  {
    QString gr = *it;

    sett.beginGroup(gr);

    QString grName = sett.value("name").toString();

    int size = sett.beginReadArray("Actions");

    QToolBar* toolbar = new QToolBar(barsParent);
    toolbar->setObjectName(grName);
    toolbar->setWindowTitle(grName);

    for(int i=0; i<size; i++)
    {
      sett.setArrayIndex(i);
      QString act = sett.value("action").toString();
      QAction* a = ApplicationController::getInstance().findAction(act);
      
      if(a != 0)
        toolbar->addAction(a);
    }

    sett.endArray();
    sett.endGroup();

    bars.push_back(toolbar);
  }

  sett.endGroup();

  return bars;
}

void te::qt::af::GetProjectInformationsFromSettings(QString& defaultAuthor, int& maxSaved)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("projects");
  defaultAuthor = sett.value("default author").toString();
  maxSaved = sett.value("maximum saved").toInt();
  sett.endGroup();
}

void te::qt::af::SaveProjectInformationsOnSettings(const QString& defaultAuthor, const int& maxSaved)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("projects");
  sett.setValue("default author", defaultAuthor);
  sett.setValue("maximum saved", maxSaved);
  sett.endGroup();
}

void te::qt::af::SaveLastDatasourceOnSettings(const QString& dsType)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.setValue("projects/last datasource used", dsType);
}

void te::qt::af::SaveOpenLastProjectOnSettings(bool openLast)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.setValue("projects/openLastDataSource", openLast);
}

QString te::qt::af::GetLastDatasourceFromSettings()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  return sett.value("projects/last datasource used").toString();
}

bool te::qt::af::GetOpenLastProjectFromSettings()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  QVariant variant = sett.value("projects/openLastDataSource");

  // If the option was never edited
  if(variant.isNull() || !variant.isValid())
    return true;

  return variant.toBool();
}

void te::qt::af::CreateDefaultSettings()
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
  sett.endArray();
  sett.endGroup();

  sett.beginGroup("Map Tool Bar");
  sett.setValue("name", "Map Tool Bar");
  sett.beginWriteArray("Actions");
  sett.setArrayIndex(0);
  sett.setValue("action", "Map.Draw");
  sett.setArrayIndex(1);
  sett.setValue("action", "Map.Zoom In");
  sett.setArrayIndex(2);
  sett.setValue("action", "Map.Zoom Out");
  sett.setArrayIndex(3);
  sett.setValue("action", "Map.Zoom Area");
  sett.setArrayIndex(4);
  sett.setValue("action", "Map.Pan");
  sett.setArrayIndex(5);
  sett.setValue("action", "Map.Zoom Extent");
  sett.setArrayIndex(6);
  sett.setValue("action", "Map.Info");
  sett.setArrayIndex(7);
  sett.setValue("action", "Map.Selection");
  sett.endArray();
  sett.endGroup();

  sett.endGroup();

  sett.beginGroup("projects");

  sett.setValue("default author", "");
  sett.setValue("maximum saved", "8");

  sett.endGroup();
}

QString te::qt::af::UnsavedStar(const QString windowTitle, bool isUnsaved)
{
  QString result(windowTitle);

  if(isUnsaved)
  {
    if(result.at(result.count()-1) != '*')
      result += "*";
  }
  else
  {
    if(result.at(result.count()-1) == '*')
      result.remove((result.count()-1), 1);
  }

  return result;
}

QColor te::qt::af::GetDefaultDisplayColorFromSettings()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  QString hexColor = sett.value("display/defaultDisplayColor").toString();  
  QColor defaultColor;
  defaultColor.setNamedColor(hexColor);
  if(!defaultColor.isValid())
    return Qt::white;

  return defaultColor;
}

QString te::qt::af::GetStyleSheetFromColors(QColor primaryColor, QColor secondaryColor)
{
  QString sty("alternate-background-color: ");
  sty += "rgb(" + QString::number(secondaryColor.red()) + ", " + QString::number(secondaryColor.green());
  sty += ", " + QString::number(secondaryColor.blue()) + ")";
  sty += ";background-color: rgb(" + QString::number(primaryColor.red()) + ", " + QString::number(primaryColor.green());
  sty += ", " + QString::number(primaryColor.blue()) + ");";

  return sty;
}

QString te::qt::af::GetStyleSheetFromSettings()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  //bool isChecked = sett.value("table/tableAlternateColors").toBool();
  QColor pColor;
  pColor.setNamedColor(sett.value("table/primaryColor").toString());
  QColor sColor;
  sColor.setNamedColor(sett.value("table/secondaryColor").toString());

  if(!pColor.isValid())
    pColor = Qt::white;
  if(!sColor.isValid())
    sColor = Qt::white;

  return GetStyleSheetFromColors(pColor, sColor);
}

bool te::qt::af::GetAlternateRowColorsFromSettings()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  bool isChecked = sett.value("table/tableAlternateColors").toBool();

  return isChecked;
}

void te::qt::af::AddActionToCustomToolbars(QAction* act)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("toolbars");
  QStringList lst = sett.childGroups();
  QStringList::iterator it;

  for(it=lst.begin(); it!=lst.end(); ++it)
  {
    int size = sett.beginReadArray(*it+"/Actions");

    for(int i=0; i<size; i++)
    {
      sett.setArrayIndex(i);

      QString v = sett.value("action").toString(); 

      if (v == act->objectName())
      {
        ApplicationController::getInstance().getToolBar(*it)->addAction(act);
        break;
      }
    }

    sett.endArray();
  }

  sett.endGroup();
}
