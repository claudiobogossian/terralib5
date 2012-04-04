// Terralib Data Source Info tool
#include <DSInfo.h>

DSInfo::DSInfo()
{
  m_dataSourceType = "";
  m_connStr = "";
  m_dataSource = 0;
}

bool DSInfo::init(std::string dstype, std::string connstr)
{

  if(dstype.empty())
  {
    std::cout << "Command Line Error: Data Source Type (ds-type) missing!" << std::endl;
    return false;
  }
  else if(connstr.empty())
  {
    std::cout << "Command Line Error: Data Source Connection String (conn-str) missing!" << std::endl;
    return false;
  }

  m_dataSourceType = te::common::Convert2UCase(dstype);
  m_connStr = connstr;

  std::string errorMessage;
  Utils::loadModules(errorMessage);
   
  Utils::getDataSource(dstype, connstr, m_dataSource, errorMessage);
  
  if(m_dataSource == 0)
  {
    std::cout << std::endl << "Could not get a connection with the DataSource." << std::endl;
    return false;
  }
    
  return true;
}

PKInfo DSInfo::getPrimaryKeyInfo(std::string dataSetName)
{
  te::da::DataSourceTransactor* transactor = m_dataSource->getTransactor();  
  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();
  te::da::DataSetType* dataSetType = catalog->getDataSetType(dataSetName, true);
   
  te::da::PrimaryKey* pk = 0;
  pk = dataSetType->getPrimaryKey();

  PKInfo pkInfo;

  if(pk)
  {    
    pkInfo.name = pk->getName();
    pkInfo.porperties = Utils::getPKPropertiesNames(pk);
  }
  else
  {
    pkInfo = PKInfo();
  }

  delete transactor;
  delete catalog;
  delete dataSetType;

  return pkInfo;
}

std::vector<FKInfo> DSInfo::getForeignKeyInfo(std::string dataSetName)
{
  te::da::DataSourceTransactor* transactor = m_dataSource->getTransactor();
  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();
  te::da::DataSetType* dataSetType = catalog->getDataSetType(dataSetName, true);

  std::vector<FKInfo> vecFkInfo;

  unsigned int fkNumber = dataSetType->getNumberOfForeignKeys();

  for(unsigned int i = 0; i < fkNumber; i++)
  {
    te::da::ForeignKey* fk = dataSetType->getForeignKey(i);

    FKInfo fkInfo;
    fkInfo.name = fk->getName();
    fkInfo.porperties = Utils::getFKPropertiesNames(fk);

    vecFkInfo.push_back(fkInfo);
  }

  delete transactor;
  delete catalog;
  delete dataSetType;

  return vecFkInfo;
}

std::vector<UKInfo> DSInfo::getUniqueKeyInfo(std::string dataSetName)
{
  te::da::DataSourceTransactor* transactor = m_dataSource->getTransactor();
  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();
  te::da::DataSetType* dataSetType = catalog->getDataSetType(dataSetName, true);

  std::vector<UKInfo> vecUkInfo;

  unsigned int ukNumber = dataSetType->getNumberOfUniqueKeys();

  for(unsigned int i = 0; i < ukNumber; i++)
  {
    te::da::UniqueKey* uk = dataSetType->getUniqueKey(i);

    UKInfo ukInfo;
    ukInfo.name = uk->getName();
    ukInfo.porperties = Utils::getUKPropertiesNames(uk);

    vecUkInfo.push_back(ukInfo);
  }

  delete transactor;
  delete catalog;
  delete dataSetType;

  return vecUkInfo;
}

std::vector<IdxInfo> DSInfo::getIndexesKeyInfo(std::string dataSetName)
{
  te::da::DataSourceTransactor* transactor = m_dataSource->getTransactor();
  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();
  te::da::DataSetType* dataSetType = catalog->getDataSetType(dataSetName, true);

  std::vector<IdxInfo> vecIdxInfo;

  unsigned int idxNumber = dataSetType->getNumberOfIndexes();

  for(unsigned int i = 0; i < idxNumber; i++)
  {
    te::da::Index* idx = dataSetType->getIndex(i);

    IdxInfo idxInfo;
    idxInfo.name = idx->getName();    
    idxInfo.type = Utils::getIdxTypeName(idx->getIndexType());
    idxInfo.porperties = Utils::getIdxPropertiesNames(idx);

    vecIdxInfo.push_back(idxInfo);
  }

  delete transactor;
  delete catalog;
  delete dataSetType;

  return vecIdxInfo;
}

std::vector<CCInfo> DSInfo::getCheckConstraintInfo(std::string dataSetName)
{
  te::da::DataSourceTransactor* transactor = m_dataSource->getTransactor();
  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();
  te::da::DataSetType* dataSetType = catalog->getDataSetType(dataSetName, true);

  std::vector<CCInfo> vecCcInfo;

  unsigned int ccNumber = dataSetType->getNumberOfCheckConstraints();

  for(unsigned int i = 0; i < ccNumber; i++)
  {
    te::da::CheckConstraint* cc = dataSetType->getCheckConstraint(i);

    CCInfo ccInfo;
    ccInfo.name = cc->getName();    
    ccInfo.expression = cc->getExpression();

    vecCcInfo.push_back(ccInfo);
  }

  delete transactor;
  delete catalog;
  delete dataSetType;

  return vecCcInfo;
}

DataSetStruct DSInfo::getDataSetInfo(std::string dataSetName)
{
  DataSetStruct ds;

  te::da::DataSet* dataset = 0;
  dataset = Utils::getDataSet(m_dataSource, dataSetName);
  
  if(dataset)
  {
    ds.name = dataSetName;
    ds.pkInfo = getPrimaryKeyInfo(dataSetName);
    ds.hasPk = (!ds.pkInfo.name.empty() ? true : false);
    ds.vecFkInfo = getForeignKeyInfo(dataSetName);
    ds.vecUkInfo = getUniqueKeyInfo(dataSetName);
    ds.vecIdxInfo = getIndexesKeyInfo(dataSetName);
    ds.vecCcInfo = getCheckConstraintInfo(dataSetName);
  }
  else
  {
    ds = DataSetStruct();
  }    
 
  return ds;

}

std::vector<std::string*> DSInfo::getDataSetNameList()
{
  std::vector<std::string*> dataSetsNames;

  te::da::DataSourceTransactor* transactor = m_dataSource->getTransactor();

  te::da::DataSourceCatalogLoader* catalogLoader = transactor->getCatalogLoader();
  
  catalogLoader->getDataSets(dataSetsNames);

  delete transactor;
  delete catalogLoader;

  return dataSetsNames;
}

std::vector<DataSetStruct> DSInfo::getDataSetsInfo()
{
  std::vector<DataSetStruct> vecDataSetStruct;

  std::map<std::string, te::da::DataSet*>  dataSets;

  std::vector<std::string*> dataSetsNames;

  te::da::DataSourceTransactor* transactor = m_dataSource->getTransactor();

  te::da::DataSourceCatalogLoader* catalogLoader = transactor->getCatalogLoader();
  
  catalogLoader->getDataSets(dataSetsNames);

  for(std::vector<std::string*>::const_iterator it = dataSetsNames.begin(); it < dataSetsNames.end(); ++it)
  {
    const std::string* datasetName = *it;

    vecDataSetStruct.push_back(getDataSetInfo(*datasetName));
  }

  delete transactor;
  delete catalogLoader;

  return vecDataSetStruct;
}

DataSourceHeader DSInfo::getDataSourceHeader()
{
  DataSourceHeader dsh;

  std::string dsType = m_dataSource->getType();

  std::map<std::string, std::string> connInfo = m_dataSource->getConnectionInfo();  

  dsh.name = (!connInfo["dbname"].empty() ? connInfo["dbname"] : std::string());
  dsh.type = dsType;

  return dsh;
}

std::map<std::string, std::string> DSInfo::getCapabilities()
{
  std::map<std::string, std::string> capMap;

  m_dataSource->getCapabilities(capMap);
  
  if(capMap.empty())
    return std::map<std::string, std::string>();

  return capMap;
}

std::map<std::string, std::vector<std::string>> DSInfo::getDataSourceParameters(std::string dsType)
{
  std::map<std::string, std::vector<std::string>> mapParams;

  std::string errorMessage;
  Utils::loadModules(errorMessage);

  if(dsType.empty())
  { 
    te::da::DataSourceFactory::dictionary_type::const_iterator it = te::da::DataSourceFactory::getDictionary().begin();
    te::da::DataSourceFactory::dictionary_type::const_iterator itend =  te::da::DataSourceFactory::getDictionary().end();

    while(it != itend)
    {
      std::string datasourceType = it->first;

      const te::da::DataSourceFactory* factory = dynamic_cast<te::da::DataSourceFactory*>(it->second);
               
      std::vector<std::string> datasourceParams;
     
      factory->getConnectionParameters(datasourceParams);
       
      mapParams[datasourceType] = datasourceParams;

      ++it;
    }
  }
  else
  {
    dsType = te::common::Convert2UCase(dsType);
         
    const te::da::DataSourceFactory* factory = 0;
    
    factory = dynamic_cast<te::da::DataSourceFactory*>(te::da::DataSourceFactory::getDictionary().find(dsType));    

    if(factory)
    {
      std::vector<std::string> datasourceParams;
     
      factory->getConnectionParameters(datasourceParams);

      mapParams[dsType] = datasourceParams;
    }
    else
    {
      std::cout << std::endl << "Parameters Unknown!" << std::endl;
    }
  }

  return mapParams;
}