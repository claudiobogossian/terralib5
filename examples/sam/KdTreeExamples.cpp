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
  \file KdTreeExamples.cpp

  \brief This file contains several examples on how to use the K-d tree spatial access method.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/sam.h>
#include "SAMExamples.h"

// STL
#include <vector>
#include <utility>

void IndexPointUsingKdTree()
{
  typedef te::sam::kdtree::AdaptativeNode<te::gm::Coord2D, std::vector<te::gm::Point>, te::gm::Point> KDNODE;
  typedef te::sam::kdtree::AdaptativeIndex<KDNODE> KDTREE;

  std::size_t n = 100;

  std::vector<std::pair<te::gm::Coord2D, te::gm::Point> > dataset;

  te::gm::Envelope e;

  for(std::size_t i = 0; i < n; ++i)
  {
    te::gm::Coord2D coord(static_cast<double>(i), static_cast<double>(i));
    te::gm::Point point = te::gm::Point(coord.x, coord.y, 4326);

    e.Union(*point.getMBR());

    dataset.push_back(std::pair<te::gm::Coord2D, te::gm::Point>(coord, point));
  }

  KDTREE tree(e);

  tree.build(dataset);

  std::vector<KDNODE*> reports;

  tree.search(e, reports);
}
