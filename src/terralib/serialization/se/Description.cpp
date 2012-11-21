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
  \file terralib/serialization/se/Description.cpp
 
  \brief Support for Description serialization.
*/

// TerraLib
#include "../../se/Description.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "Description.h"

// STL
#include <cassert>
#include <memory>

te::se::Description* te::serialize::ReadDescription(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Description");

  reader.next();

  std::auto_ptr<te::se::Description> description(new te::se::Description);

  // Title
  if(reader.getElementLocalName() == "Title")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string title = reader.getElementValue();
    description->setTitle(title);
    reader.next();
  }

  // Abstract
  if(reader.getElementLocalName() == "Abstract")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string abs = reader.getElementValue();
    description->setAbstract(abs);
    reader.next();
  }

  return description.release();
}

void te::serialize::Save(const te::se::Description* d, te::xml::Writer& writer)
{
  if(d == 0)
    return;

  writer.writeStartElement("Description");
  
  if(!d->getTitle().empty())
    writer.writeElement("Title", d->getTitle());
  
  if(!d->getAbstract().empty())
    writer.writeElement("Abstract", d->getAbstract());

  writer.writeEndElement("Description");
}

