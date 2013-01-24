/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/serialization/raster/Raster.cpp
 
  \brief Support for serialization of Raster information.
*/

// TerraLib
#include "../../raster/Band.h"
#include "../../raster/Raster.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "BandProperty.h"
#include "Grid.h"
#include "Raster.h"

// STL
#include <cassert>
#include <fstream>

void te::serialize::ReadRasterInfo(std::map<std::string, std::string>& rinfo, te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Info");

  reader.next();

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "kvp")
  {
    std::string k = reader.getAttr(0);
    std::string v = reader.getAttr(1);

    rinfo[k] = v;

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }
}

void te::serialize::SaveRasterInfo(std::map<std::string, std::string>& rinfo, te::xml::Writer& writer)
{
  if(rinfo.empty())
    return;

  writer.writeStartElement("Info");

  std::map<std::string, std::string>::const_iterator it = rinfo.begin();
  std::map<std::string, std::string>::const_iterator itend = rinfo.end();

  while(it != itend)
  {
    writer.writeStartElement("kvp");
    writer.writeAttribute("k", it->first);
    writer.writeAttribute("v", it->second);
    writer.writeEndElement("kvp");

    ++it;
  }

  writer.writeEndElement("Info");
}

void te::serialize::Save(const te::rst::Raster* raster, const std::string& fileName)
{
  std::fstream ostr(fileName.c_str(), std::ios_base::out);

  Save(raster, ostr);

  ostr.close();
}

void te::serialize::Save(const te::rst::Raster* raster, std::ostream& ostr)
{
  te::xml::Writer w(ostr);

  Save(raster, w);
}

void te::serialize::Save(const te::rst::Raster* raster, te::xml::Writer& writer)
{
  writer.writeStartDocument("UTF-8", "no");

  writer.writeStartElement("Raster");
  
  writer.writeAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
  writer.writeAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance");
  writer.writeAttribute("xmlns", "http://www.terralib.org/schemas/raster");
  writer.writeAttribute("xsd:schemaLocation", "http://www.terralib.org/schemas/raster C:/Users/gribeiro/Documents/terralib5/trunk/myschemas/terralib/raster/raster.xsd");
  writer.writeAttribute("version", "5.0.0");
  writer.writeAttribute("release", "2011-01-01");

  writer.writeElement("Name", raster->getName());

  Save(raster->getGrid(), writer);

  writer.writeStartElement("Bands");

  for(std::size_t i = 0; i < raster->getNumberOfBands(); ++i)
    Save(raster->getBand(i)->getProperty(), writer);

  writer.writeEndElement("Bands");

  std::map<std::string, std::string> rinfo = raster->getInfo();

  SaveRasterInfo(rinfo, writer);

  writer.writeEndElement("Raster");
}

