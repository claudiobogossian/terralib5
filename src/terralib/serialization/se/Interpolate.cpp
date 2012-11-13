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
#include "../../se/InterpolationPoint.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "Interpolate.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <vector>

te::se::Interpolate* te::serialize::ReadInterpolate(te::xml::Reader& reader)
{
  return 0;
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

  const std::vector<te::se::InterpolationPoint*> ipts = interpolate->getInterpolationPoints();
  for(std::size_t i = 0; i < ipts.size(); ++i)
  {
    writer.writeStartElement("InterpolationPoint");
    writer.writeElement("Data", ipts[i]->getData());
    assert(ipts[i]->getValue());
    WriteParameterValuePtrHelper("Value", ipts[i]->getValue(), writer);
    writer.writeEndElement("InterpolationPoint");
  }

   WriteParameterValuePtrHelper("LookupValue", interpolate->getLookupValue(), writer);

  writer.writeEndElement("Interpolate");
}
