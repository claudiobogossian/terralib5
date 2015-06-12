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
  \file STDataLoaderFromMemDSFactory.cpp

  \brief This is the concrete factory for a STDataLoaderFromMemDS.    
*/

// ST
#include "STDataLoaderFromMemDSFactory.h"
#include "STDataLoaderFromMemDS.h"
#include "../Globals.h"

te::st::STDataLoaderFromMemDSFactory* te::st::STDataLoaderFromMemDSFactory::sm_factory(0);

te::st::STDataLoaderFromMemDSFactory::~STDataLoaderFromMemDSFactory()
{
}

void te::st::STDataLoaderFromMemDSFactory::initialize()
{
  finalize();
  sm_factory = new STDataLoaderFromMemDSFactory;  
}

void te::st::STDataLoaderFromMemDSFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;

}

const std::string& te::st::STDataLoaderFromMemDSFactory::getType() const
{
  return te::st::Globals::sm_loaderFromMemDSIdentifier;
}

te::st::STDataLoaderImpl* te::st::STDataLoaderFromMemDSFactory::build()
{
  return new STDataLoaderFromMemDS();
}

te::st::STDataLoaderFromMemDSFactory::STDataLoaderFromMemDSFactory()
  : te::st::STDataLoaderImplFactory(te::st::Globals::sm_loaderFromMemDSIdentifier)
{
}

