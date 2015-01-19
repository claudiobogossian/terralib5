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
  \file EnumType.h
   
  \brief Class that represents the value of an enumeration. An enumeration is made of "1..n" objects EnumType.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_TYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_TYPE_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    /*!
      \brief Class that represents the value of an enumeration. An enumeration is made of "1..n" objects EnumType.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT EnumType 
    {
      public:

        /*!
         \brief Constructor

         \param id numeric value assigned
         \param name  
        */
        EnumType(int id, std::string name);

        /*!
         \brief Destructor
        */
        virtual ~EnumType();

        /*!
          \brief Returns numeric value assigned
		  
		      \return numeric value assigned 		  
        */
        int getId();

        /*!
          \brief Returns name
		  
		      \return name 		  
        */
        std::string getName();

        /*!
          \brief Change name
		  
		      \return new name 		  
        */
        void setName(std::string name); 

        /*!
          \brief Returns label
		  
		      \return label 		  
        */
        std::string getLabel();
        
        /*!
          \brief Change label 
		  
		      \return new label 		  
        */
        void setLabel(std::string label);
                
      protected:

        int         m_id; //!< numeric value assigned
        std::string m_name; //!< name
        std::string m_label; //!< label
    };
  }
}

#endif
