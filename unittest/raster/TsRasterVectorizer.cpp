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
  \file TsRaterVectorizer.cpp

  \brief A test suit for the Raster Vectorizer class.
 */

// TerraLib
#include "../Config.h"
#include "TsRasterVectorizer.h"
#include <terralib/geometry.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/Vectorizer.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TsRasterVectorizer);

void TsRasterVectorizer::tcRasterVectorizer()
{
// define raster info and load
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TE_DATA_DIR"/data/rasters/pattern1.tif";
  te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

  std::vector<te::gm::Geometry*> polygons;
  inraster->vectorize(polygons, 0);

  std::cout << "vectorizer created " << polygons.size() << " polygons" << std::endl;
  for (unsigned int i = 0; i < polygons.size(); i++)
    std::cout << "  polygon " << i << ": " << polygons[i]->toString() << std::endl;

// clean up
  delete inraster;
  polygons.clear();
}
