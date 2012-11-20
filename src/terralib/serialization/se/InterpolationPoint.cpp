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
  \file terralib/serialization/se/InterpolationPoint.cpp
 
  \brief Support for InterpolationPoint serialization.
*/

// TerraLib
#include "../../se/InterpolationPoint.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "InterpolationPoint.h"
#include "ParameterValue.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::InterpolationPoint* te::serialize::ReadInterpolationPoint(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "InterpolationPoint");

  std::auto_ptr<te::se::InterpolationPoint> ip(new te::se::InterpolationPoint);

  reader.next();
  assert(reader.getElementLocalName() == "Data");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  double data = reader.getElementValueAsDouble();
  ip->setData(data);

  reader.next();

  assert(reader.getElementLocalName() == "Value");
  reader.next();
  ip->setValue(ReadParameterValue(reader));

  return ip.release();
}

void te::serialize::Save(const te::se::InterpolationPoint* ip, te::xml::Writer& writer)
{
  if(ip == 0)
    return;

  writer.writeStartElement("InterpolationPoint");
  
  writer.writeElement("Data", ip->getData());
  assert(ip->getValue());
  WriteParameterValuePtrHelper("Value", ip->getValue(), writer);

  writer.writeEndElement("InterpolationPoint");
}
