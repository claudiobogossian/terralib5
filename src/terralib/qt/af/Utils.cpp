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
#include "../../maptools/AbstractLayer.h"
#include "../../serialization/maptools/Layer.h"
#include "../../xml/Reader.h"
#include "../../xml/ReaderFactory.h"
#include "../../xml/Writer.h"
#include "Exception.h"
#include "Project.h"
#include "Utils.h"

// STL
#include <cassert>
#include <fstream>
#include <memory>

// Boost
#include <boost/format.hpp>

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

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Author");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  project->setAuthor(reader.getElementValue());

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ComponentList");

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
  std::ofstream fout(uri, std::ios_base::trunc);

  te::xml::Writer w(fout);

  Save(project, w);

  fout.close();
}

void te::qt::af::Save(const te::qt::af::Project& project, te::xml::Writer& writer)
{
  writer.writeStartDocument("UTF-8", "no");

  writer.writeStartElement("Project");

  writer.writeAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance");
  writer.writeAttribute("xmlns", "http://www.terralib.org/schemas/af");
  writer.writeAttribute("xsd:schemaLocation", "http://www.terralib.org/schemas/af ../../myschemas/terralib/af/project.xsd");
  writer.writeAttribute("version", "5.0.0");
  writer.writeAttribute("release", "2011-01-01");

  writer.writeElement("Title", project.getTitle());
  writer.writeElement("Author", project.getAuthor());

  writer.writeStartElement("LayerList");

  const te::serialize::Layer& lserial = te::serialize::Layer::getInstance();

  for(std::list<te::map::AbstractLayer*>::const_iterator it = project.getLayers().begin();
      it != project.getLayers().end();
      ++it)
    lserial.write(*it, writer);

  writer.writeEndElement("LayerList");

  writer.writeEndElement("Project");
}

