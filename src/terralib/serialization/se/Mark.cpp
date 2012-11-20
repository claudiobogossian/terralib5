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
  \file terralib/serialization/se/Mark.cpp
 
  \brief Support for Mark serialization.
*/

// TerraLib
#include "../../se/Mark.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../xlink/SimpleLink.h"
#include "Fill.h"
#include "InlineContent.h"
#include "Mark.h"
#include "Stroke.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::Mark* te::serialize::ReadMark(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Mark");

  reader.next();

  std::auto_ptr<te::se::Mark> mark(new te::se::Mark);

  // WellKnownName
  if(reader.getElementLocalName() == "WellKnownName")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    mark->setWellKnownName(new std::string(reader.getElementValue()));
    reader.next();
  }
  else if(reader.getElementLocalName() == "OnlineResource")
    mark->setOnlineResource(ReadSimpleLink(reader));
  else // InlineContent
    mark->setInlineContent(ReadInlineContent(reader));

  // Fill
  if(reader.getElementLocalName() == "Fill")
    mark->setFill(ReadFill(reader));

  // Stroke
  if(reader.getElementLocalName() == "Stroke")
    mark->setStroke(ReadStroke(reader));

  return mark.release();
}

void te::serialize::Save(const te::se::Mark* mark, te::xml::Writer& writer)
{
  if(mark == 0)
    return;

  writer.writeStartElement("Mark");

  const std::string* wellKnownName = mark->getWellKnownName();
  if(wellKnownName)
    writer.writeElement("WellKnownName", *wellKnownName);
  else
  {
    const te::xl::SimpleLink* link = mark->getOnlineResource();
    if(link)
      WriteOnlineResourceHelper(link, writer);
    else
    {
      const te::se::InlineContent* ic = mark->getInlineContent();
      assert(ic);
      Save(ic, writer);
    }
    const std::string* format = mark->getFormat();
    assert(format);
    writer.writeElement("Format", *format);

    writer.writeElement("MarkIndex", mark->getMarkIndex());
  }

  Save(mark->getFill(), writer);
  Save(mark->getStroke(), writer);

  writer.writeEndElement("Mark");
}
