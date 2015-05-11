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
  \file terralib/geometry/RSTGTFactory.cpp

  \brief 2D RST Geometric transformation factory.
*/

// TerraLib
#include "RSTGTFactory.h"
#include "RSTGT.h"

te::gm::GTFactory* te::gm::RSTGTFactory::m_initializedFactoryPtr(0);

te::gm::RSTGTFactory::RSTGTFactory()
  : GTFactory( "RST" )
{
}

te::gm::RSTGTFactory::~RSTGTFactory()
{
}

void te::gm::RSTGTFactory::initialize()
{
  finalize();
  m_initializedFactoryPtr = new RSTGTFactory();
}

void te::gm::RSTGTFactory::finalize()
{
  delete m_initializedFactoryPtr;
  m_initializedFactoryPtr = 0;
}

te::gm::GeometricTransformation* te::gm::RSTGTFactory::build()
{
  return new RSTGT();
}

