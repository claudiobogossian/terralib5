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
  \file TsDataSourceCatalogLoaderLoader.cpp
 
  \brief Test suite for the DataSourceCatalogLoader class.
 */

// Unit-Test TerraLib
#include "TsDataSourceCatalogLoader.h"
#include "TsManagerDataSource.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>

// STL
#include <ctime>

CPPUNIT_TEST_SUITE_REGISTRATION( TsDataSourceCatalogLoader );

void TsDataSourceCatalogLoader::setUp()
{  
  m_ds = TsManagerDataSource::sm_datasource;
  m_connInfo = TsManagerDataSource::sm_connInfo;
  m_dsType = TsManagerDataSource::sm_dsType;
  m_capabilit =  m_ds->getCapabilities(); //TsManagerDataSource::sm_capabilit; 

  m_vecDtNames = TsManagerDataSource::sm_vecDtNames;
  m_vecPkNames = TsManagerDataSource::sm_vecPkNames;  
  m_vecUkNames = TsManagerDataSource::sm_vecUkNames;
  m_vecIdxNames = TsManagerDataSource::sm_vecIdxNames;
  m_vecFkNames = TsManagerDataSource::sm_vecFkNames;
  m_vecCcNames = TsManagerDataSource::sm_vecCcNames;
  m_vecSeqNames = TsManagerDataSource::sm_vecSeqNames;
  m_vecDtNamesAndEnvelops = TsManagerDataSource::sm_vecDtNamesAndEnvelops;
}

void TsDataSourceCatalogLoader::tearDown()
{
  m_ds = 0;
  m_dsType = "";
}

void TsDataSourceCatalogLoader::tcGetDataSets()
{
//#ifdef TE_COMPILE_ALL
  boost::ptr_vector<std::string> datasets;
  CPPUNIT_ASSERT(m_ds->isOpened()== true);

// get a transactor to retrieve the data source catalog
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalogloader and load all the dataSet names in a vector
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  cl->getDataSets(datasets);
  CPPUNIT_ASSERT(datasets.size() > 0);

//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcGetDataSetType()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);

// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalogloader and load the dataSetType information
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  try
  { 
    std::vector<std::string>::iterator it = m_vecDtNames.begin();
    for(it = m_vecDtNames.begin(); it < m_vecDtNames.end(); it++)
    {
      te::da::DataSetType *dt =  cl->getDataSetType(*it);
      //CPPUNIT_ASSERT(dt->getTitle()  == *it);
      CPPUNIT_ASSERT(dt);

      // As the second parameter of getDataSetType is false - nothing below were loaded
      // loading fk,pk,uk,idx,cc, seq  if they exist
 
      if ((m_capabilit.getDataSetTypeCapabilities()).supportsPrimaryKey())
      {
        
        CPPUNIT_ASSERT_NO_THROW(cl->getPrimaryKey(dt)); //it does not load the index associated with pk
        CPPUNIT_ASSERT_NO_THROW(cl->getUniqueKeys(dt));

        CPPUNIT_ASSERT_NO_THROW(cl->getIndexes(dt));
        CPPUNIT_ASSERT_NO_THROW(cl->getCheckConstraints(dt));

        //getting foreign keys
        std::vector<std::string> fknames;
        std::vector<std::string> rdts;
        CPPUNIT_ASSERT_NO_THROW(cl->getForeignKeys(dt, fknames, rdts));
        std::vector<std::string>::iterator itfk, itref;
        for(itfk = fknames.begin(), itref = rdts.begin(); itfk < fknames.end(), itref < rdts.end(); itfk++, itref++)
        { 
          std::string fkname = *itfk; 
          std::string rdtname = *itref;
          te::da::DataSetType* dtref = cl->getDataSetType(rdtname);
          te::da::ForeignKey* fk = cl->getForeignKey(fkname,dt,dtref);
          dt->add(fk);
        }     
        //getting sequences
        std::vector<std::string*> sequences;
        CPPUNIT_ASSERT_NO_THROW(cl->getSequences(sequences));
        std::vector<std::string*>::iterator itseq;
        for(itseq = sequences.begin(); itseq < sequences.end(); itseq++)
        { 
          std::string* ss = *itseq;
          te::da::Sequence* seq = cl->getSequence(*ss);
          CPPUNIT_ASSERT(seq->getName().compare(*ss) == 0);
        }
      }
    }
  }
  catch (te::common::Exception  e)
  { 
    throw e;
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcDataSetExists()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);

// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalogloader and load the dataSetType information
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  CPPUNIT_ASSERT_NO_THROW(cl->loadCatalog());

  try
  { 
    std::vector<std::string>::iterator it = m_vecDtNames.begin();
    for(it = m_vecDtNames.begin(); it < m_vecDtNames.end(); it++)
    {
      CPPUNIT_ASSERT_NO_THROW(cl->datasetExists(*it));
    }
  }
  catch (te::common::Exception  e)
  { 
    throw e;
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcLoadCatalog()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  if (!m_ds->isOpened() )
  {
    CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connInfo));
    CPPUNIT_ASSERT(m_ds->isOpened()== true);
    CPPUNIT_ASSERT(m_ds->getCatalog() != 0);
  }
// get a transactor to retrieve the data source catalog
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalogloader and load all the data source catalog into the catalog in memory
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  CPPUNIT_ASSERT_NO_THROW(cl->loadCatalog(false));

//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcLoadCatalogFull()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  if (!m_ds->isOpened() )
  {
    CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connInfo));
    CPPUNIT_ASSERT(m_ds->isOpened()== true);
    CPPUNIT_ASSERT(m_ds->getCatalog() != 0);
  }
// get a transactor to retrieve the data source catalog
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalogloader and load all the data source catalog into the catalog in memory
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  CPPUNIT_ASSERT_NO_THROW(cl->loadCatalog(true));

//#endif  // TE_COMPILE_ALL
}


void TsDataSourceCatalogLoader::tcAllGets()
{
//#ifdef TE_COMPILE_ALL

  CPPUNIT_ASSERT(m_ds->isOpened()== true);

// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalogloader and load the dataSetType by its name 
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

// testing all 'get' methods ...
  std::vector<std::string>::iterator it = m_vecDtNames.begin();
  for(it = m_vecDtNames.begin(); it < m_vecDtNames.end(); it++)
  {
    te::da::DataSetType *dt =  cl->getDataSetType(*it);
    te::da::DataSetType *dtfull =  cl->getDataSetType(*it,true); 

    if ((m_capabilit.getDataSetTypeCapabilities()).supportsUniqueKey())
    {
      CPPUNIT_ASSERT(dt->getNumberOfUniqueKeys() == 0);
      CPPUNIT_ASSERT_NO_THROW(cl->getUniqueKeys(dt));
      CPPUNIT_ASSERT(dt->getNumberOfUniqueKeys() == dtfull->getNumberOfUniqueKeys());

    }
    if ((m_capabilit.getDataSetTypeCapabilities()).supportsCheckConstraints())
    {
        CPPUNIT_ASSERT(dt->getNumberOfCheckConstraints() == 0);
        CPPUNIT_ASSERT_NO_THROW(cl->getCheckConstraints(dt));
        CPPUNIT_ASSERT(dt->getNumberOfCheckConstraints() == dtfull->getNumberOfCheckConstraints());
    }
    if ((m_capabilit.getDataSetTypeCapabilities()).supportsIndex())
    {
      CPPUNIT_ASSERT(dt->getNumberOfIndexes() == 0);
      CPPUNIT_ASSERT_NO_THROW(cl->getIndexes(dt));
      CPPUNIT_ASSERT(dt->getNumberOfIndexes() == dtfull->getNumberOfIndexes());
    }
    if ((m_capabilit.getDataSetTypeCapabilities()).supportsForeignKey())
    {
      CPPUNIT_ASSERT(dt->getNumberOfForeignKeys() == 0);
      CPPUNIT_ASSERT(dt->getNumberOfForeignKeys() == dtfull->getNumberOfForeignKeys());
      std::vector<std::string> fknames;
      std::vector<std::string> rdts;
      CPPUNIT_ASSERT_NO_THROW(cl->getForeignKeys(dt, fknames, rdts));
    }
    if ((m_capabilit.getDataSetTypeCapabilities()).supportsPrimaryKey())
    {
      CPPUNIT_ASSERT_NO_THROW(cl->getPrimaryKey(dt));
    }

   // testing getExtent of each dt with geom
    if ((m_capabilit.getDataTypeCapabilities()).supportsGeometry())
    {
      if (dt->hasGeom())
      {
        te::gm::GeometryProperty* geomt = dt->getDefaultGeomProperty();
        CPPUNIT_ASSERT(geomt);
        te::gm::Envelope* bbox;
        CPPUNIT_ASSERT_NO_THROW(bbox = cl->getExtent(geomt));
        CPPUNIT_ASSERT(bbox->isValid() == true);
        delete bbox;
      }
    }
    // testing raster property ???TODO
    if ((m_capabilit.getDataTypeCapabilities()).supportsRaster())
    {
      //te::dt::Property rr = dt->getProperty(0);
    }
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcGetDataSetTypeInvalid()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  if (!m_ds->isOpened() )
  {
    CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connInfo));
    CPPUNIT_ASSERT(m_ds->isOpened()== true);
    CPPUNIT_ASSERT(m_ds->getCatalog() != 0);
  }
// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalogloader and load the dataSetType by its name 
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

// try to get the dataSetType named "something" that does not exist in the dataSource  
  CPPUNIT_ASSERT_THROW(cl->getDataSetType("something"), te::common::Exception);

  //#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcGetSequences()
{
//#ifdef TE_COMPILE_ALL

  CPPUNIT_ASSERT(m_ds->isOpened()== true);

  if ((m_capabilit.getDataSetTypeCapabilities()).supportsSequence())
  {
  // get a transactor to retrieve information about the data source 
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

  // get a catalogloader and load the sequences names in a vector
    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

  // testing getSquences by name (using the vector)
    std::vector<std::string*> sequences;
    CPPUNIT_ASSERT_NO_THROW(cl->getSequences(sequences));
    std::vector<std::string*>::iterator itseq;
    for(itseq = sequences.begin(); itseq < sequences.end(); itseq++)
    { 
      std::string* ss = *itseq;
      te::da::Sequence* seq = cl->getSequence(*ss);
      CPPUNIT_ASSERT(seq->getName().compare(*ss) == 0);
    }
  }
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcGetExtent()
{
//#ifdef TE_COMPILE_ALL

  CPPUNIT_ASSERT(m_ds->isOpened()== true);

  if ((m_capabilit.getDataTypeCapabilities()).supportsGeometry())
  {
 // get a transactor to retrieve information about the data source 
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

// get a catalogloader and load the dataSetType by its name and its extent
    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    std::vector<std::string>::iterator it = m_vecDtNames.begin();
    for(it = m_vecDtNames.begin(); it < m_vecDtNames.end(); it++)
    {
      te::da::DataSetType *dt =  cl->getDataSetType(*it,true); //checar se funciona sem o true

      if (dt->hasGeom())
      {
        te::gm::GeometryProperty* geomt = dt->getDefaultGeomProperty();
        CPPUNIT_ASSERT(geomt);
        te::gm::Envelope* bbox;
        CPPUNIT_ASSERT_NO_THROW(bbox = cl->getExtent(geomt));


        CPPUNIT_ASSERT(bbox->isValid() == true);
        delete bbox;
      }
    }
  }
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcGetExtentAll()
{
//#ifdef TE_COMPILE_ALL

  CPPUNIT_ASSERT(m_ds->isOpened()== true);

// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalogloader and load the dataSetType by its name and its extent
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  std::vector<std::pair<std::string, te::gm::Envelope> >::iterator itpair;

  if ((m_capabilit.getDataTypeCapabilities()).supportsGeometry())
  {
    for(itpair = m_vecDtNamesAndEnvelops.begin(); itpair < m_vecDtNamesAndEnvelops.end(); itpair++)
    {
      te::da::DataSetType *dt =  cl->getDataSetType((*itpair).first,true); //checar se funciona sem o true

      if (dt->hasGeom())
      {
        te::gm::GeometryProperty* geomt = dt->getDefaultGeomProperty();
        CPPUNIT_ASSERT(geomt);
        te::gm::Envelope* env;
        CPPUNIT_ASSERT_NO_THROW(env = cl->getExtent(geomt));
        CPPUNIT_ASSERT(env->isValid() == true);
        if ((*itpair).second == (*env))
        {
          std::cout << std::endl << "DataSet: " << (*itpair).first << " Size: " << dt->size() << std::endl ;
          std::cout << "Envelop equal expected: " << (*itpair).second.m_llx << "," << (*itpair).second.m_lly << "," << (*itpair).second.m_urx << "," << (*itpair).second.m_ury << std::endl;
        }
        else
        {
          std::cout << std::endl << "DataSet: " << (*itpair).first << " Size: " << dt->size() << std::endl ;
          std::cout << "Envelop test     : " << (*itpair).second.m_llx << "," << (*itpair).second.m_lly << "," << (*itpair).second.m_urx << "," << (*itpair).second.m_ury << std::endl;
          std::cout << "Envelop getExtent: " << env->m_llx << "," << env->m_lly << "," << env->m_urx << "," << env->m_ury << std::endl;
        }
      }
    }
  }
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcGetTransactor()
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

// get a transactor again - it should be equal t 
  CPPUNIT_ASSERT(t.get() == cl->getTransactor());

//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcPkExists()
{
//#ifdef TE_COMPILE_ALL

  CPPUNIT_ASSERT(m_ds->isOpened()== true);

  if ((m_capabilit.getDataSetTypeCapabilities()).supportsPrimaryKey())
  {
  // get a transactor to retrieve information about the data source 
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

  // get a catalogloader and check the existence of pk given by m_vecPkNames  
    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    std::vector<std::string>::iterator it;
    for(it = m_vecPkNames.begin(); it < m_vecPkNames.end(); it++)
    {
      CPPUNIT_ASSERT(cl->primarykeyExists(*it));
    }
    CPPUNIT_ASSERT(cl->primarykeyExists("something") == false);
  }
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcUkExists()
{
//#ifdef TE_COMPILE_ALL

  CPPUNIT_ASSERT(m_ds->isOpened()== true);

  if ((m_capabilit.getDataSetTypeCapabilities()).supportsIndex()) 
  {
// get a transactor to retrieve information about the data source 
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

// get a catalogloader and check the existence of uk given by m_vecUkNames 
    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    std::vector<std::string>::iterator it;
    for(it = m_vecUkNames.begin(); it < m_vecUkNames.end(); it++)
    {
      CPPUNIT_ASSERT(cl->uniquekeyExists(*it));                      
    }
    CPPUNIT_ASSERT(cl->uniquekeyExists("something") == false);   
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcCcExists()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
  if ((m_capabilit.getDataSetTypeCapabilities()).supportsCheckConstraints()) 
  {
// get a transactor to retrieve information about the data source 
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

// get a catalogloader and check the existence of Check Constraints given by m_vecCcNames 
    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    std::vector<std::string>::iterator it;
    for(it = m_vecCcNames.begin(); it < m_vecCcNames.end(); it++)
    {
      CPPUNIT_ASSERT(cl->checkConstraintExists(*it));
    }
    CPPUNIT_ASSERT(cl->checkConstraintExists("something") == false);
  }
//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcIdxExists()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);

  if ((m_capabilit.getDataSetTypeCapabilities()).supportsIndex())
  {
  // get a transactor to retrieve information about the data source 
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

  // get a catalogloader and check the existence of index given by m_vecIdxNames 
    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    std::vector<std::string>::iterator it;
    for(it = m_vecIdxNames.begin(); it < m_vecIdxNames.end(); it++)
    {
      CPPUNIT_ASSERT(cl->indexExists(*it));
    }
    CPPUNIT_ASSERT(cl->indexExists("something") == false);
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcFkExists()
{
//#ifdef TE_COMPILE_ALL

  CPPUNIT_ASSERT(m_ds->isOpened()== true);

  if ((m_capabilit.getDataSetTypeCapabilities()).supportsForeignKey())
  {
  // get a transactor to retrieve information about the data source 
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

  // get a catalogloader and check the existence of fk given by m_vecFkNames
    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    std::vector<std::string>::iterator it;
    for(it = m_vecFkNames.begin(); it < m_vecFkNames.end(); it++)
    {
      CPPUNIT_ASSERT(cl->foreignkeyExists(*it));
    }
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSourceCatalogLoader::tcGetFks()
{
//#ifdef TE_COMPILE_ALL

  CPPUNIT_ASSERT(m_ds->isOpened()== true);

  if ((m_capabilit.getDataSetTypeCapabilities()).supportsForeignKey()) 
  {
  // get a transactor to retrieve information about the data source 
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

  // get a catalogloader and check the existence of fk given by m_vecFkNames
    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    te::da::DataSetType* dt = cl->getDataSetType("tstcatalog1",true);
    std::vector<std::string> fknames;
    std::vector<std::string> rdts;
    CPPUNIT_ASSERT_NO_THROW(cl->getForeignKeys(dt, fknames, rdts));
    size_t n_fk = dt->getNumberOfForeignKeys();
    CPPUNIT_ASSERT_MESSAGE("Alert: Foreign keys are not being loaded by methods getDataSetType with true param and method getForeignKeys as the other get´s methods uk,pk, etc!", n_fk >0);
  }

//#endif  // TE_COMPILE_ALL
}
 