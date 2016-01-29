// Examples
#include "DataAccessExamples.h"

// TerraLib

// STL
#include <iostream>
#include <exception>

std::auto_ptr<te::da::DataSource> GetPostGISConnection();

void QueryInsertExample()
{
// Create a simple DataSetType.
  std::string outName = "exampleInsert";
  std::auto_ptr<te::da::DataSetType> dsType(new te::da::DataSetType(outName));

  // Primary key
  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(outName + "_id", te::dt::INT32_TYPE);
  pkProperty->setAutoNumber(true);
  dsType->add(pkProperty);

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(outName + "_pk", dsType.get());
  pk->add(pkProperty);
  dsType->setPrimaryKey(pk);

  te::dt::SimpleProperty* region = new te::dt::SimpleProperty("region", te::dt::STRING_TYPE);
  dsType->add(region);

  te::gm::GeometryProperty* geomProp = new te::gm::GeometryProperty("geom");
  geomProp->setGeometryType(te::gm::MultiPolygonType);
  dsType->add(geomProp);
 
  try
  {
    std::auto_ptr<te::da::DataSource> ds = GetPostGISConnection();
    if (!ds.get())
      return;
    
    ds->open();

// get fields from output datasettype
    te::da::Fields* outFields = new te::da::Fields;
    std::vector<te::dt::Property*> outputProps = dsType->getProperties();
    
    for (std::size_t i = 0; i < outputProps.size(); ++i)
    {
      te::da::Field* field = new te::da::Field(outputProps[i]->getName());
      outFields->push_back(field);
    }

// get a transactor to interact to the data source
    std::auto_ptr<te::da::DataSourceTransactor> transactor =  ds->getTransactor();

    std::map<std::string, std::string> options;
    ds->createDataSet(dsType.get(), options);

    te::da::DataSetName* dsName = new te::da::DataSetName(outName);

    te::da::Fields* fields = new te::da::Fields;

    te::da::Field* f_gid = new te::da::Field("gid");
    fields->push_back(f_gid);

    te::da::Field* f_deno = new te::da::Field("deno");
    fields->push_back(f_deno);

    te::da::Field* f_geom = new te::da::Field("the_geom");
    fields->push_back(f_geom);
    
    te::da::From* from = new te::da::From;
    te::da::FromItem* fromItem = new te::da::DataSetName("distritos");
    from->push_back(fromItem);

    te::da::Select* select = new te::da::Select(fields, from);

    te::da::Insert insert(dsName, outFields, select);

    ds->execute(insert);

// At the end, let's release transactor and close data source
    delete transactor.release();
    ds->close();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in QueryInsertExample: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in QueryInsertExample!" << std::endl;
  }
}
