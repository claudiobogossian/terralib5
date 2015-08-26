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
  \file

  \brief
*/

#ifndef __TERRALIB_PLUGINEDIT_INTERNAL_STASHER_H
#define __TERRALIB_PLUGINEDIT_INTERNAL_STASHER_H

#include <map>
#include <set>
#include <string>

// Forward declarations.
namespace te
{
  namespace gm
  {
    class Geometry;
  }

  namespace map
  {
    class AbstractLayer;
  }
}

void StashGeometries(const te::map::AbstractLayer* layer, const std::map<std::string, te::gm::Geometry*>& geoms, const std::map<std::string, int>& ops);

void GetStashedGeometries(const te::map::AbstractLayer* layer, std::map<std::string, te::gm::Geometry*>& geoms, std::map<std::string, int>& ops);

std::set<std::string> GetStashedLayers();

void RemoveStash(const te::map::AbstractLayer* layer);

#endif  // __TERRALIB_PLUGINEDIT_INTERNAL_STASHER_H



