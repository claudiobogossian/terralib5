// Examples
#include "DataAccessExamples.h"

te::da::PrimaryKey* AddPrimaryKey(const std::string& datasetname,
                                             te::da::DataSourceTransactor* transactor)
{
  //te::da::DataSetType* datasettype = (transactor->getDataSetType(datasetname)).get();  //"public.br_munic_2001");
  //te::dt::Property* gid_pk = datasettype->getProperty("gid"); 
   
  std::auto_ptr<te::dt::Property> gid_pk1 = transactor->getProperty(datasetname,"gid");
  //Create the name of primary key and add the attribute 
  te::da::PrimaryKey* pk = new te::da::PrimaryKey("attribute6_int_pk"); 
  pk->add(gid_pk1.get());

  transactor->addPrimaryKey(datasetname,pk);

  te::da::PrimaryKey* pk1 = (transactor->getPrimaryKey(datasetname)).get();

  return pk1;  
}

