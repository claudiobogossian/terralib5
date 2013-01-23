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
  \file terralib/serialization/se/InlineContent.cpp
 
  \brief Support for InlineContent serialization.
*/

// TerraLib
#include "../../se/InlineContent.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "InlineContent.h"

// STL
#include <cassert>
#include <memory>

te::se::InlineContent* te::serialize::ReadInlineContent(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "InlineContent");

  std::string encondingValue = reader.getAttr("enconding");
  assert(encondingValue == "xml" || encondingValue == "base64");

  te::se::InlineContent::EncodingType encodingType;
  encondingValue == "xml" ? encodingType = te::se::InlineContent::XML : encodingType = te::se::InlineContent::BASE64;

  std::string data = reader.getElementValue();

  std::auto_ptr<te::se::InlineContent> ic(new te::se::InlineContent(encodingType));
  ic->setData(data);

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ic.release();
}

void te::serialize::Save(const te::se::InlineContent* ic, te::xml::Writer& writer)
{
  if(ic == 0)
    return;

  writer.writeStartElement("InlineContent");
  ic->getEncoding() == te::se::InlineContent::XML ? writer.writeAttribute("enconding", "xml") : writer.writeAttribute("enconding", "base64");
  writer.writeValue(ic->getData());
  writer.writeEndElement("InlineContent");
}
