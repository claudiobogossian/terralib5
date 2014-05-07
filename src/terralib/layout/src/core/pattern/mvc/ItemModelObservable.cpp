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
  m_sharedProps(0)
{
  m_box = te::gm::Envelope(0,0,0,0);

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

  Property pro_id;
  pro_id.setName(m_sharedProps->getId());
  pro_id.setId("unknown");
  pro_id.setValue(m_id, DataTypeInt);

  m_properties->addProperty(pro_name);
  m_properties->addProperty(pro_id);

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
  
  Property pro_name = vectorProps->contains(m_name);

  if(!pro_name.isNull())
  {
    m_name = pro_name.getValue().toString();
  }

  Property pro_id = vectorProps->contains("id");

  if(!pro_id.isNull())
  {
    m_id = pro_id.getValue().toInt();
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
