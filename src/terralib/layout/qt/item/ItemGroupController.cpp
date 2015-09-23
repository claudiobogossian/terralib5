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

// TerraLib
#include "ItemGroupController.h"

#include "ItemGroup.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"


te::layout::ItemGroupController::ItemGroupController(AbstractItemModel* model)
  : AbstractItemController(model)
{
}

te::layout::ItemGroupController::~ItemGroupController()
{
}


void te::layout::ItemGroupController::itemAdded()
{
  ItemGroup* item = dynamic_cast<ItemGroup*>(m_view);
  if(item == 0)
  {
    return;
  }

  QRectF rect = item->childrenBoundingRect();
  double newWidth = rect.width();
  double newHeight = rect.height();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pCurrentWidth = getProperty("width");
  const Property& pCurrentHeight = getProperty("height");

  double currentWidth = pCurrentWidth.getValue().toDouble();
  double currentHeight = pCurrentHeight.getValue().toDouble();

  if(newWidth != currentWidth ||  newHeight != currentHeight)
  {
    Properties properties("");

    {
      Property property;
      property.setName("width");
      property.setValue(newWidth, dataType->getDataTypeDouble());
      properties.addProperty(property);
    }
    {
      Property property;
      property.setName("height");
      property.setValue(newHeight, dataType->getDataTypeDouble());
      properties.addProperty(property);
    }
    
    m_model->setProperties(properties);
  }
}
