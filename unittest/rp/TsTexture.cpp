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
  \file TsTexture.cpp

  \brief A test suit for the Texture interface.
 */

// TerraLib
#include "../Config.h"
#include "TsTexture.h"
#include <terralib/raster.h>
#include <terralib/rp/RasterAttributes.h>
#include <terralib/rp/Texture.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TsTexture);

void TsTexture::GLCM()
{
// first open the input image
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// use raster attributes to compute GLCM matrix, in northeast direction
  te::rp::RasterAttributes rattributes;
  boost::numeric::ublas::matrix<double> glcm = rattributes.getGLCM(*rin, 1, 1, -1);
  te::rp::Texture metrics = rattributes.getGLCMMetrics(glcm);

// compare texture metrics    
  double myEpsilon = 1e-4;
  CPPUNIT_ASSERT(std::abs(metrics.m_contrast - 72.554) < myEpsilon);
  CPPUNIT_ASSERT(std::abs(metrics.m_dissimilarity - 5.56127) < myEpsilon);
  CPPUNIT_ASSERT(std::abs(metrics.m_energy - 0.0806309) < myEpsilon);
  CPPUNIT_ASSERT(std::abs(metrics.m_entropy - 6.00814) < myEpsilon);
  CPPUNIT_ASSERT(std::abs(metrics.m_homogeneity - 0.286813) < myEpsilon);

// clean up
  delete rin;
}
