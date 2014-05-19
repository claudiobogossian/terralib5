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
#include "../../../../se/ChannelSelection.h"
#include "../../../../se/ContrastEnhancement.h"
#include "../../../../se/FeatureTypeStyle.h"
#include "../../../../se/Fill.h"
#include "../../../../se/LineSymbolizer.h"
#include "../../../../se/ParameterValue.h"
#include "../../../../se/PolygonSymbolizer.h"
#include "../../../../se/PointSymbolizer.h"
#include "../../../../se/RasterSymbolizer.h"
#include "../../../../se/Rule.h"
#include "../../../../se/SelectedChannel.h"
#include "../../../../se/Style.h"
#include "../../../../se/Stroke.h"
#include "../../../../se/Utils.h"
#include "Utils.h"

// TerraLib 4
#include <kernel/TeDataTypes.h>
#include <kernel/TeLegendEntry.h>
#include <kernel/TeRaster.h>
#include <kernel/TeRasterTransform.h>
#include <kernel/TeTheme.h>
#include <kernel/TeVisual.h>

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

te::se::Style* te::qt::plugins::terralib4::Convert2TerraLib5(int geometryType, TeTheme* theme, bool isRaster)
{
  te::se::Symbolizer* symb = 0;

  if(isRaster)
  {
    TeRasterVisual* vis = 0;
    vis = theme->rasterVisual();
    if(!vis)
    {
      TeRaster* raster = theme->layer()->raster();
      theme->createRasterVisual(raster);
      vis = theme->rasterVisual();
    }

    symb = GetRasterSymbolizer(vis);
  }
  else
  {

    TeLegendEntry legend = theme->defaultLegend();

    TeGeomRepVisualMap visualMap = legend.getVisualMap();

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

te::se::RasterSymbolizer* te::qt::plugins::terralib4::GetRasterSymbolizer(TeRasterTransform* visual)
{
  te::se::RasterSymbolizer* symb = new te::se::RasterSymbolizer;

  double opacity = ((visual->getTransparency()*100.0)/255)/100.0;
  double offset = visual->getOffset();
  double gain = visual->getGain();

  symb->setOpacity(new te::se::ParameterValue(boost::lexical_cast<std::string>(opacity)));
  symb->setOffset(new te::se::ParameterValue(boost::lexical_cast<std::string>(offset)));
  symb->setGain(new te::se::ParameterValue(boost::lexical_cast<std::string>(gain)));

  /*
  te::se::SelectedChannel* scRed = 0;
  te::se::SelectedChannel* scGreen = 0;
  te::se::SelectedChannel* scBlue = 0;
  te::se::SelectedChannel* scMono = 0;

  std::map<TeRasterTransform::TeRGBChannels,short> rgbMap = visual->getRGBMap();

  te::se::ChannelSelection* cs = new te::se::ChannelSelection;

  if(rgbMap.find(TeRasterTransform::TeREDCHANNEL) != rgbMap.end())
  {
    scRed = new te::se::SelectedChannel;
    scRed->setSourceChannelName(boost::lexical_cast<std::string>(rgbMap[TeRasterTransform::TeREDCHANNEL]));
    te::se::ContrastEnhancement* contrastRed = new te::se::ContrastEnhancement();
    scRed->setContrastEnhancement(contrastRed);
    contrastRed->setGammaValue(visual->getContrastR());

    cs->setRedChannel(scRed);
  }

  if(rgbMap.find(TeRasterTransform::TeGREENCHANNEL) != rgbMap.end())
  {
    scGreen = new te::se::SelectedChannel;
    scGreen->setSourceChannelName(boost::lexical_cast<std::string>(rgbMap[TeRasterTransform::TeGREENCHANNEL]));
    te::se::ContrastEnhancement* contrastGreen = new te::se::ContrastEnhancement();
    scGreen->setContrastEnhancement(contrastGreen);
    contrastGreen->setGammaValue(visual->getContrastG());

    cs->setRedChannel(scGreen);
  }

  if(rgbMap.find(TeRasterTransform::TeBLUECHANNEL) != rgbMap.end())
  {
    scBlue = new te::se::SelectedChannel;
    scBlue->setSourceChannelName(boost::lexical_cast<std::string>(rgbMap[TeRasterTransform::TeBLUECHANNEL]));
    te::se::ContrastEnhancement* contrastBlue = new te::se::ContrastEnhancement();
    scBlue->setContrastEnhancement(contrastBlue);
    contrastBlue->setGammaValue(visual->getContrastB());

    cs->setRedChannel(scBlue);
  }
  */

  return symb;
}