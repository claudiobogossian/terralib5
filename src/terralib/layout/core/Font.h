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
  \file Font.h
   
  \brief Class specifies a font.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_FONT_H 
#define __TERRALIB_LAYOUT_INTERNAL_FONT_H

// TerraLib
#include "enum/AbstractType.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    /*!
      \brief Class specifies a font.
    
      \ingroup layout
    */
    class Font 
    {
      public:

        /*!
         \brief Constructor
        */
        Font();

        /*!
         \brief Destructor
        */
        virtual ~Font();

        /*!
          \brief Returns font family name.
      
          \param font family name       
        */ 
        void setFamily(std::string family);

        /*!
          \brief Sets font family name.
      
          \return font family name       
        */
        std::string getFamily();

        /*!
          \brief Sets point size of the font.
      
          \param point size of the font      
        */
        void setPointSize(int point);

        /*!
          \brief Returns point size of the font.
      
          \return point size of the font       
        */
        int getPointSize();

        /*!
          \brief Sets font with bold style.
      
          \param true if font use bold, false otherwise
        */
        void setBold(bool bold);

        /*!
          \brief Returns true if font use bold, false otherwise  
      
          \return true if font use bold, false otherwise      
        */
        bool isBold();

        /*!
          \brief Sets font with italic style.
      
          \param true if font use italic, false otherwise      
        */
        void setItalic(bool italic);

        /*!
          \brief Returns true if font use italic, false otherwise  
      
          \return true if font use italic, false otherwise        
        */
        bool isItalic();

        /*!
          \brief Sets font with underline style.
      
          \param true if font use underline, false otherwise        
        */
        void setUnderline(bool underline);

        /*!
          \brief Returns true if font use underline, false otherwise  
      
          \return true if font use underline, false otherwise       
        */
        bool isUnderline();

        /*!
          \brief Sets font with strikeout style.
      
          \param true if font use strikeout, false otherwise      
        */
        void setStrikeout(bool strikeout);

        /*!
          \brief Returns true if font use strikeout, false otherwise  
      
          \return true if font use strikeout, false otherwise      
        */
        bool isStrikeout();

        /*!
          \brief Sets font with kerning style.
      
          \param true if font use kerning, false otherwise        
        */
        void setKerning(bool kerning);

        /*!
          \brief Returns true if font use kerning, false otherwise  
      
          \return true if font use kerning, false otherwise          
        */
        bool isKerning();

        /*!
          \brief Serialize font object.
      
          \return font object serialized       
        */
        virtual std::string toString() const;

        /*!
          \brief Sets this object state from a string.
      
          \param font object serialized        
        */
        virtual void fromString(std::string font);

      protected:

        /*!
          \brief State boolean to string.
      
          \param font style option      
        */
        virtual std::string toString(bool flag) const;

        /*!
          \brief State string to boolean.
      
          \param font style option        
        */
        virtual bool toBool(std::string str);

        std::string m_family; //!< font family name
        int m_pointSize; //!< point size of the font
        bool m_bold; //!< true if font use bold, false otherwise  
        bool m_italic; //!< true if font use italic, false otherwise
        bool m_underline; //!< true if font use underline, false otherwise
        bool m_strikeout; //!< true if font use strikeout, false otherwise
        bool m_kerning; //!< true if font use kerning, false otherwise  
        LayoutAlign m_textAlign; //!<
    };
  }
}

#endif
