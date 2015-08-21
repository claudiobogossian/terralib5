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
  \file terralib/layout/qt/core/pattern/factory/ToolFactoryParamsCreate.cpp

  \brief Parameters to create a new tool. 
*/

// TerraLib
#include "ToolFactoryParamsCreate.h"

te::layout::ToolFactoryParamsCreate::ToolFactoryParamsCreate(View* view, EnumType* itemType) :
  m_view(view),
  m_itemType(itemType)
{

}

te::layout::ToolFactoryParamsCreate::ToolFactoryParamsCreate(const ToolFactoryParamsCreate& rhs)
{
  this->operator=(rhs);
}

te::layout::ToolFactoryParamsCreate::~ToolFactoryParamsCreate()
{

}

te::common::AbstractParameters* te::layout::ToolFactoryParamsCreate::clone() const
{
  return new ToolFactoryParamsCreate(*this);
}

void te::layout::ToolFactoryParamsCreate::reset() throw(te::common::Exception)
{

}

te::layout::View* te::layout::ToolFactoryParamsCreate::getView()
{
  return m_view;
}

te::layout::EnumType* te::layout::ToolFactoryParamsCreate::getItemType()
{
  return m_itemType;
}




