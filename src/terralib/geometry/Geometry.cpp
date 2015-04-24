/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/Geometry.cpp

  \brief Geometry is the root class of the geometries hierarchy, it follows OGC Simple Feature Specification - SFS (Simple Feature Access - Part 1: Common Architecture) and ISO SQL/MM Spatial.
*/

// TerraLib
#include "terralib_config.h"
#include "../common/Globals.h"
#include "../common/Translator.h"
#include "../datatype/Enums.h"
#include "Envelope.h"
#include "Exception.h"
#include "Geometry.h"
#include "GEOSReader.h"
#include "GEOSWriter.h"
#include "Utils.h"
#include "WKTReader.h"
#include "WKTWriter.h"
#include "WKBSize.h"
#include "WKBWriter.h"

// STL
#include <cassert>
#include <cstring>
#include <memory>
#include <sstream>

#ifdef TERRALIB_GEOS_ENABLED
// GEOS
#include <geos/geom/Geometry.h>
#include <geos/geom/IntersectionMatrix.h>
#include <geos/operation/buffer/OffsetCurveBuilder.h>
#include <geos/operation/union/CascadedPolygonUnion.h>
#include <geos/util/GEOSException.h>
#endif

std::map<std::string, te::gm::GeomType> te::gm::Geometry::sm_geomTypeMap;

te::gm::Geometry::Geometry(GeomType t, int srid, Envelope* mbr) throw()
  : m_gType(t),
    m_srid(srid),
    m_mbr(mbr)
{
}

te::gm::Geometry::Geometry(const Geometry& rhs) throw()
  : m_gType(rhs.m_gType),
    m_srid(rhs.m_srid),
    m_mbr(0)
{
}

te::gm::Geometry::~Geometry()
{
  delete m_mbr;
}

te::gm::Geometry& te::gm::Geometry::operator=(const Geometry& rhs) throw()
{
  if(this != &rhs)
  {
    m_gType = rhs.m_gType;

    m_srid = rhs.m_srid;

    delete m_mbr;

    m_mbr = rhs.m_mbr ? new Envelope(*rhs.m_mbr) : 0;
  }

  return *this;
}

int te::gm::Geometry::getCoordinateDimension() const throw()
{
  return GetCoordDimension(m_gType);
}

te::gm::Geometry* te::gm::Geometry::getEnvelope() const throw()
{
  return GetGeomFromEnvelope(getMBR(), m_srid);
}

const te::gm::Envelope* te::gm::Geometry::getMBR() const throw()
{
  if(m_mbr == 0)
    computeMBR(true);

  return m_mbr;
}

std::string te::gm::Geometry::asText() const throw()
{
  std::stringstream stream(std::ios_base::in | std::ios_base::out);

  stream.precision(18);

  WKTWriter::write(this, stream);

  return stream.str();
}

char* te::gm::Geometry::asBinary(std::size_t& size) const throw(Exception)
{
  size = WKBSize::size(this);

  char* wkb = new char[size];

  getWkb(wkb, te::common::Globals::sm_machineByteOrder);

  return wkb;
}

std::size_t te::gm::Geometry::getWkbSize() const throw()
{
  return WKBSize::size(this);
}

void te::gm::Geometry::getWkb(char* wkb, te::common::MachineByteOrder byteOrder) const throw(Exception)
{
  WKBWriter::write(this, wkb, byteOrder);
}

bool te::gm::Geometry::isEmpty() const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  std::auto_ptr<geos::geom::Geometry> g(GEOSWriter::write(this));

  return g->isEmpty();

#else
  throw Exception(TE_TR("isEmpty routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::isSimple() const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  std::auto_ptr<geos::geom::Geometry> g(GEOSWriter::write(this));

  return g->isSimple();

#else
  throw Exception(TE_TR("isSimple routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::isValid() const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  std::auto_ptr<geos::geom::Geometry> g(GEOSWriter::write(this));

  return g->isValid();

#else
  throw Exception(TE_TR("isValid routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::is3D() const throw()
{
  if((m_gType & 0xF00) == 0xB00)    // it is zm
    return true;

  if((m_gType & 0x0F00) == 0x300)   // it is z
    return true;

  return false;                     // it is 2D or M
}

bool te::gm::Geometry::isMeasured() const throw()
{
  if((m_gType & 0xF00) == 0xB00)  // it is zm
    return true;

  if((m_gType & 0xF00) == 0x700)  // it is m
    return true;

  return false;                   // it is 2D or z
}

te::gm::Geometry* te::gm::Geometry::getBoundary() const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  std::auto_ptr<geos::geom::Geometry> g(GEOSWriter::write(this)); 
  std::auto_ptr<geos::geom::Geometry> b(g->getBoundary());
  return GEOSReader::read(b.get());

#else
  throw te::common::Exception(TE_TR("getBoundary routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::equals(const Geometry* const rhs, const bool exact) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
  
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  if(exact == true)
    return thisGeom->equalsExact(rhsGeom.get());
  else
    return thisGeom->equals(rhsGeom.get());
#else
  throw te::common::Exception(TE_TR("equals routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::disjoint(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->disjoint(rhsGeom.get());

#else
  throw te::common::Exception(TE_TR("disjoint routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::intersects(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->intersects(rhsGeom.get());

#else
  throw te::common::Exception(TE_TR("intersects routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::touches(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->touches(rhsGeom.get());

#else
  throw te::common::Exception(TE_TR("touches routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::crosses(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->crosses(rhsGeom.get());

#else
  throw te::common::Exception(TE_TR("crosses routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::within(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->within(rhsGeom.get());

#else
  throw te::common::Exception(TE_TR("within routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::contains(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->contains(rhsGeom.get());

#else
  throw te::common::Exception(TE_TR("contains routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::overlaps(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->overlaps(rhsGeom.get());

#else
  throw te::common::Exception(TE_TR("overlaps routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::relate(const Geometry* const rhs, const std::string& matrix) const throw(std::exception)
{
  assert(matrix.size() == 9);

#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->relate(rhsGeom.get(), matrix);

#else
  throw te::common::Exception(TE_TR("relate routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

std::string te::gm::Geometry::relate(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  std::auto_ptr<geos::geom::IntersectionMatrix> m(thisGeom->relate(rhsGeom.get()));

  return m->toString();

#else
  throw te::common::Exception(TE_TR("relate routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::covers(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->covers(rhsGeom.get());

#else
  throw te::common::Exception(TE_TR("covers routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::coveredBy(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->coveredBy(rhsGeom.get());

#else
  throw te::common::Exception(TE_TR("coveredBy routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

te::gm::Geometry* te::gm::Geometry::locateBetween(const double& /*mStart*/, const double& /*mEnd*/) const throw(Exception)
{
  return 0;
}

double te::gm::Geometry::distance(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->distance(rhsGeom.get());

#else
  throw te::common::Exception(TE_TR("distance routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

te::gm::Geometry* te::gm::Geometry::buffer(const double& distance) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  return buffer(distance, TE_GEOS_DEFAULT_QUADRANT_SEGMENTS, CapRoundType);
#else
  return buffer(distance, 16, CapRoundType);
#endif
}

te::gm::Geometry* te::gm::Geometry::buffer(const double& distance, int quadrantSegments) const throw(std::exception)
{
  return buffer(distance, quadrantSegments, CapRoundType);  
}

te::gm::Geometry* te::gm::Geometry::buffer(const double& distance,
                                           int quadrantSegments,
                                           BufferCapStyle endCapStyle) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  std::auto_ptr<geos::geom::Geometry> g(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> bg(g->buffer(distance, quadrantSegments, static_cast<int>(endCapStyle)));

  bg->setSRID(m_srid);

  return GEOSReader::read(bg.get());


#else
  throw te::common::Exception(TE_TR("buffer routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

te::gm::Geometry* te::gm::Geometry::convexHull() const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  std::auto_ptr<geos::geom::Geometry> g(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> hull(g->convexHull());

  hull->setSRID(m_srid);

  return GEOSReader::read(hull.get());

#else
  throw te::common::Exception(TE_TR("convexHull routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

te::gm::Geometry* te::gm::Geometry::intersection(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  std::auto_ptr<geos::geom::Geometry> intersectionGeom(thisGeom->intersection(rhsGeom.get()));

  intersectionGeom->setSRID(m_srid);

  return GEOSReader::read(intersectionGeom.get());

#else
  throw te::common::Exception(TE_TR("intersection routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

te::gm::Geometry* te::gm::Geometry::Union(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
  
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  std::auto_ptr<geos::geom::Geometry> unionGeom(thisGeom-> Union(rhsGeom.get()));

  unionGeom->setSRID(m_srid);

  return GEOSReader::read(unionGeom.get());

#else
  throw te::common::Exception(TE_TR("Union routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

te::gm::Geometry* te::gm::Geometry::difference(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  std::auto_ptr<geos::geom::Geometry> differenceGeom(thisGeom->difference(rhsGeom.get()));

  differenceGeom->setSRID(m_srid);

  return GEOSReader::read(differenceGeom.get());

#else
  throw te::common::Exception(TE_TR("difference routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

te::gm::Geometry* te::gm::Geometry::symDifference(const Geometry* const rhs) const throw(std::exception)
{
#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  std::auto_ptr<geos::geom::Geometry> symDifferenceGeom(thisGeom->symDifference(rhsGeom.get()));

  symDifferenceGeom->setSRID(m_srid);

  return GEOSReader::read(symDifferenceGeom.get());
#else
  throw te::common::Exception(TE_TR("symDifference routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

bool te::gm::Geometry::dWithin(const Geometry* const rhs, const double& distance) const throw(std::exception)
{
  assert(distance >= 0.0);

#ifdef TERRALIB_GEOS_ENABLED
  if( m_srid != rhs->m_srid )
  {
    throw te::common::Exception(TE_TR("this method must not be used with different SRIDs geometries."));
  }
    
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  std::auto_ptr<geos::geom::Geometry> rhsGeom(GEOSWriter::write(rhs));

  return thisGeom->isWithinDistance(rhsGeom.get(), distance);

#else
  throw te::common::Exception(TE_TR("dWithin routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

te::gm::GeomType te::gm::Geometry::getGeomTypeId(const std::string& gtype)
{
  std::map<std::string, GeomType>::const_iterator it = sm_geomTypeMap.find(gtype);

  if(it != sm_geomTypeMap.end())
    return it->second;

  return te::gm::UnknownGeometryType;
}

bool te::gm::Geometry::isGeomType(const std::string& stype)
{
  return getGeomTypeId(stype) != te::gm::UnknownGeometryType;
}

void te::gm::Geometry::loadGeomTypeId()
{
  if(!sm_geomTypeMap.empty())
    return;

  sm_geomTypeMap["GEOMETRY"] = te::gm::GeometryType;
  sm_geomTypeMap["GEOMETRYZ"] = te::gm::GeometryZType;
  sm_geomTypeMap["GEOMETRYM"] = te::gm::GeometryMType;
  sm_geomTypeMap["GEOMETRYZM"] = te::gm::GeometryZMType;

  sm_geomTypeMap["POINT"] = te::gm::PointType;
  sm_geomTypeMap["POINTM"] = te::gm::PointMType;
  sm_geomTypeMap["POINTZ"] = te::gm::PointZType;
  sm_geomTypeMap["POINTZM"] = te::gm::PointZMType;      

  sm_geomTypeMap["LINESTRING"] = te::gm::LineStringType;
  sm_geomTypeMap["LINESTRINGM"] = te::gm::LineStringMType;
  sm_geomTypeMap["LINESTRINGZ"] = te::gm::LineStringZType;
  sm_geomTypeMap["LINESTRINGZM"] = te::gm::LineStringZMType;
  
  sm_geomTypeMap["POLYGON"] = te::gm::PolygonType;
  sm_geomTypeMap["POLYGONM"] = te::gm::PolygonMType;
  sm_geomTypeMap["POLYGONZ"] = te::gm::PolygonZType;
  sm_geomTypeMap["POLYGONZM"] = te::gm::PolygonZMType;      

  sm_geomTypeMap["MULTIPOINT"] = te::gm::MultiPointType;
  sm_geomTypeMap["MULTIPOINTM"] = te::gm::MultiPointMType;
  sm_geomTypeMap["MULTIPOINTZ"] = te::gm::MultiPointZType;
  sm_geomTypeMap["MULTIPOINTZM"] = te::gm::MultiPointZMType;      

  sm_geomTypeMap["MULTILINESTRING"] = te::gm::MultiLineStringType;
  sm_geomTypeMap["MULTILINESTRINGM"] = te::gm::MultiLineStringMType;
  sm_geomTypeMap["MULTILINESTRINGZ"] = te::gm::MultiLineStringZType;
  sm_geomTypeMap["MULTILINESTRINGZM"] = te::gm::MultiLineStringZMType;

  sm_geomTypeMap["MULTIPOLYGON"] = te::gm::MultiPolygonType;
  sm_geomTypeMap["MULTIPOLYGONM"] = te::gm::MultiPolygonMType;
  sm_geomTypeMap["MULTIPOLYGONZ"] = te::gm::MultiPolygonZType;
  sm_geomTypeMap["MULTIPOLYGONZM"] = te::gm::MultiPolygonZMType;

  sm_geomTypeMap["GEOMETRYCOLLECTION"] = te::gm::GeometryCollectionType;
  sm_geomTypeMap["GEOMETRYCOLLECTIONM"] = te::gm::GeometryCollectionMType;
  sm_geomTypeMap["GEOMETRYCOLLECTIONZ"] = te::gm::GeometryCollectionZType;
  sm_geomTypeMap["GEOMETRYCOLLECTIONZM"] = te::gm::GeometryCollectionZMType;
}

int te::gm::Geometry::getTypeCode() const
{
  return te::dt::GEOMETRY_TYPE;
}

