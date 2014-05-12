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
  \file terralib/rp/ClassifierISOSegStrategy.cpp

  \brief Raster ISOSeg strategy for segmentation-based classification.
*/

// TerraLib
#include "../common/MatrixUtils.h"
#include "../common/progress/TaskProgress.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../raster/Grid.h"
#include "../raster/PositionIterator.h"
#include "../raster/Utils.h"
#include "ClassifierISOSegStrategy.h"
#include "Functions.h"
#include "Macros.h"
#include "RasterAttributes.h"

// STL
#include <iostream>
#include <set>
#include <stdlib.h>

// Boost
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>

// STL
#include <cfloat>

namespace
{
  static te::rp::ClassifierISOSegStrategyFactory classifierISOSegStrategyFactoryInstance;
}

te::rp::ClassifierISOSegStrategy::Parameters::Parameters()
{
  reset();
}

te::rp::ClassifierISOSegStrategy::Parameters::~Parameters()
{
}

const te::rp::ClassifierISOSegStrategy::Parameters& te::rp::ClassifierISOSegStrategy::Parameters::operator=(const te::rp::ClassifierISOSegStrategy::Parameters& rhs)
{
  reset();

  m_acceptanceThreshold = rhs.m_acceptanceThreshold;

  return *this;
}

void te::rp::ClassifierISOSegStrategy::Parameters::reset() throw(te::rp::Exception)
{
  m_acceptanceThreshold = 0;
}

te::common::AbstractParameters* te::rp::ClassifierISOSegStrategy::Parameters::clone() const
{
  return new te::rp::ClassifierISOSegStrategy::Parameters(*this);
}

te::rp::ClassifierISOSegStrategy::Pattern::Pattern()
{
  m_id = -1;

  m_myCluster = 0;

  m_area = 0.0;
}

te::rp::ClassifierISOSegStrategy::Pattern::Pattern(int i, double a, std::vector<double> mv, boost::numeric::ublas::matrix<double> cm)
  : m_id(i),
    m_myCluster(0),
    m_area(a),
    m_meanVector(mv),
    m_covarianceMatrix(cm)
{
  m_covarianceInversion = boost::numeric::ublas::matrix<double>(m_covarianceMatrix.size1(), m_covarianceMatrix.size2());

  te::common::GetInverseMatrix(m_covarianceMatrix, m_covarianceInversion);
}

te::rp::ClassifierISOSegStrategy::Pattern::Pattern(Pattern& rhs)
  : m_id(rhs.m_id),
    m_myCluster(rhs.m_myCluster),
    m_area(rhs.m_area)
{
  m_meanVector = rhs.m_meanVector;

  m_covarianceMatrix = rhs.m_covarianceMatrix;

  m_covarianceInversion = rhs.m_covarianceInversion;
}

te::rp::ClassifierISOSegStrategy::Pattern::~Pattern()
{
  m_id = -1;

  m_myCluster = 0;

  m_area = 0.0;

  m_meanVector.clear();

  m_covarianceMatrix.clear();

  m_covarianceInversion.clear();
}

void te::rp::ClassifierISOSegStrategy::Pattern::add(Pattern* p)
{
  assert(m_meanVector.size() == p->m_meanVector.size());
  assert(p->m_area > 0.0);
  assert(m_myCluster == 0);

// update mean vectors
  double total_area = m_area + p->m_area;
  for (unsigned i = 0; i < m_meanVector.size(); i++)
    m_meanVector[i] = (m_meanVector[i] * m_area + p->m_meanVector[i] * p->m_area) / total_area;

// update covariance matrices (the matrix with the biggest area wins)
  if (p->m_area > m_area)
  {
    m_covarianceMatrix = p->m_covarianceMatrix;

    m_covarianceInversion = p->m_covarianceInversion;
  }

// add the new area
  m_area += p->m_area;

// update the pattern values
  p->m_myCluster = this;
}

double te::rp::ClassifierISOSegStrategy::Pattern::getDistance(Pattern* p)
{
  assert(m_meanVector.size() == p->m_meanVector.size());

  unsigned int nBands = m_meanVector.size();

  boost::numeric::ublas::matrix<double> term1(1, nBands);
  boost::numeric::ublas::matrix<double> term2(nBands, 1);

  for (unsigned int i = 0; i < nBands; i++)
  {
    term1(0, i) = m_meanVector[i] - p->m_meanVector[i];

    term2(i, 0) = term1(0, i);
  }

  term1 = prod(term1, m_covarianceInversion);
  term1 = prod(term1, term2);

  if (term1(0, 0) < 0)
    return DBL_MAX;

  return term1(0, 0);
}

bool te::rp::ClassifierISOSegStrategy::Pattern::operator=(Pattern& rhs)
{
  assert(m_myCluster == 0);
  assert(rhs.m_myCluster == 0);

  return m_id == rhs.m_id;
}

te::rp::ClassifierISOSegStrategy::ClassifierISOSegStrategy()
{
  m_isInitialized = false;
}

te::rp::ClassifierISOSegStrategy::~ClassifierISOSegStrategy()
{
}

bool te::rp::ClassifierISOSegStrategy::initialize(te::rp::StrategyParameters const* const strategyParams) throw(te::rp::Exception)
{
  m_isInitialized = false;

  te::rp::ClassifierISOSegStrategy::Parameters const* paramsPtr = dynamic_cast<te::rp::ClassifierISOSegStrategy::Parameters const*>(strategyParams);

  if(!paramsPtr)
    return false;

  m_parameters = *(paramsPtr);

  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_acceptanceThreshold > 0 && m_parameters.m_acceptanceThreshold <= 100, "Invalid acceptance threshold [0, 100].")

  m_isInitialized = true;

  return true;
}

double getThreshold(double acceptanceThreshold, unsigned nBands)
{
  const double dmax = std::numeric_limits<double>::max();
  double chiTable[10][6] =
  {
//   75.0%    90.0%    95.0%    99.0%    99.9%    100.%
    { 1.32,    2.71,    3.84,    6.64,   10.83,   dmax},
    { 2.77,    4.61,    5.99,    9.21,   13.82,   dmax},
    { 4.11,    6.25,    7.82,   11.35,   16.27,   dmax},
    { 5.39,    7.78,    9.49,   13.28,   18.47,   dmax},
    { 6.63,    9.24,   11.07,   15.09,   20.52,   dmax},
    { 7.84,   10.65,   12.59,   16.81,   22.46,   dmax},
    { 9.04,   12.02,   14.07,   18.48,   24.32,   dmax},
    {10.22,   13.36,   15.51,   20.09,   26.13,   dmax},
    {11.39,   14.68,   16.92,   21.67,   27.88,   dmax},
    {12.55,   15.99,   18.31,   23.21,   29.59,   dmax}
  };

  unsigned it;
  if (nBands > 6)
    nBands = 6;

  if(acceptanceThreshold < 90.0)
    it = 0;
  else if(acceptanceThreshold < 95.0)
    it = 1;
  else if(acceptanceThreshold < 99.0)
    it = 2;
  else if(acceptanceThreshold < 99.9)
    it = 3;
  else if(acceptanceThreshold < 100.0)
    it = 4;
  else
    it = 5;

  return chiTable[nBands-1][it];
}

bool te::rp::ClassifierISOSegStrategy::execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                                               const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                                               const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception)
{
  TERP_TRUE_OR_RETURN_FALSE(m_isInitialized, "Instance not initialized")
  TERP_TRUE_OR_RETURN_FALSE(inputPolygons.size() > 0, "ISOSeg algorithm needs polygons")

  //double mean;
  te::rp::RasterAttributes rattributes;

// fill m_regions, in the beginning, each region is a cluster
  te::common::TaskProgress task_fx(TE_TR("ISOSeg algorithm - feature extraction"), te::common::TaskProgress::UNDEFINED, inputPolygons.size());
  for (unsigned i = 0; i < inputPolygons.size(); i++)
  {
    te::gm::Polygon* polygon = inputPolygons[i];

    std::vector<std::vector<double> > values_in_polygon = rattributes.getValuesFromRaster(inputRaster, *polygon, inputRasterBands);
    std::vector<double> means;
    for (unsigned int b = 0; b < values_in_polygon.size(); b++)
    {
      te::stat::NumericStatisticalSummary summary = rattributes.getStatistics(values_in_polygon[b]);
      means.push_back(summary.m_mean);
    }

    Pattern* region = new Pattern(i, polygon->getArea(), means, rattributes.getCovarianceMatrix(values_in_polygon, means));

    Pattern* cluster = new Pattern(*region);

    region->m_myCluster = cluster;

    m_regions.insert(std::pair<double, Pattern*> (region->m_area, region));

    task_fx.pulse();
  }

  double distance;
  double distance2;

// loops until all the regions are classified
  std::multimap<double, Pattern*, std::greater<double> >::iterator rit;
  std::multimap<double, Pattern*, std::greater<double> >::iterator riti;
  std::multimap<double, Pattern*, std::greater<double> >::iterator ritii;

// merge similar clusters
  bool stable = false;
  int oldid;
  std::set<std::pair<unsigned int, unsigned int> > compared;

  te::common::TaskProgress task_clustering(TE_TR("ISOSeg algorithm - detecting clusters"));
  double maxDistance = getThreshold(m_parameters.m_acceptanceThreshold, inputRasterBands.size());
  while (!stable)
  {
    stable = true;

    compared.clear();

// analyze all regions
    for (rit = m_regions.begin(); rit != m_regions.end(); ++rit)
    {
// compare all clusters to detect if they are still similar
      for (riti = rit; riti != m_regions.end(); ++riti)
      {
        if (compared.find(std::pair<unsigned int, unsigned int>(rit->second->m_myCluster->m_id, riti->second->m_myCluster->m_id)) == compared.end())
        {
          compared.insert(std::pair<unsigned int, unsigned int>(rit->second->m_myCluster->m_id, riti->second->m_myCluster->m_id));
          compared.insert(std::pair<unsigned int, unsigned int>(riti->second->m_myCluster->m_id, rit->second->m_myCluster->m_id));
        }
        else
          continue;

        if (rit->second->m_myCluster == riti->second->m_myCluster)
          continue;

// use the smallest distance between region1 <-> cluster2, and region2 <-> cluster1
        distance = rit->second->m_myCluster->getDistance(riti->second->m_myCluster);

        distance2 = riti->second->m_myCluster->getDistance(rit->second->m_myCluster);

        distance = (distance2 < distance? distance2: distance);

        if (distance <= maxDistance)
        {
// two clusters are similar and must be merged
          stable = false;

          oldid = riti->second->m_myCluster->m_id;

          for (ritii = riti; ritii != m_regions.end(); ++ritii)
          {
// find all clusters with riti class and merge with rit cluster
            if (ritii->second->m_myCluster->m_id != oldid)
              continue;

            ritii->second->m_myCluster = rit->second->m_myCluster;
          }
        }
      }
    }
    task_clustering.pulse();
  }

// remap cluster values to 1 -> N
  std::set<unsigned int> ulabels;
  std::set<unsigned int>::iterator lit;
  std::map<unsigned int, unsigned int> colormap;

  for (rit = m_regions.begin(); rit != m_regions.end(); ++rit)
    ulabels.insert(rit->second->m_myCluster->m_id);

  unsigned int color = 1;
  for (lit = ulabels.begin(); lit != ulabels.end(); ++lit)
    colormap[*lit] = color++;

  te::gm::Coord2D geoCoord;
  unsigned int pattern;

// classify output image
  te::common::TaskProgress task(TE_TR("ISOSeg algorithm - classifying image"), te::common::TaskProgress::UNDEFINED, m_regions.size());
  for (rit = m_regions.begin(); rit != m_regions.end(); ++rit)
  {
    te::gm::Polygon* polygon = inputPolygons[rit->second->m_id];

    pattern = rit->second->m_myCluster->m_id;

// iterate over polygon to classify output image
    te::rst::PolygonIterator<unsigned> it = te::rst::PolygonIterator<unsigned>::begin(&inputRaster, polygon);
    te::rst::PolygonIterator<unsigned> itend = te::rst::PolygonIterator<unsigned>::end(&inputRaster, polygon);

    while (it != itend)
    {
      outputRaster.setValue(it.getColumn(), it.getRow(), colormap[pattern], outputRasterBand);

      ++it;
    }
    task.pulse();
  }

  return true;
}

te::rp::ClassifierISOSegStrategyFactory::ClassifierISOSegStrategyFactory()
  : te::rp::ClassifierStrategyFactory("isoseg")
{
}

te::rp::ClassifierISOSegStrategyFactory::~ClassifierISOSegStrategyFactory()
{
}

te::rp::ClassifierStrategy* te::rp::ClassifierISOSegStrategyFactory::build()
{
  return new te::rp::ClassifierISOSegStrategy();
}
