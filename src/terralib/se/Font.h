/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  
  \brief A Font specifies the text font to use in a text symbolizer.
 */

#ifndef __TERRALIB_SE_INTERNAL_FONT_H
#define __TERRALIB_SE_INTERNAL_FONT_H

// TerraLib
#include "../common/BaseVisitable.h"
#include "Config.h"
#include "Visitor.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace se
  {
// Forward declarations
    class SvgParameter;

    /*!
      \class Font
      
      \brief A Font specifies the text font to use in a text symbolizer.

      The allowed SvgParameters are:
      <ul>
      <li>font-family</li>
      <li>font-style: normal, italic, and oblique</li>
      <li>font-weight: normal and bold</li>
      <li>font-size</li>
      </ul>

      \sa TextSymbolizer
     */
    class TESEEXPORT Font
    {
      public:

        /*!
          \enum FontStyleType

          \brief It defines the style to use for a font.
         */
        enum FontStyleType
        {
          STYLE_NORMAL,   /*!< Normal font style.  */
          STYLE_ITALIC,   /*!< Italic font style.  */
          STYLE_OBLIQUE   /*!< Oblique font style. */
        };

        /*!
          \enum FontWightType

          \brief It gives the amount of weight or boldness to use for a font.
         */
        enum FontWightType
        {
          WEIGHT_NORMAL,  /*!< Normal. */
          WEIGHT_BOLD     /*!< Bold.   */
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Font. */
        Font();

        /*! \brief Destructor. */
        ~Font();

        //@}

        void add(SvgParameter* p);

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other Font.
         */
        Font(const Font& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other Font.

          \return A reference for this.
         */
        Font& operator=(const Font& rhs);

        //@}

      private:

        std::vector<SvgParameter*> m_parameters;
    };    

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_FONT_H

