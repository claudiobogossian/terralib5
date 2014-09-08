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
#include "../dataaccess.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "../raster.h"
#include "../sam.h"
#include "CellSpaceOperations.h"

#include <stdio.h>

te::cellspace::CellularSpacesOperations::CellularSpacesOperations()
  : m_outputDataSetType(0),
    m_outputDataSet(0),
    m_outputRaster(0)
{
  
}

te::cellspace::CellularSpacesOperations::~CellularSpacesOperations()
{
}
#include <cmath>
void te::cellspace::CellularSpacesOperations::createCellSpace(te::da::DataSourceInfoPtr outputSource,
                                                              const std::string& name,
                                                              te::map::AbstractLayerPtr layerBase,
                                                              double resX,
                                                              double resY,
                                                              bool useMask,
                                                              CellSpaceType type)
{
  if(type == CELLSPACE_RASTER)
  {
    createRasteCellSpace(outputSource, name, layerBase, resX, resY, useMask);
    return;
  }

  te::gm::Envelope env = layerBase->getExtent();
  
  int srid = layerBase->getSRID();

  int maxcols, maxrows;

  maxcols = (int)ceil((env.m_urx-env.m_llx)/resX);
  maxrows = (int)ceil((env.m_ury-env.m_lly)/resY);

  std::auto_ptr<te::da::DataSetType> refDst = layerBase->getSchema();
  std::auto_ptr<te::da::DataSet> refDs = layerBase->getData();

  m_outputDataSetType = createCellularDataSetType(name, srid, type);

  m_outputDataSet = new te::mem::DataSet(m_outputDataSetType);

  te::mem::DataSet* ds = dynamic_cast<te::mem::DataSet*>(m_outputDataSet);

  std::auto_ptr<te::sam::rtree::Index<size_t, 8> > rtree;

  if(useMask)
  {
    rtree.reset(getRtree(layerBase));
  }

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

  save(outputSource);
}

void te::cellspace::CellularSpacesOperations::createCellSpace(te::da::DataSourceInfoPtr outputSource,
                                                              const std::string& name,
                                                              const double resX,
                                                              const double resY,
                                                              const te::gm::Envelope& env,
                                                              const int srid,
                                                              const CellSpaceType type)
{
  if(type == CELLSPACE_RASTER)
  {
    createRasteCellSpace(outputSource, name, resX, resY, env, srid);
    return;
  }

  int maxcols, maxrows;
  maxcols = (int)ceil((env.m_urx-env.m_llx)/resX);
  maxrows = (int)ceil((env.m_ury-env.m_lly)/resY);

  m_outputDataSetType = createCellularDataSetType(name, srid, type);

  m_outputDataSet = new te::mem::DataSet(m_outputDataSetType);

  te::mem::DataSet* ds = dynamic_cast<te::mem::DataSet*>(m_outputDataSet);

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

      addCell(ds, col, lin, geom.release());
    }
  }

  save(outputSource);
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

std::auto_ptr<te::rst::Raster> te::cellspace::CellularSpacesOperations::getRaster()
{
  return m_outputRaster;
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

void te::cellspace::CellularSpacesOperations::createRasteCellSpace(te::da::DataSourceInfoPtr outputSource,
                                                                   const std::string& name,
                                                                   const double resX,
                                                                   const double resY,
                                                                   const te::gm::Envelope& env,
                                                                   const int srid)
{

  te::gm::Envelope* newEnv = new te::gm::Envelope(env);

  int maxcols, maxrows;
  maxcols = (int)ceil((newEnv->m_urx-newEnv->m_llx)/resX);
  maxrows = (int)ceil((newEnv->m_ury-newEnv->m_lly)/resY);

  te::rst::Grid* grid = new te::rst::Grid(maxcols, maxrows, resX, resY, newEnv, srid);

  std::vector<te::rst::BandProperty*> bprops;
  bprops.push_back(new te::rst::BandProperty(0, te::dt::INT32_TYPE));

  m_outputRaster.reset(te::rst::RasterFactory::make(outputSource->getAccessDriver(), grid, bprops, outputSource->getConnInfo()));

  double x, y;
  for(int lin = 0; lin < maxrows; ++lin)
  {
    y = newEnv->m_lly+(lin*resY);
    for(int col = 0; col < maxcols; ++col)
    {
      x = newEnv->m_llx+(col*resX);

      m_outputRaster->setValue(col, lin, 1, 0);

    }
  }
}

void te::cellspace::CellularSpacesOperations::createRasteCellSpace(te::da::DataSourceInfoPtr outputSource,
                                                                   const std::string& name,
                                                                   te::map::AbstractLayerPtr layerBase,
                                                                   double resX,
                                                                   double resY,
                                                                   bool useMask)
{
  te::gm::Envelope env = layerBase->getExtent();

  te::gm::Envelope* newEnv = new te::gm::Envelope(te::gm::AdjustToCut(env, resX, resY));

  int srid = layerBase->getSRID();

  int maxcols, maxrows;
  maxcols = (int)ceil((newEnv->m_urx-newEnv->m_llx)/resX);
  maxrows = (int)ceil((newEnv->m_ury-newEnv->m_lly)/resY);

  std::auto_ptr<te::sam::rtree::Index<size_t, 8> > rtree;

  if(useMask)
  {
    rtree.reset(getRtree(layerBase));
  }

  te::rst::Grid* grid = new te::rst::Grid(maxcols, maxrows, resX, resY, newEnv, srid);

  std::vector<te::rst::BandProperty*> bprops;
  bprops.push_back(new te::rst::BandProperty(0, te::dt::INT32_TYPE));

  m_outputRaster.reset(te::rst::RasterFactory::make(outputSource->getAccessDriver(), grid, bprops, outputSource->getConnInfo()));

  std::auto_ptr<te::da::DataSetType> refDst = layerBase->getSchema();
  std::auto_ptr<te::da::DataSet> refDs = layerBase->getData();

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(refDs.get());

  double x, y;
  for(int lin = 0; lin < maxrows; ++lin)
  {
    y = newEnv->m_lly+(maxrows-lin-1)*resY;
    for(int col = 0; col < maxcols; ++col)
    {
      x = newEnv->m_llx+(col*resX);

      if(useMask)
      {
        std::auto_ptr<te::gm::Envelope> auxEnv(new te::gm::Envelope(x, y, x+resX, y+resY));

        std::vector<size_t> report;
        rtree->search(*auxEnv, report);

        if(!report.empty())
        {
          for(std::size_t i = 0; i < report.size(); ++i)
          {
            refDs->move(report[i]);

            std::auto_ptr<te::gm::Geometry> g = refDs->getGeometry(geomPos);
            g->setSRID(srid);

            std::auto_ptr<te::gm::Geometry> auxGeom(te::gm::GetGeomFromEnvelope(auxEnv.get(), srid));

            if(auxGeom->intersects(g.get()))
            {
              m_outputRaster->setValue(col, lin, 1, 0);
              break;
            }
            else
            {
              m_outputRaster->setValue(col, lin, 0, 0);
            }

          }
        }
      }
      else
      {
        m_outputRaster->setValue(col, lin, 1, 0);
      }

    }
  }
}

void te::cellspace::CellularSpacesOperations::save(te::da::DataSourceInfoPtr sourceInfo)
{
  std::auto_ptr<te::da::DataSource> source = te::da::DataSourceFactory::make(sourceInfo->getAccessDriver());
  source->setConnectionInfo(sourceInfo->getConnInfo());

  source->open();

  std::auto_ptr<te::da::DataSourceTransactor> t = source->getTransactor();

  try
  {
    t->begin();

    std::map<std::string, std::string> op;

    t->createDataSet(m_outputDataSetType, op);

    t->add(m_outputDataSetType->getName(), m_outputDataSet, op);

    t->commit();
  }
  catch(te::common::Exception& e)
  {
    t->rollBack();
    throw te::common::Exception(e);
  }
}