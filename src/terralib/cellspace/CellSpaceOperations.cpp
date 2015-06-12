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
  \file terralib/cellspace/CellularSpacesOperations.cpp

  \brief Implementation of the Cellular Spaces operations.
*/

// Terralib
#include "../common/progress/TaskProgress.h"
#include "../dataaccess.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../maptools/DataSetLayer.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "../raster.h"
#include "../sam.h"
#include "CellSpaceOperations.h"

#include <stdio.h>

te::cellspace::CellularSpacesOperations::CellularSpacesOperations()
{
}

te::cellspace::CellularSpacesOperations::~CellularSpacesOperations()
{
}

void te::cellspace::CellularSpacesOperations::createCellSpace(te::da::DataSourceInfoPtr outputSource,
                                                              const std::string& name,
                                                              te::map::AbstractLayerPtr layerBase,
                                                              double resX,
                                                              double resY,
                                                              bool useMask,
                                                              CellSpaceType type)
{
  te::gm::Envelope env = layerBase->getExtent();

  int srid = layerBase->getSRID();

  int maxcols, maxrows;

  maxcols = (int)ceil((env.m_urx-env.m_llx)/resX);
  maxrows = (int)ceil((env.m_ury-env.m_lly)/resY);

  std::auto_ptr<te::da::DataSet> refDs;

  if(useMask)
  {
    refDs = layerBase->getData();
  }

  std::auto_ptr<te::da::DataSetType> outputDataSetType(createCellularDataSetType(name, srid, type));

  std::auto_ptr<te::sam::rtree::Index<size_t, 8> > rtree;

  if(useMask)
  {
    rtree.reset(getRtree(layerBase));
  }

  te::common::TaskProgress task("Processing Cellular Spaces...");
  task.setTotalSteps(maxrows);
  task.useTimer(true);

  std::auto_ptr<te::da::DataSet> outputDataSet(new te::mem::DataSet(outputDataSetType.get()));

  std::auto_ptr<te::mem::DataSet> ds(dynamic_cast<te::mem::DataSet*>(outputDataSet.release()));

  double x, y;
  for(int lin = 0; lin < maxrows; ++lin)
  {

    y = env.m_lly+(lin*resY);
    for(int col = 0; col < maxcols; ++col)
    {
      x = env.m_llx+(col*resX);

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
              addCell(ds.get(), col, lin, geom.release());
              break;
            }
          }
        }
      }
      else
      {
        addCell(ds.get(), col, lin, geom.release());
      }
    }

    task.pulse();
  }

  // Output
  std::auto_ptr<te::da::DataSource> source = te::da::DataSourceFactory::make(outputSource->getAccessDriver());
  source->setConnectionInfo(outputSource->getConnInfo());
  source->open();

  std::map<std::string, std::string> options;
  // create the dataset
  source->createDataSet(outputDataSetType.get(), options);

  // copy from memory to output datasource
  ds->moveBeforeFirst();
  source->add(outputDataSetType->getName(),ds.get(), options);
}

void te::cellspace::CellularSpacesOperations::createCellSpace(te::da::DataSourceInfoPtr outputSource,
                                                              const std::string& name,
                                                              const double resX,
                                                              const double resY,
                                                              const te::gm::Envelope& env,
                                                              const int srid,
                                                              const CellSpaceType type)
{
  te::map::AbstractLayerPtr auxLayer = new te::map::DataSetLayer("");

  auxLayer->setSRID(srid);
  auxLayer->setExtent(env);

  createCellSpace(outputSource, name, auxLayer, resX, resY, false, type);
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

te::sam::rtree::Index<size_t, 8>* te::cellspace::CellularSpacesOperations::getRtree(te::map::AbstractLayerPtr layerBase)
{
  te::sam::rtree::Index<size_t, 8>* rtree = new te::sam::rtree::Index<size_t, 8>;

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

te::da::DataSetType* te::cellspace::CellularSpacesOperations::createCellularDataSetType(const std::string& name, int srid, CellSpaceType type)
{
  te::da::DataSetType* dst = new te::da::DataSetType(name);

  te::dt::Property* idProp = new te::dt::StringProperty("id");
  te::dt::Property* colProp = new te::dt::SimpleProperty("col", te::dt::INT32_TYPE);
  te::dt::Property* rowProp = new te::dt::SimpleProperty("row", te::dt::INT32_TYPE);
  te::dt::Property* geomProp = 0;

  if(type == CELLSPACE_POLYGONS)
    geomProp = new te::gm::GeometryProperty("geom", srid, te::gm::PolygonType);
  else if(type == CELLSPACE_POINTS)
    geomProp = new te::gm::GeometryProperty("geom", srid, te::gm::PointType);

  dst->add(idProp);
  dst->add(colProp);
  dst->add(rowProp);
  dst->add(geomProp);

  std::string pkName = name + "_pk_id";
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(pkName, dst);
  std::vector<te::dt::Property*> pkProp;
  pkProp.push_back(idProp);
  pk->setProperties(pkProp);

  return dst;
}