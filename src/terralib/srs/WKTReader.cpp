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
  \file WKTReader.cpp

  \brief A class that deserializes a SRS from a valid WKT.
 */

// TerraLib
#include "../common/Translator.h"
#include "Exception.h"
#include "WKTReader.h"

te::srs::SpatialReferenceSystem* te::srs::WKTReader::read(const char* wkt)
{
  WKTParser<std::string::const_iterator> parser;

  const std::string s(wkt);

  std::string::const_iterator beginS = s.begin();
  std::string::const_iterator endS = s.end();
  
  bool isValid = phrase_parse(beginS, endS, parser, boost::spirit::ascii::space);

  if(!isValid)
    throw Exception(TE_TR("Invalid SRS WKT - parsing failed!"));

  te::srs::SpatialReferenceSystem* srs = parser.getSRS();

  return srs;
}




