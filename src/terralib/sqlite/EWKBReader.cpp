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
  \file terralib/sqlite/EWKBReader.cpp
   
  \brief An utility class for reading a SpatiaLite EWKB geometry.
*/

// TerraLib
#include "../common/ByteSwapUtils.h"
#include "../common/Enums.h"
#include "../common/Globals.h"
#include "../common/HexUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/Exception.h"
#include "../geometry.h"
#include "EWKBReader.h"
#include "Utils.h"

// STL
#include <cassert>
#include <cstring>

// SQLite
#include <sqlite3.h>

#ifdef TE_ENABLE_SPATIALITE
// SpatiaLite
#include <spatialite/gaiageo.h>

namespace te
{
  namespace sqlite
  {
    template<class T> inline T* GetCoordSequence(const unsigned char* ewkb,
                                                 const unsigned char** endptr,
                                                 unsigned int gtype,
                                                 unsigned char ewkbByteOrder,
                                                 int srid,
                                                 te::gm::Envelope* mbr)
    {
      assert(ewkb && endptr);

      unsigned int nPts = 0;
      memcpy(&nPts, ewkb, 4);
      ewkb += 4;

      if(te::common::Globals::sm_machineByteOrder != ewkbByteOrder)
        te::common::SwapBytes(nPts);

      T* cs = new T(nPts, static_cast<te::gm::GeomType>(gtype), srid, mbr);

      switch(gtype)
      {
        case GAIA_LINESTRING :
          memcpy(cs->getCoordinates(), ewkb, 16 * nPts);
          ewkb += nPts * 16;
        break;
        
        case GAIA_LINESTRINGZ :
          for(unsigned int i = 0; i < nPts; ++i)
          {
            memcpy(&(cs->getCoordinates()[i]), ewkb, 16);
            memcpy(&(cs->getZ()[i]), ewkb + 16, 8);
            ewkb += 24;
          }
        break;

        case GAIA_LINESTRINGM :
          for(unsigned int i = 0; i < nPts; ++i)
          {
            memcpy(&(cs->getCoordinates()[i]), ewkb, 16);
            memcpy(&(cs->getM()[i]), ewkb + 16, 8);
            ewkb += 24;
          }
        break;

        case GAIA_LINESTRINGZM :
          for(unsigned int i = 0; i < nPts; ++i)
          {
            memcpy(&(cs->getCoordinates()[i]), ewkb, 16);
            memcpy(&(cs->getZ()[i]), ewkb + 16, 8);
            memcpy(&(cs->getM()[i]), ewkb + 24, 8);
            ewkb += 32;
          }
        break;
      }
      
      if(te::common::Globals::sm_machineByteOrder != ewkbByteOrder)
      {
        for(unsigned int i = 0; i < nPts; ++i)
        {
          te::common::SwapBytes(cs->getCoordinates()[i].x);
          te::common::SwapBytes(cs->getCoordinates()[i].y);
        }

        if(cs->getZ())
          for(unsigned int i = 0; i < nPts; ++i)
            te::common::SwapBytes(cs->getZ()[i]);

        if(cs->getM())
          for(unsigned int i = 0; i < nPts; ++i)
            te::common::SwapBytes(cs->getM()[i]);
      }

      *endptr = ewkb;

      return cs;
    }
  }
}

te::gm::Geometry* te::sqlite::EWKBReader::read(const unsigned char* ewkb)
{
  const unsigned char* ewkbb = ewkb;

  if((ewkb[0] != GAIA_MARK_START) || (ewkb[38] != GAIA_MARK_MBR))
    throw te::da::Exception(TR_COMMON("It is an invalid SpatiaLite geometry!"));

  int srid = -1;
  double llx = 0.0, lly = 0.0, urx = 0.0, ury = 0.0;
  memcpy(&srid, ewkb + 2, 4);
  memcpy(&llx, ewkb + 6, 8);
  memcpy(&lly, ewkb + 14, 8);
  memcpy(&urx, ewkb + 22, 8);
  memcpy(&ury, ewkb + 30, 8);

  if(te::common::Globals::sm_machineByteOrder != ewkb[1])
  {
    te::common::SwapBytes(srid);
    te::common::SwapBytes(llx);
    te::common::SwapBytes(lly);
    te::common::SwapBytes(urx);
    te::common::SwapBytes(ury);
  }

  te::gm::Envelope* mbr = new te::gm::Envelope(llx, lly, urx, ury);

  te::gm::Geometry* g = getGeometry(ewkb + 39, &ewkbb, ewkb[1], srid, mbr);

  if(ewkbb[0] != GAIA_MARK_END)
  {
    delete g;
    throw te::da::Exception(TR_COMMON("It is an invalid SpatiaLite geometry!"));
  }

  return g;
}

te::gm::Geometry* te::sqlite::EWKBReader::readHex(const unsigned char* hewkb)
{
  char* ewkb = te::common::Hex2Binary((const char*)hewkb);

  te::gm::Geometry* g = read((const unsigned char*)ewkb);

  delete [] ewkb;

  return g;
}

te::gm::Geometry* te::sqlite::EWKBReader::getGeometry(const unsigned char* ewkb,
                                                      const unsigned char** endptr,
                                                      unsigned char ewkbByteOrder,
                                                      int srid,
                                                      te::gm::Envelope* mbr)
{
  unsigned int gType = GAIA_UNKNOWN;

  memcpy(&gType, ewkb, 4);

  if(te::common::Globals::sm_machineByteOrder != ewkbByteOrder)
    te::common::SwapBytes(gType);

  ewkb += 4;
  *endptr = ewkb;

  switch(gType)
  {
    case GAIA_POINT:
    case GAIA_POINTZ:
    case GAIA_POINTM:
    case GAIA_POINTZM:
      return getPoint(ewkb, endptr, ewkbByteOrder, gType, srid, mbr);

    case GAIA_LINESTRING:
    case GAIA_LINESTRINGZ:
    case GAIA_LINESTRINGM:
    case GAIA_LINESTRINGZM:
      return getLineString(ewkb, endptr, ewkbByteOrder, gType, srid, mbr);

    case GAIA_POLYGON:
    case GAIA_POLYGONZ:
    case GAIA_POLYGONM:
    case GAIA_POLYGONZM:
      return getPolygon(ewkb, endptr, ewkbByteOrder, gType, srid, mbr);

    case GAIA_MULTIPOINT:
    case GAIA_MULTIPOINTZ:
    case GAIA_MULTIPOINTM:
    case GAIA_MULTIPOINTZM:
    case GAIA_MULTILINESTRING:
    case GAIA_MULTILINESTRINGZ:
    case GAIA_MULTILINESTRINGM:
    case GAIA_MULTILINESTRINGZM:
    case GAIA_MULTIPOLYGON:
    case GAIA_MULTIPOLYGONZ:
    case GAIA_MULTIPOLYGONM:
    case GAIA_MULTIPOLYGONZM:
    case GAIA_GEOMETRYCOLLECTION:
    case GAIA_GEOMETRYCOLLECTIONZ:
    case GAIA_GEOMETRYCOLLECTIONM:
    case GAIA_GEOMETRYCOLLECTIONZM:
      return getGeometryCollection(ewkb, endptr, ewkbByteOrder, gType, srid, mbr);

    default:
      throw te::da::Exception(TR_COMMON("This kind of geometry id not supported by SpatiaLite!"));
  }
}

te::gm::Point* te::sqlite::EWKBReader::getPoint(const unsigned char* ewkb,
                                                const unsigned char** endptr,
                                                unsigned char ewkbByteOrder,
                                                unsigned int gtype,
                                                int srid,
                                                te::gm::Envelope* mbr)
{
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
  double m = 0.0;

  memcpy(&x, ewkb, 8);
  memcpy(&y, ewkb + 8, 8);

  ewkb += 16;

  if(gtype != GAIA_POINT)
  {
    switch(gtype)
    {
      case GAIA_POINTZ:
        memcpy(&z, ewkb, 8);
        ewkb += 8;
        break;

      case GAIA_POINTM:
        memcpy(&m, ewkb, 8);
        ewkb += 8;
        break;

      default:
        assert(gtype == GAIA_POINTZM);
        memcpy(&z, ewkb, 8);
        memcpy(&m, ewkb + 8, 8);
        ewkb += 16;    
    }
  }

  *endptr = ewkb;

  if(te::common::Globals::sm_machineByteOrder != ewkbByteOrder)
  {
    te::common::SwapBytes(srid);
    te::common::SwapBytes(x);
    te::common::SwapBytes(y);
    te::common::SwapBytes(z);
    te::common::SwapBytes(m);
  }

  switch(gtype)
  {
    case GAIA_POINT:
      return new te::gm::Point(x, y, srid, mbr);

    case GAIA_POINTZ:
      return new te::gm::PointZ(x, y, z, srid, mbr);

    case GAIA_POINTM:
      return new te::gm::PointM(x, y, m, srid, mbr);

    case GAIA_POINTZM:
      return new te::gm::PointZM(x, y, z, m, srid, mbr);

    default:
      throw te::da::Exception(TR_COMMON("This kind of geometry id not supported by SpatiaLite!"));
  }
}

te::gm::LineString* te::sqlite::EWKBReader::getLineString(const unsigned char* ewkb,
                                                          const unsigned char** endptr,
                                                          unsigned char ewkbByteOrder,
                                                          unsigned int gtype,
                                                          int srid,
                                                          te::gm::Envelope* mbr)
{
  te::gm::LineString* l = GetCoordSequence<te::gm::LineString>(ewkb, endptr, gtype, ewkbByteOrder, srid, mbr);
  return l;
}

te::gm::LinearRing* te::sqlite::EWKBReader::getLinearRing(const unsigned char* ewkb,
                                                          unsigned int gtype,
                                                          const unsigned char** endptr,
                                                          unsigned char ewkbByteOrder,
                                                          int srid)
{
  te::gm::LinearRing* r = GetCoordSequence<te::gm::LinearRing>(ewkb, endptr, gtype, ewkbByteOrder, srid, 0);
  return r;
}

te::gm::Polygon* te::sqlite::EWKBReader::getPolygon(const unsigned char* ewkb,
                                                    const unsigned char** endptr,
                                                    unsigned char ewkbByteOrder,
                                                    unsigned int gtype,
                                                    int srid,
                                                    te::gm::Envelope* mbr)
{
  unsigned int nRings = 0;
  memcpy(&nRings, ewkb, 4);
  ewkb += 4;

  if(te::common::Globals::sm_machineByteOrder != ewkbByteOrder)
    te::common::SwapBytes(nRings);
  
  te::gm::Polygon* p = 0;
  
  unsigned int rType = GAIA_UNKNOWN;

  switch(gtype)
  {
    case GAIA_POLYGON:
      rType = GAIA_LINESTRING;
      p = new te::gm::Polygon(nRings, te::gm::PolygonType, srid, mbr);
    break;

    case GAIA_POLYGONZ:
      rType = GAIA_LINESTRINGZ;
      p = new te::gm::Polygon(nRings, te::gm::PolygonZType, srid, mbr);
    break;

    case GAIA_POLYGONM:
      rType = GAIA_LINESTRINGM;
      p = new te::gm::Polygon(nRings, te::gm::PolygonMType, srid, mbr);
    break;

    case GAIA_POLYGONZM:
      rType = GAIA_LINESTRINGZM;
      p = new te::gm::Polygon(nRings, te::gm::PolygonZMType, srid, mbr);
    break;

    default:
      throw te::da::Exception(TR_COMMON("This kind of geometry id not supported by SpatiaLite!"));
  }

  *endptr = ewkb;

  if(p == 0)
    return 0;

  for(unsigned int i = 0; i < nRings; ++i)
  {
    te::gm::LinearRing* r = getLinearRing(ewkb, rType, endptr, ewkbByteOrder, srid);
    ewkb = *endptr;
    p->getRings()[i] = r;
  }

  return p;
}

te::gm::GeometryCollection* te::sqlite::EWKBReader::getGeometryCollection(const unsigned char* ewkb,
                                                                          const unsigned char** endptr,
                                                                          unsigned char ewkbByteOrder,
                                                                          unsigned int gtype,
                                                                          int srid,
                                                                          te::gm::Envelope* mbr)
{
  unsigned int nGeoms = 0;
  memcpy(&nGeoms, ewkb, 4);
  ewkb += 4;

  if(te::common::Globals::sm_machineByteOrder != ewkbByteOrder)
    te::common::SwapBytes(nGeoms);
  
  te::gm::GeometryCollection* gc = 0;

  switch(gtype)
  {
    case GAIA_MULTIPOLYGON:
    case GAIA_MULTIPOLYGONZ:
    case GAIA_MULTIPOLYGONM:
    case GAIA_MULTIPOLYGONZM:
      gc = new te::gm::MultiPolygon(nGeoms, static_cast<te::gm::GeomType>(gtype), srid, mbr);
    break;

    case GAIA_MULTIPOINT:
    case GAIA_MULTIPOINTZ:
    case GAIA_MULTIPOINTM:
    case GAIA_MULTIPOINTZM:
      gc = new te::gm::MultiPoint(nGeoms, static_cast<te::gm::GeomType>(gtype), srid, mbr);
    break;

    case GAIA_MULTILINESTRING:
    case GAIA_MULTILINESTRINGZ:
    case GAIA_MULTILINESTRINGM:
    case GAIA_MULTILINESTRINGZM:
      gc = new te::gm::MultiLineString(nGeoms, static_cast<te::gm::GeomType>(gtype), srid, mbr);
    break;

    case GAIA_GEOMETRYCOLLECTION:
    case GAIA_GEOMETRYCOLLECTIONZ:
    case GAIA_GEOMETRYCOLLECTIONM:
    case GAIA_GEOMETRYCOLLECTIONZM:
      gc = new te::gm::GeometryCollection(nGeoms, static_cast<te::gm::GeomType>(gtype), srid, mbr);
    break;

    default:
      throw te::da::Exception(TR_COMMON("This kind of geometry id not supported by SpatiaLite!"));
  }

  *endptr = ewkb;

  if(gc == 0)
    throw te::da::Exception(TR_COMMON("This kind of geometry id not supported by SpatiaLite!"));

  for(unsigned int i = 0; i < nGeoms; ++i)
  {
    if(ewkb[0] != GAIA_MARK_ENTITY)
      return gc;

    unsigned int classtype = GAIA_UNKNOWN;

    memcpy(&classtype, ewkb + 1, 4);
    ewkb += 5;

    if(te::common::Globals::sm_machineByteOrder != ewkbByteOrder)
      te::common::SwapBytes(classtype);

    te::gm::Geometry* g = 0;

    switch(classtype)
    {
      case GAIA_POINT:
      case GAIA_POINTZ:
      case GAIA_POINTM:
      case GAIA_POINTZM:
        g = getPoint(ewkb, endptr, ewkbByteOrder, classtype, srid, 0);
      break;

      case GAIA_LINESTRING:
      case GAIA_LINESTRINGZ:
      case GAIA_LINESTRINGM:
      case GAIA_LINESTRINGZM:
        g = getLineString(ewkb, endptr, ewkbByteOrder, classtype, srid, 0);
      break;

      case GAIA_POLYGON:
      case GAIA_POLYGONZ:
      case GAIA_POLYGONM:
      case GAIA_POLYGONZM:
        g = getPolygon(ewkb, endptr, ewkbByteOrder, GAIA_POLYGON, srid, 0);
      break;

      default:
        throw te::da::Exception(TR_COMMON("This kind of geometry is not handled by TerraLib SpatiaLite driver yet!"));
    }

    g->setSRID(srid);
    ewkb = *endptr;
    gc->getGeometries()[i] = g;
  }

  return gc;
}

#endif // TE_ENABLE_SPATIALITE

