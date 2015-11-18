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
  \file examples/cellspace/main.cpp

  \brief A examples for the TerraLib Cellular Space Module.
*/

// TerraLib
#include <terralib/cellspace/CellSpaceOperations.h>
#include <terralib/common/TerraLib.h>
#include <terralib/plugin.h>
#include <terralib/dataaccess/dataset/CheckConstraint.h>
#include <terralib/dataaccess/dataset/PrimaryKey.h>
#include <terralib/dataaccess/dataset/Index.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceInfo.h>
#include <terralib/dataaccess/datasource/DataSourceInfoManager.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/geometry/Envelope.h>

// Examples

// STL
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

void LoadModules();

int main(int /*argc*/, char** /*argv*/)
{
  try
  {
    // Initialize the Terralib support
    TerraLib::getInstance().initialize();

    LoadModules();

    std::map<std::string, std::string> ogrInfo;
    ogrInfo["SOURCE"] = "C://CellspaceExample//brazil_cells.shp";

    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id_ds = boost::uuids::to_string(u);

    te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
    ds->setConnInfo(ogrInfo);
    ds->setTitle("brazil_cells");
    ds->setAccessDriver("OGR");
    ds->setType("OGR");
    ds->setId(id_ds);

    te::gm::Envelope env;
    env.m_llx = -73.99;
    env.m_lly = -33.75;
    env.m_llx = -34.79;
    env.m_llx = 5.27;

    int srid = 4618; // LatLong SAD64

    te::cellspace::CellularSpacesOperations cp;
    cp.createCellSpace(ds, "brazil_cells", 0.25, 0.25, env, srid, te::cellspace::CellularSpacesOperations::CELLSPACE_POLYGONS, 0); // without mask

    te::plugin::PluginManager::getInstance().unloadAll();

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred!" << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}