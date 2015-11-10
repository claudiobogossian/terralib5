/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

#include "../raster.h"
#include "../raster/RasterProperty.h"
#include "../se/RasterSymbolizer.h"

// Boost
#include <boost/math/special_functions/fpclassify.hpp>

// STL
#include <limits>

te::map::RasterTransform::RasterTransform(te::rst::Raster* input, te::rst::Raster* output) :
  m_rasterIn(input),
  m_rasterOut(output),
  m_transp(TE_OPAQUE),
  m_gain(1.),
  m_offset(0.),
  m_rContrast(TE_SE_DEFAULT_GAMMA_VALUE),
  m_gContrast(TE_SE_DEFAULT_GAMMA_VALUE),
  m_bContrast(TE_SE_DEFAULT_GAMMA_VALUE),
  m_mContrast(TE_SE_DEFAULT_GAMMA_VALUE),
  m_monoBand(0),
  m_monoBandOut(0),
  m_transfFuncPtr(&RasterTransform::setExtractRGB),
  m_RGBAFuncPtr(&RasterTransform::getExtractRGB)
{
  m_rstMinValue = -std::numeric_limits<double>::max();
  m_rstMaxValue = std::numeric_limits<double>::max();
}

te::map::RasterTransform::~RasterTransform()
{
  m_categorizeMap.clear();
  m_interpolateMap.clear();
  m_recodeMap.clear();
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

  if(vmax == vmin)
  {
    m_gain = 1.0;
    m_offset = 0.0;
    return;
  }

  m_gain = (double)(rmax-rmin)/(vmax-vmin);

  if(boost::math::isnan(m_gain) || boost::math::isinf(m_gain))
  {
    m_gain = 1.0;
    m_offset = 0.0;
    return;
  }

  m_offset = -1*m_gain*vmin+rmin;
}

te::map::RasterTransform::RasterTransfFunctions te::map::RasterTransform::getTransfFunction()
{
  if (m_transfFuncPtr == 0 || m_RGBAFuncPtr)
  {
    return NO_TRANSF;
  }
  else  if (m_transfFuncPtr == &RasterTransform::setMono2ThreeBand &&
            m_RGBAFuncPtr == &RasterTransform::getMono2ThreeBand)
  {
    return MONO2THREE_TRANSF;
  }
  else if (m_transfFuncPtr == &RasterTransform::setExtractRGB &&
           m_RGBAFuncPtr == &RasterTransform::getExtractRGB)
  {
    return EXTRACT2RGB_TRANSF;
  }
  else  if (m_transfFuncPtr == &RasterTransform::setRed2ThreeBand &&
            m_RGBAFuncPtr == &RasterTransform::getRed2ThreeBand)
  {
    return RED2THREE_TRANSF;
  }
  else  if (m_transfFuncPtr == &RasterTransform::setGreen2ThreeBand &&
            m_RGBAFuncPtr == &RasterTransform::getGreen2ThreeBand)
  {
    return GREEN2THREE_TRANSF;
  }
  else  if (m_transfFuncPtr == &RasterTransform::setBlue2ThreeBand &&
            m_RGBAFuncPtr == &RasterTransform::getBlue2ThreeBand)
  {
    return BLUE2THREE_TRANSF;
  }
  else  if (m_transfFuncPtr == &RasterTransform::setCategorize &&
            m_RGBAFuncPtr == &RasterTransform::getCategorize)
  {
    return CATEGORIZE_TRANSF;
  }
  else  if (m_transfFuncPtr == &RasterTransform::setInterpolate &&
            m_RGBAFuncPtr == &RasterTransform::getInterpolate)
  {
    return INTERPOLATE_TRANSF;
  }
  else  if (m_transfFuncPtr == &RasterTransform::setRecode &&
    m_RGBAFuncPtr == &RasterTransform::getRecode)
  {
    return RECODE_TRANSF;
  }
  else  if (m_transfFuncPtr == &RasterTransform::setBand2Band)
  {
    return BAND2BAND_TRANSF;
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
    m_transfFuncPtr = &RasterTransform::setMono2ThreeBand;
    m_RGBAFuncPtr = &RasterTransform::getMono2ThreeBand;
  }
  else if (func == EXTRACT2RGB_TRANSF)
  {
    m_transfFuncPtr = &RasterTransform::setExtractRGB;
    m_RGBAFuncPtr = &RasterTransform::getExtractRGB;
  }
  else if (func == RED2THREE_TRANSF)
  {
    m_transfFuncPtr = &RasterTransform::setRed2ThreeBand;
    m_RGBAFuncPtr = &RasterTransform::getRed2ThreeBand;
  }
  else if (func == GREEN2THREE_TRANSF)
  {
    m_transfFuncPtr = &RasterTransform::setGreen2ThreeBand;
    m_RGBAFuncPtr = &RasterTransform::getGreen2ThreeBand;
  }
  else if (func == BLUE2THREE_TRANSF)
  {
    m_transfFuncPtr = &RasterTransform::setBlue2ThreeBand;
    m_RGBAFuncPtr = &RasterTransform::getBlue2ThreeBand;
  }
  else if (func == CATEGORIZE_TRANSF)
  {
    m_transfFuncPtr = &RasterTransform::setCategorize;
    m_RGBAFuncPtr = &RasterTransform::getCategorize;
  }
  else if (func == INTERPOLATE_TRANSF)
  {
    m_transfFuncPtr = &RasterTransform::setInterpolate;
    m_RGBAFuncPtr = &RasterTransform::getInterpolate;
  }
  else if (func == RECODE_TRANSF)
  {
    m_transfFuncPtr = &RasterTransform::setRecode;
    m_RGBAFuncPtr = &RasterTransform::getRecode;
  }
  else if (func == BAND2BAND_TRANSF)
  {
    m_transfFuncPtr = &RasterTransform::setBand2Band;
    m_RGBAFuncPtr = 0;
  }
  else if (func == EXTRACT2RGBA_TRANSF)
  {
    m_transfFuncPtr = &RasterTransform::setExtractRGBA;
    m_RGBAFuncPtr = &RasterTransform::getExtractRGBA;
  }
  else
  {
    m_transfFuncPtr = 0;
    m_RGBAFuncPtr = 0;
  }
}

void te::map::RasterTransform::setMono2ThreeBand(double icol, double ilin, double ocol, double olin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_monoBand);

  if(checkNoValue(val, m_monoBand) == false)
  {
    val = (val * m_gain + m_offset) * m_mContrast;

    fixValue(val);

    std::vector<double> vecValues;
    vecValues.resize(3, val);

    m_rasterOut->setValues((int)ocol, (int)olin, vecValues);
  }
}

te::color::RGBAColor te::map::RasterTransform::getMono2ThreeBand(double icol, double ilin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_monoBand);

  if(checkNoValue(val, m_monoBand) == false)
  {
    val = (val * m_gain + m_offset) * m_mContrast;

    fixValue(val);

    te::color::RGBAColor c(static_cast<int>(val), static_cast<int>(val), static_cast<int>(val), static_cast<int>(m_transp));

    return c;
  }

  return te::color::RGBAColor();
}

void te::map::RasterTransform::setExtractRGB(double icol, double ilin, double ocol, double olin)
{
  double valR, valG, valB;

  m_rasterIn->getValue((int)icol, (int)ilin, valR, m_rgbMap[RED_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valG, m_rgbMap[GREEN_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valB, m_rgbMap[BLUE_CHANNEL]);

  if(checkNoValue(valR, m_rgbMap[RED_CHANNEL]) == false ||
     checkNoValue(valG, m_rgbMap[GREEN_CHANNEL]) == false ||
     checkNoValue(valB, m_rgbMap[BLUE_CHANNEL]) == false)
  {
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

    m_rasterOut->setValues((int)ocol, (int)olin, vecValues);
  }
}

te::color::RGBAColor te::map::RasterTransform::getExtractRGB(double icol, double ilin)
{
  double valR, valG, valB;

  m_rasterIn->getValue((int)icol, (int)ilin, valR, m_rgbMap[RED_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valG, m_rgbMap[GREEN_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valB, m_rgbMap[BLUE_CHANNEL]);

  if(checkNoValue(valR, m_rgbMap[RED_CHANNEL]) == false ||
     checkNoValue(valG, m_rgbMap[GREEN_CHANNEL]) == false ||
     checkNoValue(valB, m_rgbMap[BLUE_CHANNEL]) == false)
  {
    valR = (valR * m_gain + m_offset) * m_rContrast;
    fixValue(valR);

    valG = (valG * m_gain + m_offset) * m_gContrast;
    fixValue(valG);

    valB = (valB * m_gain + m_offset) * m_bContrast;
    fixValue(valB);

    te::color::RGBAColor c(static_cast<int>(valR), static_cast<int>(valG), static_cast<int>(valB), static_cast<int>(m_transp));

    return c;
  }

  return te::color::RGBAColor();
}

void te::map::RasterTransform::setExtractRGBA(double icol, double ilin, double ocol, double olin)
{
  double valR, valG, valB, valA;

  m_rasterIn->getValue((int)icol, (int)ilin, valR, m_rgbMap[RED_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valG, m_rgbMap[GREEN_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valB, m_rgbMap[BLUE_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valA, m_rgbMap[ALPHA_CHANNEL]);

  if(checkNoValue(valR, m_rgbMap[RED_CHANNEL]) == false ||
     checkNoValue(valG, m_rgbMap[GREEN_CHANNEL]) == false ||
     checkNoValue(valB, m_rgbMap[BLUE_CHANNEL]) == false ||
     checkNoValue(valA, m_rgbMap[ALPHA_CHANNEL]) == false)
  {
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

    fixValue(valA);
    vecValues.push_back(valB);

    m_rasterOut->setValues((int)ocol, (int)olin, vecValues);
  }
}

te::color::RGBAColor te::map::RasterTransform::getExtractRGBA(double icol, double ilin)
{
  double valR, valG, valB, valA;

  m_rasterIn->getValue((int)icol, (int)ilin, valR, m_rgbMap[RED_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valG, m_rgbMap[GREEN_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valB, m_rgbMap[BLUE_CHANNEL]);
  m_rasterIn->getValue((int)icol, (int)ilin, valA, m_rgbMap[ALPHA_CHANNEL]);

  if(checkNoValue(valR, m_rgbMap[RED_CHANNEL]) == false ||
     checkNoValue(valG, m_rgbMap[GREEN_CHANNEL]) == false ||
     checkNoValue(valB, m_rgbMap[BLUE_CHANNEL]) == false ||
     checkNoValue(valA, m_rgbMap[ALPHA_CHANNEL]) == false)
  {
    valR = (valR * m_gain + m_offset) * m_rContrast;
    fixValue(valR);

    valG = (valG * m_gain + m_offset) * m_gContrast;
    fixValue(valG);

    valB = (valB * m_gain + m_offset) * m_bContrast;
    fixValue(valB);

    fixValue(valA);

    if(valA < m_transp)
      return te::color::RGBAColor(static_cast<int>(valR), static_cast<int>(valG), static_cast<int>(valB), static_cast<int>(valA));
    else
      return te::color::RGBAColor(static_cast<int>(valR), static_cast<int>(valG), static_cast<int>(valB), static_cast<int>(m_transp));
  }

  return te::color::RGBAColor();
}

void te::map::RasterTransform::setRed2ThreeBand(double icol, double ilin, double ocol, double olin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_rgbMap[RED_CHANNEL]);

  if(checkNoValue(val, m_rgbMap[RED_CHANNEL]) == false)
  {
    val = (val * m_gain + m_offset) * m_rContrast;

    fixValue(val);

    std::vector<double> vecValues;
    vecValues.push_back(val);
    vecValues.push_back(0.);
    vecValues.push_back(0.);

    m_rasterOut->setValues((int)ocol, (int)olin, vecValues);
  }
}

te::color::RGBAColor te::map::RasterTransform::getRed2ThreeBand(double icol, double ilin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_rgbMap[RED_CHANNEL]);

  if(checkNoValue(val, m_rgbMap[RED_CHANNEL]) == false)
  {
    val = (val * m_gain + m_offset) * m_rContrast;

    fixValue(val);

    te::color::RGBAColor c(static_cast<int>(val), static_cast<int>(0.), static_cast<int>(0.), static_cast<int>(m_transp));

    return c;

  }

  return te::color::RGBAColor();
}

void te::map::RasterTransform::setGreen2ThreeBand(double icol, double ilin, double ocol, double olin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_rgbMap[GREEN_CHANNEL]);

  if(checkNoValue(val, m_rgbMap[GREEN_CHANNEL]) == false)
  {
    val = (val * m_gain + m_offset) * m_gContrast;

    fixValue(val);

    std::vector<double> vecValues;
    vecValues.push_back(0.);
    vecValues.push_back(val);
    vecValues.push_back(0.);

    m_rasterOut->setValues((int)ocol, (int)olin, vecValues);
  }
}

te::color::RGBAColor te::map::RasterTransform::getGreen2ThreeBand(double icol, double ilin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_rgbMap[GREEN_CHANNEL]);

  if(checkNoValue(val, m_rgbMap[GREEN_CHANNEL]) == false)
  {
    val = (val * m_gain + m_offset) * m_gContrast;

    fixValue(val);

    te::color::RGBAColor c(static_cast<int>(0.), static_cast<int>(val), static_cast<int>(0.), static_cast<int>(m_transp));

    return c;

  }

  return te::color::RGBAColor();
}

void te::map::RasterTransform::setBlue2ThreeBand(double icol, double ilin, double ocol, double olin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_rgbMap[BLUE_CHANNEL]);

  if(checkNoValue(val, m_rgbMap[BLUE_CHANNEL]) == false)
  {
    val = (val * m_gain + m_offset) * m_bContrast;

    fixValue(val);

    std::vector<double> vecValues;
    vecValues.push_back(0.);
    vecValues.push_back(0.);
    vecValues.push_back(val);

    m_rasterOut->setValues((int)ocol, (int)olin, vecValues);
  }
}

te::color::RGBAColor te::map::RasterTransform::getBlue2ThreeBand(double icol, double ilin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_rgbMap[BLUE_CHANNEL]);

  if(checkNoValue(val, m_rgbMap[BLUE_CHANNEL]) == false)
  {
    val = (val * m_gain + m_offset) * m_bContrast;

    fixValue(val);

    te::color::RGBAColor c(static_cast<int>(0.), static_cast<int>(0.), static_cast<int>(val), static_cast<int>(m_transp));

    return c;

  }

  return te::color::RGBAColor();
}

void te::map::RasterTransform::setCategorize(double icol, double ilin, double ocol, double olin)
{
   double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_monoBand);

  te::color::RGBAColor c=  getCategorizedColor(val);

  std::vector<double> vecValues;
  vecValues.push_back(c.getRed());
  vecValues.push_back(c.getGreen());
  vecValues.push_back(c.getBlue());

  m_rasterOut->setValues((int)ocol, (int)olin, vecValues);
}

te::color::RGBAColor te::map::RasterTransform::getCategorize(double icol, double ilin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_monoBand);

  if(checkNoValue(val, m_monoBand) == false)
    return getCategorizedColor(val);

  return te::color::RGBAColor();
}

void te::map::RasterTransform::setInterpolate(double icol, double ilin, double ocol, double olin)
{
   double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_monoBand);

  te::color::RGBAColor c=  getInterpolatedColor(val);

  std::vector<double> vecValues;
  vecValues.push_back(c.getRed());
  vecValues.push_back(c.getGreen());
  vecValues.push_back(c.getBlue());

  m_rasterOut->setValues((int)ocol, (int)olin, vecValues);
}

te::color::RGBAColor te::map::RasterTransform::getInterpolate(double icol, double ilin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_monoBand);

  if(checkNoValue(val, m_monoBand) == false)
    return getInterpolatedColor(val);

  return te::color::RGBAColor();
}

void te::map::RasterTransform::setRecode(double icol, double ilin, double ocol, double olin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_monoBand);

  te::color::RGBAColor c = getRecodedColor(val);

  std::vector<double> vecValues;
  vecValues.push_back(c.getRed());
  vecValues.push_back(c.getGreen());
  vecValues.push_back(c.getBlue());

  m_rasterOut->setValues((int)ocol, (int)olin, vecValues);
}

te::color::RGBAColor te::map::RasterTransform::getRecode(double icol, double ilin)
{
  double val;

  m_rasterIn->getValue((int)icol, (int)ilin, val, m_monoBand);

  if (checkNoValue(val, m_monoBand) == false)
    return getRecodedColor(val);

  return te::color::RGBAColor();
}

void te::map::RasterTransform::setBand2Band(double icol, double ilin, double ocol, double olin)
{
 double val;

  for(std::size_t t = 0; t < m_rasterIn->getNumberOfBands(); ++t)
  {
    m_rasterIn->getValue((int)icol, (int)ilin, val, t);

    if(checkNoValue(val, t) == false)
    {
      val = (val * m_gain + m_offset);

      fixValue(val);

      m_rasterOut->setValue((int)ocol, (int)olin, val, t);
    }
  }
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

bool te::map::RasterTransform::checkNoValue(double& value, int band)
{
  if(m_rasterIn->getBand(band)->getProperty()->m_noDataValue == value)
  {
    return true;
  }

  return false;
}

te::color::RGBAColor te::map::RasterTransform::getInterpolatedColor(double value)
{
  InterpolatedMap::iterator it = m_interpolateMap.begin();

  while(it != m_interpolateMap.end())
  {
    RasterThreshold rt = it->first;

    if(rt.first<= value && rt.second > value)
    {
      int distance = int(value - it->first.first);

      if(distance < 0)
      {
        distance = 0;
      }
      else if(distance >= (int)it->second.getColorBar().size())
      {
        distance = it->second.getColorBar().size() - 1;
      }

      return it->second.getColorBar()[distance];
    }

    ++it;
  }

  return te::color::RGBAColor();
}

te::color::RGBAColor te::map::RasterTransform::getCategorizedColor(double value)
{
  CategorizedMap::iterator it = m_categorizeMap.begin();

  while(it != m_categorizeMap.end())
  {
    RasterThreshold rt = it->first;

    if(rt.first<= value && rt.second > value)
    {
      return it->second;
    }

    ++it;
  }

  return te::color::RGBAColor();
}

te::color::RGBAColor te::map::RasterTransform::getRecodedColor(double value)
{
  return m_recodeMap[(int)value];
}