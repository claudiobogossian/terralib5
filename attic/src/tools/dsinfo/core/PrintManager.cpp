// Terralib Data Source Info tool
#include <PrintManager.h>

bool PrintManager::init(std::string dstype,std::string connstr)
{
  return m_dsi.init(dstype, connstr);
}

void PrintManager::printAllInfo(bool pkDetails, bool fkDetails, bool ukDetails, bool idxDetails, bool ccDetails)
{
  printHeader();
  printCapabilities();
  printCatalog(pkDetails, fkDetails, ukDetails, idxDetails, ccDetails);
}

void PrintManager::printDataSetNameList()
{
  printHeader();

  std::ostringstream output;

  std::vector<std::string*> names = m_dsi.getDataSetNameList();

  output << "DataSets:" << std::endl;

  for(unsigned int i = 0; i < names.size(); i++)
    output << "  " << *names[i] << std::endl;

  output << std::endl;

  std::cout << output.str() << std::endl;
}

void PrintManager::printHeader()
{

  std::ostringstream output;

  DataSourceHeader dsh = m_dsi.getDataSourceHeader();

  output << "DataSource:" << std::endl << std::endl;

  if(!dsh.name.empty())
  {
    output << "  " << dsh.name << std::endl;
  }

  output << "  " << dsh.type << std::endl;

  std::cout << output.str() << std::endl;

}

void PrintManager::printCatalog(bool pkDetails, bool fkDetails, bool ukDetails, bool idxDetails, bool ccDetails)
{
  std::vector<std::string*> vecDataSetName = m_dsi.getDataSetNameList();

  if(!vecDataSetName.empty())
  {

    std::cout << "Catalog:" << std::endl;
    std::cout << "--------" << std::endl << std::endl;

    for(unsigned int i = 0; i < vecDataSetName.size(); i++)
    {
      printDataSetInfo(*vecDataSetName[i], pkDetails, fkDetails, ukDetails, idxDetails, ccDetails);
    }
  }
  
}

void PrintManager::printCapabilities()
{
  std::ostringstream output;

  std::map<std::string, std::string> mapCap = m_dsi.getCapabilities();

  if(!mapCap.empty())
  {
    output << "Capabilities:" << std::endl << std::endl;

    std::map<std::string, std::string>::iterator it = mapCap.begin();

    while(it != mapCap.end())
    {
      output << "  " << it->first << " = " << it->second << std::endl;

      ++it;
    }

    std::cout << output.str() << std::endl;
  }
}

void PrintManager::printPKDetails(PKInfo pk)
{
  std::ostringstream output;

  output << "    " << pk.name << std::endl;

  for(unsigned int i = 0; i < pk.porperties.size(); i++)
  {
    output << "      " << pk.porperties[i] << std::endl;
  }

  std::cout << output.str();

}

void PrintManager::printFKDetails(FKInfo fk)
{
  std::ostringstream output;

  output << "    " << fk.name << std::endl;

  for(unsigned int i = 0; i < fk.porperties.size(); i++)
  {
    output << "      " << fk.porperties[i] << std::endl;
  }

  std::cout << output.str();
}

void PrintManager::printUKDetails(UKInfo uk)
{
  std::ostringstream output;

  output << "    " << uk.name << std::endl;

  for(unsigned int i = 0; i < uk.porperties.size(); i++)
  {
    output << "      " << uk.porperties[i] << std::endl;
  }

  std::cout << output.str();
}

void PrintManager::printIdxDetails(IdxInfo idx)
{
  std::ostringstream output;

  output << "    " << idx.name << " | " << idx.type << std::endl;

  for(unsigned int i = 0; i < idx.porperties.size(); i++)
  {
    output << "      " << idx.porperties[i] << std::endl;
  }

  std::cout << output.str();
}

void PrintManager::printCCDetails(CCInfo cc)
{
  std::ostringstream output;

  output << "    " << cc.name << std::endl;
  output << "      " << cc.expression << std::endl;

  std::cout << output.str();
}

void PrintManager::printDataSourceParameters(std::string dsType)
{
  std::ostringstream params;

  std::map<std::string, std::vector<std::string>> mapParams = m_dsi.getDataSourceParameters(dsType);

  std::map<std::string, std::vector<std::string>>::iterator it = mapParams.begin();

  params << "Data Source Parameters" << std::endl;
  params << "----------------------" << std::endl << std::endl;

  while(it != mapParams.end())
  {
    params << it->first << std::endl;

    for(unsigned int i = 0; i < it->second.size(); i++)
    {
      params << "  " << it->second[i] << std::endl;
    }

    params << std::endl;

    ++it;
  }

  std::cout << std::endl << params.str() << std::endl;
}

void PrintManager::printDataSetInfo(std::vector<std::string> dataSetNameList, bool pkDetails, bool fkDetails, bool ukDetails, bool idxDetails, bool ccDetails)
{
  for(unsigned int i = 0; i < dataSetNameList.size(); i++)
  {
    printDataSetInfo(dataSetNameList[i], pkDetails, fkDetails, ukDetails, idxDetails, ccDetails);
  }
}

void PrintManager::printDataSetInfo(std::string dataSetName, bool pkDetails, bool fkDetails, bool ukDetails, bool idxDetails, bool ccDetails)
{
  DataSetStruct ds = m_dsi.getDataSetInfo(dataSetName);

  std::cout << ds.name << std::endl;
      
  std::cout << "  Has Primary Key: " <<  (ds.hasPk ? "true" : "false") << std::endl;      
  if(pkDetails && ds.hasPk)
  {
    printPKDetails(ds.pkInfo);
  }
      
  std::cout << "  Number of Foreign Keys: " << ds.vecFkInfo.size() << std::endl; 
  if(fkDetails)
  {
    for(unsigned int j = 0; j < ds.vecFkInfo.size(); j++)
    {
      printFKDetails(ds.vecFkInfo[j]);
    }
  }

  std::cout << "  Number of Unique Keys: " <<  ds.vecUkInfo.size() << std::endl;
  if(ukDetails)
  {
    for(unsigned int j = 0; j < ds.vecUkInfo.size(); j++)
    {
      printUKDetails(ds.vecUkInfo[j]);
    }
  }

  std::cout << "  Number of Indexes: " <<  ds.vecIdxInfo.size() << std::endl;
  if(idxDetails)
  {
    for(unsigned int j = 0; j < ds.vecIdxInfo.size(); j++)
    {
      printIdxDetails(ds.vecIdxInfo[j]);
    }
  }

  std::cout << "  Number of Check Constraints: " <<  ds.vecCcInfo.size() << std::endl;
  if(ccDetails)
  {
    for(unsigned int j = 0; j < ds.vecCcInfo.size(); j++)
    {
      printCCDetails(ds.vecCcInfo[j]);
    }
  }

  std::cout << std::endl;
}