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
  \file WriterFactory.cpp

  \brief This is the concrete factory for XML writer built on top of Xerces-C++ parser.
*/

// TerraLib
#include "Writer.h"
#include "WriterFactory.h"

te::xerces::WriterFactory* te::xerces::WriterFactory::sm_factory(0);

te::xerces::WriterFactory::~WriterFactory()
{
}

te::xml::AbstractWriter* te::xerces::WriterFactory::build()
{
  return new Writer;
}

te::xerces::WriterFactory::WriterFactory()
  : te::xml::AbstractWriterFactory(XERCES_DRIVER_IDENTIFIER)
{
}

void te::xerces::WriterFactory::initialize()
{
  finalize();
  sm_factory = new WriterFactory;
}

void te::xerces::WriterFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

