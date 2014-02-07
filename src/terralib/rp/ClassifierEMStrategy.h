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
  \file terralib/rp/ClassifierEMStrategy.h

  \brief EM (Expectation-Maximization) strategy for pixel-based classification.
*/

#ifndef __TERRALIB_RP_INTERNAL_CLASSIFIEREMSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_CLASSIFIEREMSTRATEGY_H

// TerraLib
#include "ClassifierStrategy.h"
#include "ClassifierStrategyFactory.h"
#include "Config.h"

// STL
#include <vector>

// Boost
#include <boost/numeric/ublas/matrix.hpp>

namespace te
{
  namespace rp
  {
    /*!
      \class ClassifierEMStrategy

      \brief EM strategy for pixel-based classification. This is an unsupervised
      and pixel-based classification algorithm. Expectation-Maximization (EM) works
      iteratively by applying two steps: the E-step (Expectation) and the M-step
      (Maximization). The method aims to approximate the parameter estimates to
      real data distribution, along the iterations:
      1. The E-step calculates the conditional expectation of the complete a
      posteriori probability function.
      2. The M-step updates the parameter estimation.
      
      \ingroup rp_class
    */
    class TERPEXPORT ClassifierEMStrategy : public ClassifierStrategy
    {
      public:

        /*!
          \class Parameters

          \brief Classifier Parameters
        */
        class TERPEXPORT Parameters : public StrategyParameters
        {
          public:

            unsigned int m_numberOfClusters;                    //!< The number of clusters (classes) to estimate in the image.
            unsigned int m_maxIterations;                       //!< The maximum of iterations (E/M steps) to perform if convergence is not achieved.
            unsigned int m_maxInputPoints;                      //!< The maximum number of points used to estimate the clusters (default = 1000).
            double m_epsilon;                                   //!< The stop criteria. When the clusters change in a value smaller then epsilon, the convergence is achieved.
            std::vector<std::vector<double> > m_clustersMeans;  //!< The previously estimated means of the clusters (optional).

            Parameters();

            ~Parameters();

            //overload
            const Parameters& operator=(const Parameters& params);

            //overload
            void reset() throw( te::rp::Exception );

            //overload
            AbstractParameters* clone() const;
        };

      public:

        ClassifierEMStrategy();

        ~ClassifierEMStrategy();

        //overload
        bool initialize(StrategyParameters const* const strategyParams) throw(te::rp::Exception);

        //overload
        bool execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                     const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                     const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception);

      protected:

        bool m_isInitialized;                                        //!< True if this instance is initialized.
        ClassifierEMStrategy::Parameters m_parameters;               //!< Internal execution parameters.

    };

    /*!
      \class ClassifierEMStrategyFactory

      \brief Raster EM Classifier strategy factory.
    */
    class TERPEXPORT ClassifierEMStrategyFactory : public ClassifierStrategyFactory
    {
      public:

        ClassifierEMStrategyFactory();

        ~ClassifierEMStrategyFactory();

        //overload
        te::rp::ClassifierStrategy* build();
    };

  } // end namespace rp
}   // end namespace te

#endif // __TERRALIB_RP_INTERNAL_CLASSIFIEREMSTRATEGY_H
