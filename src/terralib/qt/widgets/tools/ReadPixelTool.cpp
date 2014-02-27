/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ReadPixel.cpp

  \brief An example of MapDisplay Tool. The only purpose of this tool is to show how you can implement a new tool. Do not consider it as a final application.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../maptools/RasterTransform.h"
#include "../../../maptools/RasterTransformConfigurer.h"
#include "../../../qt/widgets/canvas/Canvas.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../raster/Raster.h"
#include "../../../raster/RasterSummary.h"
#include "../../../raster/RasterSummaryManager.h"
#include "../../../raster/Utils.h"
#include "../../../se/Utils.h"
#include "../../../se.h"
#include "ReadPixelTool.h"

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QToolTip>

// STL
#include <cassert>

te::qt::widgets::ReadPixelTool::ReadPixelTool(te::qt::widgets::MapDisplay* display, te::map::AbstractLayerPtr layer, QObject* parent) 
  : te::qt::widgets::AbstractTool(display, parent)
{
  assert(layer);
  m_layer = layer;
}

te::qt::widgets::ReadPixelTool::~ReadPixelTool()
{
}

bool te::qt::widgets::ReadPixelTool::mouseReleaseEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  // Converts clicked point to world coordinates
  QPointF qpoint = m_display->transform(e->posF());

  te::map::DataSetLayer* dsL = dynamic_cast<te::map::DataSetLayer*>(m_layer.get());

  if(dsL == 0)
    return false;

  // * Under revision *
  te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(dsL->getStyle());
  assert(cs);

// get the raster symbolizer
  std::size_t nRules = cs->getRules().size();
  assert(nRules >= 1);

// for while, consider one rule
  const te::se::Rule* rule = cs->getRule(0);

  const std::vector<te::se::Symbolizer*>& symbolizers = rule->getSymbolizers();
  assert(!symbolizers.empty());

// for while, consider one raster symbolizer
  te::se::RasterSymbolizer* rs = dynamic_cast<te::se::RasterSymbolizer*>(symbolizers[0]);
  assert(rs);

 //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    te::rst::Raster* inputRst = ds->getRaster(rpos).get();

    if(inputRst)
    {

      //create the raster transform
      te::map::RasterTransform rt(inputRst, 0);

      double rMin, rMax;

      const te::rst::RasterSummary* rsMin = te::rst::RasterSummaryManager::getInstance().get(inputRst, te::rst::SUMMARY_MIN);
      const te::rst::RasterSummary* rsMax = te::rst::RasterSummaryManager::getInstance().get(inputRst, te::rst::SUMMARY_MAX);
      const std::complex<double>* cmin = rsMin->at(0).m_minVal;
      const std::complex<double>* cmax = rsMax->at(0).m_maxVal;
      rMin = cmin->real();
      rMax = cmax->real();

      rt.setLinearTransfParameters(0, 255, rMin, rMax);

      //configure the raster transformation using the raster symbolizer
      te::map::RasterTransformConfigurer rtc(rs, &rt);

      rtc.configure();

      te::gm::Coord2D coord = inputRst->getGrid()->geoToGrid(qpoint.x(), qpoint.y());

      int x = te::rst::Round(coord.x);
      int y = te::rst::Round(coord.y);

      if((x >=0 && x < (int)inputRst->getNumberOfColumns()) && (y >=0 && y < (int)inputRst->getNumberOfRows()))
      {
        QString information("<h2>Read Pixel</h2><ul>");

        information += "<h3>Location</h3>";

        QString column, line, xCoord, yCoord;
        column.setNum(x);
        line.setNum(y);
        xCoord.setNum(qpoint.x());
        yCoord.setNum(qpoint.y());

        information += "<li><b>" + QString("Line ") + ":</b> " + line + "</li>";
        information += "<li><b>" + QString("Column ") + ":</b> " + column + "</li>";
        information += "<li><b>" + QString("Coord X ") + ":</b> " + xCoord + "</li>";
        information += "<li><b>" + QString("Coord Y ") + ":</b> " + yCoord + "</li>";

        information += "<h3>Original Values</h3>";

        for(size_t t = 0; t < inputRst->getNumberOfBands(); ++t )
        {
          double val;
          inputRst->getValue(x, y, val, t);

          QString band, value;
          band.setNum(t);
          value.setNum(val);

          information += "<li><b>" + QString("Value for band ") + band + ":</b> " + value + "</li>";

        }
    
        te::color::RGBAColor color = rt.apply(x, y);

        information += "<h3>Transformed Values</h3>";

        QString red, green, blue, alpha;
        red.setNum(color.getRed());
        green.setNum(color.getGreen());
        blue.setNum(color.getBlue());
        alpha.setNum(color.getAlpha());

        QString rBand, gBand, bBand;
        rBand.setNum(rt.getRGBMap()[te::map::RasterTransform::RED_CHANNEL]);
        gBand.setNum(rt.getRGBMap()[te::map::RasterTransform::GREEN_CHANNEL]);
        bBand.setNum(rt.getRGBMap()[te::map::RasterTransform::BLUE_CHANNEL]);

        information += "<li><b>" + QString("Red Channel: </b>") + rBand + "<b> - Value:</b> " + red + "</li>";
        information += "<li><b>" + QString("Green Channel: </b>") + gBand + "<b> - Value:</b> " + green + "</li>";
        information += "<li><b>" + QString("Blue Channel: </b>") + bBand + "<b> - Value:</b> " + blue + "</li>";
        information += "<li><b>" + QString("Alpha Channel value") + ":</b> " + alpha + "</li>";


        information += "<h3>Symbolizer</h3>";

        QString type;

        if(rs->getChannelSelection()->getColorCompositionType() == te::se::RGB_COMPOSITION)
        {
          type = "RGB Composition";
        }
        else if(rs->getChannelSelection()->getColorCompositionType() == te::se::GRAY_COMPOSITION)
        {
          type = "Gray Composition";
        }
        else
        {
          type = "Unknown Composition";
        }

        information += "<li><b>" + QString("Color Composition ") + ":</b> " + type + "</li>";

        if(rs->getOpacity())
        {
          QString opacity = te::se::GetString(rs->getOpacity()).c_str();
          information += "<li><b>" + QString("Opacity Value ") + ":</b> " + opacity + "</li>";
        }

        if(rs->getGain())
        {
          QString gain = te::se::GetString(rs->getGain()).c_str();
          information += "<li><b>" + QString("Gain Value ") + ":</b> " + gain + "</li>";
        }

        if(rs->getOffset())
        {
          QString offset = te::se::GetString(rs->getOffset()).c_str();
          information += "<li><b>" + QString("Offset Value ") + ":</b> " + offset + "</li>";
        }

        if(rs->getChannelSelection()->getRedChannel() && 
           rs->getChannelSelection()->getRedChannel()->getContrastEnhancement())
        {
          QString c;
          c.setNum(rs->getChannelSelection()->getRedChannel()->getContrastEnhancement()->getGammaValue());

          information += "<li><b>" + QString("Red Contrast") + ":</b> " + c + "</li>";
        }

        if(rs->getChannelSelection()->getGreenChannel() && 
           rs->getChannelSelection()->getGreenChannel()->getContrastEnhancement())
        {
          QString c;
          c.setNum(rs->getChannelSelection()->getGreenChannel()->getContrastEnhancement()->getGammaValue());

          information += "<li><b>" + QString("Green Contraste") + ":</b> " + c + "</li>";
        }

        if(rs->getChannelSelection()->getBlueChannel() && 
           rs->getChannelSelection()->getBlueChannel()->getContrastEnhancement())
        {
          QString c;
          c.setNum(rs->getChannelSelection()->getBlueChannel()->getContrastEnhancement()->getGammaValue());

          information += "<li><b>" + QString("Blue Contraste") + ":</b> " + c + "</li>";
        }

        if(rs->getChannelSelection()->getGrayChannel() && 
           rs->getChannelSelection()->getGrayChannel()->getContrastEnhancement())
        {
          QString c;
          c.setNum(rs->getChannelSelection()->getGrayChannel()->getContrastEnhancement()->getGammaValue());

          information += "<li><b>" + QString("Gray Contraste") + ":</b> " + c + "</li>";
        }

        information += "</ul>";

        // Show attributes
        QToolTip::showText(QCursor::pos(), information, m_display, m_display->rect());
      }
    }
  }

  return true;
}
