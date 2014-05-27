// Examples
#include "RasterExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/raster.h>

// STL
#include <exception>
#include <iostream>
#include <memory>
#include <map>
#include <string>

#if TE_EXAMPLE_USE_GRIB

// GDAL
#include <gdal_priv.h>
#include <gdalwarper.h>
#include <ogr_spatialref.h>

void ReprojectRaster(GDALDataset* iraster,
                     std::string outfile,
                     std::string outWKT)
{
  GDALDataType eDT = GDALGetRasterDataType(GDALGetRasterBand(iraster, 1));

  std::string dName = "GTiff";

  GDALDriverH hDriver = GDALGetDriverByName(dName.c_str());

  if(hDriver == 0)
    return;

  const char *pszSrcWKT = GDALGetProjectionRef(iraster);

  if(pszSrcWKT == 0 || strlen(pszSrcWKT) == 0)
    return;

  char *pszDstWKT = (char*) outWKT.c_str();

  if(pszDstWKT == 0 || strlen(pszDstWKT) == 0)
    return;

  void* hTransformArg = GDALCreateGenImgProjTransformer(iraster, 0, NULL,
                                                        pszDstWKT, FALSE, 0, 1);
  if (hTransformArg == 0)
    return;

  /* Get approximate output georeferenced bounds and resolution for file */
  double adfDstGeoTransform[6];

  int nPixels=0, nLines=0;

  if(GDALSuggestedWarpOutput(iraster, GDALGenImgProjTransform, hTransformArg,
                              adfDstGeoTransform, &nPixels, &nLines) != CE_None)
    return;

  int nBands = GDALGetRasterCount(iraster);

  GDALDestroyGenImgProjTransformer(hTransformArg);

  /* Create the output file */
  GDALDatasetH hDstDS = GDALCreate(hDriver, outfile.c_str(), nPixels,
                                   nLines, nBands, eDT, NULL);
  if (hDstDS == 0)
    return;

  /* Write out the projection definition */
  GDALSetProjection(hDstDS, pszDstWKT);
  GDALSetGeoTransform(hDstDS, adfDstGeoTransform);

  /* Copy the color table, if required */
  GDALColorTableH hCT;

  hCT = GDALGetRasterColorTable(GDALGetRasterBand(iraster,1));

  if(hCT != NULL)
    GDALSetRasterColorTable(GDALGetRasterBand(hDstDS,1), hCT);

  /* Define warp options */
  GDALWarpOptions *psWarpOptions = GDALCreateWarpOptions();
  psWarpOptions->hSrcDS = iraster;
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
  psWarpOptions->pTransformerArg = GDALCreateGenImgProjTransformer(iraster, GDALGetProjectionRef(iraster),
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

  GDALClose(hDstDS);


  return;
}
#endif
void GribExample()
{
#if TE_EXAMPLE_USE_GRIB
  try
  {
// open raster for read
    std::auto_ptr<te::rst::Raster> raster(te::rst::RasterFactory::open("GRIB",
                                                                       "URI",
                                                                       "/home/ecmwf/terralib5/trunk/examples/raster/Z500.grb"));
//                                                                       "/home/ecmwf/terralib5/trunk/examples/raster/image.grb"));


    te::rst::Grid* grid = raster->getGrid();

    int nrows = grid->getNumberOfRows();
    int ncols = grid->getNumberOfColumns();
    
    double* gt = (double*)(grid->getGeoreference());

    void* buff = raster->getBand(0)->read(0, 0);
    double* buffd = (double*)buff;

#if 1
    char* buffbyte = new char[nrows*ncols];
    for(int i = 0; i < nrows*ncols; ++i)
    {
      buffbyte[i] = (char)(buffd[i]);
    }
#endif

    boost::int64_t buffaddress = (boost::int64_t)buffbyte;
//    boost::int64_t buffaddress = (boost::int64_t)buffd;

    std::string memraster  = "MEM:::DATAPOINTER=";
                memraster += te::common::Convert2String(buffaddress);
                memraster += ",PIXELS=";
                memraster += te::common::Convert2String(ncols);
                memraster += ",LINES=";
                memraster += te::common::Convert2String(nrows);
                //memraster += ",BANDS=1,DATATYPE=float64";
                memraster += ",BANDS=1,DATATYPE=byte";

    GDALAllRegister();

    GDALDataset* gds = (GDALDataset*)GDALOpen(memraster.c_str(), GA_ReadOnly);

    if(gds == 0)
      throw te::rst::Exception("caca!");

    double xc = grid->getExtent()->getLowerLeftX() + grid->getExtent()->getWidth() / 2.0;
    double yc = grid->getExtent()->getLowerLeftY() + grid->getExtent()->getHeight() / 2.0;

#if 0
    // projecao satellite
    std::string cpprojsrs  = "+proj=geos +lon_0=0 +h=35785831 +x_0=";
                cpprojsrs += te::common::Convert2String(xc);
                cpprojsrs += " +y_0=";
                cpprojsrs += te::common::Convert2String(yc);
                cpprojsrs += " +ellps=WGS84 +units=m +no_defs";
    const char* projsrs = cpprojsrs.c_str();
    
    OGRSpatialReference iSRS;
    iSRS.importFromProj4(projsrs);
#else
    // projecao latlong
    OGRSpatialReference iSRS;

    // lat/long
    iSRS.importFromEPSG(4326);
#endif

    char* iWKT = NULL;
    iSRS.exportToWkt(&iWKT);

    gds->SetProjection(iWKT);

    gds->SetGeoTransform(gt);

    OGRSpatialReference oSRS;

    // lat/long
    oSRS.importFromEPSG(4326);

    // polar stereografica norte
    //oSRS.importFromEPSG(3995);
    //oSRS.importFromProj4(projsrs);

    // polar stereografica norte
    //oSRS.importFromProj4("+proj=stere +lat_0=90 +lat_ts=90 +lon_0=0 +k=0.994 +x_0=2000000 +y_0=2000000 +ellps=WGS84 +datum=WGS84 +units=m +no_defs");

    char* oWKT = NULL;
    oSRS.exportToWkt(&oWKT);

    ReprojectRaster(gds,
                    "/home/ecmwf/terralib5/trunk/examples/raster/Z500_ll.tif",
                    oWKT);

    GDALClose(gds);

  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in Grib example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unknown exception has occurred in Grib example!" << std::endl;
  }

#endif  // TE_EXAMPLE_USE_GRIB

  return;
}
