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
  \file STDataLoaderFromDSFactory.cpp

  \brief This is the concrete factory for a STDataLoaderFromDS.  
*/

// ST
#include "STDataLoaderFromDSFactory.h"
#include "STDataLoaderFromDS.h"
#include "../Globals.h"

te::st::STDataLoaderFromDSFactory* te::st::STDataLoaderFromDSFactory::sm_factory(0);

te::st::STDataLoaderFromDSFactory::~STDataLoaderFromDSFactory()
{
}

void te::st::STDataLoaderFromDSFactory::initialize()
{
  finalize();
  sm_factory = new STDataLoaderFromDSFactory;
}

void te::st::STDataLoaderFromDSFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

const std::string& te::st::STDataLoaderFromDSFactory::getType() const
{
  return te::st::Globals::sm_loaderFromDSIdentifier;
}

te::st::STDataLoaderImpl* te::st::STDataLoaderFromDSFactory::build()
{
  return new STDataLoaderFromDS();
}

te::st::STDataLoaderFromDSFactory::STDataLoaderFromDSFactory()
  : te::st::STDataLoaderImplFactory(te::st::Globals::sm_loaderFromDSIdentifier)
{
}

