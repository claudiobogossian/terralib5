/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mnt/core/Profile.cpp

  \brief This file contains a class that represents the profile.

 */

//TerraLib

#include "../../../../src/terralib/common/Translator.h"
#include "../../../../src/terralib/common/progress/TaskProgress.h"
#include "../../../../src/terralib/common/Logger.h"

#include "../../../../src/terralib/dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../../src/terralib/dataaccess/dataset/DataSetTypeCapabilities.h"

#include "../../../../src/terralib/dataaccess/datasource/DataSource.h"
#include "../../../../src/terralib/dataaccess/datasource/DataSourceFactory.h"
#include "../../../../src/terralib/dataaccess/datasource/DataSourceInfo.h"
#include "../../../../src/terralib/dataaccess/datasource/DataSourceManager.h"
#include "../../../../src/terralib/dataaccess/query/EqualTo.h"


#include "../../../../src/terralib/datatype/Property.h"
#include "../../../../src/terralib/datatype/SimpleProperty.h"
#include "../../../../src/terralib/datatype/StringProperty.h"

#include "../../../../src/terralib/geometry/Geometry.h"
#include "../../../../src/terralib/geometry/GeometryProperty.h"

#include "../../../../src/terralib/memory/DataSet.h"
#include "../../../../src/terralib/memory/DataSetItem.h"

#include "../../../../src/terralib/raster/RasterProperty.h"
#include "../../../../src/terralib/raster/RasterFactory.h" 
#include "../../../../src/terralib/raster/Utils.h"
#include "../../../../src/terralib/raster/BandProperty.h"
#include "../../../../src/terralib/geometry/PointZ.h"
#include "../../../../src/terralib/geometry/MultiLineString.h"
#include "../../../../src/terralib/common/UnitsOfMeasureManager.h"
#include "../../../../src/terralib/srs/SpatialReferenceSystemManager.h"
#include "../../../../src/terralib/statistics/core/Enums.h"

#include "Profile.h"

//STL
#include <cassert>

//Boost
#include <boost/lexical_cast.hpp>



te::mnt::Profile::Profile()
{
}

te::mnt::Profile::~Profile()
{
}
void te::mnt::Profile::setInput(te::da::DataSourcePtr inRasterDsrc, std::string inRasterName, std::auto_ptr<te::da::DataSetType> inDsetType)
{
  m_inRasterDsrc = inRasterDsrc;
  m_inRasterName = inRasterName;
  m_inRasterDsType = inDsetType;
}

std::vector<te::gm::LineString*> te::mnt::Profile::prepareVector(std::string &inDsetName, te::da::DataSourcePtr &inDsrc, std::string &geostype)
{
  std::vector<te::gm::LineString*> lsOut;
  std::auto_ptr<te::da::DataSet> inDset;
  //size_t nsamples = mpt.getNumGeometries();
  inDset = inDsrc->getDataSet(inDsetName);
  
  std::size_t geo_pos = te::da::GetFirstPropertyPos(inDset.get(), te::dt::GEOMETRY_TYPE);
  inDset->moveBeforeFirst();


  while (inDset->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> gin = inDset->getGeometry(geo_pos);
    geostype = gin.get()->getGeometryType();

    if (geostype == "LineString")
    {
      te::gm::LineString *l = dynamic_cast<te::gm::LineString*>(gin.get());
      lsOut.push_back(l);
    }
    if (geostype == "MultiLineString")
    {
      te::gm::MultiLineString *g = dynamic_cast<te::gm::MultiLineString*>(gin.get());
      std::size_t np = g->getNumGeometries();
      for (std::size_t i = 0; i < np; ++i)
      {
        te::gm::LineString *l = dynamic_cast<te::gm::LineString*>(g->getGeometryN(i)->clone());
        lsOut.push_back(l);
      }
    }
  }
  return lsOut;
}
std::auto_ptr<te::mem::DataSet> te::mnt::Profile::createDataSet(te::da::DataSet* inputDataSet, te::da::DataSetType* dsType)
{
  std::auto_ptr<te::mem::DataSet> outDataset(new te::mem::DataSet(dsType));

  std::size_t nProp = inputDataSet->getNumProperties();

  inputDataSet->moveBeforeFirst();

  while (inputDataSet->moveNext())
  {
    //create dataset item
    te::mem::DataSetItem* outDSetItem = new te::mem::DataSetItem(outDataset.get());

    for (std::size_t t = 0; t < nProp; ++t)
    {
      te::dt::AbstractData* ad = inputDataSet->getValue(t).release();

      outDSetItem->setValue(t, ad);
    }

    //set kernel default value
    outDSetItem->setDouble("TE_MNT_CREATEISOLINES_ATTR_NAME", 0.);

    //add item into dataset
    outDataset->add(outDSetItem);
  }

  return outDataset;
}

std::auto_ptr<te::rst::Raster> te::mnt::Profile::getPrepareRaster()
{
  std::auto_ptr<te::da::DataSetType>dsTypeRaster = m_inRasterDsrc->getDataSetType(m_inRasterName);
  //prepare raster
  te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(dsTypeRaster.get());
  std::auto_ptr<te::da::DataSet> dsRaster = m_inRasterDsrc->getDataSet(m_inRasterName);
  std::auto_ptr<te::rst::Raster> raster = dsRaster->getRaster(rasterProp->getName());
  return raster;
}


bool te::mnt::Profile::runRasterProfile(std::auto_ptr<te::rst::Raster> raster, std::vector<te::gm::LineString*> visadas, std::vector< std::vector<te::gm::LineString*> >& profileSet)
{
  double distbase = 0;
  int ind_pf = -1;
  int ind_pfant = -1;

  double xmin, xmax, ymin, ymax, zval, pt1X, pt1Y, pt2X, pt2Y;
  int col, row;

  //te::gm::Coord2D pt1Theme, pt2Theme, collin;
  te::gm::Coord2D collin;

  std::map<double, double> line;
  
  te::common::UnitOfMeasurePtr unitin = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(m_srid);

  std::vector<te::gm::LineString*>::iterator itVisadas = visadas.begin();
  
  //itVisadas será uma linha e para isso tem que percorrer os pontos dela
  while (itVisadas != visadas.end())
  {
    //int t = (*itVisadas)->getNPoints();
    for (std::size_t i = 0; i < (*itVisadas)->getNPoints(); i++)
    {
      //pt1Theme = itVisadas[i]->getX(i);
      //pt1Theme = itVisadas[i]->getY;

      pt1X = itVisadas[i]->getX(i);
      pt1Y = itVisadas[i]->getY(i);

      //collin = raster->getGrid()->geoToGrid(pt1Theme.getX(), pt1Theme.getY());
      collin = raster->getGrid()->geoToGrid(pt1X, pt1Y);
      col = (unsigned int)collin.getX();
      row = (unsigned int)collin.getY();

      //raster->getValue(col, row, zval);
      //if (zval < 0 || zval > 0)
      line.insert(std::map<double, double>::value_type(0.0, zval));

      ind_pf++;
      //i++;
      te::gm::Point ptEnd((*itVisadas)->getEndPoint()->getX(), (*itVisadas)->getEndPoint()->getY());

      while (itVisadas[i]->getPointN(i) != (*itVisadas)->getEndPoint())
      {
        //pt2Theme = itVisadas[i]->getX;
       // pt2Theme = itVisadas[i]->getY;
        pt2X = itVisadas[i]->getX(i);
        pt2Y = itVisadas[i]->getY(i);

        if (pt1X > pt2X)
        {
          xmin = pt2X;
          xmax = pt1X;
        }
        else
        {
          xmin = pt1X;
          xmax = pt2X;
        }

        if (pt1Y > pt2Y)
        {
          ymin = pt2Y;
          ymax = pt1Y;
        }
        else
        {
          ymin = pt1Y;
          ymax = pt2Y;
        }

        double boxLowerLeft = raster->getExtent()->getLowerLeftX(); //x1 = lowerLeftX
        double boxUpperRigth = raster->getExtent()->getUpperRightY(); //y2 = UpperRigthY

        double resX = raster->getResolutionX();
        double resY = raster->getResolutionY();

        int initcol = (int)((xmin - boxLowerLeft) / resX);
        int finalcol = (int)((xmax - boxLowerLeft) / resX + .9999);
        int initline = (int)((boxUpperRigth - ymax) / resY);
        int finalline = (int)((boxUpperRigth - ymin) / resY + .9999);

        double dx = pt2X - pt1X;
        double dy = pt2Y - pt1Y;

        //Calculate intersections of the segment with the columns
        if (((finalcol - initcol) > 1) && (fabs((double)dx) > 1.0e-6))
        {
          for (int c = (initcol + 1); c < finalcol; c++)
          {
            double x = (boxLowerLeft + c * resX);
            double y = (pt1Y + (dy / dx) * (x - pt1X));

            collin = raster->getGrid()->geoToGrid(x, y);
            col = (unsigned int)collin.getX();
            row = (unsigned int)collin.getY();

            ind_pf++;

            raster->getValue(col, row, zval);
            double dist = sqrt(((double)(x - pt1X)*(double)(x - pt1X)) + ((double)(y - pt1Y)*(double)(y - pt1Y))) + distbase;
            line.insert(std::map<double, double>::value_type(dist, zval));
          }
        }

        //Calculate intersections of the segment with the lines
        if (((finalline - initline) > 1) && (fabs((double)dy) > 1.0e-6))
        {
          for (int l = (initline + 1); l < finalline; l++)
          {
            //Calculate intersections of the segment with the bottom line
            double y = boxUpperRigth - (l * resY);
            double x = pt1X + (dx / dy) * (y - pt1Y);

            collin = raster->getGrid()->geoToGrid(x, y);
            col = (unsigned int)collin.getX();
            row = (unsigned int)collin.getY();

            ind_pf++;
            raster->getValue(col, row, zval);
            //Calculates the distance of the current point with point x1, y1
            double dist = sqrt(((double)(x - pt1X)*(double)(x - pt1X)) + ((double)(y - pt1Y)*(double)(y - pt1Y))) + distbase;
            line.insert(std::map<double, double>::value_type(dist, zval));
          }
        }
       
        //Stores second end (pt2) in the profile structure
        collin = raster->getGrid()->geoToGrid(pt2X, pt2Y);
        col = (unsigned int)collin.getX();
        row = (unsigned int)collin.getY();
        ind_pf++;

        raster->getValue(col, row, zval);
        double dist = sqrt(((double)dx * (double)dx) + ((double)dy* (double)dy)) + distbase;

        if (ind_pf != ind_pfant)
          distbase = dist;

        ind_pfant = ind_pf;
        pt1X = pt2X;
        pt1Y = pt2Y;
        line.insert(std::map<double, double>::value_type(dist, zval));
        //i++;
      }

      te::gm::LineString* profile = new te::gm::LineString(0, te::gm::LineStringType);
      std::map<double, double>::iterator it = line.begin();

      while (it != line.end())
      {
        profile->setNumCoordinates(profile->size() + 1);
        profile->setPoint(profile->size() - 1, it->first, it->second);
        it++;
      }
      profileSet[i].push_back(profile);

      line.clear();
      itVisadas++;
      distbase = 0;

    }
  }
  return true;
}

/*
while (itVisadas != visadas.end())
{
bool flag = false;

//Stores first end (pt1) in the profile structure
//std::vector<te::gm::LineString*>::iterator itLine = itVisadas->begin();

//std::vector<te::gm::LineString*>::iterator itLine = itVisadas->begin();

for (int i = 0; i < visadas.size(); i++)
{
//onde estiver itLine será itLine[i]
}
while (flag == false && itLine != itVisadas->end())
{
pt1Theme = *itLine;
collin = raster->getGrid()->geoToGrid(pt1Theme.getX, pt1Theme.getY);
col = collin.getX();
row = collin.getY();
//collin = raster->coord2Index(pt1Theme);
//col = TeRound(collin.x());
//row = TeRound(collin.y());

line.insert(std::map<double, double>::value_type(0.0, zval));
flag = true;

    }

    if (itLine == itVisadas->end())
    {
      itVisadas++;
      continue;
    }

    ind_pf++;
    itLine++;

    while (itLine != itVisadas->end())
    {
      pt2Theme = *itLine;

      if (pt1Theme.getX > pt2Theme.getX)
      {
        xmin = pt2Theme.getX;
        xmax = pt1Theme.getX;
      }
      else
      {
        xmin = pt1Theme.getX;
        xmax = pt2Theme.getX;
      }

      if (pt1Theme.getY > pt2Theme.getY)
      {
        ymin = pt2Theme.getY;
        ymax = pt1Theme.getY;
      }
      else
      {
        ymin = pt1Theme.getY;
        ymax = pt2Theme.getY;
      }

      double boxLowerLeft = raster->getExtent()->getLowerLeftX(); //x1 = lowerLeftX
      double boxUpperRigth = raster->getExtent()->getUpperRightY(); //y2 = UpperRigthY

      double resX = raster->getResolutionX();
      double resY = raster->getResolutionY();

      int initcol = (int)((xmin - boxLowerLeft) / resX);
      int finalcol = (int)((xmax - boxLowerLeft) / resX + .9999);
      int initline = (int)((boxUpperRigth - ymax) / resY);
      int finalline = (int)((boxUpperRigth - ymin) / resY + .9999);

      double dx = pt2Theme.getX - pt1Theme.getX;
      double dy = pt2Theme.getY - pt1Theme.getY;

      //Calculate intersections of the segment with the columns
      if (((finalcol - initcol) > 1) && (fabs((double)dx) > 1.0e-6))
      {
        for (int c = (initcol + 1); c < finalcol; c++)
        {
          double x = (boxLowerLeft + c * resX);
          double y = (pt1Theme.getY + (dy / dx) * (x - pt1Theme.getX));

          collin = raster->getGrid()->geoToGrid(x, y); //collin = raster->GeoToGrid(TeCoord2D(x, y));
          col = collin.getX();
          row = collin.getY();

          ind_pf++;

          raster->getValue(col, row, zval);
          double dist = sqrt(((double)(x - pt1Theme.getX)*(double)(x - pt1Theme.getX)) + ((double)(y - pt1Theme.getY)*(double)(y - pt1Theme.getY))) + distbase;
          line.insert(std::map<double, double>::value_type(dist, zval));
        }
      }

      //Calculate intersections of the segment with the lines
      if (((finalline - initline) > 1) && (fabs((double)dy) > 1.0e-6))
      {
        for (int l = (initline + 1); l < finalline; l++)
        {
          //Calculate intersections of the segment with the bottom line
          double y = boxUpperRigth - (l * resY);
          double x = pt1Theme.getX + (dx / dy) * (y - pt1Theme.getY);

          collin = raster->getGrid()->geoToGrid(x, y);
          col = collin.getX();
          row = collin.getY();

          ind_pf++;
          raster->getValue(col, row, zval);
          //Calculates the distance of the current point with point x1, y1
          double dist = sqrt(((double)(x - pt1Theme.getX)*(double)(x - pt1Theme.getX)) + ((double)(y - pt1Theme.getY)*(double)(y - pt1Theme.getY))) + distbase;
          line.insert(std::map<double, double>::value_type(dist, zval));
        }
      }
      //Stores second end (pt2) in the profile structure

      collin = raster->getGrid()->geoToGrid(pt2Theme.getX, pt2Theme.getY);
      col = collin.getX();
      row = collin.getY();
      ind_pf++;

      raster->getValue(col, row, zval);
      double dist = sqrt(((double)dx * (double)dx) + ((double)dy* (double)dy)) + distbase;

      if (ind_pf != ind_pfant)
        distbase = dist;

      ind_pfant = ind_pf;
      pt1Theme = pt2Theme;
      line.insert(std::map<double, double>::value_type(dist, zval));
      itLine++;
    }
    te::gm::LineString* profile = new te::gm::LineString(0, te::gm::LineStringType);
    std::map<double, double>::iterator it = line.begin();

    while (it != line.end())
    {
      profile->setNumCoordinates(profile->size() + 1);
      profile->setPoint(profile->size() - 1, it->first, it->second);
      it++;
    }
    profileSet[0].push_back(profile);

    line.clear();
    itVisadas++;
    distbase = 0;
  }
*/
