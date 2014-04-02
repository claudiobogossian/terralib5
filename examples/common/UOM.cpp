#include "CommonExamples.h"

// TerraLib
#include <terralib/srs.h>

// STL
#include <iostream>

void UOMManage()
{
  try
  {
    // list all units in the manager
    te::common::UnitsOfMeasureManager::const_iterator it = te::common::UnitsOfMeasureManager::getInstance().begin();
    te::common::UnitsOfMeasureManager::const_iterator itend = te::common::UnitsOfMeasureManager::getInstance().end();
    std::cout << "Units in the manager:"<< std::endl;
    while (it!=itend)
    {
      te::common::UnitOfMeasurePtr uptr = it->second;
      std::cout << "\t Id:" << uptr->getId() << " name: " << uptr->getName() << " symbol: " << uptr->getSymbol() << std::endl;
      ++it;
    }
    
    // searching by name and displaying information about unit
    te::common::UnitOfMeasurePtr munit = te::common::UnitsOfMeasureManager::getInstance().find("metre");
    if (munit.get())
      std::cout << "Unit \"metre\" found."<< std::endl;
    else
      return;
    
    unsigned int id = munit->getId();
    std::string symb = munit->getSymbol();
    
    munit = te::common::UnitsOfMeasureManager::getInstance().find(id);
    if (munit.get())
      std::cout << "Unit " << id << " found."<< std::endl;
    else
      return;
    
    munit = te::common::UnitsOfMeasureManager::getInstance().findBySymbol(symb);
    if (munit.get())
      std::cout << "Unit " << symb << " found."<< std::endl;
    else
      return;
    
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

void UnitSRS()
{
  try
  {
    std::cout << "Geographic:" << std::endl;
    te::common::UnitOfMeasurePtr uptr = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(TE_SRS_SAD69);
    if(uptr)
      std::cout << "\t Id:" << uptr->getId() << " name: " << uptr->getName() << " symbol: " << uptr->getSymbol() << std::endl;
    
    std::cout << "Projected:" << std::endl;
    uptr = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(TE_SRS_SAD69_UTM_ZONE_23S);
    if(uptr)
      std::cout << "\t Id:" << uptr->getId() << " name: " << uptr->getName() << " symbol: " << uptr->getSymbol() << std::endl;
  }
  catch (te::common::Exception& ex)
  {
    std::cout << ex.what() << std::endl;
  }
}

