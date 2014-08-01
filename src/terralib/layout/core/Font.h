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
  \file Font.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_FONT_H 
#define __TERRALIB_LAYOUT_INTERNAL_FONT_H

// TerraLib
#include "enum\AbstractType.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class Font 
    {
      public:

        /*!
         \brief 
        */
        Font();

        /*!
         \brief 
        */
        virtual ~Font();

        void setFamily(std::string family);
        std::string getFamily();

        void setPointSize(int point);
        int getPointSize();

        void setBold(bool bold);
        bool isBold();

        void setItalic(bool italic);
        bool isItalic();

        void setUnderline(bool underline);
        bool isUnderline();

        void setStrikeout(bool strikeout);
        bool isStrikeout();

        void setKerning(bool kerning);
        bool isKerning();

      protected:

        std::string m_family;
        int m_pointSize;
        bool m_bold;
        bool m_italic;
        bool m_underline;
        bool m_strikeout;
        bool m_kerning;   
        LayoutAlign m_textAlign;
    };
  }
}

#endif