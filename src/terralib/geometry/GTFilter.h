/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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

#include "Config.h"
#include "GTParameters.h"
#include "GeometricTransformation.h"

#include <memory>
#include <vector>
#include <map>

#include <boost/thread.hpp>

namespace te
{
  namespace gm
  {
    /*!
      \class GTFilter
      
      \brief 2D Geometric transformation tie-points filter (outliers remotion).
    */
    class TEGEOMEXPORT GTFilter
    {
      public:
        
        /*! \brief RANSAC iterations counter type. */
        typedef unsigned long long int RansacItCounterT;        

        /*! \brief Destructor. */
        ~GTFilter();
        
        /*!
          \brief Apply a RANSAC based outliers remotion strategy.

          \param transfName Transformation name (see te::gm::GTFactory dictionary for reference).
          
          \param inputParams Input transformation parameters.
          
          \param expectedDirectMapRmse The expected direct mapping root mean square error.
          
          \param expectedInverseMapRmse The expected inverse mapping root mean square error.
          
          \param maxIterations The maximum number of iterations (Use 0-zero to let this number be automatically found).
          
          \param assurance The error-free selection percent assurance - valid range (0-1) - Use Lower values good tie-points sets - Use 0-zero to let this number be automatically found.
          
          \param enableMultiThread Enable multi-threaded processing (good for multi-processor or multi-core systems).
          
          \param outTransf The generated output transformation.
          
          \param tiePointsWeights Optional tie-points weights (non-zero, positive values) or an empty vector if no weights must be used.

          \return true if OK, false on errors.
          
          \note Reference: Martin A. Fischler and Robert C. Bolles, Random Sample Consensus: A Paradigm for Model Fitting with Applications to Image Analysis and Automated Cartography, Communications of the ACM  archive, Volume 24 , Issue 6  (June 1981).
        */
        static bool applyRansac(const std::string& transfName, 
          const GTParameters& inputParams,
          const double expectedDirectMapRmse, const double expectedInverseMapRmse,
          const RansacItCounterT& maxIterations,
          const double& assurance,
          const bool enableMultiThread,
          std::auto_ptr< GeometricTransformation >& outTransf,
          const std::vector< double >& tiePointsWeights );
        
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
            double m_expectedDirectMapRmse;
            double m_expectedInverseMapRmse;
            double m_assurance;
            RansacItCounterT m_maxIterations;
            RansacItCounterT m_maxIterationsDivFactor;
            bool* m_returnValuePtr;
            boost::mutex* m_mutexPtr;
            bool* m_keepRunningFlagPtr;
            std::map< double, GTParameters::TiePoint > const* m_tpsMapPtr; //!< A map from accumulated probabilities (normalized between 0 and 1) to tie-points data.
            std::auto_ptr< GeometricTransformation >* m_bestTransformationPtrPtr;
            double* m_bestParamsDRMSEPtr;
            double* m_bestParamsIRMSEPtr;
            double* m_bestParamsConvexHullAreaPtr;
            
            ApplyRansacThreadEntryThreadParams() {};
            
            ApplyRansacThreadEntryThreadParams( const ApplyRansacThreadEntryThreadParams& other )
            {
              operator=( other );
            };
            
            ~ApplyRansacThreadEntryThreadParams() {};
            
            const ApplyRansacThreadEntryThreadParams& operator=(
              const ApplyRansacThreadEntryThreadParams& other )
            {
              m_transfNamePtr = other.m_transfNamePtr;
              m_inputGTParamsPtr = other.m_inputGTParamsPtr;
              m_expectedDirectMapRmse = other.m_expectedDirectMapRmse;
              m_expectedInverseMapRmse = other.m_expectedInverseMapRmse;
              m_assurance = other.m_assurance;
              m_maxIterations = other.m_maxIterations;
              m_maxIterationsDivFactor = other.m_maxIterationsDivFactor;
              m_returnValuePtr = other.m_returnValuePtr;
              m_mutexPtr = other.m_mutexPtr;
              m_keepRunningFlagPtr = other.m_keepRunningFlagPtr;
              m_tpsMapPtr = other.m_tpsMapPtr;
              m_bestTransformationPtrPtr = other.m_bestTransformationPtrPtr;
              m_bestParamsDRMSEPtr = other.m_bestParamsDRMSEPtr;
              m_bestParamsIRMSEPtr = other.m_bestParamsIRMSEPtr;
              m_bestParamsConvexHullAreaPtr = other.m_bestParamsConvexHullAreaPtr;
              
              return other;
            };
        };
        

        /*! \brief Default constructor. */
        GTFilter();

        /*!
          \brief Copy constructor.

          \param rhs The input filter.
        */
        GTFilter(const GTFilter& rhs);

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side filter.

          \return A reference for this.
        */
        GTFilter& operator=(const GTFilter& rhs);
        
        /*!
          \brief Returns the tie-points convex hull area (GTParameters::TiePoint::first).

          \param tiePoints Tie points.

          \return The tie-points convex hull area 
        */
        static double getPt1ConvexHullArea( 
          const std::vector< GTParameters::TiePoint >& tiePoints );
          
        /*! 
          \brief Surf locator thread entry.
          
          \param paramsPtr A pointer to the thread parameters.
        */      
        static void applyRansacThreadEntry( 
          te::gm::GTFilter::ApplyRansacThreadEntryThreadParams* paramsPtr);          

    };
  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOMETRICTRANSFORMATION_H

