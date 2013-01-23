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
  \file terralib/serialization/se/LabelPlacement.cpp
 
  \brief Support for LabelPlacement serialization.
*/

// TerraLib
#include "../../se/LabelPlacement.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "LabelPlacement.h"
#include "LinePlacement.h"
#include "PointPlacement.h"

// STL
#include <cassert>
#include <memory>

te::se::LabelPlacement* te::serialize::ReadLabelPlacement(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "LabelPlacement");

  reader.next();

  std::auto_ptr<te::se::LabelPlacement> lp(new te::se::LabelPlacement);

  if(reader.getElementLocalName() == "PointPlacement")
    lp->setPointPlacement(ReadPointPlacement(reader));
  else // LinePlacement
    lp->setLinePlacement(ReadLinePlacement(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return lp.release();
}

void te::serialize::Save(const te::se::LabelPlacement* lp, te::xml::Writer& writer)
{
  if(lp == 0)
    return;

  writer.writeStartElement("LabelPlacement");

  const te::se::PointPlacement* pp = lp->getPointPlacement();
  if(pp)
    Save(pp, writer);
  else
  {
    const te::se::LinePlacement* linePlacement = lp->getLinePlacement();
    assert(linePlacement);
    Save(linePlacement, writer);
  }

  writer.writeEndElement("LabelPlacement");
}
