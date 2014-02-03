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
  \file terralib/rp/ClassifierStrategy.h

  \brief Raster classifier strategy base class.
 */

#ifndef __TERRALIB_RP_INTERNAL_CLASSIFIERSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_CLASSIFIERSTRATEGY_H

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
      \class ClassifierStrategy
      \brief Raster classifier strategy base class.
     */
    class TERPEXPORT ClassifierStrategy
    {
      public:

        /*! \brief Virtual destructor. */
        virtual ~ClassifierStrategy();

        /*!
          \brief Initialize the classification strategy.

          \param strategyParams A pointer to the user given specific classification strategy parameters ou NULL if no parameters are present.

          \return true if OK, false on errors.
        */
        virtual bool initialize(StrategyParameters const* const strategyParams) throw(te::rp::Exception) = 0;

        /*!
          \brief Executes the classification strategy.

          \param inputRaster                Input raster.
          \param inputRasterBands           Input raster bands.
          \param inputPolygons              The polygons to be classified when using object-based image analysis (OBIA).
          \param outputRaster               Output raster.
          \param outputRasterBand           Output raster band.
          \param enableProgressInterface    Enable the internal strategy to update the progress interface.

          \return true if OK, false on errors.
        */
        virtual bool execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                             const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                             const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception) = 0;

      protected:

        /*! \brief Default constructor. */
        ClassifierStrategy();

      private:

        /*!
          \brief Copy constructor.

          \param rhs The right-hand side Raster.
        */
        ClassifierStrategy(const ClassifierStrategy& rhs);

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
        */
        const ClassifierStrategy& operator=(const ClassifierStrategy& rhs);
    };

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_CLASSIFIERSTRATEGY_H

