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
  \file AbstractEnum.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractEnum.h"


te::layout::AbstractEnum::AbstractEnum()
{

}

te::layout::AbstractEnum::~AbstractEnum()
{

}

te::layout::EnumType* te::layout::AbstractEnum::getEnum( int enumId ) const
{
  EnumType* enumTp = 0;

  for(std::vector<EnumType*>::const_iterator it = m_enums.begin(); it != m_enums.end(); it++)
  {
    if((*it)->getId() == enumId)
    {
      enumTp = (*it);
      break;
    }
  }

  return enumTp;
}

te::layout::EnumType* te::layout::AbstractEnum::getEnum( std::string name ) const
{
  EnumType* enumTp = 0;

  for(std::vector<EnumType*>::const_iterator it = m_enums.begin(); it != m_enums.end(); it++)
  {
    if((*it)->getName().compare(name) == 0)
    {
      enumTp = (*it);
      break;
    }
  }

  return enumTp;
}
