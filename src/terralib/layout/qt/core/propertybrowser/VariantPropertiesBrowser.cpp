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
  \file PropertyBrowser.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "VariantPropertiesBrowser.h"
#include "../../../core/property/Properties.h"
#include "../../../core/enum/Enums.h"

// Qt
#include <QVariant>
#include <QFont>
#include <QColor>

te::layout::VariantPropertiesBrowser::VariantPropertiesBrowser(QObject* parent) :
  AbstractPropertiesBrowser(parent),
  m_variantPropertyEditorManager(0),
  m_variantPropertyEditorFactory(0)
{
  createManager();
}

te::layout::VariantPropertiesBrowser::~VariantPropertiesBrowser()
{
  if(m_variantPropertyEditorManager)
  {
    delete m_variantPropertyEditorManager;
    m_variantPropertyEditorManager = 0;
  }

  if(m_variantPropertyEditorFactory)
  {
    delete m_variantPropertyEditorFactory;
    m_variantPropertyEditorFactory = 0;
  }
}

void te::layout::VariantPropertiesBrowser::createManager()
{  
  m_variantPropertyEditorManager = new QtVariantPropertyManager; 
  m_variantPropertyEditorFactory = new QtVariantEditorFactory;
}

QtVariantProperty* te::layout::VariantPropertiesBrowser::addProperty( Property property )
{
  QtVariantProperty* vproperty = 0;

  if(!property.isVisible())
  {
    return vproperty;
  }

  te::color::RGBAColor color;
  QColor qcolor;
  QFont qfont;
  Font font;

  if(!m_variantPropertyEditorManager)
  {
    return vproperty;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
  {
    return vproperty;
  }

  int type = getVariantType(property.getType());
  vproperty = m_variantPropertyEditorManager->addProperty(type, tr(property.getName().c_str()));
  changeQtVariantPropertyValue(vproperty, property);

  addPropertyItem(vproperty, QLatin1String(property.getName().c_str()));

  return vproperty;
}

void te::layout::VariantPropertiesBrowser::addAttribute( QtVariantProperty* vproperty, Property property )
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

te::layout::Property te::layout::VariantPropertiesBrowser::getProperty( std::string name )
{
  Property prop;
  prop.setName(name);
  
  QVariant variant = findPropertyValue(name);

  if(variant.isNull() || !variant.isValid())
  {
    return prop;
  }

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
  else if(type == dataType->getDataTypeEnvelope())
  {
    QRectF rect = variant.value<QRectF>();

    te::gm::Envelope envelope(rect.left(), rect.bottom(), rect.right(), rect.top());
    prop.setValue(envelope, dataType->getDataTypeEnvelope());
  }

  return prop;
}

te::layout::EnumType* te::layout::VariantPropertiesBrowser::getLayoutType( QVariant::Type type, std::string name )
{
  EnumDataType* dtType = Enums::getInstance().getEnumDataType();

  EnumType* dataType = dtType->getDataTypeNone();
  QVariant variant;
  QtVariantProperty* vproperty = 0;
  QtProperty* prop = 0;

  switch(type)
  {
    case QVariant::String:
      {
        dataType = dtType->getDataTypeString();
      }
      break;
    case QVariant::StringList:
      prop = findProperty(name);
      vproperty = dynamic_cast<QtVariantProperty*>(prop);
      if(vproperty)
      {
        if(QtVariantPropertyManager::enumTypeId() == vproperty->propertyType())
        {
          dataType = dtType->getDataTypeStringList();
        }
        else if(QtVariantPropertyManager::groupTypeId() == vproperty->propertyType())
        {
          dataType = dtType->getDataTypeGroup();
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
    case QVariant::RectF:
      dataType = dtType->getDataTypeEnvelope();
      break;
    default:
      dataType = dtType->getDataTypeNone();
  }

  return dataType;
}

int te::layout::VariantPropertiesBrowser::getVariantType( te::layout::EnumType* dataType )
{
  int type = QVariant::Invalid;

  EnumDataType* dtType = Enums::getInstance().getEnumDataType();
  
  if(!dtType)
  {
    return type;
  }

  if(dataType == dtType->getDataTypeString())
  {
    type = QVariant::String;
  }
  else if(dataType == dtType->getDataTypeStringList())
  {
    type = QtVariantPropertyManager::enumTypeId();
  }
  else if(dataType == dtType->getDataTypeGroup())
  {
    type = QtVariantPropertyManager::groupTypeId();
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
  else if(dataType == dtType->getDataTypeEnvelope())
  {
    type = QVariant::RectF;
  }

  return type;
}

bool te::layout::VariantPropertiesBrowser::changeQtVariantPropertyValue( QtVariantProperty* vproperty, Property property )
{
  bool result = true;

  QColor qcolor;
  QFont qfont;
  te::color::RGBAColor color;
  Font font;

  m_changeProperty = true;
  
  if(!vproperty)
  {
    return false;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  if(property.getType() == dataType->getDataTypeString())
  {
    vproperty->setValue(property.getValue().toString().c_str());    
  }
  else if(property.getType() == dataType->getDataTypeStringList())
  {    
    addAttribute(vproperty, property);

    std::string currentValue = property.getOptionByCurrentChoice().toString();

    std::vector<Variant> vecChoices = property.getOptionChoices();
    for(size_t i = 0; i < vecChoices.size(); ++i)
    {
      if(currentValue == vecChoices[i].toString())
      {
        vproperty->setValue(i);
        break;
      }
    }
  }
  else if(property.getType() == dataType->getDataTypeGroup())
  {    
    vproperty->setValue(property.getValue().toString().c_str());  
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
  else if(property.getType() == dataType->getDataTypeEnvelope())
  {
    const te::gm::Envelope& envelope  = property.getValue().toEnvelope();
    QRectF rect(envelope.getLowerLeftX(), envelope.getUpperRightY(), envelope.getWidth(), envelope.getHeight());

    vproperty->setValue(rect);
  }
  else
  {
    result = false;
  }

  m_changeProperty = false;

  return result;
}

QtVariantEditorFactory* te::layout::VariantPropertiesBrowser::getVariantEditorFactory()
{
  return m_variantPropertyEditorFactory;
}

QtVariantPropertyManager* te::layout::VariantPropertiesBrowser::getVariantPropertyManager()
{
  return m_variantPropertyEditorManager;
}

bool te::layout::VariantPropertiesBrowser::updateProperty( Property property )
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




