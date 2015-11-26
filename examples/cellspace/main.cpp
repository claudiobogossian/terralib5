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
#include "../Config.h"
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
#include <terralib/dataaccess/utils/Utils.h>
#include <terralib/geometry/Envelope.h>
#include <terralib/geometry/GeometryProperty.h>
#include <terralib/maptools/DataSetLayer.h>

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

void CreateCellSpace();
void CreateMaskCellSpace();

int main(int /*argc*/, char** /*argv*/)
{
  try
  {
    // Initialize the Terralib support
    TerraLib::getInstance().initialize();

    LoadModules();

    /*
    Before running, make sure that the TERRALIB_DATA_DIR variable, of CMAKE, was set correctly with the 
    address of the folder "data" provided by the link: http://www.dpi.inpe.br/terralib5-devel/data.zip
    */

    try
    {

      CreateCellSpace();
      CreateMaskCellSpace();

    }
    catch (const std::exception& e)
    {
      std::cout << "Error: " << e.what() << std::endl;
    }
    catch (...)
    {
      std::cout << "Unexpected error" << std::endl;
    }

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

void CreateCellSpace()
{
  std::map<std::string, std::string> ogrInfo;
  ogrInfo["URI"] = "C://CellspaceExample//brazil_cells.shp";

  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id_ds = boost::uuids::to_string(u);

  te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
  ds->setConnInfo(ogrInfo);
  ds->setAccessDriver("OGR");
  ds->setId(id_ds);

  te::gm::Envelope env;
  env.m_llx = -73.99;
  env.m_lly = -33.75;
  env.m_urx = -33.75;
  env.m_ury = 5.27;

  int srid = 4618; // LatLong SAD64

  te::cellspace::CellularSpacesOperations cp;
  cp.createCellSpace(ds, "brazil_cells", 0.25, 0.25, env, srid, te::cellspace::CellularSpacesOperations::CELLSPACE_POLYGONS, 0); // without mask
}

void CreateMaskCellSpace()
{
  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();

  std::string id_source = boost::uuids::to_string(u);

  std::string data_dir = TERRALIB_DATA_DIR;

  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = data_dir + "/shp/munic_2001.shp";

  te::da::DataSourcePtr source( te::da::DataSourceFactory::make("OGR") );
  source->setConnectionInfo(connInfo);
  source->open();
  source->setId(id_source);

  te::da::DataSourceManager::getInstance().insert(source);

  if (source.get())
  {
    std::auto_ptr<te::da::DataSetType> dsType = source->getDataSetType("munic_2001");
    std::auto_ptr<te::da::DataSet> dataset = source->getDataSet("munic_2001");

    std::auto_ptr<te::gm::GeometryProperty> gp(te::da::GetFirstGeomProperty(dsType.get()));

    u = gen();
    std::string id_layer = boost::uuids::to_string(u);

    std::auto_ptr<te::gm::Envelope> mbr(te::da::GetExtent(dsType->getName(), gp->getName(), id_source));

    std::auto_ptr<te::map::DataSetLayer> layer(new te::map::DataSetLayer(id_layer));
    layer->setDataSetName(dsType->getName());
    layer->setDataSourceId(id_source);
    layer->setVisibility(te::map::NOT_VISIBLE);
    layer->setRendererType("ABSTRACT_LAYER_RENDERER");
    layer->setSRID(gp->getSRID());

    if (mbr.get())
      layer->setExtent(*mbr);

    std::map<std::string, std::string> ogrInfo;
    ogrInfo["URI"] = "C://CellspaceExample//brazil_cells_mask.shp";

    std::string id_source = boost::uuids::to_string(u);

    te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
    ds->setConnInfo(ogrInfo);
    ds->setAccessDriver("OGR");
    ds->setId(id_source);

    te::cellspace::CellularSpacesOperations cp;
    cp.createCellSpace(ds, "brazil_cells_mask", 1, 1, *mbr, gp->getSRID(), te::cellspace::CellularSpacesOperations::CELLSPACE_POLYGONS, layer.get()); // without mask
  }
}