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
  \file Enums.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUMS_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUMS_H

// TerraLib
#include "../../../common/Singleton.h"
#include "../Config.h"
#include "EnumType.h"
#include "EnumDataType.h"
#include "EnumObjectType.h"
#include "EnumGridStyleType.h"
#include "EnumLineStyleType.h"

namespace te
{
  namespace layout
  {
    class TELAYOUTEXPORT Enums : public te::common::Singleton<Enums>
    {
      friend class te::common::Singleton<Enums>;

      public:

        virtual ~Enums();

        virtual EnumDataType* getEnumDataType();

        virtual EnumObjectType* getEnumObjectType();

        virtual EnumGridStyleType* getEnumGridStyleType();

        virtual EnumLineStyleType* getEnumLineStyleType();

      protected:

        Enums();  

        virtual void init();

      protected:

        EnumDataType* m_dataType;
        EnumObjectType* m_objType;
        EnumGridStyleType* m_gridStyleType;
        EnumLineStyleType* m_lineStyleType;

      private:
      
          /*!
          \brief Copy constructor not allowed.
       
          \param rhs The right-hand-side copy that would be used to copy from.
          */
          Enums(const Enums& rhs);
      
          /*!
          \brief Assignment operator not allowed.
       
          \param rhs The right-hand-side copy that would be used to copy from.
       
          \return A reference to this object.
          */
          Enums& operator=(const Enums& rhs);       
    };
  }
}

#endif
