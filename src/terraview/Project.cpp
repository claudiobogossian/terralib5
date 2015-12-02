///*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

//    This file is part of the TerraLib - a Framework for building GIS enabled applications.

//    TerraLib is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License,
//    or (at your option) any later version.

//    TerraLib is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU Lesser General Public License for more details.

//    You should have received a copy of the GNU Lesser General Public License
//    along with TerraLib. See COPYING. If not, write to
//    TerraLib Team at <terralib-team@terralib.org>.
// */

///*!
//  \file Project.cpp

//  \brief This class models the concept of a project for the TerraView application.
//*/

// TerraLib
#include "Project.h"

#include "../terralib/common/BoostUtils.h"
#include "../terralib/common/PlatformUtils.h"
#include "../terralib/dataaccess/datasource/DataSourceInfoManager.h"
#include "../terralib/dataaccess/serialization/xml/Serializer.h"
#include "../terralib/maptools/serialization/xml/Layer.h"
#include "../terralib/xml/AbstractWriter.h"
#include "../terralib/xml/AbstractWriterFactory.h"
#include "../terralib/xml/Reader.h"
#include "../terralib/xml/ReaderFactory.h"
#include "../terralib/Version.h"

// Boost
#include <boost/algorithm/string/replace.hpp>

// Qt
#include <QFileInfo>


void SaveProject(const ProjectMetadata& proj, const std::list<te::map::AbstractLayerPtr>& layers)
{
  std::auto_ptr<te::xml::AbstractWriter> writer(te::xml::AbstractWriterFactory::make());

  writer->setURI(proj.m_fileName.toStdString());

  std::string schema_loc = te::common::FindInTerraLibPath("share/terralib/schemas/terralib/qt/af/project.xsd");

  writer->writeStartDocument("UTF-8", "no");

  writer->writeStartElement("Project");

  boost::replace_all(schema_loc, " ", "%20");

  writer->writeAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance");
  writer->writeAttribute("xmlns:te_da", "http://www.terralib.org/schemas/dataaccess");
  writer->writeAttribute("xmlns:te_map", "http://www.terralib.org/schemas/maptools");
  writer->writeAttribute("xmlns:te_qt_af", "http://www.terralib.org/schemas/common/af");

  writer->writeAttribute("xmlns:se", "http://www.opengis.net/se");
  writer->writeAttribute("xmlns:ogc", "http://www.opengis.net/ogc");
  writer->writeAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");

  writer->writeAttribute("xmlns", "http://www.terralib.org/schemas/qt/af");
  writer->writeAttribute("xsd:schemaLocation", "http://www.terralib.org/schemas/qt/af " + schema_loc);
  writer->writeAttribute("version", TERRALIB_VERSION_STRING);

  writer->writeElement("Title", proj.m_title.toStdString());
  writer->writeElement("Author", proj.m_author.toStdString());

  //write data source list
  writer->writeStartElement("te_da:DataSourceList");

  writer->writeAttribute("xmlns:te_common", "http://www.terralib.org/schemas/common");

  te::da::DataSourceInfoManager::iterator itBegin = te::da::DataSourceInfoManager::getInstance().begin();
  te::da::DataSourceInfoManager::iterator itEnd = te::da::DataSourceInfoManager::getInstance().end();
  te::da::DataSourceInfoManager::iterator it;

  std::vector<std::string> dsIdVec;
  for (std::list<te::map::AbstractLayerPtr>::const_iterator itL = layers.begin(); itL != layers.end(); ++itL)
    dsIdVec.push_back(itL->get()->getDataSourceId());

  for(it=itBegin; it!=itEnd; ++it)
  {

    if (std::find(dsIdVec.begin(), dsIdVec.end(), it->second->getId()) == dsIdVec.end())
      continue;

    bool ogrDsrc = it->second->getAccessDriver() == "OGR";

    writer->writeStartElement("te_da:DataSource");

    writer->writeAttribute("id", it->second->getId());
    writer->writeAttribute("type", it->second->getType());
    writer->writeAttribute("access_driver", it->second->getAccessDriver());

    writer->writeStartElement("te_da:Title");
    writer->writeValue((!ogrDsrc) ? it->second->getTitle() : te::common::ConvertLatin1UTFString(it->second->getTitle()));
    writer->writeEndElement("te_da:Title");

    writer->writeStartElement("te_da:Description");
    writer->writeValue((!ogrDsrc) ? it->second->getDescription() : te::common::ConvertLatin1UTFString(it->second->getDescription()));
    writer->writeEndElement("te_da:Description");

    writer->writeStartElement("te_da:ConnectionInfo");
    std::map<std::string, std::string> info = it->second->getConnInfo();
    std::map<std::string, std::string>::iterator conIt;

    for(conIt=info.begin(); conIt!=info.end(); ++conIt)
    {
      writer->writeStartElement("te_common:Parameter");

      writer->writeStartElement("te_common:Name");
      writer->writeValue(conIt->first);
      writer->writeEndElement("te_common:Name");

      writer->writeStartElement("te_common:Value");
      writer->writeValue((ogrDsrc && (conIt->first == "URI" || conIt->first == "SOURCE")) ? te::common::ConvertLatin1UTFString(conIt->second) : conIt->second);
      writer->writeEndElement("te_common:Value");

      writer->writeEndElement("te_common:Parameter");
    }
    writer->writeEndElement("te_da:ConnectionInfo");

    writer->writeEndElement("te_da:DataSource");
  }

  writer->writeEndElement("te_da:DataSourceList");
  //end write

  writer->writeStartElement("ComponentList");
  writer->writeEndElement("ComponentList");

  writer->writeStartElement("te_map:LayerList");

  const te::map::serialize::Layer& lserial = te::map::serialize::Layer::getInstance();

  for(std::list<te::map::AbstractLayerPtr>::const_iterator itL = layers.begin(); itL != layers.end(); ++itL)
    lserial.write((*itL).get(), *writer.get());

  writer->writeEndElement("te_map:LayerList");

  writer->writeEndElement("Project");

  writer->writeToFile();
}


void LoadProject(const QString& projFile, ProjectMetadata& proj, std::list<te::map::AbstractLayerPtr>& layers)
{
  QFileInfo info(projFile);
  std::string fName = projFile.toStdString();

  if(!info.exists() || !info.isFile())
  {
    QString msg = QObject::tr("Could not read project file: ") + projFile;
    throw te::common::Exception(msg.toStdString());
  }

  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());
  xmlReader->setValidationScheme(false);

  xmlReader->read(fName);

  if(!xmlReader->next())
  {
    QString msg = QObject::tr("Could not read project information in the file: ") + projFile + ".";
    throw te::common::Exception(msg.toStdString());
  }

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
  {
    QString msg = QObject::tr("Error reading the document ") + projFile + QObject::tr(", the start element wasn't found.");
    throw te::common::Exception(msg.toStdString());
  }

  if(xmlReader->getElementLocalName() != "Project")
  {
    QString msg = QObject::tr("The first tag in the document ") + projFile + QObject::tr(" is not 'Project'.");
    throw te::common::Exception(msg.toStdString());
  }

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Title");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);
  proj.m_title = xmlReader->getElementValue().c_str();

  xmlReader->next(); // End element

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Author");

  xmlReader->next();

  if(xmlReader->getNodeType() == te::xml::VALUE)
  {
    proj.m_author = xmlReader->getElementValue().c_str();
    xmlReader->next(); // End element
  }

  //  //read data source list from this project
  xmlReader->next();

  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "DataSourceList");

  xmlReader->next();

  // DataSourceList contract form
  if(xmlReader->getNodeType() == te::xml::END_ELEMENT &&
     xmlReader->getElementLocalName() == "DataSourceList")
  {
    xmlReader->next();
  }

  while((xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "DataSource"))
  {
    te::da::DataSourceInfoPtr ds(te::serialize::xml::ReadDataSourceInfo(*(xmlReader.get())));
    te::da::DataSourceInfoManager::getInstance().add(ds);
  }

  //  //end read data source list

  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "ComponentList");
  xmlReader->next(); // End element
  xmlReader->next(); // next after </ComponentList>

  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "LayerList");

  xmlReader->next();

  const te::map::serialize::Layer& lserial = te::map::serialize::Layer::getInstance();

  // Read the layers
  while((xmlReader->getNodeType() != te::xml::END_ELEMENT) &&
        (xmlReader->getElementLocalName() != "LayerList"))
  {
    te::map::AbstractLayerPtr layer(lserial.read(*(xmlReader.get())));

    assert(layer.get());

    layers.push_back(layer);
  }

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
  assert(xmlReader->getElementLocalName() == "LayerList");

  xmlReader->next();
  assert((xmlReader->getNodeType() == te::xml::END_ELEMENT) || (xmlReader->getNodeType() == te::xml::END_DOCUMENT));
  assert(xmlReader->getElementLocalName() == "Project");

  proj.m_fileName = projFile;
}
