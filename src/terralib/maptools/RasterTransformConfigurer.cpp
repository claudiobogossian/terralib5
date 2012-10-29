/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
}

void te::map::RasterTransformConfigurer::getGeneralProperties()
{
  //get transparency
  if(m_rstSymbolizer->getOpacity())
  {
    double transp = (double)TE_OPAQUE * te::map::GetDouble(m_rstSymbolizer->getOpacity());

    m_rstTransform->setTransparency(transp);
  }

  //get gain
  if(m_rstSymbolizer->getGain())
  {
    double gain = te::map::GetDouble(m_rstSymbolizer->getGain());

    m_rstTransform->setGain(gain);
  }

  //get offset
  if(m_rstSymbolizer->getOffset())
  {
    double offset = te::map::GetDouble(m_rstSymbolizer->getOffset());

    m_rstTransform->setOffset(offset);
  }
}

void te::map::RasterTransformConfigurer::getChannelSelection()
{
  te::se::ChannelSelection* cs = m_rstSymbolizer->getChannelSelection();

  te::se::ColorCompositionType type = cs->getColorCompositionType();

  if(type == te::se::GRAY_COMPOSITION)
  {
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
