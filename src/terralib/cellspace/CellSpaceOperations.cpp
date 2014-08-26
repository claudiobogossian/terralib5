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
#include "../dataaccess/utils/Utils.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../geometry/Utils.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "../raster.h"
#include "../sam.h"
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

  if(type == CELLSPACE_RASTER)
  {
    te::rst::Grid* grid = new te::rst::Grid(resX, resY, &newEnv);
    grid->setGeoreference(te::gm::Coord2D(newEnv.m_urx, newEnv.m_ury), srid, resX, resY);

    std::vector<te::rst::BandProperty*> bprops;
    bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));

    std::map<std::string, std::string> rinfo;

    te::rst::RasterProperty* rstp = new te::rst::RasterProperty(grid, bprops, rinfo);

    m_outputDataSetType = new te::da::DataSetType(name+".tif");
    m_outputDataSetType->add(rstp);

    return;
  }

  std::auto_ptr<te::da::DataSetType> refDst = layerBase->getSchema();
  std::auto_ptr<te::da::DataSet> refDs = layerBase->getData();

  m_outputDataSetType = new te::da::DataSetType(name);

  te::dt::Property* idProp = new te::dt::StringProperty("id");
  te::dt::Property* colProp = new te::dt::SimpleProperty("col", te::dt::INT32_TYPE);
  te::dt::Property* rowProp = new te::dt::SimpleProperty("row", te::dt::INT32_TYPE);
  te::dt::Property* geomProp = 0;

  if(type == CELLSPACE_POLYGONS)
    geomProp = new te::gm::GeometryProperty("geom", srid, te::gm::PolygonType);
  else if(type == CELLSPACE_POINTS)
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
  maxcols = te::rst::Round((x2-x1)/resX);
  maxlines = te::rst::Round((y2-y1)/resY);

  std::auto_ptr<te::sam::rtree::Index<size_t, 8> > rtree;

  if(useMask)
  {
    rtree.reset(getRtree(layerBase));
  }

  double x;
  double y;
  for(int lin = 0; lin < maxlines; ++lin)
  {
    y = y1+(lin*resY);
    for(int col = 0; col < maxcols; ++col)
    {
      x = x1+(col*resX);
      te::gm::Envelope* env = new te::gm::Envelope(x, y, x+resX, y+resY);

      std::auto_ptr<te::gm::Geometry> geom;

      if(type == CELLSPACE_POLYGONS)
      {
        geom.reset(te::gm::GetGeomFromEnvelope(env, srid));
      }
      else if(type == CELLSPACE_POINTS)
      {
        double pX = env->m_llx +( (env->m_urx - env->m_llx) / 2);
        double pY = env->m_lly +( (env->m_ury - env->m_lly) / 2);
        geom.reset(new te::gm::Point(pX, pY, srid));
      }

      if(useMask)
      {
        std::vector<size_t> report;
        rtree->search(*geom->getMBR(), report);

        std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(refDs.get());

        if(!report.empty())
        {
          for(std::size_t i = 0; i < report.size(); ++i)
          {
            refDs->move(report[i]);

            std::auto_ptr<te::gm::Geometry> g = refDs->getGeometry(geomPos);
            g->setSRID(srid);

            if(geom->intersects(g.get()))
            {
              addCell(ds, col, lin, geom.release());
              break;
            }
          }
        }
      }
      else
      {
        addCell(ds, col, lin, geom.release());
      }
    }
  }
}

void te::cellspace::CellularSpacesOperations::addCell(te::mem::DataSet* ds, int col, int row, te::gm::Geometry* geom)
{
  char celId[32];
  sprintf(celId,"C%02dL%02d",col,row);

  te::mem::DataSetItem* item = new te::mem::DataSetItem(ds);
  item->setString(0, celId);
  item->setInt32(1, col);
  item->setInt32(2, row);
  item->setGeometry(3, geom);
  ds->add(item);
}

te::da::DataSetType* te::cellspace::CellularSpacesOperations::getDataSetType()
{
  return m_outputDataSetType;
}

te::da::DataSet* te::cellspace::CellularSpacesOperations::getDataSet()
{
  return m_outputDataSet;
}

te::sam::rtree::Index<size_t, 8>* te::cellspace::CellularSpacesOperations::getRtree(te::map::AbstractLayerPtr layerBase)
{
  te::sam::rtree::Index<size_t, 8>* rtree = new te::sam::rtree::Index<size_t, 8>;

  std::auto_ptr<te::da::DataSetType> dst = layerBase->getSchema();
  std::auto_ptr<te::da::DataSet> ds = layerBase->getData();

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(ds.get());

  ds->moveBeforeFirst();

  int count = 0;

  while(ds->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> geom = ds->getGeometry(geomPos);

    rtree->insert(*geom->getMBR(), count);

    ++count;
  }

  return rtree;
}