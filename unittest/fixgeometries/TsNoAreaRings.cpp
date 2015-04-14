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
  \file TsNoAreaRings.cpp
 
  \brief A test suit for the Fix Geometries class.
 */

// Unit-Test Terralib
#include "TsNoAreaRings.h"
#include "../Config.h"

// Terralib
#include <terralib/Defines.h>
#include <terralib/common.h>
#include <terralib/common/Globals.h>
#include <terralib/geometry.h>

// STL
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Boost
#include <boost/shared_ptr.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( TsNoAreaRings );

void TsNoAreaRings::tcCreatePolygon()
{
  //#ifdef TE_COMPILE_ALL
  
  te::gm::LinearRing* line = new te::gm::LinearRing(4, te::gm::LineStringType, 4326);
  line->setPoint(0, 1.0, 1.5);
  line->setPoint(1, 1.0, 3.3);
  line->setPoint(2, 3.0, 3.3);
  line->setPoint(3, 3.0, 1.5);

  te::gm::Polygon* poly = new te::gm::Polygon(1, te::gm::PolygonType, 4326);
  poly->setRingN(0, line);

  //test para garantir que ele eh invalido
  try
  {
    CPPUNIT_ASSERT(poly->isValid() == false);
  }
  catch (const std::exception& e)
  {
  }

  //fix geometry
  //line->setPointN(4, te::gm::Point(1.0, 1.5, 4326));
  
  //teste para garantir que ele eh valido
    
  try
  {
    CPPUNIT_ASSERT(poly->isValid() == true);
  }
  catch (const std::exception& e)
  {
    CPPUNIT_ASSERT(false);
  }

  //#endif
}
