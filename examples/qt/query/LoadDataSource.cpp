#include "QueryExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>

// STL
#include <cassert>

te::da::DataSourcePtr LoadShapeDataSource(const std::string& fileName, const std::string& dsId)
{
  //shape
  std::string connInfo ("File://" + fileName);
  
  te::da::DataSource* dsShape = te::da::DataSourceFactory::make("OGR", connInfo).get();
  dsShape->open();
  dsShape->setId(dsId);

  te::da::DataSourcePtr dsPtrShape(dsShape);

  te::da::DataSourceManager::getInstance().insert(dsPtrShape);

  return dsPtrShape;
}

te::da::DataSourcePtr LoadPGISDataSource(const std::string& dsId)
{
  // let's give the minimal server connection information needed to connect to the database server
  std::string aux, user, password, host, port, path, query;
  std::string strURI = "pgsql://"; // The base of the URI

  std::cout << "Inform the Host for your postGIS server (ENTER to accept default \'atlas.dpi.inpe.br\'): ";
  std::getline(std::cin, aux);
  host = aux.empty() ? "atlas.dpi.inpe.br" : aux;

  std::cout << "Inform the Port number to access your postGIS server (ENTER to accept default \'5433\'): ";
  std::getline(std::cin, aux);
  port = aux.empty() ? "5433" : aux;

  std::cout << "Inform the User to access your postGIS server (ENTER to accept default \'postgres\'): ";
  std::getline(std::cin, aux);
  user = aux.empty() ? "postgres" : aux;

  std::cout << "Inform the Password to access your postGIS server (ENTER to accept default \'postgres\'): ";
  std::getline(std::cin, aux);
  password = aux.empty() ? "postgres" : aux;

  std::cout << "Inform the Database name to connect to your postGIS server (ENTER to accept default \'terralib4\'): ";
  std::getline(std::cin, aux);
  path = aux.empty() ? "terralib4" : aux;

  std::cout << "Inform the Client enconding to connect to your postGIS server (ENTER to accept default \'UTF-8\'): ";
  std::getline(std::cin, aux);
  query = aux.empty() ? "&PG_CLIENT_ENCODING=" + te::core::CharEncoding::getEncodingName(te::core::EncodingType::UTF8) : aux;

  std::cout << "Inform the Connection Time Out to connect to your postGIS server (ENTER to accept default \'4\'): ";
  std::getline(std::cin, aux);
  query += aux.empty() ? "&PG_CONNECT_TIMEOUT=4" : "&PG_CONNECT_TIMEOUT=" + aux;

  strURI += user + ":";
  strURI += password + "@";
  strURI += host + ":";
  strURI += port + "/";
  strURI += path + "?";
  strURI += query;

  // create a data source using the data source factory
  std::unique_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("POSTGIS", strURI);

  try
  {
    ds->open();
  }
  catch (const std::exception& e)
  {
    std::cout << "Datasource " << host << "/" << path << " can not be used!\nMake sure to have the correct connection parameters\n";
    std::cout << "Error: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cout << "Datasource " << host << "/" << path << " can not be used!\nMake sure to have the correct connection parameters\n";
  }

  ds->setId(dsId);
  te::da::DataSourcePtr dsPtrPGIS(ds.release());
  te::da::DataSourceManager::getInstance().insert(dsPtrPGIS);

  return dsPtrPGIS;
}

te::da::DataSet* GetDataSet(te::da::Select& s, te::da::DataSourcePtr& ds)
{
  te::da::DataSourceTransactor* transactor = ds->getTransactor().get();

  te::da::DataSet* dataset = transactor->query(s).get();

  delete transactor;

  return dataset;
}

