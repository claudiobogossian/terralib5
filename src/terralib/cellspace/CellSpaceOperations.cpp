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
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../geometry/Utils.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "CellSpaceOperations.h"

#include <stdio.h>

te::cellspace::CellularSpacesOperations::CellularSpacesOperations()
  : m_outputDataSetType(0),
    m_outputDataSet(0)
{
  
}

te::cellspace::CellularSpacesOperations::~CellularSpacesOperations()
{
}

void te::cellspace::CellularSpacesOperations::createCellSpace(const std::string& name,
                                                              te::map::AbstractLayerPtr layerBase,
                                                              double resX, double resY, bool useMask,
                                                              CellSpaceType type)
{
  te::gm::Envelope box = layerBase->getExtent();

  te::gm::Envelope newEnv = te::gm::AdjustToCut(box, resX, resY);

  int srid = layerBase->getSRID();

  m_outputDataSetType = new te::da::DataSetType(name);

  te::dt::Property* idProp = new te::dt::StringProperty("id");
  te::dt::Property* colProp = new te::dt::SimpleProperty("col", te::dt::INT32_TYPE);
  te::dt::Property* rowProp = new te::dt::SimpleProperty("row", te::dt::INT32_TYPE);
  te::dt::Property* geomProp = 0;

  if(type = CELLSPACE_POLYGONS)
    geomProp = new te::gm::GeometryProperty("geom", srid, te::gm::PolygonType);
  else if(type = CELLSPACE_POINTS)
    geomProp = new te::gm::GeometryProperty("geom", srid, te::gm::PointType);

  m_outputDataSetType->add(idProp);
  m_outputDataSetType->add(colProp);
  m_outputDataSetType->add(rowProp);
  m_outputDataSetType->add(geomProp);

  std::string pkName = name + "_pk_id";
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(pkName, m_outputDataSetType);
  std::vector<te::dt::Property*> pkProp;
  pkProp.push_back(idProp);
  pk->setProperties(pkProp);

  m_outputDataSet = new te::mem::DataSet(m_outputDataSetType);

  te::mem::DataSet* ds = dynamic_cast<te::mem::DataSet*>(m_outputDataSet);

  double x1,x2,y1,y2;
  x1 = newEnv.getLowerLeftX();
  y1 = newEnv.getLowerLeftY();
  x2 = newEnv.getUpperRightX();
  y2 = newEnv.getUpperRightY();

  int maxcols, maxlines;
  maxcols = (int)((x2-x1)/resX);
  maxlines = (int)((y2-y1)/resY);

  double x;
  double y = y2;
  for (int lin = 0; lin < maxlines; ++lin)
  {
    double yu = y;
    y=y-resY;
    x=x1;

    for (int col = 0; col < maxcols; ++col)
    {
      te::gm::Envelope* env = new te::gm::Envelope(x, y, x+resX, yu);

      te::gm::Geometry* geom = 0;

      if(type == CELLSPACE_POLYGONS)
      {
        geom = new te::gm::Polygon(0, te::gm::PolygonType);
        geom = te::gm::GetGeomFromEnvelope(env, srid);
      }
      else if(type == CELLSPACE_POINTS)
      {
        double pX = env->m_llx +( (env->m_urx - env->m_llx) / 2);
        double pY = env->m_lly +( (env->m_ury - env->m_lly) / 2);
        geom = new te::gm::Point(pX, pY, srid);
      }

      char celId[32];
      sprintf(celId,"C%02dL%02d",col,lin);

      te::mem::DataSetItem* item = new te::mem::DataSetItem(ds);
      item->setString(0, celId);
      item->setInt32(1, col);
      item->setInt32(2, lin);
      item->setGeometry(3, geom);
      ds->add(item);
      
      x=x+resX;
    }
  }
}

te::da::DataSetType* te::cellspace::CellularSpacesOperations::getDataSetType()
{
  return m_outputDataSetType;
}

te::da::DataSet* te::cellspace::CellularSpacesOperations::getDataSet()
{
  return m_outputDataSet;
}