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
  \file TsPropertyTypes.cpp
 
  \brief Test suite for the PropertyType class hierarchy.
 */

// Unit-Test TerraLib
#include "TsPropertyTypes.h"

// TerraLib
#include <terralib/geometry.h>
#include <terralib/datatype.h>
#include <terralib/raster.h>

// STL
#include <ctime>

CPPUNIT_TEST_SUITE_REGISTRATION( TsPropertyTypes );

void TsPropertyTypes::setUp()
{
}

void TsPropertyTypes::tearDown()
{
}

void TsPropertyTypes::tcNumericProperty()
{
//#ifdef TE_COMPILE_ALL
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

  // Copy constructor
  te::dt::NumericProperty numeric_copy(numeric);
  CPPUNIT_ASSERT(numeric_copy.getName() == "attribute_numeric");
  CPPUNIT_ASSERT(numeric_copy.getPrecision() == 10);
  CPPUNIT_ASSERT(numeric_copy.getScale() == 3);
  CPPUNIT_ASSERT(numeric_copy.isRequired() == true);
  CPPUNIT_ASSERT(numeric_copy.getDefaultValue());
  CPPUNIT_ASSERT(*numeric_copy.getDefaultValue() == "34.78");
  CPPUNIT_ASSERT(numeric_copy.getId() == 1);
  CPPUNIT_ASSERT(numeric_copy.getParent() == 0);
    
  // Changing some values on 'numeric_copy' before assigning it to 'numeric_assign' 
  numeric_copy.setName("attribute_numeric_assign");
  numeric_copy.setPrecision(12);
  numeric_copy.setScale(4);
  numeric_copy.setDefaultValue(new std::string("50.25"));
  numeric_copy.setId(2);

  //Assign operator
  te::dt::NumericProperty numeric_assign(numeric); //there is no default constructor
  numeric_assign.operator=(numeric_copy); // assigning another value...
  CPPUNIT_ASSERT(numeric_assign.getName() == "attribute_numeric_assign");
  CPPUNIT_ASSERT(numeric_assign.getPrecision() == 12);
  CPPUNIT_ASSERT(numeric_assign.getScale() == 4);
  CPPUNIT_ASSERT(numeric_assign.isRequired() == true);
  CPPUNIT_ASSERT(numeric_assign.getDefaultValue());
  CPPUNIT_ASSERT(*numeric_assign.getDefaultValue() == "50.25");
  CPPUNIT_ASSERT(numeric_assign.getId() == 2);
  CPPUNIT_ASSERT(numeric_assign.getParent() == 0);

  // clone
  te::dt::Property* numeric_clone = numeric.clone();
  CPPUNIT_ASSERT(numeric_clone->getName() == "attribute_numeric");
  CPPUNIT_ASSERT(numeric_clone->getType() == te::dt::NUMERIC_TYPE);
  CPPUNIT_ASSERT(numeric_clone->getId() == 1);
  CPPUNIT_ASSERT(numeric_clone->getParent() == 0);

  CPPUNIT_ASSERT((static_cast<te::dt::NumericProperty*>(numeric_clone)->getPrecision()) == 10);
  CPPUNIT_ASSERT(static_cast<te::dt::NumericProperty*>(numeric_clone)->getScale() == 3);
  CPPUNIT_ASSERT(static_cast<te::dt::NumericProperty*>(numeric_clone)->isRequired() == true);
  CPPUNIT_ASSERT(*(static_cast<te::dt::NumericProperty*>(numeric_clone)->getDefaultValue()) == "34.78");  
}

void TsPropertyTypes::tcStringProperty()
{ 
  // te::dt::FIXED_STRING
  te::dt::StringProperty fixed_string("attribute_fixed_string", te::dt::FIXED_STRING, 50);
    
  CPPUNIT_ASSERT(fixed_string.getType() == te::dt::STRING_TYPE);
  CPPUNIT_ASSERT(fixed_string.getSubType() == te::dt::FIXED_STRING);
  CPPUNIT_ASSERT(fixed_string.getName() == "attribute_fixed_string");
  CPPUNIT_ASSERT(fixed_string.size() == 50);
  CPPUNIT_ASSERT(fixed_string.isRequired() == false);
  CPPUNIT_ASSERT(fixed_string.getDefaultValue() == 0);
  CPPUNIT_ASSERT(fixed_string.getParent() == 0);

  // Copy Constructor
  te::dt::StringProperty fixed_string_copy(fixed_string);
  CPPUNIT_ASSERT(fixed_string_copy.getType() == te::dt::STRING_TYPE);
  CPPUNIT_ASSERT(fixed_string_copy.getSubType() == te::dt::FIXED_STRING);
  CPPUNIT_ASSERT(fixed_string_copy.getName() == "attribute_fixed_string");
  CPPUNIT_ASSERT(fixed_string_copy.size() == 50);
  CPPUNIT_ASSERT(fixed_string_copy.isRequired() == false);
  CPPUNIT_ASSERT(fixed_string_copy.getDefaultValue() == 0);
  CPPUNIT_ASSERT(fixed_string_copy.getParent() == 0);

  // Assign operator
  fixed_string.setSubtype(te::dt::VAR_STRING);
  CPPUNIT_ASSERT(fixed_string.getSubType() == te::dt::VAR_STRING);

  te::dt::StringProperty fixed_string_assign(fixed_string);
  fixed_string_assign.operator=(fixed_string_copy);

  CPPUNIT_ASSERT(fixed_string_assign.getType() == te::dt::STRING_TYPE);
  CPPUNIT_ASSERT(fixed_string_assign.getSubType() == te::dt::FIXED_STRING);
  CPPUNIT_ASSERT(fixed_string_assign.getName() == "attribute_fixed_string");
  CPPUNIT_ASSERT(fixed_string_assign.size() == 50);
  CPPUNIT_ASSERT(fixed_string_assign.isRequired() == false);
  CPPUNIT_ASSERT(fixed_string_assign.getDefaultValue() == 0);
  CPPUNIT_ASSERT(fixed_string_assign.getParent() == 0);

  // Clone
  te::dt::Property* fixed_string_clone = fixed_string.clone();
  CPPUNIT_ASSERT(fixed_string_clone->getType() == te::dt::STRING_TYPE);
  CPPUNIT_ASSERT(fixed_string_clone->getId() == 0);
  CPPUNIT_ASSERT(fixed_string_clone->getName() == "attribute_fixed_string");
  CPPUNIT_ASSERT(fixed_string_clone->getParent() == 0); 

  CPPUNIT_ASSERT(static_cast<te::dt::StringProperty*>(fixed_string_clone)->getSubType() ==  te::dt::VAR_STRING);  
  CPPUNIT_ASSERT(static_cast<te::dt::StringProperty*>(fixed_string_clone)->size() == 50);
  CPPUNIT_ASSERT(static_cast<te::dt::StringProperty*>(fixed_string_clone)->isRequired() == false);
  CPPUNIT_ASSERT(static_cast<te::dt::StringProperty*>(fixed_string_clone)->getDefaultValue() == 0);

  // te::dt::VAR_STRING
  te::dt::StringProperty var_string("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82);    
  CPPUNIT_ASSERT(var_string.getType() == te::dt::STRING_TYPE);
  CPPUNIT_ASSERT(var_string.getSubType() == te::dt::VAR_STRING);
  CPPUNIT_ASSERT(var_string.getName() == "attribute_var_string");
  CPPUNIT_ASSERT(var_string.size() == 50);
  CPPUNIT_ASSERT(var_string.isRequired() == false);
  CPPUNIT_ASSERT(var_string.getDefaultValue() == 0);
  CPPUNIT_ASSERT(var_string.getId() == 82);
  CPPUNIT_ASSERT(var_string.getParent() == 0);

  // te::dt::STRING  - unlimited string
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

void TsPropertyTypes::tcSimpleProperty()
{ 
  te::dt::SimpleProperty sp32(  "attribute_int", te::dt::INT32_TYPE, true, 0, 1);

  CPPUNIT_ASSERT(sp32.getType() == te::dt::INT32_TYPE);
  CPPUNIT_ASSERT(sp32.getName() == "attribute_int");
  CPPUNIT_ASSERT(sp32.isAutoNumber() == false);
  CPPUNIT_ASSERT(sp32.isRequired() == true);
  CPPUNIT_ASSERT(sp32.getDefaultValue() == 0);
  CPPUNIT_ASSERT(sp32.getId() == 1);
  CPPUNIT_ASSERT(sp32.getParent() == 0);

  te::dt::SimpleProperty sp16("attribute_int", te::dt::INT16_TYPE, true, new std::string("25"), 1);
  CPPUNIT_ASSERT(sp16.getType() == te::dt::INT16_TYPE);
  CPPUNIT_ASSERT(sp16.getName() == "attribute_int");
  CPPUNIT_ASSERT(sp16.isAutoNumber() == false);
  CPPUNIT_ASSERT(sp16.isRequired() == true);
  std::string* res = sp16.getDefaultValue();
  CPPUNIT_ASSERT(*sp16.getDefaultValue()  == "25");
  CPPUNIT_ASSERT(sp16.getId() == 1);
  CPPUNIT_ASSERT(sp16.getParent() == 0);

  // auto number
  te::dt::SimpleProperty int_serial("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23);
  int_serial.setAutoNumber(true);
  CPPUNIT_ASSERT(int_serial.getType() == te::dt::INT32_TYPE);
  CPPUNIT_ASSERT(int_serial.getName() == "attribute_int_serial");
  CPPUNIT_ASSERT(int_serial.isAutoNumber());
  CPPUNIT_ASSERT(int_serial.isRequired() == false);
  CPPUNIT_ASSERT(int_serial.getDefaultValue() == 0);
  CPPUNIT_ASSERT(int_serial.getId() == 23);
  CPPUNIT_ASSERT(int_serial.getParent() == 0);

  //Complex_type 
  te::dt::SimpleProperty attr_cint16("attr_cint16", te::dt::CINT16_TYPE, false, 0, 23);

  CPPUNIT_ASSERT(attr_cint16.getType() == te::dt::CINT16_TYPE);
  CPPUNIT_ASSERT(attr_cint16.getName() == "attr_cint16");
  CPPUNIT_ASSERT(attr_cint16.isAutoNumber() == false);
  CPPUNIT_ASSERT(attr_cint16.isRequired() == false);
  CPPUNIT_ASSERT(attr_cint16.getDefaultValue() == 0);
  CPPUNIT_ASSERT(attr_cint16.getId() == 23);
  CPPUNIT_ASSERT(attr_cint16.getParent() == 0);

  // ENUM 2-13 and 23-26 - basic data types supported by terralib as SimpleProperties 
}

void TsPropertyTypes::tcGeometryPropertyFirstConstructor()
{
  te::gm::GeometryProperty geom_name("attribute_geom_name",  true, 0, 1);
  CPPUNIT_ASSERT(geom_name.getType() == te::dt::GEOMETRY_TYPE );
  CPPUNIT_ASSERT(geom_name.getGeometryType() == te::gm::UnknownGeometryType );
  CPPUNIT_ASSERT(geom_name.getName() == "attribute_geom_name");
  CPPUNIT_ASSERT(geom_name.isAutoNumber() ==  false);
  CPPUNIT_ASSERT(geom_name.isRequired() == true);
  CPPUNIT_ASSERT(geom_name.getDefaultValue() == 0);
  CPPUNIT_ASSERT(geom_name.getId() == 1);
  CPPUNIT_ASSERT(geom_name.getSRID() == 0);
  CPPUNIT_ASSERT(geom_name.getParent() == 0);
  CPPUNIT_ASSERT_MESSAGE("te::gm::GetCoordDimension(te::gm::UnknownGeometryType) cai - Should be tested in geometry/Utils ", 1 == 2); 
}

void TsPropertyTypes::tcGeometryPropertyFirstConstructorAssign()
{
  // Using first constructor and then assigning other values...
  te::gm::GeometryProperty geom_name("attribute_geom_name",  true, 0, 1);
  geom_name.setGeometryType(te::gm::MultiPointType);
  geom_name.setName("attribute_name_changed");
  std::string ss = "MULTIPOINT(1 1, 2 2, 3 3)" ;
  geom_name.setDefaultValue(new std::string("MULTIPOINT(1 1, 2 2, 3 3)"));
  //geom_name.setDefaultValue(&ss); //destructor is called and boom!
  CPPUNIT_ASSERT(geom_name.getType() == te::dt::GEOMETRY_TYPE );
  CPPUNIT_ASSERT(geom_name.getGeometryType() == te::gm::MultiPointType );
  CPPUNIT_ASSERT(geom_name.getName() == "attribute_name_changed");
  CPPUNIT_ASSERT(geom_name.isAutoNumber() ==  false);
  CPPUNIT_ASSERT(geom_name.isRequired() == true);
  CPPUNIT_ASSERT(*geom_name.getDefaultValue() == ss);
  CPPUNIT_ASSERT(geom_name.getId() == 1);
  CPPUNIT_ASSERT(geom_name.getSRID() == 0);
  CPPUNIT_ASSERT(geom_name.getParent() == 0);
}

void TsPropertyTypes::tcGeometryPropertyCopyConstructor()
{
  const te::gm::GeometryProperty geom("attribute_geom",  0, te::gm::GeometryType, true, 0, 2333); 
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

  // Copy constructor
  te::gm::GeometryProperty geom_copy(geom);
  CPPUNIT_ASSERT(geom_copy.getType() == te::dt::GEOMETRY_TYPE );
  CPPUNIT_ASSERT(geom_copy.getGeometryType() == te::gm::GeometryType );
  CPPUNIT_ASSERT(geom_copy.getName() == "attribute_geom");
  CPPUNIT_ASSERT(geom_copy.isAutoNumber() ==  false);
  CPPUNIT_ASSERT(geom_copy.isRequired() == true);
  CPPUNIT_ASSERT(geom_copy.getDefaultValue() == 0);
  CPPUNIT_ASSERT(geom_copy.getId() == 2333);
  CPPUNIT_ASSERT(geom_copy.getSRID() == 0);
  CPPUNIT_ASSERT(geom_copy.getParent() == 0);
}

void TsPropertyTypes::tcGeometryProperty()
{  
  const te::gm::GeometryProperty geom("attribute_geom",  0, te::gm::GeometryType, true, 0, 2333);
  
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

  // PolygonMType
  te::gm::GeometryProperty geompol("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331);

  CPPUNIT_ASSERT(geompol.getType() == te::dt::GEOMETRY_TYPE );
  CPPUNIT_ASSERT(geompol.getName() == "attribute_geom_polm");
  CPPUNIT_ASSERT(geompol.isAutoNumber() ==  false);
  CPPUNIT_ASSERT(geompol.isRequired() == true);
  CPPUNIT_ASSERT(geompol.getDefaultValue() == 0);
  CPPUNIT_ASSERT(geompol.getId() == 23331);
  CPPUNIT_ASSERT(te::gm::GetCoordDimension(geompol.getGeometryType()) == 3);
  CPPUNIT_ASSERT(geompol.getSRID() == 4326);
  CPPUNIT_ASSERT(geompol.getGeometryType() == te::gm::PolygonMType);
  CPPUNIT_ASSERT(geompol.getParent() == 0);

  // Clone
  te::dt::Property* geom_cl = geompol.clone();
  CPPUNIT_ASSERT(geom_cl->getType() == te::dt::GEOMETRY_TYPE );
  CPPUNIT_ASSERT(geom_cl->getName() == "attribute_geom_polm");
  CPPUNIT_ASSERT(geom_cl->getId() == 23331);
  CPPUNIT_ASSERT(geom_cl->getParent() == 0);
  CPPUNIT_ASSERT(static_cast<te::gm::GeometryProperty*>(geom_cl)->isAutoNumber() ==  false);
  CPPUNIT_ASSERT(static_cast<te::gm::GeometryProperty*>(geom_cl)->isRequired() == true);
  CPPUNIT_ASSERT(static_cast<te::gm::GeometryProperty*>(geom_cl)->getDefaultValue() == 0);
  CPPUNIT_ASSERT(te::gm::GetCoordDimension(static_cast<te::gm::GeometryProperty*>(geom_cl)->getGeometryType()) == 3);
  CPPUNIT_ASSERT(static_cast<te::gm::GeometryProperty*>(geom_cl)->getSRID() == 4326);
  CPPUNIT_ASSERT(static_cast<te::gm::GeometryProperty*>(geom_cl)->getGeometryType() == te::gm::PolygonMType); 
}

void TsPropertyTypes::tcCompositeProperty()
{
  te::dt::CompositeProperty composite_type("attribute_composite_type", "composite_name", 666);

  te::dt::NumericProperty* num_prop =  new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1);
  composite_type.add(num_prop);

  // adding vector of properties
  std::vector<te::dt::Property*> ps;
  ps.push_back(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING, 50));
  ps.push_back(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
  composite_type.add(ps);

  composite_type.add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
  composite_type.add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
  composite_type.add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));
 
  CPPUNIT_ASSERT(composite_type.getCompositeName() == "attribute_composite_type");
  CPPUNIT_ASSERT(composite_type.getName() == "composite_name");
  CPPUNIT_ASSERT(composite_type.getId() == 666);
  CPPUNIT_ASSERT(composite_type.getParent() == 0);
  CPPUNIT_ASSERT(composite_type.size() == 6);
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty(0)));
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty(1)));
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty(2)));
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty(3)));
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty(4)));
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty(5)));
  CPPUNIT_ASSERT(composite_type.getPropertyPosition("attribute_numeric") == 0 );
  CPPUNIT_ASSERT(composite_type.getPropertyPosition("attribute_fixed_string") == 1 );
  CPPUNIT_ASSERT(composite_type.getPropertyPosition("attribute_geom_polm") == 2 );
  CPPUNIT_ASSERT(composite_type.getPropertyPosition("attribute_var_string") == 3 );
  CPPUNIT_ASSERT(composite_type.getPropertyPosition("attribute_var_unlimited_string") == 4 );
  CPPUNIT_ASSERT(composite_type.getPropertyPosition("attribute_int_serial") == 5 );

  te::dt::NumericProperty* auxp = static_cast<te::dt::NumericProperty*>(composite_type.getProperty("attribute_numeric"));
  unsigned int i = auxp->getId(); 
  CPPUNIT_ASSERT(static_cast<te::dt::NumericProperty*>(composite_type.getProperty("attribute_numeric"))->getId() == num_prop->getId() );
  CPPUNIT_ASSERT(composite_type.getPropertyPosition(num_prop) == 0 );
  CPPUNIT_ASSERT(static_cast<te::dt::NumericProperty*>(composite_type.getPropertyById(1))->getName() == num_prop->getName() );

  // Do not have this property
  te::dt::SimpleProperty intt("attribute_int", te::dt::INT32_TYPE, true, 0, 23);
  CPPUNIT_ASSERT(composite_type.has(&intt) == false);
  CPPUNIT_ASSERT(composite_type.getProperty("attribute_int") == 0);

  // Finding properties
  te::dt::Property* firstp = composite_type.findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);
  CPPUNIT_ASSERT(composite_type.hasPropertyOfType(te::dt::INT32_TYPE));  //"attribute_int_serial"
  CPPUNIT_ASSERT((composite_type.findFirstPropertyOfType(te::dt::INT32_TYPE))->getName() == "attribute_int_serial");
  CPPUNIT_ASSERT((composite_type.findFirstPropertyOfType(te::dt::NUMERIC_TYPE))->getName() == "attribute_numeric");
  CPPUNIT_ASSERT((composite_type.findFirstPropertyOfType(te::dt::GEOMETRY_TYPE))->getName() == "attribute_geom_polm");
  CPPUNIT_ASSERT((composite_type.findFirstPropertyOfType(te::dt::STRING_TYPE))->getName() == "attribute_fixed_string");

  // Clone
  te::dt::Property* composite_type_clone = composite_type.clone();
  CPPUNIT_ASSERT(composite_type_clone->getName() == "composite_name");
  CPPUNIT_ASSERT(composite_type_clone->getId() == 666);
  CPPUNIT_ASSERT(composite_type_clone->getParent() == 0);

  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getCompositeName() == "attribute_composite_type");
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->size() == 6);
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->has(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getProperty(0)));
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->has(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getProperty(1)));
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->has(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getProperty("attribute_var_string")));
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->has(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getProperty(3)));
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->has(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getProperty(4)));
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->has(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getProperty(5)));

  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getPropertyPosition("attribute_numeric") == 0 );
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getPropertyPosition("attribute_fixed_string") == 1 );
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getPropertyPosition("attribute_geom_polm") == 2 );
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getPropertyPosition("attribute_var_string") == 3 );
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getPropertyPosition("attribute_var_unlimited_string") == 4 );
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getPropertyPosition("attribute_int_serial") == 5 );

  // Remove one property from composite_type instance
  te::dt::StringProperty* rem = static_cast<te::dt::StringProperty*>(composite_type.getProperty("attribute_fixed_string"));
  composite_type.remove(rem);
  CPPUNIT_ASSERT(composite_type.size() == 5);
  CPPUNIT_ASSERT(composite_type.getCompositeName() == "attribute_composite_type");
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty(0)));
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty(1)));
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty("attribute_fixed_string")) == false);
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty(2)));
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty(3)));
  CPPUNIT_ASSERT(composite_type.has(composite_type.getProperty(4)));
  CPPUNIT_ASSERT(composite_type.getPropertyPosition("attribute_numeric") == 0 );
  CPPUNIT_ASSERT(composite_type.getPropertyPosition("attribute_geom_polm") == 1 );
  CPPUNIT_ASSERT(composite_type.getPropertyPosition("attribute_var_string") == 2 );
  CPPUNIT_ASSERT(composite_type.getPropertyPosition("attribute_var_unlimited_string") == 3 );
  CPPUNIT_ASSERT(composite_type.getPropertyPosition("attribute_int_serial") == 4 );

  // Get vector of properties
  std::vector<te::dt::Property*> pss = composite_type.getProperties();
  CPPUNIT_ASSERT(pss.size() == 5 );

  // Clear CompositeProperty cloned above (6 properties)
  static_cast<te::dt::CompositeProperty*>(composite_type_clone)->clear();
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->size() == 0);
  CPPUNIT_ASSERT(static_cast<te::dt::CompositeProperty*>(composite_type_clone)->getCompositeName() == "attribute_composite_type");  
  CPPUNIT_ASSERT(composite_type_clone->getName() == "composite_name");
  CPPUNIT_ASSERT(composite_type_clone->getId() == 666);
  CPPUNIT_ASSERT(composite_type_clone->getParent() == 0);
}

void TsPropertyTypes::tcBitProperty()
{
  te::dt::BitProperty bitp("attr_bit");
  te::dt::BitProperty bitpn("attr_bitn", 4, false, new std::string("0000"), 23, 0);

  CPPUNIT_ASSERT(bitp.getName() == "attr_bit");
  CPPUNIT_ASSERT(bitp.getType() == te::dt::BIT_TYPE);
  CPPUNIT_ASSERT(bitp.getId() == 0);
  CPPUNIT_ASSERT(bitp.getParent() == 0); 
  CPPUNIT_ASSERT(bitp.isRequired() == false);
  CPPUNIT_ASSERT(bitp.getDefaultValue() == 0);
  CPPUNIT_ASSERT(bitp.numBits() == 1);

  bitp.setNumBits(2);
  bitp.setDefaultValue(new std::string("11"));
  std::string* def = bitp.getDefaultValue();
  CPPUNIT_ASSERT(*(bitp.getDefaultValue()) == "11");
  CPPUNIT_ASSERT(bitp.numBits() == 2);

  // Copy constructor
  te::dt::BitProperty bit_copy(bitpn);
  CPPUNIT_ASSERT(bit_copy.getName() == "attr_bitn");
  CPPUNIT_ASSERT(bit_copy.getType() == te::dt::BIT_TYPE);
  CPPUNIT_ASSERT(bit_copy.getId() == 23);
  CPPUNIT_ASSERT(bit_copy.getParent() == 0); 
  CPPUNIT_ASSERT(bit_copy.isRequired() == false);
  CPPUNIT_ASSERT(*(bit_copy.getDefaultValue()) == "0000");
  CPPUNIT_ASSERT(bit_copy.numBits() == 4);

  // clone
  bit_copy.setDefaultValue(new std::string("11111"));
  te::dt::Property* bit_clone = bit_copy.clone();
  CPPUNIT_ASSERT(bit_clone->getName() == "attr_bitn");
  CPPUNIT_ASSERT(bit_clone->getType() == te::dt::BIT_TYPE);
  CPPUNIT_ASSERT(bit_clone->getId() == 23);
  CPPUNIT_ASSERT(bit_clone->getParent() == 0); 
  CPPUNIT_ASSERT(static_cast<te::dt::BitProperty*>(bit_clone)->isRequired() == false);
  CPPUNIT_ASSERT(*(static_cast<te::dt::BitProperty*>(bit_clone)->getDefaultValue()) == "11111");
  CPPUNIT_ASSERT(static_cast<te::dt::BitProperty*>(bit_clone)->numBits() == 4);

  std::string* ss = static_cast<te::dt::BitProperty*>(bit_clone)->getDefaultValue();
  CPPUNIT_ASSERT_MESSAGE("DefaultValue is accepting value bigger than size!",static_cast<te::dt::BitProperty*>(bit_clone)->numBits() == ss->size());
}

void TsPropertyTypes::tcArrayProperty()
{
  te::dt::BitProperty* bitp= new te::dt::BitProperty("attr_bit");
  te::dt::ArrayProperty arrb("attr_array", bitp, false,0,23,0);
  te::dt::ArrayProperty arrb_copy(arrb);
  te::dt::ArrayProperty* arrb_clone = static_cast<te::dt::ArrayProperty*>(arrb_copy.clone());

  CPPUNIT_ASSERT(arrb.getType() == te::dt::ARRAY_TYPE );
  CPPUNIT_ASSERT(arrb.getName() == "attr_array");
  CPPUNIT_ASSERT(arrb.isAutoNumber() ==  false);
  CPPUNIT_ASSERT(arrb.isRequired() == false);
  CPPUNIT_ASSERT(arrb.getDefaultValue() == 0);
  CPPUNIT_ASSERT(arrb.getId() == 23); 
  CPPUNIT_ASSERT(arrb.getElementType() == bitp );
  // Check elementType´s parent
  CPPUNIT_ASSERT(((arrb.getElementType())->getParent()) == &arrb );

  te::gm::GeometryProperty* geomp = new te::gm::GeometryProperty("geom");
  te::dt::ArrayProperty arrg("attr_array", geomp);
  te::dt::ArrayProperty arrg_copy(arrg);
  CPPUNIT_ASSERT(((arrg.getElementType())->getParent()) == &arrg );
  CPPUNIT_ASSERT(arrg.getElementType() == geomp );

  //CPPUNIT_ASSERT(arrg_copy.getElementType() == geomp );
  CPPUNIT_ASSERT(arrg_copy.getElementType()->getType() == te::dt::GEOMETRY_TYPE );

  // Clone
  te::dt::ArrayProperty* arrg_clone = static_cast<te::dt::ArrayProperty*>(arrg_copy.clone());
  CPPUNIT_ASSERT(arrg_clone->getType() == te::dt::ARRAY_TYPE );
  CPPUNIT_ASSERT(arrg_clone->getName() == "attr_array");
  CPPUNIT_ASSERT(arrg_clone->isAutoNumber() ==  false);
  CPPUNIT_ASSERT(arrg_clone->isRequired() == false);
  CPPUNIT_ASSERT(arrg_clone->getDefaultValue() == 0);
  CPPUNIT_ASSERT(arrg_clone->getId() == 0);

  //CPPUNIT_ASSERT(arrg_clone->getElementType() == geomp ); 
  CPPUNIT_ASSERT(arrg_clone->getElementType()->getType() == te::dt::GEOMETRY_TYPE );
}

void TsPropertyTypes::tcRasterProperty()
{
  te::rst::RasterProperty rs("rst_prop");

  // describes the raster that you want
  te::rst::Grid* grid = new te::rst::Grid(100, 100);
  grid->setGeoreference(te::gm::Coord2D(100, 100), 29183, 1,1);

  std::vector<te::rst::BandProperty*> bprops;
  bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));

  std::map<std::string, std::string> rinfo;

  // property name is empty in this constructor
  te::rst::RasterProperty* rstp = new te::rst::RasterProperty(grid, bprops, rinfo);
   
  te::rst::RasterProperty rs_grid(grid, bprops, rinfo);

  // SimpleProperties gets...
  CPPUNIT_ASSERT(rs_grid.getType() == te::dt::RASTER_TYPE);
  CPPUNIT_ASSERT(rs_grid.getName() == "");
  CPPUNIT_ASSERT(rs_grid.isAutoNumber() == false);
  CPPUNIT_ASSERT(rs_grid.isRequired() == false);
  CPPUNIT_ASSERT(rs_grid.getDefaultValue() == 0);
  CPPUNIT_ASSERT(rs_grid.getId() == 0);
  CPPUNIT_ASSERT(rs_grid.getParent() == 0);
  
  // RasterProperties gets and sets
  std::vector<te::rst::BandProperty*> bprops_ret = rs_grid.getBandProperties();
  CPPUNIT_ASSERT(bprops_ret == bprops);
  rs_grid.setName("rst_name");
  CPPUNIT_ASSERT(rs_grid.getName() == "rst_name");  

  te::rst::BandProperty* band_add1 = new te::rst::BandProperty(1, te::dt::DOUBLE_TYPE);
  te::rst::BandProperty* band_add2 = new te::rst::BandProperty(2, te::dt::DOUBLE_TYPE);
  rs_grid.add(band_add1);
  rs_grid.set(0,band_add2);
  bprops_ret =  rs_grid.getBandProperties();
  CPPUNIT_ASSERT(band_add2 == bprops_ret[0]);
  CPPUNIT_ASSERT(band_add1 == bprops_ret[1]);
}

void TsPropertyTypes::tcRasterPropertyClone()
{
  // describes the raster that you want
  te::rst::Grid* grid = new te::rst::Grid(100, 100);
  grid->setGeoreference(te::gm::Coord2D(100, 100), 29183, 1,1);

  std::vector<te::rst::BandProperty*> bprops;
  bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));

  std::map<std::string, std::string> rinfo;

  // property name is empty in this constructor
  te::rst::RasterProperty* rstp = new te::rst::RasterProperty(grid, bprops, rinfo);
   
  te::rst::RasterProperty rs_grid(grid, bprops, rinfo);
/*
  // SimpleProperties gets...
  CPPUNIT_ASSERT(rs_grid.getType() == te::dt::RASTER_TYPE);
  CPPUNIT_ASSERT(rs_grid.getName() == "");
  CPPUNIT_ASSERT(rs_grid.isAutoNumber() == false);
  CPPUNIT_ASSERT(rs_grid.isRequired() == false);
  CPPUNIT_ASSERT(rs_grid.getDefaultValue() == 0);
  CPPUNIT_ASSERT(rs_grid.getId() == 0);
  CPPUNIT_ASSERT(rs_grid.getParent() == 0);
  
  // RasterProperties gets and sets
  std::vector<te::rst::BandProperty*> bprops_ret = rs_grid.getBandProperties();
  CPPUNIT_ASSERT(bprops_ret == bprops);
  rs_grid.setName("rst_name");
  CPPUNIT_ASSERT(rs_grid.getName() == "rst_name");  

  te::rst::BandProperty* band_add1 = new te::rst::BandProperty(1, te::dt::DOUBLE_TYPE);
  te::rst::BandProperty* band_add2 = new te::rst::BandProperty(2, te::dt::DOUBLE_TYPE);
  rs_grid.add(band_add1);
  rs_grid.set(0,band_add2);
  bprops_ret =  rs_grid.getBandProperties();
  CPPUNIT_ASSERT(band_add2 == bprops_ret[0]);
  CPPUNIT_ASSERT(band_add1 == bprops_ret[1]);
*/
  // clone 
  te::rst::RasterProperty* rstp_clone = static_cast<te::rst::RasterProperty*>(rstp->clone());
  te::rst::RasterProperty* rstp_clone1 = static_cast<te::rst::RasterProperty*>(rs_grid.clone());

  CPPUNIT_ASSERT(rstp_clone->getType() == te::dt::RASTER_TYPE);
  CPPUNIT_ASSERT(rstp_clone->getName() == "");
  CPPUNIT_ASSERT(rstp_clone->isAutoNumber() == false);
  CPPUNIT_ASSERT(rstp_clone->isRequired() == false);
  CPPUNIT_ASSERT(rstp_clone->getDefaultValue() == 0);
  CPPUNIT_ASSERT(rstp_clone->getId() == 0);
  CPPUNIT_ASSERT(rstp_clone->getParent() == 0); 
  CPPUNIT_ASSERT(rstp_clone->getBandProperties()[0] == rstp->getBandProperties()[0]); 
  CPPUNIT_ASSERT(rstp_clone->getBandProperties()[1] == rstp->getBandProperties()[1]); 
}

void TsPropertyTypes::tcRasterPropertyAssignOp()
{
  // describes the raster that you want
  te::rst::Grid* grid = new te::rst::Grid(100, 100);
  grid->setGeoreference(te::gm::Coord2D(100, 100), 29183, 1,1);

  std::vector<te::rst::BandProperty*> bprops;
  bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));

  std::map<std::string, std::string> rinfo;

  // property name is empty in this constructor
  te::rst::RasterProperty* rstp = new te::rst::RasterProperty(grid, bprops, rinfo);
   
  te::rst::RasterProperty rs_grid(grid, bprops, rinfo);
/*
  // SimpleProperties gets...
  CPPUNIT_ASSERT(rs_grid.getType() == te::dt::RASTER_TYPE);
  CPPUNIT_ASSERT(rs_grid.getName() == "");
  CPPUNIT_ASSERT(rs_grid.isAutoNumber() == false);
  CPPUNIT_ASSERT(rs_grid.isRequired() == false);
  CPPUNIT_ASSERT(rs_grid.getDefaultValue() == 0);
  CPPUNIT_ASSERT(rs_grid.getId() == 0);
  CPPUNIT_ASSERT(rs_grid.getParent() == 0);
  
  // RasterProperties gets and sets
  std::vector<te::rst::BandProperty*> bprops_ret = rs_grid.getBandProperties();
  CPPUNIT_ASSERT(bprops_ret == bprops);
  rs_grid.setName("rst_name");
  CPPUNIT_ASSERT(rs_grid.getName() == "rst_name");  

  te::rst::BandProperty* band_add1 = new te::rst::BandProperty(1, te::dt::DOUBLE_TYPE);
  te::rst::BandProperty* band_add2 = new te::rst::BandProperty(2, te::dt::DOUBLE_TYPE);
  rs_grid.add(band_add1);
  rs_grid.set(0,band_add2);
  bprops_ret =  rs_grid.getBandProperties();
  CPPUNIT_ASSERT(band_add2 == bprops_ret[0]);
  CPPUNIT_ASSERT(band_add1 == bprops_ret[1]);
*/
  // Assign Operator
  te::rst::RasterProperty rstp_assign("rs_assign_name");
  rstp_assign = *rstp;
  rstp_assign.operator=(*rstp);
  //te::rst::RasterProperty rstp_assign1("rs_assign_name1");
  //rstp_assign1.operator=(rs_grid);

  //CPPUNIT_ASSERT_MESSAGE ("Assign operator and clone do not work at te::common::Copy(rhs.m_bands, m_bands)", 1 > 1);
 
  CPPUNIT_ASSERT(rstp_assign.getType() == te::dt::RASTER_TYPE);
  CPPUNIT_ASSERT(rstp_assign.getName() == "");
  CPPUNIT_ASSERT(rstp_assign.isAutoNumber() == false);
  CPPUNIT_ASSERT(rstp_assign.isRequired() == false);
  CPPUNIT_ASSERT(rstp_assign.getDefaultValue() == 0);
  CPPUNIT_ASSERT(rstp_assign.getId() == 0);
  CPPUNIT_ASSERT(rstp_assign.getParent() == 0); 
  CPPUNIT_ASSERT(rstp_assign.getBandProperties()[0] == rstp->getBandProperties()[0]); 
  CPPUNIT_ASSERT(rstp_assign.getBandProperties()[1] == rstp->getBandProperties()[1]); 
}

void TsPropertyTypes::tcDateTimeProperty()
{
  te::dt::DateTimeProperty d1("data1", te::dt::DATE);
  te::dt::DateTimeProperty d2("datap", te::dt::DATE_PERIOD);
  te::dt::DateTimeProperty d3("data_dur", te::dt::DATE_DURATION,te::dt::YEAR);
  te::dt::DateTimeProperty d4("time_dur", te::dt::TIME_DURATION,te::dt::YEAR);
  te::dt::DateTimeProperty d5("time_inst", te::dt::TIME_INSTANT,te::dt::YEAR);
  te::dt::DateTimeProperty d6("time_period", te::dt::TIME_PERIOD,te::dt::YEAR);
  te::dt::DateTimeProperty d7("time_inst", te::dt::TIME_INSTANT_TZ,te::dt::YEAR);
  te::dt::DateTimeProperty d8("ordinal_inst", te::dt::ORDINAL_TIME_INSTANT,te::dt::YEAR);
  te::dt::DateTimeProperty d9("ordinal_period", te::dt::ORDINAL_TIME_PERIOD,te::dt::YEAR);

  CPPUNIT_ASSERT(d1.getName() == "data1");
  CPPUNIT_ASSERT(d1.getType() == te::dt::DATETIME_TYPE);
  CPPUNIT_ASSERT(d1.getSubType() == te::dt::DATE);

  CPPUNIT_ASSERT(d2.getType() == te::dt::DATETIME_TYPE);
  CPPUNIT_ASSERT(d2.getSubType() == te::dt::DATE_PERIOD);
  CPPUNIT_ASSERT(d2.getResolution() == te::dt::UNKNOWN);

  // Clone
  te::dt::DateTimeProperty* d10 = static_cast<te::dt::DateTimeProperty*>(d9.clone());
  CPPUNIT_ASSERT(d10->getName() == "ordinal_period");
  CPPUNIT_ASSERT(d10->getType() == te::dt::DATETIME_TYPE);
  CPPUNIT_ASSERT(d10->getSubType() == te::dt::ORDINAL_TIME_PERIOD);

  te::dt::DateTimeProperty d11("data1");
  d11.operator=(*d10) ;
  CPPUNIT_ASSERT(d10->getName() == "ordinal_period");
  CPPUNIT_ASSERT(d10->getType() == te::dt::DATETIME_TYPE);
  CPPUNIT_ASSERT(d10->getSubType() == te::dt::ORDINAL_TIME_PERIOD);
}
