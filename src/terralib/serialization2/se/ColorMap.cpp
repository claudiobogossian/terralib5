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
  \file terralib/serialization/se/ColorMap.cpp
 
  \brief Support for ColorMap serialization.
*/

// TerraLib
#include "../../se/ColorMap.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "Categorize.h"
#include "ColorMap.h"
#include "Interpolate.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::ColorMap* te::serialize::ReadColorMap(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ColorMap");

  reader.next();

  std::auto_ptr<te::se::ColorMap> cm(new te::se::ColorMap);

  if(reader.getElementLocalName() == "Categorize")
    cm->setCategorize(ReadCategorize(reader));
  else // Interpolate
    cm->setInterpolate(ReadInterpolate(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return cm.release();
}

void te::serialize::Save(const te::se::ColorMap* cm, te::xml::Writer& writer)
{
  if(cm == 0)
    return;

  writer.writeStartElement("se:ColorMap");

  te::se::Categorize* categorize = cm->getCategorize();
  if(categorize)
    Save(categorize, writer);
  else
  {
    te::se::Interpolate* interpolate = cm->getInterpolate();
    assert(interpolate);
    Save(interpolate, writer);
  }

  writer.writeEndElement("se:ColorMap");
}
