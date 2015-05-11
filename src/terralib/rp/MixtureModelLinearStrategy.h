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
  \file terralib/rp/MixtureModelLinearStrategy.h

  \brief Raster linear strategy for mixture model classification.
 */

#ifndef __TERRALIB_RP_INTERNAL_MIXTUREMODELLINEARSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_MIXTUREMODELLINEARSTRATEGY_H

// TerraLib
#include "MixtureModelStrategy.h"
#include "MixtureModelStrategyFactory.h"
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
      \class MixtureModelLinearStrategy

      \ingroup rp_class
     */
    class TERPEXPORT MixtureModelLinearStrategy : public MixtureModelStrategy
    {
      public:

        /*!
          \class Parameters

          \brief MixtureModel Parameters
        */
        class TERPEXPORT Parameters : public StrategyParameters
        {
          public:

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

        MixtureModelLinearStrategy();

        ~MixtureModelLinearStrategy();

        //overload
        bool initialize(StrategyParameters const* const strategyParams) throw(te::rp::Exception);

        //overload
        bool execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                     const std::vector<std::string>& inputSensorBands, const std::map<std::string, std::vector<double> >& components,
                     te::rst::Raster& outputRaster, const bool enableProgressInterface) throw(te::rp::Exception);

      protected:

        bool m_isInitialized;                                             //!< True if this instance is initialized.
        MixtureModelLinearStrategy::Parameters m_parameters;              //!< Internal execution parameters.
    };

    /*!
      \class MixtureModelLinearStrategyFactory

      \brief Raster linear mixture model strategy factory.

      \note Factory key: RegionGrowing
    */
    class TERPEXPORT MixtureModelLinearStrategyFactory : public MixtureModelStrategyFactory
    {
      public:

        MixtureModelLinearStrategyFactory();

        ~MixtureModelLinearStrategyFactory();

        //overload
        te::rp::MixtureModelStrategy* build();
    };

  } // end namespace rp
}   // end namespace te

#endif // __TERRALIB_RP_INTERNAL_MIXTUREMODELLINEARSTRATEGY_H
