/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file SQLDialect.cpp
   
  \brief Auxiliary classes and functions to read SQL dialect information from a XML document.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/ReaderFactory.h"
#include "../../dataaccess/query/BinaryOpEncoder.h"
#include "../../dataaccess/query/FunctionEncoder.h"
#include "../../dataaccess/query/SQLDialect.h"
#include "../../dataaccess/query/SQLFunctionEncoder.h"
#include "../../dataaccess/query/TemplateEncoder.h"
#include "../../dataaccess/query/UnaryOpEncoder.h"
#include "../Exception.h"
#include "SQLDialect.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/format.hpp>

te::da::SQLDialect* te::serialize::ReadDialect(const std::string& dialectFileName)
{
  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());

  xmlReader->read(dialectFileName);

  if(!xmlReader->next())
    throw Exception((boost::format(TR_DATAACCESS("Could not read query dialect information in file: %1%.")) % dialectFileName).str());

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw Exception((boost::format(TR_DATAACCESS("Error reading the document %1%, the start element wasn't found.")) % dialectFileName).str());

  if(xmlReader->getElementLocalName() != "QueryDialect")
    throw Exception((boost::format(TR_DATAACCESS("The first tag in the document %1% is not 'QueryDialect'.")) % dialectFileName).str());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "DriverName");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);
  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Functions");

  std::auto_ptr<te::da::SQLDialect> mydialect(new te::da::SQLDialect);

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {
    std::string fname = xmlReader->getAttr(0);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);

    std::string encoderType = xmlReader->getElementLocalName();

    te::da::SQLFunctionEncoder* sfe = 0;

    if(encoderType == "FunctionEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::FunctionEncoder(alias);
    }
    else if(encoderType == "BinaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::BinaryOpEncoder(alias);
    }
    else if(encoderType == "UnaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::UnaryOpEncoder(alias);
    }
    else
    {
      throw Exception(TR_DATAACCESS("Unsupported encoder type!"));
    }

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // End of Encoder

    mydialect->insert(fname, sfe);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // End of Function
  }

  assert(xmlReader->getNodeType() == te::xml::END_DOCUMENT); // End of QueryDialect

  return mydialect.release();
}
