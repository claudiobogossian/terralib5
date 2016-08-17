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
  \file TsPropertyTypes.cpp
 
  \brief Test suite for the PropertyType class hierarchy.
 */

// Unit-Test TerraLib
#include "TsPropertyTypes.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/datatype.h>

// STL
#include <ctime>

CPPUNIT_TEST_SUITE_REGISTRATION( TsPropertyTypes );

void TsPropertyTypes::setUp()
{
}

void TsPropertyTypes::tearDown()
{
}

void TsPropertyTypes::tcCreatePropertyTypes()
{
//#ifdef TE_COMPILE_ALL
  {
    te::dt::NumericProperty numeric("attribute_numeric", 10, 3, true, new std::string("34.78"), 1);
    CPPUNIT_ASSERT(numeric.getType() == te::dt::NUMERIC_TYPE);
    
    CPPUNIT_ASSERT(numeric.getName() == "attribute_numeric");
    CPPUNIT_ASSERT(numeric.getPrecision() == 10);
    CPPUNIT_ASSERT(numeric.getScale() == 3);
    CPPUNIT_ASSERT(numeric.isRequired() == true);
    CPPUNIT_ASSERT(numeric.getDefaultValue());
    CPPUNIT_ASSERT(*numeric.getDefaultValue() == "34.78");
    CPPUNIT_ASSERT(numeric.getId() == 1);
    CPPUNIT_ASSERT(numeric.getParent() == 0);
  }

  {
    te::dt::StringProperty fixed_string("attribute_fixed_string", te::dt::FIXED_STRING, 50);
    
    CPPUNIT_ASSERT(fixed_string.getType() == te::dt::STRING_TYPE);
    CPPUNIT_ASSERT(fixed_string.getSubType() == te::dt::FIXED_STRING);
    CPPUNIT_ASSERT(fixed_string.getName() == "attribute_fixed_string");
    CPPUNIT_ASSERT(fixed_string.size() == 50);
    CPPUNIT_ASSERT(fixed_string.isRequired() == false);
    CPPUNIT_ASSERT(fixed_string.getDefaultValue() == 0);
    CPPUNIT_ASSERT(fixed_string.getParent() == 0);
  }

  {
    te::dt::StringProperty var_string("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82);
    
    CPPUNIT_ASSERT(var_string.getType() == te::dt::STRING_TYPE);
    CPPUNIT_ASSERT(var_string.getSubType() == te::dt::VAR_STRING);
    CPPUNIT_ASSERT(var_string.getName() == "attribute_var_string");
    CPPUNIT_ASSERT(var_string.size() == 50);
    CPPUNIT_ASSERT(var_string.isRequired() == false);
    CPPUNIT_ASSERT(var_string.getDefaultValue() == 0);
    CPPUNIT_ASSERT(var_string.getId() == 82);
    CPPUNIT_ASSERT(var_string.getParent() == 0);
  }

  {
    te::dt::StringProperty var_unlimited_string("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79);
    
    CPPUNIT_ASSERT(var_unlimited_string.getType() == te::dt::STRING_TYPE);
    CPPUNIT_ASSERT(var_unlimited_string.getSubType() == te::dt::STRING);
    CPPUNIT_ASSERT(var_unlimited_string.getName() == "attribute_var_unlimited_string");
    CPPUNIT_ASSERT(var_unlimited_string.size() == 0);
    CPPUNIT_ASSERT(var_unlimited_string.isRequired() == true);
    CPPUNIT_ASSERT(var_unlimited_string.getDefaultValue() == 0);
    CPPUNIT_ASSERT(var_unlimited_string.getId() == 79);
    CPPUNIT_ASSERT(var_unlimited_string.getParent() == 0);
  }

  {
    te::dt::SimpleProperty intt("attribute_int", te::dt::INT32_TYPE, true, 0, 23);

    CPPUNIT_ASSERT(intt.getType() == te::dt::INT32_TYPE);
    CPPUNIT_ASSERT(intt.getName() == "attribute_int");
    CPPUNIT_ASSERT(intt.isAutoNumber() == false);
    CPPUNIT_ASSERT(intt.isRequired() == true);
    CPPUNIT_ASSERT(intt.getDefaultValue() == 0);
    CPPUNIT_ASSERT(intt.getId() == 23);
    CPPUNIT_ASSERT(intt.getParent() == 0);
  }

  {
    te::dt::SimpleProperty int_serial("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23);
    int_serial.setAutoNumber(true);

    CPPUNIT_ASSERT(int_serial.getType() == te::dt::INT32_TYPE);
    CPPUNIT_ASSERT(int_serial.getName() == "attribute_int_serial");
    CPPUNIT_ASSERT(int_serial.isAutoNumber());
    CPPUNIT_ASSERT(int_serial.isRequired() == false);
    CPPUNIT_ASSERT(int_serial.getDefaultValue() == 0);
    CPPUNIT_ASSERT(int_serial.getId() == 23);
    CPPUNIT_ASSERT(int_serial.getParent() == 0);
  }

  {
    te::gm::GeometryProperty geom("attribute_geom",  0, te::gm::GeometryType, true, 0, 2333);

    CPPUNIT_ASSERT(geom.getType() == te::dt::GEOMETRY_TYPE );
    CPPUNIT_ASSERT(geom.getName() == "attribute_geom");
    CPPUNIT_ASSERT(geom.isAutoNumber() ==  false);
    CPPUNIT_ASSERT(geom.isRequired() == true);
    CPPUNIT_ASSERT(geom.getDefaultValue() == 0);
    CPPUNIT_ASSERT(geom.getId() == 2333);
    CPPUNIT_ASSERT(te::gm::GetCoordDimension(geom.getGeometryType()) == 2);
    CPPUNIT_ASSERT(geom.getSRID() == 0);
    CPPUNIT_ASSERT(geom.getGeometryType() == te::gm::GeometryType);
    CPPUNIT_ASSERT(geom.getParent() == 0);
  }

  {
    te::gm::GeometryProperty geom("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331);

    CPPUNIT_ASSERT(geom.getType() == te::dt::GEOMETRY_TYPE );
    CPPUNIT_ASSERT(geom.getName() == "attribute_geom_polm");
    CPPUNIT_ASSERT(geom.isAutoNumber() ==  false);
    CPPUNIT_ASSERT(geom.isRequired() == true);
    CPPUNIT_ASSERT(geom.getDefaultValue() == 0);
    CPPUNIT_ASSERT(geom.getId() == 23331);
    CPPUNIT_ASSERT(te::gm::GetCoordDimension(geom.getGeometryType()) == 3);
    CPPUNIT_ASSERT(geom.getSRID() == 4326);
    CPPUNIT_ASSERT(geom.getGeometryType() == te::gm::PolygonMType);
    CPPUNIT_ASSERT(geom.getParent() == 0);
  }

  {
    te::dt::CompositeProperty complex_type("attribute_composite_type", "composite_name", 666);

    complex_type.add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    complex_type.add(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING, 50));
    complex_type.add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    complex_type.add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    complex_type.add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    complex_type.add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));
 
    CPPUNIT_ASSERT(complex_type.getCompositeName() == "attribute_composite_type");
    CPPUNIT_ASSERT(complex_type.getName() == "composite_name");
    CPPUNIT_ASSERT(complex_type.getId() == 666);
    CPPUNIT_ASSERT(complex_type.getParent() == 0);
    CPPUNIT_ASSERT(complex_type.size() == 6);
    CPPUNIT_ASSERT(complex_type.has(complex_type.getProperty(0)));
    CPPUNIT_ASSERT(complex_type.has(complex_type.getProperty(1)));
    CPPUNIT_ASSERT(complex_type.has(complex_type.getProperty(2)));
    CPPUNIT_ASSERT(complex_type.has(complex_type.getProperty(3)));
    CPPUNIT_ASSERT(complex_type.has(complex_type.getProperty(4)));
    CPPUNIT_ASSERT(complex_type.has(complex_type.getProperty(5)));

    te::dt::SimpleProperty intt("attribute_int", te::dt::INT32_TYPE, true, 0, 23);
    CPPUNIT_ASSERT(complex_type.has(&intt) == false);
  }


//#endif  // TE_COMPILE_ALL
}

void TsPropertyTypes::tcClone()
{
//#ifdef TE_COMPILE_ALL
//#endif  // TE_COMPILE_ALL
}

