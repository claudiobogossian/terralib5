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
  \file TsTiePointsLocator.h
 
  \brief A test suit for the TiePointsLocator Class.
 */

#ifndef __TERRALIB_UNITTEST_RP_TiePoints_INTERNAL_H
#define __TERRALIB_UNITTEST_RP_TiePoints_INTERNAL_H

#include <terralib/raster/Raster.h>
#include <terralib/geometry/GeometricTransformation.h>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

#include <vector>

/*!
  \class TsTiePointsLocator

  \brief A test suit for the TiePointsLocator class interface.
 */
class TsTiePointsLocator : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsTiePointsLocator );
  
  CPPUNIT_TEST( SurfStrategySameImage );
 
  CPPUNIT_TEST( SurfStrategyUpsampledImage );  
  
  CPPUNIT_TEST( SurfStrategy90ClkWRotationImage );    
  
  CPPUNIT_TEST( SurfStrategySameImageDifBoxes );
  
  CPPUNIT_TEST( SurfStrategyHalfRotated90Image );
  
  CPPUNIT_TEST( SurfStrategyMultipleThreads );
  
  CPPUNIT_TEST( SurfStrategyRescaleFactor );

  CPPUNIT_TEST( MoravecStrategySameImage );

  CPPUNIT_TEST( MoravecStrategyUpsampledImage );
  
  CPPUNIT_TEST( MoravecStrategy90ClkWRotationImage );   
  
  CPPUNIT_TEST( MoravecStrategySameImageDifBoxes );
  
  CPPUNIT_TEST( MoravecStrategyHalfRotated90Image );
  
  CPPUNIT_TEST( MoravecStrategyMultipleThreads );
  
  CPPUNIT_TEST( MoravecStrategyRescaleFactor );
  
  CPPUNIT_TEST_SUITE_END();

protected :

  void MoravecStrategySameImage();
  
  void MoravecStrategyUpsampledImage();
  
  void MoravecStrategy90ClkWRotationImage();
  
  void MoravecStrategyHalfRotated90Image();
  
  void MoravecStrategySameImageDifBoxes();
  
  void MoravecStrategyMultipleThreads();
  
  void MoravecStrategyMaximumOffset();
  
  void MoravecStrategyRescaleFactor();
  
  void SurfStrategySameImage();
  
  void SurfStrategyUpsampledImage();
  
  void SurfStrategy90ClkWRotationImage();
  
  void SurfStrategySameImageDifBoxes();
  
  void SurfStrategyHalfRotated90Image();
  
  void SurfStrategyMultipleThreads();
  
  void SurfStrategyRescaleFactor();

  /*!
    \brief Save images with their respective tie points market green.
    
    \param raster1 The first raster (related to tie points first coordinate pair).
    
    \param raster2 The second raster (related to tie points second coordinate pair).
    
    \param tiePoints Tie points.          

    \param tifFileNameBeginning Tif file name beginning.
  */             
  void saveImagesAndTiePoints( 
    const te::rst::Raster& raster1,
    const unsigned int band1,
    const te::rst::Raster& raster2,
    const unsigned int band2,
    const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
    const std::string& tifFileNameBeginning );  
};

#endif
