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
  \file RasterTransformConfigurer.h

  \brief A Raster Transform configurer generates a Raster Transform given a RasterSymbolzier.
*/

// TerraLib
#include "RasterTransformConfigurer.h"
#include "Utils.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "../se/RasterSymbolizer.h"
#include "../se/ChannelSelection.h"
#include "../se/ContrastEnhancement.h"
#include "../se/SelectedChannel.h"
#include "../se/Utils.h"
#include "../se.h"

#include <limits>


te::map::RasterTransformConfigurer::RasterTransformConfigurer(te::se::RasterSymbolizer* rs, te::map::RasterTransform* rt) :
  m_rstSymbolizer(rs),
  m_rstTransform(rt)
{
}

te::map::RasterTransformConfigurer::~RasterTransformConfigurer()
{
}

void te::map::RasterTransformConfigurer::configure()
{
  assert(m_rstSymbolizer);

  getGeneralProperties();

  if(m_rstSymbolizer->getChannelSelection())
  {
    getChannelSelection();
  }

  if(m_rstSymbolizer->getColorMap())
  {
    getColorMapInformation();
  }
}

void te::map::RasterTransformConfigurer::getGeneralProperties()
{
  //get transparency
  if(m_rstSymbolizer->getOpacity())
  {
    double transp = (double)TE_OPAQUE * te::se::GetDouble(m_rstSymbolizer->getOpacity());

    m_rstTransform->setTransparency(transp);
  }

  //get gain
  if(m_rstSymbolizer->getGain())
  {
    double gain = te::se::GetDouble(m_rstSymbolizer->getGain());

    double curGain = m_rstTransform->getGain();

    curGain += gain;

    m_rstTransform->setGain(curGain);
  }

  //get offset
  if(m_rstSymbolizer->getOffset())
  {
    double offset = te::se::GetDouble(m_rstSymbolizer->getOffset());

    double curOffset = m_rstTransform->getOffset();

    curOffset += offset;

    m_rstTransform->setOffset(curOffset);
  }
}

void te::map::RasterTransformConfigurer::getChannelSelection()
{
  te::se::ChannelSelection* cs = m_rstSymbolizer->getChannelSelection();

  te::se::ColorCompositionType type = cs->getColorCompositionType();

  if(type == te::se::GRAY_COMPOSITION)
  {
    if((m_rstTransform->getInputRaster() && m_rstTransform->getOutputRaster()) &&
      (m_rstTransform->getInputRaster()->getNumberOfBands() == m_rstTransform->getOutputRaster()->getNumberOfBands()))
      m_rstTransform->setTransfFunction(te::map::RasterTransform::BAND2BAND_TRANSF);
    else
      m_rstTransform->setTransfFunction(te::map::RasterTransform::MONO2THREE_TRANSF);

    getGrayChannelProperties(cs->getGrayChannel());
  }
  else if(type == te::se::RGB_COMPOSITION)
  {
    m_rstTransform->setTransfFunction(te::map::RasterTransform::EXTRACT2RGB_TRANSF);

    m_rstTransform->clearRGBMap();

    getRedChannelProperties(cs->getRedChannel());

    getGreenChannelProperties(cs->getGreenChannel());

    getBlueChannelProperties(cs->getBlueChannel());
  }
  else if(type == te::se::RED_COMPOSITION)
  {
    m_rstTransform->setTransfFunction(te::map::RasterTransform::RED2THREE_TRANSF);

    m_rstTransform->clearRGBMap();

    getRedChannelProperties(cs->getRedChannel());
  }
  else if(type == te::se::GREEN_COMPOSITION)
  {
    m_rstTransform->setTransfFunction(te::map::RasterTransform::GREEN2THREE_TRANSF);

    m_rstTransform->clearRGBMap();

    getGreenChannelProperties(cs->getGreenChannel());
  }
  else if(type == te::se::BLUE_COMPOSITION)
  {
    m_rstTransform->setTransfFunction(te::map::RasterTransform::BLUE2THREE_TRANSF);

    m_rstTransform->clearRGBMap();

    getBlueChannelProperties(cs->getBlueChannel());
  }
  else
  {
     m_rstTransform->setTransfFunction(te::map::RasterTransform::NO_TRANSF);
  }
}

void te::map::RasterTransformConfigurer::getGrayChannelProperties(te::se::SelectedChannel* se)
{
  assert(se);

  int band = atoi(se->getSourceChannelName().c_str());

  m_rstTransform->setSrcBand(band);

  double gamma = getGammaProperty(se->getContrastEnhancement());

  m_rstTransform->setContrastM(gamma);
}

void te::map::RasterTransformConfigurer::getRedChannelProperties(te::se::SelectedChannel* se)
{
  assert(se);

  int band = atoi(se->getSourceChannelName().c_str());

  m_rstTransform->setBChannelMapping(band, te::map::RasterTransform::RED_CHANNEL);

  double gamma = getGammaProperty(se->getContrastEnhancement());

  m_rstTransform->setContrastR(gamma);
}

void te::map::RasterTransformConfigurer::getGreenChannelProperties(te::se::SelectedChannel* se)
{
  assert(se);

  int band = atoi(se->getSourceChannelName().c_str());

  m_rstTransform->setBChannelMapping(band, te::map::RasterTransform::GREEN_CHANNEL);

  double gamma = getGammaProperty(se->getContrastEnhancement());

  m_rstTransform->setContrastG(gamma);
}

void te::map::RasterTransformConfigurer::getBlueChannelProperties(te::se::SelectedChannel* se)
{
  assert(se);

  int band = atoi(se->getSourceChannelName().c_str());

  m_rstTransform->setBChannelMapping(band, te::map::RasterTransform::BLUE_CHANNEL);

  double gamma = getGammaProperty(se->getContrastEnhancement());

  m_rstTransform->setContrastB(gamma);
}

double te::map::RasterTransformConfigurer::getGammaProperty(te::se::ContrastEnhancement* ce)
{
  if(ce)
  {
    return ce->getGammaValue();
  }
  else
  {
    return TE_SE_DEFAULT_GAMMA_VALUE;
  }
}

void te::map::RasterTransformConfigurer::getColorMapInformation()
{
  te::se::ChannelSelection* cs = m_rstSymbolizer->getChannelSelection();

  if(cs && cs->getGrayChannel())
  {
    getGrayChannelProperties(cs->getGrayChannel());
  }
  else
  {
    return;
  }

  if(m_rstSymbolizer->getColorMap()->getCategorize())
  {
    m_rstTransform->setTransfFunction(te::map::RasterTransform::CATEGORIZE_TRANSF);

    getCategorizedMap(m_rstSymbolizer->getColorMap()->getCategorize());
  }
  else if(m_rstSymbolizer->getColorMap()->getInterpolate())
  {
    m_rstTransform->setTransfFunction(te::map::RasterTransform::INTERPOLATE_TRANSF);

    getInterpolatedMap(m_rstSymbolizer->getColorMap()->getInterpolate());
  }
  else
  {
    m_rstTransform->setTransfFunction(te::map::RasterTransform::NO_TRANSF);
  }
}

void te::map::RasterTransformConfigurer::getInterpolatedMap(te::se::Interpolate* interpolate)
{
  te::map::RasterTransform::InterpolatedMap map;

  std::vector<te::se::InterpolationPoint*> ip = interpolate->getInterpolationPoints();

  for(size_t i = 0; i < ip.size() - 1; ++i)
  {
    te::se::InterpolationPoint* ipItemInit = ip[i];

    std::string colorName = te::se::GetString(ipItemInit->getValue());
    te::color::RGBAColor colorInit(colorName);
    colorInit.setColor(colorInit.getRed(), colorInit.getGreen(), colorInit.getBlue());
    double lowerLimit = ipItemInit->getData();

    te::se::InterpolationPoint* ipItemEnd = ip[i + 1];

    colorName = te::se::GetString(ipItemEnd->getValue());
    te::color::RGBAColor colorEnd(colorName);
    colorEnd.setColor(colorEnd.getRed(), colorEnd.getGreen(), colorEnd.getBlue());
    double upperLimit = ipItemEnd->getData();

    te::color::ColorBar cb(colorInit, colorEnd, (int)(upperLimit - lowerLimit));

    map.insert(te::map::RasterTransform::InterpolatedMap::value_type(std::pair<double, double>(lowerLimit, upperLimit), cb)); 
  }

  m_rstTransform->setInterpolatedMap(map);
}

void te::map::RasterTransformConfigurer::getCategorizedMap(te::se::Categorize* caterogize)
{
  te::map::RasterTransform::CategorizedMap map;

  std::vector<te::se::ParameterValue*> t = caterogize->getThresholds();
  std::vector<te::se::ParameterValue*> tV = caterogize->getThresholdValues();

  for(size_t i = 0; i < tV.size(); ++i)
  {
    double lowerLimit, upperLimit;

    if(i == 0)
    {
      lowerLimit = std::numeric_limits<double>::min();
      upperLimit = te::se::GetDouble(t[i]);
    }
    else if(i == tV.size() - 1)
    {
      lowerLimit = te::se::GetDouble(t[i - 1]);
      upperLimit = std::numeric_limits<double>::max();
    }
    else
    {
      lowerLimit = te::se::GetDouble(t[i - 1]);
      upperLimit = te::se::GetDouble(t[i]);
    }

    std::string colorName = te::se::GetString(tV[i]);
    te::color::RGBAColor color(colorName);
    color.setColor(color.getRed(), color.getGreen(), color.getBlue());

    map.insert(te::map::RasterTransform::CategorizedMap::value_type(std::pair<double, double>(lowerLimit, upperLimit), color)); 
  }

  m_rstTransform->setCategorizedMap(map);
}
