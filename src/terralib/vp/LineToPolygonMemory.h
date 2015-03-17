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
  \file LineToPolygonMemory.h
   
  \brief 
 
  \ingroup vp
*/

#ifndef __TERRALIB_VP_INTERNAL_LINETOPOLYGON_MEMORY_H
#define __TERRALIB_VP_INTERNAL_LINETOPOLYGON_MEMORY_H

//Terralib
#include "../datatype/Property.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../geometry/Geometry.h"
#include "../memory/DataSet.h"
#include "../statistics/core/Enums.h"

#include "LineToPolygonOp.h"
#include "Config.h"
#include "Enums.h"
#include "Exception.h"


// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace vp
  {
    class TEVPEXPORT LineToPolygonMemory : public LineToPolygonOp
    {

    public:

      LineToPolygonMemory();

      ~LineToPolygonMemory();

      bool run();

    private:

      std::auto_ptr<te::gm::MultiPolygon> line2Polygon(te::gm::Geometry* geom);

      void getPolygons(te::gm::Geometry* geom, std::vector<te::gm::Polygon*>& polygons);

      void getPolygons(te::gm::GeometryCollection* gc, std::vector<te::gm::Polygon*>& polygons);

      void getPolygons(te::gm::LineString* l, std::vector<te::gm::Polygon*>& polygons);

      void getPolygons(te::gm::Polygon* p, std::vector<te::gm::Polygon*>& polygons);

    }; // end class
  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_LINETOPOLYGON_MEMORY_H

