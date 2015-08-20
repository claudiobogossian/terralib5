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
  \file TemplateEditor.h
   
  \brief Calls the factory to create a template of the specified type and keep. Responsible for delete the created template.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEMPLATE_EDITOR_H 
#define __TERRALIB_LAYOUT_INTERNAL_TEMPLATE_EDITOR_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class AbstractTemplate;
    class EnumType;

    /*!
      \brief Calls the factory to create a template of the specified type and keep. Responsible for delete the created template.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT TemplateEditor
    {
      public:

        TemplateEditor(EnumType* type, std::string path);

        virtual ~TemplateEditor();

        virtual te::layout::AbstractTemplate* getTemplate();

      protected:

        te::layout::AbstractTemplate* m_template;
    };
  }
}

#endif
