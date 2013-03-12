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
  \file terralib/serialization/se/ExternalGraphic.cpp
 
  \brief Support for ExternalGraphic serialization.
*/

// TerraLib
#include "../../se/ExternalGraphic.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../xlink/SimpleLink.h"
#include "ExternalGraphic.h"
#include "InlineContent.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::ExternalGraphic* te::serialize::ReadExternalGraphic(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ExternalGraphic");

  reader.next();

  std::auto_ptr<te::se::ExternalGraphic> eg(new te::se::ExternalGraphic);

  if(reader.getElementLocalName() == "OnlineResource")
    eg->setOnlineResource(ReadSimpleLink(reader));
  else // InlineContent
    eg->setInlineContent(ReadInlineContent(reader));

  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  reader.next();

  // Format
  assert(reader.getElementLocalName() == "Format");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  eg->setFormat(reader.getElementValue());
  reader.next();

  // Recodes (...)

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return eg.release();
}

void te::serialize::Save(const te::se::ExternalGraphic* eg, te::xml::Writer& writer)
{
  if(eg == 0)
    return;

  writer.writeStartElement("se:ExternalGraphic");

  const te::xl::SimpleLink* link = eg->getOnlineResource();
  if(link)
    te::serialize::WriteOnlineResourceHelper(link, writer);
  else
  {
    const te::se::InlineContent* ic = eg->getInlineContent();
    assert(ic);
    Save(ic, writer);
  }
  const std::string& format = eg->getFormat();
  assert(!format.empty());
  writer.writeElement("se:Format", format);

  // Recodes (...)

  writer.writeEndElement("se:ExternalGraphic");
}
