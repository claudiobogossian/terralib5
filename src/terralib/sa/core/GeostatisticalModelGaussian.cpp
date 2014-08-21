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
  \file terralib/sa/core/GeostatisticalModelGaussian.cpp

  \brief This file contains a class that represents the geostatistical gaussian model.

  \reference Adapted from TerraLib4.
*/

// Terralib Includes
#include "GeostatisticalModelGaussian.h"

te::sa::GeostatisticalModelGaussian::GeostatisticalModelGaussian() : te::sa::GeostatisticalModel()
{
  m_type = te::sa::Gaussian;
}

te::sa::GeostatisticalModelGaussian::~GeostatisticalModelGaussian()
{
}

boost::numeric::ublas::matrix<double> te::sa::GeostatisticalModelGaussian::calculate(boost::numeric::ublas::matrix<double> matrix)
{
  //get the number of lags used
  std::size_t nLags = matrix.size1();

  //create the model matrix
  boost::numeric::ublas::matrix<double> m(nLags + 1, 2);

  //fill matrix
  std::vector<double> gaussianValues;

  for(std::size_t t= 0; t < nLags; ++t)
  {
    if(matrix(t, 0) == 0.)
    {
      gaussianValues.push_back(m_nugget);
    }
    else
    {
      gaussianValues.push_back(m_nugget + m_sill * (1 - exp(-3 * pow(matrix(t, 0) / m_range, 2))));
    }

    m(t + 1, 0) = matrix(t, 0);
    m(t + 1, 1) = gaussianValues[t];
  }

  m(0, 1) = m_nugget;

  //set last value
  m(nLags, 0) = m(nLags - 1, 0) + (m(nLags - 1, 0) - m(nLags - 2, 0));
  m(nLags, 1) = m(nLags - 1, 1);

  return m;
}