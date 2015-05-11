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
  \file Module.cpp
   
  \brief This singleton defines the TerraLib Vector Geometry module entry.  
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "../datatype/DataConverterManager.h"
#include "../datatype/Enums.h"
#include "Geometry.h"
#include "GeometryConverters.h"
#include "GEOSGeometryFactory.h"
#include "Module.h"
#include "AffineGTFactory.h"
#include "ProjectiveGTFactory.h"
#include "RSTGTFactory.h"
#include "SecondDegreePolynomialGTFactory.h"
#include "ThirdDegreePolynomialGTFactory.h"

const te::gm::Module& sm_module = te::gm::Module::getInstance();

te::gm::Module::Module()
{
  TerraLib::Module m = { TE_GEOMETRY_MODULE_NAME,
                         te::gm::Module::initialize,
                         te::gm::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::gm::Module::~Module()
{
  TerraLib::getInstance().remove(TE_GEOMETRY_MODULE_NAME);
}

void te::gm::Module::initialize()
{
// it loads the geometry type id map
  te::gm::Geometry::loadGeomTypeId();

#ifdef TERRALIB_GEOS_ENABLED
// it initializes the GEOS Geometry Factory
  te::gm::GEOSGeometryFactory::initialize();
#endif

// registering data type converters
  te::dt::DataConverterManager::getInstance().add(te::dt::GEOMETRY_TYPE, te::dt::BYTE_ARRAY_TYPE, GeometryToByteArrayConverter);
  te::dt::DataConverterManager::getInstance().add(te::dt::GEOMETRY_TYPE, te::dt::STRING_TYPE, GeometryToStringConverter);
  te::dt::DataConverterManager::getInstance().add(te::dt::BYTE_ARRAY_TYPE, te::dt::GEOMETRY_TYPE, ByteArrayToGeometryConverter);
  te::dt::DataConverterManager::getInstance().add(te::dt::STRING_TYPE, te::dt::GEOMETRY_TYPE, StringToGeometryConverter);
  
  te::gm::AffineGTFactory::initialize();
  te::gm::ProjectiveGTFactory::initialize();
  te::gm::RSTGTFactory::initialize();
  te::gm::SecondDegreePolynomialGTFactory::initialize();
  te::gm::ThirdDegreePolynomialGTFactory::initialize();

  TE_LOG_TRACE(TE_TR("TerraLib Vector Geometry module Initialized!"));
}

void te::gm::Module::finalize()
{
#ifdef TERRALIB_GEOS_ENABLED
// it finalizes the GEOS Geometry Factory
  te::gm::GEOSGeometryFactory::finalize();
#endif

  te::gm::AffineGTFactory::finalize();
  te::gm::ProjectiveGTFactory::finalize();
  te::gm::RSTGTFactory::finalize();
  te::gm::SecondDegreePolynomialGTFactory::finalize();
  te::gm::ThirdDegreePolynomialGTFactory::finalize();

  TE_LOG_TRACE(TE_TR("TerraLib Vector Geometry module Finalized!"));
}


