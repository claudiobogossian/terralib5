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
  \file terralib/qt/af/Utils.cpp

  \brief Utility routines for the TerraLib Application Framework module.
*/

// Boost
#include <boost/foreach.hpp> // Boost => don't change this include order, otherwise you may have compiling problems! 

// TerraLib
#include "../../common/BoostUtils.h"
#include "../../common/PlatformUtils.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/UserApplicationSettings.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/serialization/xml/Serializer.h"
#include "../../maptools/AbstractLayer.h"
#include "../../plugin/PluginManager.h"
#include "../../plugin/PluginInfo.h"
#include "../../maptools/serialization/xml/Layer.h"
#include "../../xml/AbstractWriter.h"
#include "../../xml/AbstractWriterFactory.h"
#include "../../xml/Reader.h"
#include "../../xml/ReaderFactory.h"
#include "../../Version.h"
#include "ApplicationController.h"
#include "Exception.h"
#include "Project.h"
#include "Utils.h"
#include "XMLFormatter.h"

// STL
#include <cassert>
#include <fstream>
#include <memory>

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

// Qt
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QString>
#include <QTextStream>
#include <QApplication>
#include <QAction>
#include <QMainWindow>
#include <QMessageBox>
#include <QToolBar>


te::qt::af::Project* te::qt::af::ReadProject(const std::string& uri)
{
  boost::filesystem::path furi(uri);
  
  if (!boost::filesystem::exists(furi) || !boost::filesystem::is_regular_file(furi))   
    throw Exception((boost::format(TE_TR("Could not read project file: %1%.")) % uri).str());
  
  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());
  xmlReader->setValidationScheme(false);
  
  xmlReader->read(uri);
  
  if(!xmlReader->next())
    throw Exception((boost::format(TE_TR("Could not read project information in the file: %1%.")) % uri).str());
  
  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw Exception((boost::format(TE_TR("Error reading the document %1%, the start element wasn't found.")) % uri).str());
  
  if(xmlReader->getElementLocalName() != "Project")
    throw Exception((boost::format(TE_TR("The first tag in the document %1% is not 'Project'.")) % uri).str());
  
  Project* proj = ReadProject(*xmlReader);
  
  proj->setFileName(uri);
  
  XMLFormatter::format(proj, false);

  proj->setProjectAsChanged(false);

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

  //read data source list from this project
  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSourceList");

  reader.next();

  // DataSourceList contract form
  if(reader.getNodeType() == te::xml::END_ELEMENT &&
     reader.getElementLocalName() == "DataSourceList")
  {
    reader.next();
  }

  while((reader.getNodeType() == te::xml::START_ELEMENT) &&
        (reader.getElementLocalName() == "DataSource"))
  {
    te::da::DataSourceInfoPtr ds(te::serialize::xml::ReadDataSourceInfo(reader));
    te::da::DataSourceInfoManager::getInstance().add(ds);
  }

  //end read data source list

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ComponentList");
  reader.next(); // End element
  reader.next(); // next after </ComponentList>

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "LayerList");

  reader.next();

  const te::map::serialize::Layer& lserial = te::map::serialize::Layer::getInstance();

  // Read the layers
  while((reader.getNodeType() != te::xml::END_ELEMENT) &&
        (reader.getElementLocalName() != "LayerList"))
  {
    te::map::AbstractLayerPtr layer(lserial.read(reader));

    assert(layer.get());

    project->add(layer);
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  assert(reader.getElementLocalName() == "LayerList");

  reader.next();
  assert((reader.getNodeType() == te::xml::END_ELEMENT) || (reader.getNodeType() == te::xml::END_DOCUMENT));
  assert(reader.getElementLocalName() == "Project");

  project->setProjectAsChanged(false);

  return project.release();
}

void te::qt::af::Save(const te::qt::af::Project& project, const std::string& uri)
{

  std::auto_ptr<te::xml::AbstractWriter> writer(te::xml::AbstractWriterFactory::make());

  writer->setURI(uri);

  Project p(project);

  Save(p, *writer.get());

  /* old way
  std::ofstream fout(uri.c_str(), std::ios_base::trunc);

  te::xml::Writer w(fout);

  Project p(project);

  XMLFormatter::format(&p, true);

  Save(p, w);

  XMLFormatter::format(&p, false);

  fout.close();*/
}

void te::qt::af::Save(const te::qt::af::Project& project, te::xml::AbstractWriter& writer)
{
  std::string schema_loc = te::common::FindInTerraLibPath("share/terralib/schemas/terralib/qt/af/project.xsd");

  writer.writeStartDocument("UTF-8", "no");
  
  writer.writeStartElement("Project");

  boost::replace_all(schema_loc, " ", "%20");

  writer.writeAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance");
  writer.writeAttribute("xmlns:te_da", "http://www.terralib.org/schemas/dataaccess");
  writer.writeAttribute("xmlns:te_map", "http://www.terralib.org/schemas/maptools");
  writer.writeAttribute("xmlns:te_qt_af", "http://www.terralib.org/schemas/common/af");

  writer.writeAttribute("xmlns:se", "http://www.opengis.net/se");
  writer.writeAttribute("xmlns:ogc", "http://www.opengis.net/ogc");
  writer.writeAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");

  writer.writeAttribute("xmlns", "http://www.terralib.org/schemas/qt/af");
  writer.writeAttribute("xsd:schemaLocation", "http://www.terralib.org/schemas/qt/af " + schema_loc);
  writer.writeAttribute("version", TERRALIB_VERSION_STRING);

  writer.writeElement("Title", project.getTitle());
  writer.writeElement("Author", project.getAuthor());

  //write data source list 
  writer.writeStartElement("te_da:DataSourceList");

  writer.writeAttribute("xmlns:te_common", "http://www.terralib.org/schemas/common");

  te::da::DataSourceInfoManager::iterator itBegin = te::da::DataSourceInfoManager::getInstance().begin();
  te::da::DataSourceInfoManager::iterator itEnd = te::da::DataSourceInfoManager::getInstance().end();
  te::da::DataSourceInfoManager::iterator it;

  for(it=itBegin; it!=itEnd; ++it)
  {
    bool ogrDsrc = it->second->getAccessDriver() == "OGR";

    writer.writeStartElement("te_da:DataSource");

    writer.writeAttribute("id", it->second->getId());
    writer.writeAttribute("type", it->second->getType());
    writer.writeAttribute("access_driver", it->second->getAccessDriver());

    writer.writeStartElement("te_da:Title");
    writer.writeValue((!ogrDsrc) ? it->second->getTitle() : te::common::ConvertLatin1UTFString(it->second->getTitle()));
    writer.writeEndElement("te_da:Title");

    writer.writeStartElement("te_da:Description");
    writer.writeValue((!ogrDsrc) ? it->second->getDescription() : te::common::ConvertLatin1UTFString(it->second->getDescription()));
    writer.writeEndElement("te_da:Description");

    writer.writeStartElement("te_da:ConnectionInfo");
    std::map<std::string, std::string> info = it->second->getConnInfo();
    std::map<std::string, std::string>::iterator conIt;

    for(conIt=info.begin(); conIt!=info.end(); ++conIt)
    {
      writer.writeStartElement("te_common:Parameter");

      writer.writeStartElement("te_common:Name");
      writer.writeValue(conIt->first);
      writer.writeEndElement("te_common:Name");

      writer.writeStartElement("te_common:Value");
      writer.writeValue((ogrDsrc && (conIt->first == "URI" || conIt->first == "SOURCE")) ? te::common::ConvertLatin1UTFString(conIt->second) : conIt->second);
      writer.writeEndElement("te_common:Value");

      writer.writeEndElement("te_common:Parameter");
    }
    writer.writeEndElement("te_da:ConnectionInfo");

    writer.writeEndElement("te_da:DataSource");
  }

  writer.writeEndElement("te_da:DataSourceList");
  //end write

  writer.writeStartElement("ComponentList");
  writer.writeEndElement("ComponentList");

  writer.writeStartElement("te_map:LayerList");

  const te::map::serialize::Layer& lserial = te::map::serialize::Layer::getInstance();

  for(std::list<te::map::AbstractLayerPtr>::const_iterator it = project.getTopLayers().begin();
      it != project.getTopLayers().end();
      ++it)
    lserial.write(it->get(), writer);

  writer.writeEndElement("te_map:LayerList");

  writer.writeEndElement("Project");

  writer.writeToFile();

}

void te::qt::af::UpdateUserSettings(const QStringList& prjFiles, const QStringList& prjTitles, const std::string& userConfigFile)
{
  QSettings user_settings(QSettings::IniFormat,
                          QSettings::UserScope,
                          QApplication::instance()->organizationName(),
                          QApplication::instance()->applicationName());
  
// save recent projects
  if(!prjFiles.empty() && !prjTitles.empty() && (prjFiles.size() == prjTitles.size()))
  {
    user_settings.setValue("projects/most_recent/path", prjFiles.at(0));
    user_settings.setValue("projects/most_recent/title", prjTitles.at(0));
    
    if(prjFiles.size() > 1)
    {
      user_settings.beginGroup("projects");
      
      user_settings.beginWriteArray("recents");
      
      for(int i = 1; i != prjFiles.size(); ++i)
      {
        user_settings.setArrayIndex(i - 1);
        user_settings.setValue("projects/path", prjFiles.at(i));
        user_settings.setValue("projects/title", prjTitles.at(i));
      }
      
      user_settings.endArray();
      
      user_settings.endGroup();
    }
  }
  
// save enabled plugins
  user_settings.remove("plugins/enabled");
  
  user_settings.beginGroup("plugins");
  user_settings.remove("enabled");
  user_settings.remove("unloaded");
  user_settings.remove("broken");
  
  user_settings.beginWriteArray("enabled");
  
  std::vector<std::string> plugins = te::plugin::PluginManager::getInstance().getPlugins();
  
  int aidx = 0;
  
  for(std::size_t i = 0; i != plugins.size(); ++i)
  {
    if(!te::plugin::PluginManager::getInstance().isLoaded(plugins[i]))
      continue;
    
    user_settings.setArrayIndex(aidx++);
    
    user_settings.setValue("name", plugins[i].c_str());
  }

  user_settings.endArray();

  // save unloaded plugins
  user_settings.beginWriteArray("unloaded");

  int unloadedidx = 0;

  for (std::size_t i = 0; i != plugins.size(); ++i)
  {
    if (!te::plugin::PluginManager::getInstance().isUnloadedPlugin(plugins[i]))
      continue;

    user_settings.setArrayIndex(unloadedidx++);
    user_settings.setValue("name", plugins[i].c_str());
  }

  user_settings.endArray();

  // save broken plugins
  user_settings.beginWriteArray("broken");

  int brokenidx = 0;

  for (std::size_t i = 0; i != plugins.size(); ++i)
  {
    if (!te::plugin::PluginManager::getInstance().isBrokenPlugin(plugins[i]))
      continue;

    user_settings.setArrayIndex(brokenidx++);
    user_settings.setValue("name", plugins[i].c_str());
  }
  
  user_settings.endArray();
  
  user_settings.endGroup();
}

void te::qt::af::SaveDataSourcesFile()
{
  QSettings usettings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  
  QVariant fileName = usettings.value("data_sources/data_file");

  if(fileName.isNull())
  {
    // Fred: revisar
    const QString& udir=""; //= ApplicationController::getInstance().getUserDataDir();
    
    fileName = udir + "/" + QString(TERRALIB_APPLICATION_DATASOURCE_FILE_NAME);
    
    usettings.setValue("data_sources/data_file", fileName);
  }

  te::serialize::xml::Save(fileName.toString().toStdString());
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
  // Fred: revisar

  std::vector<QToolBar*> bars;// = te::qt::af::ApplicationController::getInstance().getToolBars();
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

      if(act == "")
      {
        toolbar->addSeparator();
      }
      else
      {
        // Fred: revisar

        QAction* a;// = ApplicationController::getInstance().findAction(act);
      
        if(a != 0)
          toolbar->addAction(a);
      }
    }

    sett.endArray();
    sett.endGroup();

    bars.push_back(toolbar);
  }

  sett.endGroup();

  return bars;
}

void te::qt::af::SaveState(QMainWindow* mainWindow)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  
  sett.beginGroup("mainWindow");
  sett.setValue("geometry", mainWindow->saveGeometry());
  sett.setValue("windowState", mainWindow->saveState());
  sett.endGroup();
}

void te::qt::af::RestoreState(QMainWindow* mainWindow)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("mainWindow");
  mainWindow->restoreGeometry(sett.value("geometry").toByteArray());
  mainWindow->restoreState(sett.value("windowState").toByteArray());
  sett.endGroup();
}

void te::qt::af::GetProjectInformationsFromSettings(QString& defaultAuthor, int& maxSaved)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("projects");
  defaultAuthor = sett.value("author_name").toString();
  maxSaved = sett.value("recents_history_size").toInt();
  sett.endGroup();
}

void te::qt::af::SaveProjectInformationsOnSettings(const QString& defaultAuthor, const int& maxSaved)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("projects");
  sett.setValue("author_name", defaultAuthor);
  sett.setValue("recents_history_size", maxSaved);
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
        // Fred: revisar
//        ApplicationController::getInstance().getToolBar(*it)->addAction(act);
        break;
      }
    }

    sett.endArray();
  }

  sett.endGroup();
}

std::vector<std::string> te::qt::af::GetPluginsFiles()
{
  std::vector<std::string> res;

  QStringList filters;

  filters << "*.teplg";

  QDir d(te::common::FindInTerraLibPath("share/terralib/plugins").c_str());

  QFileInfoList files = d.entryInfoList(filters, QDir::Files);

  foreach(QFileInfo file, files)
  {
    res.push_back(file.absoluteFilePath().toStdString());
  }

  return res;
}

std::vector<std::string> te::qt::af::GetDefaultPluginsNames()
{
  std::vector<std::string> res;

// Finding the Default plugins file.
  // Fred: revisar
  std::string pluginsPath; //= te::qt::af::ApplicationController::getInstance().getAppPluginsPath().toStdString();

  if (pluginsPath == "")
    return res;

// Reading JSON
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json(pluginsPath, pt);

  BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("plugins"))
  {
    res.push_back(v.second.get<std::string>("plugin"));
  }

  return res;
}

std::vector<std::string> te::qt::af::GetPluginsNames(const std::vector<std::string>& plgFiles)
{
  std::vector<std::string> res;
  std::vector<std::string>::const_iterator it;

  for(it=plgFiles.begin(); it!=plgFiles.end(); ++it)
  {
    boost::property_tree::ptree p;
    boost::property_tree::read_xml(*it, p, boost::property_tree::xml_parser::trim_whitespace);

    res.push_back(p.get<std::string>("PluginInfo.Name"));
  }

  return res;
}

QString te::qt::af::GetDateTime()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  return sett.value("configuration/generation").toString();
}

void te::qt::af::SetDateTime(const QString& dateTime)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.setValue("configuration/generation", dateTime);
}

QString te::qt::af::GetDefaultConfigFileOutputDir()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  QFileInfo info(sett.fileName());

  return info.absolutePath();
}

void te::qt::af::UpdateUserSettingsFile(const QString& fileName, const bool& removeOlder)
{
  QFileInfo info(fileName);
  te::common::UserApplicationSettings& usrSett = te::common::UserApplicationSettings::getInstance();
  te::common::SystemApplicationSettings& appSett = te::common::SystemApplicationSettings::getInstance();

  if(info.exists())
    info.dir().remove(info.fileName());

  std::string olderFile = appSett.getValue("Application.UserSettingsFile.<xmlattr>.xlink:href");

  appSett.setValue("Application.UserSettingsFile.<xmlattr>.xlink:href", fileName.toStdString());

  if(removeOlder)
  {
    info.setFile(olderFile.c_str());
    info.dir().remove(info.fileName());
  }

  info.setFile(fileName);

  if(!info.exists())
  {
#if BOOST_VERSION > 105600
    boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
#else
    boost::property_tree::xml_writer_settings<char> settings('\t', 1);
#endif
    boost::property_tree::write_xml(fileName.toStdString(), usrSett.getAllSettings(), std::locale(), settings);
  }

  usrSett.load(fileName.toStdString());
}

void te::qt::af::WriteDefaultProjectFile(const QString& fileName)
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

QString te::qt::af::GetGenerationDate()
{
  QString fileName = qApp->applicationDirPath() + "/../.generated";

  QFile f(fileName);
  if (!f.open(QFile::ReadOnly | QFile::Text))
    return "";

  QTextStream in(&f);
  QString s = in.readAll();

  f.close();

  return s;
}

QString te::qt::af::GetWindowTitle(const te::qt::af::Project& project)
{
  // Fred: revisar
  QString title;// = te::qt::af::ApplicationController::getInstance().getAppTitle() + " - ";
  title += TE_TR("Project:");
  title += " ";
  title += project.getTitle().c_str();
  title += " - ";

  boost::filesystem::path p(project.getFileName());

  std::string filename = p.filename().string();

  title += filename.c_str();

  return title;
}

QString te::qt::af::GetExtensionFilter()
{
  // Fred: revisar
  QString appName;// = te::qt::af::ApplicationController::getInstance().getAppName();
  QString appProjectExtension;// = te::qt::af::ApplicationController::getInstance().getAppProjectExtension();
  QString extensionFilter = appName;
  extensionFilter += QString(" (*.");
  extensionFilter += appProjectExtension + ")";

  return extensionFilter;
}
