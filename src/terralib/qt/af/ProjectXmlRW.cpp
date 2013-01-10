/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file ProjectXmlRW.cpp

  \brief Application project reader/writer.
*/

// TerraLib
#include <terralib/xml.h>
#include <terralib/xml/Writer.h>
#include <terralib/maptools.h>
#include <terralib/serialization/maptools/Layer.h>
#include "ProjectXmlRW.h"
#include "Project.h"

// STL
#include <string>
#include <iostream>
#include <fstream>

namespace aff = te::qt::af;

te::qt::af::ProjectXmlRW::ProjectXmlRW()
{

}

te::qt::af::ProjectXmlRW::~ProjectXmlRW()
{

}

te::qt::af::Project* te::qt::af::ProjectXmlRW::read(std::string path)
{
  aff::Project* pro = new aff::Project();

  std::auto_ptr<te::xml::Reader> reader(te::xml::ReaderFactory::make("XERCES"));

  reader->read(path);

  reader->next();
  assert(reader->getNodeType() == te::xml::START_ELEMENT);
  assert(reader->getElementLocalName() == "Project");

  reader->next();
  assert(reader->getNodeType() == te::xml::START_ELEMENT);
  assert(reader->getElementLocalName() == "Title");

  reader->next();
  assert(reader->getNodeType() == te::xml::VALUE);
  std::string title = reader->getElementValue();

  reader->next();
  assert(reader->getNodeType() == te::xml::START_ELEMENT);
  assert(reader->getElementLocalName() == "Author");

  reader->next();
  assert(reader->getNodeType() == te::xml::VALUE);
  std::string author = reader->getElementValue();

  reader->next();
  assert(reader->getNodeType() == te::xml::START_ELEMENT);
  assert(reader->getElementLocalName() == "ComponentList");

  reader->next();
  assert(reader->getNodeType() == te::xml::START_ELEMENT);
  assert(reader->getElementLocalName() == "LayerList");

  while(reader->next())
    pro->addLayer(te::serialize::Layer::getInstance().read(*reader));
  
  pro->setTitle(title);
  pro->setAuthor(author);

  return pro;
}

void te::qt::af::ProjectXmlRW::write(te::qt::af::Project* project, std::string path)
{
  std::ofstream stream(path.c_str());

  assert(stream.good());

  te::xml::Writer writer(stream);
  
  writer.writeStartDocument("UTF-8", "no");
  
  writer.writeStartElement("Project");

  writer.writeElement("Title", project->getTitle());

  writer.writeElement("Author", project->getAuthor());

  writer.writeStartElement("ComponentList");
  writer.writeEndElement("ComponentList");

  writer.writeStartElement("LayerList");

  for(size_t i = 0; i < project->getLayers().size(); i++)
    te::serialize::Layer::getInstance().write(project->getLayers()[i], writer);

  writer.writeEndElement("LayerList");

  writer.writeEndElement("Project");

}