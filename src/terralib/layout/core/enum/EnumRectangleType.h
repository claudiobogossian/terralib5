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
  \file EnumEnumRectangleType.h

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_RECTANGLE_TYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_RECTANGLE_TYPE_H

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {
    class EnumType;

    class TELAYOUTEXPORT EnumRectangleType : public AbstractEnum
    {
      public:

        /*!
          \brief Constructor
        */
        EnumRectangleType(); 

        /*!
          \brief Destructor
        */ 
        virtual ~EnumRectangleType();

        /*!
          \brief Returns value that represents none type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getNoneType() const;
        
        /*!
          \brief Returns value that represents simple rectangle type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getSimpleRectangleType() const;

        /*!
          \brief Returns value that represents rounded retangle type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getRoundedRetangleType() const;

				 /*!
          \brief Returns value that represents single corner trimmed rectangle type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getSingleCornerTrimmedRectangleType() const;
				
		
			protected:

        /*!
          \brief Reimplemented from AbstractEnum
        */
        virtual void init();

      protected:

        EnumType* m_noneType; //!< value that represents none type belonging to enumeration
        EnumType* m_simpleRect; //!< value that represents simple retangle type belonging to enumeration
				EnumType* m_roundedRect; //!< value that represents rounded retangle type belonging to enumeration
				EnumType* m_singleCornerTrimmedRect; //!< value that represents single corner trimmed rectangle type belonging to enumeration
 
    };
  }
}

#endif
