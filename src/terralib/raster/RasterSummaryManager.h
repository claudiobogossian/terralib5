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
  \file terralib/raster/RasterSummaryManager.h
 
  \brief A singleton for keeping raster summaries (most statistics).
*/

#ifndef __TERRALIB_RASTER_INTERNAL_RASTERSUMMARYMANAGER_H
#define __TERRALIB_RASTER_INTERNAL_RASTERSUMMARYMANAGER_H

// TerraLib
#include "../common/Singleton.h"
#include "Config.h"
#include "Enums.h"
#include "RasterSummary.h"

// Boost
#include <boost/ptr_container/ptr_map.hpp>

namespace te
{
  namespace rst
  {
// Forward declaration
    class Raster;

    /*!
      \class RasterSummaryManager

      \brief A singleton for keeping raster summaries (most statistics).
             It stores an internal map of raster references and their
             respective summaries.

      \ingroup rst

      \sa RasterSummary, BandSummary.
    */
    class TERASTEREXPORT RasterSummaryManager : public te::common::Singleton<RasterSummaryManager>
    {
      friend class te::common::Singleton<RasterSummaryManager>;

      public:

        /*!
          \brief Adds a new entry in the summary manager.

          \param raster      The raster to be added.
          \param summary     The raster summary of the respective raster.
                             The manager will take its ownership.

          \note If the raster was already added, the summary will be replaced.
        */
        void add(const Raster* raster, RasterSummary* summary);

        /*!
          \brief It searches for a raster summary.

          \param raster The raster to be found.

          \return The calculated raster summary, or a null summary otherwise.
        */
        const RasterSummary* find(const Raster* raster) const;

        /*!
          \brief Removes the summary from the specified raster.

          \param raster The raster to remove the summary.
        */
        void remove(const Raster* raster);

        /*!
          \brief It searches for a raster summary. If not found it creates the summary and returns it.

          \param raster      The raster to be found.
          \param types       The desired types of summary to be calculated (min, max, ...).
          \param readall     Force the reading the entire image (can be slow) for computing min and max values.

          \return The calculated raster summary.
        */
        const RasterSummary* get(const Raster* raster, const SummaryTypes st, bool readall = false);

        /*! \brief Destructor. */
        ~RasterSummaryManager();

      protected:

        /*! \brief Constructor. */
        RasterSummaryManager();

      private:

        boost::ptr_map<const Raster*, RasterSummary> m_rasterSummaries;    //!< A map of rasters and their respective summaries.
    };

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_RASTERSUMMARYMANAGER_H

