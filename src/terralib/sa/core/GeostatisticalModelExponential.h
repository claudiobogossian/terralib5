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
  \file terralib/sa/core/GeostatisticalModelExponential.h

  \brief This file contains a class that represents the geostatistical exponential model.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_GEOSTATISTICALMODELEXPONENTIAL_H
#define __TERRALIB_SA_INTERNAL_GEOSTATISTICALMODELEXPONENTIAL_H

// Terralib Includes
#include "GeostatisticalModel.h"

namespace te
{
  namespace sa
  {
    /*!
      \class GeostatisticalModelExponential

      \brief Class that represents the geostatistical exponential model.

             gamma(h)=(s-n)(1-\exp(-h/(ra)))+n 1_{(0,\infty)}(h)

      \sa GeostatisticalModel
    */
    class TESAEXPORT GeostatisticalModelExponential : public te::sa::GeostatisticalModel
    {
      public:

        /*! \brief Default constructor. */
        GeostatisticalModelExponential();

        /*! \brief Virtual destructor. */
        virtual ~GeostatisticalModelExponential();

      public:

        /*! \brief Function to calculate the geostatistical information from model. */
        virtual boost::numeric::ublas::matrix<double> calculate(boost::numeric::ublas::matrix<double> matrix);

    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_GEOSTATISTICALMODELEXPONENTIAL_H
