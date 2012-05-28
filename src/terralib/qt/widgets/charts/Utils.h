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
  \file Utils.h

  \brief This file contains a set of utility chart functions
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTUTILS_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTUTILS_H

//TerraLib
#include "../Config.h"

//Qwt
#include <qwt_text.h>

namespace te
{
  // Forward declarations
  namespace color { class RGBAColor; }
  namespace se    { class Font; class Fill; class Stroke; }
 
  namespace qt
  {
    namespace widgets
    {      
      /*!
        \function Terralib2Qwt

        This function returns a default QwtText.

        \param text A text.
        
        \return A QwtText

        \note The caller will take the ownership of the returned pointer.
      */
      QwtText* Terralib2Qwt(const std::string& title);
      
      
      /*!
        \function Terralib2Qwt

        This function translates TerraLib text styles into a QwtText.

        \param text A text...
        
        \return A QwtText

        \note The caller will take the ownership of the returned pointer.
        \note It will not take the ownership of the symbol pointer.
      */
      QwtText* Terralib2Qwt(const std::string& text,  te::color::RGBAColor* color, 
                   te::se::Font*  font, te::se::Fill* backFill, 
                   te::se::Stroke* backStroke);
                    
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTUTILS_H

