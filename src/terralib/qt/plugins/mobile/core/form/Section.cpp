/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
\file src/terralib/qt/plugins/mobile/core/form/Section.cpp

\brief This file defines the section class.
*/

//TerraLib
#include "../../../../../common/STLUtils.h"
#include "Form.h"
#include "Section.h"

te::qt::plugins::terramobile::Section::Section():
  m_name(""),
  m_description("")
{

}

te::qt::plugins::terramobile::Section::~Section()
{
  te::common::FreeContents(m_forms);
  m_forms.clear();
}

te::qt::plugins::terramobile::Form* te::qt::plugins::terramobile::Section::getForm(std::string formName)
{
  for (std::size_t t = 0; t < m_forms.size(); ++t)
  {
    if (m_forms[t]->getName() == formName)
      return m_forms[t];
  }

  return 0;
}
