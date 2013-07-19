/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess2/dataset/DataSetType.h"
#include "../dataaccess2/dataset/PrimaryKey.h"
#include "../dataaccess2/query/Select.h"
#include "../dataaccess2/utils/Utils.h"
#include "../datatype/ByteArray.h"
#include "../datatype/Date.h"
#include "../datatype/TimeInstant.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"

#include "DataSet.h"
#include "DataSource.h"
#include "Globals.h"
#include "SQLVisitor.h"
#include "Utils.h"

// OGR
#include <ogrsf_frmts.h>

// STL
#include <cassert>
#include <cstring>

// Boost
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

te::da::SQLDialect* te::ogr::DataSource::sm_myDialect(0);


std::string RemoveSpatialSql(const std::string& sql)
{
  std::string newQuery;

  size_t pos = sql.find("AND Intersection");

  if(pos != std::string::npos)
  {
    size_t pos2 = sql.find("))", pos);

    newQuery = sql.substr(0, pos);
    newQuery += sql.substr(pos2+2);
  }
  else
    newQuery = sql;

  return newQuery;
}

te::ogr::DataSource::DataSource()
  : m_ogrDS(0),
    m_isValid(false)
{
}

te::ogr::DataSource::~DataSource()
{
  close();
}

std::string te::ogr::DataSource::getType() const
{
  return Globals::m_driverIdentifier;
}

const std::map<std::string, std::string>& te::ogr::DataSource::getConnectionInfo() const
{
  return m_connectionInfo;
}

void te::ogr::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connectionInfo = connInfo;
}

void te::ogr::DataSource::open()
{
  close();

  if(m_connectionInfo.empty())
    throw Exception(TR_OGR("There is no information about the data source")); 

  std::string path = m_connectionInfo.begin()->second;
  m_ogrDS = OGRSFDriverRegistrar::Open(path.c_str());
  
  if(m_ogrDS == 0)
  {
    m_isValid = false;
    throw Exception(TR_OGR("Error open data source: " + path));
  }

  m_isValid = true;
}

void te::ogr::DataSource::close()
{
  OGRDataSource::DestroyDataSource(m_ogrDS);
  m_ogrDS = 0;  
  
  m_isValid = false;
}

bool te::ogr::DataSource::isOpened() const
{
  return m_ogrDS != 0;    
}

bool te::ogr::DataSource::isValid() const
{
  return m_isValid;
}

const te::da::DataSourceCapabilities& te::ogr::DataSource::getCapabilities() const
{
  return m_capabilities;
}

const te::da::SQLDialect* te::ogr::DataSource::getDialect() const
{
  return sm_myDialect;
}

void te::ogr::DataSource::setDialect(te::da::SQLDialect* dialect)
{
  sm_myDialect = dialect;
}

void te::ogr::DataSource::begin()
{
  OGRLayer* result = m_ogrDS->ExecuteSQL("START TRANSACTION", 0, 0);

  if(result)
    m_ogrDS->ReleaseResultSet(result);

  m_isInTransaction = true;
}

void te::ogr::DataSource::commit()
{
  m_isInTransaction = false;

  OGRLayer* result = m_ogrDS->ExecuteSQL("COMMIT", 0, 0);

  if(result)
    m_ogrDS->ReleaseResultSet(result);
}

void te::ogr::DataSource::rollBack()
{
  m_isInTransaction = false;

  OGRLayer* result = m_ogrDS->ExecuteSQL("ROLLBACK", 0, 0);

  if(result)
    m_ogrDS->ReleaseResultSet(result);
}

bool te::ogr::DataSource::isInTransaction() const
{
  return m_isInTransaction;
}

std::auto_ptr<te::da::DataSet> te::ogr::DataSource::getDataSet(const std::string& name, 
                                          te::common::TraverseType /*travType*/)
{
  std::string sql = "SELECT FID, * FROM " + name;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(Exception(TR_OGR("The informed data set could not be found in the data source.")));

  return std::auto_ptr<te::da::DataSet>(new DataSet(m_ogrDS, layer, true));
}

std::auto_ptr<te::da::DataSet> te::ogr::DataSource::getDataSet(const std::string& name,
                                  const std::string& /*propertyName*/,
                                  const te::gm::Envelope* e,
                                  te::gm::SpatialRelation /*r*/,
                                  te::common::TraverseType /*travType*/)
{
  std::string sql = "SELECT FID, * FROM " + name;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(Exception(TR_OGR("The informed data set could not be found in the data source.")));

  layer->SetSpatialFilterRect(e->m_llx, e->m_lly, e->m_urx, e->m_ury);
  
  return std::auto_ptr<te::da::DataSet>(new DataSet(m_ogrDS, layer, true));
}

std::auto_ptr<te::da::DataSet> te::ogr::DataSource::getDataSet(const std::string& name,
                                  const std::string& /*propertyName*/,
                                  const te::gm::Geometry* g,
                                  te::gm::SpatialRelation /*r*/,
                                  te::common::TraverseType /*travType*/)
{
  std::string sql = "SELECT FID, * FROM " + name;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(Exception(TR_OGR("The informed data set could not be found in the data source.")));
  
  OGRGeometry* ogrg = Convert2OGR(g);

  layer->SetSpatialFilter(ogrg);
  
  OGRGeometryFactory::destroyGeometry(ogrg);
  
  return std::auto_ptr<te::da::DataSet>(new DataSet(m_ogrDS, layer, true));
}

std::auto_ptr<te::da::DataSet> te::ogr::DataSource::query(const te::da::Select& q, te::common::TraverseType /*travType*/)
{
  std::string sql;

  SQLVisitor visitor(*sm_myDialect, sql);
  q.accept(visitor);

  sql = RemoveSpatialSql(sql);

  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(Exception(TR_OGR("Could not retrieve the DataSet from data source.")));

  std::auto_ptr<te::gm::Envelope> e(visitor.getMBR());

  if(e.get() != 0)
    layer->SetSpatialFilterRect(e->m_llx, e->m_lly, e->m_urx, e->m_ury);

  return std::auto_ptr<te::da::DataSet>(new DataSet(m_ogrDS, layer, true));
}

std::auto_ptr<te::da::DataSet> te::ogr::DataSource::query(const std::string& query, te::common::TraverseType /*travType*/)
{
  OGRLayer* layer = m_ogrDS->ExecuteSQL(query.c_str(), 0, "");

  if(layer == 0)
    throw(Exception(TR_OGR("Could not retrieve the DataSet from data source.")));

  return std::auto_ptr<te::da::DataSet>(new DataSet(m_ogrDS, layer, true));
}

void te::ogr::DataSource::execute(const te::da::Query& command)
{
}

void te::ogr::DataSource::execute(const std::string& command)
{
  OGRLayer* layer = m_ogrDS->ExecuteSQL(command.c_str(), 0, "");

  if(layer)
     m_ogrDS->ReleaseResultSet(layer);
}

void te::ogr::DataSource::cancel()
{
  return;
}

boost::int64_t te::ogr::DataSource::getLastGeneratedId()
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

std::string te::ogr::DataSource::escape(const std::string& value)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

bool te::ogr::DataSource::isDataSetNameValid(const std::string& datasetName)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

bool te::ogr::DataSource::isPropertyNameValid(const std::string& propertyName)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

std::vector<std::string> te::ogr::DataSource::getDataSetNames()
{
  std::vector<std::string> datasets;
  
  int nLayers = m_ogrDS->GetLayerCount(); 

  for(int i = 0; i < nLayers; i++) // for each layer
  {
    OGRLayer* layer = m_ogrDS->GetLayer(i);
    std::string n = layer->GetLayerDefn()->GetName();
    datasets.push_back(n);
  }
  
  return datasets;
}

const te::da::DataSetTypePtr& te::ogr::DataSource::getDataSetType(const std::string& name)
{
  std::string sql = "SELECT FID, * FROM " + name;

  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(Exception(TR_OGR("Could not retrieve the DataSet from data source.")));

  te::da::DataSetType* dt = 0;
 
  try
  {
    dt = Convert2TerraLib(layer->GetLayerDefn());

    if(dt->hasGeom())
    {
      OGRSpatialReference* osrs = layer->GetSpatialRef();

      if(osrs)
      {
        te::gm::GeometryProperty* gp = dynamic_cast<te::gm::GeometryProperty*>(te::da::GetFirstSpatialProperty(dt));

        if(gp == 0)
          throw Exception(TR_OGR("Could not convert spatial property to geometry!"));

        gp->setSRID(Convert2TerraLibProjection(osrs));
      }
    }

    const char* colIdName = layer->GetFIDColumn();

    if(colIdName == 0 || colIdName[0] == '\0')
      colIdName = "FID";

    int pos = layer->GetLayerDefn()->GetFieldIndex(colIdName);

    if(pos >= 0)
    {
      te::da::PrimaryKey* pk = new te::da::PrimaryKey(colIdName, dt);
      pk->add(dt->getProperty(pos));
    }
  }
  catch(Exception& e)
  {
    m_ogrDS->ReleaseResultSet(layer);

    delete dt;

    throw e;
  }

  m_ogrDS->ReleaseResultSet(layer);

  //return std::auto_ptr<te::da::DataSetType>(dt);
  return te::da::DataSetTypePtr(dt);
}

std::size_t te::ogr::DataSource::getNumberOfProperties(const std::string& datasetName)
{
  std::string sql = "SELECT FID, * FROM " + datasetName;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(Exception(TR_OGR("Could not retrieve the DataSet from data source.")));
  
  std::auto_ptr<te::da::DataSetType> dt(Convert2TerraLib(layer->GetLayerDefn()));

  m_ogrDS->ReleaseResultSet(layer);

  return dt->getProperties().size();
}

boost::ptr_vector<te::dt::Property> te::ogr::DataSource::getProperties(const std::string& datasetName)
{
  std::string sql = "SELECT FID, * FROM " + datasetName;

  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);
  
  if(layer == 0)
    throw(Exception(TR_OGR("Could not retrieve the DataSet from data source.")));

  std::auto_ptr<te::da::DataSetType> dt(Convert2TerraLib(layer->GetLayerDefn()));

  std::vector<te::dt::Property*> pps = dt->getProperties();
  std::vector<te::dt::Property*>::iterator it;

  boost::ptr_vector<te::dt::Property> res;

  for(it=pps.begin(); it != pps.end(); ++it)
    res.push_back(*it);

  m_ogrDS->ReleaseResultSet(layer);

  return res;
}

std::auto_ptr<te::dt::Property> te::ogr::DataSource::getProperty(const std::string& datasetName, const std::string& propertyName)
{
  std::string sql = "SELECT FID, * FROM " + datasetName;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);
  int pos =-1;

  if(layer == 0)
    throw(Exception(TR_OGR("Could not retrieve the DataSet from data source.")));

  try
  {
    int pos = layer->GetLayerDefn()->GetFieldIndex(propertyName.c_str());
  
    if(pos < 0)
      throw Exception(TR_OGR("Could not find property!"));
  }
  catch(Exception& e)
  {
    m_ogrDS->ReleaseResultSet(layer);

    throw e;
  }

  m_ogrDS->ReleaseResultSet(layer);

  return getProperty(datasetName, (size_t)pos);
}

std::auto_ptr<te::dt::Property> te::ogr::DataSource::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  std::string sql = "SELECT FID, * FROM " + datasetName;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(Exception(TR_OGR("Could not retrieve the DataSet from data source.")));

  std::auto_ptr<te::da::DataSetType> dt(Convert2TerraLib(layer->GetLayerDefn()));

  te::dt::Property* prp = dt->getProperty(propertyPos);

  m_ogrDS->ReleaseResultSet(layer);

  if(prp == 0)
    throw Exception(TR_OGR("Could not find property!"));

  return std::auto_ptr<te::dt::Property>(prp);
}

te::da::PrimaryKey* te::ogr::DataSource::getPrimaryKey(const std::string& datasetName)
{
  std::string sql = "SELECT FID, * FROM " + datasetName;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(Exception(TR_OGR("Could not retrieve the DataSet from data source.")));

  const char* colIdName = layer->GetFIDColumn();

  if(colIdName == 0 || colIdName[0] == '\0')
    colIdName = "FID";

  int pos = layer->GetLayerDefn()->GetFieldIndex(colIdName);
  
  te::da::PrimaryKey* res;

  if(pos >= 0)
  {
    res = new te::da::PrimaryKey;
    res->add(getProperty(datasetName, pos).get());
  }

  m_ogrDS->ReleaseResultSet(layer);

  return res;
}

std::vector<std::string> te::ogr::DataSource::getUniqueKeyNames(const std::string& datasetName)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

te::da::UniqueKey* te::ogr::DataSource::getUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

std::vector<std::string> te::ogr::DataSource::getForeignKeyNames(const std::string& datasetName)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

te::da::ForeignKey* te::ogr::DataSource::getForeignKey(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

std::vector<std::string> te::ogr::DataSource::getIndexNames(const std::string& datasetName)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

te::da::Index* te::ogr::DataSource::getIndex(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

std::vector<std::string> te::ogr::DataSource::getCheckConstraintNames(const std::string& datasetName)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

te::da::CheckConstraint* te::ogr::DataSource::getCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

std::vector<std::string> te::ogr::DataSource::getSequenceNames()
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

std::auto_ptr<te::da::Sequence> te::ogr::DataSource::getSequence(const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

std::auto_ptr<te::gm::Envelope> te::ogr::DataSource::getExtent(const std::string& /*datasetName*/, const std::string& propertyName)
{
  std::string sql = "SELECT FID, * FROM " + propertyName;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(Exception(TR_OGR("Could not retrieve the DataSet from data source.")));

  OGREnvelope* envOGR = new OGREnvelope();

  if(!(layer->GetExtent(envOGR) == OGRERR_NONE))
  {
    delete envOGR;

    m_ogrDS->ReleaseResultSet(layer);

    throw(Exception(TR_OGR("Error when attempting get extent.")));
  }

  te::gm::Envelope* envTL = Convert2TerraLib(envOGR);
  delete envOGR;

  m_ogrDS->ReleaseResultSet(layer);

  return std::auto_ptr<te::gm::Envelope>(envTL);
}

std::auto_ptr<te::gm::Envelope> te::ogr::DataSource::getExtent(const std::string& datasetName, std::size_t propertyPos)
{
  //descobrir o nome da coluna e chamar o metodo acima.
  std::string propName;

  return getExtent(datasetName, propName);
}

std::size_t te::ogr::DataSource::getNumberOfItems(const std::string& datasetName)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

bool te::ogr::DataSource::hasDataSets()
{
  return (m_ogrDS->GetLayerCount() > 0); 
}

bool te::ogr::DataSource::datasetExists(const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

bool te::ogr::DataSource::primarykeyExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

bool te::ogr::DataSource::uniquekeyExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

bool te::ogr::DataSource::foreignkeyExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

bool te::ogr::DataSource::checkConstraintExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

bool te::ogr::DataSource::indexExists(const std::string& datasetName, const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

bool te::ogr::DataSource::sequenceExists(const std::string& name)
{
  throw Exception(TR_OGR("Not implemented for OGR!"));
}

void te::ogr::DataSource::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  OGRwkbGeometryType geomType = wkbUnknown;

  if(dt->hasGeom())
    geomType = Convert2OGR(te::da::GetFirstGeomProperty(dt)->getGeometryType());

  OGRLayer* newLayer = m_ogrDS->CreateLayer(dt->getName().c_str(), 0, geomType);

  if(newLayer == 0)
    throw(te::common::Exception(TR_OGR("Error when attempting create the dataset type.")));

// add the feature-type to the catalog if needed
  //unsigned int id = m_t->getOGRDataSource()->GetLayerCount() - 1;
  //if(dt->getCatalog())
  //{
    //dt->getCatalog()->setId(dt, id);
  //}
  //else
  //{
    //dt->setId(id);
    //m_t->getDataSource()->getCatalog()->add(dt);
  //}

// add the properties
  for(size_t i = 0; i < dt->size(); ++i)
    addProperty(dt->getName(), dt->getProperty(i));
}

void te::ogr::DataSource::cloneDataSet(const std::string& name,
                  const std::string& cloneName,
                  const std::map<std::string, std::string>& options)
{
  throw Exception(TR_OGR("Not implemented yet!"));
}

void te::ogr::DataSource::dropDataSet(const std::string& name)
{
  throw Exception(TR_OGR("Not implemented yet!"));
}

void te::ogr::DataSource::renameDataSet(const std::string& name, const std::string& newName)
{
}

void te::ogr::DataSource::addProperty(const std::string& datasetName, const te::dt::Property* p)
{
  OGRLayer* layer = m_ogrDS->GetLayerByName(datasetName.c_str());

  if(layer == 0)
    throw(te::common::Exception(TR_OGR("DataSet not found.")));

  if(p->getType() != te::dt::GEOMETRY_TYPE)
  {
    //OGRErr error = layer->CreateField(Convert2OGR(p));

    //if(error != OGRERR_NONE)
    //  throw(Exception(TR_OGR("Error when attempting add the property type.")));
  }
//  throw(te::common::Exception(TR_OGR("OGR Driver not support adding geometry type.")));
}

void te::ogr::DataSource::dropProperty(const std::string& datasetName, const std::string& propertyName)
{
}

void te::ogr::DataSource::renameProperty(const std::string& datasetName,
                    const std::string& propertyName,
                    const std::string& newPropertyName)
{
}

void te::ogr::DataSource::addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk)
{
}

void te::ogr::DataSource::dropPrimaryKey(const std::string& datasetName)
{
}

void te::ogr::DataSource::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
{
}

void te::ogr::DataSource::dropUniqueKey(const std::string& datasetName, const std::string& uniqueKeyName)
{
}

void te::ogr::DataSource::addIndex(const std::string& datasetName, te::da::Index* idx,
                                   const std::map<std::string, std::string>& options)
{
}              

void te::ogr::DataSource::dropIndex(const std::string& datasetName, const std::string& idxName)
{
}

void te::ogr::DataSource::addForeignKey(const std::string& datasetName, const te::da::ForeignKey* fk)
{
}

void te::ogr::DataSource::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
}

void te::ogr::DataSource::addCheckConstraint(const std::string& datasetName, const te::da::CheckConstraint* cc)
{
}

void te::ogr::DataSource::dropCheckConstraint(const std::string& datasetName, const std::string& name)
{
}

void te::ogr::DataSource::createSequence(const te::da::Sequence* sequence)
{
}

void te::ogr::DataSource::dropSequence(const std::string& name)
{
}

void te::ogr::DataSource::add(const std::string& datasetName, te::da::DataSet* d,
        const std::map<std::string, std::string>& options,
        std::size_t limit)
{
  if(limit == 0)
    limit = std::string::npos;

  std::string sql = "SELECT FID, * FROM " + datasetName;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(Exception(TR_OGR("Could not retrieve the DataSet from data source.")));

  try
  {
    begin();

    std::size_t nproperties = d->getNumProperties();

    std::size_t nProcessedRows = 0;

    do
    {
      OGRFeature* feat = OGRFeature::CreateFeature(layer->GetLayerDefn());

      std::size_t currfield = 0;

      for(std::size_t i = 0; i != nproperties; ++i)
      {
        if(d->isNull(i))
        {
          if(d->getPropertyDataType(i) != te::dt::GEOMETRY_TYPE)
            ++currfield;

          continue;
        }

        switch(d->getPropertyDataType(i))
        {
          case te::dt::INT32_TYPE:
            feat->SetField(currfield, d->getInt32(i));
            ++currfield;
          break;

          case te::dt::STRING_TYPE:
            feat->SetField(currfield, d->getAsString(i).c_str());
            ++currfield;
          break;

          case te::dt::DOUBLE_TYPE:
            feat->SetField(currfield, d->getDouble(i));
            ++currfield;
           break;

          case te::dt::NUMERIC_TYPE:
            feat->SetField(currfield, atof(d->getNumeric(i).c_str()));
            ++currfield;
          break;

          case te::dt::BYTE_ARRAY_TYPE:
            {
              std::auto_ptr<te::dt::ByteArray> ba(d->getByteArray(i));
              feat->SetField(currfield, ba->bytesUsed(), reinterpret_cast<unsigned char*>(ba->getData()));
              ++currfield;
            }
          break;

          case te::dt::DATETIME_TYPE:
            {
              std::auto_ptr<te::dt::DateTime> dtm(d->getDateTime(i));

              te::dt::Date* dtime = dynamic_cast<te::dt::Date*>(dtm.get());

              if(dtime)
              {
                feat->SetField(currfield,
                               static_cast<int>(dtime->getYear()),
                               static_cast<int>(dtime->getMonth()),
                               static_cast<int>(dtime->getDay()));
                ++currfield;
                break;
              }

              te::dt::TimeDuration* tduration = dynamic_cast<te::dt::TimeDuration*>(dtm.get());

              if(tduration)
              {
                feat->SetField(currfield, 0, 0, 0,
                               static_cast<int>(tduration->getHours()),
                               static_cast<int>(tduration->getMinutes()),
                               static_cast<int>(tduration->getSeconds()));
                ++currfield;
                break;
              }

              te::dt::TimeInstant* tinst = dynamic_cast<te::dt::TimeInstant*>(dtm.get());

              if(tinst)
              {
                feat->SetField(currfield,
                               static_cast<int>(dtime->getYear()),
                               static_cast<int>(dtime->getMonth()),
                               static_cast<int>(dtime->getDay()),
                               static_cast<int>(tduration->getHours()),
                               static_cast<int>(tduration->getMinutes()),
                               static_cast<int>(tduration->getSeconds()));
                ++currfield;
                break;
              }

              throw(Exception(TR_OGR("Unsupported date and time type by OGR.")));
            }
          break;

          case te::dt::GEOMETRY_TYPE:
            {
              std::auto_ptr<te::gm::Geometry> geom(d->getGeometry(i));
              OGRGeometry* OGRgeom = Convert2OGR(geom.get());
              feat->SetGeometryDirectly(OGRgeom);
            }
          break;

          default:
            throw(Exception(TR_OGR("Unsupported data type by OGR.")));
        }
      }

      if(layer->CreateFeature(feat) != OGRERR_NONE)
      {
        OGRFeature::DestroyFeature(feat);
        throw Exception(TR_OGR("Fail to insert dataset item."));
      }

      OGRFeature::DestroyFeature(feat);

    }while(d->moveNext() && (nProcessedRows != limit));

    commit();
  }
  catch(Exception& e)
  {
    m_ogrDS->ReleaseResultSet(layer);

    throw e;
  }

  m_ogrDS->ReleaseResultSet(layer);
}


void te::ogr::DataSource::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids = 0*/)
{
  throw Exception(TR_OGR("OGR driver: not implemented yet."));
}

void te::ogr::DataSource::update(const std::string& /*datasetName*/,
                    te::da::DataSet* /*dataset*/,
                    const std::vector<std::size_t>& /*properties*/,
                    const te::da::ObjectIdSet* /*oids*/,
                    const std::map<std::string, std::string>& /*options*/,
                    std::size_t /*limit = 0*/)
{
  throw Exception(TR_OGR("OGR driver does not support this method."));
}

void te::ogr::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  std::string path = dsInfo.begin()->second;
 
  if (boost::filesystem::is_directory(path.c_str()))
  {  
    try
    {
      boost::filesystem::create_directory(path);
    }
    catch (boost::filesystem::filesystem_error &e)
    { 
      std::string m = TR_OGR("Could not create directory. ");
                  m += e.what();
      throw Exception(m);
    }
    setConnectionInfo(dsInfo);
    //open();
  }
  else
  {
    OGRSFDriverRegistrar* driverManager = OGRSFDriverRegistrar::GetRegistrar();
    OGRSFDriver* driver = driverManager->GetDriverByName(GetDriverName(path).c_str());
    if (driver == 0)
      throw(Exception(TR_OGR("Driver not found.")));

    if(!driver->TestCapability(ODrCCreateDataSource))
      throw(Exception(TR_OGR("The Driver does not have create capability.")));

    m_ogrDS = driver->CreateDataSource(path.c_str());
    if(m_ogrDS == 0)
      throw(Exception(TR_OGR("Error when attempting create the data source.")));   

    //OGRDataSource::DestroyDataSource(newDS);
    setConnectionInfo(dsInfo);
  }
}

void te::ogr::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
}

bool te::ogr::DataSource::exists(const std::map<std::string, std::string>& /*dsInfo*/)
{
  throw Exception(TR_OGR("Not implemented yet!"));
}

std::vector<std::string> te::ogr::DataSource::getDataSourceNames(const std::map<std::string, std::string>& info)
{
  throw Exception(TR_OGR("Not implemented yet!"));
}
