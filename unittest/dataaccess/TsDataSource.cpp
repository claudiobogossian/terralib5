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
  \file TsDataSource.cpp
 
  \brief A test suit for the DataSource interface.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>

// Unit-Test TerraLib
#include "TsDataSource.h"
#include "TsManagerDataSource.h"


CPPUNIT_TEST_SUITE_REGISTRATION( TsDataSource );

void TsDataSource::setUp()
{
  m_ds = TsManagerDataSource::sm_datasource;
  m_connInfo = TsManagerDataSource::sm_connInfo;
  m_connInfoExist = TsManagerDataSource::sm_connInfoExist;
  m_dsType = TsManagerDataSource::sm_dsType;
  m_connInfoNewDs  = TsManagerDataSource::sm_connInfoNewDs;
  m_connInfoDropDs = TsManagerDataSource::sm_connInfoDropDs;
  m_connStr = TsManagerDataSource::sm_connStr;
  m_capabilit = TsManagerDataSource::sm_capabilit;

  //To use equivalent methods from transactor
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

  int i=0;
}

void TsDataSource::tearDown()
{
  m_ds = 0; 
}

void TsDataSource::tcGetType()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT(m_ds->getType()== m_dsType);
//#endif
}

void TsDataSource::tcOpenWithMap()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  //CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connInfo));
  CPPUNIT_ASSERT_NO_THROW(m_ds->open());
//#endif
}

void TsDataSource::tcOpenWithStr()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  //CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connStr));
  CPPUNIT_ASSERT_NO_THROW(m_ds->open());

//#endif
}

void TsDataSource::tcGetId()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT_NO_THROW(m_ds->open());
  std::string ds_id = m_ds->getId();
  CPPUNIT_ASSERT(ds_id.size()!=0);

//#endif
}

void TsDataSource::tcSetId()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  std::string id_ds = "xxx";
  m_ds->setId(id_ds);
  CPPUNIT_ASSERT(m_ds->getId()==id_ds);

//#endif
}
void TsDataSource::tcClose()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
  CPPUNIT_ASSERT_NO_THROW(m_ds->close());
  CPPUNIT_ASSERT(m_ds->isOpened()== false);
//#endif
}


void TsDataSource::tcIsOpened()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
//#endif
}

void TsDataSource::tcIsValid()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isValid()== true);
//#endif
}

void TsDataSource::tcGetCatalog()
{
/*//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->getCatalog() != 0);
  CPPUNIT_ASSERT(m_ds->getCatalog()->getNumberOfDataSets() == 0);

// get a transactor to retrieve the datasource catalog
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalog loader and load the datasource catalog before calling getCatalog
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  CPPUNIT_ASSERT_NO_THROW(cl->loadCatalog(false)); //false: minimum information (properties)

// now you can get the datasource catalog
  te::da::DataSourceCatalog* ct = m_ds->getCatalog();
  CPPUNIT_ASSERT(ct);
  CPPUNIT_ASSERT(ct->getNumberOfDataSets() != 0);

//#endif
*/
}

void TsDataSource::tcGetCatalogFull()
{
/*
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->getCatalog() != 0);
// This time the catalog is not completetly empty (m_ds->close() does not clear it).

// get a transactor to retrieve the data source catalog
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalog loader and load the data source catalog before calling getCatalog
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  CPPUNIT_ASSERT_NO_THROW(cl->loadCatalog(true)); //true: Load maximum information(fk,pk,uk,idx,etc)

  te::da::DataSourceCatalog* ct = m_ds->getCatalog();
  CPPUNIT_ASSERT(ct);
  CPPUNIT_ASSERT(ct->getNumberOfDataSets() != 0);

//#endif
*/
}

void TsDataSource::tcGetCapabilities()
{
//#ifdef TE_COMPILE_ALL
  const te::da::DataSourceCapabilities& capabilities = m_ds->getCapabilities();

  CPPUNIT_ASSERT_NO_THROW(m_ds->getCapabilities());
//#endif
}


void TsDataSource::tcGetTransactor()
{
//#ifdef TE_COMPILE_ALL
  // get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(m_ds->getTransactor());
  CPPUNIT_ASSERT(t.get());

//#endif
}

void TsDataSource::tcGetConnectionInfo()
{
//#ifdef TE_COMPILE_ALL
  std::map<std::string, std::string>  connInfo;
  connInfo = m_ds->getConnectionInfo();
  size_t size_connInfo = connInfo.size();

  CPPUNIT_ASSERT(size_connInfo > 0);

//#endif
}

void TsDataSource::tcExist()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT_NO_THROW(m_ds->setConnectionInfo(m_connInfoExist));
  CPPUNIT_ASSERT(m_ds->exists(m_dsType, m_connInfoExist)== true);
  int ii=0;
//#endif

}
void TsDataSource::tcCreate()
{
//#ifdef TE_COMPILE_ALL

// creating a postgis database 
  try 
  {
    //te::da::DataSourcePtr* newDb = te::da::DataSource::create(m_dsType, m_connInfoNewDs);
    std::auto_ptr<te::da::DataSource>  newDb = te::da::DataSource::create(m_dsType, m_connInfoNewDs);

    CPPUNIT_ASSERT(newDb.get());
    //delete newDb;
  }
  catch (te::common::Exception  e)
  {
    throw e;
  }

//#endif
}

void TsDataSource::tcDrop()
{
//#ifdef TE_COMPILE_ALL

// dropping the datasource (Postgis, Gdal, Ogr) created before 
  te::da::DataSource::drop(m_dsType, m_connInfoDropDs);

//#endif
}

//void TsDataSource::tcGetConnection()
//{
////#ifdef TE_COMPILE_ALL
//  te::da::Connection* conn;
//  CPPUNIT_ASSERT(m_ds);
//  conn = m_ds->getConnection();
//  CPPUNIT_ASSERT(conn!= 0);
////#endif
//}

void TsDataSource::tcSetConnectionInfo()
{
//#ifdef TE_COMPILE_ALL
  std::map<std::string, std::string>  connInfo;

  CPPUNIT_ASSERT(m_ds);
  m_ds->setConnectionInfo(m_connInfo);
  connInfo = m_ds->getConnectionInfo();
  CPPUNIT_ASSERT(connInfo == m_connInfo);

//#endif
}

void TsDataSource::tcCloseConnection()
{
////#ifdef TE_COMPILE_ALL
//  te::da::Connection* conn = m_ds->getConnection();
//  CPPUNIT_ASSERT(m_ds);
//  //m_ds->closeConnection(conn);
//  CPPUNIT_ASSERT_NO_THROW(m_ds->closeConnection(conn));
//
////#endif
}

void TsDataSource::tcSQLDialect()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  const te::da::SQLDialect* dialect = m_ds->getDialect();
  CPPUNIT_ASSERT(dialect != 0);
//#endif
}

////void TsDataSource::tcGetDataSources()
////{
//////#ifdef TE_COMPILE_ALL
////  CPPUNIT_ASSERT(m_ds.get());
////  std::vector<std::string> dbs = m_ds->getDataSources(m_dsType,m_connInfo);
//// 
////  CPPUNIT_ASSERT(dbs.size() >= 0);
//////#endif
////}

void TsDataSource::tcGetEncodings()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  std::vector<te::common::CharEncoding> encodings = m_ds->getEncodings(m_dsType,m_connInfo);
 
  CPPUNIT_ASSERT(encodings.size() >= 0);
//#endif
}

//marisa
void TsDataSource::tcGetDataSetByName()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);

  std::vector<std::string>  datasets = m_ds->getDataSetNames();
  CPPUNIT_ASSERT(datasets.size() > 0);
  std::auto_ptr<te::da::DataSet> dt; 

// get dataSet by name without transactor
  try
  {
    size_t i=0;
    while (i < datasets.size() && i < 10)   //to save time during test
    {
      dt = m_ds->getDataSet(datasets[i]);
      CPPUNIT_ASSERT_NO_THROW(m_ds->getDataSet(datasets[i]));
      i++;
    }
  }
  catch (te::common::Exception e)
  {
    throw e;
  }
//#endif
}

void TsDataSource::tcGetDataSetByGeometry()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);

  if (!((m_capabilit.getDataTypeCapabilities()).supportsGeometry()))
    //CPPUNIT_ASSERT_THROW_MESSAGE("GEOMETRY_DT is not supported by this datasource",(m_capabilit.getDataTypeCapabilities()).supportsGeometry() == 0, te::common::Exception);
    CPPUNIT_ASSERT_THROW_MESSAGE("GEOMETRY_DT is not supported by this datasource",(m_capabilit.getDataTypeCapabilities()).supportsGeometry(), te::common::Exception);

  std::auto_ptr<te::da::DataSet> dt;
  std::vector<std::string>::iterator it = m_vecDtNames.begin();
  std::auto_ptr<te::da::DataSetType> datasetType = m_ds->getDataSetType(*it);
  te::dt::Property* p = datasetType->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);
  std::string propname = p->getName();

  //Testing getDataSet(name,geometry, ...)
  try
  {
    dt = m_ds->getDataSet((*it),propname, &m_pt,te::gm::INTERSECTS);
    CPPUNIT_ASSERT_NO_THROW(dt = m_ds->getDataSet((*it),propname, &m_pt,te::gm::INTERSECTS)); //point
    int nprop = dt->getNumProperties();
    int pos = static_cast<int>(te::da::GetFirstSpatialPropertyPos(dt.get()));
    dt->moveNext();
    m_geom = static_cast<te::gm::Geometry*>(dt->getGeometry(pos)->clone());
    m_geom->computeMBR(true);

    std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() <<  std::endl ;

    CPPUNIT_ASSERT_NO_THROW(dt = m_ds->getDataSet((*it),propname, &m_box, te::gm::INTERSECTS)); //box
    std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() << std::endl ;

    CPPUNIT_ASSERT_NO_THROW(dt = m_ds->getDataSet((*it),propname, m_pol, te::gm::INTERSECTS)); //polygon
    std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size()  << std::endl ;
  
    CPPUNIT_ASSERT_NO_THROW(dt = m_ds->getDataSet((*it),propname, m_geom , te::gm::INTERSECTS)); //using the geometry recovered above
    std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size()  << std::endl ;
  }
  catch (te::common::Exception  e)
  { 
    throw e;
  }

//#endif
}

void TsDataSource::tcGetDataSetByProperty()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
// get a transactor to retrieve information about the data source 
  CPPUNIT_ASSERT_NO_THROW(m_ds->open());

  if (!((m_capabilit.getDataTypeCapabilities()).supportsGeometry()))
    //CPPUNIT_ASSERT_THROW_MESSAGE("GEOMETRY_DT is not supported by this datasource", (m_capabilit.getDataTypeCapabilities()).supportsGeometry() == false,te::common::Exception);
    CPPUNIT_ASSERT_THROW_MESSAGE("GEOMETRY_DT is not supported by this datasource", (m_capabilit.getDataTypeCapabilities()).supportsGeometry(),te::common::Exception);

  std::vector<std::string>  datasets = m_ds->getDataSetNames();

  std::auto_ptr<te::da::DataSet> dt;
  std::vector<std::string>::iterator it = m_vecDtNames.begin();
  std::auto_ptr<te::da::DataSetType> datasetType = m_ds->getDataSetType(*it);
  te::dt::Property* p = datasetType->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);
  std::string propname = p->getName();

  //Testing getDataSet(datasetname,propertyname,geometry, ...)

  try
  {
    dt = m_ds->getDataSet((*it),propname, &m_pt, te::gm::INTERSECTS);
    CPPUNIT_ASSERT_NO_THROW(dt = m_ds->getDataSet((*it),propname, &m_pt, te::gm::INTERSECTS)); //point
    int pos = datasetType->getPropertyPosition(p->getName());
    dt->moveNext();
    m_geom = static_cast<te::gm::Geometry*>(dt->getGeometry(pos)->clone());
    m_geom->computeMBR(true);

    std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() << std::endl ;

    CPPUNIT_ASSERT_NO_THROW(dt = m_ds->getDataSet((*it),propname, &m_box, te::gm::INTERSECTS)); //box
    std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() << std::endl ;

    CPPUNIT_ASSERT_NO_THROW(dt = m_ds->getDataSet((*it),propname, m_pol, te::gm::INTERSECTS)); //polygon
    std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() << std::endl ;
  
    CPPUNIT_ASSERT_NO_THROW(dt = m_ds->getDataSet((*it),propname, m_geom , te::gm::INTERSECTS)); //using the geometry recovered above
    std::cout << std::endl << "DataSet Name: " << (*it)  << " DataSet Size: " << dt->size() << std::endl ;
  }
  catch (te::common::Exception  e)
  { 
    throw e;
  }

//#endif
}

void TsDataSource::tcGetDataSetByEnvRec()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);

  if (!((m_capabilit.getDataTypeCapabilities()).supportsGeometry()))
    //CPPUNIT_ASSERT_THROW_MESSAGE("GEOMETRY_DT is not supported by this datasource", (m_capabilit.getDataTypeCapabilities()).supportsGeometry() == false,te::common::Exception);
    CPPUNIT_ASSERT_THROW_MESSAGE("GEOMETRY_DT is not supported by this datasource", (m_capabilit.getDataTypeCapabilities()).supportsGeometry(),te::common::Exception);
 
  std::vector<std::string>::iterator it = m_vecDtNames.begin();
  std::auto_ptr<te::da::DataSetType> datasetType = m_ds->getDataSetType(*it);
  te::dt::Property* p = datasetType->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);
  std::string propname = p->getName();
 
  te::gm::Envelope *rec = new te::gm::Envelope(-60.0,-20.0,-35.0,2.0);

// Testing getDataSet(name,property,envelope, spatialRelation,...)
  std::auto_ptr<te::da::DataSet> dtRec;

  CPPUNIT_ASSERT_NO_THROW(dtRec = m_ds->getDataSet(*it,propname,rec,te::gm::INTERSECTS));

  if (dtRec.get())
  {
    std::auto_ptr<te::gm::Envelope> envRec; 
    int pos = static_cast<int>(te::da::GetFirstSpatialPropertyPos(dtRec.get()));
    CPPUNIT_ASSERT_NO_THROW(envRec = (dtRec.get())->getExtent(pos));
    if (envRec.get())
    {
      std::cout << std::endl << "DataSetRec Name: " << (*it) << "Size: "<< dtRec->size() << std::endl ;
      std::cout << "DataSetRec Envelop :  " << envRec->m_llx << "," << envRec->m_lly << "," << envRec->m_urx << "," << envRec->m_ury << std::endl;
      std::cout << "Test Envelop :  " << rec->m_llx << "," << rec->m_lly << "," << rec->m_urx << "," << rec->m_ury << std::endl;
    }
  }
//#endif
}

void TsDataSource::tcGetDataSetByEnvRec1()
{
//#ifdef TE_COMPILE_ALL
  if (!((m_capabilit.getDataTypeCapabilities()).supportsGeometry()))
    //CPPUNIT_ASSERT_THROW_MESSAGE("GEOMETRY_DT is not supported by this datasource", (m_capabilit.getDataTypeCapabilities()).supportsGeometry() == false,te::common::Exception);
    CPPUNIT_ASSERT_THROW_MESSAGE("GEOMETRY_DT is not supported by this datasource", (m_capabilit.getDataTypeCapabilities()).supportsGeometry(),te::common::Exception);
 
  std::vector<std::string>::iterator it = m_vecDtNames.begin();
  std::auto_ptr<te::da::DataSetType> datasetType = m_ds->getDataSetType(*it);

  te::dt::Property* p = datasetType->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);
  std::string propname = p->getName();

  std::auto_ptr<te::da::DataSet> dtRec;

  std::vector<std::pair<std::string, te::gm::Envelope> >::iterator itpair;  
  for(itpair = m_vecNamesAndRecs.begin(); itpair < m_vecNamesAndRecs.end(); itpair++)
  {
    try
    {
      CPPUNIT_ASSERT_NO_THROW(dtRec = m_ds->getDataSet( (*itpair).first,propname, &(*itpair).second, te::gm::INTERSECTS ));
      if (dtRec.get())
      {
        std::auto_ptr<te::gm::Envelope> envRec; 
        int pos = static_cast<int>(te::da::GetFirstSpatialPropertyPos(dtRec.get()));
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

void TsDataSource::tcGetDataSetByEnvRec2()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
  if (!((m_capabilit.getDataTypeCapabilities()).supportsGeometry()))
    //CPPUNIT_ASSERT_THROW_MESSAGE("DataTypeCapabilites GEOMETRY is not supported by this datasource", (m_capabilit.getDataTypeCapabilities()).supportsGeometry() == false,te::common::Exception);
    CPPUNIT_ASSERT_THROW_MESSAGE("DataTypeCapabilites GEOMETRY is not supported by this datasource", (m_capabilit.getDataTypeCapabilities()).supportsGeometry(),te::common::Exception);

  std::auto_ptr<te::da::DataSet> dtRec;

  std::vector<std::pair<std::string, te::gm::Envelope> >::iterator itpair;
  std::vector<std::pair<std::string, size_t> >::iterator itsize;

  for(itpair = m_vecNamesAndRecs.begin(), itsize = m_vecNamesSizesRec.begin() ; itpair < m_vecNamesAndRecs.end(), itsize < m_vecNamesSizesRec.end() ; itpair++, itsize++)
  {
    try
    { 
      //getting first geometry property name...
      std::auto_ptr<te::da::DataSetType> datasetType = m_ds->getDataSetType((*itpair).first);
      te::dt::Property* p = datasetType->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);
      std::string propname = p->getName();

      CPPUNIT_ASSERT_NO_THROW(dtRec = m_ds->getDataSet( (*itpair).first, propname,&(*itpair).second, te::gm::INTERSECTS));
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

void TsDataSource::tcGetDataSetByOids()
{
}
