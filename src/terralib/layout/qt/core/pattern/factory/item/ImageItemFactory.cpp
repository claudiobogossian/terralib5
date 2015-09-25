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
  \file terralib/layout/qt/core/pattern/factory/ImageItemFactory.cpp

  \brief This is the concrete factory for image item.
*/

// TerraLib
#include "ImageItemFactory.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../../../item/ImageModel.h"
#include "../../../../item/ImageController.h"
#include "../../../../item/ImageItem.h"

te::layout::AbstractItemView* te::layout::ImageItemFactory::build(ItemFactoryParamsCreate params)
{
  Properties      props = params.getProperties(); 

  ImageModel* model = new ImageModel();
  if (props.getProperties().empty())
  {
    setProperties(model, params);
  }

  AbstractItemController* controller = new ImageController(model);
  ImageItem* view = new ImageItem(controller);
  controller->setView(view);

  if (!props.getProperties().empty())
  {
    model->setProperties(props);
  }
  return dynamic_cast<AbstractItemView*>(view);
}

te::layout::ImageItemFactory::ImageItemFactory() :
  ItemFactory(Enums::getInstance().getEnumObjectType()->getImageItem()->getName())
{

}

te::layout::ImageItemFactory::~ImageItemFactory()
{

}
