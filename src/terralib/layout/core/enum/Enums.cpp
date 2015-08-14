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
  \file Enums.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Enums.h"

te::layout::Enums::Enums() :
  m_dataType(new EnumDataType()),
  m_objType(new EnumObjectType()),
  m_gridStyleType(new EnumGridStyleType()),
  m_lineStyleType(new EnumLineStyleType()),
  m_modeType(new EnumModeType()),
  m_templateType(new EnumTemplateType()),
	m_toolType(new EnumToolType())
{

}

te::layout::Enums::~Enums()
{
	if (m_toolType)
	{
		delete m_toolType;
		m_toolType = 0;
	}
}

void te::layout::Enums::setEnumDataType( EnumDataType* type )
{
	if (m_dataType)
	{
		delete type;
		type = 0;
	}
  m_dataType = type;
}

te::layout::EnumDataType* te::layout::Enums::getEnumDataType()
{
  return m_dataType;
}

void te::layout::Enums::setEnumObjectType( EnumObjectType* type )
{
	if (m_objType)
	{
		delete m_objType;
		m_objType = 0;
	}
  m_objType = type;
}

te::layout::EnumObjectType* te::layout::Enums::getEnumObjectType()
{
  return m_objType;
}

void te::layout::Enums::setEnumGridStyleType( EnumGridStyleType* type )
{
	if (m_gridStyleType)
	{
		delete m_gridStyleType;
		m_gridStyleType = 0;
	}
  m_gridStyleType = type;
}

te::layout::EnumGridStyleType* te::layout::Enums::getEnumGridStyleType()
{
  return m_gridStyleType;
}

void te::layout::Enums::setEnumLineStyleType( EnumLineStyleType* type )
{
	if (m_lineStyleType)
	{
		delete m_lineStyleType;
		m_lineStyleType = 0;
	}
  m_lineStyleType = type;
}

te::layout::EnumLineStyleType* te::layout::Enums::getEnumLineStyleType()
{
  return m_lineStyleType;
}

void te::layout::Enums::setEnumModeType( EnumModeType* type )
{
	if (m_modeType)
	{
		delete m_modeType;
		m_modeType = 0;
	}
  m_modeType = type;
}

te::layout::EnumModeType* te::layout::Enums::getEnumModeType()
{
  return m_modeType;
}

void te::layout::Enums::setEnumTemplateType( EnumTemplateType* type )
{
	if (m_templateType)
	{
		delete m_templateType;
		m_templateType = 0;
	}
  m_templateType = type;
}

te::layout::EnumTemplateType* te::layout::Enums::getEnumTemplateType()
{
  return m_templateType;
}

te::layout::EnumToolType* te::layout::Enums::getEnumToolType()
{
	return m_toolType;
}


