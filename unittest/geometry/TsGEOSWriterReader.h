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
  \file TsGEOSWriterReader.h 
 
  \brief Test suite for the GEOSWriter and GEOSReader class.
 */

#ifndef __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GEOSWRITERREADER_H
#define __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GEOSWRITERREADER_H

// STL
#include <string>
#include <vector>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsGEOSWriterReader

  \brief Test suite for the GEOSWriter and GEOSReader class.

  This test suite will check the following:
  <ul>
  <li>The convertion of a TerraLib geometry to a GEOS geometry;</li>
  <li>Official OGC WKB reading/writing;</li>
  <li>Spatial Operators;</li>
  <li>The convertion of a GEOS geometry to a Terralib geometry;</li>
  </ul>
 */
class TsGEOSWriterReader : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsGEOSWriterReader );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcCreatePointsFromWKTAndApplyGEOSTests );
  CPPUNIT_TEST( tcCreateLinesFromWKTAndApplyGEOSTests );
  CPPUNIT_TEST( tcCreatePolygonFromWKTAndApplyGEOSTests );

  //CPPUNIT_TEST( tcCreateGeometriesAndApplyReadTests );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: Creating basic Geom and apply read test. */
    void tcCreateGeometriesAndApplyReadTests();

    /*! \brief Test Case: Creating POINTS Geom from WKT and apply GEOS Write and Read test. */
    void tcCreatePointsFromWKTAndApplyGEOSTests();

    /*! \brief Test Case: Creating LINES Geom from WKT and apply GEOS Write and Read test. */
    void tcCreateLinesFromWKTAndApplyGEOSTests();

    /*! \brief Test Case: Creating POLYGONS Geom from WKT and apply GEOS Write and Read test. */
    void tcCreatePolygonFromWKTAndApplyGEOSTests();

    /*! \brief Test Case: Creating PPLYGONS Geom from WKT and apply GEOS Write and Read test. */
    void tcCreateGeomCollectionFromWKTAndApplyGEOSTests();

  private:

    /* Auxiliary data structure when testing geometry basic methods */
    struct WKBEntry
    {
      std::string m_hwkb;       //!<
      int m_srid;               //!<
      std::string m_geomType;   //!<
      int m_dimensonality;      //!<
      int m_ndims;              //!<
      std::string m_wkt;        //!<
      int m_geomTypeId;         //!<
      bool m_is3D;              //!<
      bool m_isMeasured;        //!<
    };

    /*!
      \brief Auxiliary method for reading geometry information from an ASCII file.

      \param hwkbVec An output vector with geometry information.
     */
    void loadGeometry(std::vector<WKBEntry>& hwkbVec) const;
    void loadWKT(std::vector<std::string>& geom_wkt) const;
    void loadWKT(std::string filewkt, std::vector<std::string>& geom_wkt) const;

};

#endif  // __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GEOSWRITERREADER_H
