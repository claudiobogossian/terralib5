

#include <iostream>
#include <string>
#include <vector>

#include "modis_database.h"

void mmain(std::string& rootpath)
{
  if (!boost::filesystem::is_directory(rootpath))
  {
    std::cout << "Enter with a valid root directory!" << std::endl;
    return;
  }
  
  MODISDatabase myDb(rootpath);
  
  std::cout << "There are " << myDb.size() << " file MODIS in " << rootpath << ":" << std::endl;
  
  if (myDb.size() == 0)
    return;
  
  myDb.printAll();
  
  std::cout << std::endl << std::endl << "Search by product. Enter with a product name: " ;
  std::string prod;
  std::cin >> prod;

  if (!prod.empty())
  {
    std::vector<MODISRecord> result = myDb.findByProduct(prod);
    if (result.empty())
      std::cout << "None found!" << std::endl;
    else
    {
      BOOST_FOREACH(MODISRecord& rec, result)
      {
        std::cout << "\t";
        rec.printToCout();
      }
    }
  }
  
  std::cout << std::endl << std::endl << "Search by year. Enter with a year (> 2000): " ;
  unsigned int year = 0;
  std::cin >> year;
  
  if (year > 2000)
  {
    std::vector<MODISRecord> result = myDb.findByYear(year);
    if (result.empty())
      std::cout << "None found!" << std::endl;
    else
    {
      BOOST_FOREACH(MODISRecord& rec, result)
      {
        std::cout << "\t";
        rec.printToCout();
      }
    }
  }
  else
    std::cout << "Invalid year!" << std::endl;


  std::cout << std::endl << std::endl << "Search by day. Enter with a day (1-365): " ;
  unsigned int day = 0;
  std::cin >> day;
  
  if (day <= 365 || day >=
      1)
  {
    std::vector<MODISRecord> result = myDb.findByDay(day);
    if (result.empty())
      std::cout << "None found!" << std::endl;
    else
    {
      BOOST_FOREACH(MODISRecord& rec, result)
      {
        std::cout << "\t";
        rec.printToCout();
      }
    }
  }
}