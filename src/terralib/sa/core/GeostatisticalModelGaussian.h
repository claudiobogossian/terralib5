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
  \file terralib/sa/core/GeostatisticalModelGaussian.h

  \brief This file contains a class that represents the geostatistical gaussian model.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_GEOSTATISTICALMODELGAUSSIAN_H
#define __TERRALIB_SA_INTERNAL_GEOSTATISTICALMODELGAUSSIAN_H

// Terralib Includes
#include "GeostatisticalModel.h"

namespace te
{
  namespace sa
  {
    /*!
      \class GeostatisticalModelGaussian

      \brief Class that represents the geostatistical gaussian model.

             gamma(h)=(s-n)\left(1-\exp\left(-\frac{h^2}{r^2a}\right)\right) + n1_{(0,\infty)}(h)

      \sa GeostatisticalModel
    */
    class TESAEXPORT GeostatisticalModelGaussian : public te::sa::GeostatisticalModel
    {
      public:

        /*! \brief Default constructor. */
        GeostatisticalModelGaussian();

        /*! \brief Virtual destructor. */
        virtual ~GeostatisticalModelGaussian();

      public:

        /*! \brief Function to calculate the geostatistical information from model. */
        virtual boost::numeric::ublas::matrix<double> calculate(boost::numeric::ublas::matrix<double> matrix);

    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_GEOSTATISTICALMODELGAUSSIAN_H
