/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/plugins/terralib5/Utils.h

  \brief
*/

// TerraLib
#include "../../../../color/RGBAColor.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../se/FeatureTypeStyle.h"
#include "../../../../se/Fill.h"
#include "../../../../se/LineSymbolizer.h"
#include "../../../../se/PolygonSymbolizer.h"
#include "../../../../se/PointSymbolizer.h"
#include "../../../../se/Rule.h"
#include "../../../../se/Style.h"
#include "../../../../se/Stroke.h"
#include "../../../../se/Utils.h"
#include "Utils.h"

// TerraLib 4
#include <terralib/kernel/TeDataTypes.h>
#include <terralib/kernel/TeLegendEntry.h>
#include <terralib/kernel/TeTheme.h>
#include <terralib/kernel/TeVisual.h>

// Boost
#include <boost/lexical_cast.hpp>

te::color::RGBAColor te::qt::plugins::terralib4::Convert2TerraLib5(TeColor color)
{
  te::color::RGBAColor tl5Color(color.red_, color.green_, color.blue_, 0);

  return tl5Color;
}

std::string te::qt::plugins::terralib4::GetLineStyle(int type)
{
  switch(type)
  {
    case TeLnTypeDashed:
      return"4 2";
    case TeLnTypeDotted:
      return "1 2";
    case TeLnTypeDashDot:
      return "4 2 1 2";
    case TeLnTypeDashDotDot:
      return "4 2 1 2 1 2";
    case TeLnTypeNone:
      return "NONE";
    case TeLnTypeContinuous:
    default:
      return "";
  }
}

std::string te::qt::plugins::terralib4::GetPointStyle(int type)
{
  switch(type)
  {
    case TePtTypePlus:
      return "cross";
    case TePtTypeStar:
      return "star";
    case TePtTypeCircle:
    case TePtTypeHollowCircle:
      return "circle";
    case TePtTypeX:
      return "x";
    case TePtTypeBox:
    case TePtTypeHollowBox:
      return "square";
    case TePtTypeDiamond:
    case TePtTypeHollowDiamond:
      return "diamond";
    default:
      return "";
  }
}

bool te::qt::plugins::terralib4::IsHollow(int type)
{
  switch(type)
  {
    case TePtTypeHollowCircle:
      return true;
    case TePtTypeHollowBox:
      return true;
    case TePtTypeHollowDiamond:
      return true;
    default:
      return false;
  }
}

te::se::Style* te::qt::plugins::terralib4::Convert2TerraLib5(int geometryType, TeTheme* theme)
{
  TeLegendEntry legend = theme->defaultLegend();

  TeGeomRepVisualMap visualMap = legend.getVisualMap();

  te::se::Symbolizer* symb = 0;

  TeVisual* visual = 0;

  switch(geometryType)
  {
    case te::gm::PolygonType:
    {
      visual = visualMap[TePOLYGONS];
      symb = GetPolygonSymbolizer(visual);
      break;
    }
    case te::gm::LineStringType:
    {
      visual = visualMap[TeLINES];
      symb = GetLineSymbolizer(visual);
      break;
    }
    case te::gm::PointType:
    {
      visual = visualMap[TePOINTS];
      symb = GetPointSymbolizer(visual);
      break;
    }
    default:
    {
      break;
    }
  }

  te::se::Rule* rule = new te::se::Rule;

  if(symb != 0)
    rule->push_back(symb);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::PolygonSymbolizer* te::qt::plugins::terralib4::GetPolygonSymbolizer(TeVisual* visual)
{
  TeColor tl4Color = visual->color();

  // Stroke
  std::string color = Convert2TerraLib5(visual->contourColor()).getColor();
  std::string width = boost::lexical_cast<std::string>(visual->contourWidth());

  te::se::Stroke* stroke = te::se::CreateStroke(color, width, "1.0");

  std::string style = GetLineStyle(visual->style());

  if(!style.empty())
  {
    if(style == "NOME")
      stroke = 0;
    else
      stroke->setDashArray(style);
  }

  // Fill
  color = Convert2TerraLib5(visual->color()).getColor();
  double numericOpacity = (100-visual->transparency())/100.0;
  std::string opacity = boost::lexical_cast<std::string>(numericOpacity);

  te::se::Fill* fill = te::se::CreateFill(color, opacity);

  return te::se::CreatePolygonSymbolizer(stroke, fill);
}

te::se::LineSymbolizer* te::qt::plugins::terralib4::GetLineSymbolizer(TeVisual* visual)
{
  std::string color = Convert2TerraLib5(visual->color()).getColor();
  std::string width = boost::lexical_cast<std::string>(visual->width());

  te::se::Stroke* stroke = te::se::CreateStroke(color, width);

  std::string style = GetLineStyle(visual->style());

  if(!style.empty())
  {
    if(style == "NOME")
      stroke = 0;
    else
      stroke->setDashArray(style);
  }

  return te::se::CreateLineSymbolizer(stroke);
}

te::se::PointSymbolizer* te::qt::plugins::terralib4::GetPointSymbolizer(TeVisual* visual)
{
  std::string color = Convert2TerraLib5(visual->color()).getColor();

  te::se::Fill* fill = te::se::CreateFill(color, "1.0");

  std::string style = GetPointStyle(visual->style());

  if(IsHollow(visual->style()))
    fill = 0;

  te::se::Mark* mark = te::se::CreateMark(style, 0, fill);

  std::string size = boost::lexical_cast<std::string>(visual->size());

  te::se::Graphic* gr = te::se::CreateGraphic(mark, size, "0", "1.0");

  return te::se::CreatePointSymbolizer(gr);
}