#include "Transactor.h"

//TerraLib
#include "../../../core/Exception.h"

//STL
#include <algorithm>

te::ws::ogc::wms::da::Transactor::Transactor(const std::shared_ptr<te::ws::ogc::WMSClient> wms) :
  m_wms(wms)
{
  te::ws::ogc::wms::WMSCapabilities capabilities = m_wms->getCapabilities();

  this->createLayerMap(capabilities.m_capability.m_layer);
}

te::ws::ogc::wms::da::Transactor::~Transactor()
{
}

te::da::DataSource* te::ws::ogc::wms::da::Transactor::getDataSource() const
{
  return 0;
}

void te::ws::ogc::wms::da::Transactor::begin()
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method begin() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::commit()
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method commit() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::rollBack()
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method rollBack() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wms::da::Transactor::getDataSet(const std::string &name, te::common::TraverseType travType, bool connected, const te::common::AccessPolicy accessPolicy)
{
  return std::auto_ptr<te::da::DataSet>();
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wms::da::Transactor::getDataSet(const std::string &name, const std::string &propertyName, const te::gm::Envelope *e, te::gm::SpatialRelation r, te::common::TraverseType travType, bool connected, const te::common::AccessPolicy accessPolicy)
{
  return std::auto_ptr<te::da::DataSet>();
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wms::da::Transactor::getDataSet(const std::string &name, const std::string &propertyName, const te::gm::Geometry *g, te::gm::SpatialRelation r, te::common::TraverseType travType, bool connected, const te::common::AccessPolicy accessPolicy)
{
  return std::auto_ptr<te::da::DataSet>();
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wms::da::Transactor::getDataSet(const std::string &name,
                                                                            const te::da::ObjectIdSet *oids,
                                                                            te::common::TraverseType travType,
                                                                            bool connected,
                                                                            const te::common::AccessPolicy accessPolicy)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The ObjectIdSet concept is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wms::da::Transactor::query(const te::da::Select &q,
                                                                       te::common::TraverseType travType,
                                                                       bool connected,
                                                                       const te::common::AccessPolicy accessPolicy)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("Query operations is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::DataSet> te::ws::ogc::wms::da::Transactor::query(const std::string &query,
                                                                       te::common::TraverseType travType,
                                                                       bool connected,
                                                                       const te::common::AccessPolicy accessPolicy)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("Query operations is not supported by the WMS driver!"));
}

std::vector<std::string> te::ws::ogc::wms::da::Transactor::getLayerNames()
{
  std::vector<std::string> layerNames;

  std::map<std::string, te::ws::ogc::wms::Layer>::const_iterator it;

  for(it = m_layerMap.begin();it != m_layerMap.end(); it++)
  {
    layerNames.push_back(it->first);
  }

  return layerNames;
}

te::ws::ogc::wms::Layer te::ws::ogc::wms::da::Transactor::getLayerByName(const std::string &layerName)
{
  if(!this->dataSetExists(layerName))
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The informed data set could not be found in the data source."));

  return m_layerMap[layerName];
}

void te::ws::ogc::wms::da::Transactor::createLayerMap(const te::ws::ogc::wms::Layer &rootLayer)
{
  m_layerMap[rootLayer.m_name] = rootLayer;

  if(rootLayer.m_layers.size() > 0)
  {
    for(unsigned int i = 0; i < rootLayer.m_layers.size(); i++)
    {
      this->createLayerMap(rootLayer.m_layers[i]);
    }
  }
}

std::vector<std::string> te::ws::ogc::wms::da::Transactor::getDataSetNames()
{
  std::vector<std::string> dataSetNames;

  dataSetNames = this->getLayerNames();

  return dataSetNames;
}

std::size_t te::ws::ogc::wms::da::Transactor::getNumberOfDataSets()
{
  std::vector<std::string> layers = this->getLayerNames();

  return layers.size();
}

std::auto_ptr<te::da::DataSetType> te::ws::ogc::wms::da::Transactor::getDataSetType(const std::string &name)
{
  if(!this->dataSetExists(name))
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The informed data set could not be found in the data source."));
  
  te::ws::ogc::wms::Layer layer = this->getLayerByName(name);

  te::da::DataSetType* type = new te::da::DataSetType(layer.m_name, 0);
  type->setTitle(layer.m_title);

  return std::auto_ptr<te::da::DataSetType>(type);
}

boost::ptr_vector<te::dt::Property> te::ws::ogc::wms::da::Transactor::getProperties(const std::string &datasetName)
{
  boost::ptr_vector<te::dt::Property> properties;

  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();
  for(std::size_t i = 0; i < props.size(); ++i)
      properties.push_back(props[i]->clone());

  return properties;
}

std::auto_ptr<te::dt::Property> te::ws::ogc::wms::da::Transactor::getProperty(const std::string &datasetName, const std::string &name)
{
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();
  for(std::size_t i = 0; i < props.size(); ++i)
  {
    if(props[i]->getName() == name)
      return std::auto_ptr<te::dt::Property>(props[i]->clone());
  }

  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The informed property name could not be found in the data set."));
}

std::auto_ptr<te::dt::Property> te::ws::ogc::wms::da::Transactor::getProperty(const std::string &datasetName, std::size_t propertyPos)
{
  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();

  if(propertyPos >= props.size())
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The informed property position could not be found in the data set."));

  return std::auto_ptr<te::dt::Property>(props[propertyPos]->clone());
}

std::vector<std::string> te::ws::ogc::wms::da::Transactor::getPropertyNames(const std::string &datasetName)
{
  std::vector<std::string> propertyNames;

  std::auto_ptr<te::da::DataSetType> type = getDataSetType(datasetName);

  const std::vector<te::dt::Property*>& props = type->getProperties();
  for(std::size_t i = 0; i < props.size(); ++i)
    propertyNames.push_back(props[i]->getName());

  return propertyNames;
}

std::size_t te::ws::ogc::wms::da::Transactor::getNumberOfProperties(const std::string &datasetName)
{
  return getPropertyNames(datasetName).size();
}

bool te::ws::ogc::wms::da::Transactor::propertyExists(const std::string &datasetName, const std::string &name)
{
  std::vector<std::string> propertyNames = this->getPropertyNames(datasetName);

  if(std::find(propertyNames.begin(), propertyNames.end(), name) != propertyNames.end())
    return true;

  return false;
}

void te::ws::ogc::wms::da::Transactor::execute(const te::da::Query &command)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method execute() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::execute(const std::string &command)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method execute() is not supported by the WMS driver!"));
}

bool te::ws::ogc::wms::da::Transactor::isInTransaction() const
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method isInTransaction() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::PreparedQuery> te::ws::ogc::wms::da::Transactor::getPrepared(const std::string& /*qName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getPrepared() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::BatchExecutor> te::ws::ogc::wms::da::Transactor::getBatchExecutor()
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getBatchExecutor() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::cancel()
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method cancel() is not supported by the WMS driver!"));
}

boost::int64_t te::ws::ogc::wms::da::Transactor::getLastGeneratedId()
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getLastGeneratedId() is not supported by the WMS driver!"));
}

std::string te::ws::ogc::wms::da::Transactor::escape(const std::string& /*value*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method escape() is not supported by the WMS driver!"));
}

bool te::ws::ogc::wms::da::Transactor::isDataSetNameValid(const std::string& /*datasetName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method isDataSetNameValid() is not supported by the WMS driver!"));
}

bool te::ws::ogc::wms::da::Transactor::isPropertyNameValid(const std::string& /*propertyName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method isPropertyNameValid() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::addProperty(const std::string& /*datasetName*/, te::dt::Property* /*p*/)
{
 throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method addProperty() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::dropProperty(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method dropProperty() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::renameProperty(const std::string& /*datasetName*/, const std::string& /*propertyName*/, const std::string& /*newPropertyName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method renameProperty() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::PrimaryKey> te::ws::ogc::wms::da::Transactor::getPrimaryKey(const std::string& /*datasetName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getPrimaryKey() is not supported by the WMS driver!"));
}

bool te::ws::ogc::wms::da::Transactor::primaryKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method primaryKeyExists() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::addPrimaryKey(const std::string& /*datasetName*/, te::da::PrimaryKey* /*pk*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method addPrimaryKey() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::dropPrimaryKey(const std::string& /*datasetName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method dropPrimaryKey() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::ForeignKey> te::ws::ogc::wms::da::Transactor::getForeignKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getForeignKey() is not supported by the WMS driver!"));
}

std::vector<std::string> te::ws::ogc::wms::da::Transactor::getForeignKeyNames(const std::string& /*datasetName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getForeignKeyNames() is not supported by the WMS driver!"));
}

bool te::ws::ogc::wms::da::Transactor::foreignKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method foreignKeyExists() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::addForeignKey(const std::string& /*datasetName*/, te::da::ForeignKey* /*fk*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method addForeignKey() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::dropForeignKey(const std::string& /*datasetName*/, const std::string& /*fkName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method dropForeignKey() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::UniqueKey> te::ws::ogc::wms::da::Transactor::getUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getUniqueKey() is not supported by the WMS driver!"));
}

std::vector<std::string> te::ws::ogc::wms::da::Transactor::getUniqueKeyNames(const std::string& /*datasetName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getUniqueKeyNames() is not supported by the WMS driver!"));
}

bool te::ws::ogc::wms::da::Transactor::uniqueKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method uniqueKeyExists() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::addUniqueKey(const std::string& /*datasetName*/, te::da::UniqueKey* /*uk*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method addUniqueKey() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::dropUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method dropUniqueKey() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::CheckConstraint> te::ws::ogc::wms::da::Transactor::getCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getCheckConstraint() is not supported by the WMS driver!"));
}

std::vector<std::string> te::ws::ogc::wms::da::Transactor::getCheckConstraintNames(const std::string& /*datasetName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getCheckConstraintNames() is not supported by the WMS driver!"));
}

bool te::ws::ogc::wms::da::Transactor::checkConstraintExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method checkConstraintExists() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::addCheckConstraint(const std::string& /*datasetName*/, te::da::CheckConstraint* /*cc*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method addCheckConstraint() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::dropCheckConstraint(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method dropCheckConstraint() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::Index> te::ws::ogc::wms::da::Transactor::getIndex(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getIndex() is not supported by the WMS driver!"));
}

std::vector<std::string> te::ws::ogc::wms::da::Transactor::getIndexNames(const std::string& /*datasetName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getIndexNames() is not supported by the WMS driver!"));
}

bool te::ws::ogc::wms::da::Transactor::indexExists(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method indexExists() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::addIndex(const std::string& /*datasetName*/, te::da::Index* /*idx*/, const std::map<std::string, std::string>& /*options*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method addIndex() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::dropIndex(const std::string& /*datasetName*/, const std::string& /*idxName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method dropIndex() is not supported by the WMS driver!"));
}

std::auto_ptr<te::da::Sequence> te::ws::ogc::wms::da::Transactor::getSequence(const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getSequence() is not supported by the WMS driver!"));
}

std::vector<std::string> te::ws::ogc::wms::da::Transactor::getSequenceNames()
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method getSequenceNames() is not supported by the WMS driver!"));
}

bool te::ws::ogc::wms::da::Transactor::sequenceExists(const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method sequenceExists() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::addSequence(te::da::Sequence* /*sequence*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method addSequence() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::dropSequence(const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method dropSequence() is not supported by the WMS driver!"));
}

std::auto_ptr<te::gm::Envelope> te::ws::ogc::wms::da::Transactor::getExtent(const std::string &datasetName, const std::string &propertyName)
{
  return std::auto_ptr<te::gm::Envelope>();
}

std::auto_ptr<te::gm::Envelope> te::ws::ogc::wms::da::Transactor::getExtent(const std::string &datasetName, std::size_t propertyPos)
{
  return std::auto_ptr<te::gm::Envelope>();
}

std::size_t te::ws::ogc::wms::da::Transactor::getNumberOfItems(const std::string &datasetName)
{
  return 1;
}

bool te::ws::ogc::wms::da::Transactor::hasDataSets()
{
  if(this->getDataSetNames().size() > 0)
    return true;

  return false;
}

bool te::ws::ogc::wms::da::Transactor::dataSetExists(const std::string &name)
{
  if(m_layerMap.find(name) != m_layerMap.end())
  {
    return true;
  }

  return false;
}

void te::ws::ogc::wms::da::Transactor::createDataSet(te::da::DataSetType* /*dt*/,
                                        const std::map<std::string, std::string>& /*options*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method createDataSet() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::cloneDataSet(const std::string& /*name*/,
                                       const std::string& /*cloneName*/,
                                       const std::map<std::string, std::string>& /*options*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method cloneDataSet() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::dropDataSet(const std::string& /*name*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method dropDataSet() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::renameDataSet(const std::string& /*name*/,
                                        const std::string& /*newName*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method renameDataSet() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::add(const std::string& /*datasetName*/,
                              te::da::DataSet* /*d*/,
                              const std::map<std::string, std::string>& /*options*/,
                              std::size_t /*limit*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method add() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method remove() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::update(const std::string& /*datasetName*/,
                                 te::da::DataSet* /*dataset*/,
                                 const std::vector<std::size_t>& /*properties*/,
                                 const te::da::ObjectIdSet* /*oids*/,
                                 const std::map<std::string, std::string>& /*options*/,
                                 std::size_t /*limit*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method update() is not supported by the WMS driver!"));
}

void te::ws::ogc::wms::da::Transactor::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("The method optimize() is not supported by the WMS driver!"));
}

te::core::EncodingType te::ws::ogc::wms::da::Transactor::getEncoding()
{
  return te::core::EncodingType::UNKNOWN;
}
