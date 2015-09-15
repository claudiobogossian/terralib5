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
  \file terralib/layout/qt/core/pattern/factory/RectangleItemFactory.cpp

  \brief This is the concrete factory for rectangle item.
*/

// TerraLib
#include "PaperItemFactory.h"
#include "../../../../../item/PaperModel.h"
#include "../../../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../../../core/property/Properties.h"
#include "../../../../item/PaperItem.h"

te::layout::AbstractItemView* te::layout::PaperItemFactory::build(ItemFactoryParamsCreate params)
{
  Properties props = params.getProperties();

  PaperModel* model = new PaperModel();
  if (props.getProperties().empty())
  {
    setProperties(model, params);
  }

  AbstractItemController* controller = new AbstractItemController(model);
  PaperItem* view = new PaperItem(controller);
  controller->setView(view);

  if (!props.getProperties().empty())
  {
    model->setProperties(props);
  }
  return view;
}

te::layout::PaperItemFactory::PaperItemFactory() :
  ItemFactory(Enums::getInstance().getEnumObjectType()->getPaperItem()->getName())
{

}

te::layout::PaperItemFactory::~PaperItemFactory()
{

}



