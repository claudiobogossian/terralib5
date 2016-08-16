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
  \file terralib/unittest/edit/movegeometry/TsMoveGeometry.cpp

  \brief A test suit for the Move Geometries.
*/

// TerraLib
#include "../Config.h"
#include <terralib/edit/Utils.h>
#include <terralib/geometry.h>

// STL
#include <memory>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(movegeometry_tests);

BOOST_AUTO_TEST_CASE(movePolygon_test)
{
  te::gm::LinearRing* line = new te::gm::LinearRing(5, te::gm::LineStringType, 4326);
  line->setPoint(0, 1.0, 1.5);
  line->setPoint(1, 1.0, 3.3);
  line->setPoint(2, 3.0, 3.3);
  line->setPoint(3, 3.0, 1.5);
  line->setPoint(4, 1.0, 1.5); // closing

  te::gm::Polygon* poly = new te::gm::Polygon(1, te::gm::PolygonType, 4326);
  poly->setRingN(0, line);

  te::gm::Point* pOrigin = poly->getCentroid();

  //move geometry
  te::edit::MoveGeometry(poly, 1, 1);

  double distance = pOrigin->distance(poly->getCentroid());

  //moved?
  BOOST_CHECK(distance > 0.);
}

BOOST_AUTO_TEST_SUITE_END()