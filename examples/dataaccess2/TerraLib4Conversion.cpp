// Examples
#include "DataAccessExamples.h"

#include <terralib/dataaccess.h>
#include <terralib/dataaccess/dataset/DataSetTypeConverter.h>

void TerraLib4Converter()
{
  try
  {
    std::map<std::string, std::string> connInfo;
    connInfo["T4_DRIVER"] = "Ado";
    connInfo["T4_DB_NAME"] = "C:/temp/TL4SemRaster.mdb";

    std::auto_ptr<te::da::DataSource> tl4Database(te::da::DataSourceFactory::make("TERRALIB4"));
    tl4Database->setConnectionInfo(connInfo);
    tl4Database->open();

    std::map<std::string, std::string> adoConnInfo;
    adoConnInfo["PROVIDER"] = "Microsoft.Jet.OLEDB.4.0";
    adoConnInfo["DB_NAME"] = "C:/temp/ExampleTerralib4.mdb";
    adoConnInfo["CREATE_OGC_METADATA_TABLES"] = "TRUE";

    if(te::da::DataSource::exists("ADO", adoConnInfo))
    {
      te::da::DataSource::drop("ADO", adoConnInfo);
    }

    std::auto_ptr<te::da::DataSource> tl5Database(te::da::DataSource::create("ADO", adoConnInfo));
    tl5Database->open();

    std::vector<std::string> dsNames = tl4Database->getDataSetNames();

    for(std::size_t i = 0; i < dsNames.size(); ++i)
    {
      std::auto_ptr<te::da::DataSetType> dst = tl4Database->getDataSetType(dsNames[i]);

      std::auto_ptr<te::da::DataSetTypeConverter> dt_adapter(new te::da::DataSetTypeConverter(dst.get(), tl5Database->getCapabilities()));

      std::auto_ptr<te::da::DataSet> ds(tl4Database->getDataSet(dsNames[i]));

      std::auto_ptr<te::da::DataSetAdapter> ds_adapter(te::da::CreateAdapter(ds.get(), dt_adapter.get()));

      std::map<std::string, std::string> op;

      te::da::Create(tl5Database.get(), dt_adapter->getResult(), ds_adapter.get(), op);
    }
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in Terralib4 Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in Terralib4 Example!" << std::endl;
  }
}

