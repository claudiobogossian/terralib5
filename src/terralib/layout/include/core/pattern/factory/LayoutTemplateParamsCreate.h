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
  \file LayoutOutsideParamsCreate.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEMPLATE_PARAMS_CREATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_TEMPLATE_PARAMS_CREATE_H

// TerraLib
#include "LayoutParams.h"
#include <string>

namespace te
{
  namespace layout
  {
    class LayoutTemplateParamsCreate : public LayoutParams 
    {
      public:

        LayoutTemplateParamsCreate();
        LayoutTemplateParamsCreate(std::string path);
        virtual ~LayoutTemplateParamsCreate();

        virtual std::string getPath();

      protected:

        std::string m_path;
    };

    /*
    ** ---------------------------------------------------------------
    ** Inline methods:
    */
    inline LayoutTemplateParamsCreate::LayoutTemplateParamsCreate()
    {
    }

    inline LayoutTemplateParamsCreate::LayoutTemplateParamsCreate(std::string path) :
      m_path(path)
    {
    }

    inline LayoutTemplateParamsCreate::~LayoutTemplateParamsCreate() 
    {
    }

    inline std::string LayoutTemplateParamsCreate::getPath()
    {
      return m_path;
    }

  }
}

#endif