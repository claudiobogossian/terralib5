// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>

// STL
#include <iostream>

void PrintDataSetType(te::da::DataSet* /*ds*/)
{
//  if(ds == 0)
//  {
//    std::cout << "The informed data set is NULL!" << std::endl;
//    return;
//  }
//
//  te::da::DataSetType* type = ds->getType();
//  if(type == 0)
//  {
//    ds->loadTypeInfo();
//    type = ds->getType();    
//  }
// 
//// print data set type
//  std::cout << "Printing the data set schema..." << std::endl;
//
//  std::cout << "DataSet: " << type->getName() << std::endl;
//  std::cout << "\t" << "Number of attributes:" << type->size() << std::endl;
//  std::cout << "\t" << "Has PK:" << (type->getPrimaryKey() != 0) << std::endl;
//  std::cout << "\t" << "Number of UKs:" << type->getNumberOfUniqueKeys() << std::endl;
//  std::cout << "\t" << "Number of Indexes:" << type->getNumberOfIndexes() << std::endl;
//  std::cout << "\t" << "Number of Check-Constraints:" << type->getNumberOfCheckConstraints() << std::endl;
//  std::cout << std::endl;
//
//  for(unsigned int i=0; i<type->size(); ++i)
//  {
//    te::dt::Property* prop = type->getProperty(i);
//    std::cout << "Name: " << prop->getName() << std::endl;
//    std::cout << "Type: " << prop->getType() << std::endl;      
//  }
}
