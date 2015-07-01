/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/raster/serialization/xml/Serializer.cpp

  \brief Support for serialization of Raster information.
*/

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../geometry/serialization/xml/Serializer.h"
#include "../../../xml/AbstractWriter.h"
#include "../../../xml/AbstractWriterFactory.h"
#include "../../../xml/Reader.h"
#include "../../Band.h"
#include "../../BandProperty.h"
#include "../../Grid.h"
#include "../../Raster.h"
#include "Serializer.h"

// STL
#include <cassert>
#include <fstream>
#include <memory>

// Boost
#include <boost/lexical_cast.hpp>

void te::serialize::xml::ReadRasterInfo(std::map<std::string, std::string>& rinfo, te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Info");

  reader.next();

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "kvp")
  {
    std::string k = reader.getAttr("k");
    std::string v = reader.getAttr("v");

    rinfo[k] = v;

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }
}

void te::serialize::xml::SaveRasterInfo(std::map<std::string, std::string>& rinfo, te::xml::AbstractWriter& writer)
{
  if(rinfo.empty())
    return;

  writer.writeStartElement("Info");

  std::map<std::string, std::string>::const_iterator it = rinfo.begin();
  std::map<std::string, std::string>::const_iterator itend = rinfo.end();

  while(it != itend)
  {
    writer.writeStartElement("kvp");
    writer.writeAttribute("k", it->first);
    writer.writeAttribute("v", it->second);
    writer.writeEndElement("kvp");

    ++it;
  }

  writer.writeEndElement("Info");
}

void te::serialize::xml::Save(const te::rst::Raster* raster, const std::string& fileName)
{
  std::fstream ostr(fileName.c_str(), std::ios_base::out);

  Save(raster, ostr);

  ostr.close();
}

void te::serialize::xml::Save(const te::rst::Raster* raster, std::ostream& ostr)
{
  std::auto_ptr<te::xml::AbstractWriter> w(te::xml::AbstractWriterFactory::make());

  Save(raster, *w.get());
}

void te::serialize::xml::Save(const te::rst::Raster* raster, te::xml::AbstractWriter& writer)
{
  writer.writeStartDocument("UTF-8", "no");

  writer.writeStartElement("Raster");
  
  writer.writeAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
  writer.writeAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance");
  writer.writeAttribute("xmlns", "http://www.terralib.org/schemas/raster");
  writer.writeAttribute("xsd:schemaLocation", "http://www.terralib.org/schemas/raster C:/Users/gribeiro/Documents/terralib5/trunk/myschemas/terralib/raster/raster.xsd");
  writer.writeAttribute("version", "5.0.0");
  writer.writeAttribute("release", "2011-01-01");

  writer.writeElement("Name", raster->getName());

  Save(raster->getGrid(), writer);

  writer.writeStartElement("Bands");

  for(std::size_t i = 0; i < raster->getNumberOfBands(); ++i)
    Save(raster->getBand(i)->getProperty(), writer);

  writer.writeEndElement("Bands");

  std::map<std::string, std::string> rinfo = raster->getInfo();

  SaveRasterInfo(rinfo, writer);

  writer.writeEndElement("Raster");
}

double* te::serialize::xml::ReadGeoTransform(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "GeoTransform");

  double* geo = new double[6];

  geo[0] = reader.getAttrAsDouble(0);
  geo[1] = reader.getAttrAsDouble(1);
  geo[2] = reader.getAttrAsDouble(2);
  geo[3] = reader.getAttrAsDouble(3);
  geo[4] = reader.getAttrAsDouble(4);
  geo[5] = reader.getAttrAsDouble(5);

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return geo;
}

te::rst::Grid* te::serialize::xml::ReadGrid(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Grid");

  reader.next();

  /* Extent Element */
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Extent");
  std::auto_ptr<te::gm::Envelope> e(ReadExtent(reader));

  /* NumCols Element */
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "NumCols");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  unsigned int ncols = reader.getElementValueAsInt32();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* NumRows Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "NumRows");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  unsigned int nrows = reader.getElementValueAsInt32();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* SRID Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "SRID");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  int srid = reader.getElementValueAsInt32();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* GeoTransform Element */
  reader.next();
  double* geo = ReadGeoTransform(reader);

  te::rst::Grid* grid = new te::rst::Grid(ncols, nrows);

  grid->setGeoreference(geo, srid);

  delete []  geo;

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return grid;
}

void te::serialize::xml::Save(const te::rst::Grid* grid, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("Grid");

  const te::gm::Envelope* e = grid->getExtent();

  SaveExtent(*e, writer);

  writer.writeElement("NumCols", grid->getNumberOfColumns());
  writer.writeElement("NumRows", grid->getNumberOfRows());
  writer.writeElement("SRID", grid->getSRID());

  writer.writeStartElement("GeoTransform");
  writer.writeAttribute("c1", grid->getGeoreference()[0]);
  writer.writeAttribute("c2", grid->getGeoreference()[1]);
  writer.writeAttribute("c3", grid->getGeoreference()[2]);
  writer.writeAttribute("c4", grid->getGeoreference()[3]);
  writer.writeAttribute("c5", grid->getGeoreference()[4]);
  writer.writeAttribute("c6", grid->getGeoreference()[5]);
  writer.writeEndElement("GeoTransform");

  writer.writeEndElement("Grid");
}

std::vector<te::rst::BandProperty*> te::serialize::xml::ReadBandPropertyVector(te::xml::Reader& reader)
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

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return bands;
}

te::rst::BandProperty* te::serialize::xml::ReadBandProperty(te::xml::Reader& reader)
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
  assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of BlockInfo Element

  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of Band Element

  reader.next();

  te::rst::BandProperty* bp = new te::rst::BandProperty(num, datatype);

  bp->m_blkw = blkw;
  bp->m_blkh = blkh;
  bp->m_nblocksx = nblocksx;
  bp->m_nblocksy = nblocksy;
  bp->m_noDataValue = noData;

  return bp;
}

void te::serialize::xml::Save(const te::rst::BandProperty* bp, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("Band");

  writer.writeAttribute("num", boost::lexical_cast<std::string>(bp->m_idx));
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



