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
  \file terralib/serialization/se/Interpolate.cpp
 
  \brief Support for Interpolate serialization.
*/

// TerraLib
#include "../../se/Interpolate.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "Interpolate.h"
#include "InterpolationPoint.h"
#include "ParameterValue.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <vector>

te::se::Interpolate* te::serialize::ReadInterpolate(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Interpolate");

  assert(reader.hasAttrs());

  std::auto_ptr<te::se::Interpolate> interpolate(new te::se::Interpolate);

  // FallBackValue Attribute
  std::string fbv = reader.getAttr("fallbackValue");
  assert(!fbv.empty());
  interpolate->setFallbackValue(fbv);

  // Mode Attribute
  std::string mode = reader.getAttr("mode");
  assert(!mode.empty() && (mode == "linear" || mode == "cosine" || mode == "cubic"));
  if(mode == "linear")
    interpolate->setModeType(te::se::Interpolate::LINEAR);
  else if(mode == "cosine")
    interpolate->setModeType(te::se::Interpolate::COSINE);
  else
    interpolate->setModeType(te::se::Interpolate::CUBIC);

  // Method Attribute
  std::string method = reader.getAttr("method");
  assert(!method.empty() && (method == "numeric" || method == "color"));
  method == "numeric" ? interpolate->setMethodType(te::se::Interpolate::NUMERIC) : interpolate->setMethodType(te::se::Interpolate::COLOR);

  reader.next();

  // LookupValue
  assert(reader.getElementLocalName() == "LookupValue");
  reader.next();
  interpolate->setLookupValue(ReadParameterValue(reader));

  // InterpolationPoints
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "InterpolationPoint")
  {
    reader.next();
    interpolate->add(ReadInterpolationPoint(reader));
  }

  return interpolate.release();
}

void te::serialize::Save(const te::se::Interpolate* interpolate, te::xml::Writer& writer)
{
  if(interpolate == 0)
    return;

  writer.writeStartElement("Interpolate");

  writer.writeAttribute("fallbackValue", interpolate->getFallbackValue());

  te::se::Interpolate::ModeType modeType = interpolate->getModeType();
  switch(modeType)
  {
    case te::se::Interpolate::LINEAR:
      writer.writeAttribute("mode", "linear");
    break;

    case te::se::Interpolate::COSINE:
      writer.writeAttribute("mode", "cosine");
    break;

    case te::se::Interpolate::CUBIC:
      writer.writeAttribute("mode", "cubic");
    break;  
  }

  te::se::Interpolate::MethodType methodType = interpolate->geMethodType();
  switch(methodType)
  {
    case te::se::Interpolate::NUMERIC:
      writer.writeAttribute("method", "numeric");
    break;

    case te::se::Interpolate::COLOR:
      writer.writeAttribute("mode", "color");
    break;
  }

  WriteParameterValuePtrHelper("LookupValue", interpolate->getLookupValue(), writer);

  const std::vector<te::se::InterpolationPoint*> ipts = interpolate->getInterpolationPoints();
  assert(!ipts.empty());
  for(std::size_t i = 0; i < ipts.size(); ++i)
    Save(ipts[i], writer);

  writer.writeEndElement("Interpolate");
}
