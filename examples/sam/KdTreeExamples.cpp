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
#include <limits>
#include <vector>
#include <utility>

void KdTree()
{
  // K-d Tree
  typedef te::sam::kdtree::Node<te::gm::Coord2D, std::size_t, std::size_t> KD_NODE;
  typedef te::sam::kdtree::Index<KD_NODE> KD_TREE;

  te::gm::Envelope e;

  KD_TREE tree(e);

  std::size_t n = 100;

  for(std::size_t i = 0; i < n; ++i)
  {
    te::gm::Coord2D coord(static_cast<double>(i), static_cast<double>(i));
    tree.insert(coord, i);
  }

  for(std::size_t i = 0; i < n; ++i)
  {
    std::vector<KD_NODE*> reports;

    te::gm::Envelope e(static_cast<double>(i), static_cast<double>(i), static_cast<double>(i), static_cast<double>(i));
    tree.search(e, reports);
  }
}

void AdaptativeKdTree()
{
  // Adaptative K-d Tree
  typedef te::sam::kdtree::AdaptativeNode<te::gm::Coord2D, std::vector<te::gm::Point>, te::gm::Point> KD_ADAPTATIVE_NODE;
  typedef te::sam::kdtree::AdaptativeIndex<KD_ADAPTATIVE_NODE> KD_ADAPTATIVE_TREE;

  std::vector<std::pair<te::gm::Coord2D, te::gm::Point> > dataset;

  te::gm::Envelope e;

  std::size_t n = 100;

  for(std::size_t i = 0; i < n; ++i)
  {
    te::gm::Coord2D coord(static_cast<double>(i), static_cast<double>(i));
    te::gm::Point point = te::gm::Point(coord.x, coord.y, 4326);

    e.Union(*point.getMBR());

    dataset.push_back(std::pair<te::gm::Coord2D, te::gm::Point>(coord, point));
  }

  KD_ADAPTATIVE_TREE adaptativeTree(e);
  adaptativeTree.build(dataset);

  // Search 1
  std::vector<KD_ADAPTATIVE_NODE*> reports;
  adaptativeTree.search(e, reports);

  // Search 2
  std::vector<te::gm::Point> reports2;
  adaptativeTree.search(e, reports2);

  te::gm::Coord2D coord = te::gm::Coord2D(28.0, 28.0);

  std::vector<te::gm::Point> points;
  points.push_back(te::gm::Point(std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));
  std::vector<double> sqrDists;

  adaptativeTree.nearestNeighborSearch(coord, points, sqrDists, 1);
}

void IndexPointUsingKdTree()
{
  KdTree();

  AdaptativeKdTree();
}
