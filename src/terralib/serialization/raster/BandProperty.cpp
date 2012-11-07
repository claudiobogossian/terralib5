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
  \file terralib/serialization/raster/BandProperty.cpp
 
  \brief Support for BandProperty serialization.
*/

// TerraLib
#include "../../raster/BandProperty.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "BandProperty.h"

// STL
#include <cassert>

std::vector<te::rst::BandProperty*> te::serialize::ReadBandPropertyVector(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Bands");

  std::vector<te::rst::BandProperty*> bands;

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Band");

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "Band")
  {
    te::rst::BandProperty* bp = ReadBandProperty(reader);
    bands.push_back(bp);
  }

  return bands;
}

te::rst::BandProperty* te::serialize::ReadBandProperty(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Band");

  int num = reader.getAttrAsInt32(0);
  int datatype = reader.getAttrAsInt32(1);
  double noData = reader.getAttrAsDouble(2);

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "BlockInfo");

  int blkw = reader.getAttrAsInt32(0);
  int blkh = reader.getAttrAsInt32(1);
  int nblocksx = reader.getAttrAsInt32(2);
  int nblocksy = reader.getAttrAsInt32(3);

  reader.next();

  te::rst::BandProperty* bp = new te::rst::BandProperty(num, datatype);

  bp->m_blkw = blkw;
  bp->m_blkh = blkh;
  bp->m_nblocksx = nblocksx;
  bp->m_nblocksy = nblocksy;
  bp->m_noDataValue = noData;

  return bp;
}

void te::serialize::Save(const te::rst::BandProperty* bp, te::xml::Writer& writer)
{
  writer.writeStartElement("Band");

  writer.writeAttribute("num", bp->m_idx);
  writer.writeAttribute("datatype", bp->m_type);
  writer.writeAttribute("no_data", bp->m_noDataValue);

  writer.writeStartElement("BlockInfo");
  writer.writeAttribute("blkw", bp->m_blkw);
  writer.writeAttribute("blkh", bp->m_blkh);
  writer.writeAttribute("nblocksx", bp->m_nblocksx);
  writer.writeAttribute("nblocksy", bp->m_nblocksy);
  writer.writeEndElement("BlockInfo");

  //writer.writeElement(ostr, "Description", bp->m_description);

  writer.writeEndElement("Band");
}

