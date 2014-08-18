/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/cellspace/CellularSpacesOperations.cpp

  \brief Implementation of the Cellular Spaces operations.
*/

// Terralib
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Polygon.h"
#include "../geometry/Utils.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "CellSpaceOperations.h"


void te::cellspace::CellularSpacesOperations::createCellSpace(const std::string& layerName, te::map::AbstractLayerPtr layerBase, double resX, double resY, bool useMask)
{
  te::gm::Envelope box = layerBase->getExtent();

  int srid = layerBase->getSRID();

  te::da::DataSetType* dt = new te::da::DataSetType(layerName);

  te::dt::Property* idProp = new te::dt::StringProperty("id");
  te::dt::Property* colProp = new te::dt::SimpleProperty("col", te::dt::INT32_TYPE);
  te::dt::Property* rowProp = new te::dt::SimpleProperty("row", te::dt::INT32_TYPE);
  te::dt::Property* geomProp = new te::gm::GeometryProperty("geom", srid, te::gm::PolygonType);

  dt->add(idProp);
  dt->add(colProp);
  dt->add(rowProp);
  dt->add(geomProp);

  te::da::DataSet* ds = new te::mem::DataSet(dt);

  double x1,x2,y1,y2;
  x1 = box.getLowerLeftX();
  y1 = box.getLowerLeftY();
  x2 = box.getUpperRightX();
  y2 = box.getUpperRightY();

  int maxcols, maxlines;
  maxcols = (int)((y2-y1)/resY);
  maxlines = (int)((x2-x1)/resX);

  double x;
  double y = y2;
  for (std::size_t lin = 0; lin < maxlines; ++lin)
  {
    double yu = y;
    y=y-resY;
    x=x1;

    for (std::size_t col = 0; col < maxcols; ++col)
    {
      te::gm::Envelope* env = new te::gm::Envelope(x, y, x+resX, yu);

      te::gm::Geometry* geom = new te::gm::Polygon(0, te::gm::PolygonType);

      geom = te::gm::GetGeomFromEnvelope(env, srid);

      std::string itemId = "C"+col;
      itemId += "L"+lin;

      te::mem::DataSetItem* item = new te::mem::DataSetItem(ds);
      item->setString(0, itemId);
      item->setInt32(1, col);
      item->setInt32(2, lin);
      item->setGeometry(3, geom);
      x=x+resX;
    }
  }
}