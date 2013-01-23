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
  \file terralib/serialization/se/ContrastEnhancement.cpp
 
  \brief Support for ContrastEnhancement serialization.
*/

// TerraLib
#include "../../se/ContrastEnhancement.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "ContrastEnhancement.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::ContrastEnhancement* te::serialize::ReadContrastEnhancement(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ContrastEnhancement");

  reader.next();

  std::auto_ptr<te::se::ContrastEnhancement> ce(new te::se::ContrastEnhancement);
  ce->setContrastEnhancementType(te::se::ContrastEnhancement::ENHANCEMENT_NONE);

  if(reader.getElementLocalName() == "Histogram")
  {
    ce->setContrastEnhancementType(te::se::ContrastEnhancement::ENHANCEMENT_HISTOGRAM);
    reader.next();
  }
  else if(reader.getElementLocalName() == "Normalize")
  {
    ce->setContrastEnhancementType(te::se::ContrastEnhancement::ENHANCEMENT_NORMALIZE);
    reader.next();
  }

  if(reader.getElementLocalName() == "GammaValue")
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    ce->setGammaValue(reader.getElementValueAsDouble());
    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ce.release();
}

void te::serialize::Save(const te::se::ContrastEnhancement* ce, te::xml::Writer& writer)
{
  if(ce == 0)
    return;

  writer.writeStartElement("ContrastEnhancement");
  te::se::ContrastEnhancement::ContrastEnhancementType type = ce->getContrastEnhancementType();
  switch(type)
  {
    case te::se::ContrastEnhancement::ENHANCEMENT_NONE:
    break;

    case te::se::ContrastEnhancement::ENHANCEMENT_HISTOGRAM:
      writer.writeElement("Histogram", ""); // TODO: Writer method to writer a empty element, i.e. <Histogram/>
    break;

    case te::se::ContrastEnhancement::ENHANCEMENT_NORMALIZE:
      writer.writeElement("Normalize", ""); // TODO: Writer method to writer a empty element, i.e. <Normalize/>
    break;
  }

  writer.writeElement("GammaValue", ce->getGammaValue());

  writer.writeEndElement("ContrastEnhancement");
}
