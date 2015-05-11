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
  \file terralib/rp/ClassifierDummyStrategy.h

  \brief Dummy strategy (just for testing purposes).
*/

#ifndef __TERRALIB_RP_INTERNAL_CLASSIFIERDUMMYSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_CLASSIFIERDUMMYSTRATEGY_H

#include "ClassifierStrategy.h"
#include "ClassifierStrategyFactory.h"
#include "Config.h"

namespace te
{
  namespace rp
  {
    /*!
      \class ClassifierDummyStrategy

      \brief Dummy strategy (just for testing purposes).
      
      \ingroup rp_class
    */
    class TERPEXPORT ClassifierDummyStrategy : public ClassifierStrategy
    {
      public:

        /*!
          \class Parameters

          \brief Classifier Parameters
        */
        class TERPEXPORT Parameters : public StrategyParameters
        {
          public:

            unsigned int m_dummyParameter;  //!< A dummy parameter.

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

        ClassifierDummyStrategy();

        ~ClassifierDummyStrategy();

        //overload
        bool initialize(StrategyParameters const* const strategyParams) throw(te::rp::Exception);

        //overload
        bool execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                     const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                     const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception);

      protected:

        bool m_isInitialized;                                        //!< True if this instance is initialized.
        ClassifierDummyStrategy::Parameters m_parameters;            //!< Internal execution parameters.
    };

    /*!
      \class ClassifierDummyStrategyFactory

      \brief Raster dummy Classifier strategy factory.

      \note Factory key: RegionGrowing
    */
    class TERPEXPORT ClassifierDummyStrategyFactory : public ClassifierStrategyFactory
    {
      public:

        ClassifierDummyStrategyFactory();

        ~ClassifierDummyStrategyFactory();

        //overload
        te::rp::ClassifierStrategy* build();
    };

  } // end namespace rp
}   // end namespace te

#endif // __TERRALIB_RP_INTERNAL_CLASSIFIERDUMMYSTRATEGY_H

