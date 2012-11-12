// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/memory.h>

// Examples
#include "DataAccessExamples.h"

// STL
#include <iostream>

void AddingDataSetPropertyValues(te::da::DataSet* ds)
{
  try
  {  
    //============= (1) Add a new property in the data set type
    
    //first of all, we have to verify if the dataSetType associated to 
    //'ds' has enough information, such as primary key. If not, we
    // have to load this information. 
    if(!ds->getType()->isFullLoaded()) 
      ds->loadTypeInfo();
    
    te::da::DataSetType* dt = ds->getType(); 
    te::da::DataSourceTransactor* transactor = ds->getTransactor();
    
    //create a new property 
    te::dt::StringProperty* prop01 = new te::dt::StringProperty("description", te::dt::VAR_STRING, 200);
        
    //add it into data set type
    te::da::DataSetTypePersistence* dstPersistence = transactor->getDataSetTypePersistence();
    
    dstPersistence->add(dt, prop01);
    
    delete dstPersistence;
        
    //============= (2) Create a dataset in memory and populate it with the new property  
    
    //create a data set in memory only with primary key and the new property
    std::vector<te::dt::Property*> properties;
    
    //primary key
    te::da::PrimaryKey* pKey = dt->getPrimaryKey();
    std::size_t numProps = pKey->getProperties().size();
    for(std::size_t i=0; i<numProps; ++i)
      properties.push_back(pKey->getProperties()[i]);
          
    //create a memory data set
    te::mem::DataSet* memDs = new te::mem::DataSet(*ds, properties); 
      
    //add the new property in the data set in memory
    te::dt::Property* newP = prop01->clone();
    memDs->add(newP); 
        
    memDs->moveBeforeFirst();
    while(memDs->moveNext())
      memDs->setString(newP->getName(), "teste...."); //description
      
    te::da::DataSetPersistence* dsPersistence = transactor->getDataSetPersistence();   
    
    std::vector<te::dt::Property*> props;
    props.push_back(newP);
    
    memDs->moveBeforeFirst();
    const std::map<std::string, std::string> options;
    dsPersistence->update(dt, memDs, props, options);

    delete memDs;
    delete dsPersistence; 
    return;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in Adding DataSet Property Values Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in Adding DataSet Property Values Example!" << std::endl;
  }
}


