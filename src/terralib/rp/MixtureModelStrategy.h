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
  \file terralib/rp/MixtureModelStrategy.h

  \brief Raster mixture model strategy base class.
 */

#ifndef __TERRALIB_RP_INTERNAL_MIXTUREMODELSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_MIXTUREMODELSTRATEGY_H

// TerraLib
#include "../raster/Raster.h"
#include "StrategyParameters.h"
#include "Config.h"
#include "Exception.h"

// STL
#include <vector>

namespace te
{
  namespace rp
  {
    /*!
      \class MixtureModelStrategy
      \brief Raster mixture model strategy base class.
     */
    class TERPEXPORT MixtureModelStrategy
    {
      public:

        /*! \brief Virtual destructor. */
        virtual ~MixtureModelStrategy();

        /*!
          \brief Initialize the segmentation strategy.

          \param strategyParams A pointer to the user given specific segmentation strategy parameters ou NULL if no parameters are present.

          \return true if OK, false on errors.
        */
        virtual bool initialize(StrategyParameters const* const strategyParams) throw(te::rp::Exception) = 0;

        /*!
          \brief Executes the segmentation strategy.

          \param inputRaster                Input raster.
          \param inputRasterBands           Input raster bands.
          \param components                 The set of endmembers and its radiances.
          \param outputRaster               Output raster.
          \param enableProgressInterface    Enable the internal strategy to update the progress interface.

          \return true if OK, false on errors.
        */
        virtual bool execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                             const std::vector<std::string>& inputSensorBands, const std::map<std::string, std::vector<double> >& components,
                             te::rst::Raster& outputRaster, const bool enableProgressInterface) throw(te::rp::Exception) = 0;

      protected:

        /*! \brief Default constructor. */
        MixtureModelStrategy();

      private:

        /*!
          \brief Copy constructor.

          \param rhs The right-hand side Raster.
        */
        MixtureModelStrategy(const MixtureModelStrategy& rhs);

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
        */
        const MixtureModelStrategy& operator=(const MixtureModelStrategy& rhs);
    };

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_MIXTUREMODELSTRATEGY_H

