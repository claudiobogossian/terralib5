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
  \file EnumPointType.h
   
  \brief Class to represent a scale point enumeration. Ex.: X, square, circle, etc.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_POINT_TYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_POINT_TYPE_H

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {
    class EnumType;

    /*!
      \brief Class to represent a scale point enumeration. Ex.: X, square, circle, etc.
	  
	    \ingroup layout

	    \sa te::layout::AbstractEnum
	  */
    class TELAYOUTEXPORT EnumPointType : public AbstractEnum
    {
      public:

        /*!
          \brief Constructor
        */
        EnumPointType(); 

        /*!
          \brief Destructor
        */ 
        virtual ~EnumPointType();

        /*!
          \brief Returns value that represents none type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getNoneType() const;
        
        /*!
          \brief Returns value that represents star type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStarType() const;

        /*!
          \brief Returns value that represents circle type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getCircleType() const;

        /*!
          \brief Returns value that represents X type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getXType() const;

        /*!
          \brief Returns value that represents square type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getSquareType() const;

        /*!
          \brief Returns value that represents rhombus type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getRhombusType() const;

        /*!
          \brief Returns value that represents cross type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getCrossType() const;

      protected:

        /*!
          \brief Reimplemented from AbstractEnum
        */
        virtual void init();

      protected:

        EnumType* m_noneType; //!< value that represents none type belonging to enumeration
        EnumType* m_star; //!< value that represents star type belonging to enumeration
        EnumType* m_circle; //!< value that represents circle type belonging to enumeration
        EnumType* m_x; //!< value that represents x type belonging to enumeration
        EnumType* m_square; //!< value that represents square type belonging to enumeration
        EnumType* m_rhombus; //!< value that represents rhombus type belonging to enumeration
        EnumType* m_cross; //!< value that represents cross type belonging to enumeration
    };
  }
}

#endif
