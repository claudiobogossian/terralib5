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
  \file LegendChildModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LegendChildModel.h"
#include "../core/enum/Enums.h"
#include "../core/property/Properties.h"
#include "../core/property/SharedProperties.h"
#include "../../maptools/CanvasConfigurer.h"
#include "../../se/Symbolizer.h"
#include "../../se/Style.h"
#include "../../se/Rule.h"
#include "MapModel.h"

// STL
#include <string>
#include <sstream> 

te::layout::LegendChildModel::LegendChildModel() :
  m_rule(0)
{
  m_type = Enums::getInstance().getEnumObjectType()->getLegendChildItem();

  m_box = te::gm::Envelope(0., 0., 70., 50.);

  m_font.setPointSize(8);
  m_fontColor.setColor(0,0,0);
}

te::layout::LegendChildModel::~LegendChildModel()
{

}

void te::layout::LegendChildModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();

  if((!canvas) || (!utils))
    return;

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);
  
  drawBackground(context);

  drawLegend(canvas, utils);

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);

  context.setPixmap(pixmap);
  notifyAll(context);
}

void te::layout::LegendChildModel::drawLegend( te::map::Canvas* canvas, Utils* utils )
{ 
  if(!m_rule)
    return;

  double wtxt = 0;
  double htxt = 0;

  double x1 = m_box.getLowerLeftX() + m_borderDisplacement;
  double y1 = m_box.getUpperRightY() - m_borderDisplacement - htxt;

  // Creates a canvas configurer
  te::map::CanvasConfigurer cc(canvas);

  // Gets the set of symbolizers defined on current rule
  const std::vector<te::se::Symbolizer*>& symbolizers = m_rule->getSymbolizers();

  if(symbolizers.empty())
  {
    return;
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
    std::string* nameRule = const_cast<std::string*>(m_rule->getName());
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

    utils->textBoundingBox(wtxt, htxt, name);
    canvas->drawText(box.getLowerLeftX() + m_symbolsize + m_displacementBetweenSymbolsAndText, box.m_ury, name, 0);
  } // end for each <Symbolizer>
}

te::layout::Properties* te::layout::LegendChildModel::getProperties() const
{
  return LegendModel::getProperties();
}

void te::layout::LegendChildModel::updateProperties( te::layout::Properties* properties )
{
  LegendModel::updateProperties(properties);
}

void te::layout::LegendChildModel::visitDependent(ContextItem context)
{
  MapModel* map = dynamic_cast<MapModel*>(m_visitable);

  if(map)
  {
    m_layer = map->getLayer();

    refreshRule();

    draw(context);
  }	
}

te::se::Rule* te::layout::LegendChildModel::getRule()
{
  return m_rule;
}

void te::layout::LegendChildModel::setRule( te::se::Rule* rule )
{
  m_rule = rule;
}

void te::layout::LegendChildModel::refreshRule()
{
  if(!m_layer)
    return;

  if(!m_layer->getStyle())
    return;
  
  // Number of rules defined on feature type style
  std::size_t nRules = m_layer->getStyle()->getRules().size();

  for(std::size_t i = 0; i < nRules; ++i) // for each <Rule>
  {
    // The current rule
    te::se::Rule* rule = m_layer->getStyle()->getRule(i);
    
    if(rule == m_rule)
    {
      m_rule = rule;
    }
  }   // end for each <Rule>
}
