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
  \file Enums.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Enums.h"

te::layout::Enums::Enums() :
  m_dataType(0),
  m_objType(0),
  m_gridStyleType(0),
  m_lineStyleType(0),
  m_modeType(0),
  m_templateType(0)
{

}

te::layout::Enums::~Enums()
{
  
}

void te::layout::Enums::setEnumDataType( EnumDataType* type )
{
  m_dataType = type;
}

te::layout::EnumDataType* te::layout::Enums::getEnumDataType()
{
  return m_dataType;
}

void te::layout::Enums::setEnumObjectType( EnumObjectType* type )
{
  m_objType = type;
}

te::layout::EnumObjectType* te::layout::Enums::getEnumObjectType()
{
  return m_objType;
}

void te::layout::Enums::setEnumGridStyleType( EnumGridStyleType* type )
{
  m_gridStyleType = type;
}

te::layout::EnumGridStyleType* te::layout::Enums::getEnumGridStyleType()
{
  return m_gridStyleType;
}

void te::layout::Enums::setEnumLineStyleType( EnumLineStyleType* type )
{
  m_lineStyleType = type;
}

te::layout::EnumLineStyleType* te::layout::Enums::getEnumLineStyleType()
{
  return m_lineStyleType;
}

void te::layout::Enums::setEnumModeType( EnumModeType* type )
{
  m_modeType = type;
}

te::layout::EnumModeType* te::layout::Enums::getEnumModeType()
{
  return m_modeType;
}

void te::layout::Enums::setEnumTemplateType( EnumTemplateType* type )
{
  m_templateType = type;
}

te::layout::EnumTemplateType* te::layout::Enums::getEnumTemplateType()
{
  return m_templateType;
}
