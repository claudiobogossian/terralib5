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
  \file EnumToolType.h
   
  \brief Class to represent a tool type enumeration. Ex.: json, etc.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_TOOL_TYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_TOOL_TYPE_H

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {
    class EnumType;

    /*!
      \brief Class to represent a tool type enumeration. Ex.: zoom area, etc.
    
      \ingroup layout

      \sa te::layout::AbstractEnum
    */
    class TELAYOUTEXPORT EnumToolType : public AbstractEnum
    {
      public:

        /*!
          \brief Constructor
        */
        EnumToolType();

        /*!
          \brief Destructor
        */ 
        virtual ~EnumToolType();
        
        /*!
        \brief Returns value that represents none type belonging to enumeration.

        \return enum value
        */
        virtual EnumType* getNoneTool() const;

        /*!
          \brief Returns value that represents zoom area tool type belonging to enumeration.
      
          \return enum value      
        */
        virtual EnumType* getZoomAreaTool() const;

        virtual EnumType* getCreateLineItemTool() const;

        virtual EnumType* getCreatePolygonItemTool() const;

        virtual EnumType* getCreateItemTool() const;

      protected:

        /*!
          \brief Reimplemented from AbstractEnum
        */
        virtual void init();

      protected:

        EnumType* m_noneTool; //!< value that represents none type belonging to enumeration
        EnumType* m_zoomAreaTool; //!< value that represents zoom area type belonging to enumeration
        EnumType* m_createLineItemTool;
        EnumType* m_createPolygonItemTool;
        EnumType* m_createItemTool;
    };
  }
}

#endif
