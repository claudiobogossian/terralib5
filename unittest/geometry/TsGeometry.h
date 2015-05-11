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
  \file TsGeometry.h
 
  \brief Test suite for the Vector Geometry Model.
 */

#ifndef __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GEOMETRY_H
#define __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GEOMETRY_H

// STL
#include <string>
#include <vector>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsGeometry

  \brief Test suite for the Vector Geometry Model.

  This test suite will check the following:
  <ul>
  <li>PostGIS Extended-WKB reading and writing;</li>
  <li>Official OGC WKB reading/writing;</li>
  <li>Spatial Operators;</li>
  <li>Re-projection (transformation of SRS);</li>
  </ul>
 */
class TsGeometry : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsGeometry );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcMisc );
  CPPUNIT_TEST( tcCreatePoint );
  CPPUNIT_TEST( tcCreatePointZ );
  CPPUNIT_TEST( tcCreatePointM );
  CPPUNIT_TEST( tcCreatePointZM );
  CPPUNIT_TEST( tcCreateLineString );
  CPPUNIT_TEST( tcCreateLineStringZ );
  CPPUNIT_TEST( tcCreateLineStringM );
  CPPUNIT_TEST( tcCreateLineStringZM );
  CPPUNIT_TEST( tcCreatePolygon );
  //CPPUNIT_TEST( tcCreatePolygonZ );
  //CPPUNIT_TEST( tcCreatePolygonM );
  //CPPUNIT_TEST( tcCreatePolygonZM );
  CPPUNIT_TEST( tcAsText );
  CPPUNIT_TEST( tcTransform );
  CPPUNIT_TEST( tcBasicGeometryMethods );
  CPPUNIT_TEST( tcSpatialRelationsMethods );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: Creating several type of objects and testing some basic methods. */
    void tcMisc();

    /*! \brief Test Case: Creating a point object. */
    void tcCreatePoint();

    /*! \brief Test Case: Creating a point with z value. */
    void tcCreatePointZ();

    /*! \brief Test Case: Creating a point with m value. */
    void tcCreatePointM();

    /*! \brief Test Case: Creating a point with z and m value. */
    void tcCreatePointZM();

    /*! \brief Test Case: Creating a linestring object. */
    void tcCreateLineString();

    /*! \brief Test Case: Creating a linestring with z value. */
    void tcCreateLineStringZ();

    /*! \brief Test Case: Creating a linestring with m value. */
    void tcCreateLineStringM();

    /*! \brief Test Case: Creating a linestring with z and m value. */
    void tcCreateLineStringZM();

    /*! \brief Test Case: Creating a polygon. */
    void tcCreatePolygon();

    /*! \brief Test Case: Creating a polygon with Z values. */
    void tcCreatePolygonZ();

    /*! \brief Test Case: Creating a polygon with M values. */
    void tcCreatePolygonM();

    /*! \brief Test Case: Creating a polygon with ZM values. */
    void tcCreatePolygonZM();

    /*! \brief Test Case: Reading (binary) geometries and getting the WKT. */
    void tcAsText();

   /*! \brief Test Case: Reading WKT geometries and conveting to another proj SRID. */
    void tcTransform();

    /*! \brief Test Case: testing the basic geometry methods. */
    void tcBasicGeometryMethods();

    /*! \brief Test Case: testing the basic geometry methods. */
    void tcSpatialRelationsMethods();

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

};

#endif  // __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GEOMETRY_H

