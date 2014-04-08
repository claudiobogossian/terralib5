/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file LayoutJSONTemplate.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LAYOUT_JSON_TEMPLATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_LAYOUT_JSON_TEMPLATE_H

// TerraLib
#include "LayoutTemplate.h"

namespace te
{
  namespace layout
  {
    class LayoutProperties;

    class LayoutJSONTemplate : LayoutTemplate
    {
      public:

        LayoutJSONTemplate(std::string path);
        virtual ~LayoutJSONTemplate();

        virtual bool exportTemplate(std::vector<te::layout::LayoutProperties*> properties);

        virtual std::vector<te::layout::LayoutProperties*> importTemplate();

        virtual bool deleteTemplate();

        virtual LayoutTemplateType getType();
    
        protected:

         LayoutTemplateType m_type;
    };
  }
}

#endif