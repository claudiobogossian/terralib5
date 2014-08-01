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
  \file ScaleModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "DefaultTextModel.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/property/SharedProperties.h"

// STL
#include <string>
#include <sstream> // std::stringstream


te::layout::DefaultTextModel::DefaultTextModel() :
  m_text("")
{
  m_box = te::gm::Envelope(0., 0., 70., 30.);
}

te::layout::DefaultTextModel::~DefaultTextModel()
{

}

void te::layout::DefaultTextModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();
  
  if((!canvas) || (!utils))
    return;

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);

  drawText(canvas, utils, m_box);

  if(m_border)
  {
    canvas->setPolygonContourWidth(2);
    canvas->setPolygonContourColor(te::color::RGBAColor(0, 0, 0, 255));
    canvas->setPolygonFillColor(m_backgroundColor);

    utils->drawRectW(m_box);
  }

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);

  context.setPixmap(pixmap);
  notifyAll(context);
}

void te::layout::DefaultTextModel::drawText( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box )
{

}

te::layout::Properties* te::layout::DefaultTextModel::getProperties() const
{
  ItemModelObservable::getProperties();

  Property pro_text;
  pro_text.setName("Text");
  pro_text.setId("");
  pro_text.setValue(m_text, DataTypeString);
  m_properties->addProperty(pro_text);

  Property pro_font;
  pro_font.setName("Font");
  pro_font.setId("");
  pro_font.setValue(m_font, DataTypeFont);
  m_properties->addProperty(pro_font);

  return m_properties;
}

void te::layout::DefaultTextModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);
  
  Property pro_text = vectorProps->contains("Text");

  if(!pro_text.isNull())
  {
    m_text = pro_text.getValue().toString();
  }

  Property pro_font = vectorProps->contains("Font");

  if(!pro_font.isNull())
  {
    m_font = pro_font.getValue().toFont();
  }
}

void te::layout::DefaultTextModel::setText( std::string txt )
{
  m_text = txt;
}

std::string te::layout::DefaultTextModel::getText()
{
  return m_text;
}

te::layout::Font te::layout::DefaultTextModel::getFont()
{
  return m_font;
}
