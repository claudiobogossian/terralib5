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
  \file LegendModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LegendModel.h"
#include "MapModel.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/property/SharedProperties.h"
#include "../../maptools/CanvasConfigurer.h"
#include "../../se/Symbolizer.h"
#include "../../se/Style.h"
#include "../../se/Rule.h"
#include "../../geometry/Polygon.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/Envelope.h"
#include "../core/enum/Enums.h"

// STL
#include <string>
#include <sstream> 

te::layout::LegendModel::LegendModel() :
  m_mapName(""),
  m_layer(0),
  m_borderDisplacement(2),
  m_displacementBetweenSymbols(2),
  m_displacementBetweenTitleAndSymbols(5),
  m_displacementBetweenSymbolsAndText(3),
  m_symbolsize(10)
{
  m_type = Enums::getInstance().getEnumObjectType()->getLegendItem();

  m_box = te::gm::Envelope(0., 0., 70., 50.);

  m_font.setPointSize(8);
  m_fontColor.setColor(0,0,0);
}

te::layout::LegendModel::~LegendModel()
{

}

void te::layout::LegendModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();

  if((!canvas) || (!utils))
    return;

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);
  
  drawBackground(context);

  childrenFreeMemory();

  drawLegend(canvas, utils);

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);

  context.setPixmap(pixmap);
  notifyAll(context);
}

void te::layout::LegendModel::drawLegend( te::map::Canvas* canvas, Utils* utils )
{
  if(!m_layer)
    return;

  if(!m_layer->getStyle())
    return;
  
  // Number of rules defined on feature type style
  std::size_t nRules = m_layer->getStyle()->getRules().size();

  //Header
  std::string layerName = m_layer->getTitle();
  
  double wtxt = 0;
  double htxt = 0;

  canvas->setTextPointSize(m_font.getPointSize());
  canvas->setTextUnderline(m_font.isUnderline());
  canvas->setTextStrikeOut(m_font.isStrikeout());
  canvas->setTextColor(m_fontColor);

  utils->textBoundingBox(wtxt, htxt, layerName);

  double x1 = m_box.getLowerLeftX() + m_borderDisplacement;
  double y1 = m_box.getUpperRightY() - m_borderDisplacement - htxt;
  canvas->drawText(x1, y1, layerName, 0);

  //Line between title and symbols
  /*canvas->setLineColor(te::color::RGBAColor(0,0,0,255));
  te::gm::Envelope newBox(m_box.getLowerLeftX(), y1, m_box.getUpperRightX(), y1);
  te::gm::LinearRing* line = utils->createSimpleLine(newBox);
  utils->drawLineW(line);
  if(line) delete line;*/
  
  // Creates a canvas configurer
  te::map::CanvasConfigurer cc(canvas);

  for(std::size_t i = 0; i < nRules; ++i) // for each <Rule>
  {
    // The current rule
    te::se::Rule* rule = m_layer->getStyle()->getRule(i);
    assert(rule);
        
    // Gets the set of symbolizers defined on current rule
    const std::vector<te::se::Symbolizer*>& symbolizers = rule->getSymbolizers();

    if(symbolizers.empty())
    {
      continue;
    }

    std::size_t nSymbolizers = symbolizers.size();

    for(std::size_t j = 0; j < nSymbolizers; ++j) // for each <Symbolizer>
    {
      // The current symbolizer
      te::se::Symbolizer* symb = symbolizers[j];
      
      // Let's config the canvas based on the current symbolizer
      cc.config(symb);

      //Test
      te::gm::Envelope box(x1, y1 - m_displacementBetweenTitleAndSymbols - (m_displacementBetweenSymbols * j), 
                    x1 + m_symbolsize, y1 - m_displacementBetweenTitleAndSymbols - (m_displacementBetweenSymbols * j) - m_symbolsize);
      utils->drawRectW(box);
      
      canvas->setTextPointSize(m_font.getPointSize());
      canvas->setTextUnderline(m_font.isUnderline());
      canvas->setTextStrikeOut(m_font.isStrikeout());
      canvas->setTextColor(m_fontColor);
      std::string* nameRule = const_cast<std::string*>(rule->getName());
      std::string n = symb->getName();
      std::string name;
      if(nameRule)
      {
        name = nameRule->c_str();
      }
      else
      {
        std::ostringstream convert;
        convert << j;
        name = "Legend_" + convert.str();
      }

      te::gm::Point* coord = new te::gm::Point(x1, y1 - m_displacementBetweenTitleAndSymbols - (m_displacementBetweenSymbols * j));
      m_coordChildren[coord] = name;

      utils->textBoundingBox(wtxt, htxt, name);
      canvas->drawText(box.getLowerLeftX() + m_symbolsize + m_displacementBetweenSymbolsAndText, box.m_ury, name, 0);
    } // end for each <Symbolizer>

  }   // end for each <Rule>

}

te::layout::Properties* te::layout::LegendModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_mapName;
  pro_mapName.setName(m_sharedProps->getMapName());
  pro_mapName.setId("");
  pro_mapName.setValue(m_mapName, dataType->getDataTypeStringList());
  Variant v;
  v.setValue(m_mapName, dataType->getDataTypeString());
  pro_mapName.addOption(v);
  
  m_properties->addProperty(pro_mapName);

  Property pro_font;
  pro_font.setName("Font");
  pro_font.setId("");
  pro_font.setValue(m_font, dataType->getDataTypeFont());
  pro_font.setMenu(true);
  m_properties->addProperty(pro_font);

  Property pro_fontcolor;
  pro_fontcolor.setName("font_color");
  pro_fontcolor.setId("unknown");
  pro_fontcolor.setValue(m_fontColor, dataType->getDataTypeColor());
  pro_fontcolor.setMenu(true);
  m_properties->addProperty(pro_fontcolor);

  return m_properties;
}

void te::layout::LegendModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_mapName = vectorProps->contains(m_sharedProps->getMapName());

  if(!pro_mapName.isNull())
  {
    m_mapName = pro_mapName.getOptionByCurrentChoice().toString();
  }

  Property pro_font = vectorProps->contains("Font");

  if(!pro_font.isNull())
  {
    m_font = pro_font.getValue().toFont();
  }

  Property pro_fontColor = vectorProps->contains("font_color");

  if(!pro_fontColor.isNull())
  {
    m_fontColor = pro_fontColor.getValue().toColor();
  }
}

void te::layout::LegendModel::visitDependent(ContextItem context)
{
  MapModel* map = dynamic_cast<MapModel*>(m_visitable);

  if(map)
  {
    m_layer = map->getLayer();

    draw(context);
  }	
}

void te::layout::LegendModel::setBorderDisplacement( double value )
{
  m_borderDisplacement = value;
}

double te::layout::LegendModel::getBorderDisplacement()
{
  return m_borderDisplacement;
}

void te::layout::LegendModel::setDisplacementBetweenSymbols( double value )
{
  m_displacementBetweenSymbols = value;
}

double te::layout::LegendModel::getDisplacementBetweenSymbols()
{
  return m_displacementBetweenSymbols;
}

void te::layout::LegendModel::setDisplacementBetweenTitleAndSymbols( double value )
{
  m_displacementBetweenTitleAndSymbols = value;
}

double te::layout::LegendModel::getDisplacementBetweenTitleAndSymbols()
{
  return m_displacementBetweenTitleAndSymbols;
}

void te::layout::LegendModel::setDisplacementBetweenSymbolAndText( double value )
{
  m_displacementBetweenSymbolsAndText = value;
}

double te::layout::LegendModel::getDisplacementBetweenSymbolAndText()
{
  return m_displacementBetweenSymbolsAndText;
}

std::map<te::gm::Point*, std::string> te::layout::LegendModel::getCoordChildren()
{
  return m_coordChildren;
}

void te::layout::LegendModel::childrenFreeMemory()
{
  std::map<te::gm::Point*, std::string>::iterator it;
  for(it = m_coordChildren.begin() ; it != m_coordChildren.end() ; ++it)
  {
    te::gm::Point* p = it->first;
    if(p)
    {
      delete p;
      p = 0;
    }
  }
  m_coordChildren.clear();
}
