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
  \file TsDataSourceCatalog.cpp
 
  \brief Test suite for the DataSourceCatalog class.
 */

// Unit-Test TerraLib
#include "TsDataSourceCatalog.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
// STL
#include <ctime>

CPPUNIT_TEST_SUITE_REGISTRATION( TsDataSourceCatalog );

void TsDataSourceCatalog::setUp()
{
}

void TsDataSourceCatalog::tearDown()
{
}

void TsDataSourceCatalog::tcCreateEmptyCatalog()
{
//#ifdef TE_COMPILE_ALL
  {
    te::da::DataSourceCatalog catalog;
    catalog.setId(101010);    

    CPPUNIT_ASSERT(catalog.getId() == 101010);

    CPPUNIT_ASSERT(catalog.getDataSource() == 0);

    CPPUNIT_ASSERT(catalog.getDataSetTypeById(10010) == 0);
    CPPUNIT_ASSERT(catalog.getDataSetType("10010") == 0);
    CPPUNIT_ASSERT(catalog.getSequenceById(11010) == 0);
    CPPUNIT_ASSERT(catalog.getSequence("11010") == 0);
  }
//#endif  // TE_COMPILE_ALL
}

//CPPUNIT_ASSERT_THROW_MESSAGE(, , te::common::Exception);

void TsDataSourceCatalog::tcOneDataSetType()
{
//#ifdef TE_COMPILE_ALL
  {
    te::da::DataSourceCatalog catalog;

    te::da::DataSetType* dt = new te::da::DataSetType("dataset_type", 666);

    CPPUNIT_ASSERT_NO_THROW(catalog.add(dt));

    dt->add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    dt->add(new te::dt::StringProperty("attribute_fixed_string",te::dt::FIXED_STRING ,50));//  te::dt::FIXED_STRING, 50));
    dt->add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    dt->add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    //dt->add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    dt->add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));
  }

  {
    te::da::DataSourceCatalog catalog;

    te::da::DataSetType* dt = new te::da::DataSetType("dataset_type", 666);

    CPPUNIT_ASSERT_NO_THROW(catalog.add(dt));

    dt->add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    dt->add(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING /*te::dt::FIXED_STRING*/ , 50));
    dt->add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    dt->add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    dt->add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    dt->add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));

    CPPUNIT_ASSERT_NO_THROW(catalog.remove(dt));
  }

  {
    te::da::DataSourceCatalog catalog;

    te::da::DataSetType* dt = new te::da::DataSetType("dataset_type", 666);

    CPPUNIT_ASSERT_NO_THROW(catalog.add(dt));

    dt->add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    dt->add(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING, 50));
    dt->add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    dt->add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    dt->add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    dt->add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));

    CPPUNIT_ASSERT_NO_THROW(catalog.rename(dt, "abacaxi"));
    CPPUNIT_ASSERT(dt->getName() == "abacaxi");
    CPPUNIT_ASSERT(catalog.getDataSetType("abacaxi") == dt);

    CPPUNIT_ASSERT_NO_THROW(catalog.setId(dt, 667));
    CPPUNIT_ASSERT(dt->getId() == 667);
    CPPUNIT_ASSERT(catalog.getDataSetTypeById(667) == dt);

    CPPUNIT_ASSERT_THROW(catalog.setId(dt, 667), te::common::Exception);
    CPPUNIT_ASSERT(dt->getId() == 667);
    CPPUNIT_ASSERT(catalog.getDataSetTypeById(667) == dt);
  }

  {
    te::da::DataSourceCatalog catalog;

    te::da::DataSetType* dt = new te::da::DataSetType("dataset_type", 666);

    CPPUNIT_ASSERT_NO_THROW(catalog.add(dt));

    dt->add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    dt->add(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING, 50));
    dt->add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    dt->add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    dt->add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    dt->add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));

    te::da::ForeignKey* fk = new te::da::ForeignKey(101011);
    fk->setName("fk1_dataset_type");
    fk->add(dt->getProperty(1));
    fk->add(dt->getProperty(2));

    CPPUNIT_ASSERT_THROW(dt->add(fk), te::common::Exception);
    CPPUNIT_ASSERT(fk->getDataSetType() == 0);

    delete fk;
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalog::tcDataSetTypes()
{
//#ifdef TE_COMPILE_ALL
  {
    te::da::DataSourceCatalog catalog;

    te::da::DataSetType* dt1 = new te::da::DataSetType("table_1", 666);

    CPPUNIT_ASSERT_NO_THROW(catalog.add(dt1));

    dt1->add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    dt1->add(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING, 50));
    dt1->add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    dt1->add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    dt1->add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    dt1->add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));

    te::da::DataSetType* dt2 = new te::da::DataSetType("table_N", 777);

    dt2->add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    dt2->add(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING, 50));
    dt2->add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    dt2->add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    dt2->add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    dt2->add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));

    te::da::ForeignKey* fk = new te::da::ForeignKey(101011);
    fk->setName("fk1_dataset_type");
    fk->add(dt2->getProperty(1));
    fk->add(dt2->getProperty(2));
    fk->addRefProperty(dt1->getProperty(1));
    fk->addRefProperty(dt1->getProperty(2));
    fk->setReferencedDataSetType(dt1);

    CPPUNIT_ASSERT_NO_THROW(dt2->add(fk));
    CPPUNIT_ASSERT(fk->getDataSetType() == dt2);
    CPPUNIT_ASSERT(fk->getReferencedDataSetType() == dt1);

    CPPUNIT_ASSERT_NO_THROW(catalog.add(dt2));

    te::da::ForeignKey* fk2 = new te::da::ForeignKey(10101134);
    fk2->setName("fk2_dataset_type");
    fk2->add(dt2->getProperty(1));
    fk2->add(dt2->getProperty(2));
    fk2->addRefProperty(dt1->getProperty(1));
    fk2->addRefProperty(dt1->getProperty(2));
    fk2->setReferencedDataSetType(dt1);

    CPPUNIT_ASSERT_NO_THROW(dt2->add(fk2));

    CPPUNIT_ASSERT_NO_THROW(dt2->remove(fk2));
    CPPUNIT_ASSERT_THROW(dt1->remove(fk), te::common::Exception);
  }

    {
    te::da::DataSourceCatalog catalog;

    te::da::DataSetType* dt1 = new te::da::DataSetType("table_1", 666);

    CPPUNIT_ASSERT_NO_THROW(catalog.add(dt1));

    dt1->add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    dt1->add(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING, 50));
    dt1->add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    dt1->add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    dt1->add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    dt1->add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));

    te::da::DataSetType* dt2 = new te::da::DataSetType("table_N", 777);

    dt2->add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    dt2->add(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING, 50));
    dt2->add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    dt2->add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    dt2->add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    dt2->add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));

    te::da::ForeignKey* fk = new te::da::ForeignKey(101011);
    fk->setName("fk1_dataset_type");
    fk->add(dt2->getProperty(1));
    fk->add(dt2->getProperty(2));
    fk->addRefProperty(dt1->getProperty(1));
    fk->addRefProperty(dt1->getProperty(2));
    fk->setReferencedDataSetType(dt1);

    CPPUNIT_ASSERT_NO_THROW(dt2->add(fk));
    CPPUNIT_ASSERT(fk->getDataSetType() == dt2);
    CPPUNIT_ASSERT(fk->getReferencedDataSetType() == dt1);

    CPPUNIT_ASSERT_NO_THROW(catalog.add(dt2));

    te::da::ForeignKey* fk2 = new te::da::ForeignKey(10101134);
    fk2->setName("fk2_dataset_type");
    fk2->add(dt2->getProperty(1));
    fk2->add(dt2->getProperty(2));
    fk2->addRefProperty(dt1->getProperty(1));
    fk2->addRefProperty(dt1->getProperty(2));
    fk2->setReferencedDataSetType(dt1);

    CPPUNIT_ASSERT_NO_THROW(dt2->add(fk2));

    CPPUNIT_ASSERT_NO_THROW(catalog.remove(dt1, true));
  }
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalog::tcOneSequence()
{
//#ifdef TE_COMPILE_ALL
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalog::tcSequences()
{
//#ifdef TE_COMPILE_ALL
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalog::tcDataSetTypesAndSequences()
{
//#ifdef TE_COMPILE_ALL
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalog::tcDataSetTypesWithForeignKeys()
{
//#ifdef TE_COMPILE_ALL
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalog::tcSequencesOwnedByDataSetTypes()
{
//#ifdef TE_COMPILE_ALL
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalog::tcMixed()
{
//#ifdef TE_COMPILE_ALL
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalog::tcClone()
{
//#ifdef TE_COMPILE_ALL
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalog::tcPerformance()
{
//#ifdef TE_COMPILE_ALL
//#endif  // TE_COMPILE_ALL
}

