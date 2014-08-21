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
  \file terralib/sa/core/GeostatisticalFunctions.h

  \brief Auxiliary functions used to calculate the geostatistical methods.

  \reference Methods adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_GEOSTATISTICALFUNCTIONS_H
#define __TERRALIB_SA_INTERNAL_GEOSTATISTICALFUNCTIONS_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

// Boost
#include <boost/numeric/ublas/matrix.hpp>

namespace te
{
  // Forward declaration
  namespace da { class DataSet; }
  namespace gm { class Geometry; }

  namespace sa
  {
    struct GeostatisticalDataItem
    {
      double m_value;
      te::gm::Geometry* m_geom;

      GeostatisticalDataItem()
      {
        m_value = 0.;
        m_geom = 0;
      }

      ~GeostatisticalDataItem()
      {
        delete m_geom;
      }

    };

    typedef std::vector<te::sa::GeostatisticalDataItem*> GeostatisticalData;

    /*!
      \brief Function used to create a matrix with values, distance and angle for each element from dataset.

      \param dataSet Pointer to input data.
      \param attrIdx Attribute index used to create the matrix.
      \param geomIdx Attribute index used to identify the geometric attribute.

      \return A matrix with values from dataset (main diagonal), distance (top of the matrix), angle (bottom of the matrix). 

    */
    TESAEXPORT boost::numeric::ublas::matrix<double> CreateMatrixFromDataSet(te::da::DataSet* dataSet, int attrIdx, int geomIdx);

    /*!
      \brief Function used to set new values in the matrix main diagonal.

      \param matrix Reference to a valid matrix.
      \param dataSet Pointer to input data.
      \param attrIdx Attribute index used to reset the diagonal values.

      \return A matrix with values from dataset (main diagonal), distance (top of the matrix), angle (bottom of the matrix). 

    */
    TESAEXPORT void SetMainDiagonal(boost::numeric::ublas::matrix<double>& matrix, te::da::DataSet* dataSet, int attrIdx);

    /*!
      \brief Function used to calculate mean and variance from a matrix.

      \param matrix Reference to a valid matrix.
      \param mean Reference to set the mean value.
      \param variance Reference to set the variance value.

    */
    TESAEXPORT void CalculateMoments(const boost::numeric::ublas::matrix<double>& matrix, double& mean, double& variance);

    TESAEXPORT te::sa::GeostatisticalData CreateGeostatisticalData(te::da::DataSet* dataSet, int attrIdx, int geomIdx);


  } // end namespace sa
}   // end namespace te

#endif  // __TERRALIB_SA_INTERNAL_GEOSTATISTICALFUNCTIONS_H

