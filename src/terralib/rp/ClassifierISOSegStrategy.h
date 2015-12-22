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
  \file terralib/rp/ClassifierISOSegStrategy.h
  \brief ISOSeg strategy for segmentation-based classification.
 */

#ifndef __TERRALIB_RP_INTERNAL_CLASSIFIERISOSEGSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_CLASSIFIERISOSEGSTRATEGY_H

// TerraLib
#include "ClassifierStrategy.h"
#include "ClassifierStrategyFactory.h"
#include "Config.h"

// STL
#include <map>

// Boost
#include <boost/numeric/ublas/matrix.hpp>

namespace te
{
  namespace rp
  {
    /*!
      \class ClassifierISOSegStrategy

      \brief ISOSeg strategy for OBIA classification. The algorithm orders regions by area (larger first),
             and classify the largest region as Cluster 1. All regions similar to this cluster are inserted
             in Cluster 1, otherwise new Clusters are created. After all regions belong to a cluster, the
             algorithm merges similar clusters. The acceptance threshold is the only parameter given by
             the user, and it indicates the maximum distance between two regions to be clustered togheter.

       \ingroup rp_class
     */
    class TERPEXPORT ClassifierISOSegStrategy : public ClassifierStrategy
    {
      public:

        /*!
          \class Parameters

          \brief Classifier Parameters
        */
        class TERPEXPORT Parameters : public StrategyParameters
        {
          public:

            double m_acceptanceThreshold;   //!< The acceptance threshold (the closer to 100\%, few clusters are created).

 		    int m_outputNumberBands;  
	        
			std::vector< unsigned int > m_outputDataType;

			Parameters();

            ~Parameters();

            //overload
            const Parameters& operator=(const Parameters& params);

            //overload
            void reset() throw( te::rp::Exception );

            //overload
            AbstractParameters* clone() const;
        };

        /*!
          \class Pattern

          \brief Describes a region or a cluster (group of regions with similar properties) to be used by ISOSeg method.
        */
        class TERPEXPORT Pattern
        {
          public:

            Pattern();

            /*!
              \brief Constructor.

              \param i       The id of the initial region.
              \param a       The area of the region.
              \param mv      The vector of mean values, 1 value per band.
              \param cm      The covariance matrix between bands.
            */
            Pattern(int i, double a, std::vector<double> mv, boost::numeric::ublas::matrix<double> cm);

            /*!
              \brief Copy constructor.

              \param rhs The right-hand-side copy that would be used to copy from.
            */
            Pattern(Pattern& rhs);

            ~Pattern();

            /*!
              \brief Add a region inside a cluster.

              \param p       The region to be added.
            */
            void add(Pattern* p);

            /*! \brief Returns the Mahalanobis distance between two patterns */
            double getDistance(Pattern* p);

            /*! \brief Return true if two clusters are equal. */
            bool operator=(Pattern& rhs);

            int m_id;                                                     //!< The id of the region of the pattern.
            Pattern* m_myCluster;                                         //!< The associated cluster of this pattern (optional).
            double m_area;                                                //!< The area of all regions inside a pattern.
            std::vector<double> m_meanVector;                             //!< The vector of mean values, 1 value per band;
            boost::numeric::ublas::matrix<double> m_covarianceMatrix;     //!< The covariance matrix between bands.
            boost::numeric::ublas::matrix<double> m_covarianceInversion;  //!< The inversion of covariance matrix between bands.
        };

      public:

        ClassifierISOSegStrategy();

        ~ClassifierISOSegStrategy();

        //overload
        bool initialize(StrategyParameters const* const strategyParams) throw(te::rp::Exception);

        //overload
        bool execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                     const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                     const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception);
		
		std::vector< unsigned int > getOutputDataType(void); 

		unsigned int getOutputNumberBands(void);

      protected:

        bool m_isInitialized;                                             //!< True if this instance is initialized.
        ClassifierISOSegStrategy::Parameters m_parameters;                //!< Internal execution parameters.
        std::multimap<double, Pattern*, std::greater<double> > m_regions; //!< A descriptive set of regions (area, features).
    };

    /*!
      \class ClassifierISOSegStrategyFactory

      \brief Raster ISOSeg Classifier strategy factory.

      \note Factory key: RegionGrowing
    */
    class TERPEXPORT ClassifierISOSegStrategyFactory : public ClassifierStrategyFactory
    {
      public:

        ClassifierISOSegStrategyFactory();

        ~ClassifierISOSegStrategyFactory();

        //overload
        te::rp::ClassifierStrategy* build();
    };

  } // end namespace rp
}   // end namespace te

#endif // __TERRALIB_RP_INTERNAL_CLASSIFIERISOSEGSTRATEGY_H
