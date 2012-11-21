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
  \file terralib/serialization/se/ShadedRelief.cpp
 
  \brief Support for ShadedRelief serialization.
*/

// TerraLib
#include "../../se/ShadedRelief.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "ShadedRelief.h"

// STL
#include <cassert>
#include <memory>

te::se::ShadedRelief* te::serialize::ReadShadedRelief(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ShadedRelief");

  reader.next();

  std::auto_ptr<te::se::ShadedRelief> sr(new te::se::ShadedRelief);

  if(reader.getElementLocalName() == "BrightnessOnly")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    sr->setBrightnessOnly(reader.getElementValueAsBoolean());
    reader.next();
  }

  if(reader.getElementLocalName() == "ReliefFactor")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    sr->setReliefFactor(reader.getElementValueAsDouble());
    reader.next();
  }

  return sr.release();
}

void te::serialize::Save(const te::se::ShadedRelief* sr, te::xml::Writer& writer)
{
  if(sr == 0)
    return;

  writer.writeStartElement("ShadedRelief");

  std::string brightnessOnlyValue;
  sr->isBrightnessOnly() ? brightnessOnlyValue = "true" : brightnessOnlyValue = "false";

  writer.writeElement("BrightnessOnly", brightnessOnlyValue);
  writer.writeElement("ReliefFactor", sr->getReliefFactor());

  writer.writeEndElement("ShadedRelief");
}
