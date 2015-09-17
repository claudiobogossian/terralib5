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
  \file terralib/layout/qt/core/pattern/factory/SVGItemFactory.cpp

  \brief This is the concrete factory for svg item.
*/

// TerraLib
#include "SVGItemFactory.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../../../item/SVGModel.h"
#include "../../../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../../item/SVGItem.h"

te::layout::AbstractItemView* te::layout::SVGItemFactory::build(ItemFactoryParamsCreate params)
{
  Properties      props = params.getProperties();  

  SVGModel* model = new SVGModel();
  if (props.getProperties().empty())
  {
    setProperties(model, params);
  }

  AbstractItemController* controller = new AbstractItemController(model);
  SVGItem* view = new SVGItem(controller);
  controller->setView(view);

  if (!props.getProperties().empty())
  {
    model->setProperties(props);
  }
  return view;
}

te::layout::SVGItemFactory::SVGItemFactory() :
  ItemFactory(Enums::getInstance().getEnumObjectType()->getSVGItem()->getName())
{

}

te::layout::SVGItemFactory::~SVGItemFactory()
{

}
