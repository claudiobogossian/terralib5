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
  \file BuildEnums.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildEnums.h"
#include "../../../layout/core/enum/EnumDataType.h"
#include "../../../layout/core/enum/EnumObjectType.h"
#include "../../../layout/core/enum/EnumGridStyleType.h"
#include "../../../layout/core/enum/EnumLineStyleType.h"
#include "../../../layout/core/enum/EnumModeType.h"
#include "../../../layout/core/enum/EnumTemplateType.h"
#include "../../../layout/core/enum/Enums.h"

te::layout::BuildEnums::BuildEnums() :
  m_dataType(0),
  m_objType(0),
  m_gridStyleType(0),
  m_lineStyleType(0),
  m_modeType(0),
  m_templateType(0)
{

}

te::layout::BuildEnums::~BuildEnums()
{
  if(m_dataType)
  {
    delete m_dataType;
    m_dataType = 0;
  }

  if(m_objType)
  {
    delete m_objType;
    m_objType = 0;
  }

  if(m_gridStyleType)
  {
    delete m_gridStyleType;
    m_gridStyleType = 0;
  }

  if(m_lineStyleType)
  {
    delete m_lineStyleType;
    m_lineStyleType = 0;
  }

  if(m_modeType)
  {
    delete m_modeType;
    m_modeType = 0;
  }

  if(m_templateType)
  {
    delete m_templateType;
    m_templateType = 0;
  }
}

void te::layout::BuildEnums::build()
{
  if(!Enums::getInstance().getEnumDataType())
  {
    m_dataType = new EnumDataType;
    Enums::getInstance().setEnumDataType(m_dataType);
  }

  if(!Enums::getInstance().getEnumObjectType())
  {
    m_objType = new EnumObjectType;
    Enums::getInstance().setEnumObjectType(m_objType);
  }

  if(!Enums::getInstance().getEnumGridStyleType())
  {
    m_gridStyleType = new EnumGridStyleType;
    Enums::getInstance().setEnumGridStyleType(m_gridStyleType);
  }

  if(!Enums::getInstance().getEnumLineStyleType())
  {
    m_lineStyleType = new EnumLineStyleType;
    Enums::getInstance().setEnumLineStyleType(m_lineStyleType);
  }

  if(!Enums::getInstance().getEnumModeType())
  {
    m_modeType = new EnumModeType;
    Enums::getInstance().setEnumModeType(m_modeType);
  }

  if(!Enums::getInstance().getEnumTemplateType())
  {
    m_templateType = new EnumTemplateType;
    Enums::getInstance().setEnumTemplateType(m_templateType);
  }
}
