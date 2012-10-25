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
  \file RasterTransform.h

  \brief A Raster Transform is a class that defines functions to transform a styled raster.
*/

// TerraLib
#include "RasterTransform.h"

#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "../se/RasterSymbolizer.h"


te::map::RasterTransform::RasterTransform(te::rst::Raster* input, te::rst::Raster* output) :
  m_rasterIn(input),
  m_rasterOut(output),
  m_gain(1.),
  m_offset(0.),
  m_rContrast(TE_SE_DEFAULT_GAMMA_VALUE),
  m_gContrast(TE_SE_DEFAULT_GAMMA_VALUE),
  m_bContrast(TE_SE_DEFAULT_GAMMA_VALUE),
  m_mContrast(TE_SE_DEFAULT_GAMMA_VALUE),
  m_monoBand(0),
  m_monoBandOut(0),
  m_transfFuncPtr(&RasterTransform::ExtractRGB)
{
}

te::map::RasterTransform::~RasterTransform()
{
  m_rgbMap.clear();
}

void te::map::RasterTransform::setRGBMap(std::map<RGBChannels, short>& rgbMap)
{
  m_rgbMap.clear();
  m_rgbMap[RED_CHANNEL] = -1;
  m_rgbMap[GREEN_CHANNEL] = -1;
  m_rgbMap[BLUE_CHANNEL] = -1;

  std::map<RGBChannels,short>::iterator it = m_rgbMap.begin();
  while (it != m_rgbMap.end())
  {
    if (it->first == RED_CHANNEL)
    m_rgbMap[RED_CHANNEL] = it->second;

    else if (it->first == GREEN_CHANNEL)
    m_rgbMap[GREEN_CHANNEL] = it->second;

    else if (it->first == BLUE_CHANNEL)
    m_rgbMap[BLUE_CHANNEL] = it->second;

    ++it;
  }
}

void te::map::RasterTransform::setLinearTransfParameters(double vmin, double vmax, double rmin, double rmax)
{
  m_rstMinValue = rmin;
  m_rstMaxValue = rmax;

  m_gain = (double)(rmax-rmin)/(vmax-vmin);
  m_offset = -1*m_gain*vmin+rmin;
}

te::map::RasterTransform::RasterTransfFunctions te::map::RasterTransform::getTransfFunction()
{
  if (m_transfFuncPtr == 0)
  {
    return NO_TRANSF;
  }
  else  if (m_transfFuncPtr == &RasterTransform::Mono2ThreeBand)
  {
    return MONO2THREE_TRANSF;
  }
  else if (m_transfFuncPtr == &RasterTransform::ExtractRGB)
  {
    return EXTRACT2RGB_TRANSF;
  }
  else
  {
    return NO_TRANSF;
  }
}

void te::map::RasterTransform::setTransfFunction(RasterTransfFunctions func)
{
  if (func == MONO2THREE_TRANSF)
  {
    m_transfFuncPtr = &RasterTransform::Mono2ThreeBand;
  }
  else if (func == EXTRACT2RGB_TRANSF)
  {
    m_transfFuncPtr = &RasterTransform::ExtractRGB;
  }
  else
  {
    m_transfFuncPtr = 0;
  }
}

void te::map::RasterTransform::Mono2ThreeBand(double icol, double ilin, double ocol, double olin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_monoBand);

  val = (val * m_gain + m_offset) * m_mContrast;

  fixValue(val);

  std::vector<double> vecValues;
  vecValues.resize(3, val);

  m_rasterOut->setValues((int)icol, (int)ilin, vecValues);
}

void te::map::RasterTransform::ExtractRGB(double icol, double ilin, double ocol, double olin)
{
  double valR, valG, valB;

  m_rasterIn->getValue((int)icol, (int)ilin, valR, m_rgbMap[RED_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valG, m_rgbMap[GREEN_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valB, m_rgbMap[BLUE_CHANNEL]);

  std::vector<double> vecValues;

  valR = (valR * m_gain + m_offset) * m_rContrast;
  fixValue(valR);
  vecValues.push_back(valR);

  valG = (valG * m_gain + m_offset) * m_gContrast;
  fixValue(valG);
  vecValues.push_back(valG);

  valB = (valB * m_gain + m_offset) * m_bContrast;
  fixValue(valB);
  vecValues.push_back(valB);

  m_rasterOut->setValues((int)icol, (int)ilin, vecValues);
}

void te::map::RasterTransform::fixValue(double& value)
{
  if (value < m_rstMinValue)
  {
    value = m_rstMinValue;
  }
  else if (value > m_rstMaxValue)
  {
    value = m_rstMaxValue;
  }
}

