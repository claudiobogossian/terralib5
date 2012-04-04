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
  \file TsDataSource.cpp
 
  \brief Test suite for TerraLib OGR driver.
 */

// Unit-Test TerraLib
#include "TsDataSource.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/ogr.h>

// STL
#include <ctime>

CPPUNIT_TEST_SUITE_REGISTRATION( TsDataSource );

void TsDataSource::setUp()
{
}

void TsDataSource::tearDown()
{
}

te::da::DataSource* TsDataSource::connectDataSource()
{
  // let's create an connect to a ORG data source
  te::ogr::DataSourceInfo* connInfo = new te::ogr::DataSourceInfo("D:\\Dados\\rodovias.shp");
  //te::ogr::DataSourceInfo* connInfo = new te::ogr::DataSourceInfo("D:\\Dados\\dgn\\samp4.dgn");
  
  CPPUNIT_ASSERT(connInfo);

  te::da::DataSource* ds = 0;
  CPPUNIT_ASSERT_NO_THROW(ds = te::da::DataSourceFactory::make(connInfo));
  CPPUNIT_ASSERT(ds);
  CPPUNIT_ASSERT_NO_THROW(ds->open());
  CPPUNIT_ASSERT_NO_THROW(ds->loadCatalog(true));

  return ds;
}

void TsDataSource::tcOpen()
{
  te::da::DataSource* ds = connectDataSource();
  CPPUNIT_ASSERT_NO_THROW(ds->close());
  CPPUNIT_ASSERT_NO_THROW(delete ds);
}

void TsDataSource::tcGetFeature()
{
  te::da::DataSource* ds = connectDataSource();
  
  te::da::DataSourceTransactor* trans = 0;
  CPPUNIT_ASSERT_NO_THROW(trans = ds->getTransactor());
  
  te::da::Feature* f = 0;
  CPPUNIT_ASSERT_NO_THROW(f = trans->getFeature(0));
  CPPUNIT_ASSERT(f);

  const te::da::FeatureType* ft = 0;
  CPPUNIT_ASSERT_NO_THROW(ft = f->getType()); 
  CPPUNIT_ASSERT(ft);

  size_t nproperties = 0;
  CPPUNIT_ASSERT_NO_THROW(nproperties = ft->size()); 

  while(f->moveNext())
  {
    for(size_t i = 0; i < nproperties; ++i)
    {
      te::da::PropertyType* p = 0;
      CPPUNIT_ASSERT_NO_THROW(p = ft->getProperty(i));
      CPPUNIT_ASSERT(p);

      switch(p->getType())
      {
        case te::da::PropertyType::INT:
          {
            int val;
            CPPUNIT_ASSERT_NO_THROW(val = f->getInt(i));
          }
        break;

        case te::da::PropertyType::VAR_STRING:
        case te::da::PropertyType::STRING:
          {
            std::string val;
            CPPUNIT_ASSERT_NO_THROW(f->getString(i, val));
          }
        break;

        case te::da::PropertyType::DOUBLE:
          {
            double val;
            CPPUNIT_ASSERT_NO_THROW(val = f->getDouble(i));
          }
        break;

        case te::da::PropertyType::GEOMETRY:
          {
            te::gm::Geometry* geom = 0;
            CPPUNIT_ASSERT_NO_THROW(geom = f->getGeometry(i));           
            CPPUNIT_ASSERT_NO_THROW(delete geom);
          }
        break;
      }
    }
  }

  CPPUNIT_ASSERT_NO_THROW(delete f);
  CPPUNIT_ASSERT_NO_THROW(delete trans);
  CPPUNIT_ASSERT_NO_THROW(ds->close());
  CPPUNIT_ASSERT_NO_THROW(delete ds);
}

void TsDataSource::tcGetFeatureWithSpatialRestriction()
{
  te::da::DataSource* ds = connectDataSource();
  CPPUNIT_ASSERT_NO_THROW(ds->loadCatalog(true));

  te::da::DataSourceCatalog* catalog = 0;
  CPPUNIT_ASSERT_NO_THROW(catalog = ds->getCatalog());
  CPPUNIT_ASSERT(catalog);

  te::da::FeatureType* ft = 0;
  CPPUNIT_ASSERT_NO_THROW(ft = catalog->getFeatureType(0));
  CPPUNIT_ASSERT(ft);

  te::gm::Envelope* env = 0;
  CPPUNIT_ASSERT_NO_THROW(env = ft->getDefaultGeomProperty()->getExtent());
  // Modify the box to execute the getFeature
  env->m_llx *= 0.8;
  env->m_lly *= 0.8;
  env->m_urx *= 0.8;
  env->m_ury *= 0.8;
   
  te::da::DataSourceTransactor* trans = 0;
  CPPUNIT_ASSERT_NO_THROW(trans = ds->getTransactor());
  
  te::da::Feature* f = 0;
  CPPUNIT_ASSERT_NO_THROW(f = trans->getFeature(0, env, te::gm::SpatialRelation::INTERSECTS));
  CPPUNIT_ASSERT(f);

  int n = 0;
  while(f->moveNext())
  {
    n++;
  }

  std::cout << "Number of Features: " << n << std::endl;

  CPPUNIT_ASSERT_NO_THROW(delete f);
  CPPUNIT_ASSERT_NO_THROW(delete trans);
  CPPUNIT_ASSERT_NO_THROW(ds->close());
  CPPUNIT_ASSERT_NO_THROW(delete ds);
}
