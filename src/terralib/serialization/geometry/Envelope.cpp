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
  \file Envelope.cpp
   
  \brief Auxiliary classes and functions to read envelope information from a XML document.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/ReaderFactory.h"
#include "../../xml/Writer.h"
#include "../../geometry/Envelope.h"
#include "../Exception.h"
#include "Envelope.h"

te::gm::Envelope* te::serialize::ReadExtent(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Extent");

  double llx = reader.getAttrAsDouble(0);
  double lly = reader.getAttrAsDouble(1);
  double urx = reader.getAttrAsDouble(2);
  double ury = reader.getAttrAsDouble(3);

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return new te::gm::Envelope(llx, lly, urx, ury);
}

void te::serialize::SaveExtent(const te::gm::Envelope* e, te::xml::Writer& writer)
{
  assert(e);

  writer.writeStartElement("Extent");
  writer.writeAttribute("llx", e->m_llx);
  writer.writeAttribute("lly", e->m_lly);
  writer.writeAttribute("urx", e->m_urx);
  writer.writeAttribute("ury", e->m_lly);
  writer.writeEndElement("Extent");
}

