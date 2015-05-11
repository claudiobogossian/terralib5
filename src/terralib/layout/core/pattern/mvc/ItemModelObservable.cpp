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
  \file ItemModelObservable.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemModelObservable.h"
#include "Observer.h"
#include "../../property/Property.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/Coord2D.h"
#include "../../property/Properties.h"
#include "../../property/SharedProperties.h"
#include "../singleton/Context.h"
#include "../../enum/Enums.h"
#include "ItemObserver.h"

// STL
#include <ctime>
#include <iostream>

te::layout::ItemModelObservable::ItemModelObservable() :
  m_id(0),
  m_publicProperties(0),
  m_type(0),
  m_zValue(0),
  m_sharedProps(0),
  m_border(true),
  m_name("unknown"),
  m_resizable(true),
  m_angle(0),
  m_hashCode(0),
  m_oldAngle(0),
  m_enableChildren(false)
{
  EnumObjectType* type = Enums::getInstance().getEnumObjectType();
  m_type = type->getObjectUnknown();

  m_box = te::gm::Envelope(0,0,0,0);

  m_oldPos.x = 0.;
  m_oldPos.y = 0.;

  m_backgroundColor = te::color::RGBAColor(255, 255, 255, 0);

  m_borderColor = te::color::RGBAColor(0, 0, 0, 255);
  
  m_sharedProps = new SharedProperties;

  m_properties = new Properties(m_name);

  m_hashCode = calculateHashCode();
  m_properties->setHashCode(m_hashCode);

  m_publicProperties = new Properties(m_name, 0, m_hashCode);
}

te::layout::ItemModelObservable::~ItemModelObservable()
{
  if(m_properties)
  {
    delete m_properties;
    m_properties = 0;
  }

  m_children.clear();

  if(m_publicProperties)
  {
    delete m_publicProperties;
    m_publicProperties = 0;
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
  if(!m_properties)
  {
    return 0;
  }

  m_properties->clear();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_name(m_hashCode);
  pro_name.setName(m_sharedProps->getName());
  pro_name.setEditable(false);
  pro_name.setValue(m_name, dataType->getDataTypeString());
  m_properties->addProperty(pro_name);

  Property pro_id(m_hashCode);
  pro_id.setName(m_sharedProps->getId());
  pro_id.setValue(m_id, dataType->getDataTypeInt());
  m_properties->addProperty(pro_id);

  Property pro_angle(m_hashCode);
  pro_angle.setName(m_sharedProps->getAngle());
  pro_angle.setValue(m_angle, dataType->getDataTypeDouble());
  m_properties->addProperty(pro_angle);

  Property pro_backgroundcolor(m_hashCode);
  pro_backgroundcolor.setName(m_sharedProps->getBackgroundcolor());
  pro_backgroundcolor.setValue(m_backgroundColor, dataType->getDataTypeColor());
  pro_backgroundcolor.setMenu(true);
  m_properties->addProperty(pro_backgroundcolor);

  Property pro_bordercolor(m_hashCode);
  pro_bordercolor.setName(m_sharedProps->getBordercolor());
  pro_bordercolor.setValue(m_borderColor, dataType->getDataTypeColor());
  pro_bordercolor.setMenu(true);
  m_properties->addProperty(pro_bordercolor);
  
  /* Box */

  double x1 = m_box.getLowerLeftX();
  double y1 = m_box.getLowerLeftY();
  double width = m_box.getWidth();
  double height = m_box.getHeight();

  Property pro_x1(m_hashCode);
  pro_x1.setName(m_sharedProps->getX1());
  pro_x1.setValue(x1, dataType->getDataTypeDouble());
  pro_x1.setEditable(false);
  m_properties->addProperty(pro_x1);

  Property pro_y1(m_hashCode);
  pro_y1.setName(m_sharedProps->getY1());
  pro_y1.setValue(y1, dataType->getDataTypeDouble());
  pro_y1.setEditable(false);
  m_properties->addProperty(pro_y1);

  Property pro_width(m_hashCode);
  pro_width.setName(m_sharedProps->getWidth());
  pro_width.setValue(width, dataType->getDataTypeDouble());
  pro_width.setEditable(false);
  m_properties->addProperty(pro_width);

  Property pro_height(m_hashCode);
  pro_height.setName(m_sharedProps->getHeight());
  pro_height.setValue(height, dataType->getDataTypeDouble());
  pro_height.setEditable(false);
  m_properties->addProperty(pro_height);

  /* ---------- */

  Property pro_zValue(m_hashCode);
  pro_zValue.setName(m_sharedProps->getZValue());
  pro_zValue.setValue(m_zValue, dataType->getDataTypeInt());
  pro_zValue.setEditable(false);
  m_properties->addProperty(pro_zValue);
  
  Property pro_border(m_hashCode);
  pro_border.setName(m_sharedProps->getBorder());
  pro_border.setValue(m_border, dataType->getDataTypeBool());
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

  if(m_publicProperties)
    m_publicProperties->setObjectName(m_name);
}

std::string te::layout::ItemModelObservable::getName()
{
  return m_name;
}

void te::layout::ItemModelObservable::setPosition( const double& x, const double& y )
{
  m_oldPos.x = m_box.m_llx;
  m_oldPos.y = m_box.m_lly;

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

  if(!vectorProps)
  {
    return;
  }
  
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

  Property pro_angle = vectorProps->contains(m_sharedProps->getAngle());
  if(!pro_angle.isNull())
  {
    m_oldAngle = m_angle;
    m_angle = pro_angle.getValue().toDouble();
  }

  Property pro_backgroundcolor = vectorProps->contains(m_sharedProps->getBackgroundcolor());
  if(!pro_backgroundcolor.isNull())
  {
    m_backgroundColor = pro_backgroundcolor.getValue().toColor();
  }

  Property pro_bordercolor = vectorProps->contains(m_sharedProps->getBordercolor());
  if(!pro_bordercolor.isNull())
  {
    m_borderColor = pro_bordercolor.getValue().toColor();
  }

  /* Box */
  
  Property pro_x1 = vectorProps->contains(m_sharedProps->getX1());
  if(!pro_x1.isNull())
  {
    m_oldPos.x = m_box.m_llx;
    m_box.m_llx = pro_x1.getValue().toDouble();
  }

  Property pro_y1 = vectorProps->contains(m_sharedProps->getY1());
  if(!pro_y1.isNull())
  {
    m_oldPos.y = m_box.m_lly;
    m_box.m_lly = pro_y1.getValue().toDouble();
  }

  Property pro_width = vectorProps->contains(m_sharedProps->getWidth());
  if(!pro_width.isNull())
  {
    m_box.m_urx = m_box.m_llx + pro_width.getValue().toDouble();
  }

  Property pro_height = vectorProps->contains(m_sharedProps->getHeight());
  if(!pro_height.isNull())
  {
    m_box.m_ury = m_box.m_lly + pro_height.getValue().toDouble();
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

te::layout::EnumType* te::layout::ItemModelObservable::getType()
{
  return m_type;
}

void te::layout::ItemModelObservable::setType( EnumType* type )
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

void te::layout::ItemModelObservable::resize( double w, double h )
{
  if((w <= 0) || (h <= 0))
    return;

  if(w == m_box.getWidth() && h == m_box.getHeight())
    return;

  m_box.m_urx = m_box.m_llx + w;
  m_box.m_ury = m_box.m_lly + h;
}

void te::layout::ItemModelObservable::setResizable( bool resize )
{
  m_resizable = resize;
}

bool te::layout::ItemModelObservable::isResizable()
{
  return m_resizable;
}

void te::layout::ItemModelObservable::drawBackground( ContextItem context )
{
  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();

  if((!canvas) || (!utils))
    return;

  if(m_border)
  {
    canvas->setPolygonContourWidth(1);
    canvas->setPolygonContourColor(m_borderColor);
  }
  else
  {
    canvas->setPolygonContourColor(te::color::RGBAColor(255,255,255, 0));
  }
  canvas->setPolygonFillColor(m_backgroundColor);

  te::gm::Envelope box(m_box);
  box.m_llx += 0.4;
  box.m_lly += 0.4;
  box.m_urx -= 0.2;

  utils->drawRectW(box);
}

void te::layout::ItemModelObservable::setAngle( double angle )
{
  m_oldAngle = m_angle;
  m_angle = angle;
}

double te::layout::ItemModelObservable::getAngle()
{
  return m_angle;
}

int te::layout::ItemModelObservable::getHashCode()
{
  return m_hashCode;
}

int te::layout::ItemModelObservable::calculateHashCode()
{
  int nameLength = m_name.length();
  int id = m_id;
  int type = 0;
  if(m_type)
  {
    type = m_type->getId();
  }

  int propertiesLength = 0;

  if(m_properties)
  {
    Properties* props = getProperties();

    if(props)
    {
      propertiesLength = getProperties()->getProperties().size();
    }
  }

  int hashcode = (nameLength + id + type + propertiesLength) * 5;

  // current date/time based on current system
  std::time_t now = std::time(0);
  
  std::tm *ltm = localtime(&now);

  if(!ltm)
    return hashcode;

  // print various components of tm structure.
  int year = 1900 + ltm->tm_year;
  int month = 1 + ltm->tm_mon;
  int day = ltm->tm_mday;
  int hour = 1 + ltm->tm_hour;
  int numberSecond = 1 + ltm->tm_min;
  int numberMilliseconds =  1 + ltm->tm_sec;

  hashcode *= year + month + day;
  hashcode *= hour + numberSecond + numberMilliseconds;

  return hashcode;
}

double te::layout::ItemModelObservable::getOldAngle()
{
  return m_oldAngle;
}

te::gm::Coord2D te::layout::ItemModelObservable::getOldPos()
{
  return m_oldPos;
}

void te::layout::ItemModelObservable::draw( ContextItem context )
{

}

bool te::layout::ItemModelObservable::isEnableChildren()
{
  return m_enableChildren;
}

void te::layout::ItemModelObservable::setEnableChildren( bool value )
{
  m_enableChildren = value;
}

std::set<te::layout::ItemObserver*> te::layout::ItemModelObservable::getChildren() const
{
  return m_children;
}

bool te::layout::ItemModelObservable::addChildren( ItemObserver* item )
{
  bool result = false;
  if(!item)
  {
    return result;
  }

  if(!m_enableChildren)
  {
    return result;
  }

  if(item->getModel()->getHashCode() == m_hashCode)
  {
    return result;
  }

  std::pair<std::set<ItemObserver*>::iterator,bool> p = m_children.insert(item);

  if(p.second == true)
  {
    result = true;
  }

  return result;
}

bool te::layout::ItemModelObservable::removeChildren( int hashCode )
{
  bool result = false;
  std::set<ItemObserver*>::iterator it = m_children.begin();
  
  for( ; it != m_children.end(); it++)
  {
    if((*it)->getModel()->getHashCode() == hashCode)
    {
      int num = m_children.erase(*it);

      if(num == 1)
      {
        result = true;
        break;
      }
    }
  }
  return result;
}

te::layout::Properties* te::layout::ItemModelObservable::getPublicProperties() const
{
  if(!m_properties || m_publicProperties)
  {
    return 0;
  }

  m_publicProperties->clear();

  std::vector<Property>::iterator it = m_properties->getProperties().begin();

  for( ; it != m_properties->getProperties().end() ; ++it )
  {
    if((*it).isPublic())
    {
      m_publicProperties->addProperty(*it);
    }
  }

  m_publicProperties->setTypeObj(m_type);

  return m_publicProperties;
}

void te::layout::ItemModelObservable::addChildrenProperties( Properties* properties )
{
  std::set<ItemObserver*>::iterator it = m_children.begin();

  for( ; it != m_children.end(); it++)
  {
    Properties* props = (*it)->getModel()->getPublicProperties();
    
    std::vector<Property>::iterator itProp = props->getProperties().begin();
    for( ; itProp != props->getProperties().end(); itProp++)
    {
      properties->addProperty(*itProp);
    }
  }
}

void te::layout::ItemModelObservable::updateChildrenProperties( Property prop )
{
  int hashCode = prop.getParentItemHashCode();

  std::set<ItemObserver*>::iterator it = m_children.begin();

  for( ; it != m_children.end(); it++)
  {
    if((*it)->getModel()->getHashCode() == hashCode)
    {
      
    }
  }
}








