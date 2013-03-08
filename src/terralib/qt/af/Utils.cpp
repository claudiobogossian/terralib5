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
#include "../../maptools/AbstractLayer.h"
#include "../../plugin/PluginManager.h"
#include "../../plugin/PluginInfo.h"
#include "../../serialization/maptools/Layer.h"
#include "../../serialization/dataaccess/DataSourceInfo.h"
#include "../../xml/Reader.h"
#include "../../xml/ReaderFactory.h"
#include "../../xml/Writer.h"
#include "ApplicationPlugins.h"
#include "Exception.h"
#include "Project.h"
#include "Utils.h"

// STL
#include <cassert>
#include <fstream>
#include <memory>

// Boost
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>

//Qt
#include <QtCore/QDir>

te::qt::af::Project* te::qt::af::ReadProject(const std::string& uri)
{
  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());

  xmlReader->read(uri);

  if(!xmlReader->next())
    throw Exception((boost::format(TR_QT_AF("Could not read project information in the file: %1%.")) % uri).str());

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw Exception((boost::format(TR_QT_AF("Error reading the document %1%, the start element wasn't found.")) % uri).str());

  if(xmlReader->getElementLocalName() != "Project")
    throw Exception((boost::format(TR_QT_AF("The first tag in the document %1% is not 'Project'.")) % uri).str());

  return ReadProject(*xmlReader);
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
  assert(reader.getNodeType() == te::xml::VALUE);
  project->setAuthor(reader.getElementValue());
  reader.next(); // End element

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ComponentList");
  reader.next(); // End element

  reader.next();

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() != "LayerList")
  {
    // pegar os valores de cada componente e enviar um sinal para que ele possa ler os valores
  }

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "LayerList");

  reader.next();

  const te::serialize::Layer& lserial = te::serialize::Layer::getInstance();

  while(reader.getNodeType() == te::xml::START_ELEMENT)
  {
    te::map::AbstractLayer* layer(lserial.read(reader));

    if(layer == 0)
      break;

    project->add(layer);
  }

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
  writer.writeAttribute("xmlns:te_map", "http://www.terralib.org/schemas/maptools");
  writer.writeAttribute("xmlns:te_qt_af", "http://www.terralib.org/schemas/common/af");
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

  te::serialize::Save(fileName);
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

  // ** Por algum motivo essa instrução não funciona: o arquivo fica todo errado, não sei porque!
  // ApplicationPlugins::getInstance().changed();
}
