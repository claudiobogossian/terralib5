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
  \file Annotated.cpp

  \brief A base class for XSD classes that may allow annotation.
*/

// TerraLib
#include "Annotated.h"
#include "Annotation.h"

te::xsd::Annotation* te::xsd::Annotated::getAnnotation() const
{
  return m_annotation;
}

void te::xsd::Annotated::setAnnotation(Annotation* ann)
{
  delete m_annotation;
  m_annotation = ann;
}

te::xsd::Annotated::Annotated(Annotation* ann)
  : m_annotation(ann)
{
}

te::xsd::Annotated::Annotated(const Annotated& rhs)
  : m_annotation(0)
{
  m_annotation = rhs.m_annotation ? new Annotation(*rhs.m_annotation) : 0;
}

te::xsd::Annotated::~Annotated()
{
  delete m_annotation;
}

te::xsd::Annotated& te::xsd::Annotated::operator=(const Annotated& rhs)
{
  if(this != &rhs)
  {
    delete m_annotation;

    m_annotation = rhs.m_annotation ? new Annotation(*rhs.m_annotation) : 0;
  }

  return *this;
}

