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
 \file VectorToRaster.h
 
 \brief Vector to Raster processing.
 
 \ingroup attributefill
 */

#ifndef __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTOR_TO_RASTER_H
#define __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTOR_TO_RASTER_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"

#include "../datatype/Property.h"

#include "../memory/DataSet.h"
#include "../statistics/core/Enums.h"

#include "Config.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace attributefill
  {
    class TEATTRIBUTEFILLEXPORT VectorToRaster
    {
    public:

      VectorToRaster();

      virtual ~VectorToRaster() {}

    };
  }
}
#endif // __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTOR_TO_RASTER_H