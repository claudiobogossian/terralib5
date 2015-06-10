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
  \file terralib/sa/core/GeostatisticalFunctions.cpp

  \brief Auxiliary functions used to calculate the geostatistical methods.

  \reference Methods adapted from TerraLib4.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/Coord2D.h"
#include "GeostatisticalFunctions.h"
#include "Utils.h"

// STL
#include <cassert>

boost::numeric::ublas::matrix<double> te::sa::CreateMatrixFromDataSet(te::da::DataSet* dataSet, int attrIdx, int geomIdx)
{
  assert(dataSet);

  //create geostatistical data
  te::sa::GeostatisticalData geoData = CreateGeostatisticalData(dataSet, attrIdx, geomIdx);

  //create output matrix
  boost::numeric::ublas::matrix<double> matrix(geoData.size(), geoData.size());

  //fill matrix
  for(std::size_t i = 0; i < geoData.size(); ++i)
  {
    te::sa::GeostatisticalDataItem* itemI = geoData[i];

    //set diagonal value
    matrix(i, i) = itemI->m_value;

    te::gm::Coord2D coordI = te::sa::GetCentroidCoord(itemI->m_geom);

    for(std::size_t j = i + 1; j < geoData.size(); ++j)
    {
      te::sa::GeostatisticalDataItem* itemJ = geoData[j];

      te::gm::Coord2D coordJ = te::sa::GetCentroidCoord(itemJ->m_geom);

      //calculate distance and set at the top of the matrix
      double distance = sqrt(pow((coordI.x - coordJ.x), 2) + pow((coordI.y - coordJ.y), 2));

      matrix(i, j) = distance;

      //calculate angle and set at the bottom of the matrix
      double angle = -( atan ((coordI.y - coordJ.y)/(coordI.x - coordJ.x)) * (180/3.14) ) + 90.0;

      matrix(j, i) = angle;
    }
  }

  //clear geostatistical data
  te::common::FreeContents(geoData);

  return matrix;
}

void te::sa::SetMainDiagonal(boost::numeric::ublas::matrix<double>& matrix, te::da::DataSet* dataSet, int attrIdx)
{
  assert(dataSet);

  dataSet->moveBeforeFirst();

  std::size_t t = 0;

  //reset only the main diagonal values
  while(dataSet->moveNext())
  {
    double value = te::sa::GetDataValue(dataSet->getValue(attrIdx).get());

    matrix(t, t) = value;

    ++t;
  }
}

void te::sa::CalculateMoments(const boost::numeric::ublas::matrix<double>& matrix, double& mean, double& variance)
{
  double sum = 0.;
  double sumSquare = 0.;

  for(std::size_t t = 0; t < matrix.size1(); ++t)
  {
    sum += matrix(t,t);
    sumSquare += pow(matrix(t,t), 2);
  }

  mean = sum / (double)matrix.size1();
  variance = (sumSquare / (double)matrix.size1()) - pow(mean, 2);
}

TESAEXPORT te::sa::GeostatisticalData te::sa::CreateGeostatisticalData(te::da::DataSet* dataSet, int attrIdx, int geomIdx)
{
  assert(dataSet);

  te::sa::GeostatisticalData geoData;

  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    te::sa::GeostatisticalDataItem* item = new te::sa::GeostatisticalDataItem();

    item->m_value = te::sa::GetDataValue(dataSet->getValue(attrIdx).get());
    item->m_geom = dataSet->getGeometry(geomIdx).release();

    geoData.push_back(item);
  }

  return geoData;
}