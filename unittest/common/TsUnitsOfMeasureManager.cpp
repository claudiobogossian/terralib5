/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

// TerraLib
#include "TsUnitsOfMeasureManager.h"

// STL
#include <string.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsUnitsOfMeasureManager );

void TsUnitsOfMeasureManager::setUp()
{
  uomA = te::common::UnitOfMeasurePtr(new te::common::UnitOfMeasure(9030, "NAUTICAL MILE", "", te::common::Length, 9001, 1852, 0, 0, 1, ""));
  uomB = te::common::UnitOfMeasurePtr(new te::common::UnitOfMeasure(9101, "RADIAN", "", te::common::Angle, 9101, 1, 0, 0, 1, ""));
  uomC = te::common::UnitOfMeasurePtr(new te::common::UnitOfMeasure(9102, "DEGREE", "", te::common::Angle, 9101, 3.14159265358979, 0, 0, 180, ""));
  uomD = te::common::UnitOfMeasurePtr(new te::common::UnitOfMeasure(9201, "UNITY", "", te::common::Scale, 9201, 1, 0, 0, 1, ""));

  std::string altNameA = "AltNameA";
  std::string altNameB = "AltNameB";

  alternativeNames.push_back(altNameA);
  alternativeNames.push_back(altNameB);

  te::common::UnitsOfMeasureManager::getInstance().insert(uomA);
  te::common::UnitsOfMeasureManager::getInstance().insert(uomB); 
  te::common::UnitsOfMeasureManager::getInstance().insert(uomC);
  te::common::UnitsOfMeasureManager::getInstance().insert(uomD, alternativeNames);
}

void TsUnitsOfMeasureManager::tearDown()
{
  te::common::UnitsOfMeasureManager::getInstance().clear();
}

/*
void TsUnitsOfMeasureManager::tcFindUomById ()
{
  CPPUNIT_ASSERT(uomB  ==   te::common::UnitsOfMeasureManager::getInstance().findById(9101));
}
*/

void TsUnitsOfMeasureManager::tcFindUomByName ()
{
  CPPUNIT_ASSERT(uomC == te::common::UnitsOfMeasureManager::getInstance().find("degree"));
}

void TsUnitsOfMeasureManager::tcFindUomByAltName ()
{
  CPPUNIT_ASSERT(uomD == te::common::UnitsOfMeasureManager::getInstance().find("AltNameA"));
}

void TsUnitsOfMeasureManager::tcFindUomByNames ()
{
  CPPUNIT_ASSERT(uomD == te::common::UnitsOfMeasureManager::getInstance().find("AltNameB"));
  CPPUNIT_ASSERT(uomD == te::common::UnitsOfMeasureManager::getInstance().find("unity"));
}

void TsUnitsOfMeasureManager::tcGeAltNames ()
{
  std::vector<std::string> namesOutput;
  te::common::UnitsOfMeasureManager::getInstance().getNames(uomD, namesOutput);

  CPPUNIT_ASSERT(namesOutput.size() == alternativeNames.size());

  for(std::size_t i = 0; i < namesOutput.size(); ++i)
  {
    CPPUNIT_ASSERT(namesOutput[i].compare(alternativeNames[i]));
  }
}

void TsUnitsOfMeasureManager::tcConvert ()
{
  CPPUNIT_ASSERT(te::common::UnitsOfMeasureManager::getInstance().getConversion("DEGREE", "RADIAN") == 0.017453292519943278);
}