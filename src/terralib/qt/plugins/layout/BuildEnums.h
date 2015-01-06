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
  \file BuildEnums.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TE_QT_PLUGINS_LAYOUT_INTERNAL_BUILD_ENUM_H
#define __TE_QT_PLUGINS_LAYOUT_INTERNAL_BUILD_ENUM_H

namespace te
{
  namespace layout
  {
    class EnumDataType;
    class EnumObjectType;
    class EnumGridStyleType;
    class EnumLineStyleType;
    class EnumModeType;
    class EnumTemplateType;

    class BuildEnums
    {
      public:

        BuildEnums(); 

        virtual ~BuildEnums();
        
        virtual void build();

      protected:

    protected:

      EnumDataType* m_dataType;
      EnumObjectType* m_objType;
      EnumGridStyleType* m_gridStyleType;
      EnumLineStyleType* m_lineStyleType;
      EnumModeType* m_modeType;
      EnumTemplateType* m_templateType;
    };
  }
}

#endif
