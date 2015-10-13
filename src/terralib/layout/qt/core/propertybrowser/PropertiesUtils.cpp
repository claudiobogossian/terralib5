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
  \file PropertiesUtils.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PropertiesUtils.h"
#include "../../../core/property/Properties.h"
#include "../../../core/property/SharedProperties.h"
#include "../../../core/enum/Enums.h"
#include "../../../core/pattern/singleton/Context.h"
#include "../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../core/pattern/mvc/AbstractItemModel.h"
#include "../ItemUtils.h"
#include "../Scene.h"
#include "../pattern/command/ChangePropertyCommand.h"

// Qt
#include <QGraphicsItem>
#include <QObject>
#include <QUndoCommand>

te::layout::PropertiesUtils::PropertiesUtils() 
{

}

te::layout::PropertiesUtils::~PropertiesUtils()
{
  
}

te::layout::Properties te::layout::PropertiesUtils::intersection( const QList<QGraphicsItem*>& graphicsItems, bool& window )
{
  Properties props("");

  if(graphicsItems.empty())
  {
    return props;
  }

  if(graphicsItems.size() == 1)
  {
    QGraphicsItem* item = graphicsItems.first();
    if (item)
    {
      AbstractItemView* lItem = dynamic_cast<AbstractItemView*>(item);
      if(lItem)
      {
        if(lItem->getController())
        {
          props = lItem->getController()->getProperties();
          window = props.hasWindows();
        }
      }
      else
      {
        AbstractItemView* absItem = dynamic_cast<AbstractItemView*>(item);
        if(absItem != 0)
        {
          props = absItem->getController()->getProperties();
        }
      }
    }
  }
  else
  {
    props = sameProperties(graphicsItems, window);
  }

  return props;
}

te::layout::Properties te::layout::PropertiesUtils::sameProperties( const QList<QGraphicsItem*>& graphicsItems, bool& window )
{
  Properties props("");
  std::vector<Properties> propsVec = getAllProperties(graphicsItems, window);

  QGraphicsItem* firstItem = graphicsItems.first();
  AbstractItemView* lItem = dynamic_cast<AbstractItemView*>(firstItem);

  if(!lItem)
  {
    return props;
  }

  if(lItem->getController() == 0)
  {
    return props;
  }
  const Properties& firstProps = lItem->getController()->getProperties();
  if(firstProps.getProperties().empty())
  {
    return props;
  }

  std::vector<Properties>::iterator it = propsVec.begin();
  std::vector<Properties>::iterator itend = propsVec.end();
  bool result = false;
  const std::vector<Property>& vecProperty = firstProps.getProperties();
  foreach(Property prop, vecProperty)
  {
    contains(itend, it, prop.getName(), result);
    if(result)
    {
      prop.setParentItemHashCode(0);
      props.addProperty(prop);
    }
  }  

  return props;
}

void te::layout::PropertiesUtils::contains( std::vector<Properties>::iterator itend, std::vector<Properties>::iterator it, std::string name, bool& result )
{
  Property prop = (*it).getProperty(name);
  if(prop.isNull())
  {
    result = false;
    return;
  }
  else
  {
    ++it;
    result = true;
    if(it != itend)
    {
      contains(itend, it, name, result);
    }
  }
}

std::vector<te::layout::Properties> te::layout::PropertiesUtils::getAllProperties( const QList<QGraphicsItem*>& graphicsItems, bool& window )
{
  std::vector<Properties> propsVec;
  bool result = true;

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {      
      AbstractItemView* lItem = dynamic_cast<AbstractItemView*>(item);
      if(lItem)
      {
        if(!lItem->getController())
        {
          continue;
        }

        const Properties& propsItem = lItem->getController()->getProperties();
        if(!propsItem.getProperties().empty())
        {
          propsVec.push_back(propsItem);
          if(result)
          {
            result = propsItem.hasWindows();
          }
        }
      }
    }
  }

  window = result;
  return propsVec;
}

void te::layout::PropertiesUtils::addDynamicOptions( Property& property, const std::vector<std::string>& list )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  foreach(std::string str, list) 
  {
    Variant v;
    v.setValue(str, dataType->getDataTypeString());
    property.addOption(v);
  }
}

void te::layout::PropertiesUtils::checkDynamicProperty( Property& property, const QList<QGraphicsItem*>& graphicsItems )
{
  SharedProperties sharedProps;  
  if(property.getName().compare(sharedProps.getMapName()) == 0)
  {
    mapNameDynamicProperty(property, graphicsItems);
  }
}

void te::layout::PropertiesUtils::mapNameDynamicProperty( Property& property, const QList<QGraphicsItem*>& graphicsItems )
{
  std::string currentName = property.getValue().toString();

  if(currentName.compare("") == 0)
  {
    currentName = property.getOptionByCurrentChoice().toString();
  }

  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  std::vector<std::string> strList = iUtils->mapNameList();


  addDynamicOptions(property, strList);
}











