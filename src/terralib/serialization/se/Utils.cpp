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
  \file terralib/serialization/se/Utils.cpp
 
  \brief Utility methods for Symbology serialization.
*/

// TerraLib
#include "../../se/Symbolizer.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../xlink/SimpleLink.h"
#include "Description.h"
#include "ParameterValue.h"
#include "SelectedChannel.h"
#include "Utils.h"

// STL
#include <cassert>

void te::serialize::WriteStringPtrHelper(const std::string& elementName, const std::string* s, te::xml::Writer& writer)
{
  if(s != 0 && !s->empty()) writer.writeElement(elementName, *s);
}

void te::serialize::WriteParameterValuePtrHelper(const std::string& elementName, const te::se::ParameterValue* p, te::xml::Writer& writer)
{
  if(p == 0)
    return;

  writer.writeStartElement(elementName);
  Save(p, writer);
  writer.writeEndElement(elementName);
}

void te::serialize::WriteBaseSymbolizerHelper(const te::xl::SimpleLink* link, te::xml::Writer& writer)
{
  if(link == 0)
    return;

  writer.writeStartElement("BaseSymbolizer");
  WriteOnlineResourceHelper(link, writer);
  writer.writeEndElement("BaseSymbolizer");
}

void te::serialize::WriteOnlineResourceHelper(const te::xl::SimpleLink* link, te::xml::Writer& writer)
{
  if(link == 0)
    return;

  writer.writeStartElement("OnlineResource");
  Save(link, writer);
  writer.writeEndElement("OnlineResource");
}

void te::serialize::WriterSymbolizerHelper(const te::se::Symbolizer* symbolizer, te::xml::Writer& writer)
{
  assert(symbolizer);

  writer.writeAttribute("version", symbolizer->getVersion());
  //writer.writeAttribute("uom", symbolizer->getUom()); // TODO: URI from te::common:: UnitOfMeasure!
  WriteStringPtrHelper("Name", &symbolizer->getName(), writer);
  Save(symbolizer->getDescription(), writer);
  WriteBaseSymbolizerHelper(symbolizer->getBaseSymbolizer(), writer);
}

void te::serialize::WriterSelectedChannelHelper(const std::string& elementName, const te::se::SelectedChannel* sc, te::xml::Writer& writer)
{
  if(sc == 0)
    return;

  writer.writeStartElement(elementName);
  Save(sc, writer);
  writer.writeEndElement(elementName);
}
