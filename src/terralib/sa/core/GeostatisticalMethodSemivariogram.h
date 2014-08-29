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
  \file terralib/sa/core/GeostatisticalMethodSemivariogram.h

  \brief This file contains a class that represents the geostatistical semivariogram method.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_GEOSTATISTICALMETHODSEMIVARIOGRAM_H
#define __TERRALIB_SA_INTERNAL_GEOSTATISTICALMETHODSEMIVARIOGRAM_H

// Terralib Includes
#include "GeostatisticalMethod.h"

namespace te
{
  namespace sa
  {
    /*!
      \class GeostatisticalMethodSemivariogram

      \brief Semivariogram is a function describing the degree of spatial dependence of a spatial random field.
             It is defined as the variance of the difference between field values at two locations (x and y) 
             across realizations of the field.

    */
    class TESAEXPORT GeostatisticalMethodSemivariogram : public te::sa::GeostatisticalMethod
    {
      public:

        /*! \brief Default constructor. */
        GeostatisticalMethodSemivariogram();

        /*! \brief Virtual destructor. */
        virtual ~GeostatisticalMethodSemivariogram();

      public:

        /*! \brief Function to calculate the geostatistical information. */
        virtual boost::numeric::ublas::matrix<double> calculate();
    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_GEOSTATISTICALMETHODSEMIVARIOGRAM_H