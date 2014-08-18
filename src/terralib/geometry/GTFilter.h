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
  \file terralib/geometry/GTFilter.h

  \brief 2D Geometric transformation outliers remotion filter.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GTFILTER_H
#define __TERRALIB_GEOMETRY_INTERNAL_GTFILTER_H

// TerraLib
#include "Config.h"
#include "GTParameters.h"
#include "GeometricTransformation.h"

// STL
#include <map>
#include <memory>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

namespace te
{
  namespace gm
  {
    /*!
      \class GTFilter
      
      \brief 2D Geometric transformation tie-points filter (outliers remotion).
    */
    class TEGEOMEXPORT GTFilter : boost::noncopyable
    {
      public:
        
        /*! \brief RANSAC integer counter type. */
        typedef unsigned long int RansacIntegerCounterT;
        
        /*! \brief RANSAC floating point counter type. */
        typedef long double RansacFloatCounterT;

        /*! \brief Default constructor. */
        GTFilter();

        /*! \brief Destructor. */
        ~GTFilter();

        /*!
          \brief Apply a RANSAC based outliers remotion strategy.

          \param transfName             Transformation name (see te::gm::GTFactory dictionary for reference).
          \param inputParams            Input transformation parameters.
          \param maxDirectMapError      The maximum allowed direct mapping error.
          \param maxInverseMapError     The maximum allowed inverse mapping error.
          \param maxIterations          The maximum number of iterations (Use 0-zero to let this number be automatically found).
          \param assurance              The error-free selection assurance - valid range (0-1) - Use Lower values for good tie-points sets - Higher values may increase the number of iterations.
          \param enableMultiThread      Enable multi-threaded processing (good for multi-processor or multi-core systems).
          \param outTransf              The generated output transformation (with the base mininum required tie-points set).
          \param tiePointsWeights       Optional tie-points weights (only positive values) or an empty vector if no weights must be used.
          \param outTiePoints           The filtered output tie-points (non-outliers) in agreenment with the generated transformation.

          \return true if OK, false on errors.

          \note Reference: Martin A. Fischler and Robert C. Bolles, Random Sample Consensus: A Paradigm for Model Fitting with Applications to Image Analysis and Automated Cartography, Communications of the ACM archive, Volume 24, Issue 6 (June 1981).
        */
        bool applyRansac(const std::string& transfName,
                         const GTParameters& inputParams,
                         const double maxDirectMapError, 
                         const double maxInverseMapError,
                         const RansacIntegerCounterT& maxIterations,
                         const double& assurance,
                         const bool enableMultiThread,
                         const std::vector< double >& tiePointsWeights,
                         std::vector< te::gm::GTParameters::TiePoint >& outTiePoints,
                         std::auto_ptr< GeometricTransformation >& outTransf
                         );

      private:

        /*!
          \class ApplyRansacThreadEntryThreadParams

          \brief Parameters used by the GTFilter::applyRansacThreadEntry method.
        */
        class ApplyRansacThreadEntryThreadParams
        {
          public:

            std::string const* m_transfNamePtr;
            GTParameters const* m_inputGTParamsPtr;
            double m_maxDirectMapError;
            double m_maxInverseMapError;
            double m_assurance;
            bool m_useDynamicIterationsNumber;
            RansacIntegerCounterT* m_dynamicMaxIterationsPtr;
            RansacIntegerCounterT m_procsNumber;
            bool* m_returnValuePtr;
            boost::mutex* m_mutexPtr;
            bool* m_keepRunningFlagPtr;
            std::map< double, GTParameters::TiePoint > const* m_tpsMapPtr; //!< A map from accumulated probabilities (normalized between 0 and 1) to tie-points data.
            std::auto_ptr< GeometricTransformation >* m_bestTransformationPtrPtr;
            double* m_bestParamsMaxDMapErrorPtr;
            double* m_bestParamsMaxIMapErrorPtr;
            double* m_bestParamsConvexHullAreaPtr;
            std::vector< te::gm::GTParameters::TiePoint >* m_bestTiePoinsPtr;

            ApplyRansacThreadEntryThreadParams();

            ApplyRansacThreadEntryThreadParams( const ApplyRansacThreadEntryThreadParams& other );

            ~ApplyRansacThreadEntryThreadParams();

            const ApplyRansacThreadEntryThreadParams& operator=(
              const ApplyRansacThreadEntryThreadParams& other );
        };

        /*!
          \brief Returns the tie-points convex hull area (GTParameters::TiePoint::first).

          \param tiePoints Tie points.

          \return The tie-points convex hull area 
        */
        static double getPt1ConvexHullArea(const std::vector<GTParameters::TiePoint>& tiePoints);

        /*! 
          \brief Surf locator thread entry.

          \param paramsPtr A pointer to the thread parameters.
        */
        static void applyRansacThreadEntry(te::gm::GTFilter::ApplyRansacThreadEntryThreadParams* paramsPtr);

    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GTFILTER_H

