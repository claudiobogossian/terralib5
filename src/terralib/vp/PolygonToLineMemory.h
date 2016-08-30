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
  \file PolygonToLineMemory.h
   
  \brief 
 
  \ingroup vp
*/

#ifndef __TERRALIB_VP_INTERNAL_POLYGONTOLINE_MEMORY_H
#define __TERRALIB_VP_INTERNAL_POLYGONTOLINE_MEMORY_H

//Terralib
#include "../common/Exception.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/LineString.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/Polygon.h"

#include "PolygonToLineOp.h"
#include "Config.h"
#include "Exception.h"

// STL
#include <memory>
#include <vector>

namespace te
{
  namespace vp
  {
    class TEVPEXPORT PolygonToLineMemory : public PolygonToLineOp
    {

    public:

      PolygonToLineMemory();

      ~PolygonToLineMemory();

      bool run() throw(te::common::Exception);

    private:

      std::auto_ptr<te::gm::MultiLineString> polygon2Line(te::gm::Geometry* geom);

      void getLines(te::gm::Geometry* geom, std::vector<te::gm::LineString*>& lines);

      void getLines(te::gm::GeometryCollection* gc, std::vector<te::gm::LineString*>& lines);

      void getLines(te::gm::Polygon* p, std::vector<te::gm::LineString*>& lines);

      void getLines(te::gm::LineString* l, std::vector<te::gm::LineString*>& lines);

    }; // end class
  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_POLYGONTOLINE_MEMORY_H

