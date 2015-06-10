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
  \file terralib/raster/Band.cpp

  \brief It gives access to values in one band (dimension) of a raster.
*/

// TerraLib

#include "Band.h"
#include "BandProperty.h"
#include "PositionIterator.h"
#include "Utils.h"

// STL
#include <cassert>
#include <limits>

te::rst::Band::Band(BandProperty* p, std::size_t idx)
  : m_property(p),
    m_idx(idx)
{
}

te::rst::Band::~Band()
{
  delete m_property;
}

te::rst::Band& te::rst::Band::operator=(const Band& rhs)
{
  if(this != &rhs)
  {
    delete m_property;

    m_property = rhs.m_property ? new BandProperty(*rhs.m_property) : 0;

    m_idx = rhs.m_idx;
  }

  return *this;
}

void te::rst::Band::getValue(unsigned int c, unsigned int r, std::complex<double>& value) const
{
  double vr = 0.0;

  double vi = 0.0;

  getValue(c, r, vr);

  getIValue(c, r, vi);

  value = std::complex<double>(vr, vi);
}

void te::rst::Band::setValue(unsigned int c, unsigned int r, const std::complex<double>& value)
{
  setValue(c, r, value.real());

  setIValue(c, r, value.imag());
}

std::complex<double> te::rst::Band::getMinValue(bool readall, unsigned int rs, unsigned int cs, unsigned int rf, unsigned int cf) const
{
  std::complex<double> pixel;
  double min_img = std::numeric_limits<double>::max();
  double min_real = std::numeric_limits<double>::max();
  double no_data = getProperty()->m_noDataValue;

  if ((rf == cf) && (rf == 0))
  {
    rf = getRaster()->getNumberOfRows() - 1;
    cf = getRaster()->getNumberOfColumns() - 1;

// read up to 1000 pixels in random positions
    const unsigned int maxInputPoints = 1000;
    if (readall == false && (rf * cf) > maxInputPoints)
    {
      std::vector<te::gm::Point*> randomPoints = te::rst::GetRandomPointsInRaster(*getRaster(), maxInputPoints);
      te::rst::PointSetIterator<double> pit = te::rst::PointSetIterator<double>::begin(getRaster(), randomPoints);
      te::rst::PointSetIterator<double> pitend = te::rst::PointSetIterator<double>::end(getRaster(), randomPoints);
      while (pit != pitend)
      {
        getValue(pit.getColumn(), pit.getRow(), pixel);
        ++pit;

        if(pixel.real() == no_data)
          continue;

        if (pixel.real() < min_real)
          min_real = pixel.real();

        if (pixel.imag() < min_img)
          min_img = pixel.imag();
      }
      
      return std::complex<double>(min_real, min_img);
    }
  }
  
// read all pixels in range
  for (unsigned r = rs; r <= rf; r++)
    for (unsigned c = cs; c <= cf; c++)
    {
      getValue(c, r, pixel);

      if(pixel.real() == no_data)
        continue;

      if (pixel.real() < min_real)
        min_real = pixel.real();

      if (pixel.imag() < min_img)
        min_img = pixel.imag();
    }

  return std::complex<double>(min_real, min_img);
}

std::complex<double> te::rst::Band::getMaxValue(bool readall, unsigned int rs, unsigned int cs, unsigned int rf, unsigned int cf) const
{
  std::complex<double> pixel;
  double max_img = std::numeric_limits<double>::min();
  double max_real = std::numeric_limits<double>::min();
  double no_data = getProperty()->m_noDataValue;

  if ((rf == cf) && (rf == 0))
  {
    rf = getRaster()->getNumberOfRows() - 1;
    cf = getRaster()->getNumberOfColumns() - 1;

// read up to 1000 pixels in random positions
    const unsigned int maxInputPoints = 1000;
    if (readall == false && (rf * cf) > maxInputPoints)
    {
      std::vector<te::gm::Point*> randomPoints = te::rst::GetRandomPointsInRaster(*getRaster(), maxInputPoints);
      te::rst::PointSetIterator<double> pit = te::rst::PointSetIterator<double>::begin(getRaster(), randomPoints);
      te::rst::PointSetIterator<double> pitend = te::rst::PointSetIterator<double>::end(getRaster(), randomPoints);
      while (pit != pitend)
      {
        getValue(pit.getColumn(), pit.getRow(), pixel);
        ++pit;

        if(pixel.real() == no_data)
          continue;

        if (pixel.real() > max_real)
          max_real = pixel.real();

        if (pixel.imag() > max_img)
          max_img = pixel.imag();
      }
      
      return std::complex<double>(max_real, max_img);
    }
  }
  
// read all pixels in range
  for (unsigned r = rs; r <= rf; r++)
    for (unsigned c = cs; c <= cf; c++)
    {
      getValue(c, r, pixel);

      if(pixel.real() == no_data)
        continue;

      if (pixel.real() > max_real)
        max_real = pixel.real();

      if (pixel.imag() > max_img)
        max_img = pixel.imag();
    }

  return std::complex<double>(max_real, max_img);
}

std::complex<double> te::rst::Band::getStdValue(unsigned int rs, unsigned int cs, unsigned int rf, unsigned int cf) const
{
  if ((rf == cf) && (rf  == 0))
  {
    rf = getRaster()->getNumberOfRows() - 1;

    cf = getRaster()->getNumberOfColumns() - 1;
  }

  std::complex<double> pixel;

  std::complex<double> mean = getMeanValue(rs, cs, rf, cf);

  std::complex<double> diff;

  std::complex<double> sumDiffs(0.0, 0.0);

  unsigned n = (rf-rs+1) * (cf-cs+1) - 1;

  if (n == 0)
    return std::complex<double> (1.0, 1.0);

  for (unsigned r = rs; r <= rf; r++)
    for (unsigned c = cs; c <= cf; c++)
    {
      getValue(c, r, pixel);

      diff = pixel - mean;

      sumDiffs += diff * diff;
    }

  return std::complex<double> (std::sqrt(sumDiffs.real() / n), std::sqrt(sumDiffs.imag() / n));
}

std::complex<double> te::rst::Band::getMeanValue(unsigned int rs, unsigned int cs, unsigned int rf, unsigned int cf) const
{
  if ((rf == cf) && (rf  == 0))
  {
    rf = getRaster()->getNumberOfRows() - 1;

    cf = getRaster()->getNumberOfColumns() - 1;
  }

  std::complex<double> pixel;

  std::complex<double> sumValue(0.0, 0.0);

  unsigned int n = (rf-rs+1) * (cf-cs+1);

  for (unsigned r = rs; r <= rf; r++)
    for (unsigned c = cs; c <= cf; c++)
    {
      getValue(c, r, pixel);

      sumValue += pixel;
    }

  if (n == 0)
    return std::complex<double> (0.0, 0.0);

  return std::complex<double> (sumValue.real() / n, sumValue.imag() / n);
}

std::map<double, unsigned> te::rst::Band::getHistogramR(unsigned int rs, unsigned int cs, unsigned int rf, unsigned int cf, unsigned int b) const
{
  if ((rf == cf) && (rf  == 0))
  {
    rf = getRaster()->getNumberOfRows() - 1;

    cf = getRaster()->getNumberOfColumns() - 1;
  }

  std::map<double, unsigned> hist;

  double pixel;

  if (b == 0)
    for (unsigned r = rs; r <= rf; r++)
      for (unsigned c = cs; c <= cf; c++)
      {
        getValue(c, r, pixel);

        hist[pixel]++;
      }
  else
  {
// find limits to divide into bins
    double pmin = std::numeric_limits<double>::max();
    double pmax = -1.0 * std::numeric_limits<double>::max();

    for (unsigned r = rs; r <= rf; r++)
      for (unsigned c = cs; c <= cf; c++)
      {
        getValue(c, r, pixel);

        if (pixel > pmax)
          pmax = pixel;

        if (pixel < pmin)
          pmin = pixel;
      }

// create histogram with bins
    double delta = (pmax * 1.000001 - pmin) / b;

    std::map<std::size_t, double> binsLocations;

    std::size_t location = 0;

    for (double bins = pmin; bins < pmax; bins += delta)
    {
      hist[bins] = 0;

      binsLocations[location++] = bins;
    }

// fill histogram
    for (unsigned r = rs; r <= rf; r++)
      for (unsigned c = cs; c <= cf; c++)
      {
        getValue(c, r, pixel);

        location = (std::size_t) ((pixel - pmin) / delta);

        hist[binsLocations[location]]++;
      }

// removing empty bins from histogram
    for (double bins = pmin; bins < pmax; bins += delta)
      if(hist[bins] == 0)
        hist.erase(bins);

  }

  return hist;
}

std::map<double, unsigned> te::rst::Band::getHistogramI(unsigned int rs, unsigned int cs, unsigned int rf, unsigned int cf, unsigned int b) const
{
  if ((rf == cf) && (rf  == 0))
  {
    rf = getRaster()->getNumberOfRows() - 1;

    cf = getRaster()->getNumberOfColumns() - 1;
  }

  std::map<double, unsigned> hist;

  double pixel;

  if (b == 0)
    for (unsigned r = rs; r <= rf; r++)
      for (unsigned c = cs; c <= cf; c++)
      {
        getIValue(c, r, pixel);

        hist[pixel]++;
      }
  else
  {
// find limits to divide into bins
    double pmin = std::numeric_limits<double>::max();
    double pmax = -1.0 * std::numeric_limits<double>::max();

    for (unsigned r = rs; r <= rf; r++)
      for (unsigned c = cs; c <= cf; c++)
      {
        getIValue(c, r, pixel);

        if (pixel > pmax)
          pmax = pixel;

        if (pixel < pmin)
          pmin = pixel;
      }

// create histogram with bins
    double delta = (pmax * 1.000001 - pmin) / b;

    std::map<std::size_t, double> binsLocations;

    std::size_t location = 0;

    for (double bins = pmin; bins < pmax; bins += delta)
    {
      hist[bins] = 0;

      binsLocations[location++] = bins;
    }

// fill histogram
    for (unsigned r = rs; r <= rf; r++)
      for (unsigned c = cs; c <= cf; c++)
      {
        getIValue(c, r, pixel);

        location = (std::size_t) ((pixel - pmin) / delta);

        hist[binsLocations[location]]++;
      }

// removing empty bins from histogram
    for (double bins = pmin; bins < pmax; bins += delta)
      if(hist[bins] == 0)
        hist.erase(bins);

  }

  return hist;
}

std::complex<double> te::rst::Band::getScaleValue() const
{
  return m_property->m_valuesScale;
}

void te::rst::Band::setScaleValue(const std::complex<double> s)
{
  m_property->m_valuesScale = s;
}

std::complex<double> te::rst::Band::getOffsetValue() const
{
  return m_property->m_valuesOffset;
}

void te::rst::Band::setOffsetValue(const std::complex<double> o)
{
  m_property->m_valuesOffset = o;
}

te::rst::BandProperty* te::rst::Band::getProperty()
{
  return m_property;
}

const te::rst::BandProperty* te::rst::Band::getProperty() const
{
  return m_property;
}

te::rst::Band& te::rst::Band::callOperator(std::complex<double>(*f)(std::complex<double>, std::complex<double>), te::rst::Band& rhs)
{
  assert(getRaster()->getNumberOfRows() == rhs.getRaster()->getNumberOfRows());
  assert(getRaster()->getNumberOfColumns() == rhs.getRaster()->getNumberOfColumns());
  assert(getRaster()->getAccessPolicy() == te::common::RWAccess || getRaster()->getAccessPolicy() == te::common::WAccess);

  std::complex<double> lhsv, rhsv;

  std::vector<std::pair<unsigned, unsigned> > rcStPos, rcFPos;

  unsigned last_y;
  unsigned last_x;
  for (unsigned x = 0; x < (unsigned) getProperty()->m_nblocksx; x++)
  {
    for (unsigned y = 0; y < (unsigned) getProperty()->m_nblocksy; y++)
    {
      rcStPos.push_back(std::pair<unsigned, unsigned> (y * getProperty()->m_blkh, x * getProperty()->m_blkw));
      last_y = (y + 1) * getProperty()->m_blkh;
      last_x = (x + 1) * getProperty()->m_blkw;
      if (last_y > getRaster()->getNumberOfRows())
        last_y = getRaster()->getNumberOfRows();
      if (last_x > getRaster()->getNumberOfColumns())
        last_x = getRaster()->getNumberOfColumns();
      rcFPos.push_back(std::pair<unsigned, unsigned> (last_y, last_x));
    }
  }

// rasters without no data values
  if (getProperty()->m_noDataValue == std::numeric_limits<double>::max())
  {
    for (unsigned i = 0; i < rcStPos.size(); i++)
    {
      for (unsigned r = rcStPos[i].first; r < rcFPos[i].first; r++)
        for (unsigned c = rcStPos[i].second; c < rcFPos[i].second; c++)
        {
          getValue(c, r, lhsv);
          rhs.getValue(c, r, rhsv);

          lhsv = f(lhsv, rhsv);
          setValue(c, r, lhsv);
        }
      }
  }
// rasters with no data values
  else
  {
    for (unsigned i = 0; i < rcStPos.size(); i++)
    {
      for (unsigned r = rcStPos[i].first; r < rcFPos[i].first; r++)
        for (unsigned c = rcStPos[i].second; c < rcFPos[i].second; c++)
        {
// lhs data
          getValue(c, r, lhsv);
          if (lhsv.real() == getProperty()->m_noDataValue)
            continue;

// rhs data
          rhs.getValue(c, r, rhsv);
          if (rhsv.real() == rhs.getProperty()->m_noDataValue)
            continue;

          lhsv = f(lhsv, rhsv);
          setValue(c, r, lhsv);
        }
    }
  }

  return *this;
}

te::rst::Band& te::rst::Band::callOperator(std::complex<double>(*f)(std::complex<double>, std::complex<double>), std::complex<double>& cvalue)
{
  assert(getRaster()->getAccessPolicy() == te::common::RWAccess || getRaster()->getAccessPolicy() == te::common::WAccess);

  std::complex<double> lhsv;

  std::vector<std::pair<unsigned, unsigned> > rcStPos, rcFPos;

  unsigned last_y;
  unsigned last_x;
  for (unsigned x = 0; x < (unsigned) getProperty()->m_nblocksx; x++)
  {
    for (unsigned y = 0; y < (unsigned) getProperty()->m_nblocksy; y++)
    {
      rcStPos.push_back(std::pair<unsigned, unsigned> (y * getProperty()->m_blkh, x * getProperty()->m_blkw));
      last_y = (y + 1) * getProperty()->m_blkh;
      last_x = (x + 1) * getProperty()->m_blkw;
      if (last_y > getRaster()->getNumberOfRows())
        last_y = getRaster()->getNumberOfRows();
      if (last_x > getRaster()->getNumberOfColumns())
        last_x = getRaster()->getNumberOfColumns();
      rcFPos.push_back(std::pair<unsigned, unsigned> (last_y, last_x));
    }
  }

// rasters without no data values
  if (getProperty()->m_noDataValue == std::numeric_limits<double>::max())
  {
    for (unsigned i = 0; i < rcStPos.size(); i++)
    {
      for (unsigned r = rcStPos[i].first; r < rcFPos[i].first; r++)
        for (unsigned c = rcStPos[i].second; c < rcFPos[i].second; c++)
        {
          getValue(c, r, lhsv);

          lhsv = f(lhsv, cvalue);
          setValue(c, r, lhsv);
        }
      }
  }
// rasters with no data values
  else
  {
    for (unsigned i = 0; i < rcStPos.size(); i++)
    {
      for (unsigned r = rcStPos[i].first; r < rcFPos[i].first; r++)
        for (unsigned c = rcStPos[i].second; c < rcFPos[i].second; c++)
        {
// lhs data
          getValue(c, r, lhsv);
          if (lhsv.real() == getProperty()->m_noDataValue)
            continue;

          lhsv = f(lhsv, cvalue);
          setValue(c, r, lhsv);
        }
    }
  }

  return *this;
}

std::complex<double> plus(std::complex<double> lhs, std::complex<double> rhs)
{
  return lhs + rhs;
}

std::complex<double> minus(std::complex<double> lhs, std::complex<double> rhs)
{
  return lhs - rhs;
}

std::complex<double> times(std::complex<double> lhs, std::complex<double> rhs)
{
  return lhs * rhs;
}

std::complex<double> divide(std::complex<double> lhs, std::complex<double> rhs)
{
  return lhs / rhs;
}

te::rst::Band& te::rst::Band::operator+=(te::rst::Band& rhs)
{
  return callOperator(plus, rhs);
}

te::rst::Band& te::rst::Band::operator+=(std::complex<double>& cvalue)
{
  return callOperator(plus, cvalue);
}

te::rst::Band& te::rst::Band::operator-=(te::rst::Band& rhs)
{
  return callOperator(minus, rhs);
}

te::rst::Band& te::rst::Band::operator-=(std::complex<double>& cvalue)
{
  return callOperator(minus, cvalue);
}

te::rst::Band& te::rst::Band::operator*=(std::complex<double>& cvalue)
{
  return callOperator(times, cvalue);
}

te::rst::Band& te::rst::Band::operator*=(te::rst::Band& rhs)
{
  return callOperator(times, rhs);
}

te::rst::Band& te::rst::Band::operator/=(te::rst::Band& rhs)
{
  return callOperator(divide, rhs);
}

te::rst::Band& te::rst::Band::operator/=(std::complex<double>& cvalue)
{
  return callOperator(divide, cvalue);
}

int te::rst::Band::getBlockSize() const
{
  int blkw = m_property->m_blkw;

  int blkh = m_property->m_blkh;

  int pxlsize = GetPixelSize(m_property->getType());

  int blksize = blkw * blkh * pxlsize;

  return blksize;
}

te::rst::Band::Band(const Band& rhs)
  : m_property(0),
    m_idx(rhs.m_idx)
{
  m_property = rhs.m_property ? new BandProperty(*rhs.m_property) : 0;
}

