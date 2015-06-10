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
  \file AbstractEnum.h
   
  \brief Abstract class to represent an enumeration.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ENUM_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ENUM_H

// TerraLib
#include "EnumType.h"
#include "../Config.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace layout
  {
    /*!
      \brief Abstract class to represent an enumeration.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT AbstractEnum
    {
      public:
       
        /*!
          \brief Constructor
        */
        AbstractEnum();

        /*!
          \brief Destructor
        */ 
        virtual ~AbstractEnum();

        /*!
          \brief Searching for a value of the enumeration by id

          \param enumId id
          \return value found in the enumeration or null 
        */ 
        virtual EnumType* getEnum(int enumId) const;

        /*!
          \brief Searching for an enumeration value by name

          \param name 
          \return value found in the enumeration or null 
        */ 
        virtual EnumType* getEnum(std::string name) const;

        /*!
          \brief Searching for an enumeration value by name

          \param name 
          \return value found in the enumeration or null 
        */ 
        virtual EnumType* searchLabel(std::string label) const;

        /*!
          \brief Searching for a max value of the id

          \return -1 if no found, otherwise the id
        */ 
        virtual int maxId();

        /*!
          \brief Searching for a min value of the id

          \return -1 if no found, otherwise the id
        */ 
        virtual int minId();

        virtual int size();

      protected:
        
          /*!
            \brief Creates the enumeration values and adds the list
              Reimplement this function in a AbstractEnum subclass to provide the enum's init implementation.
          */ 
          virtual void init() = 0;

      protected:

        std::vector<EnumType*> m_enums; //!< list of enumeration values
    };
  }
}

#endif
