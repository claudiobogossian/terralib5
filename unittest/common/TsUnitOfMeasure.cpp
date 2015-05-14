/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/UnitOfMeasure.cpp

  \brief A class for representing a unit of measure.
*/

#include "../../src/terralib/common/UnitOfMeasure.h"
#include "TsUnitOfMeasure.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TsUnitOfMeasure );

void TsUnitOfMeasure::setUp()
{
  uomA = new te::common::UnitOfMeasure(9030, "NAUTICAL MILE", "TestSymbolA", te::common::Length, 9001, 1852, 0, 0, 1, "TestDescriptionA");
  uomB = new te::common::UnitOfMeasure(9101, "RADIAN", "TestSymbolB", te::common::Angle, 9101, 1, 0, 0, 1, "TestDescriptionB");
  uomC = new te::common::UnitOfMeasure(9102, "DEGREE", "TestSymbolC", te::common::Angle, 9101, 3.14159265358979, 0, 0, 180, "TestDescriptionC");
  uomD = new te::common::UnitOfMeasure(9201, "UNITY", "TesSymbolD", te::common::Scale, 9201, 1, 0, 0, 1, "TestDescriptionD");
}

void TsUnitOfMeasure::tearDown()
{
  delete uomA;
  delete uomB;
  delete uomC;
  delete uomD;
}

void TsUnitOfMeasure::tcGetId()
{
  CPPUNIT_ASSERT(uomA->getId() == 9030);
}

void TsUnitOfMeasure::tcGetName()
{
  CPPUNIT_ASSERT(uomB->getName() == "RADIAN");
}

void TsUnitOfMeasure::tcGetDescription()
{
  CPPUNIT_ASSERT(uomC->getDescription() == "TestDescriptionC");
}

void TsUnitOfMeasure::tcGetSymbol()
{
   CPPUNIT_ASSERT(uomD->getSymbol() == "TesSymbolD");
}

void TsUnitOfMeasure::tcGetType()
{
  CPPUNIT_ASSERT(uomA->getType() == te::common::Length);
}

void TsUnitOfMeasure::tcIsBaseUnit()
{
  CPPUNIT_ASSERT(uomB->isBaseUnit() == true);
  CPPUNIT_ASSERT(uomC->isBaseUnit() == false);
}

void TsUnitOfMeasure::tcGetBaseUnitId()
{
  CPPUNIT_ASSERT(uomD->getBaseUnitId() == 9201);
}

void TsUnitOfMeasure::tcGetConversionFactors()
{
  double a, b, c, d;
  uomA->getConversionFactors(a, b, c, d);

  CPPUNIT_ASSERT(a == 1852);
  CPPUNIT_ASSERT(b == 0);
  CPPUNIT_ASSERT(c == 0);
  CPPUNIT_ASSERT(d == 1);
}

void TsUnitOfMeasure::tcGetConversionValue()
{
  CPPUNIT_ASSERT(uomA->getConversionValue() == 1852);
  CPPUNIT_ASSERT(uomB->getConversionValue() == 1);
}
