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
  \file terralib/wfs/Transactor.cpp

  \brief Implementation of the transactor for the WFS driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Property.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../ogr/DataSet.h"
#include "../ogr/SQLVisitor.h"
#include "../ogr/Utils.h"
#include "DataSource.h"
#include "Exception.h"
#include "Transactor.h"
#include "Utils.h"

// OGR
#include <ogrsf_frmts.h>

// STL
#include <vector>

te::wfs::Transactor::Transactor(DataSource* ds)
  : te::da::DataSourceTransactor(),
    m_ds(ds)
{
}

te::wfs::Transactor::~Transactor()
{
}

te::da::DataSource* te::wfs::Transactor::getDataSource() const
{
  return m_ds;
}

std::auto_ptr<te::da::DataSet> te::wfs::Transactor::getDataSet(const std::string& name,
                                                               te::common::TraverseType /*travType*/,
                                                               bool /*connected*/,
                                                               const te::common::AccessPolicy /*accessPolicy*/)
{
  assert(m_ds->getOGRDataSource());
  
  //OGRDataSource* ds = OGRSFDriverRegistrar::Open(m_ds->getOGRDataSource()->GetName());
  GDALDataset* ds = (GDALDataset*)GDALOpenEx(m_ds->getOGRDataSource()->GetDescription(), GDAL_OF_READONLY, NULL, NULL, NULL);

  std::string sql = "SELECT FID, * FROM \"" + name + "\"";
  OGRLayer* layer = ds->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TE_TR("The informed data set could not be found in the data source!"));

  return std::auto_ptr<te::da::DataSet>(new te::ogr::DataSet(ds, layer));
}

std::auto_ptr<te::da::DataSet> te::wfs::Transactor::getDataSet(const std::string& name,
                                                               const std::string& /*propertyName*/,
                                                               const te::gm::Envelope* e,
                                                               te::gm::SpatialRelation /*r*/,
                                                               te::common::TraverseType /*travType*/,
                                                               bool /*connected*/,
                                                               const te::common::AccessPolicy /*accessPolicy*/)
{
  assert(m_ds->getOGRDataSource());

  //OGRDataSource* ds = OGRSFDriverRegistrar::Open(m_ds->getOGRDataSource()->GetName());
  GDALDataset* ds = (GDALDataset*)GDALOpenEx(m_ds->getOGRDataSource()->GetDescription(), GDAL_OF_READONLY, NULL, NULL, NULL);

  std::string sql = "SELECT FID, * FROM \"" + name + "\"";
  OGRLayer* layer = ds->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TE_TR("The informed data set could not be found in the data source!"));

  layer->SetSpatialFilterRect(e->m_llx, e->m_lly, e->m_urx, e->m_ury);
  
  return std::auto_ptr<te::da::DataSet>(new te::ogr::DataSet(ds, layer));
}

std::auto_ptr<te::da::DataSet> te::wfs::Transactor::getDataSet(const std::string& name,
                                                               const std::string& /*propertyName*/,
                                                               const te::gm::Geometry* g,
                                                               te::gm::SpatialRelation /*r*/,
                                                               te::common::TraverseType /*travType*/,
                                                               bool /*connected*/,
                                                               const te::common::AccessPolicy /*accessPolicy*/)
{
  assert(m_ds->getOGRDataSource());

  //OGRDataSource* ds = OGRSFDriverRegistrar::Open(m_ds->getOGRDataSource()->GetName());
  GDALDataset* ds = (GDALDataset*)GDALOpenEx(m_ds->getOGRDataSource()->GetDescription(), GDAL_OF_READONLY, NULL, NULL, NULL);

  std::string sql = "SELECT FID, * FROM \"" + name + "\"";
  OGRLayer* layer = ds->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TE_TR("The informed data set could not be found in the data source!"));
  
  OGRGeometry* ogrg = te::ogr::Convert2OGR(g);

  layer->SetSpatialFilter(ogrg);
  
  OGRGeometryFactory::destroyGeometry(ogrg);
  
  return std::auto_ptr<te::da::DataSet>(new te::ogr::DataSet(ds, layer));
}

std::auto_ptr<te::da::DataSet> te::wfs::Transactor::query(const te::da::Select& q,
                                                          te::common::TraverseType /*travType*/,
                                                          bool /*connected*/,
                                                          const te::common::AccessPolicy /*accessPolicy*/)
{
  assert(m_ds->getOGRDataSource());

  //OGRDataSource* ds = OGRSFDriverRegistrar::Open(m_ds->getOGRDataSource()->GetName());
  GDALDataset* ds = (GDALDataset*)GDALOpenEx(m_ds->getOGRDataSource()->GetDescription(), GDAL_OF_READONLY, NULL, NULL, NULL);

  std::string sql;

  te::ogr::SQLVisitor visitor(*m_ds->getDialect(), sql);

  q.accept(visitor);

  sql = te::ogr::RemoveSpatialSql(sql);

  OGRLayer* layer = ds->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TE_TR("The informed data set could not be found in the data source!"));

  te::gm::Envelope* e = visitor.getMBR();

  if(e != 0)
    layer->SetSpatialFilterRect(e->m_llx, e->m_lly, e->m_urx, e->m_ury);

  return std::auto_ptr<te::da::DataSet>(new te::ogr::DataSet(ds, layer));
}

std::auto_ptr<te::da::DataSet> te::wfs::Transactor::query(const std::string& query,
                                                          te::common::TraverseType /*travType*/,
                                                          bool /*connected*/,
                                                          const te::common::AccessPolicy /*accessPolicy*/)
{
  assert(m_ds->getOGRDataSource());

  //OGRDataSource* ds = OGRSFDriverRegistrar::Open(m_ds->getOGRDataSource()->GetName());
  GDALDataset* ds = (GDALDataset*)GDALOpenEx(m_ds->getOGRDataSource()->GetDescription(), GDAL_OF_READONLY, NULL, NULL, NULL);

  OGRLayer* layer = ds->ExecuteSQL(query.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TE_TR("The informed data set could not be found in the data source!"));

  return std::auto_ptr<te::da::DataSet>(new te::ogr::DataSet(ds, layer));
}

std::vector<std::string> te::wfs::Transactor::getDataSetNames()
{
  std::vector<std::string> names;

  if(!m_ds->getOGRDataSource())
    return names;

  for(int i = 0; i < m_ds->getOGRDataSource()->GetLayerCount(); ++i)
    names.push_back(m_ds->getOGRDataSource()->GetLayer(i)->GetName());

  return names;
}

std::size_t te::wfs::Transactor::getNumberOfDataSets()
{
  if(!m_ds->getOGRDataSource())
    return 0;

  return m_ds->getOGRDataSource()->GetLayerCount();
}

std::auto_ptr<te::da::DataSetType> te::wfs::Transactor::getDataSetType(const std::string& name)
{
  assert(m_ds->getOGRDataSource());

  std::string sql("SELECT FID, * FROM \"");
  sql += name + "\"";

  OGRLayer* layer = m_ds->getOGRDataSource()->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TE_TR("Could not retrieve the informed data set!"));

  std::auto_ptr<te::da::DataSetType> type(te::ogr::Convert2TerraLib(layer->GetLayerDefn()));
  type->setName(name);

  const char* colIdName = layer->GetFIDColumn();

  if(colIdName == 0 || colIdName[0] == '\0')
    colIdName = "FID";

  int pos = layer->GetLayerDefn()->GetFieldIndex(colIdName);
  if(pos >= 0)
  {
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(colIdName, type.get());
    pk->add(type->getProperty(pos));
  }

  int srs = te::ogr::Convert2TerraLibProjection(layer->GetSpatialRef());

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(type.get());
  
  if(gp != 0)
    gp->setSRID(srs);

  m_ds->getOGRDataSource()->ReleaseResultSet(layer);

  return type;
}

boost::ptr_vector<te::dt::Property> te::wfs::Transactor::getProperties(const std::string& datasetName)
{
  boost::ptr_vector<te::dt::Property> properties;

  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();
  for(std::size_t i = 0; i < props.size(); ++i)
      properties.push_back(props[i]->clone());
  
  return properties;
}

std::auto_ptr<te::dt::Property> te::wfs::Transactor::getProperty(const std::string& datasetName, const std::string& name)
{
  assert(m_ds->getOGRDataSource());

  std::string sql("SELECT FID, * FROM \"");
  sql += datasetName + "\"";

  OGRLayer* layer = m_ds->getOGRDataSource()->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TE_TR("Could not retrieve the informed data set!"));

  int propertyPos = layer->GetLayerDefn()->GetFieldIndex(name.c_str());

  m_ds->getOGRDataSource()->ReleaseResultSet(layer);

  return getProperty(datasetName, static_cast<std::size_t>(propertyPos));
}

std::auto_ptr<te::dt::Property> te::wfs::Transactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  assert(m_ds->getOGRDataSource());

  std::string sql("SELECT FID, * FROM \"");
  sql += datasetName + "\"";

  OGRLayer* layer = m_ds->getOGRDataSource()->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TE_TR("Could not retrieve the informed data set!"));

  OGRFeatureDefn* def = layer->GetLayerDefn();
  OGRFieldDefn* fdef = def->GetFieldDefn(propertyPos);

  std::auto_ptr<te::dt::Property> prop;
  if(fdef != 0)
    prop.reset(te::ogr::Convert2TerraLib(fdef));

  m_ds->getOGRDataSource()->ReleaseResultSet(layer);

  return prop;
}

std::vector<std::string> te::wfs::Transactor::getPropertyNames(const std::string& datasetName)
{
  assert(m_ds->getOGRDataSource());

  std::string sql("SELECT FID, * FROM \"");
  sql += datasetName + "\"";

  OGRLayer* layer = m_ds->getOGRDataSource()->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TE_TR("Could not retrieve the informed data set!"));

  std::vector<std::string> propertyNames;

  OGRFeatureDefn* def = layer->GetLayerDefn();

  for(int i = 0; i < def->GetFieldCount(); ++i)
    propertyNames.push_back(def->GetFieldDefn(i)->GetNameRef());

  m_ds->getOGRDataSource()->ReleaseResultSet(layer);

  return propertyNames;
}

std::size_t te::wfs::Transactor::getNumberOfProperties(const std::string& datasetName)
{
  return getPropertyNames(datasetName).size();
}

bool te::wfs::Transactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  std::vector<std::string> propertyNames = getPropertyNames(datasetName);

  for(std::size_t i = 0; i < propertyNames.size(); ++i)
    if(propertyNames[i] == name)
      return true;

  return false;
}

std::auto_ptr<te::gm::Envelope> te::wfs::Transactor::getExtent(const std::string& datasetName,
                                                               const std::string& propertyName)
{
  assert(m_ds->getOGRDataSource());

  std::string sql("SELECT ");
  sql += propertyName + " FROM \"";
  sql += datasetName + "\"";

  OGRLayer* layer = m_ds->getOGRDataSource()->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw Exception(TE_TR("Could not retrieve the informed data set!"));

  std::auto_ptr<OGREnvelope> ogrEnv(new OGREnvelope);

  if(layer->GetExtent(ogrEnv.get()) != OGRERR_NONE)
  {
    m_ds->getOGRDataSource()->ReleaseResultSet(layer);
    throw Exception(TE_TR("Error when attempting to get the extent!"));
  }

  std::auto_ptr<te::gm::Envelope> teEnv(te::ogr::Convert2TerraLib(ogrEnv.get()));

  m_ds->getOGRDataSource()->ReleaseResultSet(layer);

  return teEnv;
}

std::auto_ptr<te::gm::Envelope> te::wfs::Transactor::getExtent(const std::string& datasetName,
                                                               std::size_t /*propertyPos*/)
{
  return getExtent(datasetName, "OGR_GEOMETRY");
}

std::size_t te::wfs::Transactor::getNumberOfItems(const std::string& datasetName)
{
  assert(m_ds->getOGRDataSource());

  OGRLayer* layer = m_ds->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(layer == 0)
    throw Exception(TE_TR("Could not retrieve the informed data set!"));

  return layer->GetFeatureCount();
}

bool te::wfs::Transactor::hasDataSets()
{
  if(!m_ds->getOGRDataSource())
    return false;

  return (m_ds->getOGRDataSource()->GetLayerCount() > 0);
}

bool te::wfs::Transactor::dataSetExists(const std::string& name)
{
  if(!m_ds->getOGRDataSource())
    return false;

  return (m_ds->getOGRDataSource()->GetLayerByName(name.c_str()) != 0);
}

/** NOT SUPPORTED METHODS */
//@{

void te::wfs::Transactor::begin()
{
  throw Exception(TE_TR("The method begin() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::commit()
{
  throw Exception(TE_TR("The method commit() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::rollBack()
{
  throw Exception(TE_TR("The method rollBack() is not supported by the WFS driver!"));
}

bool te::wfs::Transactor::isInTransaction() const
{
  throw Exception(TE_TR("The method isInTransaction() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::execute(const te::da::Query& /*command*/)
{
  throw Exception(TE_TR("The method execute() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::execute(const std::string& /*command*/)
{
  throw Exception(TE_TR("The method execute() is not supported by the WFS driver!"));
}

std::auto_ptr<te::da::PreparedQuery> te::wfs::Transactor::getPrepared(const std::string& /*qName*/)
{
  throw Exception(TE_TR("The method getPrepared() is not supported by the WFS driver!"));
}

std::auto_ptr<te::da::BatchExecutor> te::wfs::Transactor::getBatchExecutor()
{
  throw Exception(TE_TR("The method getBatchExecutor() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::cancel()
{
  throw Exception(TE_TR("The method cancel() is not supported by the WFS driver!"));
}

boost::int64_t te::wfs::Transactor::getLastGeneratedId()
{
  throw Exception(TE_TR("The method getLastGeneratedId() is not supported by the WFS driver!"));
}

std::string te::wfs::Transactor::escape(const std::string& /*value*/)
{
  throw Exception(TE_TR("The method escape() is not supported by the WFS driver!"));
}

bool te::wfs::Transactor::isDataSetNameValid(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method isDataSetNameValid() is not supported by the WFS driver!"));
}

bool te::wfs::Transactor::isPropertyNameValid(const std::string& /*propertyName*/)
{
  throw Exception(TE_TR("The method isPropertyNameValid() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::addProperty(const std::string& /*datasetName*/, te::dt::Property* /*p*/)
{
  throw Exception(TE_TR("The method addProperty() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::dropProperty(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method dropProperty() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::renameProperty(const std::string& /*datasetName*/, const std::string& /*propertyName*/, const std::string& /*newPropertyName*/)
{
  throw Exception(TE_TR("The method renameProperty() is not supported by the WFS driver!"));
}

std::auto_ptr<te::da::PrimaryKey> te::wfs::Transactor::getPrimaryKey(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method getPrimaryKey() is not supported by the WFS driver!"));
}

bool te::wfs::Transactor::primaryKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method primaryKeyExists() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::addPrimaryKey(const std::string& /*datasetName*/, te::da::PrimaryKey* /*pk*/)
{
  throw Exception(TE_TR("The method addPrimaryKey() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::dropPrimaryKey(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method dropPrimaryKey() is not supported by the WFS driver!"));
}

std::auto_ptr<te::da::ForeignKey> te::wfs::Transactor::getForeignKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method getForeignKey() is not supported by the WFS driver!"));
}

std::vector<std::string> te::wfs::Transactor::getForeignKeyNames(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method getForeignKeyNames() is not supported by the WFS driver!"));
}

bool te::wfs::Transactor::foreignKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method foreignKeyExists() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::addForeignKey(const std::string& /*datasetName*/, te::da::ForeignKey* /*fk*/)
{
  throw Exception(TE_TR("The method addForeignKey() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::dropForeignKey(const std::string& /*datasetName*/, const std::string& /*fkName*/)
{
  throw Exception(TE_TR("The method dropForeignKey() is not supported by the WFS driver!"));
}

std::auto_ptr<te::da::UniqueKey> te::wfs::Transactor::getUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method getUniqueKey() is not supported by the WFS driver!"));
}

std::vector<std::string> te::wfs::Transactor::getUniqueKeyNames(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method getUniqueKeyNames() is not supported by the WFS driver!"));
}

bool te::wfs::Transactor::uniqueKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method uniqueKeyExists() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::addUniqueKey(const std::string& /*datasetName*/, te::da::UniqueKey* /*uk*/)
{
  throw Exception(TE_TR("The method addUniqueKey() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::dropUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method dropUniqueKey() is not supported by the WFS driver!"));
}

std::auto_ptr<te::da::CheckConstraint> te::wfs::Transactor::getCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method getCheckConstraint() is not supported by the WFS driver!"));
}

std::vector<std::string> te::wfs::Transactor::getCheckConstraintNames(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method getCheckConstraintNames() is not supported by the WFS driver!"));
}

bool te::wfs::Transactor::checkConstraintExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method checkConstraintExists() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::addCheckConstraint(const std::string& /*datasetName*/, te::da::CheckConstraint* /*cc*/)
{
  throw Exception(TE_TR("The method addCheckConstraint() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::dropCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method dropCheckConstraint() is not supported by the WFS driver!"));
}

std::auto_ptr<te::da::Index> te::wfs::Transactor::getIndex(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method getIndex() is not supported by the WFS driver!"));
}

std::vector<std::string> te::wfs::Transactor::getIndexNames(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method getIndexNames() is not supported by the WFS driver!"));
}

bool te::wfs::Transactor::indexExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method indexExists() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::addIndex(const std::string& /*datasetName*/, te::da::Index* /*idx*/, const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TE_TR("The method addIndex() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::dropIndex(const std::string& /*datasetName*/, const std::string& /*idxName*/)
{
  throw Exception(TE_TR("The method dropIndex() is not supported by the WFS driver!"));
}

std::auto_ptr<te::da::Sequence> te::wfs::Transactor::getSequence(const std::string& /*name*/)
{
  throw Exception(TE_TR("The method getSequence() is not supported by the WFS driver!"));
}

std::vector<std::string> te::wfs::Transactor::getSequenceNames()
{
  throw Exception(TE_TR("The method getSequenceNames() is not supported by the WFS driver!"));
}

bool te::wfs::Transactor::sequenceExists(const std::string& /*name*/)
{
  throw Exception(TE_TR("The method sequenceExists() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::addSequence(te::da::Sequence* /*sequence*/)
{
  throw Exception(TE_TR("The method addSequence() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::dropSequence(const std::string& /*name*/)
{
  throw Exception(TE_TR("The method dropSequence() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::createDataSet(te::da::DataSetType* /*dt*/,
                                        const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TE_TR("The method createDataSet() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::cloneDataSet(const std::string& /*name*/,
                                       const std::string& /*cloneName*/,
                                       const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TE_TR("The method cloneDataSet() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::dropDataSet(const std::string& /*name*/)
{
  throw Exception(TE_TR("The method dropDataSet() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::renameDataSet(const std::string& /*name*/,
                                        const std::string& /*newName*/)
{
  throw Exception(TE_TR("The method renameDataSet() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::add(const std::string& /*datasetName*/,
                              te::da::DataSet* /*d*/,
                              const std::map<std::string, std::string>& /*options*/,
                              std::size_t /*limit*/)
{
  throw Exception(TE_TR("The method add() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw Exception(TE_TR("The method remove() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::update(const std::string& /*datasetName*/,
                                 te::da::DataSet* /*dataset*/,
                                 const std::vector<std::size_t>& /*properties*/,
                                 const te::da::ObjectIdSet* /*oids*/,
                                 const std::map<std::string, std::string>& /*options*/,
                                 std::size_t /*limit*/)
{
  throw Exception(TE_TR("The method update() is not supported by the WFS driver!"));
}

void te::wfs::Transactor::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TE_TR("The method optimize() is not supported by the WFS driver!"));
}

te::common::CharEncoding te::wfs::Transactor::getEncoding()
{
  return te::common::UTF8;
}

//@}
