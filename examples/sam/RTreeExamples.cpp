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
  \file RTreeExamples.cpp

  \brief This file contains several examples on how to use the R-tree spatial access method.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/sam.h>
#include "SAMExamples.h"

// STL
#include <vector>

void IndexPointUsingRTree()
{
  std::vector<te::gm::Point*> pointVec;  
  te::sam::rtree::Index<std::size_t, 4> rtree;

  pointVec.reserve(100);

  for(int i = 0; i < 100; ++i)
  {
    te::gm::Point* pt = new te::gm::Point(static_cast<double>(i), static_cast<double>(i), 4326);
    pointVec.push_back(pt);
    rtree.insert(*(pt->getMBR()), i);
  }

  for(int i = 0; i < 100; ++i)
  {
    std::vector<std::size_t> report;
    const te::gm::Envelope* mbr = pointVec[i]->getMBR();

    assert(rtree.search(*mbr, report) == 1);
    assert(report[0] == static_cast<std::size_t>(i));
  }

  te::common::FreeContents(pointVec);
}

