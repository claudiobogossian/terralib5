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
  \file EnumDataType.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_DATATYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_DATATYPE_H

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {
    class EnumType;

    class TELAYOUTEXPORT EnumDataType : public AbstractEnum
    {
      public:

        EnumDataType(); 

        virtual ~EnumDataType();

        virtual EnumType* getEnum(int enumId) const;

        virtual EnumType* getEnum(std::string name) const;

        virtual EnumType* getDataTypeNone() const;

        virtual EnumType* getDataTypeInt() const;

        virtual EnumType* getDataTypeDouble() const;

        virtual EnumType* getDataTypeLong() const;

        virtual EnumType* getDataTypeFloat() const;

        virtual EnumType* getDataTypeString() const;

        virtual EnumType* getDataTypeBool() const;

        virtual EnumType* getDataTypeColor() const;

        virtual EnumType* getDataTypeGridSettings() const;

        virtual EnumType* getDataTypeStringList() const;

        virtual EnumType* getDataTypeFont() const;

        virtual EnumType* getDataTypeImage() const;

        virtual EnumType* getDataTypeTextGridSettings() const;

      protected:

        virtual void init();

      protected:

        EnumType* m_dataTypeNone;
        EnumType* m_dataTypeInt;
        EnumType* m_dataTypeDouble;
        EnumType* m_dataTypeLong;
        EnumType* m_dataTypeFloat;
        EnumType* m_dataTypeString;
        EnumType* m_dataTypeBool;
        EnumType* m_dataTypeColor;
        EnumType* m_dataTypeGridSettings;
        EnumType* m_dataTypeStringList;
        EnumType* m_dataTypeFont;
        EnumType* m_dataTypeImage;
        EnumType* m_dataTypeTextGridSettings;
    };
  }
}

#endif
