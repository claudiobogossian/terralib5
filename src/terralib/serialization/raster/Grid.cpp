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
  \file terralib/serialization/raster/Grid.cpp
 
  \brief Support for Grid serialization.
*/

// TerraLib
#include "../../geometry/Envelope.h"
#include "../../raster/Grid.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../geometry/Envelope.h"
#include "Grid.h"

// STL
#include <cassert>
#include <memory>

double* te::serialize::ReadGeoTransform(te::xml::Reader& reader)
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

  return geo;
}

te::rst::Grid* te::serialize::ReadGrid(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Grid");

  reader.next();

  std::auto_ptr<te::gm::Envelope> e(ReadExtent(reader));

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "NumCols");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  unsigned int ncols = reader.getElementValueAsInt32();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "NumRows");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  unsigned int nrows = reader.getElementValueAsInt32();

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "SRID");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  int srid = reader.getElementValueAsInt32();

  reader.next();

  double* geo = ReadGeoTransform(reader);

  te::rst::Grid* grid = new te::rst::Grid(ncols, nrows);

  grid->setGeoreference(geo, srid);

  delete []  geo;

  return grid;
}

void te::serialize::Save(const te::rst::Grid* grid, te::xml::Writer& writer)
{
  writer.writeStartElement("Grid");

  const te::gm::Envelope* e = grid->getExtent();

  SaveExtent(e, writer);

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

