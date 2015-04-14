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
#include "../../core/enum/Enums.h"

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
  m_hasWindows(false),
  m_changeQtPropertyVariantValue(false)
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
  if(!m_changeQtPropertyVariantValue)
  {
    QList<QtBrowserItem *> list = m_propertyEditor->items(property);
    changePropertyValue(property, list);

    Property prop = getProperty(property->propertyName().toStdString());
    changePropertyValue(prop);
  }
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

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
  {
    return false;
  }

  if(property.getType() == dataType->getDataTypeStringList())
  {
    /* The type of property is enum, and so a combobox appears. 
    The type of the property value is int, as is the position in which the attribute is in the list of Enum. */
    vproperty = m_variantPropertyEditorManager->addProperty(QtVariantPropertyManager::enumTypeId(), tr(property.getName().c_str()));
    addAttribute(vproperty, property);
    vproperty->setValue(property.getOptionByCurrentChoice().toString().c_str());
  }
  else
  {
    QVariant::Type type = getVariantType(property.getType());
    vproperty = m_variantPropertyEditorManager->addProperty(type, tr(property.getName().c_str()));
    changeQtVariantPropertyValue(vproperty, property);
  }  

  if(vproperty)
  {
    bool is_readOnly = !property.isEditable();
    vproperty->setAttribute(QLatin1String("readOnly"), is_readOnly);
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
  QtProperty* removeProp = 0;
  QList<QtProperty*> list = m_propertyEditor->properties();
  foreach( QtProperty* prop, list) 
  {
    if(property.getName().compare(prop->propertyName().toStdString()) == 0)
    {
      removeProp = prop;
    }
  }
  
  if(!removeProp)
    return false;

  m_propertyToId.remove(removeProp);
  m_idToProperty.remove(removeProp->propertyName());

  m_propertyEditor->removeProperty(removeProp);

  if(removeProp)
  {
    delete removeProp;
    removeProp = 0;
  }

  return true;
}

te::layout::Property te::layout::PropertyBrowser::getProperty( std::string name )
{
  Property prop;
  prop.setName(name);
  
  QVariant variant = findPropertyValue(name);
  QtProperty* property = findProperty(name);
  EnumType* type = getLayoutType(variant.type(), name);
  
  QtVariantProperty* vproperty = 0;
  if(property)
  {
    vproperty = dynamic_cast<QtVariantProperty*>(property);
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  QColor qcolor;
  te::color::RGBAColor color;
  Variant v;
  QStringList list;
  std::string value;
  Font font;
  QFont qfont;

  if(type == dataType->getDataTypeString())
  {
    prop.setValue(variant.toString().toStdString(), type);
  }
  else if(type == dataType->getDataTypeStringList())
  {
    prop.setValue(variant.toString().toStdString(), type);
    if(vproperty)
    {
      list = variant.toStringList();
      value = list.value(vproperty->value().toInt()).toStdString();

      foreach(QString s, list)
      {
        v.clear();
        v.setValue(s.toStdString(), dataType->getDataTypeString());
        prop.addOption(v);
        if(value.compare(s.toStdString()) == 0)
        {
          prop.setOptionChoice(v);
        }
      }
    }
  }
  else if(type == dataType->getDataTypeDouble())
  {
    prop.setValue(variant.toDouble(), type);
  }
  else if(type == dataType->getDataTypeInt())
  {
    prop.setValue(variant.toInt(), type);
  }
  else if(type == dataType->getDataTypeBool())
  {
    prop.setValue(variant.toBool(), type);
  }
  else if(type == dataType->getDataTypeColor())
  {
    qcolor = variant.value<QColor>();
    if(qcolor.isValid()) 
    {
      color.setColor(qcolor.red(), qcolor.green(), qcolor.blue(), qcolor.alpha());
      prop.setValue(color, type);
    }
  }
  else if(type == dataType->getDataTypeFont())
  {
    qfont = variant.value<QFont>();    
    font.setFamily(qfont.family().toStdString());
    font.setPointSize(qfont.pointSize());
    font.setBold(qfont.bold());
    font.setItalic(qfont.italic());
    font.setUnderline(qfont.underline());
    font.setStrikeout(qfont.strikeOut());
    font.setKerning(qfont.kerning());
    prop.setValue(font, type);
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

te::layout::EnumType* te::layout::PropertyBrowser::getLayoutType( QVariant::Type type, std::string name )
{
  EnumDataType* dtType = Enums::getInstance().getEnumDataType();

  EnumType* dataType = dtType->getDataTypeNone();
  QVariant variant;
  QtVariantProperty* vproperty = 0;

  switch(type)
  {
    case QVariant::String:
      {
        dataType = dtType->getDataTypeString();
      }
      break;
    case QVariant::StringList:
      vproperty = dynamic_cast<QtVariantProperty*>(m_idToProperty[name.c_str()]);
      if(vproperty)
      {
        if(QtVariantPropertyManager::enumTypeId() == vproperty->propertyType())
        {
          dataType = dtType->getDataTypeStringList();
        }
      }
      break;
    case QVariant::Double:
      dataType = dtType->getDataTypeDouble();
      break;
    case QVariant::Int:
      {
        dataType = dtType->getDataTypeInt();
      }
      break;
    case QVariant::Bool:
      dataType = dtType->getDataTypeBool();
      break;
    case QVariant::Color:
      dataType = dtType->getDataTypeColor();
      break;
    case QVariant::Font:
      dataType = dtType->getDataTypeFont();
      break;
    default:
      dataType = dtType->getDataTypeNone();
  }

  return dataType;
}

QVariant::Type te::layout::PropertyBrowser::getVariantType( te::layout::EnumType* dataType )
{
  EnumDataType* dtType = Enums::getInstance().getEnumDataType();

  QVariant::Type type = QVariant::Invalid;
  
  if(dataType == dtType->getDataTypeString())
  {
    type = QVariant::String;
  }
  else if(dataType == dtType->getDataTypeStringList())
  {
    type = QVariant::Int;
  }
  else if(dataType == dtType->getDataTypeDouble())
  {
    type = QVariant::Double;
  }
  else if(dataType == dtType->getDataTypeInt())
  {
    type = QVariant::Int;
  }
  else if(dataType == dtType->getDataTypeBool())
  {
    type = QVariant::Bool;
  }
  else if(dataType == dtType->getDataTypeColor())
  {
    type = QVariant::Color;
  }
  else if(dataType == dtType->getDataTypeFont())
  {
    type = QVariant::Font;
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

void te::layout::PropertyBrowser::selectProperty( std::string name )
{
  QList<QtProperty*> props = m_propertyEditor->properties();
  foreach( QtProperty* prop, props) 
  {
    if(prop)
    {
      if(name.compare(prop->propertyName().toStdString()) == 0)
      {
        QList<QtBrowserItem *> list = m_propertyEditor->items(prop);
        QtBrowserItem* item = list.first();
        if(item)
        {
          m_propertyEditor->setCurrentItem(item);
        }
      }
    }
  }
}

bool te::layout::PropertyBrowser::updateProperty( Property property )
{
  std::string name = property.getName();
  QtProperty* qprop = findProperty(name);
  QColor qcolor;
  te::color::RGBAColor color;
  Font font;
  QFont qfont;
  
  QtVariantProperty* vproperty = m_variantPropertyEditorManager->variantProperty(qprop);
  if(!vproperty)
  {
    return false;
  }  

  changeQtVariantPropertyValue(vproperty, property);

  return true;
}

void te::layout::PropertyBrowser::updateProperties( Properties* props )
{
  foreach( Property prop, props->getProperties()) 
  {
    updateProperty(prop);
  }
}

void te::layout::PropertyBrowser::changeQtVariantPropertyValue( QtVariantProperty* vproperty, Property property )
{
  QColor qcolor;
  QFont qfont;
  te::color::RGBAColor color;
  Font font;
  m_changeQtPropertyVariantValue = true;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  if(property.getType() == dataType->getDataTypeString())
  {
    vproperty->setValue(property.getValue().toString().c_str());    
  }
  else if(property.getType() == dataType->getDataTypeStringList())
  {    
    addAttribute(vproperty, property);
    vproperty->setValue(property.getOptionByCurrentChoice().toString().c_str());
  }
  else if(property.getType() == dataType->getDataTypeDouble())
  {
    vproperty->setValue(property.getValue().toDouble());
  }
  else if(property.getType() == dataType->getDataTypeInt())
  {
    vproperty->setValue(property.getValue().toInt());
  }
  else if(property.getType() == dataType->getDataTypeBool())
  {
    vproperty->setValue(property.getValue().toBool());
  }
  else if(property.getType() == dataType->getDataTypeColor())
  {
    color = property.getValue().toColor();
    qcolor.setRed(color.getRed());
    qcolor.setGreen(color.getGreen());
    qcolor.setBlue(color.getBlue());
    qcolor.setAlpha(color.getAlpha());
    vproperty->setValue(qcolor);
  }
  else if(property.getType() == dataType->getDataTypeFont())
  {
    font = property.getValue().toFont();
    qfont.setFamily(font.getFamily().c_str());
    qfont.setPointSize(font.getPointSize());
    qfont.setBold(font.isBold());
    qfont.setItalic(font.isItalic());
    qfont.setUnderline(font.isUnderline());
    qfont.setStrikeOut(font.isStrikeout());
    qfont.setKerning(font.isKerning());
    vproperty->setValue(qfont);    
  }
  
  m_changeQtPropertyVariantValue = false;
}



