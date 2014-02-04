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
  \file terralib/rp/MixtureModelPCAStrategy.h

  \brief PCA (Principal Component Analysis) strategy for mixture model.
 */

#ifndef __TERRALIB_RP_INTERNAL_MIXTUREMODELPCASTRATEGY_H
#define __TERRALIB_RP_INTERNAL_MIXTUREMODELPCASTRATEGY_H

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
      \class MixtureModelPCAStrategy

      \ingroup rp_class
     */
    class TERPEXPORT MixtureModelPCAStrategy : public MixtureModelStrategy
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

        MixtureModelPCAStrategy();

        ~MixtureModelPCAStrategy();

        //overload
        bool initialize(StrategyParameters const* const strategyParams) throw(te::rp::Exception);

        //overload
        bool execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                     const std::vector<std::string>& inputSensorBands, const std::map<std::string, std::vector<double> >& components,
                     te::rst::Raster& outputRaster, const bool enableProgressInterface) throw(te::rp::Exception);

      protected:

        bool m_isInitialized;                                             //!< True if this instance is initialized.
        MixtureModelPCAStrategy::Parameters m_parameters;                 //!< Internal execution parameters.
    };

    /*!
      \class MixtureModelPCAStrategyFactory

      \brief Raster PCA mixture model strategy factory.

      \note Factory key: RegionGrowing
    */
    class TERPEXPORT MixtureModelPCAStrategyFactory : public MixtureModelStrategyFactory
    {
      public:

        MixtureModelPCAStrategyFactory();

        ~MixtureModelPCAStrategyFactory();

        //overload
        te::rp::MixtureModelStrategy* build();
    };

  } // end namespace rp
}   // end namespace te

#endif // __TERRALIB_RP_INTERNAL_MIXTUREMODELPCASTRATEGY_H
