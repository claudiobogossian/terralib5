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
  \file terralib/ws/ogc/wcs/dataaccess/Transactor.cpp

  \brief Implementation of the transactor for WS OGC WCS driver

  \author Vinicius Campanha
*/


// STL
#include <vector>

// TerraLib
#include "Transactor.h"
#include "Exception.h"
#include "../../wcs/dataaccess/DataSource.h"
#include "../../../../core/translator/Translator.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../raster/Grid.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/dataset/DataSet.h"
#include "../../../../ws/ogc/wcs/client/WCS.h"
#include "../../../../datatype.h"
#include "../../../../raster/RasterProperty.h"
#include "../../../../raster/Grid.h"
#include "../../../../raster/BandProperty.h"
#include "../../../../common/StringUtils.h"

te::ws::ogc::wcs::da::Transactor::Transactor(WCS wcs)
  : te::da::DataSourceTransactor(),
    m_wcs(wcs)
{
}

te::ws::ogc::wcs::da::Transactor::~Transactor()
{
}

te::ws::ogc::CoverageDescription te::ws::ogc::wcs::da::Transactor::coverageDescription(const std::string coverageName) const
{
  return m_wcs.describeCoverage(coverageName);
}

void te::ws::ogc::wcs::da::Transactor::setCoverageRequest(const te::ws::ogc::CoverageRequest coverageRequest)
{
  m_coverageRequest = coverageRequest;
}

te::da::DataSource* te::ws::ogc::wcs::da::Transactor::getDataSource() const
{
  return 0;
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wcs::da::Transactor::getDataSet(const std::string& name,
                                                               te::common::TraverseType /*travType*/,
                                                               bool /*connected*/,
                                                               const te::common::AccessPolicy /*accessPolicy*/)
{
  if(!dataSetExists(name))
    throw Exception(TE_TR("The informed data set could not be found in the data source!"));

  m_coverageRequest = te::ws::ogc::CoverageRequest();
  m_coverageRequest.coverageID = name;
  
  std::string coveragePath = m_wcs.getCoverage(m_coverageRequest);

  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = coveragePath;

  std::auto_ptr<te::da::DataSource> dataSource = te::da::DataSourceFactory::make("GDAL");
  dataSource->setConnectionInfo(connInfo);
  dataSource->open();

  if (!dataSource->isOpened() || !dataSource->isValid())
    throw Exception(TE_TR("Fail to build Data Set. Data Source isn't valid or open!"));

  return dataSource->getDataSet(name);
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wcs::da::Transactor::getDataSet(const std::string& name,
                                                               const std::string& propertyName,
                                                               const te::gm::Envelope* e,
                                                               te::gm::SpatialRelation r,
                                                               te::common::TraverseType travType,
                                                               bool connected,
                                                               const te::common::AccessPolicy accessPolicy)
{
  if(!dataSetExists(name))
    throw Exception(TE_TR("The informed data set could not be found in the data source!"));


  te::ws::ogc::CoverageDescription description = m_wcs.describeCoverage(name);

  m_coverageRequest = te::ws::ogc::CoverageRequest();
  m_coverageRequest.coverageID = name;

  std::string coveragePath = m_wcs.getCoverage(m_coverageRequest);

  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = coveragePath;

  std::auto_ptr<te::da::DataSource> dataSource = te::da::DataSourceFactory::make("GDAL");
  dataSource->setConnectionInfo(connInfo);
  dataSource->open();

  if (!dataSource->isOpened() || !dataSource->isValid())
    throw Exception(TE_TR("Fail to build Data Set. Data Source isn't valid or open!"));

  return dataSource->getDataSet(name, propertyName, e, r, travType, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wcs::da::Transactor::getDataSet(const std::string& name,
                                                               const std::string& propertyName,
                                                               const te::gm::Geometry* g,
                                                               te::gm::SpatialRelation r,
                                                               te::common::TraverseType travType,
                                                               bool connected,
                                                               const te::common::AccessPolicy accessPolicy)
{
  return getDataSet(name, propertyName, g->getMBR(), r, travType, connected, accessPolicy);
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wcs::da::Transactor::getDataSet(const std::string& name,
                                                               const te::da::ObjectIdSet* oids,
                                                               te::common::TraverseType travType,
                                                               bool connected,
                                                               const te::common::AccessPolicy accessPolicy)
{
  throw Exception(TE_TR("The ObjectIdSet concept is not supported by the WCS driver!"));
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wcs::da::Transactor::query(const te::da::Select& q,
                                                          te::common::TraverseType travType,
                                                          bool connected,
                                                          const te::common::AccessPolicy accessPolicy)
{
  throw Exception(TE_TR("Query operations is not supported by the WCS driver!"));
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wcs::da::Transactor::query(const std::string& query,
                                                          te::common::TraverseType travType,
                                                          bool connected,
                                                          const te::common::AccessPolicy accessPolicy)
{
  throw Exception(TE_TR("Query operations is not supported by the WCS driver!"));
}

std::vector<std::string> te::ws::ogc::wcs::da::Transactor::getDataSetNames()
{
  return m_wcs.getCapabilities().coverages;
}

std::size_t te::ws::ogc::wcs::da::Transactor::getNumberOfDataSets()
{
  return m_wcs.getCapabilities().coverages.size();
}

std::auto_ptr<te::da::DataSetType> te::ws::ogc::wcs::da::Transactor::getDataSetType(const std::string& name)
{
  if(!dataSetExists(name))
    throw Exception(TE_TR("The informed data set could not be found in the data source!"));

  te::ws::ogc::CoverageDescription description =  this->coverageDescription(name);

  te::da::DataSetType* type = new te::da::DataSetType(description.coverageId, 0);
  type->setTitle(description.coverageId);

  te::ws::ogc::EnvelopeWithTimePeriod boundedBy = description.envelope;

  double llx = atof(boundedBy.lowerCorner_X.c_str());
  double lly = atof(boundedBy.lowerCorner_Y.c_str());
  double urx = atof(boundedBy.upperCorner_X.c_str());
  double ury = atof(boundedBy.upperCorner_Y.c_str());

  int srid = TE_UNKNOWN_SRS;

  if (!boundedBy.srsName.empty())
  {
    int pos = boundedBy.srsName.find(":");
    std::string sridStr = boundedBy.srsName.substr(pos + 1);

    srid = atoi(sridStr.c_str());
  }

  te::gm::Envelope* env = new te::gm::Envelope(llx, lly, urx, ury);

  unsigned int width = 0;
  unsigned int height = 0;

  if (description.domainSet.subSet.size() == 2)
  {
    const SubSet& subSetX = description.domainSet.subSet[0];
    const SubSet& subSetY = description.domainSet.subSet[1];

    width = atoi(subSetX.max.c_str());
    height = atoi(subSetY.max.c_str());
  }

  te::rst::Grid* grid = new te::rst::Grid(width, height, env, srid);

  te::rst::RasterProperty* rp = new te::rst::RasterProperty("raster");
  rp->set(grid);

  for(std::size_t i = 0; i < description.fieldNames.size(); ++i)
  {
    te::rst::BandProperty* prop = new te::rst::BandProperty(i,  te::dt::UCHAR_TYPE, description.fieldNames[i]);
    rp->add(prop);
  }

  type->add(rp);

  return std::auto_ptr<te::da::DataSetType>(type);
}

boost::ptr_vector<te::dt::Property> te::ws::ogc::wcs::da::Transactor::getProperties(const std::string& datasetName)
{
  boost::ptr_vector<te::dt::Property> properties;

  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();
  for(std::size_t i = 0; i < props.size(); ++i)
      properties.push_back(props[i]->clone());

  return properties;
}

std::auto_ptr<te::dt::Property> te::ws::ogc::wcs::da::Transactor::getProperty(const std::string& datasetName, const std::string& name)
{
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();
  for(std::size_t i = 0; i < props.size(); ++i)
  {
    if(props[i]->getName() == name)
      return std::auto_ptr<te::dt::Property>(props[i]->clone());
  }

  throw Exception(TE_TR("The informed property name could not be found in the data set."));
}

std::auto_ptr<te::dt::Property> te::ws::ogc::wcs::da::Transactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();

  if(propertyPos >= props.size())
    throw Exception(TE_TR("The informed property position could not be found in the data set."));

  return std::auto_ptr<te::dt::Property>(props[propertyPos]->clone());
}

std::vector<std::string> te::ws::ogc::wcs::da::Transactor::getPropertyNames(const std::string& datasetName)
{
  std::vector<std::string> propertyNames;

  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();
  for(std::size_t i = 0; i < props.size(); ++i)
    propertyNames.push_back(props[i]->getName());

  return propertyNames;
}

std::size_t te::ws::ogc::wcs::da::Transactor::getNumberOfProperties(const std::string& datasetName)
{
  return getPropertyNames(datasetName).size();
}

bool te::ws::ogc::wcs::da::Transactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  std::vector<std::string> propertyNames = getPropertyNames(datasetName);

  for(std::size_t i = 0; i < propertyNames.size(); ++i)
    if(propertyNames[i] == name)
      return true;

  return false;
}

std::auto_ptr<te::gm::Envelope> te::ws::ogc::wcs::da::Transactor::getExtent(const std::string& datasetName,
                                                               const std::string& propertyName)
{
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  std::auto_ptr<te::dt::Property> prop = getProperty(datasetName, propertyName);
/*
  te::rst::RasterProperty* rasterProperty = static_cast<te::rst::RasterProperty*>(prop.get());
  te::gm::Envelope* e = rasterProperty->getGrid()->getExtent();

  return std::auto_ptr<te::gm::Envelope>(new te::gm::Envelope(e->getLowerLeftX(), e->getLowerLeftY(),
                                                              e->getUpperRightX(), e->getUpperRightY()));
  */
  return std::auto_ptr<te::gm::Envelope>();
}

std::auto_ptr<te::gm::Envelope> te::ws::ogc::wcs::da::Transactor::getExtent(const std::string& datasetName,
                                                               std::size_t propertyPos)
{
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  std::auto_ptr<te::dt::Property> prop = getProperty(datasetName, propertyPos);
/*
  te::rst::RasterProperty* rasterProperty = static_cast<te::rst::RasterProperty*>(prop.get());
  te::gm::Envelope* e = rasterProperty->getGrid()->getExtent();

  return std::auto_ptr<te::gm::Envelope>(new te::gm::Envelope(e->getLowerLeftX(), e->getLowerLeftY(),
                                                              e->getUpperRightX(), e->getUpperRightY()));
*/
  return std::auto_ptr<te::gm::Envelope>();
}

std::size_t te::ws::ogc::wcs::da::Transactor::getNumberOfItems(const std::string& datasetName)
{
  return 1;
}

bool te::ws::ogc::wcs::da::Transactor::hasDataSets()
{
  if(m_wcs.getCapabilities().coverages.size() > 0)
    return true;

  return false;
}

bool te::ws::ogc::wcs::da::Transactor::dataSetExists(const std::string& name)
{
  std::vector< std::string > coverages = m_wcs.getCapabilities().coverages;

  for(int i = 0; i < coverages.size(); i ++)
  {
    if(coverages.at(i).compare(name) == 0)
      return true;
  }

  return false;
}

te::core::EncodingType te::ws::ogc::wcs::da::Transactor::getEncoding()
{
  return te::core::EncodingType::UNKNOWN;
}

/** NOT SUPPORTED METHODS */
//@{

void te::ws::ogc::wcs::da::Transactor::begin()
{
  throw Exception(TE_TR("The method begin() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::commit()
{
  throw Exception(TE_TR("The method commit() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::rollBack()
{
  throw Exception(TE_TR("The method rollBack() is not supported by the WCS driver!"));
}

bool te::ws::ogc::wcs::da::Transactor::isInTransaction() const
{
  throw Exception(TE_TR("The method isInTransaction() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::execute(const te::da::Query& /*command*/)
{
  throw Exception(TE_TR("The method execute() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::execute(const std::string& /*command*/)
{
  throw Exception(TE_TR("The method execute() is not supported by the WCS driver!"));
}

std::auto_ptr<te::da::PreparedQuery> te::ws::ogc::wcs::da::Transactor::getPrepared(const std::string& /*qName*/)
{
  throw Exception(TE_TR("The method getPrepared() is not supported by the WCS driver!"));
}

std::auto_ptr<te::da::BatchExecutor> te::ws::ogc::wcs::da::Transactor::getBatchExecutor()
{
  throw Exception(TE_TR("The method getBatchExecutor() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::cancel()
{
  throw Exception(TE_TR("The method cancel() is not supported by the WCS driver!"));
}

boost::int64_t te::ws::ogc::wcs::da::Transactor::getLastGeneratedId()
{
  throw Exception(TE_TR("The method getLastGeneratedId() is not supported by the WCS driver!"));
}

std::string te::ws::ogc::wcs::da::Transactor::escape(const std::string& /*value*/)
{
  throw Exception(TE_TR("The method escape() is not supported by the WCS driver!"));
}

bool te::ws::ogc::wcs::da::Transactor::isDataSetNameValid(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method isDataSetNameValid() is not supported by the WCS driver!"));
}

bool te::ws::ogc::wcs::da::Transactor::isPropertyNameValid(const std::string& /*propertyName*/)
{
  throw Exception(TE_TR("The method isPropertyNameValid() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::addProperty(const std::string& /*datasetName*/, te::dt::Property* /*p*/)
{
  throw Exception(TE_TR("The method addProperty() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::dropProperty(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method dropProperty() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::renameProperty(const std::string& /*datasetName*/, const std::string& /*propertyName*/, const std::string& /*newPropertyName*/)
{
  throw Exception(TE_TR("The method renameProperty() is not supported by the WCS driver!"));
}

std::auto_ptr<te::da::PrimaryKey> te::ws::ogc::wcs::da::Transactor::getPrimaryKey(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method getPrimaryKey() is not supported by the WCS driver!"));
}

bool te::ws::ogc::wcs::da::Transactor::primaryKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method primaryKeyExists() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::addPrimaryKey(const std::string& /*datasetName*/, te::da::PrimaryKey* /*pk*/)
{
  throw Exception(TE_TR("The method addPrimaryKey() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::dropPrimaryKey(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method dropPrimaryKey() is not supported by the WCS driver!"));
}

std::auto_ptr<te::da::ForeignKey> te::ws::ogc::wcs::da::Transactor::getForeignKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method getForeignKey() is not supported by the WCS driver!"));
}

std::vector<std::string> te::ws::ogc::wcs::da::Transactor::getForeignKeyNames(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method getForeignKeyNames() is not supported by the WCS driver!"));
}

bool te::ws::ogc::wcs::da::Transactor::foreignKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method foreignKeyExists() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::addForeignKey(const std::string& /*datasetName*/, te::da::ForeignKey* /*fk*/)
{
  throw Exception(TE_TR("The method addForeignKey() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::dropForeignKey(const std::string& /*datasetName*/, const std::string& /*fkName*/)
{
  throw Exception(TE_TR("The method dropForeignKey() is not supported by the WCS driver!"));
}

std::auto_ptr<te::da::UniqueKey> te::ws::ogc::wcs::da::Transactor::getUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method getUniqueKey() is not supported by the WCS driver!"));
}

std::vector<std::string> te::ws::ogc::wcs::da::Transactor::getUniqueKeyNames(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method getUniqueKeyNames() is not supported by the WCS driver!"));
}

bool te::ws::ogc::wcs::da::Transactor::uniqueKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method uniqueKeyExists() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::addUniqueKey(const std::string& /*datasetName*/, te::da::UniqueKey* /*uk*/)
{
  throw Exception(TE_TR("The method addUniqueKey() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::dropUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method dropUniqueKey() is not supported by the WCS driver!"));
}

std::auto_ptr<te::da::CheckConstraint> te::ws::ogc::wcs::da::Transactor::getCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method getCheckConstraint() is not supported by the WCS driver!"));
}

std::vector<std::string> te::ws::ogc::wcs::da::Transactor::getCheckConstraintNames(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method getCheckConstraintNames() is not supported by the WCS driver!"));
}

bool te::ws::ogc::wcs::da::Transactor::checkConstraintExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method checkConstraintExists() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::addCheckConstraint(const std::string& /*datasetName*/, te::da::CheckConstraint* /*cc*/)
{
  throw Exception(TE_TR("The method addCheckConstraint() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::dropCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method dropCheckConstraint() is not supported by the WCS driver!"));
}

std::auto_ptr<te::da::Index> te::ws::ogc::wcs::da::Transactor::getIndex(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method getIndex() is not supported by the WCS driver!"));
}

std::vector<std::string> te::ws::ogc::wcs::da::Transactor::getIndexNames(const std::string& /*datasetName*/)
{
  throw Exception(TE_TR("The method getIndexNames() is not supported by the WCS driver!"));
}

bool te::ws::ogc::wcs::da::Transactor::indexExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw Exception(TE_TR("The method indexExists() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::addIndex(const std::string& /*datasetName*/, te::da::Index* /*idx*/, const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TE_TR("The method addIndex() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::dropIndex(const std::string& /*datasetName*/, const std::string& /*idxName*/)
{
  throw Exception(TE_TR("The method dropIndex() is not supported by the WCS driver!"));
}

std::auto_ptr<te::da::Sequence> te::ws::ogc::wcs::da::Transactor::getSequence(const std::string& /*name*/)
{
  throw Exception(TE_TR("The method getSequence() is not supported by the WCS driver!"));
}

std::vector<std::string> te::ws::ogc::wcs::da::Transactor::getSequenceNames()
{
  throw Exception(TE_TR("The method getSequenceNames() is not supported by the WCS driver!"));
}

bool te::ws::ogc::wcs::da::Transactor::sequenceExists(const std::string& /*name*/)
{
  throw Exception(TE_TR("The method sequenceExists() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::addSequence(te::da::Sequence* /*sequence*/)
{
  throw Exception(TE_TR("The method addSequence() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::dropSequence(const std::string& /*name*/)
{
  throw Exception(TE_TR("The method dropSequence() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::createDataSet(te::da::DataSetType* /*dt*/,
                                        const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TE_TR("The method createDataSet() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::cloneDataSet(const std::string& /*name*/,
                                       const std::string& /*cloneName*/,
                                       const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TE_TR("The method cloneDataSet() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::dropDataSet(const std::string& /*name*/)
{
  throw Exception(TE_TR("The method dropDataSet() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::renameDataSet(const std::string& /*name*/,
                                        const std::string& /*newName*/)
{
  throw Exception(TE_TR("The method renameDataSet() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::add(const std::string& /*datasetName*/,
                              te::da::DataSet* /*d*/,
                              const std::map<std::string, std::string>& /*options*/,
                              std::size_t /*limit*/)
{
  throw Exception(TE_TR("The method add() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw Exception(TE_TR("The method remove() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::update(const std::string& /*datasetName*/,
                                 te::da::DataSet* /*dataset*/,
                                 const std::vector<std::size_t>& /*properties*/,
                                 const te::da::ObjectIdSet* /*oids*/,
                                 const std::map<std::string, std::string>& /*options*/,
                                 std::size_t /*limit*/)
{
  throw Exception(TE_TR("The method update() is not supported by the WCS driver!"));
}

void te::ws::ogc::wcs::da::Transactor::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TE_TR("The method optimize() is not supported by the WCS driver!"));
}

//@}

