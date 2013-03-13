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
  CPPUNIT_ASSERT(geom_name.getExtent() == 0);
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
  geom_name.setExtent(new te::gm::Envelope(1.0, 1.0, 3.0, 3.0));
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
  CPPUNIT_ASSERT(geom_name.getExtent()->m_llx == 1 &&  geom_name.getExtent()->m_lly == 1 && geom_name.getExtent()->m_urx == 3 && geom_name.getExtent()->m_ury == 3);
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
  CPPUNIT_ASSERT(geom.getExtent() == 0);
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
  CPPUNIT_ASSERT(geom_copy.getExtent() == 0);
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
  CPPUNIT_ASSERT(geom.getExtent() == 0);
  CPPUNIT_ASSERT(geom.getSRID() == 0);
  CPPUNIT_ASSERT(geom.getGeometryType() == te::gm::GeometryType);
  CPPUNIT_ASSERT(geom.getParent() == 0);

  // PolygonMType
  te::gm::GeometryProperty geompol("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331);
  geompol.setExtent(new te::gm::Envelope(-180.0, -90.0, +180.0, +90.0));

  CPPUNIT_ASSERT(geompol.getType() == te::dt::GEOMETRY_TYPE );
  CPPUNIT_ASSERT(geompol.getName() == "attribute_geom_polm");
  CPPUNIT_ASSERT(geompol.isAutoNumber() ==  false);
  CPPUNIT_ASSERT(geompol.isRequired() == true);
  CPPUNIT_ASSERT(geompol.getDefaultValue() == 0);
  CPPUNIT_ASSERT(geompol.getId() == 23331);
  CPPUNIT_ASSERT(te::gm::GetCoordDimension(geompol.getGeometryType()) == 3);
  CPPUNIT_ASSERT(geompol.getExtent());
  CPPUNIT_ASSERT(geompol.getExtent()->m_llx == -180.0 &&  geompol.getExtent()->m_lly == -90.0 && geompol.getExtent()->m_urx == +180.0 && geompol.getExtent()->m_ury == +90.0);
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
  CPPUNIT_ASSERT(static_cast<te::gm::GeometryProperty*>(geom_cl)->getExtent());

  CPPUNIT_ASSERT(static_cast<te::gm::GeometryProperty*>(geom_cl)->getExtent()->m_llx == -180.0  &&  
    static_cast<te::gm::GeometryProperty*>(geom_cl)->getExtent()->m_lly == -90.0 && 
    static_cast<te::gm::GeometryProperty*>(geom_cl)->getExtent()->m_urx == +180.0 && 
    static_cast<te::gm::GeometryProperty*>(geom_cl)->getExtent()->m_ury == +90.0);

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