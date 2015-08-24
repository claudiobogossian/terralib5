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
  \file terralib/layout/qt/core/pattern/factory/EllipseItemFactory.cpp

  \brief This is the concrete factory for ellipse item.
*/

// TerraLib
#include "EllipseItemFactory.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../../../item/EllipseModel.h"
#include "../../../../../core/pattern/mvc/AbstractItemController.h"

te::layout::AbstractItemView* te::layout::EllipseItemFactory::build(ItemFactoryParamsCreate params)
{
  Properties      props = params.getProperties();

  EllipseModel* model = new EllipseModel();
  if (props.getProperties().empty())
  {
    setProperties(model, params);
  }

  AbstractItemController* controller = new AbstractItemController(model);
  AbstractItemView* view = controller->getView();

  if (!props.getProperties().empty())
  {
    model->setProperties(props);
  }
  return dynamic_cast<AbstractItemView*>(view);
}

te::layout::EllipseItemFactory::EllipseItemFactory() :
  NewItemFactory(Enums::getInstance().getEnumObjectType()->getEllipseItem()->getName())
{

}

te::layout::EllipseItemFactory::~EllipseItemFactory()
{

}
