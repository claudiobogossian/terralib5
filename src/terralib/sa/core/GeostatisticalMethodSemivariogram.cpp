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
  \file terralib/sa/core/GeostatisticalMethodSemivariogram.cpp

  \brief This file contains a class that represents the geostatistical semivariogram method.

  \reference Adapted from TerraLib4.
*/

//TerraLib Include
#include "GeostatisticalMethodSemivariogram.h"

te::sa::GeostatisticalMethodSemivariogram::GeostatisticalMethodSemivariogram() : te::sa::GeostatisticalMethod()
{
}

te::sa::GeostatisticalMethodSemivariogram::~GeostatisticalMethodSemivariogram()
{
}

boost::numeric::ublas::matrix<double> te::sa::GeostatisticalMethodSemivariogram::calculate()
{
  //increment tolerance will be 50% of the lag increment.
  double incrementTolerance = m_lagIncrement / 2.;

  //define the minimum and maximum directions
  double dirMin = 0.;
  double dirMax = 0.;

  if(m_angleTolerance != 90.0)
  {
    dirMin = m_angleDirection - m_angleTolerance;
    dirMax = m_angleDirection + m_angleTolerance;
  }

  //set increment value
  double incrementCount = 0.;

  //start aux vectors
  std::vector<double> np, gam, dis;
  np.resize(m_nLags, 0.);
  gam.resize(m_nLags, 0.);
  dis.resize(m_nLags, 0.);

  //calculate information for each lag
  for(std::size_t lag = 0; lag < m_nLags; ++ lag)
  {
    incrementCount += m_lagIncrement;

    double minDistance = incrementCount - incrementTolerance;
    double maxDistance = incrementCount + incrementTolerance;

    //fill matrix
    for(std::size_t line = 0; line < m_dataMatrix.size1() - 1; ++line)
    {
      for(std::size_t column = 0; column < m_dataMatrix.size2() - 1; ++column)
      {
        if(m_angleTolerance == 90.0) //Omnidirectional
        {
          if(line < column && m_dataMatrix(line, column) > 0. && m_dataMatrix(line, column) <= incrementTolerance && lag == 0)
          {
            np[lag] = np[lag] + 1.;
            dis[lag] = dis[lag] + m_dataMatrix(line, column);
            gam[lag] = gam[lag] + ((m_dataMatrix(line, line) - m_dataMatrix(column, column)) * (m_dataMatrix(line, line) - m_dataMatrix(column, column)));
          }
          else if(line < column && m_dataMatrix(line, column) > minDistance && m_dataMatrix(line, column) <= maxDistance && lag != 0)
          {
            np[lag] = np[lag] + 1.;
            dis[lag] = dis[lag] + m_dataMatrix(line, column);
            gam[lag] = gam[lag] + ((m_dataMatrix(line, line) - m_dataMatrix(column, column)) * (m_dataMatrix(line, line) - m_dataMatrix(column, column)));
          }
        }
        else //Directional
        {
          if(line < column && m_dataMatrix(line, column) > 0. && m_dataMatrix(line, column) <= incrementTolerance && m_dataMatrix(column, line) >= dirMin  && m_dataMatrix(column, line) <= dirMax && lag == 0)
          {
            np[lag] = np[lag] + 1.;
            dis[lag] = dis[lag] + m_dataMatrix(line, column);
            gam[lag] = gam[lag] + ((m_dataMatrix(line, line) - m_dataMatrix(column, column)) * (m_dataMatrix(line, line) - m_dataMatrix(column, column)));
          }
          else if(line < column && m_dataMatrix(line, column) > minDistance && m_dataMatrix(line, column) <= maxDistance && m_dataMatrix(column, line) >= dirMin && m_dataMatrix(column, line) <= dirMax && lag !=0)
          {
            np[lag] = np[lag] + 1.;
            dis[lag] = dis[lag] + m_dataMatrix(line, column);
            gam[lag] = gam[lag] + ((m_dataMatrix(line, line) - m_dataMatrix(column, column)) * (m_dataMatrix(line, line) - m_dataMatrix(column, column)));
          }
        }
      }
    }
  }

  //set output matrix
  boost::numeric::ublas::matrix<double> matrix(m_nLags, 2);

  for(std::size_t lag = 0; lag < m_nLags; ++ lag)
  {
    if(np[lag] != 0)
    {
      dis[lag] = dis[lag] / np[lag];
      gam[lag] = (gam[lag] / np[lag]) * .5;
    }

    matrix(lag,0) = dis[lag];
    matrix(lag,1) = gam[lag];
  }

  return matrix;
}