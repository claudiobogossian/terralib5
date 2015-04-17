/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/fe/serialization/xml/Filter.cpp
 
  \brief Support for the Filter serialization.
*/

// TerraLib
#include "../../../fe/Filter.h"
#include "../../../xml/AbstractWriter.h"
#include "../../../xml/Reader.h"
#include "AbstractOp.h"
#include "Filter.h"

// STL
#include <cassert>
#include <memory>

te::fe::Filter* te::fe::serialize::ReadFilter(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Filter");

  std::auto_ptr<te::fe::Filter> filter(new te::fe::Filter);

  reader.next();

  filter->setOp(te::fe::serialize::AbstractOp::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return filter.release();
}

void te::fe::serialize::Save(const te::fe::Filter* filter, te::xml::AbstractWriter& writer)
{
  if(filter == 0)
    return;

  writer.writeStartElement("ogc:Filter");

  AbstractOp::getInstance().write(filter->getOp(), writer);

  writer.writeEndElement("ogc:Filter");
}
