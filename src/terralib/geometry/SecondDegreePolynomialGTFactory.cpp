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
  \file terralib/geometry/SecondDegreePolynomialGTFactory.cpp

  \brief 2D Second Degree Polynomial Geometric transformation factory.
*/

// TerraLib
#include "SecondDegreePolynomialGTFactory.h"
#include "SecondDegreePolynomialGT.h"

te::gm::GTFactory* te::gm::SecondDegreePolynomialGTFactory::m_initializedFactoryPtr(0);

te::gm::SecondDegreePolynomialGTFactory::SecondDegreePolynomialGTFactory()
  : GTFactory( "SecondDegreePolynomial" )
{
}

te::gm::SecondDegreePolynomialGTFactory::~SecondDegreePolynomialGTFactory()
{
}

void te::gm::SecondDegreePolynomialGTFactory::initialize()
{
  finalize();
  m_initializedFactoryPtr = new SecondDegreePolynomialGTFactory();
}

void te::gm::SecondDegreePolynomialGTFactory::finalize()
{
  delete m_initializedFactoryPtr;
  m_initializedFactoryPtr = 0;
}

te::gm::GeometricTransformation* te::gm::SecondDegreePolynomialGTFactory::build()
{
  return new SecondDegreePolynomialGT();
}

