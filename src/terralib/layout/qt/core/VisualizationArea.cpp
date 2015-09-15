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
  \file VisualizationArea.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "VisualizationArea.h"
#include "pattern/factory/item/ItemFactoryParamsCreate.h"
#include "../../core/property/Properties.h"
#include "../../core/Utils.h"
#include "Scene.h"
#include "BuildGraphicsItem.h"
#include "../../core/enum/Enums.h"

te::layout::VisualizationArea::VisualizationArea( Scene* scene, te::gm::Envelope boxArea ) :
  m_boxArea(boxArea),
  m_scene(scene)
{
  build();
}

te::layout::VisualizationArea::~VisualizationArea()
{

}

void te::layout::VisualizationArea::build()
{
  createPaper();
}

void te::layout::VisualizationArea::createPaper()
{
  PaperConfig* paperConfig = m_scene->getPaperConfig();
  int zValue = 0;

  Properties properties = Utils::convertToProperties(*paperConfig);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  EnumObjectType* objType = Enums::getInstance().getEnumObjectType();

  Property prop(0);
  prop.setName("zValue");
  prop.setLabel("zValue");
  prop.setValue(zValue, dataType->getDataTypeInt());
  properties.addProperty(prop);
  properties.setTypeObj(objType->getPaperItem());
  
  // Create paper item
  BuildGraphicsItem build(m_scene);
  QGraphicsItem* item = build.buildItem(properties);
  if (item)
  {
    item->setPos(0, 0);
  }
}

void te::layout::VisualizationArea::changeBoxArea( te::gm::Envelope boxArea )
{
  m_boxArea = boxArea;
}

