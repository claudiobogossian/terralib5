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
  \file terralib/rp/ClassifierKMeansStrategy.h

  \brief KMeans strategy for image classification.
*/

#ifndef __TERRALIB_RP_INTERNAL_CLASSIFIERKMEANSSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_CLASSIFIERKMEANSSTRATEGY_H

#include "ClassifierStrategy.h"
#include "ClassifierStrategyFactory.h"
#include "Config.h"

namespace te
{
  namespace rp
  {
    /*!
      \class ClassifierKMeansStrategy

      \brief KMeans strategy for image classification.
             Step-by-step:
             1. Define a random solution, creating K values of mean.
             2. Classify each pixel according to the closest mean.
             3. Calculate new means using the classified pixels.
             4. Go back to step 2, or stop depending on the following checks:
             5. Check if the new means are different from the previous iteration,
                using the convergence threshold.
             6. Check if maximum number of iterations has achieved.
             
       \ingroup rp_class
    */
    class TERPEXPORT ClassifierKMeansStrategy : public ClassifierStrategy
    {
      public:

        /*!
          \class Parameters

          \brief Classifier Parameters
        */
        class TERPEXPORT Parameters : public ClassifierStrategyParameters
        {
          public:

            unsigned int m_K;                    //!< The number of clusters (means) to detect in image.
            unsigned int m_maxIterations;        //!< The maximum of iterations to perform if convergence is not achieved.
            unsigned int m_maxInputPoints;       //!< The maximum number of points used to estimate the clusters (default = 1000).
            double m_epsilon;                    //!< The stop criteria. When the clusters change in a value smaller then epsilon, the convergence is achieved.

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

        ClassifierKMeansStrategy();

        ~ClassifierKMeansStrategy();

        //overload
        bool initialize(ClassifierStrategyParameters const* const strategyParams) throw(te::rp::Exception);

        //overload
        bool execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                     const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                     const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception);
				
        // overload
        std::vector< int > getOutputDataTypes() const; 

      protected:

        bool m_isInitialized;                                        //!< True if this instance is initialized.
        ClassifierKMeansStrategy::Parameters m_parameters;           //!< Internal execution parameters.

    };

    /*!
      \class ClassifierKMeansStrategyFactory

      \brief Raster KMeans Classifier strategy factory.

      \note Factory key: RegionGrowing
    */
    class TERPEXPORT ClassifierKMeansStrategyFactory : public ClassifierStrategyFactory
    {
      public:

        ClassifierKMeansStrategyFactory();

        ~ClassifierKMeansStrategyFactory();

        //overload
        te::rp::ClassifierStrategy* build();
    };

  } // end namespace rp
}   // end namespace te

#endif // __TERRALIB_RP_INTERNAL_CLASSIFIERKMEANSSTRATEGY_H

