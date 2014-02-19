/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/memory/ExpansibleRasterFactory.h

  \brief This is the concrete factory for expansible raster.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_EXPANSIBLERASTERFACTORY_H
#define __TERRALIB_MEMORY_INTERNAL_EXPANSIBLERASTERFACTORY_H

// TerraLib
#include "../raster/RasterFactory.h"
#include "Config.h"

namespace te
{
  namespace mem
  {
    /*!
      \class ExpansibleRasterFactory

      \briefThis is the concrete factory for expansible raster.
      
      \note The factory ID for this factory is EXPANSIBLE.
      
      \note This factory accepts the following raster infor (rinfo) parameter: MAXMEMPERCENTUSED - The maximum free memory percentual to use valid range: [1:100].
      
      \note This factory accepts the following raster infor (rinfo) parameter: MAXNUMBEROFRAMBLOCKS - The maximum number of RAM blocks.
      
      \sa te::rst::RasterFactory
    */
    class TEMEMORYEXPORT ExpansibleRasterFactory : public te::rst::RasterFactory
    {
      public:

        ~ExpansibleRasterFactory();

        const std::string& getType() const;

        void getCreationalParameters(std::vector< std::pair<std::string, std::string> >& params) const;

        std::map<std::string, std::string> getCapabilities() const;

      protected:

        ExpansibleRasterFactory();

        te::rst::Raster* create(te::rst::Grid* g, const std::vector<te::rst::BandProperty*> bands, const std::map<std::string, std::string>& rinfo, void* h = 0, void (*deleter)(void*) = 0);

        te::rst::Raster* build();

      private:

        static ExpansibleRasterFactory sm_factory;
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_EXPANSIBLERASTERFACTORY_H
