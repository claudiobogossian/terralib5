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
  \file terralib/rp/ClassifierEDStrategy.h
  \brief Euclidean Distance Classifier strategy.
 */

#ifndef __TERRALIB_RP_INTERNAL_CLASSIFIEREDSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_CLASSIFIEREDSTRATEGY_H

#include "ClassifierStrategy.h"
#include "ClassifierStrategyFactory.h"
#include "../common/progress/TaskProgress.h"
#include "Config.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/shared_ptr.hpp>

#include <map>
#include <vector>

namespace te
{
  namespace rp
  {
    /*!
      \class ClassifierEDStrategy

      \brief Euclidean Distance Classifier strategy.
      
      \ingroup rp_class
     */
    class TERPEXPORT ClassifierEDStrategy : public ClassifierStrategy
    {
      public:

        /*!
          \class Parameters

          \brief Classifier Parameters
        */
        class TERPEXPORT Parameters : public ClassifierStrategyParameters
        {
          public:
            
            typedef unsigned int ClassIDT; //!< Class ID type definition (zero means invalid ID).
            
            typedef std::vector< double > ClassSampleT; //!< Class sample type definition.
            
            typedef std::vector< ClassSampleT > ClassSamplesContainerT; //!< Class samples container type definition.
            
            typedef std::map< ClassIDT, ClassSamplesContainerT > MClassesSamplesCT; //!< Multi-classes samples container type definition.

            typedef boost::shared_ptr<MClassesSamplesCT> MClassesSamplesCTPtr; //!< A shared pointer to a multi classes samples container type definition.
            
            MClassesSamplesCTPtr m_trainSamplesPtr; //!< A shared pointer to a always-valid structure where trainning samples are stored.

            Parameters();

            ~Parameters();

            //overload
            const Parameters& operator=(const Parameters& params);

            //overload
            void reset() throw( te::rp::Exception );

            //overload
            AbstractParameters* clone() const;
        };

        ClassifierEDStrategy();

        ~ClassifierEDStrategy();

        //overload
        bool initialize(ClassifierStrategyParameters const* const strategyParams) throw(te::rp::Exception);

        //overload
        bool execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                     const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                     const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception);

        // overload
        std::vector< int > getOutputDataTypes() const; 

         unsigned int getOutputNumberBands(void);

      protected :

        bool m_isInitialized; //!< Is this instance initialized?

        ClassifierEDStrategy::Parameters m_initParams; //!< Initialization parameters.

        std::vector< Parameters::ClassIDT > m_classesIndex2ID; //!< An class index ordered vector of classes IDs;

        std::vector< std::vector< double > > m_classesMeans; //!< Classes means;

    };

    /*!
      \class ClassifierEDStrategyFactory

      \brief Euclidean Distance strategy factory.

      \note Factory key: RegionGrowing
    */
    class TERPEXPORT ClassifierEDStrategyFactory : public ClassifierStrategyFactory
    {
      public:

        ClassifierEDStrategyFactory();

        ~ClassifierEDStrategyFactory();

        //overload
        te::rp::ClassifierStrategy* build();
    };

  } // end namespace rp
}   // end namespace te

#endif // __TERRALIB_RP_INTERNAL_CLASSIFIEREDSTRATEGY_H

