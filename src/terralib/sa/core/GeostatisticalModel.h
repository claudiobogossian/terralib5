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
  \file terralib/sa/core/GeostatisticalModel.h

  \brief This file contains a virtual class that represents the geostatistical model.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_GEOSTATISTICALMODEL_H
#define __TERRALIB_SA_INTERNAL_GEOSTATISTICALMODEL_H

// Terralib Includes
#include "../Config.h"
#include "../Enums.h"

// STL Includes
#include <memory>

// Boost
#include <boost/numeric/ublas/matrix.hpp>

namespace te
{
  namespace sa
  {
    /*!
      \class GeostatisticalModel

      \brief The empirical variogram cannot be computed at every lag distance h and due to variation in the estimation 
             it is not ensured that it is a valid variogram, as defined above. However some Geostatistical methods 
             such as kriging need valid semivariograms. In applied geostatistics the empirical variograms are thus 
             often approximated by model function ensuring validity (Chiles&Delfiner 1999).

      \sa GeostatisticalMethod
    */
    class TESAEXPORT GeostatisticalModel
    {
      public:

        /*! \brief Default constructor. */
        GeostatisticalModel()
        {
          m_nugget = 0.;
          m_sill = 0.;
          m_range = 0.;
        };

        /*! \brief Virtual destructor. */
        virtual ~GeostatisticalModel(){};

      public:

        /*! \brief Function to calculate the geostatistical information from model. */
        virtual boost::numeric::ublas::matrix<double> calculate(boost::numeric::ublas::matrix<double> matrix) = 0;

        /*! \brief Function to set the nugget value. */
        void setNugget(double value)
        {
          m_nugget = value;
        }

        /*! \brief Function to get the nugget value. */
        double getNugget()
        {
          return m_nugget;
        }

        /*! \brief Function to set the sill value. */
        void setSill(double value)
        {
          m_sill = value;
        }

        /*! \brief Function to get the sill value. */
        double getSill()
        {
          return m_sill;
        }

        /*! \brief Function to set the range value. */
        void setRange(double value)
        {
          m_range = value;
        }

        /*! \brief Function to get the range value. */
        double getRange()
        {
          return m_range;
        }

      protected:

        double m_nugget;              //!< The height of the jump of the semivariogram at the discontinuity at the origin.

        double m_sill;                //!< Limit of the variogram tending to infinity lag distances.

        double m_range;               //!< The distance in which the difference of the variogram from the sill becomes negligible. 

        te::sa::GeostatisticalModelType m_type; //!< Attribute used to define the model type used.

    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_GEOSTATISTICALMODEL_H
