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

#include "DataSource.h"
#include "DataSet.h"
#include "SQLVisitor.h"
#include "Transactor.h"
#include "Utils.h"

#include "../common/Translator.h"
#include "../dataaccess2/dataset/ObjectId.h"
#include "../dataaccess2/dataset/ObjectIdSet.h"
#include "../dataaccess2/query/Query.h"
#include "../dataaccess2/query/Select.h"
#include "../dataaccess2/utils/Utils.h"
#include "../datatype/ByteArray.h"
#include "../datatype/Date.h"
#include "../datatype/TimeInstant.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"

// OGR
#include <ogrsf_frmts.h>

//Boost
#include <boost/filesystem.hpp>

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


te::ogr::Transactor::Transactor(DataSource* ds) :
te::da::DataSourceTransactor(),
m_ogrDs(ds),
m_isInTransaction(false)
{
}

te::ogr::Transactor::~Transactor()
{
}
    
te::da::DataSource* te::ogr::Transactor::getDataSource() const
{
  return m_ogrDs;
}
    
void te::ogr::Transactor::begin()
{
  if(m_ogrDs->getOGRDataSource()->TestCapability(OLCTransactions) == FALSE)
    return;

  if(m_ogrDs->getOGRDataSource()->GetLayerCount() == 0)
    return;

  m_ogrDs->getOGRDataSource()->GetLayer(0)->StartTransaction();

  m_isInTransaction = true;
}

void te::ogr::Transactor::commit()
{
  if(m_ogrDs->getOGRDataSource()->TestCapability(OLCTransactions) == FALSE)
    return;

  if(m_ogrDs->getOGRDataSource()->GetLayerCount() == 0)
    return;

  m_ogrDs->getOGRDataSource()->GetLayer(0)->CommitTransaction();

  m_isInTransaction = false;
}

void te::ogr::Transactor::rollBack()
{
  if(m_ogrDs->getOGRDataSource()->TestCapability(OLCTransactions) == FALSE)
    return;

  if(m_ogrDs->getOGRDataSource()->GetLayerCount() == 0)
    return;

  m_ogrDs->getOGRDataSource()->GetLayer(0)->RollbackTransaction();

  m_isInTransaction = false;
}

bool te::ogr::Transactor::isInTransaction() const
{
  return m_isInTransaction;
}

std::auto_ptr<te::da::DataSet> te::ogr::Transactor::getDataSet(const std::string& name, 
                                          te::common::TraverseType /*travType*/, 
                                          bool /*connected*/)
{
  std::string sql = "SELECT FID, * FROM " + name;
  OGRLayer* layer = m_ogrDs->getOGRDataSource()->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TR_OGR("The informed data set could not be found in the data source."));

  return std::auto_ptr<te::da::DataSet>(new DataSet(m_ogrDs->getOGRDataSource(), layer, true));
}

std::auto_ptr<te::da::DataSet> te::ogr::Transactor::getDataSet(const std::string& name,
                                          const std::string& /*propertyName*/,
                                          const te::gm::Envelope* e,
                                          te::gm::SpatialRelation /*r*/,
                                          te::common::TraverseType /*travType*/, 
                                          bool /*connected*/)
{
  std::string sql = "SELECT FID, * FROM " + name;
  OGRLayer* layer = m_ogrDs->getOGRDataSource()->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TR_OGR("The informed data set could not be found in the data source."));

  layer->SetSpatialFilterRect(e->m_llx, e->m_lly, e->m_urx, e->m_ury);
  
  return std::auto_ptr<te::da::DataSet>(new DataSet(m_ogrDs->getOGRDataSource(), layer, true));
}

std::auto_ptr<te::da::DataSet> te::ogr::Transactor::getDataSet(const std::string& name,
                                          const std::string& /*propertyName*/,
                                          const te::gm::Geometry* g,
                                          te::gm::SpatialRelation /*r*/,
                                          te::common::TraverseType /*travType*/, 
                                          bool /*connected*/)
{
  std::string sql = "SELECT FID, * FROM " + name;
  OGRLayer* layer = m_ogrDs->getOGRDataSource()->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TR_OGR("The informed data set could not be found in the data source."));
  
  OGRGeometry* ogrg = Convert2OGR(g);

  layer->SetSpatialFilter(ogrg);
  
  OGRGeometryFactory::destroyGeometry(ogrg);
  
  return std::auto_ptr<te::da::DataSet>(new DataSet(m_ogrDs->getOGRDataSource(), layer, true));
}

std::auto_ptr<te::da::DataSet> te::ogr::Transactor::query(const te::da::Select& q,
                                      te::common::TraverseType /*travType*/, 
                                      bool /*connected*/)
{
  std::string sql;

  SQLVisitor visitor(*m_ogrDs->getDialect(), sql);
  q.accept(visitor);

  sql = RemoveSpatialSql(sql);

  OGRLayer* layer = m_ogrDs->getOGRDataSource()->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TR_OGR("Could not retrieve the DataSet from data source."));

  std::auto_ptr<te::gm::Envelope> e(visitor.getMBR());

  if(e.get() != 0)
    layer->SetSpatialFilterRect(e->m_llx, e->m_lly, e->m_urx, e->m_ury);

  return std::auto_ptr<te::da::DataSet>(new DataSet(m_ogrDs->getOGRDataSource(), layer, true));
}

std::auto_ptr<te::da::DataSet> te::ogr::Transactor::query(const std::string& query,
                                      te::common::TraverseType /*travType*/, 
                                      bool /*connected*/)
{
  OGRLayer* layer = m_ogrDs->getOGRDataSource()->ExecuteSQL(query.c_str(), 0, "");

  if(layer == 0)
    throw Exception(TR_OGR("Could not retrieve the DataSet from data source."));

  return std::auto_ptr<te::da::DataSet>(new DataSet(m_ogrDs->getOGRDataSource(), layer, true));
}

void te::ogr::Transactor::execute(const te::da::Query& command)
{
  std::string sql;
  SQLVisitor v(*m_ogrDs->getDialect(), sql);

  command.accept(v);

  execute(sql);
}

void te::ogr::Transactor::execute(const std::string& command)
{
  OGRLayer* layer = m_ogrDs->getOGRDataSource()->ExecuteSQL(command.c_str(), 0, "");

  if(layer == 0)
    throw Exception(TR_OGR("Could not retrieve the DataSet from data source."));

  m_ogrDs->getOGRDataSource()->ReleaseResultSet(layer);
}

std::auto_ptr<te::da::PreparedQuery> te::ogr::Transactor::getPrepared(const std::string& /*qName*/)
{
  return std::auto_ptr<te::da::PreparedQuery>(0);
}

std::auto_ptr<te::da::BatchExecutor> te::ogr::Transactor::getBatchExecutor()
{
  return std::auto_ptr<te::da::BatchExecutor>(0);
}

void te::ogr::Transactor::cancel()
{
}

boost::int64_t te::ogr::Transactor::getLastGeneratedId()
{
  return 0;
}

std::string te::ogr::Transactor::escape(const std::string& value)
{
  return value;
}

bool te::ogr::Transactor::isDataSetNameValid(const std::string& /*datasetName*/)
{
  return true;
}

bool te::ogr::Transactor::isPropertyNameValid(const std::string& /*propertyName*/)
{
  return true;
}

std::vector<std::string> te::ogr::Transactor::getDataSetNames()
{
  std::vector<std::string> names;
  OGRDataSource* src = m_ogrDs->getOGRDataSource();

  int count = src->GetLayerCount();

  for(int i=0; i<count; i++)
    names.push_back(src->GetLayer(i)->GetName());

  return names;
}

std::size_t te::ogr::Transactor::getNumberOfDataSets()
{
  return m_ogrDs->getOGRDataSource()->GetLayerCount();
}
    
te::da::DataSetTypePtr te::ogr::Transactor::getDataSetType(const std::string& name)
{
  OGRLayer* l = m_ogrDs->getOGRDataSource()->GetLayerByName(name.c_str());

  if(l==0)
    return te::da::DataSetTypePtr();

  return te::da::DataSetTypePtr(Convert2TerraLib(l->GetLayerDefn()));
}

boost::ptr_vector<te::dt::Property> te::ogr::Transactor::getProperties(const std::string& datasetName)
{
  boost::ptr_vector<te::dt::Property> ps;

  OGRLayer* l = m_ogrDs->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(l!=0)
  {
    std::auto_ptr<te::da::DataSetType> dt(Convert2TerraLib(l->GetLayerDefn()));
    std::vector<te::dt::Property*> props = dt->getProperties();
    std::vector<te::dt::Property*>::iterator it;

    for(it=props.begin(); it!=props.end(); ++it)
      ps.push_back(*it);
  }

  return ps;
}

std::auto_ptr<te::dt::Property> te::ogr::Transactor::getProperty(const std::string& datasetName, const std::string& name)
{
  int idx = -1;

  OGRLayer* l = m_ogrDs->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(l != 0)
    idx = l->GetLayerDefn()->GetFieldIndex(name.c_str());

  return getProperty(datasetName, idx);
}

std::auto_ptr<te::dt::Property> te::ogr::Transactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  std::auto_ptr<te::dt::Property> res;

  OGRLayer* l = m_ogrDs->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(l != 0)
  {
    OGRFeatureDefn* def = l->GetLayerDefn();
    OGRFieldDefn* fdef = def->GetFieldDefn(propertyPos);

    if(fdef != 0)
      res.reset(Convert2TerraLib(fdef));
  }

  return res;
}

std::vector<std::string> te::ogr::Transactor::getPropertyNames(const std::string& datasetName)
{
  std::vector<std::string> res;

  OGRLayer* l = m_ogrDs->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(l != 0)
  {
    OGRFeatureDefn* def = l->GetLayerDefn();

    for(int i=0; i<def->GetFieldCount(); i++)
      res.push_back(def->GetFieldDefn(i)->GetNameRef());
  }

  return res;
}

std::size_t te::ogr::Transactor::getNumberOfProperties(const std::string& datasetName)
{
  OGRLayer* l = m_ogrDs->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(l != 0)
    return l->GetLayerDefn()->GetFieldCount();

  return 0;
}

bool te::ogr::Transactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  OGRLayer* l = m_ogrDs->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  return ((l!=0) && (l->GetLayerDefn()->GetFieldIndex(name.c_str()) != -1));
}

void te::ogr::Transactor::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  OGRLayer* l = m_ogrDs->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(l != 0)
  {
    if(p->getType() != te::dt::GEOMETRY_TYPE)
    {
      if(!l->TestCapability(OLCCreateField))
        throw Exception(TR_OGR("This dataset do not support add fields operation."));

      OGRFieldDefn* nField = Convert2OGR(p);
      OGRErr error = l->CreateField(nField);

      delete nField;

      if(error != OGRERR_NONE)
        throw Exception(TR_OGR("Error when attempting add the property."));
    }
  }
}

void te::ogr::Transactor::dropProperty(const std::string& /*datasetName*/, const std::string& /*name*/)
{
}

void te::ogr::Transactor::renameProperty(const std::string& datasetName,
                            const std::string& propertyName,
                            const std::string& newPropertyName)
{
  OGRLayer* l = m_ogrDs->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(l != 0)
  {
    int idx = l->GetLayerDefn()->GetFieldIndex(propertyName.c_str());

    if(idx == -1)
      throw Exception(TR_OGR("Field to be renamed does not exists."));

    OGRFieldDefn* df = l->GetLayerDefn()->GetFieldDefn(idx);

    df->SetName(newPropertyName.c_str());
  }
}

std::auto_ptr<te::da::PrimaryKey> te::ogr::Transactor::getPrimaryKey(const std::string& datasetName)
{
  std::auto_ptr<te::da::PrimaryKey> res;

  OGRLayer* layer = m_ogrDs->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(layer != 0)
  {
    const char* colIdName = layer->GetFIDColumn();

    if(colIdName == 0 || colIdName[0] == '\0')
      colIdName = "FID";

    int pos = layer->GetLayerDefn()->GetFieldIndex(colIdName);
  
    if(pos >= 0)
    {
      res.reset(new te::da::PrimaryKey);
      res->add(getProperty(datasetName, pos).get());
    }
  }

  return res;
}

bool te::ogr::Transactor::primaryKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::ogr::Transactor::addPrimaryKey(const std::string& /*datasetName*/, te::da::PrimaryKey* /*pk*/)
{
}

void te::ogr::Transactor::dropPrimaryKey(const std::string& /*datasetName*/)
{
}

std::auto_ptr<te::da::ForeignKey> te::ogr::Transactor::getForeignKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::ForeignKey>();
}

std::vector<std::string> te::ogr::Transactor::getForeignKeyNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::ogr::Transactor::foreignKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::ogr::Transactor::addForeignKey(const std::string& /*datasetName*/, te::da::ForeignKey* /*fk*/)
{
}

void te::ogr::Transactor::dropForeignKey(const std::string& /*datasetName*/, const std::string& /*fkName*/)
{
}

std::auto_ptr<te::da::UniqueKey> te::ogr::Transactor::getUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::UniqueKey>();
}

std::vector<std::string> te::ogr::Transactor::getUniqueKeyNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::ogr::Transactor::uniqueKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::ogr::Transactor::addUniqueKey(const std::string& /*datasetName*/, te::da::UniqueKey* /*uk*/)
{
}

void te::ogr::Transactor::dropUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
}

std::auto_ptr<te::da::CheckConstraint> te::ogr::Transactor::getCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::CheckConstraint>();
}

std::vector<std::string> te::ogr::Transactor::getCheckConstraintNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::ogr::Transactor::checkConstraintExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::ogr::Transactor::addCheckConstraint(const std::string& /*datasetName*/, te::da::CheckConstraint* /*cc*/)
{
}

void te::ogr::Transactor::dropCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
}

std::auto_ptr<te::da::Index> te::ogr::Transactor::getIndex(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return std::auto_ptr<te::da::Index>();
}

std::vector<std::string> te::ogr::Transactor::getIndexNames(const std::string& /*datasetName*/)
{
  return std::vector<std::string>();
}

bool te::ogr::Transactor::indexExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  return false;
}

void te::ogr::Transactor::addIndex(const std::string& /*datasetName*/, te::da::Index* /*idx*/,
                      const std::map<std::string, std::string>& /*options*/)
{
}

void te::ogr::Transactor::dropIndex(const std::string& /*datasetName*/, const std::string& /*idxName*/)
{
}

std::auto_ptr<te::da::Sequence> te::ogr::Transactor::getSequence(const std::string& /*name*/)
{
  return std::auto_ptr<te::da::Sequence>();
}

std::vector<std::string> te::ogr::Transactor::getSequenceNames()
{
  return std::vector<std::string>();
}

bool te::ogr::Transactor::sequenceExists(const std::string& /*name*/)
{
  return false;
}

void te::ogr::Transactor::addSequence(te::da::Sequence* /*sequence*/)
{
}

void te::ogr::Transactor::dropSequence(const std::string& /*name*/)
{
}

std::auto_ptr<te::gm::Envelope> te::ogr::Transactor::getExtent(const std::string& datasetName,
                                                  const std::string& propertyName)
{
  std::auto_ptr<te::gm::Envelope> res;
  std::string sql("SELECT ");
  sql += propertyName + " FROM ";
  sql += datasetName;

  OGRLayer* l = m_ogrDs->getOGRDataSource()->ExecuteSQL(sql.c_str(), 0, 0);

  if(l != 0)
  {
    std::auto_ptr<OGREnvelope> env(new OGREnvelope);

    if(l->GetExtent(env.get()) != OGRERR_NONE)
    {
      m_ogrDs->getOGRDataSource()->ReleaseResultSet(l);
      throw Exception(TR_OGR("Error when attempting get extent."));
    }

    res.reset(Convert2TerraLib(env.get()));

    m_ogrDs->getOGRDataSource()->ReleaseResultSet(l);
  }

  return res;
}

std::auto_ptr<te::gm::Envelope> te::ogr::Transactor::getExtent(const std::string& datasetName,
                                                  std::size_t /*propertyPos*/)
{
  return getExtent(datasetName, "OGR_GEOMETRY");
}

std::size_t te::ogr::Transactor::getNumberOfItems(const std::string& datasetName)
{
  OGRLayer* l = m_ogrDs->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(l != 0)
    return l->GetFeatureCount();

  return 0;
}

bool te::ogr::Transactor::hasDataSets()
{
  return (m_ogrDs->getOGRDataSource()->GetLayerCount() > 0);
}

bool te::ogr::Transactor::dataSetExists(const std::string& name)
{
  return (m_ogrDs->getOGRDataSource()->GetLayerByName(name.c_str()) != 0);
}

void te::ogr::Transactor::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& /*options*/)
{
  if(!m_ogrDs->getOGRDataSource()->TestCapability(ODsCCreateLayer))
    throw Exception(TR_OGR("This driver does not support dataset creation."));

  OGRwkbGeometryType geomType = wkbUnknown;

  if(dt->hasGeom())
    geomType = Convert2OGR(te::da::GetFirstGeomProperty(dt)->getGeometryType());

  OGRLayer* newLayer = m_ogrDs->getOGRDataSource()->CreateLayer(dt->getName().c_str(), 0, geomType);

  if(newLayer == 0)
    throw Exception(TR_OGR("Error when attempting create the dataset type."));

// add the properties
  for(size_t i = 0; i < dt->size(); ++i)
    addProperty(dt->getName(), dt->getProperty(i));
}

void te::ogr::Transactor::cloneDataSet(const std::string& name,
                          const std::string& cloneName,
                          const std::map<std::string, std::string>& /*options*/)
{
  OGRDataSource* dsrc = m_ogrDs->getOGRDataSource();

  if(!dsrc->TestCapability(ODsCCreateLayer))
    throw Exception(TR_OGR("This driver does not support creates a dataset."));

  OGRLayer* l = dsrc->GetLayerByName(name.c_str());

  if(l == 0)
    throw Exception(TR_OGR("Could not retrieve the DataSet from data source."));

  OGRLayer* cl = dsrc->CopyLayer(l, cloneName.c_str());

  if(cl == 0)
    throw Exception(TR_OGR("Error when attempting clone the dataset."));
}

void te::ogr::Transactor::dropDataSet(const std::string& name)
{
  OGRDataSource* dsrc = m_ogrDs->getOGRDataSource();

  if(!dsrc->TestCapability(ODsCDeleteLayer))
    throw Exception(TR_OGR("This driver does not support remove a dataset."));

  int i=0;

  for(; i<dsrc->GetLayerCount(); i++)
    if(name.compare(dsrc->GetLayer(i)->GetName()) == 0)
      break;

  if(i == dsrc->GetLayerCount())
    throw Exception(TR_OGR("Could not retrieve the DataSet from data source."));

  if(dsrc->DeleteLayer(i) != OGRERR_NONE)
    throw Exception(TR_OGR("Error when attempting to remove the dataset."));
}

void te::ogr::Transactor::renameDataSet(const std::string& /*name*/, const std::string& /*newName*/)
{
}

void te::ogr::Transactor::add(const std::string& datasetName,
                  te::da::DataSet* d,
                  const std::map<std::string, std::string>& options,
                  std::size_t limit)
{
  if(limit == 0)
    limit = std::string::npos;

  OGRLayer* layer = m_ogrDs->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(layer == 0)
    throw Exception(TR_OGR("Could not retrieve the DataSet from data source."));

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

              throw Exception (TR_OGR("Unsupported date and time type by OGR."));
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
            throw Exception(TR_OGR("Unsupported data type by OGR."));
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
    rollBack();
    throw e;
  }
}

void te::ogr::Transactor::remove(const std::string& datasetName, const te::da::ObjectIdSet* oids)
{
  OGRDataSource* dsrc = m_ogrDs->getOGRDataSource();

  OGRLayer* l = dsrc->GetLayerByName(datasetName.c_str());

  if(l == 0)
    throw Exception(TR_OGR("Could not retrieve the DataSet from data source."));

  if(!l->TestCapability(OLCDeleteFeature))
    throw Exception(TR_OGR("Driver does not support remotion of features."));

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it = oids->begin();

  while(it != oids->end())
  {
    begin();
    
    if(l->DeleteFeature(atoi((*it)->getValue()[0].toString().c_str())) != OGRERR_NONE)
    {
      rollBack();
      throw Exception(TR_OGR("Error when attempting to remove the feature."));
    }

    ++it;
  }

  commit();
}

void te::ogr::Transactor::update(const std::string& /*datasetName*/,
                    te::da::DataSet* /*dataset*/,
                    const std::vector<std::size_t>& /*properties*/,
                    const te::da::ObjectIdSet* /*oids*/,
                    const std::map<std::string, std::string>& /*options*/,
                    std::size_t /*limit*/)
{
}

void te::ogr::Transactor::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
}
