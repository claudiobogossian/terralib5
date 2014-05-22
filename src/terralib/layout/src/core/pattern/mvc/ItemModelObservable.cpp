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
  \file ItemModelObservable.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemModelObservable.h"
#include "Observer.h"
#include "Property.h"
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/Coord2D.h"
#include "Properties.h"
#include "SharedProperties.h"

te::layout::ItemModelObservable::ItemModelObservable() :
  m_id(0),
  m_name("unknown"),
  m_type(TPObjectUnknown),
  m_sharedProps(0),
  m_zValue(0),
  m_border(true)
{
  m_box = te::gm::Envelope(0,0,0,0);

  m_backgroundColor = te::color::RGBAColor(255,255,255, 0);

  m_properties = new Properties(m_name);

  m_sharedProps = new SharedProperties;
}

te::layout::ItemModelObservable::~ItemModelObservable()
{
  if(m_properties)
  {
    delete m_properties;
    m_properties = 0;
  }

  if(m_sharedProps)
  {
    delete m_sharedProps;
    m_sharedProps = 0;
  }
}

bool te::layout::ItemModelObservable::addObserver( Observer* o )
{
  std::pair<std::set<Observer*>::iterator,bool> p = m_observers.insert(o);

  if(p.second == true)
    return true;

  return false;
}

bool te::layout::ItemModelObservable::removeObserver( Observer* o )
{
  int num = m_observers.erase(o);

  if(num == 1)
    return true;

  return false;
}

void te::layout::ItemModelObservable::notifyAll( ContextItem context )
{
  std::set<Observer*>::iterator it;
  for(it = m_observers.begin(); it != m_observers.end(); ++it)
  {
    (*it)->updateObserver(context);
  }
}

te::layout::Properties* te::layout::ItemModelObservable::getProperties() const
{  
  m_properties->clear();

  Property pro_name;
  pro_name.setName(m_sharedProps->getName());
  pro_name.setId("unknown");
  pro_name.setValue(m_name, DataTypeString);
  m_properties->addProperty(pro_name);

  Property pro_id;
  pro_id.setName(m_sharedProps->getId());
  pro_id.setId("unknown");
  pro_id.setValue(m_id, DataTypeInt);
  m_properties->addProperty(pro_id);
  
  /* Box */

  double x1 = m_box.getLowerLeftX();
  double x2 = m_box.getUpperRightX();
  double y1 = m_box.getLowerLeftY();
  double y2 = m_box.getUpperRightY();

  Property pro_x1;
  pro_x1.setName(m_sharedProps->getX1());
  pro_x1.setId("unknown");
  pro_x1.setValue(x1, DataTypeDouble);
  m_properties->addProperty(pro_x1);

  Property pro_x2;
  pro_x2.setName(m_sharedProps->getX2());
  pro_x2.setId("unknown");
  pro_x2.setValue(x2, DataTypeDouble);
  m_properties->addProperty(pro_x2);

  Property pro_y1;
  pro_y1.setName(m_sharedProps->getY1());
  pro_y1.setId("unknown");
  pro_y1.setValue(y1, DataTypeDouble);
  m_properties->addProperty(pro_y1);

  Property pro_y2;
  pro_y2.setName(m_sharedProps->getY2());
  pro_y2.setId("unknown");
  pro_y2.setValue(y2, DataTypeDouble);
  m_properties->addProperty(pro_y2);

  /* ---------- */

  Property pro_zValue;
  pro_zValue.setName(m_sharedProps->getZValue());
  pro_zValue.setId("unknown");
  pro_zValue.setValue(m_zValue, DataTypeInt);
  m_properties->addProperty(pro_zValue);
  
  Property pro_border;
  pro_border.setName(m_sharedProps->getBorder());
  pro_border.setId("unknown");
  pro_border.setValue(m_border, DataTypeBool);
  m_properties->addProperty(pro_border);

  m_properties->setTypeObj(m_type);
  return m_properties;
}

te::gm::Envelope te::layout::ItemModelObservable::getBox()
{
  return m_box;
}

void te::layout::ItemModelObservable::setBox(te::gm::Envelope box)
{
  m_box = box;
}

te::color::RGBAColor te::layout::ItemModelObservable::getBackgroundColor()
{
  return m_backgroundColor;
}

void te::layout::ItemModelObservable::setBackgroundColor( te::color::RGBAColor color )
{
  m_backgroundColor = color;
}

void te::layout::ItemModelObservable::setBorderColor( te::color::RGBAColor color )
{
  m_borderColor = color;
}

te::color::RGBAColor te::layout::ItemModelObservable::getBorderColor()
{
  return m_borderColor;
}

void te::layout::ItemModelObservable::setName( std::string name )
{
  m_name = name;
  if(m_properties)
    m_properties->setObjectName(m_name);
}

std::string te::layout::ItemModelObservable::getName()
{
  return m_name;
}

void te::layout::ItemModelObservable::setPosition( const double& x, const double& y )
{
  //Initial point to draw is : x1, y1, that corresponds 0,0 of local coordinate of a item  
  double x1 = x; 
  double x2 = x + m_box.getWidth();
  
  double y1 = y;
  double y2 = y + m_box.getHeight();
  
  m_box = te::gm::Envelope(x1, y1, x2, y2);
}

bool te::layout::ItemModelObservable::contains( const te::gm::Coord2D &coord ) const
{
  te::gm::Envelope env(coord.x, coord.y, coord.x, coord.y);

  if(env.isValid())
    return m_box.contains(env);

  return false;
}

void te::layout::ItemModelObservable::updateProperties( te::layout::Properties* properties )
{
  Properties* vectorProps = const_cast<Properties*>(properties);
  
  Property pro_name = vectorProps->contains(m_sharedProps->getName());
  if(!pro_name.isNull())
  {
    m_name = pro_name.getValue().toString();
  }

  Property pro_id = vectorProps->contains(m_sharedProps->getId());
  if(!pro_id.isNull())
  {
    m_id = pro_id.getValue().toInt();
  }

  /* Box */
  
  Property pro_x1 = vectorProps->contains(m_sharedProps->getX1());
  if(!pro_x1.isNull())
  {
    m_box.m_llx = pro_x1.getValue().toDouble();
  }

  Property pro_x2 = vectorProps->contains(m_sharedProps->getX2());
  if(!pro_x2.isNull())
  {
    m_box.m_urx = pro_x2.getValue().toDouble();
  }

  Property pro_y1 = vectorProps->contains(m_sharedProps->getY1());
  if(!pro_y1.isNull())
  {
    m_box.m_lly = pro_y1.getValue().toDouble();
  }

  Property pro_y2 = vectorProps->contains(m_sharedProps->getY2());
  if(!pro_y2.isNull())
  {
    m_box.m_ury = pro_y2.getValue().toDouble();
  }

  Property pro_zValue = vectorProps->contains(m_sharedProps->getZValue());
  if(!pro_zValue.isNull())
  {
    m_zValue = pro_zValue.getValue().toInt();
  }

  Property pro_border = vectorProps->contains(m_sharedProps->getBorder());
  if(!pro_border.isNull())
  {
    m_border = pro_border.getValue().toBool();
  }
}

te::layout::LayoutAbstractObjectType te::layout::ItemModelObservable::getType()
{
  return m_type;
}

void te::layout::ItemModelObservable::setType( LayoutAbstractObjectType type )
{
  m_type = type;
}

int te::layout::ItemModelObservable::getZValue()
{
  return m_zValue;
}

void te::layout::ItemModelObservable::setZValue( int zValue )
{
  m_zValue = zValue;
}

bool te::layout::ItemModelObservable::isBorder()
{
  return m_border;
}

void te::layout::ItemModelObservable::setBorder( bool value )
{
  m_border = value;
}

int te::layout::ItemModelObservable::getId()
{
  return m_id;
}

void te::layout::ItemModelObservable::setId( int id )
{
  m_id = id;
}
