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
  \file terralib/serialization/se/AnchorPoint.cpp
 
  \brief Support for AnchorPoint serialization.
*/

// TerraLib
#include "../../se/AnchorPoint.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "AnchorPoint.h"
#include "ParameterValue.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::AnchorPoint* te::serialize::ReadAnchorPoint(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "AnchorPoint");

  reader.next();

  std::auto_ptr<te::se::AnchorPoint> ap(new te::se::AnchorPoint);

  assert(reader.getElementLocalName() == "AnchorPointX");
  reader.next();
  ap->setAnchorPointX(ReadParameterValue(reader));

  assert(reader.getElementLocalName() == "AnchorPointY");
  reader.next();
  ap->setAnchorPointY(ReadParameterValue(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ap.release();
}

void te::serialize::Save(const te::se::AnchorPoint* ap, te::xml::Writer& writer)
{
  if(ap == 0)
    return;

  writer.writeStartElement("se:AnchorPoint");

  WriteParameterValuePtrHelper("se:AnchorPointX", ap->getAnchorPointX(), writer);
  WriteParameterValuePtrHelper("se:AnchorPointY", ap->getAnchorPointY(), writer);

  writer.writeEndElement("se:AnchorPoint");
}
