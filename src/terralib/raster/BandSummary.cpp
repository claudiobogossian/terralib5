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
  \file terralib/raster/BandSummary.cpp
 
  \brief A struct that can be used to keep the summary of a raster band (most statistics).
*/

// TerraLib
#include "BandSummary.h"

te::rst::BandSummary::BandSummary()
  : m_minVal(0),
    m_maxVal(0),
    m_stdVal(0),
    m_meanVal(0),
    m_histogramR(0),
    m_histogramI(0)
{
}

te::rst::BandSummary::BandSummary(const BandSummary& rhs)
  : m_minVal(0),
    m_maxVal(0),
    m_stdVal(0),
    m_meanVal(0),
    m_histogramR(0),
    m_histogramI(0)
{
  m_minVal = rhs.m_minVal ? new std::complex<double>(*rhs.m_minVal) : 0;
  m_maxVal = rhs.m_maxVal ? new std::complex<double>(*rhs.m_maxVal) : 0;
  m_stdVal = rhs.m_stdVal ? new std::complex<double>(*rhs.m_stdVal) : 0;
  m_meanVal = rhs.m_meanVal ? new std::complex<double>(*rhs.m_meanVal) : 0;
  m_histogramR = rhs.m_histogramR ? new std::map<double, unsigned int>(*rhs.m_histogramR) : 0;
  m_histogramI = rhs.m_histogramI ? new std::map<double, unsigned int>(*rhs.m_histogramI) : 0;
}

te::rst::BandSummary::~BandSummary()
{
  clear();
}

te::rst::BandSummary& te::rst::BandSummary::operator=(const BandSummary& rhs)
{
  if(this != &rhs)
  {
    clear();

    m_minVal = rhs.m_minVal ? new std::complex<double>(*rhs.m_minVal) : 0;
    m_maxVal = rhs.m_maxVal ? new std::complex<double>(*rhs.m_maxVal) : 0;
    m_stdVal = rhs.m_stdVal ? new std::complex<double>(*rhs.m_stdVal) : 0;
    m_meanVal = rhs.m_meanVal ? new std::complex<double>(*rhs.m_meanVal) : 0;
    m_histogramR = rhs.m_histogramR ? new std::map<double, unsigned int>(*rhs.m_histogramR) : 0;
    m_histogramI = rhs.m_histogramI ? new std::map<double, unsigned int>(*rhs.m_histogramI) : 0;
  }

  return *this;
}

void te::rst::BandSummary::clear()
{
  delete m_minVal;
  m_minVal = 0;

  delete m_maxVal;
  m_maxVal = 0;

  delete m_stdVal;
  m_stdVal = 0;

  delete m_meanVal;
  m_meanVal = 0;

  delete m_histogramR;
  m_histogramR = 0;

  delete m_histogramI;
  m_histogramI = 0;
}

