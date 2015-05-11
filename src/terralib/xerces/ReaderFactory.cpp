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
  \file ReaderFactory.cpp

  \brief This is the concrete factory for XML reader built on top of Xerces-C++ parser.
*/

// TerraLib
#include "Reader.h"
#include "ReaderFactory.h"

te::xerces::ReaderFactory* te::xerces::ReaderFactory::sm_factory(0);

te::xerces::ReaderFactory::~ReaderFactory()
{
}

te::xml::Reader* te::xerces::ReaderFactory::build()
{
  return new Reader;
}

te::xerces::ReaderFactory::ReaderFactory()
  : te::xml::ReaderFactory(XERCES_DRIVER_IDENTIFIER)
{
}

void te::xerces::ReaderFactory::initialize()
{
  finalize();
  sm_factory = new ReaderFactory;
}

void te::xerces::ReaderFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

