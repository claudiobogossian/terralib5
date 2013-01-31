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
  \file terralib/gdal/Utils.cpp

  \brief Utility functions for GDAL.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../common/UnitOfMeasure.h"
#include "../common/UnitsOfMeasureManager.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../ogr/Utils.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterFactory.h"
#include "../raster/RasterProperty.h"
#include "Band.h"
#include "Exception.h"
#include "Raster.h"
#include "Utils.h"

// STL
#include <cmath>
#include <vector>

// GDAL
#include <gdalwarper.h>
#include <ogr_api.h>
#include <ogr_spatialref.h>
#include <ogrsf_frmts.h>

// Boost
#include <boost/format.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

bool te::gdal::IsGDALDataSet(const std::string strAccessInfo)
{
  GDALDataset* gds = (GDALDataset*) GDALOpen(strAccessInfo.c_str(), GA_ReadOnly);
  if (gds)
  {
    GDALClose(gds); 
    return true;
  }
  return false;
}

te::rst::Grid* te::gdal::GetGrid(GDALDataset* gds)
{
  if (!gds)
    return 0;

  int srid = -1;
  char* projWKT = (char*)gds->GetProjectionRef();
  if (projWKT)
  {
    char** projWKTPtr = &(projWKT);
    OGRSpatialReference oSRS;
    oSRS.importFromWkt( projWKTPtr );
    oSRS.AutoIdentifyEPSG();
    const char* srsAuth = oSRS.GetAuthorityCode(0);
    if (srsAuth)
      srid = atoi(srsAuth);
  }

  double gtp[6];
  gds->GetGeoTransform(gtp);
  te::rst::Grid* grid = new te::rst::Grid(gds->GetRasterXSize(),gds->GetRasterYSize());
  grid->setGeoreference(gtp,srid);
  return grid;
}

void te::gdal::GetBandProperties(GDALDataset* gds, std::vector<te::rst::BandProperty*>& bprops)
{
  if (!gds)
    return;

  bprops.clear();

  // retrieve the information about each band
  for (int rasterIdx = 0; rasterIdx < gds->GetRasterCount(); ++rasterIdx)
  {
    GDALRasterBand* rasterBand = gds->GetRasterBand(1 + rasterIdx);
    int dt = te::gdal::GetTeDataType(rasterBand->GetRasterDataType());

    te::rst::BandProperty* rb = new te::rst::BandProperty(rasterIdx, dt);
    rb->m_colorInterp = te::gdal::GetTeColorInterpretation(rasterBand->GetColorInterpretation());

    // find if there is a no data value
    int noDataValueIsUsed = 0;
    double nodataval = rasterBand->GetNoDataValue(&noDataValueIsUsed);
    if (noDataValueIsUsed) 
      rb->m_noDataValue = nodataval;

    // pallete information (if present)
    if (rb->m_colorInterp == te::rst::PaletteIdxCInt)
    {
      const GDALColorTable* colorTable = rasterBand->GetColorTable();
      if (!colorTable)
        throw Exception(TR_GDAL("invalid color table"));

      rb->m_paletteInterp = te::gdal::GetTePaletteInterpretation(colorTable->GetPaletteInterpretation());

      const int colorTableCount = colorTable->GetColorEntryCount();
      for (int cTIdx = 0 ; cTIdx < colorTableCount ; ++cTIdx)
      {
        const GDALColorEntry& colorEntry = *colorTable->GetColorEntry(cTIdx);

        te::rst::BandProperty::ColorEntry tcolorEntry;
        tcolorEntry.c1 = colorEntry.c1;
        tcolorEntry.c2 = colorEntry.c2;
        tcolorEntry.c3 = colorEntry.c3;
        tcolorEntry.c4 = colorEntry.c4;

        rb->m_palette.push_back(tcolorEntry);
      }
    }
    
    std::string unitName = rasterBand->GetUnitType();
    if (!unitName.empty())
      rb->setUnitOfMeasure(te::common::UnitsOfMeasureManager::getInstance().findByName(unitName));
    else
      rb->setUnitOfMeasure(0);
    rb->m_valuesOffset = rasterBand->GetOffset(0);
    rb->m_valuesScale = rasterBand->GetScale(0);

    bprops.push_back(rb);
  }
}

te::rst::BandProperty* te::gdal::GetBandProperty(GDALRasterBand* gband)
{
  if (!gband)
    return 0;

  te::rst::BandProperty* bprop = new te::rst::BandProperty(gband->GetBand()-1, GetTeDataType(gband->GetRasterDataType()), "");

  // te::rst::BandProperty* rb = new te::rst::BandProperty(rasterIdx, dt);
  bprop->m_colorInterp = GetTeColorInterpretation(gband->GetColorInterpretation());

  // find if there is a no data value
  int noDataValueIsUsed = 0;
  double nodataval = gband->GetNoDataValue(&noDataValueIsUsed);
  if (noDataValueIsUsed) 
    bprop->m_noDataValue = nodataval;

  // pallete information (if present)
  if (bprop->m_colorInterp == te::rst::PaletteIdxCInt)
  {
    const GDALColorTable* colorTable = gband->GetColorTable();
    if (!colorTable)
      throw Exception(TR_GDAL("invalid color table"));

    bprop->m_paletteInterp = te::gdal::GetTePaletteInterpretation(colorTable->GetPaletteInterpretation());

    const int colorTableCount = colorTable->GetColorEntryCount();
    for (int cTIdx = 0 ; cTIdx < colorTableCount ; ++cTIdx)
    {
      const GDALColorEntry& colorEntry = *colorTable->GetColorEntry(cTIdx);

      te::rst::BandProperty::ColorEntry tcolorEntry;
      tcolorEntry.c1 = colorEntry.c1;
      tcolorEntry.c2 = colorEntry.c2;
      tcolorEntry.c3 = colorEntry.c3;
      tcolorEntry.c4 = colorEntry.c4;

      bprop->m_palette.push_back(tcolorEntry);
    }
  }

  std::string unitName = gband->GetUnitType();
  if (!unitName.empty())
    bprop->setUnitOfMeasure(te::common::UnitsOfMeasureManager::getInstance().findByName(unitName));
  else
    bprop->setUnitOfMeasure(0);
  bprop->m_valuesOffset = gband->GetOffset(0);
  bprop->m_valuesScale = gband->GetScale(0);

  gband->GetBlockSize(&bprop->m_blkw, &bprop->m_blkh);
  bprop->m_nblocksx = (gband->GetXSize() + bprop->m_blkw - 1) / bprop->m_blkw;
  bprop->m_nblocksy = (gband->GetYSize() + bprop->m_blkh - 1) / bprop->m_blkh;

  return bprop;
}

void te::gdal::GetBands(te::gdal::Raster* rst, std::vector<te::gdal::Band*>& bands)
{
  bands.clear();

  for (int b = 0; b < rst->getGDALDataset()->GetRasterCount(); b++)
    bands.push_back( new te::gdal::Band(rst, b) );
}

te::rst::RasterProperty* te::gdal::GetRasterProperty(std::string strAccessInfo)
{
  GDALDataset* gds = (GDALDataset*) GDALOpen(strAccessInfo.c_str(), GA_ReadOnly);
  if (!gds)
    return 0;

  te::rst::Grid* grid = GetGrid(gds);

  std::vector<te::rst::BandProperty*> bprops;

  te::gdal::GetBandProperties(gds, bprops);

  std::map<std::string, std::string> rinfo;

  te::rst::RasterProperty* rp = new te::rst::RasterProperty(grid, bprops, rinfo);

  GDALClose(gds);

  return rp;
}

te::gm::Envelope* te::gdal::GetExtent(std::string strAccessInfo)
{
  GDALDataset* gds = (GDALDataset*) GDALOpen(strAccessInfo.c_str(), GA_ReadOnly);
  if (!gds)
    return 0;
  te::rst::Grid* grid = te::gdal::GetGrid(gds);
  GDALClose(gds);
  te::gm::Envelope* ext = new te::gm::Envelope(*grid->getExtent());
  delete grid;
  return ext;
}

GDALDataset* te::gdal::CreateRaster(te::rst::Grid* g,
                                    const std::vector<te::rst::BandProperty*>& bands,
                                    const std::map<std::string, std::string>& optParams)
{
  assert(g);
  assert(bands.size() > 0);

  std::string accessInfo = GetGDALConnectionInfo(optParams);

  std::string driverName = GetDriverName(accessInfo);

  GDALDriverManager* dManPtr = GetGDALDriverManager();

  GDALDriver* driverPtr = dManPtr->GetDriverByName(driverName.c_str());

  if(!driverPtr)
    throw Exception("Could not create raster because the underlying driver was not found!");

  GDALDataType targetGDataType = te::gdal::GetGDALDataType(bands[0]->m_type);

  if (targetGDataType == GDT_Unknown)
    throw Exception("Could not create raster because of unknown band data type!");

  char** papszOptions = 0;

  std::map<std::string, std::string>::const_iterator it = optParams.begin();

  std::map<std::string, std::string>::const_iterator it_end = optParams.end();

  while(it != it_end)
  {
    if(it->first == "URI" || it->first == "SOURCE")
    {
      ++it;

      continue;
    }

    papszOptions = CSLSetNameValue(papszOptions, it->first.c_str(), it->second.c_str());

    ++it;
  }

  GDALDataset* poDataset;

  if (driverName == "JPEG" || driverName == "PNG")
  {
    GDALDriver* tmpDriverPtr = dManPtr->GetDriverByName("MEM");

    poDataset = tmpDriverPtr->Create(accessInfo.c_str(),
                                     g->getNumberOfColumns(),
                                     g->getNumberOfRows(),
                                     bands.size(),
                                     targetGDataType,
                                     papszOptions);
  }
  else
    poDataset = driverPtr->Create(accessInfo.c_str(),
                                  g->getNumberOfColumns(),
                                  g->getNumberOfRows(),
                                  bands.size(),
                                  targetGDataType,
                                  papszOptions);

  if(papszOptions)
    CSLDestroy(papszOptions);

  if(poDataset == 0)
    throw Exception("Could not create raster!");

  const double* cgt = g->getGeoreference();

  double gt[6];

  gt[0] = cgt[0]; gt[1] = cgt[1]; gt[2] = cgt[2]; gt[3] = cgt[3]; gt[4] = cgt[4]; gt[5] = cgt[5];

  poDataset->SetGeoTransform(gt);

  OGRSpatialReference oSRS;

  oSRS.importFromEPSG(g->getSRID());

  char* projWKTPtr = 0;

  if(oSRS.exportToWkt(&projWKTPtr) == OGRERR_NONE)
  {
    poDataset->SetProjection(projWKTPtr);
    OGRFree(projWKTPtr);
  }

  int nb = static_cast<int>(bands.size());

  for(int dIdx = 0; dIdx < nb; ++dIdx)
  {
    GDALRasterBand* rasterBand = poDataset->GetRasterBand(1 + dIdx);

    GDALColorInterp ci = te::gdal::GetGDALColorInterpretation(bands[dIdx]->m_colorInterp);

    rasterBand->SetColorInterpretation(ci);

    if (ci == GCI_PaletteIndex)
    {
       GDALColorTable* gCTablePtr = new GDALColorTable(GPI_RGB);
       GDALColorEntry gCEntry;

       for (unsigned int pIdx=0 ; pIdx < bands[dIdx]->m_palette.size(); ++pIdx)
       {
          gCEntry.c1 = (short)bands[dIdx]->m_palette[pIdx].c1;
          gCEntry.c2 = (short)bands[dIdx]->m_palette[pIdx].c2;
          gCEntry.c3 = (short)bands[dIdx]->m_palette[pIdx].c3;
          gCEntry.c4 = (short)bands[dIdx]->m_palette[pIdx].c4;
          gCTablePtr->SetColorEntry(pIdx, &gCEntry);
       }

       rasterBand->SetColorTable(gCTablePtr);
       delete gCTablePtr;
    }
    rasterBand->SetNoDataValue(bands[dIdx]->m_noDataValue);
    rasterBand->SetOffset(bands[dIdx]->m_valuesOffset.real());
    rasterBand->SetScale(bands[dIdx]->m_valuesScale.real());

    // maybe there is something else here...
  }

  return poDataset;
}

GDALDataset* te::gdal::GetRasterHandle(const std::string strAccessInfo, te::common::AccessPolicy policy)
{
  GDALAccess gpolicy = GA_ReadOnly;

  if(policy == te::common::WAccess || policy == te::common::RWAccess)
    gpolicy = GA_Update;

  GDALDataset* gds = (GDALDataset*) GDALOpen(strAccessInfo.c_str(), gpolicy);

  return gds;
}

std::string te::gdal::MakePGConnectionStr(const std::map<std::string, std::string>& dsInfo)
{
/*PG":host='<host>' port:'<port>' dbname='<dbname>' user='<user>' password='<password>' [schema='<schema>'] [table='<raster_table>'] [where='<sql_where>'] [mode='<working_mode>']"*/

  std::map<std::string, std::string>::const_iterator it = dsInfo.find("host");
  std::map<std::string, std::string>::const_iterator it_end = dsInfo.end();
  std::string connInfo = "PG:";

  if(it != it_end)
    connInfo += " host = " + it->second;

  it = dsInfo.find("port");

  if(it != it_end)
    connInfo += " port = " + it->second;

  it = dsInfo.find("dbname");

  if(it != it_end)
    connInfo += " dbname = " + it->second;

  it = dsInfo.find("user");

  if(it != it_end)
    connInfo += " user = " + it->second;

  it = dsInfo.find("password");

  if(it != it_end)
    connInfo += " password = " + it->second;

  it = dsInfo.find("schema");

  if(it != it_end)
    connInfo += " schema = " + it->second;

  it = dsInfo.find("table");

  if(it != it_end)
    connInfo += " table = " + it->second;

  it = dsInfo.find("where");

  if(it != it_end)
    connInfo += " where = " + it->second;

  it = dsInfo.find("mode");

  if(it != it_end)
    connInfo += " mode = " + it->second;

  return connInfo;
}

bool te::gdal::RecognizesSRID(unsigned int srid)
{
  OGRSpatialReference oSRS;
  oSRS.importFromEPSG(srid);
  char* coutWKT = NULL;
  oSRS.exportToWkt(&coutWKT);
  std::string outwkt(coutWKT);
  return (!outwkt.empty());
}

/* This function is based on the WARP tutorial in http://www.gdal.org */
bool te::gdal::ReprojectRaster(te::rst::Raster* rin, te::rst::Raster* rout)
{
  assert(rin);
  assert(rout);

  te::gdal::Raster* grin  = static_cast<te::gdal::Raster*>(rin);
  te::gdal::Raster* grout = static_cast<te::gdal::Raster*>(rout);

  GDALDatasetH hSrcDS = grin->getGDALDataset();
  if (hSrcDS == 0)
    return false;

  GDALDatasetH hDstDS = grout->getGDALDataset();
  if (hDstDS == 0)
    return false;

  int nBands = GDALGetRasterCount(hSrcDS);

  /* Define warp options */
  GDALWarpOptions *psWarpOptions = GDALCreateWarpOptions();
  psWarpOptions->hSrcDS = hSrcDS;
  psWarpOptions->hDstDS = hDstDS;
  psWarpOptions->nBandCount = nBands;
  psWarpOptions->panSrcBands = (int*)CPLMalloc(sizeof(int)*psWarpOptions->nBandCount);
  psWarpOptions->panDstBands = (int*)CPLMalloc(sizeof(int)*psWarpOptions->nBandCount);
  for (int b = 0; b < psWarpOptions->nBandCount; b++)
  {
    psWarpOptions->panSrcBands[b] = b+1;
    psWarpOptions->panDstBands[b] = b+1;
  }

  /* Establish reprojection transformer */
  psWarpOptions->pTransformerArg = GDALCreateGenImgProjTransformer(hSrcDS, GDALGetProjectionRef(hSrcDS),
                                                                   hDstDS, GDALGetProjectionRef(hDstDS),
                                                                   FALSE, 0.0, 1);
  psWarpOptions->pfnTransformer = GDALGenImgProjTransform;

  /* Initialize and execute the warp operation */
  GDALWarpOperation oOperation;
  oOperation.Initialize(psWarpOptions);
  oOperation.WarpRegion(0, 0, GDALGetRasterXSize(hDstDS), GDALGetRasterYSize(hDstDS));
    
  /* Close transformer and images */
  GDALDestroyGenImgProjTransformer(psWarpOptions->pTransformerArg);
  GDALDestroyWarpOptions(psWarpOptions);

  return true;
}

std::string te::gdal::GetDriverName(const std::string& name)
{
// check if it is a filename, and tries to use its extension
  boost::filesystem::path mpath(name.c_str());

  std::string ext = te::common::Convert2UCase(mpath.extension().string());

  if(ext == ".TIF" || ext == ".TIFF")
    return std::string("GTiff");

  if(ext == ".JPG")
    return std::string("JPEG");

  if(ext == ".NTF")
    return std::string("NITF");

  if(ext == ".GRB")
    return std::string("GRIB");

  if(ext == ".PNG")
    return std::string("PNG");

  return "";
}

std::string te::gdal::GetGDALConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  std::map<std::string, std::string>::const_iterator it = connInfo.find("URI");

  if(it != connInfo.end())
    return it->second;

  it = connInfo.find("SOURCE");

  if(it != connInfo.end())
    return it->second;

  throw Exception(TR_GDAL("Invalid data source connection information!."));
}

std::string te::gdal::GetSubDataSetName(const std::string name, const std::string driverName)
{
  std::string subDataSetName = "";

  if (driverName == "NITF")
  {
    boost::filesystem::path mpath(name);

    std::size_t first_colon = mpath.string().find_first_of(":");

    std::string remain = mpath.string().substr(first_colon + 1);

    std::size_t second_colon = first_colon + remain.find_first_of(":");

    subDataSetName = mpath.string().substr(0, second_colon + 2);
  }

  return subDataSetName;
}

void te::gdal::Vectorize(GDALRasterBand* band, std::vector<te::gm::Geometry*>& geometries)
{
// create data source of geometries in memory
  OGRSFDriver *ogrDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("Memory");

  OGRDataSource* ds = ogrDriver->CreateDataSource("ds_vectorize", NULL);

  OGRLayer* ogrLayer = ds->CreateLayer("vectorization", NULL, wkbMultiPolygon, NULL);

  ogrLayer->CreateField(new OGRFieldDefn("id", OFTInteger));

// call polygonize function from gdal
  if (GDALPolygonize(band, NULL , ogrLayer, 0, NULL, NULL, NULL) == CE_Failure)
    return;

// convert geometries to terralib
  for (int g = 0; g < ogrLayer->GetFeatureCount(); g++)
    geometries.push_back(te::ogr::Convert2TerraLib(ogrLayer->GetFeature(g)->GetGeometryRef()));

  OGRDataSource::DestroyDataSource(ds);
}

void te::gdal::Rasterize(std::vector<te::gm::Geometry*> geometries, GDALDataset* outraster)
{
// define list of bands (using single band)
  std::vector<int> bandList;

  bandList.push_back(1);

  std::vector<OGRGeometryH> ogrGeometries;

  std::vector<double> burnValues;

// defining vector of ogr geometries and different values for each one in raster
  int bvalue = 254;
  for (std::size_t g = 0; g < geometries.size(); g++)
  {
    burnValues.push_back(bvalue % 255);

    bvalue = bvalue >= 127? bvalue - 126: bvalue > 255? 0: bvalue + 127;

    ogrGeometries.push_back(te::ogr::Convert2OGR(geometries[g]));
  }

  GDALRasterizeGeometries(outraster, bandList.size(), &(bandList[0]), ogrGeometries.size(), &(ogrGeometries[0]), NULL, NULL, &(burnValues[0]), NULL, NULL, NULL);
}
