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
  \file Enums.h
   
   \brief Singleton class responsible for keeping active objects of Enum while the plugin is loaded in memory 
          and make them available for access anywhere in the plugin or layout module.
      Ex .: Enum for data type, enum for object type, enum for mode type, enum for template type, etc.

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
#include "EnumModeType.h"
#include "EnumTemplateType.h"

namespace te
{
  namespace layout
  {
    /*!
      \brief Singleton class responsible for keeping active objects of Enum while the plugin is loaded in memory 
             and make them available for access anywhere in the plugin or layout module.
        Ex .: Enum for data type, enum for object type, enum for mode type, enum for template type, etc.
	  
	    \ingroup layout

	    \sa te::common::Singleton
	  */
    class TELAYOUTEXPORT Enums : public te::common::Singleton<Enums>
    {
      friend class te::common::Singleton<Enums>;

      public:

        /*!
          \brief Destructor
        */
        virtual ~Enums();

        /*!
          \brief Change data type enumeration.

          \param type data type enumeration
        */
        virtual void setEnumDataType(EnumDataType* type);

        /*!
          \brief Returns data type enumeration.

          \return data type enumeration
        */
        virtual EnumDataType* getEnumDataType();

        /*!
          \brief Change graphic object (MVC component) and widget object (MVC widget) type enumeration. 

          \param graphic object type enumeration
        */
        virtual void setEnumObjectType(EnumObjectType* type);

        /*!
          \brief Returns graphic object (MVC component) and widget object (MVC widget) type enumeration.

          \return graphic object type enumeration
        */
        virtual EnumObjectType* getEnumObjectType();

        /*!
          \brief Change grid style type enumeration.

          \param grid style type enumeration
        */
        virtual void setEnumGridStyleType(EnumGridStyleType* type);

        /*!
          \brief Returns grid style type enumeration.

          \return grid style type enumeration
        */
        virtual EnumGridStyleType* getEnumGridStyleType();

        /*!
          \brief Change line style type enumeration.

          \param line style type enumeration
        */
        virtual void setEnumLineStyleType(EnumLineStyleType* type);

        /*!
          \brief Returns line style type enumeration.

          \return line style type enumeration
        */
        virtual EnumLineStyleType* getEnumLineStyleType();

        /*!
          \brief Change mode type enumeration. 

          \param mode type enumeration
        */
        virtual void setEnumModeType(EnumModeType* type);

        /*!
          \brief Returns mode type enumeration.

          \return mode type enumeration
        */
        virtual EnumModeType* getEnumModeType();

        /*!
          \brief Change template type enumeration.

          \param template type enumeration
        */
        virtual void setEnumTemplateType(EnumTemplateType* type);

        /*!
          \brief Returns template type enumeration

          \return template type enumeration
        */
        virtual EnumTemplateType* getEnumTemplateType();

      protected:

        /*!
          \brief Constructor
        */ 
        Enums();  

      protected:

        EnumDataType* m_dataType; //!< data type enumeration
        EnumObjectType* m_objType; //!< graphic object type enumeration
        EnumGridStyleType* m_gridStyleType; //!< grid style type enumeration
        EnumLineStyleType* m_lineStyleType; //!< line style type enumeration
        EnumModeType* m_modeType; //!< mode type enumeration
        EnumTemplateType* m_templateType; //!< template type enumeration

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
