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
  \file terralib/layout/qt/core/pattern/factory/CreateItemToolFactory.cpp

  \brief This is the concrete factory for zoom area tools.
*/

// TerraLib
#include "CreateItemToolFactory.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../View.h"
#include "../../../tools/CreateItemTool.h"

te::layout::AbstractLayoutTool* te::layout::CreateItemToolFactory::build(ToolFactoryParamsCreate params)
{
  View* view = params.getView();
  EnumType* itemType = params.getItemType();
  
  return new CreateItemTool(params.getView(), itemType, params.getView());
}

te::layout::CreateItemToolFactory::CreateItemToolFactory() :
  ToolFactory(Enums::getInstance().getEnumToolType()->getCreateItemTool()->getName())
{

}

te::layout::CreateItemToolFactory::~CreateItemToolFactory()
{

}
