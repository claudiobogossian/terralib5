/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/plugins/mobile/Utils.cpp

  \brief This file contains a set of utility functions used to export data to a geopackage file
*/

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../gdal/Utils.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../../../raster/Interpolator.h"
#include "../../../../raster/RasterFactory.h"
#include "../../../../rp/Functions.h"
#include "../../../../rp/RasterHandler.h"
#include "../core/form/Serializer.h"
#include "utils.h"

void exportVectortoGPKG(te::map::AbstractLayerPtr layer, te::da::DataSource* dsGPKG, te::da::DataSetType* dataType, std::auto_ptr<te::da::DataSet> dataset, std::string outFileName)
{
  //SRID adaptation
  int inputSRID = layer->getSRID();
  int outputSRID = 4326;

  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dataType, dsGPKG->getCapabilities(), dsGPKG->getEncoding());
  te::da::AssociateDataSetTypeConverterSRID(converter, inputSRID, outputSRID);

  te::da::DataSetType* dsTypeResult = converter->getResult();

  if (dsTypeResult->getProperty("FID"))
    converter->remove("FID");

  dsTypeResult->setName(dataType->getName());

  //exporting
  std::map<std::string, std::string> nopt;

  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), converter));

  if (dataset->moveBeforeFirst())
    te::da::Create(dsGPKG, dsTypeResult, dsAdapter.get());
}

void exportRastertoGPKG(te::map::AbstractLayerPtr layer, te::da::DataSource* dsGPKG, std::auto_ptr<te::da::DataSet> dataset, std::string outFileName, const te::gm::Envelope extent)
{
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());
  std::size_t rpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> raster = dataset->getRaster(rpos);

  bool extValid = extent.isValid();
  int inputSRID = raster->getSRID();
  int bandType = raster->getBandDataType(0);
  int multiResLevel = raster->getMultiResLevelsCount();

  //Adjusting the output raster to tconform with the mobile app needs
  if ((inputSRID != 4326) || (bandType != te::dt::UCHAR_TYPE) || (multiResLevel > 0))
  {
    //Acquiring raster info
    const std::string& id = dsLayer->getDataSourceId();
    te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(id);
    std::map<std::string, std::string>  connInfo = info->getConnInfo();
    std::string uri = connInfo["SOURCE"];

    if (boost::filesystem::is_directory(uri))
      uri += ("/" + dsLayer->getDataSetName());

    std::map<std::string, std::string> rinfo;
    rinfo["SOURCE"] = uri;
    rinfo["FORCE_MEM_DRIVER"] = "TRUE";

    //Changing the raster to allow it to be exported, must be UCHAR_TYPE & the srid must be 4326
    if (bandType != te::dt::UCHAR_TYPE)
      raster = te::gdal::NormalizeRaster(raster.get(), 0, 255, rinfo, "MEM");

    if (inputSRID != 4326 || extValid)
    {
      if (extValid && raster->getExtent()->contains(extent) && (inputSRID != 4326)) //Reprojecting and slicing the desired protion of the inut raster
        raster.reset(raster->transform(4326, extent.getLowerLeftX(), extent.getLowerLeftY(), extent.getUpperRightX(), extent.getUpperRightY(), rinfo));
      else if (extValid && raster->getExtent()->contains(extent)) //Slicing the desired protion of the input raster
        raster.reset(raster->transform(inputSRID, extent.getLowerLeftX(), extent.getLowerLeftY(), extent.getUpperRightX(), extent.getUpperRightY(), rinfo));
      else
        raster.reset(raster->transform(4326, rinfo)); //Simply reprojecting a raster
    }

    //Creating the raster to be exported
    size_t bandIdx = 0;
    size_t bands = raster->getNumberOfBands();

    std::vector< te::rst::BandProperty* > bandsProperties;

    for (bandIdx = 0; bandIdx < bands; ++bandIdx)
    {
      te::rst::BandProperty* bandProp = new te::rst::BandProperty(*(raster->getBand(bandIdx)->getProperty()));
      bandsProperties.push_back(new te::rst::BandProperty(

        *(raster->getBand(bandIdx)->getProperty())));
    }

    //exporting
    boost::filesystem::wpath dir = boost::filesystem::absolute(outFileName);
    boost::filesystem::wpath file = boost::filesystem::absolute(dir.parent_path().string() + "/" + dsLayer->getDataSetName());
    {
      te::rp::RasterHandler outRasterHandler;
      te::rp::CreateNewGdalRaster(*(raster->getGrid()), bandsProperties, file.string(), outRasterHandler);
      te::rst::Raster* outRaster (outRasterHandler.getRasterPtr());

      //Adjusting the result data on the raster to be exported
      size_t col = 0;
      size_t row = 0;
      size_t bandIdx = 0;

      double value = 0;
      const unsigned int inNRows = raster->getNumberOfRows();
      const unsigned int inNCols = raster->getNumberOfColumns();

      for (bandIdx = 0; bandIdx < bandsProperties.size(); ++bandIdx)
      {
        te::rst::Band& band = *raster->getBand(bandIdx);
        const double noDataValue = band.getProperty()->m_noDataValue;

        for (row = 0; row < inNRows; ++row)
        {
          for (col = 0; col < inNCols; ++col)
          {
            try
            {
              band.getValue(col, row, value);
              outRaster->setValue(col, row, value, bandIdx);

            }
            catch (...)
            {
              continue;
            }
          }
        }
      }

      if (multiResLevel > 0)
        outRaster->createMultiResolution(multiResLevel, te::rst::NearestNeighbor);
      
      te::gdal::copyToGeopackage(outRaster, outFileName);
    }
     boost::filesystem::remove(file);
  }
  else
    te::gdal::copyToGeopackage(raster.get(), outFileName);
}

std::auto_ptr<te::da::DataSource> te::qt::plugins::terramobile::createGeopackage(std::string gpkgName)
{
  te::gdal::createGeopackage(gpkgName);

  //create data source
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = gpkgName;

  std::auto_ptr<te::da::DataSource> dsGPKG = te::da::DataSourceFactory::make("OGR");
  dsGPKG->setConnectionInfo(connInfo);
  dsGPKG->open();

  //Creating tables required by the mobile application

  //Layer Settings
  std::string sql1 = "CREATE TABLE IF NOT EXISTS tm_layer_settings";
  sql1 += "(layer_name TEXT PRIMARY KEY NOT NULL, enabled BOOLEAN NOT NULL, position INTEGER NOT NULL UNIQUE,";
  sql1 += "CONSTRAINT fk_layer_name FOREIGN KEY(LAYER_NAME) REFERENCES gpkg_contents(table_name)); ";

  //Layer style
  std::string sql2 = "CREATE TABLE IF NOT EXISTS tm_style(layer_name TEXT PRIMARY KEY NOT NULL, sld_xml TEXT); ";

  //Layer Form
  std::string sql3 = "CREATE TABLE IF NOT EXISTS tm_layer_form ";
  sql3 += "(tm_conf_id INTEGER PRIMARY KEY AUTOINCREMENT, gpkg_layer_identify TEXT NOT NULL, tm_form TEXT, tm_media_table TEXT,";
  sql3 += "CONSTRAINT fk_layer_identify_id FOREIGN KEY (gpkg_layer_identify) REFERENCES gpkg_contents(table_name));";
  
  //queryGPKG(sql1, dsGPKG.get());
  queryGPKG(sql2, dsGPKG.get());
  queryGPKG(sql3, dsGPKG.get());

  return dsGPKG;
}

void te::qt::plugins::terramobile::exportToGPKG(te::map::AbstractLayerPtr layer, te::da::DataSource* dsGPKG, std::string outFileName, const te::gm::Envelope extent)
{
  std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();
  std::auto_ptr<te::da::DataSet> dataset;

  //Checking if the layer contains a raster property
  if (dsType->hasRaster())
  {
    dataset = layer->getData();
    exportRastertoGPKG(layer, dsGPKG, dataset, outFileName, extent);
  }
  else
  {
    if (extent.isValid())
    {
      std::string geomName = te::da::GetFirstGeomProperty(dsType.get())->getName();
      dataset = layer->getData(geomName, &extent);
    }
    else
      dataset = layer->getData();

    exportVectortoGPKG(layer, dsGPKG, dsType.get(), dataset, outFileName);

    std::string name = dsType->getName();
    std::string sldString = te::qt::plugins::terramobile::WriteStyle(layer->getStyle(), (outFileName + "-temp-style.xml"));
    std::string insert = "INSERT INTO tm_style ('layer_name', 'sld_xml' )  values('" + name + "', '" + sldString + "');";

    queryGPKG(insert, dsGPKG);
  }
}

void te::qt::plugins::terramobile::queryGPKG(std::string query, te::da::DataSource* dsGPKG)
{
  std::auto_ptr<te::da::DataSourceTransactor> transactor = dsGPKG->getTransactor();
  try
  {
    transactor->execute(query);
  }
  catch (...)
  {
    transactor->rollBack();
    throw;
  }
}