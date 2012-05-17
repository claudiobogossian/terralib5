/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file main.cpp

  \brief A list of examples for the TerraLib Spatial Reference Systems Module.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/srs.h>

// Examples
#include "SRSExamples.h"

// STL
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
// initialize Terralib support
  te::common::UnitOfMeasure* m = new te::common::UnitOfMeasure(0,te::common::UnitOfMeasure::LengthMeasure);
  m->setName("METER");
  m->setSymbol("m");
  std::vector<std::string> mNames;
  mNames.push_back("METRE");  
  te::common::UnitsOfMeasureManager::getInstance().insert(m,mNames);  

  te::common::UnitOfMeasure* km = new te::common::UnitOfMeasure(1,te::common::UnitOfMeasure::LengthMeasure);
  km->setName("KILOMETER");
  km->setSymbol("km");
  std::vector<std::string> kmNames;
  kmNames.push_back("KILOMETRE");
  km->setBaseUnitId(0);
  km->setConversionFactors(0.001);
  te::common::UnitsOfMeasureManager::getInstance().insert(km,kmNames);  

  te::common::UnitOfMeasure* rad = new te::common::UnitOfMeasure(2,te::common::UnitOfMeasure::AngleMeasure);
  rad->setName("RADIAN");
  rad->setSymbol("rad");
  te::common::UnitsOfMeasureManager::getInstance().insert(rad);  

  te::common::UnitOfMeasure* deg = new te::common::UnitOfMeasure(3,te::common::UnitOfMeasure::AngleMeasure);
  deg->setName("DEGREE");
  deg->setSymbol("deg");
  deg->setBaseUnitId(2);
  deg->setConversionFactors(.0174532925199432958); 
  te::common::UnitsOfMeasureManager::getInstance().insert(deg);  

// It initializes all the data source drivers (see LoadModule.cpp)
  try
  {
    TerraLib::getInstance().initialize();
    CoordinateSystemFactory();
    RecognizeSRIDs();
    ConvertCoordinates();
    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried: " << e.what() << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried!" << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}

