/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file PropertiesOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PropertiesOutside.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../core/pattern/mvc/OutsideController.h"
#include "../../../geometry/Envelope.h"
#include "../core/PropertiesItemPropertyBrowser.h"
#include "../item/MapItem.h"
#include "../../core/property/SharedProperties.h"
#include "../../core/pattern/mvc/ItemModelObservable.h"
#include "../../item/MapModel.h"
#include "../core/ItemUtils.h"
#include "../../core/pattern/derivativevisitor/VisitorUtils.h"
#include "../../core/enum/Enums.h"
#include "../core/pattern/command/ChangePropertyCommand.h"
#include "../core/Scene.h"

// Qt
#include <QGroupBox>
#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QUndoCommand>

#include <QtPropertyBrowser/QtTreePropertyBrowser>

te::layout::PropertiesOutside::PropertiesOutside( OutsideController* controller, Observable* o, PropertiesItemPropertyBrowser* propertyBrowser ) :
	QWidget(0),
	OutsideObserver(controller, o),
  m_updatingValues(false),
  m_sharedProps(0)
{
	te::gm::Envelope box = m_model->getBox();	
	setBaseSize(box.getWidth(), box.getHeight());
	setVisible(false);
	setWindowTitle("Properties");
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  if(!propertyBrowser)
    m_layoutPropertyBrowser = new PropertiesItemPropertyBrowser;
  else
    m_layoutPropertyBrowser = propertyBrowser;

  connect(m_layoutPropertyBrowser, SIGNAL(updateOutside(Property)), 
    this, SLOT(onChangePropertyValue(Property)));

  connect(m_layoutPropertyBrowser,SIGNAL(changePropertyValue(Property)),
    this,SLOT(onChangePropertyValue(Property))); 

  createLayout();

  m_sharedProps = new SharedProperties;
}

te::layout::PropertiesOutside::~PropertiesOutside()
{
  if(m_layoutPropertyBrowser)
  {
    delete m_layoutPropertyBrowser;
    m_layoutPropertyBrowser = 0;
  }

  if(m_sharedProps)
  {
    delete m_sharedProps;
    m_sharedProps = 0;
  }
}

void te::layout::PropertiesOutside::createLayout()
{
  //Layout

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setMargin(0);

  QHBoxLayout* filterLayout = new QHBoxLayout;

  m_configurePropertyEditor = new QToolButton(this);
  m_configurePropertyEditor->setText(tr("Config"));
  m_configurePropertyEditor->setBaseSize(QSize(16,16));
  m_configurePropertyEditor->setVisible(false);

  m_propertyFilterEdit = new QLineEdit(this);
  m_propertyFilterEdit->setToolTip(tr("String or regular expression to filter property list with"));

  connect(m_propertyFilterEdit,SIGNAL(textChanged(QString)),
    m_layoutPropertyBrowser,SLOT(onChangeFilter(QString))); 

  filterLayout->addWidget(m_propertyFilterEdit);
  filterLayout->addWidget(m_configurePropertyEditor);

  layout->addLayout(filterLayout);
  m_nameLabel = new QLabel(tr("Component::"), this);
  layout->addWidget(m_nameLabel);
  layout->addWidget(m_layoutPropertyBrowser->getPropertyEditor());

  QGroupBox* groupBox = new QGroupBox;
  groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  groupBox->setLayout(layout);

  QVBoxLayout* layoutAll = new QVBoxLayout(this);
  layoutAll->setMargin(0);

  layoutAll->addWidget(groupBox);

  setLayout(layoutAll);
}

void te::layout::PropertiesOutside::updateObserver( ContextItem context )
{
	setVisible(context.isShow());
	if(context.isShow() == true)
		show();
	else
		hide();
}

void te::layout::PropertiesOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::PropertiesOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::PropertiesOutside::itemsSelected(QList<QGraphicsItem*> graphicsItems, QList<QGraphicsItem*> allItems)
{
  m_updatingValues = false;
  bool window = false;

  m_layoutPropertyBrowser->clearAll();
  m_nameLabel->setText(tr("Component::"));

  m_graphicsItems = graphicsItems;

  m_allItems = allItems;

  if(m_graphicsItems.empty())
    return;
    
  Properties* props = intersection(graphicsItems, window);
  m_layoutPropertyBrowser->setHasWindows(window);

  if(!props)
    return;

  m_nameLabel->setText(tr("Component::") + props->getObjectName().c_str());
  
  foreach( Property prop, props->getProperties()) 
  {
    if(prop.isMenu() || !prop.isVisible())
      continue;

    checkDynamicProperty(prop, allItems);
    m_layoutPropertyBrowser->addProperty(prop);
  }
   
  update();
}

void te::layout::PropertiesOutside::onChangePropertyValue( Property property )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(property.getType() == dataType->getDataTypeNone())
    return;

  Scene* lScene = dynamic_cast<Scene*>(Context::getInstance().getScene()); 

  std::vector<QGraphicsItem*> commandItems;
  std::vector<Properties*> commandOld;
  std::vector<Properties*> commandNew;

  foreach( QGraphicsItem *item, m_graphicsItems) 
  {
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {
        Properties* props = new Properties("");
        Properties* beforeProps = lItem->getProperties();
        Properties* oldCommand = new Properties(*beforeProps);
        if(props)
        {
          props->setObjectName(lItem->getProperties()->getObjectName());
          props->setTypeObj(lItem->getProperties()->getTypeObj());
          props->addProperty(property);

          lItem->updateProperties(props);

          if(beforeProps)
          {
            beforeProps = lItem->getProperties();
            Properties* newCommand = new Properties(*beforeProps);
            commandItems.push_back(item);
            commandOld.push_back(oldCommand);
            commandNew.push_back(newCommand);
          }
        }       
      }
    }
  }

  if(!m_graphicsItems.isEmpty())
  {
    QUndoCommand* command = new ChangePropertyCommand(commandItems, commandOld, commandNew, this);
    lScene->addUndoStack(command);
  }

  changeMapVisitable(property);
  lScene->update();
}

void te::layout::PropertiesOutside::closeEvent( QCloseEvent * event )
{
  m_layoutPropertyBrowser->closeAllWindows();
}

te::layout::Properties* te::layout::PropertiesOutside::intersection( QList<QGraphicsItem*> graphicsItems, bool& window )
{
  Properties* props = 0;

  if(graphicsItems.size() == 1)
  {
    QGraphicsItem* item = graphicsItems.first();
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {
        props = const_cast<Properties*>(lItem->getProperties());
        window = props->hasWindows();
      }
    }
  }
  else
  {
    props = sameProperties(graphicsItems, window);
  }

  return props;
}

te::layout::Properties* te::layout::PropertiesOutside::sameProperties( QList<QGraphicsItem*> graphicsItems, bool& window )
{
  Properties* props = 0;
  std::vector<Properties*> propsVec = getAllProperties(graphicsItems, window);

  QGraphicsItem* firstItem = graphicsItems.first();
  ItemObserver* lItem = dynamic_cast<ItemObserver*>(firstItem);
  
  if(!lItem)
  {
    return props;
  }

  Properties* firstProps = const_cast<Properties*>(lItem->getProperties());
  if(!firstProps)
  {
    return props;
  }

  std::vector<Properties*>::iterator it = propsVec.begin();
  std::vector<Properties*>::iterator itend = propsVec.end();
  bool result = false;
  foreach( Property prop, firstProps->getProperties()) 
  {
    contains(itend, it, prop.getName(), result);
    if(result)
    {
      if(!props)
      {
        props = new Properties("");
      }
      props->addProperty(prop);
    }
  }  

  return props;
}

void te::layout::PropertiesOutside::contains( std::vector<Properties*>::iterator itend, 
  std::vector<Properties*>::iterator it, std::string name, bool& result )
{
  Property prop = (*it)->contains(name);
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

std::vector<te::layout::Properties*> 
  te::layout::PropertiesOutside::getAllProperties( QList<QGraphicsItem*> graphicsItems, bool& window )
{
  std::vector<Properties*> propsVec;
  bool result = true;

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {
        Properties* propsItem = const_cast<Properties*>(lItem->getProperties());
        if(propsItem)
        {
          propsVec.push_back(propsItem);
          if(result)
          {
            result = propsItem->hasWindows();
          }
        }
      }
    }
  }

  window = result;
  return propsVec;
}

void te::layout::PropertiesOutside::addDynamicOptions( Property& property, std::vector<std::string> list )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  foreach(std::string str, list) 
  {
    Variant v;
    v.setValue(str, dataType->getDataTypeString());
    property.addOption(v);
  }
}

void te::layout::PropertiesOutside::checkDynamicProperty( Property& property, QList<QGraphicsItem*> graphicsItems )
{
  if(property.getName().compare(m_sharedProps->getMapName()) == 0)
  {
    mapNameDynamicProperty(property, graphicsItems);
  }
}

void te::layout::PropertiesOutside::mapNameDynamicProperty( Property& property, QList<QGraphicsItem*> graphicsItems )
{
  std::string currentName = property.getValue().toString();

  if(currentName.compare("") == 0)
  {
    currentName = property.getOptionByCurrentChoice().toString();
  }

  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  std::vector<std::string> strList = iUtils->mapNameList();

  if(std::find(strList.begin(), strList.end(), currentName) != strList.end())
  {
    std::vector<std::string>::iterator it = std::find(strList.begin(), strList.end(), currentName);
    strList.erase(it);
  }

  addDynamicOptions(property, strList);
}

void te::layout::PropertiesOutside::changeMapVisitable( Property property )
{
  if(property.getName().compare(m_sharedProps->getMapName()) != 0)
    return;

  std::string name = property.getValue().toString();
  if(name.compare("") == 0)
  {
    name = property.getOptionByCurrentChoice().toString();
  }

  if(name.compare("") == 0)
    return;

  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
    return;

  MapItem* item = iUtils->getMapItem(name);
  if(!item)
    return;

  ItemModelObservable* obsMdl = dynamic_cast<ItemModelObservable*>(item->getModel());
  if(!obsMdl)
    return;

  MapModel* model = dynamic_cast<MapModel*>(obsMdl);
 
  if(!model)
    return;

  te::layout::VisitorUtils::getInstance().changeMapVisitable(m_graphicsItems, model);
}

te::layout::MapModel* te::layout::PropertiesOutside::getMapModel( std::string nameMap )
{
  MapModel* map = 0;

  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
    return map;

  MapItem* item = iUtils->getMapItem(nameMap);
  if(!item)
    return map;

  ItemModelObservable* obsMdl = dynamic_cast<ItemModelObservable*>(item->getModel());
  if(!obsMdl)
    return map;

  MapModel* model = dynamic_cast<MapModel*>(obsMdl);
  map = model;

  return map;
}

void te::layout::PropertiesOutside::refreshOutside()
{
  itemsSelected(m_graphicsItems, m_allItems);
}

void te::layout::PropertiesOutside::onClear( std::vector<std::string> names )
{
  m_updatingValues = false;
  m_nameLabel->setText(tr("Component::"));
  m_layoutPropertyBrowser->clearAll();
}
