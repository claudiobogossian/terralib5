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
  \file terralib/rp/FeederRaster.h
  \brief Raster objects feeder.
 */

#ifndef __TERRALIB_RP_INTERNAL_FEEDERRASTER_H
#define __TERRALIB_RP_INTERNAL_FEEDERRASTER_H

#include "Config.h"
#include "AbstractFeeder.h"
#include "../raster/Raster.h"

namespace te
{
  namespace rp
  {
    /*!
      \class FeederRaster
      \brief Raster objects feeder.
      \ingroup RPModule
     */
    class TERPEXPORT FeederRaster : public AbstractFeeder< te::rst::Raster >
    {
      public :
        
        FeederRaster();
        
        virtual ~FeederRaster();
        
        //overloads
        virtual te::rst::Raster* getCurrentObj() = 0; 
        virtual te::rst::Raster const* getCurrentObj() const = 0;
        virtual bool moveNext() = 0;
        virtual bool isInitialized() const = 0;
        virtual bool reset() = 0;
        virtual unsigned int getObjsCount() const = 0;
        virtual unsigned int getCurrentOffset() const = 0;        
    };
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_FEEDERRASTER_H

