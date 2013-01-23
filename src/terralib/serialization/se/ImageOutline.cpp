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
  \file terralib/serialization/se/ImageOutline.cpp
 
  \brief Support for ImageOutline serialization.
*/

// TerraLib
#include "../../se/ImageOutline.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "ImageOutline.h"
#include "Symbolizer.h"

// STL
#include <cassert>
#include <memory>

te::se::ImageOutline* te::serialize::ReadImageOutline(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ImageOutline");

  reader.next();

  std::auto_ptr<te::se::ImageOutline> imageOutline(new te::se::ImageOutline);
  imageOutline->setSymbolizer(Symbolizer::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return imageOutline.release();
}

void te::serialize::Save(const te::se::ImageOutline* imageOutline, te::xml::Writer& writer)
{
  if(imageOutline == 0)
    return;

  writer.writeStartElement("ImageOutline");

  assert(imageOutline->getSymbolizer());
  Symbolizer::getInstance().write(imageOutline->getSymbolizer(), writer);

  writer.writeEndElement("ImageOutline");
}
