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
  \file LayoutTemplateEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutTemplateEditor.h"
#include "LayoutTemplateParamsCreate.h"
#include "LayoutContext.h"
#include "LayoutAbstractTemplateFactory.h"
#include "LayoutJSONTemplate.h"

te::layout::LayoutTemplateEditor::LayoutTemplateEditor( te::layout::LayoutTemplateType type, std::string path ) 
{
  LayoutAbstractTemplateFactory* factory = LayoutContext::getInstance()->getTemplateFactory(); 
  LayoutTemplateParamsCreate params(path);
  m_template = factory->make(type, params);
}

te::layout::LayoutTemplateEditor::~LayoutTemplateEditor()
{
  if(m_template)
  {
    if(m_template->getType() == TPJSONTemplate)
    {
      LayoutJSONTemplate* jt = dynamic_cast<LayoutJSONTemplate*>(m_template);

      if(jt)
        delete jt;
    }
  }
}

te::layout::LayoutTemplate* te::layout::LayoutTemplateEditor::getTemplate()
{
  return m_template;
}
