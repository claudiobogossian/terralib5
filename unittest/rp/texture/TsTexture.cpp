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
  \file terralib/unittest/rp/texture/TsTexture.cpp

  \brief A test suit for Texture interface.
*/

// TerraLib
#include "../Config.h"
#include <terralib/rp.h>
#include <terralib/raster.h>
#include <terralib/rp/RasterAttributes.h>
#include <terralib/rp/Texture.h>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE (texture_tests)

BOOST_AUTO_TEST_CASE(GLCM_test)
{
  /* First open the input image */

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

  // Retrieve the minimum and maximum values of the band to normalize GLCM
  double maxPixel, minPixel;
  te::rst::GetDataTypeRanges(rin->getBandDataType(1), minPixel, maxPixel);
  if ((maxPixel - minPixel) > 255) {
      maxPixel = rin->getBand(1)->getMaxValue(true).real();
      minPixel = rin->getBand(1)->getMinValue(true).real();
  }

  /* Use raster attributes to compute GLCM matrix, in northeast direction */

  te::rp::RasterAttributes rattributes;
  boost::numeric::ublas::matrix<double> glcm = rattributes.getGLCM(*rin, 1, 1, -1, minPixel, maxPixel);
  te::rp::Texture metrics = rattributes.getGLCMMetrics(glcm);

  /* Compare texture metrics */

  double myEpsilon = 1e-4;
  BOOST_CHECK(std::abs(metrics.m_contrast - 72.554) < myEpsilon);
  BOOST_CHECK(std::abs(metrics.m_dissimilarity - 5.56127) < myEpsilon);
  BOOST_CHECK(std::abs(metrics.m_energy - 0.0806309) < myEpsilon);
  BOOST_CHECK(std::abs(metrics.m_entropy - 6.00814) < myEpsilon);
  BOOST_CHECK(std::abs(metrics.m_homogeneity - 0.286813) < myEpsilon);

  /* Clean up */
  delete rin;
}

BOOST_AUTO_TEST_SUITE_END()
