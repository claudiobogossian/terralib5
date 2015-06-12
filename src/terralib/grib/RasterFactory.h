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
  \file terralib/grib/RasterFactory.h

  \brief This is the concrete factory for GRIB.
*/

#ifndef __TERRALIB_GRIB_INTERNAL_RASTERFACTORY_H
#define __TERRALIB_GRIB_INTERNAL_RASTERFACTORY_H

// TerraLib
#include "../raster/RasterFactory.h"
#include "Config.h"

namespace te
{
  namespace grib
  {
    /*!
      \class RasterFactory

      \brief This is the abstract factory for Rasters.

      \sa te::rst::RasterFactory
    */
    class TEGRIBEXPORT RasterFactory : public te::rst::RasterFactory
    {
      public:

        ~RasterFactory();

        const std::string& getType() const;

        void getCreationalParameters(std::vector< std::pair<std::string, std::string> >& params) const;

        /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory DataSourceFactory. */
        static void initialize();

        /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory DataSourceFactory. */
        static void finalize();
		
        /*! \brief It return a map (string, string) containing all supported formats. */
        std::map<std::string, std::string> getCapabilities() const;

      protected:

        RasterFactory();

        te::rst::Raster* create(te::rst::Grid* g, const std::vector<te::rst::BandProperty*> bands, const std::map<std::string, std::string>& rinfo, void* h = 0, void (*deleter)(void*) = 0);

        te::rst::Raster* build();

      private:

        static RasterFactory* sm_factory;
    };    

  } // end namespace grib
}   // end namespace te

#endif  // __TERRALIB_GRIB_INTERNAL_RASTERFACTORY_H

