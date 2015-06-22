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
  \file EnumStarType.h
   
  \brief Class to represent a scale point enumeration. Ex.: X, square, circle, etc.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_STAR_TYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_STAR_TYPE_H

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {
    class EnumType;

    /*!
      \brief Class to represent a scale star enumeration.
	  
	    \ingroup layout

	    \sa te::layout::AbstractEnum
	  */
    class TELAYOUTEXPORT EnumStarType : public AbstractEnum
    {
      public:

        /*!
          \brief Constructor
        */
        EnumStarType(); 

        /*!
          \brief Destructor
        */ 
        virtual ~EnumStarType();

        /*!
          \brief Returns value that represents none type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getNoneType() const;
        
        /*!
          \brief Returns value that represents star 1 type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStar1Type() const;

        /*!
          \brief Returns value that represents star 2 type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStar2Type() const;

        /*!
          \brief Returns value that represents star 3 type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStar3Type() const;

				/*!
          \brief Returns value that represents star 4 type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStar4Type() const;
		
			protected:

        /*!
          \brief Reimplemented from AbstractEnum
        */
        virtual void init();

      protected:

        EnumType* m_noneType; //!< value that represents none type belonging to enumeration
        EnumType* m_star5points; //!< value that represents star 5 points type belonging to enumeration
				EnumType* m_star6points; //!< value that represents star 6 points type belonging to enumeration
				EnumType* m_star7points; //!< value that represents star 7 points type belonging to enumeration
				EnumType* m_star8points; //!< value that represents star 8 points type belonging to enumeration
 
    };
  }
}

#endif
