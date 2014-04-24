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
#include "../../../../common/StringUtils.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../maptools/Enums.h"
#include "../../../../maptools/Grouping.h"
#include "../../../../maptools/GroupingItem.h"
#include "../../../../se/Categorize.h"
#include "../../../../se/ChannelSelection.h"
#include "../../../../se/ColorMap.h"
#include "../../../../se/ContrastEnhancement.h"
#include "../../../../se/CoverageStyle.h"
//#include "../../../../se/FeatureTypeStyle.h"
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
#include "../../../../terralib4/Utils.h"
#include "Utils.h"

// TerraLib 4
#include <terralib/kernel/TeAbstractTheme.h>
#include <terralib/kernel/TeDataTypes.h>
#include <terralib/kernel/TeLegendEntry.h>
#include <terralib/kernel/TeRaster.h>
#include <terralib/kernel/TeRasterTransform.h>
#include <terralib/kernel/TeTheme.h>
#include <terralib/kernel/TeVisual.h>

// Boost
#include <boost/lexical_cast.hpp>

// Qt
#include <QtGui/QColor>

te::color::RGBAColor te::qt::plugins::terralib4::Convert2TerraLib5(TeColor color)
{
  te::color::RGBAColor tl5Color(color.red_, color.green_, color.blue_, 0);

  return tl5Color;
}

te::map::GroupingType GetGroupingType(TeGroupingMode mode)
{

  switch(mode)
  {
    case TeEqualSteps:
      return te::map::EQUAL_STEPS;

    case TeQuantil:
      return te::map::QUANTIL;

    case TeStdDeviation:
      return te::map::STD_DEVIATION;

    case TeUniqueValue:
      return te::map::UNIQUE_VALUE;

    default:
      return te::map::EQUAL_STEPS;
  }
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

  te::se::CoverageStyle* style = new te::se::CoverageStyle;
  style->push_back(rule);

  return style;
}

te::se::Symbolizer* te::qt::plugins::terralib4::GetSymbolizer(int geometryType, TeVisual* visual)
{
  switch(geometryType)
  {
    case te::gm::PolygonType:
      return GetPolygonSymbolizer(visual);

    case te::gm::LineStringType:
      return GetLineSymbolizer(visual);

    case te::gm::PointType:
      return GetPointSymbolizer(visual);

    default:
      return 0;
  }
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

te::map::Grouping* te::qt::plugins::terralib4::GetGrouping(TeTheme* theme)
{
  TeLegendEntryVector leg = theme->legend();
  TeGrouping group = theme->grouping();

  TeAttributeRep attr = group.groupAttribute_;
  std::string propertyName = attr.name_;
  std::vector<std::string> tokens;
  te::common::Tokenize(propertyName, tokens, ".");
  propertyName = tokens[1];

  TeGroupingMode mode = group.groupMode_;
  int tl5Type = ::terralib4::Convert2T5(attr.type_);
  std::size_t precision = static_cast<std::size_t>(group.groupPrecision_);
  int slices = group.groupNumSlices_;
  double stdDeviation = group.groupStdDev_;

  te::map::Grouping* grouping = new te::map::Grouping(propertyName, GetGroupingType(mode), precision);
  grouping->setPropertyType(tl5Type);
  grouping->setStdDeviation(stdDeviation);

  std::vector<te::map::GroupingItem*> items;

  for(std::size_t i = 0; i < slices; ++i)
  {
    te::map::GroupingItem* item = new te::map::GroupingItem;

    TeLegendEntry le = leg[i];
    
    std::string title = le.label();

    item->setTitle(title);

    std::string fromValue = le.from();
    std::string toValue = le.to();

    if(mode == TeUniqueValue)
    {
      item->setValue(fromValue);
    }
    else
    {
      item->setLowerLimit(fromValue);
      item->setUpperLimit(toValue);
    }

    std::vector<te::se::Symbolizer*> symbs;

    TeGeomRepVisualMap map = le.getVisualMap();

    TeGeomRep geomRep = map.begin()->first;
    TeVisual* visual = map.begin()->second;

    symbs.push_back(GetSymbolizer(::terralib4::Convert2T5GeomType(geomRep), visual));

    item->setSymbolizers(symbs);

    items.push_back(item);
  }

  grouping->setGroupingItems(items);

  return grouping;
}

te::se::ColorMap* te::qt::plugins::terralib4::GetRasterGrouping(TeTheme* theme)
{
  TeLegendEntryVector leg = theme->legend();
  TeGrouping group = theme->grouping();

  int slices = group.groupNumSlices_;

  te::se::Categorize* c = new te::se::Categorize();

  c->setFallbackValue("#000000");
  c->setLookupValue(new te::se::ParameterValue("Rasterdata"));

  QColor cWhite(Qt::white);
  std::string colorWhiteStr = cWhite.name().toLatin1().data();

  //added dummy color for values < than min values...
  c->addValue(new te::se::ParameterValue(colorWhiteStr));

  for(std::size_t i = 0; i < slices; ++i)
  {
    TeLegendEntry le = leg[i];

    std::string title = le.label();

    TeGeomRepVisualMap map = le.getVisualMap();

    TeGeomRep geomRep = map.begin()->first;
    TeVisual* visual = map.begin()->second;

    QColor color(visual->color().red_, visual->color().green_, visual->color().blue_, 0);

    std::string rangeStr = le.from();
    std::string colorStr = color.name().toStdString();

    c->addThreshold(new te::se::ParameterValue(rangeStr));
    c->addValue(new te::se::ParameterValue(colorStr));

    if(i == slices - 1)
    {
      rangeStr = le.to();
      c->addThreshold(new te::se::ParameterValue(rangeStr));
    }
  }

  //added dummy color for values > than max values...
  c->addValue(new te::se::ParameterValue(colorWhiteStr));

  c->setThresholdsBelongTo(te::se::Categorize::SUCCEEDING);

  te::se::ColorMap* cm = new te::se::ColorMap;
  cm->setCategorize(c);

  return cm;
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

  TeRasterTransform::TeRasterTransfFunctions func = visual->getTransfFunction();

  te::se::ChannelSelection* cs = new te::se::ChannelSelection;

  te::se::SelectedChannel* scRed = 0;
  te::se::SelectedChannel* scGreen = 0;
  te::se::SelectedChannel* scBlue = 0;
  te::se::SelectedChannel* scMono = 0;

  if (func == TeRasterTransform::TeBand2Band)
  {
    scRed = new te::se::SelectedChannel;
    scRed->setSourceChannelName("0");

    scGreen = new te::se::SelectedChannel;
    scGreen->setSourceChannelName("1");

    scBlue = new te::se::SelectedChannel;
    scBlue->setSourceChannelName("2");

    cs->setColorCompositionType(te::se::RGB_COMPOSITION);

  }
  else if (func == TeRasterTransform::TeExtractBand)
  {

    short db = visual->getDestBand();

    if (db == TeRasterTransform::TeREDCHANNEL)
    {

      scRed = new te::se::SelectedChannel;
      scRed->setSourceChannelName(boost::lexical_cast<std::string>(visual->getSrcBand()));

      cs->setColorCompositionType(te::se::RED_COMPOSITION);

    }
    else if (db == TeRasterTransform::TeGREENCHANNEL)
    {

      scGreen = new te::se::SelectedChannel;
      scGreen->setSourceChannelName(boost::lexical_cast<std::string>(visual->getSrcBand()));

      cs->setColorCompositionType(te::se::GREEN_COMPOSITION);

    }
    else if (db == TeRasterTransform::TeBLUECHANNEL)
    {

      scBlue = new te::se::SelectedChannel;
      scBlue->setSourceChannelName(boost::lexical_cast<std::string>(visual->getSrcBand()));

      cs->setColorCompositionType(te::se::BLUE_COMPOSITION);

    }
  }
  else if (func == TeRasterTransform::TeMono2Three)
  {

    scMono = new te::se::SelectedChannel;
    scMono->setSourceChannelName(boost::lexical_cast<std::string>(visual->getSrcBand()));

    cs->setColorCompositionType(te::se::GRAY_COMPOSITION);

  }
  else if (func == TeRasterTransform::TeExtractRGB)
  {
    std::map<TeRasterTransform::TeRGBChannels,short>& RGBmap = visual->getRGBMap();

    scRed = new te::se::SelectedChannel;
    scRed->setSourceChannelName(boost::lexical_cast<std::string>(RGBmap[TeRasterTransform::TeREDCHANNEL]));

    scGreen = new te::se::SelectedChannel;
    scGreen->setSourceChannelName(boost::lexical_cast<std::string>(RGBmap[TeRasterTransform::TeGREENCHANNEL]));

    scBlue = new te::se::SelectedChannel;
    scBlue->setSourceChannelName(boost::lexical_cast<std::string>(RGBmap[TeRasterTransform::TeBLUECHANNEL]));

    cs->setColorCompositionType(te::se::RGB_COMPOSITION);

  }
  else if (func == TeRasterTransform::TeThreeBand2RGB)
  {
    scRed = new te::se::SelectedChannel;
    scRed->setSourceChannelName("0");

    scGreen = new te::se::SelectedChannel;
    scGreen->setSourceChannelName("1");

    scBlue = new te::se::SelectedChannel;
    scBlue->setSourceChannelName("2");

    cs->setColorCompositionType(te::se::RGB_COMPOSITION);
  }

  else if (func == TeRasterTransform::TeExtractBands)
  {

    std::map<TeRasterTransform::TeRGBChannels,short>& RGBmap = visual->getRGBMap();

    if (RGBmap[TeRasterTransform::TeREDCHANNEL] != -1)
      scRed->setSourceChannelName(boost::lexical_cast<std::string>(RGBmap[TeRasterTransform::TeREDCHANNEL]));
    if (RGBmap[TeRasterTransform::TeGREENCHANNEL] != -1)
      scGreen->setSourceChannelName(boost::lexical_cast<std::string>(RGBmap[TeRasterTransform::TeGREENCHANNEL]));
    if (RGBmap[TeRasterTransform::TeBLUECHANNEL] != -1)
      scBlue->setSourceChannelName(boost::lexical_cast<std::string>(RGBmap[TeRasterTransform::TeBLUECHANNEL]));

    cs->setColorCompositionType(te::se::RGB_COMPOSITION);
  }

  if(scRed)
    cs->setRedChannel(scRed);
  if(scGreen)
    cs->setGreenChannel(scGreen);
  if(scBlue)
    cs->setBlueChannel(scBlue);
  if(scMono)
    cs->setGrayChannel(scMono);

  symb->setChannelSelection(cs);

  return symb;
}