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
  \file PropertyBrowser.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractPropertiesBrowser.h"
#include "../../../core/property/Properties.h"
#include "../../../core/enum/Enums.h"
#include "../../../core/property/Variant.h"

// Qt
#include <QVariant>
#include <QFont>
#include <QColor>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtVariantProperty>
#include <QtPropertyBrowser/qteditorfactory.h>
#include <QtPropertyBrowser/QtProperty>

// STL
#include <algorithm>    // std::find

te::layout::AbstractPropertiesBrowser::AbstractPropertiesBrowser(QObject* parent) :
  QObject(parent),
  m_changeProperty(false)
{
  
}

te::layout::AbstractPropertiesBrowser::~AbstractPropertiesBrowser()
{
 
}

void te::layout::AbstractPropertiesBrowser::addPropertyItem( QtProperty *property, const QString &id )
{
  m_propertyToId[property] = id;
  m_idToProperty[id] = property;
}

void te::layout::AbstractPropertiesBrowser::clearAll()
{
  m_propertyToId.clear();
  m_idToProperty.clear();
}

QVariant te::layout::AbstractPropertiesBrowser::findPropertyValue( std::string name )
{
  QVariant variant;

  if(!m_idToProperty.contains(name.c_str()))
  {
    return variant;
  }

  QtVariantProperty* vproperty = 0;
  QtProperty* prop = m_idToProperty[name.c_str()];

  if(prop)
  {
    vproperty = dynamic_cast<QtVariantProperty*>(prop);
    if(vproperty)
    {
      variant = checkComplexType(vproperty);
    }
    else
    {
      variant.setValue(prop->valueText());
    }
  }

  return variant;
}

QtProperty* te::layout::AbstractPropertiesBrowser::findProperty( std::string name )
{
  QtProperty* prop = 0;

  if(!m_idToProperty.contains(name.c_str()))
  {
    return prop;  
  }

  prop = m_idToProperty[name.c_str()];
  return prop;
}

QVariant te::layout::AbstractPropertiesBrowser::checkComplexType( QtVariantProperty* property )
{
  QVariant variant;

  if(!property)
    return variant;

  variant = property->value();

  if(QtVariantPropertyManager::enumTypeId() == property->propertyType())
  {
    variant = property->attributeValue("enumNames");
    QStringList list = variant.toStringList();
    variant = QVariant(list);
  }

  return variant;
}

bool te::layout::AbstractPropertiesBrowser::removeProperty( QtProperty* prop )
{
  if(!prop)
  {
    return false;
  }

  if(!m_idToProperty.contains(prop->propertyName()))
  {
    return false;  
  }

  m_propertyToId.remove(prop);
  m_idToProperty.remove(prop->propertyName());

  return true;
}









