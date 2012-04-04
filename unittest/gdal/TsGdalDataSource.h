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
  \file TsGdal.h
 
  \brief A test suit for the Gdal class.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_GDAL_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_GDAL_H

// STL
#include <string>
#include <vector>

//#include <terralib/dataaccess_fw.h>

#include "../dataaccess/TsDataSource.h"
//#include "../dataaccess/TsDataSourceTransactor.h"

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

class TsGdal : public TsDataSource
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsGdal );

// It registers the class methods as Test Cases belonging to the suit.  
  INSERT_DATASOURCE_TC

// Adding specific Test cases for Gdal (TsGdal) 
  //...here

  CPPUNIT_TEST_SUITE_END();  
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();
    
// Adding  specific Test cases for Gdal
   //...here

};

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_GDAL_H

