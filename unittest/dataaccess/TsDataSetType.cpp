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
  \file TsDataSetType.cpp
 
  \brief Test suite for the DataSetType class.
 */

// Unit-Test TerraLib
#include "TsDataSetType.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/datatype.h>
// STL
#include <ctime>

CPPUNIT_TEST_SUITE_REGISTRATION( TsDataSetType );

void TsDataSetType::setUp()
{
}

void TsDataSetType::tearDown()
{
}

void TsDataSetType::tcCreateDataSetTypeAndChecks()
{
//#ifdef TE_COMPILE_ALL
  {
    te::da::DataSetType dt("dataset_type", 666);
    dt.add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    dt.add(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING, 50));
    dt.add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    dt.add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    dt.add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    dt.add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));
 
    CPPUNIT_ASSERT(dt.getType() == te::dt::DATASET_TYPE);
    CPPUNIT_ASSERT(dt.getName() == "dataset_type");
    CPPUNIT_ASSERT(dt.getId() == 666);
    CPPUNIT_ASSERT(dt.getParent() == 0);
    CPPUNIT_ASSERT(dt.size() == 6);
    CPPUNIT_ASSERT(dt.has(dt.getProperty(0)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(1)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(2)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(3)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(4)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(5)));

    te::dt::SimpleProperty intt("attribute_int", te::dt::INT32_TYPE, true, 0, 23);
    CPPUNIT_ASSERT(dt.has(&intt) == false);
  }

  {
    te::da::DataSetType dt("dataset_type", 666);

    dt.add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    dt.add(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING, 50));
    dt.add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    dt.add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    dt.add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    dt.add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(&dt, 1000);
    pk->setName("pk_dataset_type");
    pk->add(dt.getProperty(5));

    te::da::UniqueKey* uk = new te::da::UniqueKey(&dt, 1001);
    uk->setName("uk_dataset_type");
    uk->add(dt.getProperty(1));
    uk->add(dt.getProperty(2));

    te::da::Index* idx = new te::da::Index(&dt, 1002);
    idx->setName("uk_dataset_type");
    idx->add(dt.getProperty(1));
    idx->add(dt.getProperty(2));

    uk->setAssociatedIndex(idx);

    te::da::CheckConstraint* cc = new te::da::CheckConstraint(&dt, 10034);
    cc->setName("cc_dataset_type");
    cc->setExpression("a > b && c < d && z + 18 == 23");

    te::da::ForeignKey* fk = new te::da::ForeignKey(101011);
    fk->setName("fk1_dataset_type");
    fk->add(dt.getProperty(1));
    fk->add(dt.getProperty(2));
    dt.add(fk);

    CPPUNIT_ASSERT(dt.getType() == te::dt::DATASET_TYPE);
    CPPUNIT_ASSERT(dt.getName() == "dataset_type");
    CPPUNIT_ASSERT(dt.getId() == 666);
    CPPUNIT_ASSERT(dt.getParent() == 0);
    CPPUNIT_ASSERT(dt.size() == 6);
    CPPUNIT_ASSERT(dt.has(dt.getProperty(0)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(1)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(2)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(3)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(4)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(5)));

    CPPUNIT_ASSERT(dt.getPrimaryKey() == pk);
    CPPUNIT_ASSERT(dt.getPrimaryKey()->has(dt.getProperty(5)));
    CPPUNIT_ASSERT(dt.getPrimaryKey()->getDataSetType() == &dt);

    CPPUNIT_ASSERT(dt.getNumberOfUniqueKeys() == 1);
    CPPUNIT_ASSERT(dt.getUniqueKey(0)->has(dt.getProperty(1)));
    CPPUNIT_ASSERT(dt.getUniqueKey(0)->has(dt.getProperty(2)));
    CPPUNIT_ASSERT(dt.getUniqueKey(0)->getAssociatedIndex() == idx);

    CPPUNIT_ASSERT(dt.getNumberOfIndexes() == 1);
    CPPUNIT_ASSERT(dt.getIndex(0)->has(dt.getProperty(1)));
    CPPUNIT_ASSERT(dt.getIndex(0)->has(dt.getProperty(2)));
    CPPUNIT_ASSERT(dt.getIndex(0)->getDataSetType() == &dt);

    CPPUNIT_ASSERT(dt.getNumberOfCheckConstraints() == 1);
    CPPUNIT_ASSERT(dt.getCheckConstraint(0)->getName() == "cc_dataset_type");
    CPPUNIT_ASSERT(dt.getCheckConstraint(0)->getExpression() == "a > b && c < d && z + 18 == 23");
    CPPUNIT_ASSERT(dt.getCheckConstraint(0)->getDataSetType() == &dt);

    CPPUNIT_ASSERT(dt.getNumberOfForeignKeys() == 1);
    CPPUNIT_ASSERT(dt.getForeignKey(0)->has(dt.getProperty(1)));
    CPPUNIT_ASSERT(dt.getForeignKey(0)->has(dt.getProperty(2)));
    CPPUNIT_ASSERT(dt.getForeignKey(0)->getDataSetType() == &dt);
    CPPUNIT_ASSERT(dt.getForeignKey(0)->getReferencedDataSetType() == 0);

    te::dt::SimpleProperty intt("attribute_int", te::dt::INT32_TYPE, true, 0, 23);
    CPPUNIT_ASSERT(dt.has(&intt) == false);
    CPPUNIT_ASSERT(dt.getPrimaryKey()->has(&intt) == false);
    CPPUNIT_ASSERT(dt.getUniqueKey(0)->has(&intt) == false);
    CPPUNIT_ASSERT(dt.getIndex(0)->has(&intt) == false);
    CPPUNIT_ASSERT(dt.getForeignKey(0)->has(&intt) == false);
  }

  {
      te::da::DataSetType dt("dataset_type", 666);
      dt.add(new te::dt::DateTimeProperty("attr_date", te::dt::DATE));
      dt.add(new te::dt::DateTimeProperty("attr_date_dutarion", te::dt::DATE_DURATION));
      dt.add(new te::dt::DateTimeProperty("attr_date_period", te::dt::DATE_PERIOD));
      dt.add(new te::dt::DateTimeProperty("attr_time_duration", te::dt::TIME_DURATION));
      dt.add(new te::dt::DateTimeProperty("attr_time_instant", te::dt::TIME_INSTANT));
      dt.add(new te::dt::DateTimeProperty("attr_time_period", te::dt::TIME_PERIOD));
      dt.add(new te::dt::DateTimeProperty("attr_time_instant_tz", te::dt::TIME_INSTANT_TZ));
      dt.add(new te::dt::DateTimeProperty("attr_time_period_tz", te::dt::TIME_PERIOD_TZ));
      dt.add(new te::dt::DateTimeProperty("attr_ordinal_instant", te::dt::ORDINAL_TIME_INSTANT));
      dt.add(new te::dt::DateTimeProperty("attr_ordinal_period", te::dt::ORDINAL_TIME_PERIOD));
      int i = 0;
  }

  {
    te::da::DataSetType dt("dataset_type", 666);

    dt.add(new te::dt::NumericProperty("attribute_numeric", 10, 3, true, new std::string("34.78"), 1));
    dt.add(new te::dt::StringProperty("attribute_fixed_string", te::dt::FIXED_STRING, 50));
    dt.add(new te::dt::StringProperty("attribute_var_string", te::dt::VAR_STRING, 50, false, 0, 82));
    dt.add(new te::dt::StringProperty("attribute_var_unlimited_string", te::dt::STRING, 0, true, 0, 79));
    dt.add(new te::gm::GeometryProperty("attribute_geom_polm", 4326, te::gm::PolygonMType, true, 0, 23331));
    dt.add(new te::dt::SimpleProperty("attribute_int_serial", te::dt::INT32_TYPE, false, 0, 23));

    te::da::PrimaryKey* pk = new te::da::PrimaryKey(&dt, 1000);
    pk->setName("pk_dataset_type");
    pk->add(dt.getProperty(5));

    te::da::UniqueKey* uk = new te::da::UniqueKey(&dt, 1001);
    uk->setName("uk_dataset_type");
    uk->add(dt.getProperty(1));
    uk->add(dt.getProperty(2));

    te::da::Index* uk_idx = new te::da::Index(&dt, 1002);
    uk_idx->setName("uk_dataset_type");
    uk_idx->add(dt.getProperty(1));
    uk_idx->add(dt.getProperty(2));

    uk->setAssociatedIndex(uk_idx);

    te::da::Index* pk_idx = new te::da::Index(&dt, 1002);
    pk_idx->setName("pk_dataset_type");
    pk_idx->add(dt.getProperty(0));
    pk_idx->add(dt.getProperty(2));
    
    pk->setAssociatedIndex(pk_idx);

    te::da::Index* idx = new te::da::Index(&dt, 1002);
    idx->setName("pk_dataset_type");
    idx->add(dt.getProperty(4));

    te::da::CheckConstraint* cc = new te::da::CheckConstraint(&dt, 10034);
    cc->setName("cc_dataset_type");
    cc->setExpression("a > b && c < d && z + 18 == 23");

    te::da::ForeignKey* fk = new te::da::ForeignKey(101011);
    fk->setName("fk1_dataset_type");
    fk->add(dt.getProperty(1));
    fk->add(dt.getProperty(2));
    dt.add(fk);

    CPPUNIT_ASSERT(dt.getType() == te::dt::DATASET_TYPE);
    CPPUNIT_ASSERT(dt.getName() == "dataset_type");
    CPPUNIT_ASSERT(dt.getId() == 666);
    CPPUNIT_ASSERT(dt.getParent() == 0);
    CPPUNIT_ASSERT(dt.size() == 6);
    CPPUNIT_ASSERT(dt.has(dt.getProperty(0)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(1)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(2)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(3)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(4)));
    CPPUNIT_ASSERT(dt.has(dt.getProperty(5)));

    CPPUNIT_ASSERT(dt.getPrimaryKey() == pk);
    CPPUNIT_ASSERT(dt.getPrimaryKey()->has(dt.getProperty(5)));
    CPPUNIT_ASSERT(dt.getPrimaryKey()->getDataSetType() == &dt);
    CPPUNIT_ASSERT(dt.getPrimaryKey()->getAssociatedIndex() == pk_idx);

    CPPUNIT_ASSERT(dt.getNumberOfUniqueKeys() == 1);
    CPPUNIT_ASSERT(dt.getUniqueKey(0)->has(dt.getProperty(1)));
    CPPUNIT_ASSERT(dt.getUniqueKey(0)->has(dt.getProperty(2)));
    CPPUNIT_ASSERT(dt.getUniqueKey(0)->getAssociatedIndex() == uk_idx);

    CPPUNIT_ASSERT(dt.getNumberOfIndexes() == 3);
    CPPUNIT_ASSERT(dt.getIndex(0)->has(dt.getProperty(1)));
    CPPUNIT_ASSERT(dt.getIndex(0)->has(dt.getProperty(2)));
    CPPUNIT_ASSERT(dt.getIndex(0)->getDataSetType() == &dt);
    CPPUNIT_ASSERT(dt.getIndex(1)->has(dt.getProperty(0)));
    CPPUNIT_ASSERT(dt.getIndex(1)->has(dt.getProperty(2)));
    CPPUNIT_ASSERT(dt.getIndex(1)->getDataSetType() == &dt);
    CPPUNIT_ASSERT(dt.getIndex(2)->has(dt.getProperty(4)));
    CPPUNIT_ASSERT(dt.getIndex(2)->getDataSetType() == &dt);

    CPPUNIT_ASSERT(dt.getNumberOfCheckConstraints() == 1);
    CPPUNIT_ASSERT(dt.getCheckConstraint(0)->getName() == "cc_dataset_type");
    CPPUNIT_ASSERT(dt.getCheckConstraint(0)->getExpression() == "a > b && c < d && z + 18 == 23");
    CPPUNIT_ASSERT(dt.getCheckConstraint(0)->getDataSetType() == &dt);

    CPPUNIT_ASSERT(dt.getNumberOfForeignKeys() == 1);
    CPPUNIT_ASSERT(dt.getForeignKey(0)->has(dt.getProperty(1)));
    CPPUNIT_ASSERT(dt.getForeignKey(0)->has(dt.getProperty(2)));
    CPPUNIT_ASSERT(dt.getForeignKey(0)->getDataSetType() == &dt);
    CPPUNIT_ASSERT(dt.getForeignKey(0)->getReferencedDataSetType() == 0);

    te::dt::SimpleProperty intt("attribute_int", te::dt::INT32_TYPE, true, 0, 23);
    CPPUNIT_ASSERT(dt.has(&intt) == false);
    CPPUNIT_ASSERT(dt.getPrimaryKey()->has(&intt) == false);
    CPPUNIT_ASSERT(dt.getUniqueKey(0)->has(&intt) == false);
    CPPUNIT_ASSERT(dt.getIndex(0)->has(&intt) == false);
    CPPUNIT_ASSERT(dt.getForeignKey(0)->has(&intt) == false);

    CPPUNIT_ASSERT_NO_THROW(dt.remove(pk_idx));
    CPPUNIT_ASSERT(dt.getPrimaryKey() == 0);
    CPPUNIT_ASSERT(dt.getNumberOfIndexes() == 2);

    CPPUNIT_ASSERT_NO_THROW(dt.remove(uk_idx));
    CPPUNIT_ASSERT(dt.getNumberOfUniqueKeys() == 0);
    CPPUNIT_ASSERT(dt.getNumberOfIndexes() == 1);

    CPPUNIT_ASSERT_NO_THROW(dt.remove(fk));
    CPPUNIT_ASSERT(dt.getNumberOfForeignKeys() == 0);

    CPPUNIT_ASSERT_NO_THROW(dt.remove(dt.getIndex(0)));
    CPPUNIT_ASSERT(dt.getNumberOfIndexes() == 0);

    CPPUNIT_ASSERT_NO_THROW(dt.remove(dt.getCheckConstraint(0)));
    CPPUNIT_ASSERT(dt.getNumberOfCheckConstraints() == 0);
  }

//#endif  // TE_COMPILE_ALL
}


