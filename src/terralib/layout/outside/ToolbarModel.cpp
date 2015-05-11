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
  \file ToolbarModel.cpp
   
  \brief 

  \ingroup layout
*/


// TerraLib
#include "ToolbarModel.h"
#include "../core/enum/Enums.h"

te::layout::ToolbarModel::ToolbarModel()
{
  m_type = Enums::getInstance().getEnumObjectType()->getToolbar();
  m_box = te::gm::Envelope(0., 0., 40., 200.);
}

te::layout::ToolbarModel::~ToolbarModel()
{

}


