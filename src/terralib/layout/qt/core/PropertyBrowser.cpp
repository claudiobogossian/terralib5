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
#include "../../core/property/Properties.h"

// Qt
#include <QRegExpValidator>
#include <QRegExp>
#include <QWidget>
#include <QVariant>
#include <QFont>
#include <QColor>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/qteditorfactory.h>

// STL
#include <algorithm>    // std::find

te::layout::PropertyBrowser::PropertyBrowser(QObject* parent) :
  QObject(parent),
  m_propertyEditor(0),
  m_variantPropertyEditorManager(0),
  m_strDlgManager(0),
  m_hasWindows(false)
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

  te::color::RGBAColor color;
  QColor qcolor;
  QFont qfont;
  Font font;
  
  switch(property.getType())
  {
  case DataTypeString:
    vproperty = m_variantPropertyEditorManager->addProperty(QVariant::String, tr(property.getName().c_str()));
    vproperty->setValue(property.getValue().toString().c_str());
    break;
  case DataTypeStringList:
    /* The type of property is enum, and so a combobox appears. 
    The type of the property value is int, as is the position in which the attribute is in the list of Enum. */
    vproperty = m_variantPropertyEditorManager->addProperty(QtVariantPropertyManager::enumTypeId(), tr(property.getName().c_str()));
    addAttribute(vproperty, property);
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
    vproperty->setValue(property.getValue().toBool());
    break;
  case DataTypeColor:
    vproperty = m_variantPropertyEditorManager->addProperty(QVariant::Color, tr(property.getName().c_str()));
    color = property.getValue().toColor();
    qcolor.setRed(color.getRed());
    qcolor.setGreen(color.getGreen());
    qcolor.setBlue(color.getBlue());
    qcolor.setAlpha(color.getAlpha());
    vproperty->setValue(qcolor);
    break;
  case DataTypeFont:
    vproperty = m_variantPropertyEditorManager->addProperty(QVariant::Font, tr(property.getName().c_str()));
    font = property.getValue().toFont();
    qfont.setFamily(font.getFamily().c_str());
    qfont.setPointSize(font.getPointSize());
    qfont.setBold(font.isBold());
    qfont.setItalic(font.isItalic());
    qfont.setUnderline(font.isUnderline());
    qfont.setStrikeOut(font.isStrikeout());
    qfont.setKerning(font.isKerning());
    vproperty->setValue(qfont);
    break;
  default:
   vproperty = 0;    
  }

  if(vproperty)
  {
    if(!property.isEditable())
      vproperty->setEnabled(false);

    addPropertyItem(vproperty, QLatin1String(property.getName().c_str()));
    return true;
  }
  return false;
}

void te::layout::PropertyBrowser::addAttribute( QtVariantProperty* vproperty, Property property )
{
  std::vector<Variant> vrt = property.getOptionChoices();
  QStringList  strList;
  foreach( Variant v, vrt) 
  {
    strList.push_back(v.toString().c_str());
  }

  /* "enumNames" is a name used by default in 
  QtVariantProperty class for properties with lists */
  vproperty->setAttribute("enumNames", strList);
}

bool te::layout::PropertyBrowser::removeProperty( Property property )
{
  return true;
}

te::layout::Property te::layout::PropertyBrowser::getProperty( std::string name )
{
  Property prop;
  prop.setName(name);
  
  QVariant variant = findPropertyValue(name);
  QtProperty* property = findProperty(name);
  LayoutPropertyDataType type = getLayoutType(variant.type(), name);
  
  QtVariantProperty* vproperty = 0;
  if(property)
  {
    vproperty = dynamic_cast<QtVariantProperty*>(property);
  }
  
  QColor qcolor;
  te::color::RGBAColor color;
  Variant v;
  QStringList list;
  std::string value;
  Font font;
  QFont qfont;

  switch(type)
  {
  case DataTypeString:
    prop.setValue(variant.toString().toStdString(), type);
    break;
  case DataTypeStringList:

    prop.setValue(variant.toString().toStdString(), type);
    if(vproperty)
    {
      list = variant.toStringList();
      value = list.value(vproperty->value().toInt()).toStdString();

      foreach(QString s, list)
      {
        v.clear();
        v.setValue(s.toStdString(), DataTypeString);
        prop.addOption(v);
        if(value.compare(s.toStdString()) == 0)
        {
          prop.setOptionChoice(v);
        }
      }
    }
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
  case DataTypeColor:
    qcolor = variant.value<QColor>();
    if(qcolor.isValid()) 
    {
      color.setColor(qcolor.red(), qcolor.green(), qcolor.blue(), qcolor.alpha());
      prop.setValue(color, type);
    }
    break;
  case DataTypeFont:
    qfont = variant.value<QFont>();    
    font.setFamily(qfont.family().toStdString());
    font.setPointSize(qfont.pointSize());
    font.setBold(qfont.bold());
    font.setItalic(qfont.italic());
    font.setUnderline(qfont.underline());
    font.setStrikeout(qfont.strikeOut());
    font.setKerning(qfont.kerning());
    prop.setValue(font, type);
    break;    
  default:
    prop.setValue(0, DataTypeNone);
  }

  return prop;
}

QVariant te::layout::PropertyBrowser::findPropertyValue( std::string name )
{
  QVariant variant;
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

QtProperty* te::layout::PropertyBrowser::findProperty( std::string name )
{
  QtProperty* prop = m_idToProperty[name.c_str()];
  return prop;
}

QVariant te::layout::PropertyBrowser::checkComplexType( QtVariantProperty* property )
{
  QVariant variant;

  if(!property)
    return variant;

  variant = property->value();
  
  if(QtVariantPropertyManager::enumTypeId() == property->propertyType())
  {
    variant = property->attributeValue("enumNames");
    QStringList list = variant.toStringList();
    /*QString attr = list.value(property->value().toInt());*/
    variant = QVariant(list);
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
  QVariant variant;
  QtVariantProperty* vproperty = 0;
  int i = 0;

  switch(type)
  {
    case QVariant::String:
      {
        dataType = DataTypeString;
      }
      break;
    case QVariant::StringList:
      vproperty = dynamic_cast<QtVariantProperty*>(m_idToProperty[name.c_str()]);
      if(vproperty)
      {
        if(QtVariantPropertyManager::enumTypeId() == vproperty->propertyType())
        {
          dataType = DataTypeStringList;
        }
      }
      break;
    case QVariant::Double:
      dataType = DataTypeDouble;
      break;
    case QVariant::Int:
      {
        dataType = DataTypeInt;
      }
      break;
    case QVariant::Bool:
      dataType = DataTypeBool;
      break;
    case QVariant::Color:
      dataType = DataTypeColor;
      break;
    case QVariant::Font:
      dataType = DataTypeFont;
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
  case DataTypeStringList:
    type = QVariant::Int;
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
  case DataTypeColor:
    type = QVariant::Color;
    break;
  case DataTypeFont:
    type = QVariant::Font;
    break;
  default:
    type = QVariant::Invalid;
  }

  return type;
}

void te::layout::PropertyBrowser::setHasWindows( bool hasWindows )
{
  m_hasWindows = hasWindows;
  blockOpenWindows(!hasWindows);
}

void te::layout::PropertyBrowser::blockOpenWindows( bool block )
{

}
