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
  \file terralib/raster/RasterSummaryManager.cpp

  \brief A singleton for keeping raster summaries (most statistics).
*/

// TerraLib
#include "../common/STLUtils.h"
#include "Band.h"
#include "Raster.h"
#include "RasterSummary.h"
#include "RasterSummaryManager.h"

// STL
#include <complex>

std::string getConnInfoStr(const te::rst::Raster* raster);

void te::rst::RasterSummaryManager::add(const Raster* raster, RasterSummary* summary)
{
  std::string connInfoStr = getConnInfoStr(raster);

  if(m_rasterSummaries.find(connInfoStr) != m_rasterSummaries.end())
    m_rasterSummaries.erase(connInfoStr);

  if(!connInfoStr.empty())
    m_rasterSummaries.insert(std::map<std::string, RasterSummary*>::value_type(connInfoStr, summary));
}

const te::rst::RasterSummary* te::rst::RasterSummaryManager::find(const Raster* raster) const
{
  std::string connInfoStr = getConnInfoStr(raster);

  std::map<std::string, RasterSummary*>::const_iterator it = m_rasterSummaries.find(connInfoStr);

  if (it == m_rasterSummaries.end())
    return 0;

  return it->second;
}

void te::rst::RasterSummaryManager::remove(const Raster* raster)
{
  std::string connInfoStr = getConnInfoStr(raster);

  m_rasterSummaries.erase(connInfoStr);
}

const te::rst::RasterSummary* te::rst::RasterSummaryManager::get(const Raster* raster, const SummaryTypes types, bool readall)
{
  std::string connInfoStr = getConnInfoStr(raster);

  std::map<std::string, RasterSummary*>::const_iterator it = m_rasterSummaries.find(connInfoStr);

  te::rst::RasterSummary* rs = 0;

  if(it == m_rasterSummaries.end())
  {
    rs = new te::rst::RasterSummary(raster->getNumberOfBands());

    for (std::size_t i = 0; i < raster->getNumberOfBands(); i++)
      rs->push_back(new te::rst::BandSummary());

    add(raster, rs);
  }
  else
    rs = it->second;

  for (std::size_t b = 0; b < raster->getNumberOfBands(); b++)
  {
    te::rst::BandSummary& bs = (*rs)[b];

    if (types & te::rst::SUMMARY_R_HISTOGRAM && bs.m_histogramR == 0)
    {
      bs.m_histogramR = new std::map<double, unsigned>(raster->getBand(b)->getHistogramR());

      std::map<double, unsigned>::iterator it = bs.m_histogramR->begin();

      if (!bs.m_minVal)
        bs.m_minVal = new std::complex<double>(it->first, 0.0);
      else
        bs.m_minVal = new std::complex<double>(it->first, bs.m_minVal->imag());

      it = bs.m_histogramR->end();

      if (!bs.m_maxVal)
        bs.m_maxVal = new std::complex<double>((--it)->first, 0.0);
      else
        bs.m_maxVal = new std::complex<double>((--it)->first, bs.m_maxVal->imag());
    }

    if (types & te::rst::SUMMARY_I_HISTOGRAM && bs.m_histogramI == 0)
    {
      bs.m_histogramI = new std::map<double, unsigned>(raster->getBand(b)->getHistogramI());

      std::map<double, unsigned>::iterator it = bs.m_histogramI->begin();

      if (!bs.m_minVal)
        bs.m_minVal = new std::complex<double>(0.0, it->first);
      else
        bs.m_minVal = new std::complex<double>(bs.m_minVal->real(), it->first);

      it = bs.m_histogramI->end();

      if (!bs.m_maxVal)
        bs.m_maxVal = new std::complex<double>(0.0, (--it)->first);
      else
        bs.m_maxVal = new std::complex<double>(bs.m_maxVal->real(), (--it)->first);
    }

    if (types & te::rst::SUMMARY_MIN && bs.m_minVal == 0)
      bs.m_minVal = new std::complex<double>(raster->getBand(b)->getMinValue(readall));

    if (types & te::rst::SUMMARY_MAX && bs.m_maxVal == 0)
      bs.m_maxVal = new std::complex<double>(raster->getBand(b)->getMaxValue(readall));

    if (types & te::rst::SUMMARY_STD && bs.m_stdVal == 0)
      bs.m_stdVal = new std::complex<double>(raster->getBand(b)->getStdValue());

    if (types & te::rst::SUMMARY_MEAN && bs.m_meanVal == 0)
      bs.m_meanVal = new std::complex<double>(raster->getBand(b)->getMeanValue());
  }

  return rs;
}

te::rst::RasterSummaryManager::~RasterSummaryManager()
{
  te::common::FreeContents(m_rasterSummaries);

  m_rasterSummaries.clear();
}

te::rst::RasterSummaryManager::RasterSummaryManager()
{
}

std::string getConnInfoStr(const te::rst::Raster* raster)
{
  std::string connInfoStr = "";

  std::map<std::string, std::string> connInfo = raster->getInfo();

  std::map<std::string, std::string>::iterator it = connInfo.begin();

  while(it != connInfo.end())
  {
    connInfoStr += it->first;
    connInfoStr += "=";
    connInfoStr += it->second;
    connInfoStr += ";";

    ++it;
  }

  return connInfoStr;
}

