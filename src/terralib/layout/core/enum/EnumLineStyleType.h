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
  \file EnumLineStyleType.h
   
  \brief Class to represent a line style type enumeration. Ex.: solid, dash, dot, etc.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_LINESTYLE_TYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_LINESTYLE_TYPE_H

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {
    class EnumType;

    /*!
      \brief Class to represent a line style type enumeration. Ex.: solid, dash, dot, etc.
	  
	    \ingroup layout

	    \sa te::layout::AbstractEnum
	  */
    class TELAYOUTEXPORT EnumLineStyleType : public AbstractEnum
    {
      public:

        /*!
          \brief Constructor
        */
        EnumLineStyleType(); 

        /*!
          \brief Destructor
        */ 
        virtual ~EnumLineStyleType();
        
        /*!
          \brief Returns value that represents none style type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStyleNone() const;

        /*!
          \brief Returns value that represents solid style type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStyleSolid() const;

        /*!
          \brief Returns value that represents dash style type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStyleDash() const;

        /*!
          \brief Returns value that represents dot style type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStyleDot() const;

        /*!
          \brief Returns value that represents dash dot style type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStyleDashDot() const;

        /*!
          \brief Returns value that represents dash dot dot style type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStyleDashDotDot() const;

        /*!
          \brief Returns value that represents custom dash style type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getStyleCustomDash() const;

      protected:

        /*!
          \brief Reimplemented from AbstractEnum
        */
        virtual void init();

      protected:

        EnumType* m_styleNone; //!< value that represents none style type belonging to enumeration
        EnumType* m_styleSolid; //!< value that represents solid style type belonging to enumeration
        EnumType* m_styleDash; //!< value that represents dash style type belonging to enumeration
        EnumType* m_styleDot; //!< value that represents dot style type belonging to enumeration
        EnumType* m_styleDashDot; //!< value that represents dash dot style type belonging to enumeration
        EnumType* m_styleDashDotDot; //!< value that represents dash dot dot style type belonging to enumeration
        EnumType* m_styleCustomDash; //!< value that represents custom dash style type belonging to enumeration
    };
  }
}

#endif
