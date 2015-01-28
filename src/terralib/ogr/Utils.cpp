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
  \file terralib/ogr/Utils.cpp
   
  \brief Utility functions for OGR support.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/Globals.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/ArrayProperty.h"
#include "../datatype/DateTime.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/NumericProperty.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/WKBReader.h"
#include "../srs/Config.h"
#include "Utils.h"

// OGR
#include <ogrsf_frmts.h>
#include <ogr_spatialref.h>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

te::gm::Geometry* te::ogr::Convert2TerraLib(OGRGeometry* ogrGeom)
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

OGRGeometry* te::ogr::Convert2OGR(const te::gm::Geometry* teGeom)
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

te::gm::Envelope* te::ogr::Convert2TerraLib(const OGREnvelope* env)
{
   return new te::gm::Envelope(env->MinX, env->MinY, env->MaxX, env->MaxY);
}

OGREnvelope* te::ogr::Convert2OGR(const te::gm::Envelope* env)
{
  OGREnvelope* envOGR = new OGREnvelope();
  envOGR->MinX = env->m_llx;
  envOGR->MinY = env->m_lly;
  envOGR->MaxX = env->m_urx;
  envOGR->MaxY = env->m_ury;
  return envOGR;
}

int te::ogr::Convert2TerraLibProjection(OGRSpatialReference* osrs)
{
  if(osrs == 0)
    return TE_UNKNOWN_SRS;

  if(osrs->AutoIdentifyEPSG() != OGRERR_UNSUPPORTED_SRS) 
    return atoi(osrs->GetAuthorityCode(0));
  
  // geographic SIRGAS 2000 Datum
  std::string straux(osrs->GetRoot()->GetChild(0)->GetValue());
  
  if (!boost::find_first(straux, "SIRGAS"))
    return TE_UNKNOWN_SRS;
  
  if (osrs->IsGeographic())
    return TE_SRS_SIRGAS2000;
  
  // UTM using SIRGAS 2000 Datum
  if (boost::find_first(straux, "UTM "))
  {
    double centralm = osrs->GetProjParm(SRS_PP_CENTRAL_MERIDIAN,-1);
    if (centralm == -1)
      return  TE_UNKNOWN_SRS;
    int zone = (int)(centralm/6 + 31);
    
    double fsnorth = osrs->GetProjParm(SRS_PP_FALSE_NORTHING,-1);
    if (fsnorth > 0)
      return 31960+zone;
    else if (fsnorth == 0)
      return 31954+zone;
  }
  return TE_UNKNOWN_SRS;
}

OGRSpatialReference* te::ogr::Convert2OGRProjection(int srid)
{
  OGRSpatialReference* osrs = new OGRSpatialReference();
  
  OGRErr error = osrs->importFromEPSG(srid);
  
  if(error != OGRERR_NONE)
    throw(te::common::Exception(TE_TR("Error converting spatial reference system.")));
  
  return osrs;
}

void te::ogr::Convert2TerraLib(OGRFeatureDefn* featDef,  te::da::DataSetType* dt, int srs)
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

te::da::DataSetType* te::ogr::Convert2TerraLib(OGRFeatureDefn* featDef, int srs)
{
  te::da::DataSetType* dt = new  te::da::DataSetType(featDef->GetName());
  
  dt->setTitle(featDef->GetName());
  
  te::ogr::Convert2TerraLib(featDef,dt, srs);

  return dt;
}

OGRFeatureDefn* te::ogr::Convert2OGR(te::da::DataSetType* dt)
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

te::dt::Property* te::ogr::Convert2TerraLib(OGRFieldDefn* fieldDef)
{
  te::dt::Property* p = 0;
  std::string name = fieldDef->GetNameRef();
  switch(fieldDef->GetType())
  {
    case OFTInteger:
      p = new te::dt::SimpleProperty(name, te::dt::INT32_TYPE);
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
      sp->setCharEncoding(te::common::LATIN1);

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

OGRFieldDefn* te::ogr::Convert2OGR(te::dt::Property* p)
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

te::gm::GeomType te::ogr::Convert2TerraLib(OGRwkbGeometryType ogrGeomType)
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
      return te::gm::PolygonMType;

    case wkbMultiPoint25D:
      return te::gm::MultiPointMType;

    case wkbMultiLineString25D:
      return te::gm::MultiLineStringMType;

    case wkbMultiPolygon25D:
      return te::gm::MultiPolygonMType;

    case wkbGeometryCollection25D:
      return te::gm::GeometryCollectionMType;
      
    case wkbUnknown:
      return te::gm::GeometryType;
      
    default:  
      return te::gm::UnknownGeometryType;
  }
}

OGRwkbGeometryType te::ogr::Convert2OGR(te::gm::GeomType geomType)
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

    case te::gm::PointMType:
      return wkbPoint25D;

    case te::gm::LineStringMType:
      return wkbLineString25D;

    case te::gm::PolygonMType:
      return  wkbPolygon25D;

    case te::gm::MultiPointMType:
      return wkbMultiPoint25D;

    case te::gm::MultiLineStringMType:
      return wkbMultiLineString25D;

    case te::gm::MultiPolygonMType:
      return wkbMultiPolygon25D;

    case te::gm::GeometryCollectionMType:
      return wkbGeometryCollection25D;

    default:  
      throw(te::common::Exception(TE_TR("Unsupported geometry type by OGR Driver.")));
  }
}

std::string te::ogr::GetDriverName(const std::string& path)
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

std::vector<std::string> te::ogr::GetOGRDrivers(bool filterCreate
                                                )
{
  std::vector<std::string> drivernames;
  
  int ndrivers = OGRSFDriverRegistrar::GetRegistrar()->GetDriverCount();
  
  for (int i = 0; i < ndrivers; ++i)
  {
    OGRSFDriver* driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriver(i);
    if (filterCreate && !driver->TestCapability(ODrCCreateDataSource))
      continue;
    drivernames.push_back(driver->GetName());
  }
  
  return drivernames;
}

std::string te::ogr::GetOGRConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  std::map<std::string, std::string>::const_iterator it = connInfo.find("URI");

  if(it != connInfo.end())
    return it->second;

  it = connInfo.find("SOURCE");

  if(it != connInfo.end())
    return it->second;

  throw te::common::Exception(TE_TR("Invalid data source connection information!."));
}

std::string te::ogr::RemoveSpatialSql(const std::string& sql)
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

