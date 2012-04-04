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
  \file TsDataSetTypePersistence.cpp
 
  \brief Test suite for the PostGIS DataSetTypePersistence implemention.
 */

// Unit-Test TerraLib
#include "TsDataSetTypePersistence.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/raster.h>

// STL
#include <ctime>

void TsDataSetTypePersistence::tcCreateDataSetTypeBasedOnCapabilities()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connInfo));

  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  CPPUNIT_ASSERT_NO_THROW(cl->loadCatalog(false));

  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("DATASETTYPE_PERSISTENCE");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();
  if ((it != it_end) && (it->second == "FALSE"))
    CPPUNIT_ASSERT_MESSAGE("DATASETTYPE_PERSISTENCE is not supported by this datasource", it->second == "FALSE");
  else
  {
    te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
    te::da::DataSetType *dt;

  // drop it and create again 
    if (cl->datasetExists(m_newCapabilitiesDt))
    {
      dt = cl->getDataSetType(m_newCapabilitiesDt);
      CPPUNIT_ASSERT(dt);
      CPPUNIT_ASSERT_NO_THROW(dtP->drop(dt));
    }

    dt = new te::da::DataSetType(m_newCapabilitiesDt);
  
    it = m_capabilit.find("BIT_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_bit, te::dt::BIT_TYPE));

  // te::dt::CHAR_TYPE - loop in  util.cpp - te::pgis::SetColumnDef(std::string& s, const te::dt::Property* p, bool justDataType) default:
    it = m_capabilit.find("CHAR_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_char, te::dt::CHAR_TYPE));

    it = m_capabilit.find("UCHAR_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_uchar, te::dt::UCHAR_TYPE));

    it = m_capabilit.find("INT16_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_int16, te::dt::INT16_TYPE));

    it = m_capabilit.find("UINT16_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_uint16, te::dt::UINT16_TYPE));

    it = m_capabilit.find("INT32_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_int32, te::dt::INT32_TYPE));

    it = m_capabilit.find("UINT32_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_uint32, te::dt::UINT16_TYPE));

    it = m_capabilit.find("INT64_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_int64, te::dt::INT64_TYPE));

    it = m_capabilit.find("UINT64_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_uint64, te::dt::UINT64_TYPE));

    it = m_capabilit.find("BOOLEAN_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_bool, te::dt::BOOLEAN_TYPE));

    it = m_capabilit.find("FLOAT_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_float, te::dt::FLOAT_TYPE));

    it = m_capabilit.find("DOUBLE_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_double, te::dt::DOUBLE_TYPE));

    it = m_capabilit.find("NUMERIC_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::NumericProperty(m_attr_numeric, 10, 3, true, new std::string(m_Attr1)));

    it = m_capabilit.find("STRING_DT");
    if ((it != it_end) && (it->second == "TRUE"))
    {
  // te::dt::STRING_TYPE will be used to set the datatype inside the constructor and the second parameter is the subtype
      dt->add(new te::dt::StringProperty(m_attr_string+"fixed", te::dt::FIXED_STRING));
      dt->add(new te::dt::StringProperty(m_attr_string+"var", te::dt::VAR_STRING, 50, false));
      dt->add(new te::dt::StringProperty(m_attr_string, te::dt::STRING, 0, true));
    }

    it = m_capabilit.find("BYte::dt::ARRAY_TYPE");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::SimpleProperty(m_attr_byte_array, te::dt::ARRAY_TYPE));

    it = m_capabilit.find("GEOMETRY_DT");
    if ((it != it_end) && (it->second == "TRUE"))
    { 
  // datatype will be te::dt::GEOMETRY_TYPE and subtype will be the third parameter
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"p", 4326, te::gm::PointType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"pz", 4326, te::gm::PointZType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"pzm", 4326, te::gm::PointZMType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"pm", 4326, te::gm::PointMType, true));

      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"line", 4326, te::gm::LineStringType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"linez", 4326, te::gm::LineStringZType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"linezn", 4326, te::gm::LineStringZMType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"linem", 4326, te::gm::LineStringMType, true));

      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"poly", 4326, te::gm::PolygonType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"polyz", 4326, te::gm::PolygonZType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"polyzm", 4326, te::gm::PolygonZMType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"polym", 4326, te::gm::PolygonMType, true));

      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"collec", 4326, te::gm::GeometryCollectionType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"collecz", 4326, te::gm::GeometryCollectionZType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"colleczm", 4326, te::gm::GeometryCollectionZMType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"collecm", 4326, te::gm::GeometryCollectionMType, true));
    
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multip", 4326, te::gm::MultiPointType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multipz", 4326, te::gm::MultiPointZType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multipzm", 4326, te::gm::MultiPointZMType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multipm", 4326, te::gm::MultiPointMType, true));
    
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multil", 4326, te::gm::MultiLineStringType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multilz", 4326, te::gm::MultiLineStringZType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multilzm", 4326, te::gm::MultiLineStringZMType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multilm", 4326, te::gm::MultiLineStringMType, true));

      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multipol", 4326, te::gm::MultiPolygonType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multiPolz", 4326, te::gm::MultiPolygonZType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multipolzm", 4326, te::gm::MultiPolygonZMType, true));
      dt->add(new te::gm::GeometryProperty(m_attr_geometry+"multipolm", 4326, te::gm::MultiPolygonMType, true));
    }

    it = m_capabilit.find("ARRAY_DT");
    if ((it != it_end) && (it->second == "TRUE"))
    {
      // te::dt::ARRAY_TYPE will be used inside constructor
      te::dt::SimpleProperty* elementType = new te::dt::SimpleProperty(m_attr_array, te::dt::INT16_TYPE, true, 0);
      dt->add(new te::dt::ArrayProperty(m_attr_array, elementType, true, 0));
      te::dt::SimpleProperty* elementType1 = new te::dt::SimpleProperty(m_attr_array+"32", te::dt::INT32_TYPE, true, 0);
      dt->add(new te::dt::ArrayProperty(m_attr_array+"32", elementType1, true, 0));
    }

    it = m_capabilit.find("DATETIME_DT");
    if ((it != it_end) && (it->second == "TRUE")) 
    {
  // TE_DATETIME_DT is not used inside constructor - only subType
      dt->add(new te::dt::DateTimeProperty(m_attr_datetime, te::dt::DATE));   
      dt->add(new te::dt::DateTimeProperty(m_attr_datetime+"date", te::dt::TIME_INSTANT));
      dt->add(new te::dt::DateTimeProperty(m_attr_datetime+"time", te::dt::TIME_PERIOD));
      dt->add(new te::dt::DateTimeProperty(m_attr_datetime+"tz", te::dt::TIME_INSTANT_TZ));
      dt->add(new te::dt::DateTimeProperty(m_attr_datetime+"stamp", te::dt::TIME_INSTANT));
      dt->add(new te::dt::DateTimeProperty(m_attr_datetime+"stamp_tz", te::dt::TIME_INSTANT_TZ));
    }

    it = m_capabilit.find("COMPOSITE_DT");
    if ((it != it_end) && (it->second == "TRUE"))
      dt->add(new te::dt::CompositeProperty(m_attr_composite, m_attr_composite, te::dt::COMPOSITE_TYPE));

    it = m_capabilit.find("RASTER_DT");
    if ((it != it_end) && (it->second == "TRUE"))
    {
      te::rst::Grid* grid = new te::rst::Grid(100,100);
      grid->setGeoreference(te::gm::Coord2D(100,100), 29183, 1,1);
      std::vector<te::rst::BandProperty*> bprops;
      bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));

      std::map<std::string, std::string> rinfo;
      te::rst::RasterProperty* rstp = new te::rst::RasterProperty(grid, bprops, rinfo);
      dt->add(rstp);     
    }
    
    size_t ss = dt->size();

    try 
    {
      dtP->create(dt);
    }
    catch (te::common::Exception  e)
    {
      te::da::DataSetType *dtbase = cl->getDataSetType(m_newCapabilitiesDt);
      if (dtbase)
        CPPUNIT_ASSERT_MESSAGE("Number of properties different of expected!", dtbase->size() == ss);
      throw e;
    }
  }
//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcCreateDataSetType()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connInfo));

  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  CPPUNIT_ASSERT_NO_THROW(cl->loadCatalog(false));

  te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
  CPPUNIT_ASSERT_NO_THROW(dtP->create(m_newdt));

// adding index using property m_NameAttr6
  //te::dt::Property* g = dt->getProperty(m_NameAttr6);
  //CPPUNIT_ASSERT(g);
  //te::da::Index* idx = new te::da::Index(m_NameAttr6+"_idx", te::da::B_TREE_TYPE);
  //idx->add(g);
  ////dt->add(idx);

  //CPPUNIT_ASSERT_NO_THROW(dtP->add(dt, idx));

  //CPPUNIT_ASSERT_NO_THROW(dt->getIndex(m_NameAttr6+"_idx"));

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcAddPrimaryKey()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);


  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("PRIMARY_KEY");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    te::da::PrimaryKey* pk = 0;

    // let's add a primary key for table called m_newDataSetType
    try
    {  
      te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType,true);
      CPPUNIT_ASSERT(dt);
      CPPUNIT_ASSERT(dt->getPrimaryKey() == 0);

      te::dt::Property* dtid = dt->getProperty(m_NameAttr6);
      CPPUNIT_ASSERT(dtid);

      pk = new te::da::PrimaryKey(m_Attr6Pk);
      pk->add(dtid);
 
      te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
      CPPUNIT_ASSERT_NO_THROW(dtP->add(dt, pk));
    
      CPPUNIT_ASSERT(dt->getPrimaryKey() == pk);
      CPPUNIT_ASSERT(dt->getPrimaryKey()->getDataSetType() == dt);
      CPPUNIT_ASSERT(dt->getPrimaryKey()->getName() == m_Attr6Pk);

      CPPUNIT_ASSERT(dt->getPrimaryKey()->getAssociatedIndex() == dt->getIndex(dt->getPrimaryKey()->getAssociatedIndex()->getName()));
      CPPUNIT_ASSERT(dt->getIndex(dt->getPrimaryKey()->getAssociatedIndex()->getName()) == pk->getAssociatedIndex());
      CPPUNIT_ASSERT(dt->getPrimaryKey()->getAssociatedIndex()->getName() == "public." + pk->getName());
      CPPUNIT_ASSERT(dt->getPrimaryKey()->getAssociatedIndex()->getName() == "public." + dt->getPrimaryKey()->getName());

    } 
    catch (te::common::Exception  e)
    { 
      throw e;
    }
  }
//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcAddUniqueKey()
{
//#ifdef TE_COMPILE_ALL

  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("UNIQUE_KEY");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    te::da::UniqueKey* uk = 0;

  // let's add a unique key for table called m_newDataSetType
    te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType);
    CPPUNIT_ASSERT(dt);
    CPPUNIT_ASSERT(dt->getNumberOfUniqueKeys() == 0);

    te::dt::Property* x = dt->getProperty(m_NameAttr4);
    CPPUNIT_ASSERT(x);
    te::dt::Property* y = dt->getProperty(m_NameAttr3);
    CPPUNIT_ASSERT(y);

    uk = new te::da::UniqueKey(m_Attr3e4Uk);
    uk->add(x);
    uk->add(y);

    size_t nuks = dt->getNumberOfUniqueKeys();

    te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();  
    CPPUNIT_ASSERT_NO_THROW(dtP->add(dt, uk));
  
    CPPUNIT_ASSERT(dt->getNumberOfUniqueKeys() == (nuks + 1));
    CPPUNIT_ASSERT(dt->getUniqueKey(nuks) == uk);
    CPPUNIT_ASSERT(dt->getUniqueKey(nuks)->getDataSetType() == dt);
    CPPUNIT_ASSERT(dt->getUniqueKey(m_Attr3e4Uk)->getName() == m_Attr3e4Uk);
    CPPUNIT_ASSERT(dt->getIndex(dt->getUniqueKey(nuks)->getAssociatedIndex()->getName()) == uk->getAssociatedIndex());
    CPPUNIT_ASSERT(dt->getUniqueKey(m_Attr3e4Uk)->getAssociatedIndex() == dt->getIndex(dt->getUniqueKey(m_Attr3e4Uk)->getAssociatedIndex()->getName()));
  }

//#endif  // TE_COMPILE_ALL
}

//TODO: Transformar em AddIndexBTree
void TsDataSetTypePersistence::tcAddIndex()
{
//#ifdef TE_COMPILE_ALL

  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("INDEX");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    te::da::Index* idx = 0;
    te::da::Index* idx1 = 0;


//TODO: Tansformar em addIndexRTree
  // let's add an index for table called m_newDataSetType using property m_NameAttr5

    te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType,true);
    CPPUNIT_ASSERT(dt);

    te::dt::Property* g = dt->getProperty(m_NameAttr5);
    CPPUNIT_ASSERT(g);

    idx = new te::da::Index(m_Attr5idx, te::da::R_TREE_TYPE);
    idx->add(g);

    te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();  
    CPPUNIT_ASSERT_NO_THROW(dtP->add(dt, idx));
  
    CPPUNIT_ASSERT(dt->getNumberOfIndexes() > 0);
    CPPUNIT_ASSERT(dt->getIndex(m_Attr5idx) == idx);
    CPPUNIT_ASSERT(dt->getIndex(m_Attr5idx)->getDataSetType() == dt);

  // let's add another index for table called m_newDataSetType using property m_NameAttr2 
    te::dt::Property* p = dt->getProperty(m_NameAttr2);
    CPPUNIT_ASSERT(p);

    idx1 = new te::da::Index(m_Attr2idx, te::da::B_TREE_TYPE);
    idx1->add(p);

    CPPUNIT_ASSERT_NO_THROW(dtP->add(dt, idx1));

    std::string dt_idx1name = dt->getIndex(m_Attr2idx)->getName();
    std::string idx1name = idx1->getName();
    CPPUNIT_ASSERT(dt->getNumberOfIndexes() > 0);
    CPPUNIT_ASSERT(dt->getIndex(m_Attr2idx) == idx1);
    CPPUNIT_ASSERT(dt->getIndex(m_Attr2idx)->getDataSetType() == dt);
    CPPUNIT_ASSERT(dt->getIndex(m_Attr2idx)->getName() == idx1->getName());

  // the index name will have schema in it after loading the datasetType 
    dt = cl->getDataSetType(m_newDataSetType,true);
    dt_idx1name = dt->getIndex("public."+m_Attr2idx)->getName();
    idx1name = idx1->getName();
    CPPUNIT_ASSERT(dt->getIndex(dt_idx1name)->getDataSetType() == dt);
    CPPUNIT_ASSERT(dt->getIndex(dt_idx1name)->getName() == "public." + m_Attr2idx);
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcAddCheckConstraint()
{
//#ifdef TE_COMPILE_ALL
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("CHECK_CONSTRAINTS");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    try
    {  
      te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType,true);
      CPPUNIT_ASSERT(dt);
      te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
      CPPUNIT_ASSERT_NO_THROW(dtP->add(dt, m_cc));
      CPPUNIT_ASSERT(dt->getCheckConstraint("cc_datasettype2") == m_cc); 
    } 
    catch (te::common::Exception  e)
    { 
      throw e;
    }
  }
//#endif  // TE_COMPILE_ALL
}


void TsDataSetTypePersistence::tcAddAutoIncrementProperty()
{
//#ifdef TE_COMPILE_ALL

  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("SEQUENCE");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {

    te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType);
    CPPUNIT_ASSERT(dt);

    size_t nattributes = dt->size();

    te::dt::SimpleProperty* newP = new te::dt::SimpleProperty(m_NameAttr7, te::dt::INT16_TYPE, true, 0);
    newP->setAutoNumber(true);

    te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
    CPPUNIT_ASSERT_NO_THROW(dtP->add(dt, newP));
  
    CPPUNIT_ASSERT(dt->size() == (nattributes + 1));

    te::dt::Property* p = dt->getProperty(m_NameAttr7);
    CPPUNIT_ASSERT(p);
    CPPUNIT_ASSERT(p == newP);
    CPPUNIT_ASSERT(p->getType() == te::dt::INT16_TYPE);
    te::dt::SimpleProperty* sp = static_cast<te::dt::SimpleProperty*>(p);
    CPPUNIT_ASSERT(sp);
    CPPUNIT_ASSERT(sp->isAutoNumber());
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcAddArrayProperty()
{
//#ifdef TE_COMPILE_ALL

  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("ARRAY_DT");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    try
    {
      te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType);
      CPPUNIT_ASSERT(dt);

      size_t nattributes = dt->size();

      te::dt::SimpleProperty* elementType = new te::dt::SimpleProperty(m_NameAttr8, te::dt::INT16_TYPE, true, 0);
      te::dt::ArrayProperty* newP = new te::dt::ArrayProperty(m_NameAttr8, elementType, true, 0);

      te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
      CPPUNIT_ASSERT_NO_THROW(dtP->add(dt, newP));
  
      CPPUNIT_ASSERT(dt->size() == (nattributes + 1));

      te::dt::Property* p = dt->getProperty(m_NameAttr8);
      CPPUNIT_ASSERT(p);
      CPPUNIT_ASSERT(p == newP);
      CPPUNIT_ASSERT(p->getType() == te::dt::ARRAY_TYPE);
      te::dt::ArrayProperty* ap = static_cast<te::dt::ArrayProperty*>(p);
      CPPUNIT_ASSERT(ap);
      CPPUNIT_ASSERT(ap->getElementType());
      CPPUNIT_ASSERT(ap->getElementType()->getType() == te::dt::INT16_TYPE);
    }
    catch (te::common::Exception  e)
    { 
      throw e;
    }
  }

// TODO: Other test case with the catalog in memory and heck the catalog im memory after add

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcRenameProperty()
{
//#ifdef TE_COMPILE_ALL
  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("UPDATE_QUERY");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
    CPPUNIT_ASSERT(dtP);

    try
    {
      te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType);
      CPPUNIT_ASSERT(dt);

      size_t nattributes = dt->size();
      te::dt::Property* prop = dt->getProperty(m_oldNameProp) ;
      CPPUNIT_ASSERT(prop);
      CPPUNIT_ASSERT_NO_THROW(dtP->rename(prop, m_newNameProp));

      //changing back the name...
      CPPUNIT_ASSERT_NO_THROW(dtP->rename(dt->getProperty(m_newNameProp), m_oldNameProp));
  
      te::dt::Property* p = dt->getProperty(m_oldNameProp);
      CPPUNIT_ASSERT(p);
    }
    catch (te::common::Exception  e)
    { 
      throw e;
    }
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcUpdateProperty()
{
//#ifdef TE_COMPILE_ALL
  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("UPDATE_QUERY");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());
  
    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
    CPPUNIT_ASSERT(dtP);

    te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType);
    CPPUNIT_ASSERT(dt);

    size_t nattributes = dt->size();

    //
    te::dt::Property* oldP = dt->getProperty(m_NameAttr7);
    CPPUNIT_ASSERT(oldP);
    te::dt::SimpleProperty* newP = new te::dt::SimpleProperty("simple_prop", te::dt::INT32_TYPE, false, 0);
    CPPUNIT_ASSERT_NO_THROW(dtP->update(oldP, newP));
    te::dt::Property* propUpd = dt->getProperty("simple_prop");
    CPPUNIT_ASSERT(propUpd);

    //changing back the name...
    CPPUNIT_ASSERT_NO_THROW(dtP->rename(propUpd, m_NameAttr7));
    //CPPUNIT_ASSERT_NO_THROW(dtP->rename(dt->getProperty("simple_prop"), m_NameAttr7));
  }

  ////- What(): Could not execute the sql statement due to the following error: ERROR:  column "simple_prop" cannot be cast to type "pg_catalog.int4"
  ////te::dt::Property* prop = dt->getProperty(m_NameAttr8) ;
  ////CPPUNIT_ASSERT(prop);

  ////te::dt::SimpleProperty* newP = new te::dt::SimpleProperty("simple_prop", te::dt::INT32_TYPE, true, 0);
  ////CPPUNIT_ASSERT_NO_THROW(dtP->update(prop, newP));
  ////te::dt::Property* propUpd = dt->getProperty("simple_prop");
  ////CPPUNIT_ASSERT(propUpd);

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcRemoveProperty()
{
//#ifdef TE_COMPILE_ALL
  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("UPDATE_QUERY");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType);
    CPPUNIT_ASSERT(dt);
    size_t nattributes = dt->size();

  // Removing property 
    te::dt::Property* p = dt->getProperty(m_oldNameProp); //attr8 for postgis or rasterprop for GDAL

    te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
    CPPUNIT_ASSERT_NO_THROW(dtP->drop(p));
  
    CPPUNIT_ASSERT(dt->size() == (nattributes - 1));
    CPPUNIT_ASSERT(dt->getProperty(m_oldNameProp) == 0);
  }

// TODO: transformar em outro metodo para geometry
//////// Removing property  m_NameAttr5 (geometry type - therefore 3 constraints exists) and one Rtree index were added
//////  unsigned int nCheckContstraints = dt->getNumberOfCheckConstraints();
//////  unsigned int nIndexes = dt->getNumberOfIndexes();
//////
//////  p = dt->getProperty(m_NameAttr5);
//////  CPPUNIT_ASSERT(p);
//////
//////  CPPUNIT_ASSERT_NO_THROW(dtP->drop(p));
//////    
//////  CPPUNIT_ASSERT(dt->size() == (nattributes - 2));
//////  CPPUNIT_ASSERT(dt->getProperty(m_NameAttr5) == 0);
//////  CPPUNIT_ASSERT(dt->getNumberOfCheckConstraints() == (nCheckContstraints - 3));
//////  CPPUNIT_ASSERT(dt->getNumberOfIndexes() == (nIndexes - 1));

//#endif  // TE_COMPILE_ALL
}

//TODO: transformar em um tcRemoveReferencedPrimaryKey (shoul not remove)
void TsDataSetTypePersistence::tcRemovePrimaryKey()
{
//#ifdef TE_COMPILE_ALL
  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("PRIMARY_KEY");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType, true);

    CPPUNIT_ASSERT(dt);

    te::da::PrimaryKey* pk = dt->getPrimaryKey();
    CPPUNIT_ASSERT(pk);
    std::string pkName = pk->getName();

    te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
    // pk is part of a fk in another dataset - can not remove
    CPPUNIT_ASSERT_THROW(dtP->drop(pk),te::common::Exception);
  
    CPPUNIT_ASSERT(dt->getPrimaryKey() > 0);
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcRemoveUniqueKey()
{
//#ifdef TE_COMPILE_ALL
  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("UNIQUE_KEY");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType,true);

    CPPUNIT_ASSERT(dt);
    CPPUNIT_ASSERT(dt->getNumberOfUniqueKeys() > 0);

    unsigned int nIndexes = dt->getNumberOfIndexes();
    unsigned int nuks = dt->getNumberOfUniqueKeys();
    te::da::UniqueKey* uk = dt->getUniqueKey(m_Attr3e4Uk);
    CPPUNIT_ASSERT(uk);
    std::string ukName = uk->getName();

    te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence(); 
    CPPUNIT_ASSERT_NO_THROW(dtP->drop(uk));
  
    CPPUNIT_ASSERT(dt->getNumberOfUniqueKeys() == (nuks - 1));
    CPPUNIT_ASSERT(dt->getIndex(ukName) == 0);
    CPPUNIT_ASSERT(dt->getNumberOfIndexes() == (nIndexes - 1));
  }

//#endif  // TE_COMPILE_ALL
}
void TsDataSetTypePersistence::tcRemoveCheckConstraint()
{
//#ifdef TE_COMPILE_ALL
  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("CHECK_CONSTRAINTS");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());

    try
    {  
      te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType,true);
      CPPUNIT_ASSERT(dt);

  // remove checkConstraint named cc_datasetType2
      te::da::CheckConstraint* cc = dt->getCheckConstraint("cc_datasettype2");
      CPPUNIT_ASSERT(cc);
      te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
      CPPUNIT_ASSERT_NO_THROW(dtP->drop(cc));
    } 
    catch (te::common::Exception  e)
    { 
      throw e;
    }
  }
//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcRemoveIndex()
{
//#ifdef TE_COMPILE_ALL
  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("INDEX");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    std::auto_ptr<te::da::DataSourceTransactor> t(0);
    CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
    CPPUNIT_ASSERT(t.get());

    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
    CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
    CPPUNIT_ASSERT(cl.get());
    te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType,true);

    CPPUNIT_ASSERT(dt);
    CPPUNIT_ASSERT(dt->getNumberOfIndexes() > 0);

  // removing index named m_Attr2idx (after creation the index name has the schema in it ("public.")
    unsigned int nIndexes = dt->getNumberOfIndexes();
    te::da::Index* idx = dt->getIndex("public." + m_Attr2idx);
    CPPUNIT_ASSERT(idx);
    std::string idxName = idx->getName();  

    te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
    CPPUNIT_ASSERT_NO_THROW(dtP->drop(idx));
  
    CPPUNIT_ASSERT(dt->getNumberOfIndexes() == (nIndexes - 1));
    CPPUNIT_ASSERT(dt->getIndex(idxName) == 0);

  // removing index related with primary key - it must throw an exception
    te::da::Index* pkidx =  dt->getPrimaryKey()->getAssociatedIndex();
    CPPUNIT_ASSERT(pkidx);
    CPPUNIT_ASSERT_THROW(dtP->drop(pkidx),te::common::Exception);
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcAddForeignKey()
{
//#ifdef TE_COMPILE_ALL

  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  CPPUNIT_ASSERT_NO_THROW(cl->loadCatalog(true));

  te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();

  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("FOREIGN_KEY");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
    try
    {  
      te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType3,true);
      CPPUNIT_ASSERT(dt);

  //////// first remove Pk, because it will be transformed in fk
  //////    te::da::PrimaryKey* pk = dt->getPrimaryKey();
  //////    if (pk) dtP->drop(pk);
  //////    CPPUNIT_ASSERT_NO_THROW(dtP->drop(pk)); //it is not create before for some reason- see m_newdt3 

  ////// //adding ForeignKey named sm_Attr6Fk  
  //////    te::da::ForeignKey* fk  = new te::da::ForeignKey(m_Attr6Fk);
  //////    te::dt::Property* prop = dt->getProperty(m_NameAttr6) ;
  //////    fk->add(prop);
  //////    te::da::DataSetType* dtRef = cl->getDataSetType(m_newDataSetType,true);
  //////    fk->setReferencedDataSetType(dtRef);
  //////    fk->addRefProperty(dtRef->getProperty(m_NameAttr6));
 
      CPPUNIT_ASSERT_NO_THROW(dtP->add(dt,m_fk));
      CPPUNIT_ASSERT(dt->getNumberOfForeignKeys() == 1);
      te::da::ForeignKey* fk1 = dt->getForeignKey(0);
      CPPUNIT_ASSERT(fk1);

  // checking fk after full getDataSetType and after getForeignKeys
      dt = cl->getDataSetType(m_newDataSetType3,true);
      size_t n_fk = dt->getNumberOfForeignKeys();
      std::vector<std::string> fknames;
      std::vector<std::string> rdts;
      CPPUNIT_ASSERT_NO_THROW(cl->getForeignKeys(dt, fknames, rdts));
      //NOTE: One of these methods should set the foreign keys at dt (as the other get�s methods does)
      CPPUNIT_ASSERT_MESSAGE("Alert: Foreign keys are not being loaded in getDataSetType and getForeignKeys as the other get�s methods uk,pk, etc!", n_fk >0);
    } 
    catch (te::common::Exception  e)
    { 
      throw e;
    }
  }

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcCloneTable()
{
//#ifdef TE_COMPILE_ALL
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();

  te::dt::Property* prop;
  te::da::DataSetType* dt;
  te::da::DataSetType* dt2;

  try 
  {
    dt = cl->getDataSetType(m_newDataSetType); //com parameter true (error)
    prop = dt->clone();
    dt2 = static_cast<te::da::DataSetType*>(prop);
    CPPUNIT_ASSERT(dt2);
    //set new name to the dataset cloned
    dt2->setName(m_dtcloned);
    dt2->setTitle(m_dtcloned);
    dt2->setCompositeName(m_dtcloned);

    CPPUNIT_ASSERT_NO_THROW(dtP->create(dt2));
  }
  catch (te::common::Exception  e)
  {
    throw e;
  }
  CPPUNIT_ASSERT_NO_THROW(cl->getDataSetType(dt2->getName(),true));
 
//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcRenameDataSetTypeCloned()
{
//#ifdef TE_COMPILE_ALL
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  te::da::DataSetType* dt = cl->getDataSetType(m_dtcloned,true);
  CPPUNIT_ASSERT(dt);
  te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
  CPPUNIT_ASSERT_NO_THROW(dtP->rename(dt, m_newDataSetType2)); 

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcRemoveDataSetTypeCloned()
{
//#ifdef TE_COMPILE_ALL
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType2,true);
  CPPUNIT_ASSERT(dt);
  te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
  CPPUNIT_ASSERT_NO_THROW(dtP->drop(dt)); 

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcCreateDataSetTypeWithPkUkIdxCc()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connInfo));

  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  CPPUNIT_ASSERT_NO_THROW(cl->loadCatalog(false));

// creating new dataSetType
  te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();
  CPPUNIT_ASSERT_NO_THROW(dtP->create(m_newdt3));

// checking constraints if it makes sense...
  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("PRIMARY_KEY");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();

  if ((it != it_end) && (it->second == "TRUE"))
  {
  // verifying pk, uk and associated idx - note: pk and uk do not have schema (public.) as part of the name  
    te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType3,true);
    CPPUNIT_ASSERT(cl->getDataSetType(m_newDataSetType3)->size() == 6);    
    CPPUNIT_ASSERT(dt->size() == 6);
    CPPUNIT_ASSERT_MESSAGE("Primary key should exist after create.",dt->getPrimaryKey() != 0);
    ////CPPUNIT_ASSERT(dt->getUniqueKey(m_Attr3e4Uk+"3") == uk);
    CPPUNIT_ASSERT(dt->getPrimaryKey()->getDataSetType() == dt);
    CPPUNIT_ASSERT(dt->getPrimaryKey()->getName() == m_Attr6Pk+"3");
    CPPUNIT_ASSERT(dt->getNumberOfIndexes() == 3); //uk and pk associated index and idx
    CPPUNIT_ASSERT(dt->getPrimaryKey()->getAssociatedIndex() == dt->getIndex(dt->getPrimaryKey()->getAssociatedIndex()->getName()));
    //CPPUNIT_ASSERT(dt->getIndex(dt->getPrimaryKey()->getAssociatedIndex()->getName()) == pk->getAssociatedIndex());
    CPPUNIT_ASSERT(dt->getUniqueKey(m_Attr3e4Uk+"3")->getAssociatedIndex() == dt->getIndex(dt->getUniqueKey(m_Attr3e4Uk+"3")->getAssociatedIndex()->getName()));
    CPPUNIT_ASSERT(dt->getIndex("public."+ m_Attr5idx+"3"));
  }
//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcRemoveDataSetType()
{
//#ifdef TE_COMPILE_ALL

  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType,true);
  CPPUNIT_ASSERT(dt);

  te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();

  //It can not drop m_newDataSetType if someone depends on it (fk)
  size_t n_fk = 0;
  std::map<std::string, std::string>::const_iterator it1 = m_capabilit.find("FOREIGN_KEY");
  std::map<std::string, std::string>::const_iterator it_end1= m_capabilit.end();
  if (it1->second == "TRUE")
  {
    CPPUNIT_ASSERT_THROW(dtP->drop(dt), te::common::Exception);
  }
  else 
    CPPUNIT_ASSERT_NO_THROW(dtP->drop(dt)); //no foreign_key support

//#endif  // TE_COMPILE_ALL
}

void TsDataSetTypePersistence::tcRemoveDataSetTypeWithFk()
{
  //#ifdef TE_COMPILE_ALL
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  te::da::DataSetType* dt = cl->getDataSetType(m_newDataSetType3,true);
  CPPUNIT_ASSERT(dt);

  te::da::DataSetTypePersistence* dtP = t->getDataSetTypePersistence();

// checking FK if it makes sense...
  std::map<std::string, std::string>::const_iterator it = m_capabilit.find("FOREIGN_KEY");
  std::map<std::string, std::string>::const_iterator it_end = m_capabilit.end();
  if ((it != it_end) && (it->second == "FALSE"))
    CPPUNIT_ASSERT_NO_THROW(dtP->drop(dt));
  else
  {
    size_t n_fk = 0;
    std::vector<std::string> fknames;
    std::vector<std::string> rdts;
    std::vector<std::string>::iterator itfk, itref;

    // verifying fk, pk, uk and associated idx - note: pk and uk do not have schema (public.) as part of the name  
    n_fk = dt->getNumberOfForeignKeys(); 
    CPPUNIT_ASSERT_NO_THROW(cl->getForeignKeys(dt, fknames, rdts));
    for(itfk = fknames.begin(), itref = rdts.begin(); itfk < fknames.end(), itref < rdts.end(); itfk++, itref++)
    { 
      std::string fkname = *itfk; 
      std::string rdtname = *itref;
      te::da::DataSetType* dtref = cl->getDataSetType(rdtname);
      te::da::ForeignKey* fk = cl->getForeignKey(fkname,dt,dtref);
      dt->add(fk);
    }     
    CPPUNIT_ASSERT(dt->size() == 6);
    CPPUNIT_ASSERT(dt->getPrimaryKey());
    CPPUNIT_ASSERT(dt->getUniqueKey(m_Attr3e4Uk+"3"));
    CPPUNIT_ASSERT(dt->getPrimaryKey()->getDataSetType() == dt);
    CPPUNIT_ASSERT(dt->getPrimaryKey()->getName() == m_Attr6Pk+"3");
    CPPUNIT_ASSERT(dt->getNumberOfIndexes() == 3); //uk and pk associated index and idx
    CPPUNIT_ASSERT(dt->getPrimaryKey()->getAssociatedIndex() == dt->getIndex(dt->getPrimaryKey()->getAssociatedIndex()->getName()));
    //CPPUNIT_ASSERT(dt->getIndex(dt->getPrimaryKey()->getAssociatedIndex()->getName()) == pk->getAssociatedIndex());
    CPPUNIT_ASSERT(dt->getUniqueKey(m_Attr3e4Uk+"3")->getAssociatedIndex() == dt->getIndex(dt->getUniqueKey(m_Attr3e4Uk+"3")->getAssociatedIndex()->getName()));
    CPPUNIT_ASSERT(dt->getIndex("public."+ m_Attr5idx+"3"));

    CPPUNIT_ASSERT_NO_THROW(dtP->drop(dt));
  // now it is allowed to remove m_newDataSetType referenced by m_newDataSetType3
    for(itref = rdts.begin();  itref < rdts.end();  itref++)
    { 
      std::string rdtname = *itref;
      if (cl->datasetExists(rdtname))  
      {
        te::da::DataSetType* dtref = cl->getDataSetType(rdtname);
        CPPUNIT_ASSERT_NO_THROW(dtP->drop(dtref));
      }
    }     
  }

//#endif  // TE_COMPILE_ALL
}
