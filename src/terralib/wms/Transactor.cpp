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

/*!
  \file terralib/wms/Transactor.cpp

  \brief Implementation of the transactor for the WMS driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../gdal/DataSet.h"
#include "../gdal/Utils.h"
#include "../geometry/Envelope.h"
#include "../raster/Grid.h"
#include "Exception.h"
#include "Transactor.h"
#include "Utils.h"

// GDAL
#include <gdal_priv.h>

// STL
#include <vector>

te::wms::Transactor::Transactor(const std::string& uri, const std::map<std::string, WMSLayerInfo>& layersInfo)
  : m_uri(uri),
    m_layersInfo(layersInfo)
{
}

te::wms::Transactor::~Transactor()
{
}

te::da::DataSource* te::wms::Transactor::getDataSource() const
{
  return 0;
}

std::auto_ptr<te::da::DataSet> te::wms::Transactor::getDataSet(const std::string& name,
                                                               te::common::TraverseType /*travType*/,
                                                               bool /*connected*/,
                                                               const te::common::AccessPolicy /*accessPolicy*/)
{
  std::string layer, width, height, format;
  ExtractRequestValues(name, layer, width, height, format);

  if(layer.empty())
    throw Exception(TR_WMS("Invalid data set request. Missing the parameter 'LAYER'."));

  std::map<std::string, WMSLayerInfo>::const_iterator it = m_layersInfo.find(layer);
  if(it == m_layersInfo.end())
    throw Exception(TR_WMS("The informed data set could not be found in the data source."));

  // Retrieves the data set type
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(layer);

  // Build the GetMap request
  std::string request = BuildGetMapRequest(m_uri, it->second, width, height, format);

  return std::auto_ptr<te::da::DataSet>(new te::gdal::DataSet(type, te::common::RAccess, request));
}

std::auto_ptr<te::da::DataSet> te::wms::Transactor::getDataSet(const std::string& name,
                                                               const std::string& /*propertyName*/,
                                                               const te::gm::Envelope* e,
                                                               te::gm::SpatialRelation /*r*/,
                                                               te::common::TraverseType /*travType*/,
                                                               bool /*connected*/,
                                                               const te::common::AccessPolicy /*accessPolicy*/)
{
  std::string layer, width, height, format;
  ExtractRequestValues(name, layer, width, height, format);

  if(layer.empty())
    throw Exception(TR_WMS("Invalid data set request. Missing the parameter 'LAYER'."));

  std::map<std::string, WMSLayerInfo>::const_iterator it = m_layersInfo.find(layer);
  if(it == m_layersInfo.end())
    throw Exception(TR_WMS("The informed data set could not be found in the data source."));

  // Retrieves the data set type
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(layer);

  // Build the GetMap request
  std::string request = BuildGetMapRequest(m_uri, it->second, width, height, format, GetBBOX(*e));

  return std::auto_ptr<te::da::DataSet>(new te::gdal::DataSet(type, te::common::RAccess, request));
}

std::auto_ptr<te::da::DataSet> te::wms::Transactor::getDataSet(const std::string& name,
                                                               const std::string& propertyName,
                                                               const te::gm::Geometry* g,
                                                               te::gm::SpatialRelation r,
                                                               te::common::TraverseType travType,
                                                               bool connected,
                                                               const te::common::AccessPolicy accessPolicy)
{
  return getDataSet(name, propertyName, g->getMBR(), r, travType, connected, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::wms::Transactor::getDataSet(const std::string& name,
                                                               const te::da::ObjectIdSet* oids,
                                                               te::common::TraverseType travType,
                                                               bool connected,
                                                               const te::common::AccessPolicy accessPolicy)
{
  throw Exception(TR_WMS("The ObjectIdSet concept is not supported by the WMS driver."));
}

std::auto_ptr<te::da::DataSet> te::wms::Transactor::query(const te::da::Select& q,
                                                          te::common::TraverseType travType,
                                                          bool connected,
                                                          const te::common::AccessPolicy accessPolicy)
{
  // TODO: Review to considet GetFeatureInfo request?
  throw Exception(TR_WMS("Query operations is not supported by the WMS driver."));
}

std::auto_ptr<te::da::DataSet> te::wms::Transactor::query(const std::string& query,
                                                          te::common::TraverseType travType,
                                                          bool connected,
                                                          const te::common::AccessPolicy accessPolicy)
{
  // TODO: Review to considet GetFeatureInfo request?
  throw Exception(TR_WMS("Query operations is not supported by the WMS driver."));
}

std::vector<std::string> te::wms::Transactor::getDataSetNames()
{
  std::vector<std::string> names;

  std::map<std::string, WMSLayerInfo>::const_iterator it;
  for(it = m_layersInfo.begin(); it != m_layersInfo.end(); ++it)
    names.push_back(it->second.m_name);

  return names;
}

std::size_t te::wms::Transactor::getNumberOfDataSets()
{
  return m_layersInfo.size();
}

std::auto_ptr<te::da::DataSetType> te::wms::Transactor::getDataSetType(const std::string& name)
{
  std::map<std::string, WMSLayerInfo>::const_iterator it = m_layersInfo.find(name);
  if(it == m_layersInfo.end())
    throw Exception(TR_WMS("The informed data set could not be found in the data source."));

  const WMSLayerInfo& info = it->second;

  GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(info.m_pseudoGetMapRequest.c_str(), GA_ReadOnly));
  if(gds == 0)
    throw Exception(TR_WMS("The data set type could not be retrieved from data source."));
  
  te::da::DataSetType* type = new te::da::DataSetType(info.m_name, 0);
  type->setTitle(info.m_title);
  
  te::rst::Grid* grid = te::gdal::GetGrid(gds);

  std::vector<te::rst::BandProperty*> bandProperties;
  te::gdal::GetBandProperties(gds, bandProperties);

  te::rst::RasterProperty* rp = new te::rst::RasterProperty("raster");
  rp->set(grid);

  for(std::size_t i = 0; i < bandProperties.size(); ++i)
    rp->add(bandProperties[i]);

  type->add(rp);

  GDALClose(gds);

  return std::auto_ptr<te::da::DataSetType>(type);
}

boost::ptr_vector<te::dt::Property> te::wms::Transactor::getProperties(const std::string& datasetName)
{
  boost::ptr_vector<te::dt::Property> properties;

  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();
  for(std::size_t i = 0; i < props.size(); ++i)
      properties.push_back(props[i]->clone());
  
  return properties;
}

std::auto_ptr<te::dt::Property> te::wms::Transactor::getProperty(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();
  for(std::size_t i = 0; i < props.size(); ++i)
  {
    if(props[i]->getName() == name)
      return std::auto_ptr<te::dt::Property>(props[i]->clone()); 
  }

  throw Exception(TR_WMS("The informed property name could not be found in the data set."));
}

std::auto_ptr<te::dt::Property> te::wms::Transactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();

  if(propertyPos >= props.size())
    throw Exception(TR_WMS("The informed property position could not be found in the data set."));

  return std::auto_ptr<te::dt::Property>(props[propertyPos]->clone());
}

std::vector<std::string> te::wms::Transactor::getPropertyNames(const std::string& datasetName)
{
  std::vector<std::string> propertyNames;

  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();
  for(std::size_t i = 0; i < props.size(); ++i)
    propertyNames.push_back(props[i]->getName());

  return propertyNames;
}

std::size_t te::wms::Transactor::getNumberOfProperties(const std::string& datasetName)
{
  return getPropertyNames(datasetName).size();
}

bool te::wms::Transactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  std::vector<std::string> propertyNames = getPropertyNames(datasetName);

  for(std::size_t i = 0; i < propertyNames.size(); ++i)
    if(propertyNames[i] == name)
      return true;

  return false;
}

std::auto_ptr<te::gm::Envelope> te::wms::Transactor::getExtent(const std::string& datasetName,
                                                               const std::string& propertyName)
{
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  std::auto_ptr<te::dt::Property> prop = getProperty(datasetName, propertyName);

  te::rst::RasterProperty* rasterProperty = static_cast<te::rst::RasterProperty*>(prop.get());
  te::gm::Envelope* e = rasterProperty->getGrid()->getExtent();
  
  return std::auto_ptr<te::gm::Envelope>(new te::gm::Envelope(e->getLowerLeftX(), e->getLowerLeftY(),
                                                              e->getUpperRightX(), e->getUpperRightY()));
}

std::auto_ptr<te::gm::Envelope> te::wms::Transactor::getExtent(const std::string& datasetName,
                                                               std::size_t propertyPos)
{
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  std::auto_ptr<te::dt::Property> prop = getProperty(datasetName, propertyPos);

  te::rst::RasterProperty* rasterProperty = static_cast<te::rst::RasterProperty*>(prop.get());
  te::gm::Envelope* e = rasterProperty->getGrid()->getExtent();
  
  return std::auto_ptr<te::gm::Envelope>(new te::gm::Envelope(e->getLowerLeftX(), e->getLowerLeftY(),
                                                              e->getUpperRightX(), e->getUpperRightY()));
}

std::size_t te::wms::Transactor::getNumberOfItems(const std::string& datasetName)
{
  return 1;
}

bool te::wms::Transactor::hasDataSets()
{
  return !m_layersInfo.empty();
}

bool te::wms::Transactor::dataSetExists(const std::string& name)
{
  return m_layersInfo.find(name) != m_layersInfo.end();
}

/** NOT SUPPORTED METHODS */
//@{

void te::wms::Transactor::begin()
{
  throw Exception(TR_WMS("The method begin() is not supported by the WMS driver."));
}

void te::wms::Transactor::commit()
{
  throw Exception(TR_WMS("The method commit() is not supported by the WMS driver."));
}

void te::wms::Transactor::rollBack()
{
  throw Exception(TR_WMS("The method rollBack() is not supported by the WMS driver."));
}

bool te::wms::Transactor::isInTransaction() const
{
  throw Exception(TR_WMS("The method isInTransaction() is not supported by the WMS driver."));
}

void te::wms::Transactor::execute(const te::da::Query& /*command*/)
{
  throw Exception(TR_WMS("The method execute() is not supported by the WMS driver."));
}

void te::wms::Transactor::execute(const std::string& /*command*/)
{
  throw Exception(TR_WMS("The method execute() is not supported by the WMS driver."));
}

std::auto_ptr<te::da::PreparedQuery> te::wms::Transactor::getPrepared(const std::string& /*qName*/)
{
  throw Exception(TR_WMS("The method getPrepared() is not supported by the WMS driver."));
}

std::auto_ptr<te::da::BatchExecutor> te::wms::Transactor::getBatchExecutor()
{
  throw Exception(TR_WMS("The method getBatchExecutor() is not supported by the WMS driver."));
}

void te::wms::Transactor::cancel()
{
  throw Exception(TR_WMS("The method cancel() is not supported by the WMS driver."));
}

boost::int64_t te::wms::Transactor::getLastGeneratedId()
{
  throw Exception(TR_WMS("The method getLastGeneratedId() is not supported by the WMS driver."));
}

std::string te::wms::Transactor::escape(const std::string& /*value*/)
{
  throw Exception(TR_WMS("The method escape() is not supported by the WMS driver."));
}

bool te::wms::Transactor::isDataSetNameValid(const std::string& /*datasetName*/)
{
  throw Exception(TR_WMS("The method isDataSetNameValid() is not supported by the WMS driver."));
}

bool te::wms::Transactor::isPropertyNameValid(const std::string& /*propertyName*/)
{
  throw Exception(TR_WMS("The method isPropertyNameValid() is not supported by the WMS driver."));
}

void te::wms::Transactor::addProperty(const std::string& /*datasetName*/, te::dt::Property* /*p*/)
{
  throw Exception(TR_WMS("The method addProperty() is not supported by the WMS driver."));
}

void te::wms::Transactor::dropProperty(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method dropProperty() is not supported by the WMS driver."));
}

void te::wms::Transactor::renameProperty(const std::string& /*datasetName*/, const std::string& /*propertyName*/, const std::string& /*newPropertyName*/)
{
  throw Exception(TR_WMS("The method renameProperty() is not supported by the WMS driver."));
}

std::auto_ptr<te::da::PrimaryKey> te::wms::Transactor::getPrimaryKey(const std::string& /*datasetName*/)
{
  throw Exception(TR_WMS("The method getPrimaryKey() is not supported by the WMS driver."));
}

bool te::wms::Transactor::primaryKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method primaryKeyExists() is not supported by the WMS driver."));
}

void te::wms::Transactor::addPrimaryKey(const std::string& /*datasetName*/, te::da::PrimaryKey* /*pk*/)
{
  throw Exception(TR_WMS("The method addPrimaryKey() is not supported by the WMS driver."));
}

void te::wms::Transactor::dropPrimaryKey(const std::string& /*datasetName*/)
{
  throw Exception(TR_WMS("The method dropPrimaryKey() is not supported by the WMS driver."));
}

std::auto_ptr<te::da::ForeignKey> te::wms::Transactor::getForeignKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method getForeignKey() is not supported by the WMS driver."));
}

std::vector<std::string> te::wms::Transactor::getForeignKeyNames(const std::string& /*datasetName*/)
{
  throw Exception(TR_WMS("The method getForeignKeyNames() is not supported by the WMS driver."));
}

bool te::wms::Transactor::foreignKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method foreignKeyExists() is not supported by the WMS driver."));
}

void te::wms::Transactor::addForeignKey(const std::string& /*datasetName*/, te::da::ForeignKey* /*fk*/)
{
  throw Exception(TR_WMS("The method addForeignKey() is not supported by the WMS driver."));
}

void te::wms::Transactor::dropForeignKey(const std::string& /*datasetName*/, const std::string& /*fkName*/)
{
  throw Exception(TR_WMS("The method dropForeignKey() is not supported by the WMS driver."));
}

std::auto_ptr<te::da::UniqueKey> te::wms::Transactor::getUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method getUniqueKey() is not supported by the WMS driver."));
}

std::vector<std::string> te::wms::Transactor::getUniqueKeyNames(const std::string& /*datasetName*/)
{
  throw Exception(TR_WMS("The method getUniqueKeyNames() is not supported by the WMS driver."));
}

bool te::wms::Transactor::uniqueKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method uniqueKeyExists() is not supported by the WMS driver."));
}

void te::wms::Transactor::addUniqueKey(const std::string& /*datasetName*/, te::da::UniqueKey* /*uk*/)
{
  throw Exception(TR_WMS("The method addUniqueKey() is not supported by the WMS driver."));
}

void te::wms::Transactor::dropUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method dropUniqueKey() is not supported by the WMS driver."));
}

std::auto_ptr<te::da::CheckConstraint> te::wms::Transactor::getCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method getCheckConstraint() is not supported by the WMS driver."));
}

std::vector<std::string> te::wms::Transactor::getCheckConstraintNames(const std::string& /*datasetName*/)
{
  throw Exception(TR_WMS("The method getCheckConstraintNames() is not supported by the WMS driver."));
}

bool te::wms::Transactor::checkConstraintExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method checkConstraintExists() is not supported by the WMS driver."));
}

void te::wms::Transactor::addCheckConstraint(const std::string& /*datasetName*/, te::da::CheckConstraint* /*cc*/)
{
  throw Exception(TR_WMS("The method addCheckConstraint() is not supported by the WMS driver."));
}

void te::wms::Transactor::dropCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method dropCheckConstraint() is not supported by the WMS driver."));
}

std::auto_ptr<te::da::Index> te::wms::Transactor::getIndex(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method getIndex() is not supported by the WMS driver."));
}

std::vector<std::string> te::wms::Transactor::getIndexNames(const std::string& /*datasetName*/)
{
  throw Exception(TR_WMS("The method getIndexNames() is not supported by the WMS driver."));
}

bool te::wms::Transactor::indexExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method indexExists() is not supported by the WMS driver."));
}

void te::wms::Transactor::addIndex(const std::string& /*datasetName*/, te::da::Index* /*idx*/, const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_WMS("The method addIndex() is not supported by the WMS driver."));
}

void te::wms::Transactor::dropIndex(const std::string& /*datasetName*/, const std::string& /*idxName*/)
{
  throw Exception(TR_WMS("The method dropIndex() is not supported by the WMS driver."));
}

std::auto_ptr<te::da::Sequence> te::wms::Transactor::getSequence(const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method getSequence() is not supported by the WMS driver."));
}

std::vector<std::string> te::wms::Transactor::getSequenceNames()
{
  throw Exception(TR_WMS("The method getSequenceNames() is not supported by the WMS driver."));
}

bool te::wms::Transactor::sequenceExists(const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method sequenceExists() is not supported by the WMS driver."));
}

void te::wms::Transactor::addSequence(te::da::Sequence* /*sequence*/)
{
  throw Exception(TR_WMS("The method addSequence() is not supported by the WMS driver."));
}

void te::wms::Transactor::dropSequence(const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method dropSequence() is not supported by the WMS driver."));
}

void te::wms::Transactor::createDataSet(te::da::DataSetType* /*dt*/,
                                        const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_WMS("The method createDataSet() is not supported by the WMS driver."));
}

void te::wms::Transactor::cloneDataSet(const std::string& /*name*/,
                                       const std::string& /*cloneName*/,
                                       const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_WMS("The method cloneDataSet() is not supported by the WMS driver."));
}

void te::wms::Transactor::dropDataSet(const std::string& /*name*/)
{
  throw Exception(TR_WMS("The method dropDataSet() is not supported by the WMS driver."));
}

void te::wms::Transactor::renameDataSet(const std::string& /*name*/,
                                        const std::string& /*newName*/)
{
  throw Exception(TR_WMS("The method renameDataSet() is not supported by the WMS driver."));
}

void te::wms::Transactor::add(const std::string& /*datasetName*/,
                              te::da::DataSet* /*d*/,
                              const std::map<std::string, std::string>& /*options*/,
                              std::size_t /*limit*/)
{
  throw Exception(TR_WMS("The method add() is not supported by the WMS driver."));
}

void te::wms::Transactor::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw Exception(TR_WMS("The method remove() is not supported by the WMS driver."));
}

void te::wms::Transactor::update(const std::string& /*datasetName*/,
                                 te::da::DataSet* /*dataset*/,
                                 const std::vector<std::size_t>& /*properties*/,
                                 const te::da::ObjectIdSet* /*oids*/,
                                 const std::map<std::string, std::string>& /*options*/,
                                 std::size_t /*limit*/)
{
  throw Exception(TR_WMS("The method update() is not supported by the WMS driver."));
}

void te::wms::Transactor::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TR_WMS("The method optimize() is not supported by the WMS driver."));
}

//@}
