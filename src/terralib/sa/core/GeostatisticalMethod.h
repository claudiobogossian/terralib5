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
  \file terralib/sa/core/GeostatisticalMethod.h

  \brief This file contains a virtual class that represents the geostatistical method.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_GEOSTATISTICALMETHOD_H
#define __TERRALIB_SA_INTERNAL_GEOSTATISTICALMETHOD_H

// Terralib Includes
#include "../Config.h"

// STL Includes
#include <memory>

// Boost
#include <boost/numeric/ublas/matrix.hpp>

namespace te
{
  namespace sa
  {
    /*!
      \class GeostatisticalMethod

      \brief Geostatistics is used for modelling spatial data. It provides accurate and 
             reliable estimations of phenomena at locations where no measurements are available.

             Geostatistics has been defined by G. Matheron as "the application of probabilistic methods 
             to regionalized variables", which designates any function displayed in a real space. 
             At the difference of conventional statistics, whatever the complexity and the irregularity 
             of the real phenomenon, geostatistics search to exhibit a structure of spatial correlation. 
             This accounts for the intuitive idea that points close in the space should be likely close 
             in values. In other words randomness does not mean independence.

    */
    class TESAEXPORT GeostatisticalMethod
    {
      public:

        /*! \brief Default constructor. */
        GeostatisticalMethod()
        {
          m_angleDirection = 0.;
          m_angleTolerance = 0.;
          m_lagIncrement = 0.;
          m_nLags = 0;
        };

        /*! \brief Virtual destructor. */
        virtual ~GeostatisticalMethod()
        {
          m_dataMatrix.clear();
        }

      public:

        /*! \brief Function to calculate the geostatistical information. */
        virtual boost::numeric::ublas::matrix<double> calculate() = 0;

        /*! \brief Function to set the angle direction. */
        void setAngleDirection(double value)
        {
          m_angleDirection = value;
        }

        /*! \brief Function to get the angle direction. */
        double getAngleDirection()
        {
          return m_angleDirection;
        }

        /*! \brief Function to set the angle tolerance. */
        void setAngleTolerance(double value)
        {
          m_angleTolerance = value;
        }

        /*! \brief Function to get the angle tolerance. */
        double getAngleTolerance()
        {
          return m_angleTolerance;
        }

        /*! \brief Function to set the lag increment. */
        void setLagIncrement(double value)
        {
          m_lagIncrement = value;
        }

        /*! \brief Function to get the lag increment. */
        double getLagIncrement()
        {
          return m_lagIncrement;
        }

        /*! \brief Function to set the number of lags. */
        void setNumberOfLags(std::size_t value)
        {
          m_nLags = value;
        }

        /*! \brief Function to get the number of lags. */
        std::size_t getNumberOfLags()
        {
          return m_nLags;
        }

        /*! \brief Function to set the input matrix with data information. */
        void setMatrix(boost::numeric::ublas::matrix<double> matrix)
        {
          m_dataMatrix = matrix;
        }

        /*! \brief Function to get the input matrix with data information. */
        boost::numeric::ublas::matrix<double>& getMatrix()
        {
          return m_dataMatrix;
        }

      protected:

        double m_angleDirection;      //!< Attribute used to define the direction used to calculate the variogram.

        double m_angleTolerance;      //!< Attribute used to define the minimum and maximum angular direction.

        double m_lagIncrement;        //!< Attribute used to define the distance between each lag.

        std::size_t m_nLags;          //!< Attribute used to specifies the basic distance unit.

        boost::numeric::ublas::matrix<double> m_dataMatrix; //!< Matrix with input data information.

    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_GEOSTATISTICALMETHOD_H