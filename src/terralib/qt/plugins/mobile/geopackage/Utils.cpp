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
  \file terralib/ogr/Utils.cpp
   
  \brief Utility functions for OGR support.
*/

// TerraLib
#include "../../../../common/Exception.h"
#include "../../../../common/Globals.h"
#include "../../../../core/translator/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../datatype/ArrayProperty.h"
#include "../../../../datatype/DateTime.h"
#include "../../../../datatype/DateTimeProperty.h"
#include "../../../../datatype/NumericProperty.h"
#include "../../../../datatype/Property.h"
#include "../../../../datatype/SimpleProperty.h"
#include "../../../../datatype/StringProperty.h"
#include "../../../../gdal/Raster.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/Geometry.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../geometry/WKBReader.h"
#include "../../../../raster/BandProperty.h"
#include "../../../../raster/Grid.h"
#include "../../../../raster/RasterFactory.h"
#include "../../../../raster/RasterProperty.h"
#include "../../../../srs/SpatialReferenceSystemManager.h"
#include "../../../../srs/Config.h"
#include "Utils.h"



// GDAL
#include <gdal_priv.h>

// OGR
#include <ogrsf_frmts.h>
#include <ogr_spatialref.h>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

te::gm::Geometry* te::gpkg::Convert2TerraLib(OGRGeometry* ogrGeom)
{
  int wkbSize = ogrGeom->WkbSize();

  unsigned char* wkbArray = new unsigned char[wkbSize];

  ogrGeom->exportToWkb(wkbNDR, wkbArray);

  te::gm::Geometry* teGeom = 0;

  try
  {
    teGeom = te::gm::WKBReader::read((const char*)wkbArray);
  }
  catch(...)
  {
    delete [] wkbArray;
    throw;
  }

  delete [] wkbArray;

  return teGeom;
}

OGRGeometry* te::gpkg::Convert2OGR(const te::gm::Geometry* teGeom)
{
  size_t size = teGeom->getWkbSize();

  char* wkbArray = new char[size];

  teGeom->getWkb(wkbArray, te::common::Globals::sm_machineByteOrder);

  OGRSpatialReference* srs = Convert2OGRProjection(teGeom->getSRID());

  OGRGeometry* ogrGeom = 0;

  OGRErr result = OGRGeometryFactory::createFromWkb((unsigned char*)wkbArray, srs, &ogrGeom, static_cast<int>(size));

  delete [] wkbArray;

  if(result == OGRERR_NONE)
    return ogrGeom;

  if(result == OGRERR_NOT_ENOUGH_DATA)
    throw te::common::Exception(TE_TR("Error when attempting convert the geometry to OGR. Not enough data."));

  if(result == OGRERR_UNSUPPORTED_GEOMETRY_TYPE)
    throw te::common::Exception(TE_TR("Error when attempting convert the geometry to OGR. Unsupported geometry type."));

  if(result == OGRERR_CORRUPT_DATA)
    throw te::common::Exception(TE_TR("Error when attempting convert the geometry to OGR. Corrupt data."));

  return 0;
}

te::gm::Envelope* te::gpkg::Convert2TerraLib(const OGREnvelope* env)
{
   return new te::gm::Envelope(env->MinX, env->MinY, env->MaxX, env->MaxY);
}

OGREnvelope* te::gpkg::Convert2OGR(const te::gm::Envelope* env)
{
  OGREnvelope* envOGR = new OGREnvelope();
  envOGR->MinX = env->m_llx;
  envOGR->MinY = env->m_lly;
  envOGR->MaxX = env->m_urx;
  envOGR->MaxY = env->m_ury;
  return envOGR;
}

int te::gpkg::Convert2TerraLibProjection(OGRSpatialReference* osrs)
{
  if(osrs == 0)
    return TE_UNKNOWN_SRS;
  
  int srid = 0;
  
  OGRErr ogrReturn = osrs->AutoIdentifyEPSG();
  if( ogrReturn == OGRERR_NONE )
  {
    const char* srsAuth = osrs->GetAuthorityCode(0);
    
    if (srsAuth)
    {
      srid = atoi(srsAuth);
    }
  }  
  
  if( srid == TE_UNKNOWN_SRS )
  {
    char* wktPtr = 0;
    ogrReturn = osrs->exportToWkt( &wktPtr );
  
    if( ogrReturn == OGRERR_NONE )
    {    
      std::pair< std::string, unsigned int > customSRID;
      std::string projRefStr( wktPtr );
      
      OGRFree( wktPtr );
      
      try
      {
        customSRID = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromWkt( 
          projRefStr );
        srid = (int)customSRID.second;
      }
      catch( te::common::Exception& )
      {
      }
    }
  }  
  
  if( srid == TE_UNKNOWN_SRS )
  {
    char* proj4StrPtr = 0;
    ogrReturn = osrs->exportToProj4( &proj4StrPtr );
  
    if( ogrReturn == OGRERR_NONE )
    {    
      std::pair< std::string, unsigned int > customSRID;
      std::string projRefStr( proj4StrPtr );
      
      OGRFree( proj4StrPtr );
      
      try
      {
        customSRID = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt( 
          projRefStr );
        srid = (int)customSRID.second;
      }
      catch( te::common::Exception& )
      {
      }
    }
  }  
  
  if( srid == TE_UNKNOWN_SRS )
  {
    // geographic SIRGAS 2000 Datum
    std::string straux(osrs->GetRoot()->GetChild(0)->GetValue());
    
    if (boost::find_first(straux, "SIRGAS"))
    {
      if (osrs->IsGeographic())
        srid = TE_SRS_SIRGAS2000;
    }
    else if (boost::find_first(straux, "UTM "))
    { // UTM using SIRGAS 2000 Datum
      double centralm = osrs->GetProjParm(SRS_PP_CENTRAL_MERIDIAN,-1);
      if (centralm != -1)
      {
        int zone = (int)(centralm/6 + 31);
        
        double fsnorth = osrs->GetProjParm(SRS_PP_FALSE_NORTHING,-1);
        if (fsnorth > 0)
          srid = 31960+zone;
        else if (fsnorth == 0)
          srid = 31954+zone;
      }
    }
  }
  
  return srid;
}

OGRSpatialReference* te::gpkg::Convert2OGRProjection(int srid)
{
  std::auto_ptr< OGRSpatialReference > osrs( new OGRSpatialReference() );
  
  OGRErr error = osrs->importFromEPSG(srid);
  
  if( error != OGRERR_NONE )
  {
    try
    {
      std::string wktStr = 
        te::srs::SpatialReferenceSystemManager::getInstance().getWkt( srid );
      
      if( !wktStr.empty() )
      {
        char* wktStrPtr = (char*)wktStr.c_str();
        error = osrs->importFromWkt( &wktStrPtr );
      }
    }
    catch( te::common::Exception& )
    {
      error = OGRERR_UNSUPPORTED_SRS;
    }
  }  
  
  if( error != OGRERR_NONE )
  {
    try
    {
      std::string proj4Str = 
        te::srs::SpatialReferenceSystemManager::getInstance().getP4Txt( srid );
      
      if( !proj4Str.empty() )
      {
        char* proj4StrPtr = (char*)proj4Str.c_str();
        error = osrs->importFromProj4( proj4StrPtr );
      }
    }
    catch( te::common::Exception& )
    {
      error = OGRERR_UNSUPPORTED_SRS;
    }
  }   
  
  if(error != OGRERR_NONE)
    throw(te::common::Exception(TE_TR("Error converting spatial reference system.")));
  
  return osrs.release();
}

void te::gpkg::Convert2TerraLib(OGRFeatureDefn* featDef,  te::da::DataSetType* dt, int srs)
{
  assert(dt);
  
  int nFields = featDef->GetFieldCount();
  
  for(int i = 0; i < nFields; i++)
  {
    OGRFieldDefn* fieldDef = featDef->GetFieldDefn(i); 
    te::dt::Property* p = Convert2TerraLib(fieldDef);
    dt->add(p);
  }

  OGRwkbGeometryType ogrGeomType = featDef->GetGeomType();
  
  if(ogrGeomType != wkbNone) // has geometry?
  {
    te::gm::GeomType geomType = Convert2TerraLib(ogrGeomType);
    te::gm::GeometryProperty* geomPropertyType = new te::gm::GeometryProperty("OGR_GEOMETRY", srs, geomType);
    dt->add(geomPropertyType);
  }
}

te::da::DataSetType* te::gpkg::Convert2TerraLib(OGRFeatureDefn* featDef, int srs)
{
  te::da::DataSetType* dt = new  te::da::DataSetType(featDef->GetName());
  
  dt->setTitle(featDef->GetName());
  
  te::gpkg::Convert2TerraLib(featDef,dt, srs);

  return dt;
}

OGRFeatureDefn* te::gpkg::Convert2OGR(te::da::DataSetType* dt)
{
  OGRFeatureDefn* featDef = new OGRFeatureDefn(dt->getName().c_str());
  std::vector<te::dt::Property*> props = dt->getProperties();

  for(unsigned int i = 0; i < props.size(); i++)
  {
    te::dt::Property* p = props[i];
    if(p->getType() != te::dt::GEOMETRY_TYPE)
      featDef->AddFieldDefn(Convert2OGR(p));
  }

  if(dt->hasGeom())
  {
    te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(dt);
    featDef->SetGeomType(Convert2OGR(geom->getGeometryType()));
  }

  return featDef;
}

te::dt::Property* te::gpkg::Convert2TerraLib(OGRFieldDefn* fieldDef)
{
  te::dt::Property* p = 0;
  std::string name = fieldDef->GetNameRef();
  switch(fieldDef->GetType())
  {
    case OFTInteger:
      p = new te::dt::SimpleProperty(name, te::dt::INT32_TYPE);
    break;

    case OFTInteger64:
      p = new te::dt::SimpleProperty(name, te::dt::INT64_TYPE);
      break;

    case OFTIntegerList:
      p = new te::dt::ArrayProperty(name, new te::dt::SimpleProperty(name, te::dt::INT32_TYPE));
    break;

    case OFTString:
    {
      te::dt::StringProperty* sp = 0;

      if(fieldDef->GetWidth() == 0)
        sp = new te::dt::StringProperty(name, te::dt::STRING);
      else
        sp = new te::dt::StringProperty(name, te::dt::VAR_STRING, fieldDef->GetWidth());

      //sp->setCharEncoding(te::common::UTF8); // GDAL/OGR handles strings internally in UTF-8 - *** Need review! ***

      /* The original DBF standard defines to use ISO8859-1, and only ISO8859-1.
         So, when you get a Shapefile that is really standards conform, it should be ISO8859-1.
         Of course, this (very old) restriction is a not really usable nowadays.
         ISO8859-1 - also called "Latin 1"
         From: http://gis.stackexchange.com/questions/3529/which-character-encoding-is-used-by-the-dbf-file-in-shapefiles */
      // for while...
      sp->setCharEncoding(te::core::EncodingType::LATIN1);

      p = sp;
    }
    break;

    case OFTStringList:
      p = new te::dt::ArrayProperty(name, new te::dt::StringProperty(name, te::dt::STRING));
    break;

    case OFTReal:
      p = new te::dt::SimpleProperty(name, te::dt::DOUBLE_TYPE);
    break;

    case OFTRealList:
      p = new te::dt::ArrayProperty(name, new te::dt::SimpleProperty(name, te::dt::DOUBLE_TYPE));
    break;

    case OFTBinary:
      p = new te::dt::SimpleProperty(name, te::dt::BYTE_ARRAY_TYPE);
    break;

    case OFTDate:
      p = new te::dt::DateTimeProperty(name, te::dt::DATE);
    break;

    case OFTTime:
      p = new te::dt::DateTimeProperty(name, te::dt::TIME_DURATION);
    break;

    case OFTDateTime:
      p = new te::dt::DateTimeProperty(name, te::dt::TIME_INSTANT);
    break;

    default:
      throw(te::common::Exception(TE_TR("Unexpected data type.")));
  }

  return p;
}

OGRFieldDefn* te::gpkg::Convert2OGR(te::dt::Property* p)
{
  OGRFieldDefn* fieldDef = new OGRFieldDefn(p->getName().c_str(), OFTInteger);
  switch(p->getType())
  {
    case te::dt::INT32_TYPE:
      return fieldDef;
    break;

    case te::dt::ARRAY_TYPE:
    {
      te::dt::ArrayProperty* at = static_cast<te::dt::ArrayProperty*>(p);
      int elementType = at->getElementType()->getType();
      
      if(elementType == te::dt::INT32_TYPE)
        fieldDef->SetType(OFTIntegerList);
      else if(elementType == te::dt::STRING_TYPE)
        fieldDef->SetType(OFTStringList);
      else if(elementType == te::dt::DOUBLE_TYPE)
        fieldDef->SetType(OFTRealList);
      else
        throw(te::common::Exception(TE_TR("Unsupported data type by OGR.")));
      break;
    }

    case te::dt::STRING_TYPE:
      fieldDef->SetType(OFTString);
      fieldDef->SetWidth(static_cast<int>(static_cast<te::dt::StringProperty*>(p)->size()));
    break;

    case te::dt::DOUBLE_TYPE:
      fieldDef->SetType(OFTReal);
    break;

    case te::dt::NUMERIC_TYPE:
      fieldDef->SetType(OFTReal);
      fieldDef->SetPrecision(static_cast<te::dt::NumericProperty*>(p)->getScale());
    break;

    case te::dt::BYTE_ARRAY_TYPE:
      fieldDef->SetType(OFTBinary);
    break;

    case te::dt::DATETIME_TYPE:
    {
      const te::dt::DateTimeProperty* dp = static_cast<const te::dt::DateTimeProperty*>(p);
      te::dt::DateTimeType elementType = dp->getSubType();
      if(elementType == te::dt::DATE)
        fieldDef->SetType(OFTDate);
      else if(elementType == te::dt::TIME_DURATION)
        fieldDef->SetType(OFTTime);
      else if(elementType == te::dt::TIME_INSTANT)
        fieldDef->SetType(OFTDateTime);
      else
        throw(te::common::Exception(TE_TR("Unsupported data type by OGR.")));
      break;
    }

    default:
      throw(te::common::Exception(TE_TR("Unsupported data type by OGR.")));
  }
  
  return fieldDef;
}

te::gm::GeomType te::gpkg::Convert2TerraLib(OGRwkbGeometryType ogrGeomType)
{
  switch(ogrGeomType)
  {
    case wkbPoint:
      return te::gm::MultiPointType;

    case wkbLineString:
      return te::gm::MultiLineStringType;

    case wkbPolygon:
      return te::gm::MultiPolygonType;

    case wkbMultiPoint:
      return te::gm::MultiPointType;

    case wkbMultiLineString:
      return te::gm::MultiLineStringType;
    
    case wkbMultiPolygon:
      return te::gm::MultiPolygonType;

    case wkbGeometryCollection:
      return te::gm::GeometryCollectionType;

    case wkbLinearRing:
      return te::gm::LineStringType;

    case wkbPoint25D:
      return te::gm::PointMType;

    case wkbLineString25D:
      return te::gm::LineStringMType;

    case wkbPolygon25D:
      return te::gm::MultiPolygonType;

    case wkbMultiPoint25D:
      return te::gm::MultiPointMType;

    case wkbMultiLineString25D:
      return te::gm::MultiLineStringMType;

    case wkbMultiPolygon25D:
      return te::gm::MultiPolygonMType;

    case wkbGeometryCollection25D:
      return te::gm::GeometryCollectionMType;

    case wkbMultiSurface:
      return te::gm::MultiSurfaceType;
      
    case wkbUnknown:
      return te::gm::GeometryType;
      
    default:  
      return te::gm::UnknownGeometryType;
  }
}

OGRwkbGeometryType te::gpkg::Convert2OGR(te::gm::GeomType geomType)
{
  switch(geomType)
  {
    case te::gm::UnknownGeometryType:
      return wkbUnknown;

    case te::gm::PointType:
      return wkbPoint;

    case te::gm::LineStringType:
      return wkbLineString;

    case te::gm::PolygonType:
      return wkbPolygon;

    case te::gm::MultiPointType:
      return wkbMultiPoint;

    case te::gm::MultiLineStringType:
      return wkbMultiLineString;
    
    case te::gm::MultiPolygonType:
      return wkbMultiPolygon;

    case te::gm::GeometryCollectionType:
      return wkbGeometryCollection;

	case te::gm::PointZType:
	case te::gm::PointMType:
	case te::gm::PointZMType:
		return wkbPoint25D;

	case te::gm::LineStringZType:
	case te::gm::LineStringMType:
	case te::gm::LineStringZMType:
		return wkbLineString25D;

	case te::gm::PolygonZType:
	case te::gm::PolygonMType:
	case te::gm::PolygonZMType:
		return  wkbPolygon25D;

	case te::gm::MultiPointZType:
	case te::gm::MultiPointMType:
	case te::gm::MultiPointZMType:
		return wkbMultiPoint25D;

	case te::gm::MultiLineStringZType:
	case te::gm::MultiLineStringMType:
	case te::gm::MultiLineStringZMType:
		return wkbMultiLineString25D;

	case te::gm::MultiPolygonZType:
	case te::gm::MultiPolygonMType:
	case te::gm::MultiPolygonZMType:
		return wkbMultiPolygon25D;

	case te::gm::GeometryCollectionZType:
	case te::gm::GeometryCollectionMType:
	case te::gm::GeometryCollectionZMType:
		return wkbGeometryCollection25D;

	case te::gm::PolyhedralSurfaceType:
	case te::gm::PolyhedralSurfaceZType:
	case te::gm::PolyhedralSurfaceMType:
	case te::gm::PolyhedralSurfaceZMType:
		return wkbMultiPolygon25D;

	case te::gm::TINType:
	case te::gm::TINZType:
	case te::gm::TINMType:
	case te::gm::TINZMType:
		return wkbMultiPolygon25D;

	case te::gm::TriangleType:
	case te::gm::TriangleZType:
	case te::gm::TriangleMType:
	case te::gm::TriangleZMType:

	default:
      throw(te::common::Exception(TE_TR("Unsupported geometry type by OGR Driver.")));
  }
}

std::string te::gpkg::GetDriverName(const std::string& path)
{
  boost::filesystem::path mpath(path.c_str());

  std::string ext = mpath.extension().string();
  
  if(ext == ".shp" || ext == ".SHP")
    return std::string("ESRI Shapefile");

  if(ext == ".mif" || ext == ".MIF")
    return std::string("Mapinfo File");
  
  if(ext == ".kml" || ext == ".KML")
    return std::string("KML");
  
  if(ext == ".geojson" || ext == ".GEOJSON")
    return std::string("GeoJSON");
  
  if(ext == ".gml" || ext == ".GML")
    return std::string("GML");
  
  if(ext == ".dxf" || ext == ".DXF")
    return std::string("DXF");

  if(ext == ".dgn" || ext == ".DGN")
    return std::string("DGN");

  return "";
}

std::vector<std::string> te::gpkg::GetOGRDrivers(bool filterCreate)
{
  std::vector<std::string> drivernames;

  int ndrivers = OGRSFDriverRegistrar::GetRegistrar()->GetDriverCount();

  for (int i = 0; i < ndrivers; ++i)
  {
    GDALDriver* driver = GetGDALDriverManager()->GetDriver(i);
    //if (filterCreate && !driver->GetMetadataItem(ODrCCreateDataSource))
    if (filterCreate && !OGR_Dr_TestCapability(driver, ODrCCreateDataSource))
        continue;
    drivernames.push_back(driver->GetDescription());
  }

  return drivernames;
}

std::string te::gpkg::GetOGRConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  std::map<std::string, std::string>::const_iterator it = connInfo.find("URI");

  if(it != connInfo.end())
    return it->second;

  it = connInfo.find("SOURCE");

  if(it != connInfo.end())
    return it->second;

  throw te::common::Exception(TE_TR("Invalid data source connection information!."));
}

std::string te::gpkg::RemoveSpatialSql(const std::string& sql)
{
  // Try find AND
  std::size_t pos = sql.find("AND Intersection");

  // Try find without AND
  if(pos == std::string::npos)
    pos = sql.find("WHERE Intersection");

  if(pos == std::string::npos)
    return sql;

  std::string newQuery;

  std::size_t pos2 = sql.find("))", pos);
  newQuery = sql.substr(0, pos);
  newQuery += sql.substr(pos2 + 2);

  return newQuery;
}

void te::gpkg::createGeopackage(std::string outFileName)
{
  const char *gpkgFormat = "GPKG";

  GDALDriver *gpkgDriver;

  gpkgDriver = GetGDALDriverManager()->GetDriverByName(gpkgFormat);

  char **papszOptions = NULL;

  GDALDataset *poDstDS;
  poDstDS = gpkgDriver->Create(outFileName.c_str(), 0, 0, 0, GDT_Unknown, papszOptions);
  GDALClose((GDALDatasetH)poDstDS);
}

void te::gpkg::copyToGeopackage(te::rst::Raster* raster, std::string outFileName)
{
  te::gdal::Raster* gdalRaster = dynamic_cast<te::gdal::Raster*>(raster);

  const char *gpkgFormat = "GPKG";
  GDALDriver *gpkgDriver;
  gpkgDriver = GetGDALDriverManager()->GetDriverByName(gpkgFormat);

  char **papszOptions = NULL;
  papszOptions = CSLSetNameValue(papszOptions, "APPEND_SUBDATASET", "YES");
  papszOptions = CSLSetNameValue(papszOptions, "TILING_SCHEME", "GoogleMapsCompatible");
  papszOptions = CSLSetNameValue(papszOptions, "ZOOM_LEVEL_STRATEGY", "LOWER");

  GDALDataset *poDstDS = gpkgDriver->CreateCopy(outFileName.c_str(), gdalRaster->getGDALDataset(), FALSE, papszOptions, NULL, NULL);;

  unsigned int levels = gdalRaster->getMultiResLevelsCount();
  if (levels > 0)
  {
    boost::scoped_array< int > overviewsIndexes(new int[levels]);

    for (unsigned int overViewIdx = 1; overViewIdx <= levels; ++overViewIdx)
    {
      /*
      Power of two overview factors(2, 4, 8, 16, ...) should be favored to be conformant
      with the baseline GeoPackage specification as mentioned in gdal documentation.
      */
      unsigned int index = (unsigned int)std::pow(2, overViewIdx);
      overviewsIndexes[(overViewIdx - 1)] = index;
    }

    CPLErr returnValue = poDstDS->BuildOverviews("NEAREST", (int)levels, overviewsIndexes.get(), 0, NULL, NULL, NULL);
    poDstDS->FlushCache();
  }

  CSLDestroy(papszOptions);
  GDALClose((GDALDatasetH)poDstDS);
}
