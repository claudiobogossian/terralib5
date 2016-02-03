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
  \file terralib/rp/ClassifierDummyStrategy.cpp

  \brief Dummy strategy (just for testing purposes).
*/

// TerraLib
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../raster/Grid.h"
#include "ClassifierDummyStrategy.h"
#include "Functions.h"
#include "Macros.h"

// STL
#include <iostream>
#include <stdlib.h>

namespace
{
  static te::rp::ClassifierDummyStrategyFactory classifierDummyStrategyFactoryInstance;
}

te::rp::ClassifierDummyStrategy::Parameters::Parameters()
{
  reset();
}

te::rp::ClassifierDummyStrategy::Parameters::~Parameters()
{
}

const te::rp::ClassifierDummyStrategy::Parameters& te::rp::ClassifierDummyStrategy::Parameters::operator=(const te::rp::ClassifierDummyStrategy::Parameters& rhs)
{
  reset();

  m_dummyParameter = rhs.m_dummyParameter;

  return *this;
}

void te::rp::ClassifierDummyStrategy::Parameters::reset() throw(te::rp::Exception)
{
  m_dummyParameter = 0;
}

te::common::AbstractParameters* te::rp::ClassifierDummyStrategy::Parameters::clone() const
{
  return new te::rp::ClassifierDummyStrategy::Parameters( *this );
}

te::rp::ClassifierDummyStrategy::ClassifierDummyStrategy()
{
  m_isInitialized = false;
}

te::rp::ClassifierDummyStrategy::~ClassifierDummyStrategy()
{
}

std::vector< int > te::rp::ClassifierDummyStrategy::getOutputDataTypes() const
{
  std::vector< int > dt;
  dt.push_back( te::dt::UINT32_TYPE );
	return dt;
}

bool te::rp::ClassifierDummyStrategy::initialize(te::rp::ClassifierStrategyParameters const* const strategyParams) throw(te::rp::Exception)
{
  m_isInitialized = false;

  te::rp::ClassifierDummyStrategy::Parameters const* paramsPtr = dynamic_cast<te::rp::ClassifierDummyStrategy::Parameters const*>(strategyParams);

  if(!paramsPtr)
    return false;

  m_parameters = *(paramsPtr);

  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_dummyParameter > 1, TE_TR("The value of dummy must be at least 2."))

  m_isInitialized = true;

  return true;
}

bool te::rp::ClassifierDummyStrategy::execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                                              const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                                              const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception)
{
  TERP_TRUE_OR_RETURN_FALSE(m_isInitialized, TE_TR("Instance not initialized"))

  unsigned int c;
  unsigned int r;
  unsigned int pattern;
  te::gm::Point* point;

// check if OBIA method is employed (raster + polygons)
  if(inputPolygons.size() > 0)
  {
// iterate over geometries
    for (unsigned i = 0; i < inputPolygons.size(); i++)
    {
      te::gm::Polygon* polygon = inputPolygons[i];

      te::gm::Coord2D ll = polygon->getMBR()->getLowerLeft();
      te::gm::Coord2D ur = polygon->getMBR()->getUpperRight();

      te::gm::Coord2D startGridCoord = outputRaster.getGrid()->geoToGrid(ll.x, ur.y);
      te::gm::Coord2D endGridCoord = outputRaster.getGrid()->geoToGrid(ur.x, ll.y);

      pattern = rand() % 5 + 1;

      for(r = (unsigned) startGridCoord.y; r < endGridCoord.y; r++)
        for(c = (unsigned) startGridCoord.x; c < endGridCoord.x; c++)
        {
          te::gm::Coord2D geoCoord = outputRaster.getGrid()->gridToGeo(c, r);
          point = new te::gm::Point(geoCoord.x, geoCoord.y);

          if (polygon->intersects(point))
            outputRaster.setValue(c, r, pattern, outputRasterBand);

          delete point;
        }
    }
  }
  else
  {
    const unsigned int nrows = outputRaster.getNumberOfRows();
    const unsigned int ncols = outputRaster.getNumberOfColumns();

    for(r = 0; r < nrows; r++)
    {
      for(c = 0; c < ncols; c++)
      {
// generate random classification with 5 classes
        pattern = rand() % 5 + 1;

        outputRaster.setValue(c, r, pattern, outputRasterBand);
      }
    }
  }

  return true;
}

te::rp::ClassifierDummyStrategyFactory::ClassifierDummyStrategyFactory()
  : te::rp::ClassifierStrategyFactory("dummy")
{
}

te::rp::ClassifierDummyStrategyFactory::~ClassifierDummyStrategyFactory()
{
}

te::rp::ClassifierStrategy* te::rp::ClassifierDummyStrategyFactory::build()
{
  return new te::rp::ClassifierDummyStrategy();
}
