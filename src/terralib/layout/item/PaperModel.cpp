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
  \file PaperModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PaperModel.h"
#include "../core/enum/Enums.h"

te::layout::PaperModel::PaperModel() 
  : AbstractItemModel()
{
  this->m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getPaperItem());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  te::color::RGBAColor paperColor(255, 255, 255, 255);
  te::color::RGBAColor shadowColor(0, 0, 0, 255);
  te::color::RGBAColor frameColor(255, 255, 255, 0);
  double shadowPadding = 10.;


  double paperWidth = 0.;
  double paperHeight = 0.;
  LayoutAbstractPaperType paperType = A4;
  LayoutOrientationType paperOrientation = Portrait;

  te::gm::Envelope box(0., - shadowPadding, paperWidth + shadowPadding, paperHeight);

  //adding properties
  {
    Property property(0);
    property.setName("paper_color");
    property.setValue(paperColor, dataType->getDataTypeColor());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("shadow_color");
    property.setValue(shadowColor, dataType->getDataTypeColor());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("shadow_padding");
    property.setValue(shadowPadding, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_width");
    property.setValue(paperWidth, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_height");
    property.setValue(paperHeight, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_type");
    property.setValue((int)paperType, dataType->getDataTypeInt());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_orientation");
    property.setValue((int)paperOrientation, dataType->getDataTypeInt());
    m_properties.addProperty(property);
  }


  //updating properties
  {
    Property property(0);
    property.setName("frame_color");
    property.setValue(frameColor, dataType->getDataTypeColor());
    m_properties.updateProperty(property);
  }
}

te::layout::PaperModel::~PaperModel()
{

}

void te::layout::PaperModel::setProperty(const Property& property)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pCurrentPaperWidth = m_properties.getProperty("paper_width");
  const Property& pCurrentPaperHeight = m_properties.getProperty("paper_height");

  double currentPaperWidth = pCurrentPaperWidth.getValue().toDouble();
  double currentPaperHeight = pCurrentPaperHeight.getValue().toDouble();

  if(property.getName() == "paper_width")
  {
    double newPaperWidth = property.getValue().toDouble();

    Properties properties = handleNewPaperSize(newPaperWidth, currentPaperHeight);
    properties.addProperty(property);

    AbstractItemModel::setProperties(properties);
    return;
  }
  else if(property.getName() == "paper_height")
  {
    double newPaperHeight = property.getValue().toDouble();

    Properties properties = handleNewPaperSize(currentPaperWidth, newPaperHeight);
    properties.addProperty(property);

    AbstractItemModel::setProperties(properties);
    return;
  }

  AbstractItemModel::setProperty(property);
}

void te::layout::PaperModel::setProperties(const Properties& properties)
{
  const Property& pCurrentPaperWidth = m_properties.getProperty("paper_width");
  const Property& pCurrentPaperHeight = m_properties.getProperty("paper_height");

  double currentPaperWidth = pCurrentPaperWidth.getValue().toDouble();
  double currentPaperHeight = pCurrentPaperHeight.getValue().toDouble();

  if(properties.contains("paper_width") || properties.contains("paper_height"))
  {
    double newPaperWidth = currentPaperWidth;
    double newPaperHeight = currentPaperHeight;

    const Property& pNewPaperWidth = properties.getProperty("paper_width");
    if(pNewPaperWidth.isNull() == false)
    {
      newPaperWidth = pNewPaperWidth.getValue().toDouble();
    }
    const Property& pNewPaperHeight = properties.getProperty("paper_height");
    if(pNewPaperHeight.isNull() == false)
    {
      newPaperHeight = pNewPaperHeight.getValue().toDouble();
    }

    Properties fullProperties = handleNewPaperSize(newPaperWidth, newPaperHeight);
    for(size_t i = 0; i < properties.getProperties().size(); ++i)
    {
      fullProperties.addProperty(properties.getProperties()[i]);
    }
    
    AbstractItemModel::setProperties(fullProperties);
    return;
  }

  AbstractItemModel::setProperties(properties);
}

te::layout::Properties te::layout::PaperModel::handleNewPaperSize(double paperWidth, double paperHeight)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pShadowPadding = m_properties.getProperty("shadow_padding");
  double shadowPadding = pShadowPadding.getValue().toDouble();

  double width = paperWidth + shadowPadding;
  double height = paperHeight + shadowPadding;

  Properties properties;
  {
    Property property(0);
    property.setName("width");
    property.setValue(width, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("height");
    property.setValue(height, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  return properties;
}

/*
void te::layout::PaperModel::config()
{
  if(!m_paperConfig)
    return;

  double pw = 0.;
  double ph = 0.;

  m_paperConfig->getPaperSize(pw, ph);
  m_box = te::gm::Envelope(0., - m_shadowPadding, pw + m_shadowPadding, ph);

  ContextItem context;
  context.setChangePos(true);
  
  te::gm::Coord2D coord(m_box.m_llx, m_box.m_lly);
  context.setPos(coord);

  notifyAll(context);
}

te::color::RGBAColor te::layout::PaperModel::getShadowColor()
{
  return m_shadowColor;
}

void te::layout::PaperModel::setShadowColor( te::color::RGBAColor color )
{
  m_shadowColor = color;
}

void te::layout::PaperModel::setPaperConfig( PaperConfig* pConfig )
{
  m_paperConfig = pConfig;
  config();
}

te::layout::PaperConfig* te::layout::PaperModel::getPaperConfig() const
{
  return m_paperConfig;
}

void te::layout::PaperModel::setShadowPadding( double padding )
{
  m_shadowPadding = padding;
}

double te::layout::PaperModel::getShadowPadding()
{
  return m_shadowPadding;
}

void te::layout::PaperModel::setBox( te::gm::Envelope box )
{
  if(!m_paperConfig)
    return;

  double pw = 0.;
  double ph = 0.;

  m_paperConfig->getPaperSize(pw, ph);
  m_box = te::gm::Envelope(box.m_llx, box.m_lly - m_shadowPadding, pw + m_shadowPadding, ph);
}

te::color::RGBAColor te::layout::PaperModel::getPaperColor()
{
  return m_paperColor;
}

void te::layout::PaperModel::setPaperColor( te::color::RGBAColor color )
{
  m_paperColor = color;
}
*/