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
 \file terralib/metadata/MetadataAnnotation.cpp
 */

// TerraLib
#include "MetadataAnnotation.h"

// STL
#include <cassert>

// Boost
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

te::md::MetadataAnnotation::MetadataAnnotation(const std::string& aid):
  m_annotId(aid),
  m_annotURI(""),
  m_elementId(""),
  m_elementName("")
{
  if (m_annotId.empty())
  {
    boost::uuids::uuid tag = boost::uuids::random_generator()();
    m_annotId = boost::lexical_cast<std::string>(tag);
  }
}

te::md::MetadataAnnotation::~MetadataAnnotation()
{}

const std::string&
te::md::MetadataAnnotation::getAnnotationtId() const
{
  return m_annotId;
}

void 
te::md::MetadataAnnotation::setAnnotationURI(const std::string& uri)
{
  assert(!uri.empty());
  
  m_annotURI = uri;
}

const std::string& 
te::md::MetadataAnnotation::getAnnotationURI() const
{
  return m_annotURI;
}

void 
te::md::MetadataAnnotation::setElementId(const std::string& eid)
{
  assert(!eid.empty());
  
  m_elementId = eid;
}

const std::string&
te::md::MetadataAnnotation::getElementId() const
{
  return m_elementId;
}

void 
te::md::MetadataAnnotation::setElementName(const std::string& ename)
{
  assert(!ename.empty());
  
  m_elementName = ename;
}

const std::string& 
te::md::MetadataAnnotation::getElementName() const
{
  return m_elementName;
}
   