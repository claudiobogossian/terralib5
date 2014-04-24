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
#include "PropertyBrowser.h"
#include "Properties.h"

// Qt
#include <QRegExpValidator>
#include <QRegExp>
#include <QWidget>
#include "../../../../../../third-party/qt/propertybrowser/qtvariantproperty.h"
#include "../../../../../../third-party/qt/propertybrowser/qteditorfactory.h"

// STL
#include <algorithm>    // std::find

te::layout::PropertyBrowser::PropertyBrowser(QObject* parent) :
  QObject(parent),
  m_propertyEditor(0),
  m_variantPropertyEditorManager(0),
  m_strDlgManager(0)
{
  createManager();
}

te::layout::PropertyBrowser::~PropertyBrowser()
{
  if(m_propertyEditor)
  {
    delete m_propertyEditor;
    m_propertyEditor = 0;
  }

  if(m_variantPropertyEditorManager)
  {
    delete m_variantPropertyEditorManager;
    m_variantPropertyEditorManager = 0;
  }

  if(m_strDlgManager)
  {
    delete m_strDlgManager;
    m_strDlgManager = 0;
  }
}

void te::layout::PropertyBrowser::createManager()
{
  //Qt - The Property Browser
  m_propertyEditor = new QtTreePropertyBrowser;

  m_variantPropertyEditorManager = new QtVariantPropertyManager;
  connect(m_variantPropertyEditorManager, SIGNAL(valueChanged(QtProperty*, const QVariant &)),
    this, SLOT(propertyEditorValueChanged(QtProperty *, const QVariant &)));

  m_strDlgManager = new QtStringPropertyManager();

  QtVariantEditorFactory* variantPropertyEditorFactory = new QtVariantEditorFactory;

  QtDlgEditorFactory* dlgFactory = new QtDlgEditorFactory();

  connect(dlgFactory, SIGNAL(internalDlg(QWidget *, QtProperty *)), this, SLOT(onSetDlg(QWidget *, QtProperty *)));

  m_propertyEditor->setFactoryForManager(m_strDlgManager, dlgFactory);
  m_propertyEditor->setFactoryForManager(m_variantPropertyEditorManager, variantPropertyEditorFactory);
  m_propertyEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_propertyEditor->setResizeMode(QtTreePropertyBrowser::ResizeToContents);
}

void te::layout::PropertyBrowser::propertyEditorValueChanged( QtProperty *property, const QVariant &value )
{
  QList<QtBrowserItem *> list = m_propertyEditor->items(property);
  changePropertyValue(property, list);

  Property prop = getProperty(property->propertyName().toStdString());
  changePropertyValue(prop);
}

void te::layout::PropertyBrowser::updateExpandState()
{
  QList<QtBrowserItem *> list = m_propertyEditor->topLevelItems();
  QListIterator<QtBrowserItem *> it(list);
  while (it.hasNext()) {
    QtBrowserItem *item = it.next();
    QtProperty *prop = item->property();
    m_idToExpanded[m_propertyToId[prop]] = m_propertyEditor->isExpanded(item);
  }
}

void te::layout::PropertyBrowser::clearAll()
{
  updateExpandState();

  QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToId.constBegin();
  while (itProp != m_propertyToId.constEnd()) {
    delete itProp.key();
    itProp++;
  }
  m_propertyToId.clear();
  m_idToProperty.clear();
}

void te::layout::PropertyBrowser::addPropertyItem(QtProperty *property, const QString &id)
{
  m_propertyToId[property] = id;
  m_idToProperty[id] = property; 
  QtBrowserItem *item = m_propertyEditor->addProperty(property);
  if (m_idToExpanded.contains(id))
    m_propertyEditor->setExpanded(item, m_idToExpanded[id]);
}

void te::layout::PropertyBrowser::onChangeFilter( const QString& filter )
{
  QRegExp rx;
  QString search_text = filter;
  bool doesContain = false;
  search_text.replace(" ","|"); // to make it possible to look up all words given for the search
  rx.setPattern(search_text);
  rx.setCaseSensitivity(Qt::CaseInsensitive);
  
  QList<QtProperty*> list = m_propertyEditor->properties();
  foreach( QtProperty* prop, list) 
  {
    doesContain = false;
    if(prop) 
    {
      doesContain = prop->propertyName().contains(rx);
      changeVisibility(m_propertyEditor->items(prop), doesContain);
    }
  }
}

void te::layout::PropertyBrowser::changeVisibility( QList<QtBrowserItem*> items, bool visible )
{
  foreach(QtBrowserItem* item, items) 
  {
    if(item)
    {
      m_propertyEditor->setItemVisible(item, visible);
    }
  }
}

QtTreePropertyBrowser* te::layout::PropertyBrowser::getPropertyEditor()
{
  return m_propertyEditor;
}

QtVariantPropertyManager* te::layout::PropertyBrowser::getVariantPropertyManager()
{
  return m_variantPropertyEditorManager;
}

bool te::layout::PropertyBrowser::addProperty( Property property )
{
  QtVariantProperty* vproperty = 0;

  switch(property.getType())
  {
  case DataTypeString:
    vproperty = m_variantPropertyEditorManager->addProperty(QVariant::String, tr(property.getName().c_str()));
    vproperty->setValue(property.getValue().toString().c_str());
    break;
  case DataTypeDouble:
    vproperty = m_variantPropertyEditorManager->addProperty(QVariant::Double, tr(property.getName().c_str()));
    vproperty->setValue(property.getValue().toDouble());
    break;
  case DataTypeInt:
    vproperty = m_variantPropertyEditorManager->addProperty(QVariant::Int, tr(property.getName().c_str()));
    vproperty->setValue(property.getValue().toInt());
    break;
  case DataTypeBool:
    vproperty = m_variantPropertyEditorManager->addProperty(QVariant::Bool, tr(property.getName().c_str()));
    vproperty->setValue(property.getValue().toInt());
    break;
  default:
   vproperty = 0;    
  }

  if(vproperty)
  {
    addPropertyItem(vproperty, QLatin1String(property.getName().c_str()));
    return true;
  }
  return false;
}

bool te::layout::PropertyBrowser::removeProperty( Property property )
{
  return true;
}

te::layout::Property te::layout::PropertyBrowser::getProperty( std::string name )
{
  Property prop;
  prop.setName(name);
  
  QVariant variant = findProperty(name);
  LayoutPropertyDataType type = getLayoutType(variant.type());
  
  switch(type)
  {
  case DataTypeString:
    prop.setValue(variant.toString(), type);
    break;
  case DataTypeDouble:
    prop.setValue(variant.toDouble(), type);
    break;
  case DataTypeInt:
    prop.setValue(variant.toInt(), type);
    break;
  case DataTypeBool:
    prop.setValue(variant.toBool(), type);
    break;
  case DataTypeGridSettings:
    prop.setValue(variant.toString(), type);
    break;
  default:
    prop.setValue(0, DataTypeNone);
  }

  return prop;
}

QVariant te::layout::PropertyBrowser::findProperty( std::string name )
{
  QVariant variant;
  QtVariantProperty* vproperty = 0;

  QList<QtProperty*> props = m_propertyEditor->properties();
  foreach(QtProperty* prop, props)
  {
    if(name.compare(prop->propertyName().toStdString()) == 0)
    {
      QtVariantProperty* vproperty = dynamic_cast<QtVariantProperty*>(prop);

      if(vproperty)
      {
        variant = vproperty->value();
      }
      else
      {
        variant.setValue(prop->valueText());
      }

      return variant;
    }
  }
  return variant;
}

te::layout::Properties* te::layout::PropertyBrowser::getProperties()
{
  Properties* properties = new Properties("");

  QList<QtProperty*> props = m_propertyEditor->properties();
  foreach( QtProperty* prop, props) 
  {
    Property property = getProperty(prop->propertyName().toStdString());
    properties->addProperty(property);
  }

  return properties;
}

te::layout::LayoutPropertyDataType te::layout::PropertyBrowser::getLayoutType( QVariant::Type type, std::string name )
{
  LayoutPropertyDataType dataType = DataTypeNone;
  switch(type)
  {
    case QVariant::String:
      {
        dataType = DataTypeString;

        //Custom types: Dialog Window Type
        if(name.compare("") != 0)
        {
          QVariant variant = m_strDlgManager->property(name.c_str());
          if(!variant.isNull())
          {
            if(name.compare(m_propGridSettingsName) == 0)
            {
              dataType = DataTypeGridSettings;
            }
          }
        }
      }
      break;
    case QVariant::Double:
      dataType = DataTypeDouble;
      break;
    case QVariant::Int:
      dataType = DataTypeInt;
      break;
    case QVariant::Bool:
      dataType = DataTypeBool;
      break;
    default:
      dataType = DataTypeNone;
  }

  return dataType;
}

QVariant::Type te::layout::PropertyBrowser::getVariantType( LayoutPropertyDataType dataType )
{
  QVariant::Type type = QVariant::Invalid;
  switch(dataType)
  {
  case DataTypeString:
    type = QVariant::String;
    break;
  case DataTypeDouble:
    type = QVariant::Double;
    break;
  case DataTypeInt:
    type = QVariant::Int;
    break;
  case DataTypeBool:
    type = QVariant::Bool;
    break;
  case DataTypeGridSettings:
    type = QVariant::String;
    break;
  default:
    type = QVariant::Invalid;
  }

  return type;
}

std::string te::layout::PropertyBrowser::getPropGridSettingsName()
{
  return m_propGridSettingsName;
}
