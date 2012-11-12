/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

// Unit-Test TerraLib
#include "TsGTFilter.h"
#include "../Config.h"

// TerraLib
#include <terralib/geometry/GTParameters.h>
#include <terralib/geometry/GTFactory.h>
#include <terralib/geometry/GTFilter.h>

// boost
#include <boost/timer.hpp>

#include <cmath> 

CPPUNIT_TEST_SUITE_REGISTRATION( TsGTFilter );

void TsGTFilter::setUp()
{
}

void TsGTFilter::tearDown()
{
}

void TsGTFilter::testDirectMapping( 
  te::gm::GeometricTransformation const * transfPtr,
  const te::gm::Coord2D& pt1, const te::gm::Coord2D& pt2, 
  const double& maxError )
{
  te::gm::Coord2D mappedCoord;
  
  transfPtr->directMap( pt1, mappedCoord );
  
  const double diffx = mappedCoord.x - pt2.x;
  const double diffy = mappedCoord.y - pt2.y;
  const double error = sqrt( ( diffx * diffx ) + ( diffy * diffy ) );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, error, maxError );
}


void TsGTFilter::testInverseMapping( 
  te::gm::GeometricTransformation const * transfPtr,
  const te::gm::Coord2D& pt1, const te::gm::Coord2D& pt2, 
  const double& maxError )
{
  te::gm::Coord2D mappedCoord;
  
  transfPtr->inverseMap( pt2, mappedCoord );
  
  const double diffx = mappedCoord.x - pt1.x;
  const double diffy = mappedCoord.y - pt1.y;
  const double error = sqrt( ( diffx * diffx ) + ( diffy * diffy ) );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, error, maxError );
}

void  TsGTFilter::generateTestTPSet1( 
  std::vector< te::gm::GTParameters::TiePoint >& tiePoints )
{
  tiePoints.clear();
  
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 277.00, 82.00), te::gm::Coord2D( 372.00, 780.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 597.00, 118.00), te::gm::Coord2D( 558.00, 827.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 361.00, 162.00), te::gm::Coord2D( 363.00, 795.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 594.00, 193.00), te::gm::Coord2D( 429.00, 895.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 326.00, 206.00), te::gm::Coord2D( 440.00, 589.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 403.00, 241.00), te::gm::Coord2D( 328.00, 243.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 408.00, 235.00), te::gm::Coord2D( 411.00, 246.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 529.00, 258.00), te::gm::Coord2D( 529.00, 258.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 238.00, 282.00), te::gm::Coord2D( 238.00, 282.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 249.00, 267.00), te::gm::Coord2D( 249.00, 267.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 306.00, 266.00), te::gm::Coord2D( 306.00, 266.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 352.00, 272.00), te::gm::Coord2D( 352.00, 272.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 393.00, 276.00), te::gm::Coord2D( 393.00, 276.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 451.00, 288.00), te::gm::Coord2D( 451.00, 288.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 479.00, 275.00), te::gm::Coord2D( 479.00, 275.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 503.00, 271.00), te::gm::Coord2D( 503.00, 271.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 548.00, 265.00), te::gm::Coord2D( 548.00, 265.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 587.00, 273.00), te::gm::Coord2D( 587.00, 273.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 632.00, 266.00), te::gm::Coord2D( 632.00, 266.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 236.00, 313.00), te::gm::Coord2D( 236.00, 313.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 285.00, 319.00), te::gm::Coord2D( 285.00, 319.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 414.00, 297.00), te::gm::Coord2D( 414.00, 297.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 462.00, 314.00), te::gm::Coord2D( 462.00, 314.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 511.00, 302.00), te::gm::Coord2D( 511.00, 302.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 529.00, 314.00), te::gm::Coord2D( 529.00, 314.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 557.00, 315.00), te::gm::Coord2D( 557.00, 315.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 603.00, 317.00), te::gm::Coord2D( 603.00, 317.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 645.00, 308.00), te::gm::Coord2D( 645.00, 308.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 298.00, 333.00), te::gm::Coord2D( 298.00, 333.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 364.00, 330.00), te::gm::Coord2D( 364.00, 330.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 380.00, 353.00), te::gm::Coord2D( 380.00, 353.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 431.00, 334.00), te::gm::Coord2D( 431.00, 334.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 486.00, 342.00), te::gm::Coord2D( 486.00, 342.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 502.00, 330.00), te::gm::Coord2D( 502.00, 330.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 524.00, 347.00), te::gm::Coord2D( 524.00, 347.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 556.00, 352.00), te::gm::Coord2D( 556.00, 352.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 626.00, 345.00), te::gm::Coord2D( 625.00, 346.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 657.00, 343.00), te::gm::Coord2D( 657.00, 343.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 274.00, 367.00), te::gm::Coord2D( 274.00, 367.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 371.00, 369.00), te::gm::Coord2D( 371.00, 369.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 410.00, 365.00), te::gm::Coord2D( 410.00, 365.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 467.00, 387.00), te::gm::Coord2D( 467.00, 387.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 644.00, 368.00), te::gm::Coord2D( 644.00, 368.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 231.00, 405.00), te::gm::Coord2D( 231.00, 405.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 251.00, 400.00), te::gm::Coord2D( 251.00, 400.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 277.00, 395.00), te::gm::Coord2D( 277.00, 395.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 305.00, 417.00), te::gm::Coord2D( 305.00, 417.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 373.00, 404.00), te::gm::Coord2D( 373.00, 404.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 382.00, 418.00), te::gm::Coord2D( 382.00, 418.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 426.00, 410.00), te::gm::Coord2D( 426.00, 410.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 492.00, 394.00), te::gm::Coord2D( 492.00, 394.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 577.00, 412.00), te::gm::Coord2D( 577.00, 412.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 605.00, 412.00), te::gm::Coord2D( 605.00, 412.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 53.00, 419.00), te::gm::Coord2D( 259.00, 776.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 458.00, 429.00), te::gm::Coord2D( 458.00, 429.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 476.00, 436.00), te::gm::Coord2D( 513.00, 956.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 496.00, 427.00), te::gm::Coord2D( 496.00, 427.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 540.00, 444.00), te::gm::Coord2D( 540.00, 444.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 551.00, 435.00), te::gm::Coord2D( 551.00, 435.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 620.00, 445.00), te::gm::Coord2D( 620.00, 445.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 635.00, 425.00), te::gm::Coord2D( 635.00, 425.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 96.00, 461.00), te::gm::Coord2D( 429.00, 819.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 116.00, 457.00), te::gm::Coord2D( 392.00, 850.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 267.00, 460.00), te::gm::Coord2D( 267.00, 460.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 308.00, 460.00), te::gm::Coord2D( 308.00, 460.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 336.00, 452.00), te::gm::Coord2D( 336.00, 452.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 398.00, 451.00), te::gm::Coord2D( 398.00, 451.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 413.00, 452.00), te::gm::Coord2D( 413.00, 452.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 458.00, 455.00), te::gm::Coord2D( 458.00, 455.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 533.00, 450.00), te::gm::Coord2D( 346.00, 851.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 302.00, 507.00), te::gm::Coord2D( 302.00, 507.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 334.00, 492.00), te::gm::Coord2D( 334.00, 492.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 419.00, 485.00), te::gm::Coord2D( 419.00, 485.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 440.00, 491.00), te::gm::Coord2D( 440.00, 491.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 499.00, 501.00), te::gm::Coord2D( 499.00, 501.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 554.00, 488.00), te::gm::Coord2D( 554.00, 488.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 581.00, 485.00), te::gm::Coord2D( 581.00, 485.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 623.00, 491.00), te::gm::Coord2D( 623.00, 491.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 216.00, 533.00), te::gm::Coord2D( 216.00, 533.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 340.00, 531.00), te::gm::Coord2D( 340.00, 531.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 372.00, 531.00), te::gm::Coord2D( 372.00, 531.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 445.00, 523.00), te::gm::Coord2D( 445.00, 523.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 569.00, 515.00), te::gm::Coord2D( 569.00, 515.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 618.00, 523.00), te::gm::Coord2D( 618.00, 523.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 642.00, 518.00), te::gm::Coord2D( 617.00, 846.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 187.00, 544.00), te::gm::Coord2D( 736.00, 794.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 242.00, 567.00), te::gm::Coord2D( 242.00, 567.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 336.00, 573.00), te::gm::Coord2D( 336.00, 573.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 376.00, 548.00), te::gm::Coord2D( 376.00, 548.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 486.00, 553.00), te::gm::Coord2D( 486.00, 553.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 493.00, 567.00), te::gm::Coord2D( 493.00, 567.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 541.00, 560.00), te::gm::Coord2D( 541.00, 560.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 558.00, 549.00), te::gm::Coord2D( 558.00, 549.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 631.00, 559.00), te::gm::Coord2D( 631.00, 559.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 233.00, 598.00), te::gm::Coord2D( 233.00, 598.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 277.00, 588.00), te::gm::Coord2D( 277.00, 588.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 311.00, 593.00), te::gm::Coord2D( 311.00, 593.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 371.00, 592.00), te::gm::Coord2D( 371.00, 592.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 383.00, 594.00), te::gm::Coord2D( 383.00, 594.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 411.00, 603.00), te::gm::Coord2D( 411.00, 603.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 475.00, 594.00), te::gm::Coord2D( 475.00, 594.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 546.00, 597.00), te::gm::Coord2D( 546.00, 597.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 612.00, 594.00), te::gm::Coord2D( 612.00, 594.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 642.00, 581.00), te::gm::Coord2D( 642.00, 581.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 90.00, 627.00), te::gm::Coord2D( 468.00, 978.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 109.00, 630.00), te::gm::Coord2D( 414.00, 806.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 242.00, 633.00), te::gm::Coord2D( 242.00, 633.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 288.00, 606.00), te::gm::Coord2D( 288.00, 606.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 312.00, 631.00), te::gm::Coord2D( 312.00, 631.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 335.00, 621.00), te::gm::Coord2D( 335.00, 621.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 366.00, 608.00), te::gm::Coord2D( 366.00, 608.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 411.00, 632.00), te::gm::Coord2D( 411.00, 632.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 480.00, 612.00), te::gm::Coord2D( 480.00, 612.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 496.00, 626.00), te::gm::Coord2D( 496.00, 626.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 532.00, 621.00), te::gm::Coord2D( 532.00, 621.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 585.00, 614.00), te::gm::Coord2D( 585.00, 614.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 627.00, 611.00), te::gm::Coord2D( 627.00, 611.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 262.00, 666.00), te::gm::Coord2D( 262.00, 666.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 275.00, 640.00), te::gm::Coord2D( 275.00, 640.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 311.00, 655.00), te::gm::Coord2D( 394.00, 795.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 335.00, 641.00), te::gm::Coord2D( 573.00, 859.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 366.00, 641.00), te::gm::Coord2D( 366.00, 641.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 398.00, 638.00), te::gm::Coord2D( 398.00, 638.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 541.00, 654.00), te::gm::Coord2D( 541.00, 654.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 593.00, 659.00), te::gm::Coord2D( 592.00, 658.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 613.00, 640.00), te::gm::Coord2D( 613.00, 640.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 44.00, 679.00), te::gm::Coord2D( 664.00, 696.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 228.00, 688.00), te::gm::Coord2D( 228.00, 688.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 266.00, 694.00), te::gm::Coord2D( 266.00, 694.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 293.00, 696.00), te::gm::Coord2D( 293.00, 696.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 322.00, 685.00), te::gm::Coord2D( 322.00, 685.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 374.00, 667.00), te::gm::Coord2D( 374.00, 666.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 422.00, 684.00), te::gm::Coord2D( 422.00, 684.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 457.00, 670.00), te::gm::Coord2D( 457.00, 670.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 490.00, 671.00), te::gm::Coord2D( 490.00, 671.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 562.00, 684.00), te::gm::Coord2D( 562.00, 684.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 587.00, 675.00), te::gm::Coord2D( 587.00, 675.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 623.00, 673.00), te::gm::Coord2D( 623.00, 673.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 651.00, 678.00), te::gm::Coord2D( 651.00, 678.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 221.00, 699.00), te::gm::Coord2D( 221.00, 699.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 288.00, 721.00), te::gm::Coord2D( 242.00, 856.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 343.00, 711.00), te::gm::Coord2D( 343.00, 711.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 434.00, 718.00), te::gm::Coord2D( 434.00, 718.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 508.00, 724.00), te::gm::Coord2D( 508.00, 724.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 523.00, 700.00), te::gm::Coord2D( 523.00, 700.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 565.00, 725.00), te::gm::Coord2D( 565.00, 725.00 ) ) );
  tiePoints.push_back( te::gm::GTParameters::TiePoint( te::gm::Coord2D( 635.00, 725.00), te::gm::Coord2D( 635.00, 725.00 ) ) );
}

void TsGTFilter::tcApplyRansac()
{
  te::gm::GTParameters transfParams;
  generateTestTPSet1( transfParams.m_tiePoints );
  
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0, 0 ), te::gm::Coord2D( 0, 0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 10, 0 ), te::gm::Coord2D( 10, 0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0, 10 ), te::gm::Coord2D( 0, 10 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 10, 10 ), te::gm::Coord2D( 10, 10 ) ) );
    
  std::vector< double > tiePointsWeights;
  tiePointsWeights.resize( transfParams.m_tiePoints.size(), 1.0 );

  std::auto_ptr< te::gm::GeometricTransformation > transfPtr;
  
  boost::timer timerInstance;
  CPPUNIT_ASSERT( te::gm::GTFilter::applyRansac( "Affine", transfParams, 0.01, 
    0.01,  0.01, 0.01, 0, 0.9, false, transfPtr, tiePointsWeights ) );
  std::cout << std::endl << "Elapsed time:";
  std::cout << timerInstance.elapsed() << std::endl;
  
  std::cout << std::endl << "Final Number of tie-points:";
  std::cout << transfPtr->getParameters().m_tiePoints.size() << std::endl;
  
  CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
  
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0000001 );
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 10, 0 ), 0.0000001 );        
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 0, 10 ), 
    te::gm::Coord2D( 0, 10 ), 0.0000001 );    
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 10, 10 ), 
    te::gm::Coord2D( 10, 10 ), 0.0000001 );    
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 5, 5 ), 
    te::gm::Coord2D( 5, 5 ), 0.0000001 );    
    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0000001 );
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 10, 0 ), 0.0000001 );        
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 0, 10 ), 
    te::gm::Coord2D( 0, 10 ), 0.0000001 );    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 10, 10 ), 
    te::gm::Coord2D( 10, 10 ), 0.0000001 );    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 5, 5 ), 
    te::gm::Coord2D( 5, 5 ), 0.0000001 );    
}

void TsGTFilter::tcApplyRansacMultiThread()
{
  te::gm::GTParameters transfParams;
  generateTestTPSet1( transfParams.m_tiePoints );
  
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0, 0 ), te::gm::Coord2D( 0, 0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 10, 0 ), te::gm::Coord2D( 10, 0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0, 10 ), te::gm::Coord2D( 0, 10 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 10, 10 ), te::gm::Coord2D( 10, 10 ) ) );
    
  std::vector< double > tiePointsWeights;
  tiePointsWeights.resize( transfParams.m_tiePoints.size(), 1.0 );

  std::auto_ptr< te::gm::GeometricTransformation > transfPtr;
  
  boost::timer timerInstance;
  CPPUNIT_ASSERT( te::gm::GTFilter::applyRansac( "Affine", transfParams, 0.01, 
    0.01,  0.01, 0.01, 0, 0.9, true, transfPtr, tiePointsWeights ) );
  std::cout << std::endl << "Elapsed time:";
  std::cout << timerInstance.elapsed() << std::endl;
  
  std::cout << std::endl << "Final Number of tie-points:";
  std::cout << transfPtr->getParameters().m_tiePoints.size() << std::endl;
  
  CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
  
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0000001 );
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 10, 0 ), 0.0000001 );        
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 0, 10 ), 
    te::gm::Coord2D( 0, 10 ), 0.0000001 );    
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 10, 10 ), 
    te::gm::Coord2D( 10, 10 ), 0.0000001 );    
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 5, 5 ), 
    te::gm::Coord2D( 5, 5 ), 0.0000001 );    
    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0000001 );
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 10, 0 ), 0.0000001 );        
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 0, 10 ), 
    te::gm::Coord2D( 0, 10 ), 0.0000001 );    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 10, 10 ), 
    te::gm::Coord2D( 10, 10 ), 0.0000001 );    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 5, 5 ), 
    te::gm::Coord2D( 5, 5 ), 0.0000001 );    
}

