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
  \file EnumTextType.h
   
   \brief Class to represent a graphic object (MVC component) and widget object (MVC widget) type enumeration. 
      Ex. component: map, legend, scale, rectangle, text, etc.
      Ex. widget: object inspector, toolbar, etc.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_TEXT_TYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_TEXT_TYPE_H 

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

// STL
#include <vector>

namespace te
{
  namespace layout
  {
    /*!
      \brief Class to represent a graphic object (MVC component) and widget object (MVC widget) type enumeration. 
        Ex. component: map, legend, scale, rectangle, text, etc.
        Ex. widget: object inspector, toolbar, etc.
    
      \ingroup layout

      \sa te::layout::AbstractEnum
    */
    class TELAYOUTEXPORT EnumTextType : public AbstractEnum
    {
      public:

        /*!
          \brief Constructor
        */
        EnumTextType(); 

        /*!
          \brief Destructor
        */ 
        virtual ~EnumTextType();

         /*!
          \brief Returns font type
      
          \return enum value      
        */
      
        virtual EnumType* getFontItem() const;

      protected:

        /*!
          \brief Reimplemented from AbstractEnum
        */
        virtual void init();

      protected:
        
        EnumType* m_fontItem; //!< value that represents font type belonging to enumeration
        
    };
  }
}

#endif


