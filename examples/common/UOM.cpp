#include "UOM.h"

#include <terralib/common.h>

#include <iostream>

void UOMManage()
{
  try {
      // searching by name and displaying information about unit
    te::common::UnitOfMeasure* munit = te::common::UnitsOfMeasureManager::getInstance().find("metre");
    std::cout << "Searching \"metre\": " << munit->getId() << ", " << munit->getSymbol() << ", " << munit->getName() << ", "  << munit->getDescription();
    
    std::vector<std::string> altnames;
    te::common::UnitsOfMeasureManager::getInstance().getAlternativeNames(munit,altnames);
    if (!altnames.empty())
    {
      for (size_t i=0; i<altnames.size(); ++i)
        std::cout << " , " << altnames[i];
      std::cout << std::endl;
    }  
      // converting from units
    std::cout << "1 km = " << te::common::UnitsOfMeasureManager::getInstance().getConversion("kilometre","metre") << " m." << std::endl;
    std::cout << "1  m = " << te::common::UnitsOfMeasureManager::getInstance().getConversion("metre","kilometre") << " km." << std::endl;
    
    std::cout << "1 deg = " << te::common::UnitsOfMeasureManager::getInstance().getConversion("degree","radian") << " rad." << std::endl;
    std::cout << "1 rad = " << te::common::UnitsOfMeasureManager::getInstance().getConversion("radian","degree") << " deg." << std::endl;
  
    std::cout << "1 km = " << te::common::UnitsOfMeasureManager::getInstance().getConversion("kilometre","radian") << "rad." << std::endl;
    
  }
  catch (te::common::Exception& ex) 
  {
    std::cout << ex.what() << std::endl;
  }
}


