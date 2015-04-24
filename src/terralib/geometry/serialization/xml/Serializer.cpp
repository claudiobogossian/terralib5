/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/serialization/xml/Serializer.cpp

  \brief Auxiliary classes and functions to read envelope information from a XML document.
*/

// TerraLib
#include "../../../xml/AbstractWriter.h"
#include "../../../xml/Reader.h"
#include "../../../xml/ReaderFactory.h"
#include "../../Envelope.h"
#include "../../Exception.h"
#include "Serializer.h"

std::auto_ptr<te::gm::Envelope> te::serialize::xml::ReadExtent(te::xml::Reader& reader)
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

  return std::auto_ptr<te::gm::Envelope>(new te::gm::Envelope(llx, lly, urx, ury));
}

void te::serialize::xml::SaveExtent(const te::gm::Envelope& e, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("te_map:Extent");
  writer.writeAttribute("llx", e.m_llx);
  writer.writeAttribute("lly", e.m_lly);
  writer.writeAttribute("urx", e.m_urx);
  writer.writeAttribute("ury", e.m_ury);
  writer.writeEndElement("te_map:Extent");
}

