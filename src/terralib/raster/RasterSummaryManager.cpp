/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
#include "Band.h"
#include "Raster.h"
#include "RasterSummary.h"
#include "RasterSummaryManager.h"

// STL
#include <complex>

void te::rst::RasterSummaryManager::add(const Raster* raster, RasterSummary* summary)
{
  if (m_rasterSummaries.find(raster) != m_rasterSummaries.end())
    m_rasterSummaries.erase(raster);

  m_rasterSummaries.insert(raster, summary);
}

const te::rst::RasterSummary* te::rst::RasterSummaryManager::find(const Raster* raster) const
{
  if (m_rasterSummaries.find(raster) == m_rasterSummaries.end())
    return 0;

  return &m_rasterSummaries.at(raster);
}

void te::rst::RasterSummaryManager::remove(const Raster* raster)
{
  m_rasterSummaries.erase(raster);
}

const te::rst::RasterSummary* te::rst::RasterSummaryManager::get(const Raster* raster, const SummaryTypes types)
{
  boost::ptr_map<const Raster*, RasterSummary>::iterator it = m_rasterSummaries.find(raster);

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
    //rs = new te::rst::RasterSummary(*find(raster));

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
      bs.m_minVal = new std::complex<double>(raster->getBand(b)->getMinValue(true));

    if (types & te::rst::SUMMARY_MAX && bs.m_maxVal == 0)
      bs.m_maxVal = new std::complex<double>(raster->getBand(b)->getMaxValue(true));

    if (types & te::rst::SUMMARY_STD && bs.m_stdVal == 0)
      bs.m_stdVal = new std::complex<double>(raster->getBand(b)->getStdValue());

    if (types & te::rst::SUMMARY_MEAN && bs.m_meanVal == 0)
      bs.m_meanVal = new std::complex<double>(raster->getBand(b)->getMeanValue());
  }

  return rs;
}

te::rst::RasterSummaryManager::~RasterSummaryManager()
{
}

te::rst::RasterSummaryManager::RasterSummaryManager()
{
}

