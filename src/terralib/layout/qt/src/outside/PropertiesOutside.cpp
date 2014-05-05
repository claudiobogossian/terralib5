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
#include "Context.h"
#include "Observable.h"
#include "ItemObserver.h"
#include "OutsideObserver.h"
#include "OutsideController.h"
#include "../../../../geometry/Envelope.h"
#include "PropertiesItemPropertyBrowser.h"

// Qt
#include <QGroupBox>
#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>

te::layout::PropertiesOutside::PropertiesOutside( OutsideController* controller, Observable* o ) :
	QDockWidget("", 0, 0),
	OutsideObserver(controller, o),
  m_updatingValues(false)
{
	te::gm::Envelope box = m_model->getBox();	
	setBaseSize(box.getWidth(), box.getHeight());
	setVisible(false);
	setWindowTitle("Layout - Propriedades");
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  m_layoutPropertyBrowser = new PropertiesItemPropertyBrowser;

  connect(m_layoutPropertyBrowser, SIGNAL(updateOutside(Property)), 
    this, SLOT(onChangePropertyValue(Property)));

  connect(m_layoutPropertyBrowser,SIGNAL(changePropertyValue(Property)),
    this,SLOT(onChangePropertyValue(Property))); 

  createLayout();
}

te::layout::PropertiesOutside::~PropertiesOutside()
{
  if(m_layoutPropertyBrowser)
  {
    delete m_layoutPropertyBrowser;
    m_layoutPropertyBrowser = 0;
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

  m_propertyFilterEdit = new QLineEdit(this);
  m_propertyFilterEdit->setToolTip(tr("String or regular expression to filter property list with"));

  connect(m_propertyFilterEdit,SIGNAL(textChanged(QString)),
    m_layoutPropertyBrowser,SLOT(onChangeFilter(QString))); 

  filterLayout->addWidget(m_propertyFilterEdit);
  filterLayout->addWidget(m_configurePropertyEditor);

  layout->addLayout(filterLayout);
  m_nameLabel = new QLabel(tr("QObject::unknown"), this);
  layout->addWidget(m_nameLabel);
  layout->addWidget(m_layoutPropertyBrowser->getPropertyEditor());

  QGroupBox* groupBox = new QGroupBox;
  groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  groupBox->setLayout(layout);

  setWidget(groupBox);
}

void te::layout::PropertiesOutside::updateObserver( ContextItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
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

void te::layout::PropertiesOutside::itemsSelected(QList<QGraphicsItem*> graphicsItems)
{
  m_updatingValues = false;

  m_layoutPropertyBrowser->clearAll();

  m_graphicsItems = graphicsItems;

  if(m_graphicsItems.empty())
    return;
    
  bool gridWindow = false;
  Properties* props = intersection(graphicsItems, gridWindow);
  m_layoutPropertyBrowser->setHasGridWindows(gridWindow);

  if(!props)
    return;

  foreach( Property prop, props->getProperties()) 
  {
    m_layoutPropertyBrowser->addProperty(prop);
  }
   
  update();
}

void te::layout::PropertiesOutside::onChangePropertyValue( Property property )
{
  if(property.getType() == DataTypeNone)
    return;

  foreach( QGraphicsItem *item, m_graphicsItems) 
  {
    if (item)
    {			
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {
        Properties* props = new Properties("");
        if(props)
        {
          props->setObjectName(lItem->getProperties()->getObjectName());
          props->setTypeObj(lItem->getProperties()->getTypeObj());
          props->addProperty(property);

          lItem->updateProperties(props);
          delete props;
          props = 0;
        }       
      }
    }
  }
}

void te::layout::PropertiesOutside::closeEvent( QCloseEvent * event )
{
  m_layoutPropertyBrowser->closeAllWindows();
}

te::layout::Properties* te::layout::PropertiesOutside::intersection( QList<QGraphicsItem*> graphicsItems, bool& gridWindow )
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
        gridWindow = props->hasGridWindows();
      }
    }
  }
  else
  {
    props = sameProperties(graphicsItems, gridWindow);
  }

  return props;
}

te::layout::Properties* te::layout::PropertiesOutside::sameProperties( QList<QGraphicsItem*> graphicsItems, bool& gridWindow )
{
  Properties* props = 0;
  std::vector<Properties*> propsVec = getAllProperties(graphicsItems, gridWindow);

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
  te::layout::PropertiesOutside::getAllProperties( QList<QGraphicsItem*> graphicsItems, bool& gridWindow )
{
  Properties* props = 0;
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
            result = propsItem->hasGridWindows();
          }
        }
      }
    }
  }

  gridWindow = result;
  return propsVec;
}
