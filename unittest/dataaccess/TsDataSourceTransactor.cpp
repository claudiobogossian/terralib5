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
  \file TsDataSourceTransactor.cpp
 
  \brief A Test Suit for the TsDataSourceTransactor interface.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>

// Unit-Test TerraLib
#include "TsDataSourceTransactor.h"
#include "TsManagerDataSource.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TsDataSourceTransactor );

void TsDataSourceTransactor::setUp()
{  
  m_ds = TsManagerDataSource::sm_datasource;
  m_connInfo = TsManagerDataSource::sm_connInfo;
  m_dsType = TsManagerDataSource::sm_dsType;
  m_capabilit =  m_ds->getCapabilities(); //TsManagerDataSource::sm_capabilit; 

  m_vecDtNames = TsManagerDataSource::sm_vecDtNames;
  m_vecEnvelops = TsManagerDataSource::sm_vecEnvelops;  
  m_vecDtNamesAndEnvelops = TsManagerDataSource::sm_vecDtNamesAndEnvelops;
  m_vecNamesAndRecs = TsManagerDataSource::sm_vecNamesAndRecs;
  m_vecNamesSizes = TsManagerDataSource::sm_vecNamesSizes;
  m_vecNamesSizesRec = TsManagerDataSource::sm_vecNamesSizesRec;

  m_box = TsManagerDataSource::sm_box;
  m_pt = TsManagerDataSource::sm_pt;
  m_pol = TsManagerDataSource::sm_pol;
  m_geom = TsManagerDataSource::sm_geom;

  m_nroDataSets = TsManagerDataSource::sm_nroDataSets;
}

void TsDataSourceTransactor::tearDown()
{
  m_ds = 0;
  m_dsType = "";
  m_pol = 0;
}

void TsDataSourceTransactor::tcBegin()
{
//#ifdef TE_COMPILE_ALL
//#endif
}

void TsDataSourceTransactor::tcComit()
{
//#ifdef TE_COMPILE_ALL
//#endif
}

void TsDataSourceTransactor::tcExecuteQuery()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDataSourceTransactor::tcExecuteCommand()
{
//#ifdef TE_COMPILE_ALL
//#endif
}

void TsDataSourceTransactor::tcGetDataSource()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connInfo));
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  te::da::DataSource* ds1 = t->getDataSource();
  CPPUNIT_ASSERT(ds1);

//#endif
}

void TsDataSourceTransactor::tcGetDataSet()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);

// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalogloader 
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  boost::ptr_vector<std::string>  datasets;
  cl->getDataSets(datasets);
  //CPPUNIT_ASSERT(datasets.size() == m_nroDataSets);

  te::da::DataSet* dt;
  
// get dataSet by  name 
  try
  {
    size_t i=0;
    while (i < datasets.size() && i < 10)   //to save time during test
    {
      dt = t->getDataSet(datasets[i]);
      CPPUNIT_ASSERT_NO_THROW(t->getDataSet(datasets[i]));
      i++;
    }
  }
  catch (te::common::Exception e)
  {
    throw e;
  }
  //delete datasets;
//#endif
}

void TsDataSourceTransactor::tcGetDataSetByGeometry()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
  if (!((m_capabilit.getDataTypeCapabilities()).supportsGeometry()))
    CPPUNIT_ASSERT_THROW_MESSAGE("GEOMETRY_DT is not supported by this datasource",(m_capabilit.getDataTypeCapabilities()).supportsGeometry() == 0, te::common::Exception);

  else
  {
  // get a transactor to retrieve information about the data source 
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

    te::da::DataSet* dt;
    std::vector<std::string>::iterator it = m_vecDtNames.begin();

    //Testing getDataSet(name,geometry, ...)
    try
    {
    
      CPPUNIT_ASSERT_NO_THROW(dt = t->getDataSet( (*it), &m_pt, te::gm::INTERSECTS)); //point
      int nprop = dt->getNumProperties();
      int i;
      int pos = static_cast<int>(te::da::GetFirstSpatialPropertyPos(dt));
      dt->moveNext();
      m_geom = static_cast<te::gm::Geometry*>(dt->getGeometry(pos)->clone());
      m_geom->computeMBR(true);

      std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() <<  std::endl ;

      CPPUNIT_ASSERT_NO_THROW(dt = t->getDataSet( (*it), &m_box, te::gm::INTERSECTS)); //box
      std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() << std::endl ;

      CPPUNIT_ASSERT_NO_THROW(dt = t->getDataSet( (*it), m_pol, te::gm::INTERSECTS)); //polygon
      std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size()  << std::endl ;
  
      CPPUNIT_ASSERT_NO_THROW(dt = t->getDataSet( (*it), m_geom , te::gm::INTERSECTS)); //using the geometry recovered above
      std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size()  << std::endl ;
    }
    catch (te::common::Exception  e)
    { 
      throw e;
    }
  }
//#endif
}

void TsDataSourceTransactor::tcGetDataSetByProperty()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalogloader 
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  te::da::DataSet* dt;
  std::vector<std::string>::iterator it = m_vecDtNames.begin();
  te::da::DataSetType* datasetType = cl->getDataSetType(*it);
  te::gm::GeometryProperty* p = datasetType->getDefaultGeomProperty();

  //Testing getDataSet(name,property,geometry, ...)

  if ((m_capabilit.getDataTypeCapabilities()).supportsGeometry())
  { 
    try
    {
      dt = t-> getDataSet((*it),p, &m_pt, te::gm::INTERSECTS);
      CPPUNIT_ASSERT_NO_THROW(dt = t->getDataSet((*it),p, &m_pt, te::gm::INTERSECTS)); //point
      int pos = datasetType->getDefaultGeomPropertyPos();
      dt->moveNext();
      m_geom = static_cast<te::gm::Geometry*>(dt->getGeometry(pos)->clone());
      m_geom->computeMBR(true);

      std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() << std::endl ;

      CPPUNIT_ASSERT_NO_THROW(dt = t->getDataSet( (*it), p, &m_box, te::gm::INTERSECTS)); //box
      std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() << std::endl ;

      CPPUNIT_ASSERT_NO_THROW(dt = t->getDataSet( (*it),p, m_pol, te::gm::INTERSECTS)); //polygon
      std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() << std::endl ;
  
      CPPUNIT_ASSERT_NO_THROW(dt = t->getDataSet( (*it),p, m_geom , te::gm::INTERSECTS)); //using the geometry recovered above
      std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() << std::endl ;
    }
    catch (te::common::Exception  e)
    { 
      throw e;
    }
  }
//#endif
}

void TsDataSourceTransactor::tcGetDataSetByEnvRec()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(m_ds->getTransactor());
  CPPUNIT_ASSERT(t.get());

  // get a catalogloader 
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  std::vector<std::string>::iterator it = m_vecDtNames.begin();
  te::da::DataSetType* datasetType = cl->getDataSetType(*it);
  te::gm::GeometryProperty* p = datasetType->getDefaultGeomProperty();
  
  te::gm::Envelope *rec = new te::gm::Envelope(-60.0,-20.0,-35.0,2.0);

// Testing getDataSet(name,property,envelope, spatialRelation,...)
  te::da::DataSet* dtRec;
  CPPUNIT_ASSERT_NO_THROW(dtRec = t->getDataSet(*it,p,rec,te::gm::INTERSECTS));

  if (dtRec)
  {
    te::gm::Envelope* envRec; 
    int pos = static_cast<int>(te::da::GetFirstSpatialPropertyPos(dtRec));
    CPPUNIT_ASSERT_NO_THROW(envRec = dtRec->getExtent(pos));
    if (envRec)
    {
      std::cout << std::endl << "DataSetRec Name: " << (*it) << "Size: "<< dtRec->size() << std::endl ;
      std::cout << "DataSetRec Envelop :  " << envRec->m_llx << "," << envRec->m_lly << "," << envRec->m_urx << "," << envRec->m_ury << std::endl;
      std::cout << "Test Envelop :  " << rec->m_llx << "," << rec->m_lly << "," << rec->m_urx << "," << rec->m_ury << std::endl;
    }
  }
  delete rec;
//#endif
}

void TsDataSourceTransactor::tcGetDataSetByEnvRec1()
{

//#ifdef TE_COMPILE_ALL
  std::auto_ptr<te::da::DataSourceTransactor> t(m_ds->getTransactor());
  CPPUNIT_ASSERT(t.get());

  te::da::DataSet* dtRec;
  te::gm::GeometryProperty* dtGeomRec;
  te::gm::Envelope* envRec;
  te::gm::Envelope recE;
  std::string dtname;

  std::vector<std::pair<std::string, te::gm::Envelope> >::iterator itpair;  
  for(itpair = m_vecNamesAndRecs.begin(); itpair < m_vecNamesAndRecs.end(); itpair++)
  {
    try
    {
      CPPUNIT_ASSERT_NO_THROW(dtRec = t->getDataSet( (*itpair).first, &(*itpair).second, te::gm::INTERSECTS ));
      if (dtRec)
      {
        te::gm::Envelope* envRec; 
        int pos = static_cast<int>(te::da::GetFirstSpatialPropertyPos(dtRec));
        CPPUNIT_ASSERT_NO_THROW(envRec = dtRec->getExtent(pos));
        std::cout << std::endl << "DataSet Name: " << (*itpair).first << " Size: " << dtRec->size() << std::endl ;
        std::cout << "DataSet Envelop returned: " << envRec->m_llx << "," << envRec->m_lly << "," << envRec->m_urx << "," << envRec->m_ury << std::endl;
        std::cout << "Envelop Filter          : " << (*itpair).second.m_llx << "," << (*itpair).second.m_lly << "," << (*itpair).second.m_urx << "," << (*itpair).second.m_ury << std::endl;
      }
    }
    catch (te::common::Exception  e)
    {
      throw e;
    }
  }
//#endif
}

void TsDataSourceTransactor::tcGetDataSetByEnvRec2()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  te::da::DataSet* dtRec;

  std::vector<std::pair<std::string, te::gm::Envelope> >::iterator itpair;
  std::vector<std::pair<std::string, size_t> >::iterator itsize;

  for(itpair = m_vecNamesAndRecs.begin(), itsize = m_vecNamesSizesRec.begin() ; itpair < m_vecNamesAndRecs.end(), itsize < m_vecNamesSizesRec.end() ; itpair++, itsize++)
  {
    try
    {
      CPPUNIT_ASSERT_NO_THROW(dtRec = t->getDataSet( (*itpair).first, &(*itpair).second, te::gm::INTERSECTS));
      size_t ss = dtRec->size();
      std::cout << std::endl << "DataSet Name: " << (*itpair).first  << " DataSet Size: " << dtRec->size() << " Size expected: " << (*itsize).second << std::endl ;
      CPPUNIT_ASSERT_EQUAL_MESSAGE("DataSet size not equal expected", (*itsize).second, ss);
    }
    catch (te::common::Exception  e)
    { 
      throw e;
    }
  }
//#endif
}

void TsDataSourceTransactor::tcQueryByString()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);  
  //SELECT_INTO_QUERY

  if (!((m_capabilit.getQueryCapabilities()).supportsSelectInto()))
    CPPUNIT_ASSERT_THROW_MESSAGE("SELECT_INTO_QUERY is not supported by this datasource", m_capabilit.getQueryCapabilities().supportsSelectInto() == false,te::common::Exception);
  else
  {
    std::vector<std::pair<std::string, size_t> >::iterator it = m_vecNamesSizes.begin();
    std::string sql = "Select * FROM " + (*it).first;
    std::auto_ptr<te::da::DataSourceTransactor> dstrans(m_ds->getTransactor());
    CPPUNIT_ASSERT(dstrans.get());

    te::da::DataSet* dt;
    CPPUNIT_ASSERT_NO_THROW(dt = dstrans->query(sql));
    size_t dtsize = dt->size();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Size not equal expected: ", (*it).second, dtsize);
  }
//#endif
}
void TsDataSourceTransactor::tcQueryBySelect()
{
}

void TsDataSourceTransactor::tcRollback()
{
//#ifdef TE_COMPILE_ALL
//#endif
}

void TsDataSourceTransactor::tcGetPreparedStmt()
{
}

void TsDataSourceTransactor::tcGetBatchExecutor()
{
}

void TsDataSourceTransactor::tcGetCatalogLoader()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(m_ds->getTransactor());
  CPPUNIT_ASSERT(t.get());

// get a catalogloader 
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

//#endif
}

void TsDataSourceTransactor::tcGetDataSetTypePersistence()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(m_ds->getTransactor());
  CPPUNIT_ASSERT(t.get());

// get a DataSetTypePersistence 
  CPPUNIT_ASSERT_NO_THROW(t.get()->getDataSetTypePersistence());

//#endif
}

void TsDataSourceTransactor::tcGetDataSetPersistence()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(m_ds->getTransactor());
  CPPUNIT_ASSERT(t.get());

// get a DataSetTypePersistence 
  CPPUNIT_ASSERT_NO_THROW(t.get()->getDataSetPersistence());

//#endif
}

void TsDataSourceTransactor::tcCancel()
{
}