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
  \file terralib/layout/core/template/TemplateFactoryParamsCreate.cpp

  \brief Parameters to create a new template. 
*/

// TerraLib
#include "TemplateFactoryParamsCreate.h"

te::layout::TemplateFactoryParamsCreate::TemplateFactoryParamsCreate(std::string path) :
m_path(path)
{

}

te::layout::TemplateFactoryParamsCreate::TemplateFactoryParamsCreate(const TemplateFactoryParamsCreate& rhs)
{
  this->operator=(rhs);
}

te::layout::TemplateFactoryParamsCreate::~TemplateFactoryParamsCreate()
{

}

te::common::AbstractParameters* te::layout::TemplateFactoryParamsCreate::clone() const
{
  return new TemplateFactoryParamsCreate(*this);
}

void te::layout::TemplateFactoryParamsCreate::reset() throw(te::common::Exception)
{
  m_path.clear();
}

std::string te::layout::TemplateFactoryParamsCreate::getFilePath()
{
  return m_path;
}






