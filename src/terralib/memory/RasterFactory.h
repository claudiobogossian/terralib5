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
  \file terralib/memory/RasterFactory.h

  \brief This is the concrete factory for In-Memory driver.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_RASTERFACTORY_H
#define __TERRALIB_MEMORY_INTERNAL_RASTERFACTORY_H

// TerraLib
#include "../raster/RasterFactory.h"
#include "Config.h"

namespace te
{
  namespace mem
  {
    /*!
      \class RasterFactory

      \brief This is the abstract factory for Rasters.

      \sa te::rst::RasterFactory
    */
    class TEMEMORYEXPORT RasterFactory : public te::rst::RasterFactory
    {
      public:

        ~RasterFactory();

        const std::string& getType() const;

        void getCreationalParameters(std::vector< std::pair<std::string, std::string> >& params) const;

        std::map<std::string, std::string> getCapabilities() const;

      protected:

        RasterFactory();

        te::rst::Raster* create(te::rst::Grid* g, const std::vector<te::rst::BandProperty*> bands, const std::map<std::string, std::string>& rinfo, void* h = 0, void (*deleter)(void*) = 0);

        te::rst::Raster* build();

      private:

        static RasterFactory sm_factory;
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_RASTERFACTORY_H
