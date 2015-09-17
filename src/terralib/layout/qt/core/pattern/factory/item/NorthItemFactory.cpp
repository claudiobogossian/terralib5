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
  \file terralib/layout/qt/core/pattern/factory/NorthItemFactory.cpp

  \brief This is the concrete factory for north item.
*/

// TerraLib
#include "NorthItemFactory.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../../../item/NorthModel.h"
#include "../../../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../../item/NorthItem.h"

te::layout::AbstractItemView* te::layout::NorthItemFactory::build(ItemFactoryParamsCreate params)
{
  Properties      props = params.getProperties(); 

  NorthModel* model = new NorthModel();
  if (props.getProperties().empty())
  {
    setProperties(model, params);
  }

  AbstractItemController* controller = new AbstractItemController(model);
  NorthItem* view = new NorthItem(controller);
  controller->setView(view);

  if (!props.getProperties().empty())
  {
    model->setProperties(props);
  }
  return view;
}

te::layout::NorthItemFactory::NorthItemFactory() :
  ItemFactory(Enums::getInstance().getEnumObjectType()->getNorthItem()->getName())
{

}

te::layout::NorthItemFactory::~NorthItemFactory()
{

}
