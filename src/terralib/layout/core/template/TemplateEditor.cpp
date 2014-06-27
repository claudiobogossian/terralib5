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
  \file TemplateEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TemplateEditor.h"
#include "TemplateParamsCreate.h"
#include "Context.h"
#include "AbstractTemplateFactory.h"
#include "JSONTemplate.h"

te::layout::TemplateEditor::TemplateEditor( te::layout::LayoutTemplateType type, std::string path ) 
{
  AbstractTemplateFactory* factory = Context::getInstance()->getTemplateFactory(); 
  TemplateParamsCreate params(path);
  m_template = factory->make(type, params);
}

te::layout::TemplateEditor::~TemplateEditor()
{
  if(m_template)
  {
    if(m_template->getType() == TPJSONTemplate)
    {
      JSONTemplate* jt = dynamic_cast<JSONTemplate*>(m_template);

      if(jt)
        delete jt;
    }
  }
}

te::layout::AbstractTemplate* te::layout::TemplateEditor::getTemplate()
{
  return m_template;
}
