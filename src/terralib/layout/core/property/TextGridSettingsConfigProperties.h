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
  \file TextGridSettingsConfigProperties.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXTGRID_SETTINGS_CONFIG_PROPERTIES_H
#define __TERRALIB_LAYOUT_INTERNAL_TEXTGRID_SETTINGS_CONFIG_PROPERTIES_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class TELAYOUTEXPORT TextGridSettingsConfigProperties
    {
      public:

        TextGridSettingsConfigProperties();

        virtual ~TextGridSettingsConfigProperties();
        
        std::string getName();
        std::string getTitle();
        std::string getSpacing();
        std::string getPadding();
        std::string getTableColor();
        std::string getWidthConstraints();
        std::string getWidth();
                      
        std::string getAddRow();
        std::string getRowNumber();
        std::string getEvenRow();
        std::string getOddRow();

        std::string getAddColunm();
        std::string getColumnNumber();
        std::string getColumnWidth();
        std::string getColumnWidthConstraints();

        std::string getBorderWidth();
        std::string getBorderGridColor();
        std::string getBorderStyle();

        std::string getHeaderAlignment();
        std::string getHeaderHorizontalColor();
        std::string getHeaderVerticalColor();

        std::string getCellAlignment();

      protected:

        /* Text Table Settings */

        std::string m_name;
        std::string m_title;

        /* General */

        std::string m_spacing;
        std::string m_padding;
        std::string m_tableColor;
        std::string m_widthConstraints;
        std::string m_width;

        /* Rows */
        std::string m_addRow;
        std::string m_rowNumber;
        std::string m_evenRow;
        std::string m_oddRow;

        /* Columns */

        std::string m_addColumn;
        std::string m_columnNumber;
        std::string m_columnWidth;
        std::string m_columnWidthConstraints;

        /* Border */

        std::string m_borderWidth;
        std::string m_borderGridColor;
        std::string m_borderStyle;

        /* Header */

        std::string m_headerAlignment; 
        std::string m_headerHorizontalColor;
        std::string m_headerVerticalColor;

        /* Cell */

        std::string m_cellAlignment;
    };
  }
}

#endif
