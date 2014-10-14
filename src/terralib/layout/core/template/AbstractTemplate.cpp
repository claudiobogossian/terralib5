/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file Template.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractTemplate.h"
#include "../enum/EnumType.h"
#include "../enum/Enums.h"

// STL
#include <vector>

te::layout::AbstractTemplate::AbstractTemplate(std::string path) :
  m_path(path),
  m_type(0)
{
  unsigned found = path.find_last_of("/\\");
  m_fileName = path.substr(found + 1);

  m_type = Enums::getInstance().getEnumTemplateType()->getNoneType();
}

te::layout::AbstractTemplate::~AbstractTemplate()
{

}

std::string te::layout::AbstractTemplate::getFileName()
{
  return m_fileName;
}

te::layout::EnumType* te::layout::AbstractTemplate::getType()
{
  return m_type;
}
