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
  \file Geometry.cpp
   
  \brief Utility functions to register the Geometry class into Google JavaScript V8 engine.
 */

// TerraLib
#include "../../../../common/Globals.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/Geometry.h"
#include "../../../../geometry/GeometryCollection.h"
#include "../../../../geometry/LineString.h"
#include "../../../../geometry/MultiLineString.h"
#include "../../../../geometry/MultiPoint.h"
#include "../../../../geometry/MultiPolygon.h"
#include "../../../../geometry/Point.h"
#include "../../../../geometry/Polygon.h"
#include "../../common/Utils.h"
#include "../datatype/DataType.h"
#include "Geometry.h"

// STL
#include <cassert>

// Boost
#include <boost/cstdint.hpp>

::v8::Handle<::v8::Value> Geometry_GetDimension(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getDimension method you must use object notation: \"d = obj.getDimension();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getDimension method!"));

  te::gm::Dimensionality d = g->getDimension();

  ::v8::Local<::v8::Integer> jd = ::v8::Integer::New(static_cast<boost::int32_t>(d));

  return hs.Close(jd);
}

::v8::Handle<::v8::Value> Geometry_GetCoordinateDimension(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getCoordinateDimension method you must use object notation: \"d = obj.getCoordinateDimension();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getCoordinateDimension method!"));

  int d = g->getCoordinateDimension();

  ::v8::Local<::v8::Integer> jd = ::v8::Integer::New(d);

  return hs.Close(jd);
}

::v8::Handle<::v8::Value> Geometry_GetGeometryType(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getGeometryType method you must use object notation: \"t = obj.getGeometryType();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getGeometryType method!"));

  const std::string& gt = g->getGeometryType();

  ::v8::Local<::v8::String> jgt = ::v8::String::New(gt.c_str());

  return hs.Close(jgt);
}

::v8::Handle<::v8::Value> Geometry_GetGeomTypeId(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getGeomTypeId method you must use object notation: \"tid = obj.getGeomTypeId();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getGeomTypeId method!"));

  te::gm::GeomType gt = g->getGeomTypeId();

  ::v8::Local<::v8::Integer> jgt = ::v8::Integer::New(static_cast<boost::int32_t>(gt));

  return hs.Close(jgt);
}

::v8::Handle<::v8::Value> Geometry_GetSRID(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getSRID method you must use object notation: \"srid = obj.getSRID();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getSRID method!"));

  int srid = g->getSRID();

  ::v8::Local<::v8::Integer> jsrid = ::v8::Integer::New(static_cast<boost::int32_t>(srid));

  return hs.Close(jsrid);
}

::v8::Handle<::v8::Value> Geometry_SetSRID(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use setSRID method you must use object notation: \"obj.setSRID(4326);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("In order to use setSRID method you must use object notation: \"obj.setSRID(4326);\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in setSRID method!"));

  int srid = args[0]->ToInt32()->Value();
  
  g->setSRID(srid);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> Geometry_Transform(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use transform method you must use object notation: \"obj.transform(4326);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("In order to use transform method you must use object notation: \"obj.transform(4326);\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in transform method!"));

  int srid = args[0]->ToInt32()->Value();
  
  g->transform(srid);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> Geometry_GetEnvelope(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getEnvelope method you must use object notation: \"e = obj.getEnvelope();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getEnvelope method!"));

  std::auto_ptr<te::gm::Geometry> e(g->getEnvelope());
  
  ::v8::Local<::v8::Object> je = te::v8::jsi::Geometry_Make(e.get(), true);

  e.release();

  return hs.Close(je);
}

::v8::Handle<::v8::Value> Geometry_GetMBR(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getMBR method you must use object notation: \"e = obj.getMBR();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getMBR method!"));

  const te::gm::Envelope* e = g->getMBR();
  
  ::v8::Local<::v8::Object> je = te::v8::common::Make(e, te::v8::jsi::GetEnvelopeTemplate, false);

  return hs.Close(je);
}

::v8::Handle<::v8::Value> Geometry_ComputeMBR(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use computeMBR method you must use object notation: \"obj.computeMBR();\""));

  bool cascade = false;

  if((args.Length() == 1) && !args[0].IsEmpty() && args[0]->IsInt32())
    cascade = args[0]->ToBoolean()->Value();

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in computeMBR method!"));

  g->computeMBR(cascade);

  return hs.Close(::v8::Undefined());
}

::v8::Handle<::v8::Value> Geometry_AsText(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use asText method you must use object notation: \"wkt = obj.asText();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in asText method!"));

  std::string wkt = g->asText();

  ::v8::Local<::v8::String> jwkt = ::v8::String::New(wkt.c_str());

  return hs.Close(jwkt);
}

::v8::Handle<::v8::Value> Geometry_AsBinary(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use asBinary method you must use object notation: \"wkb = obj.asBinary();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in asBinary method!"));

  std::size_t size = 0;

  char* wkb = g->asBinary(size);

  ::v8::Local<::v8::String> jwkb = ::v8::String::New(wkb, size);

  delete [] wkb;

  return hs.Close(jwkb);
}

::v8::Handle<::v8::Value> Geometry_GetWkbSize(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getWkbSize method you must use object notation: \"size = obj.getWkbSize();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getWkbSize method!"));

  std::size_t size = g->getWkbSize();

  ::v8::Local<::v8::Integer> jsize = ::v8::Integer::New(static_cast<boost::int32_t>(size));

  return hs.Close(jsize);
}

::v8::Handle<::v8::Value> Geometry_GetWkb(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getWkb method you must use object notation: \"wkb = obj.getWkb(byteorder);\""));

  char byteOrder = te::common::Globals::sm_machineByteOrder;

  if(args.Length() == 1)
  {
    if(args[0].IsEmpty() || !args[0]->IsInt32())
      return ::v8::ThrowException(::v8::String::New("In order to use getWkb method you must use object notation: \"wkb = obj.getWkb(byteorder);\""));

    byteOrder = static_cast<char>(args[0]->ToInt32()->Value());
  }

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getWkb method!"));

  std::size_t size = g->getWkbSize();

  char* wkb = new char[size];
  
  g->getWkb(wkb, byteOrder);

  ::v8::Local<::v8::String> jwkb = ::v8::String::New(wkb, size);

  delete [] wkb;

  return hs.Close(jwkb);
}

::v8::Handle<::v8::Value> Geometry_GetHWkb(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getHWkb method you must use object notation: \"hwkb = obj.getHWkb(byteorder);\""));

  char byteOrder = te::common::Globals::sm_machineByteOrder;

  if(args.Length() == 1)
  {
    if(args[0].IsEmpty() || !args[0]->IsInt32())
      return ::v8::ThrowException(::v8::String::New("In order to use getHWkb method you must use object notation: \"hwkb = obj.getHWkb(byteorder);\""));

    byteOrder = static_cast<char>(args[0]->ToInt32()->Value());
  }

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getHWkb method!"));

  std::size_t size = g->getWkbSize();

  char* wkb = new char[size];
  
  g->getWkb(wkb, byteOrder);

  char* hwkb = te::common::Binary2Hex(wkb, size);

  delete [] wkb;  

  ::v8::Local<::v8::String> jhwkb = ::v8::String::New(hwkb, 2 * size + 1);

  delete [] hwkb;

  return hs.Close(jhwkb);
}

::v8::Handle<::v8::Value> Geometry_IsEmpty(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use isEmpty method you must use object notation: \"empty = obj.isEmpty();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in isEmpty method!"));

  bool is = g->isEmpty();

  ::v8::Handle<::v8::Boolean> jis = ::v8::Boolean::New(is);

  return hs.Close(jis);
}

::v8::Handle<::v8::Value> Geometry_IsSimple(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use isSimple method you must use object notation: \"s = obj.isSimple();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in isSimple method!"));

  bool is = g->isSimple();

  ::v8::Handle<::v8::Boolean> jis = ::v8::Boolean::New(is);

  return hs.Close(jis);
}

::v8::Handle<::v8::Value> Geometry_IsValid(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use isValid method you must use object notation: \"v = obj.isValid();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in isValid method!"));

  bool is = g->isValid();

  ::v8::Handle<::v8::Boolean> jis = ::v8::Boolean::New(is);

  return hs.Close(jis);
}

::v8::Handle<::v8::Value> Geometry_Is3D(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use is3D method you must use object notation: \"v = obj.is3D();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in is3D method!"));

  bool is = g->is3D();

  ::v8::Handle<::v8::Boolean> jis = ::v8::Boolean::New(is);

  return hs.Close(jis);
}

::v8::Handle<::v8::Value> Geometry_IsMeasured(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use isMeasured method you must use object notation: \"v = obj.isMeasured();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in isMeasured method!"));

  bool is = g->isMeasured();

  ::v8::Handle<::v8::Boolean> jis = ::v8::Boolean::New(is);

  return hs.Close(jis);
}

::v8::Handle<::v8::Value> Geometry_GetBoundary(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getBoundary method you must use object notation: \"o = obj.getBoundary();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getBoundary method!"));

  std::auto_ptr<te::gm::Geometry> boundary(g->getBoundary());

  ::v8::Local<::v8::Object> jboundary = te::v8::jsi::Geometry_Make(boundary.get(), true);

  boundary.release();

  return hs.Close(jboundary);
}

::v8::Handle<::v8::Value> Geometry_GetNPoints(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use getNPoints method you must use object notation: \"np = obj.getNPoints();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getNPoints method!"));

  std::size_t np = g->getNPoints();

  ::v8::Handle<::v8::Integer> jnp = ::v8::Integer::New(static_cast<boost::int32_t>(np));

  return hs.Close(jnp);
}

::v8::Handle<::v8::Value> Geometry_Equals(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use equals method you must use object notation: \"rel = obj.equals(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in equals method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in equals method!"));

  bool rel = thisGeom->equals(rhsGeom);

  ::v8::Handle<::v8::Boolean> jrel = ::v8::Boolean::New(rel);

  return hs.Close(jrel);
}

::v8::Handle<::v8::Value> Geometry_Disjoint(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use disjoint method you must use object notation: \"rel = obj.disjoint(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in disjoint method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in disjoint method!"));

  bool rel = thisGeom->disjoint(rhsGeom);

  ::v8::Handle<::v8::Boolean> jrel = ::v8::Boolean::New(rel);

  return hs.Close(jrel);
}

::v8::Handle<::v8::Value> Geometry_Intersects(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use intersects method you must use object notation: \"rel = obj.intersects(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in intersects method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in intersects method!"));

  bool rel = thisGeom->intersects(rhsGeom);

  ::v8::Handle<::v8::Boolean> jrel = ::v8::Boolean::New(rel);

  return hs.Close(jrel);
}

::v8::Handle<::v8::Value> Geometry_Touches(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use touches method you must use object notation: \"rel = obj.touches(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in touches method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in touches method!"));

  bool rel = thisGeom->touches(rhsGeom);

  ::v8::Handle<::v8::Boolean> jrel = ::v8::Boolean::New(rel);

  return hs.Close(jrel);
}

::v8::Handle<::v8::Value> Geometry_Crosses(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use crosses method you must use object notation: \"rel = obj.crosses(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in crosses method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in crosses method!"));

  bool rel = thisGeom->crosses(rhsGeom);

  ::v8::Handle<::v8::Boolean> jrel = ::v8::Boolean::New(rel);

  return hs.Close(jrel);
}

::v8::Handle<::v8::Value> Geometry_Within(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use within method you must use object notation: \"rel = obj.within(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in within method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in within method!"));

  bool rel = thisGeom->within(rhsGeom);

  ::v8::Handle<::v8::Boolean> jrel = ::v8::Boolean::New(rel);

  return hs.Close(jrel);
}

::v8::Handle<::v8::Value> Geometry_Contains(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use contains method you must use object notation: \"rel = obj.contains(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in contains method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in contains method!"));

  bool rel = thisGeom->contains(rhsGeom);

  ::v8::Handle<::v8::Boolean> jrel = ::v8::Boolean::New(rel);

  return hs.Close(jrel);
}

::v8::Handle<::v8::Value> Geometry_Overlaps(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use overlaps method you must use object notation: \"rel = obj.overlaps(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in overlaps method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in overlaps method!"));

  bool rel = thisGeom->overlaps(rhsGeom);

  ::v8::Handle<::v8::Boolean> jrel = ::v8::Boolean::New(rel);

  return hs.Close(jrel);
}

::v8::Handle<::v8::Value> Geometry_Relate(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use relate method you must use object notation: \"rel = obj.relation(another_obj);\""));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(thisGeom == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in relate method!"));

  if(args.Length() == 1)
  {
    if(args[0].IsEmpty() || !args[0]->IsObject())
      return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in relate method!"));

    te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

    if(rhsGeom == 0)
      return ::v8::ThrowException(::v8::String::New("Invalid geometry in relate method!"));

    std::string rel = thisGeom->relate(rhsGeom);

    ::v8::Local<::v8::String> jrel = ::v8::String::New(rel.c_str());

    return hs.Close(jrel);
  }
  else if(args.Length() == 2)
  {
    if(args[0].IsEmpty() || !args[0]->IsObject() || args[1].IsEmpty() || !args[1]->IsString()) 
      return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in relate method!"));

    te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

    if(rhsGeom == 0)
      return ::v8::ThrowException(::v8::String::New("Invalid geometry in relate method!"));

    v8::String::Utf8Value jrel(args[1]->ToString());

    bool isRel = thisGeom->relate(rhsGeom, *jrel);

    ::v8::Handle<::v8::Boolean> jisRel = ::v8::Boolean::New(isRel);

    return hs.Close(jisRel);
  }
  
  return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in relate method!"));
}

::v8::Handle<::v8::Value> Geometry_Covers(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use covers method you must use object notation: \"rel = obj.covers(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in covers method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in covers method!"));

  bool rel = thisGeom->covers(rhsGeom);

  ::v8::Handle<::v8::Boolean> jrel = ::v8::Boolean::New(rel);

  return hs.Close(jrel);
}

::v8::Handle<::v8::Value> Geometry_CoveredBy(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use coveredBy method you must use object notation: \"rel = obj.coveredBy(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in covers method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in coveredBy method!"));

  bool rel = thisGeom->coveredBy(rhsGeom);

  ::v8::Handle<::v8::Boolean> jrel = ::v8::Boolean::New(rel);

  return hs.Close(jrel);
}

//prototype->Set(::v8::String::NewSymbol("locateAlong"), ::v8::FunctionTemplate::New(0));
//prototype->Set(::v8::String::NewSymbol("locateBetween"), ::v8::FunctionTemplate::New(0));

::v8::Handle<::v8::Value> Geometry_Distance(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use distance method you must use object notation: \"dist = obj.distance(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in distance method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in distance method!"));

  double dist = thisGeom->distance(rhsGeom);

  ::v8::Local<::v8::Number> jdist = ::v8::Number::New(dist);

  return hs.Close(jdist);
}

//prototype->Set(::v8::String::NewSymbol("buffer"), ::v8::FunctionTemplate::New(0));

::v8::Handle<::v8::Value> Geometry_ConvexHull(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use convexHull method you must use object notation: \"ch = obj.convexHull();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in getEnvelope method!"));

  std::auto_ptr<te::gm::Geometry> ch(g->convexHull());
  
  ::v8::Local<::v8::Object> jch = te::v8::jsi::Geometry_Make(ch.get(), true);

  ch.release();

  return hs.Close(jch);
}

::v8::Handle<::v8::Value> Geometry_Intersection(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use intersection method you must use object notation: \"newGeom = obj.intersection(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in intersection method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in intersection method!"));

  std::auto_ptr<te::gm::Geometry> newGeom(thisGeom->intersection(rhsGeom));

  ::v8::Local<::v8::Object> jnewGeom = te::v8::jsi::Geometry_Make(newGeom.get(), true);

  newGeom.release();

  return hs.Close(jnewGeom);
}

::v8::Handle<::v8::Value> Geometry_Union(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use union method you must use object notation: \"newGeom = obj.union(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in union method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in union method!"));

  std::auto_ptr<te::gm::Geometry> newGeom(thisGeom->Union(rhsGeom));

  ::v8::Local<::v8::Object> jnewGeom = te::v8::jsi::Geometry_Make(newGeom.get(), true);

  newGeom.release();

  return hs.Close(jnewGeom);
}

::v8::Handle<::v8::Value> Geometry_Difference(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use difference method you must use object notation: \"newGeom = obj.difference(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in difference method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in difference method!"));

  std::auto_ptr<te::gm::Geometry> newGeom(thisGeom->difference(rhsGeom));

  ::v8::Local<::v8::Object> jnewGeom = te::v8::jsi::Geometry_Make(newGeom.get(), true);

  newGeom.release();

  return hs.Close(jnewGeom);
}

::v8::Handle<::v8::Value> Geometry_SymDifference(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use symDifference method you must use object notation: \"newGeom = obj.symDifference(another_obj);\""));

  if((args.Length() != 1) || args[0].IsEmpty() || !args[0]->IsObject())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in symDifference method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in symDifference method!"));

  std::auto_ptr<te::gm::Geometry> newGeom(thisGeom->symDifference(rhsGeom));

  ::v8::Local<::v8::Object> jnewGeom = te::v8::jsi::Geometry_Make(newGeom.get(), true);

  newGeom.release();

  return hs.Close(jnewGeom);
}

::v8::Handle<::v8::Value> Geometry_DWithin(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use dWithin method you must use object notation: \"result = obj.dWithin(another_obj, 10.0);\""));

  if((args.Length() != 2) || args[0].IsEmpty() || !args[0]->IsObject() || args[1].IsEmpty() || !args[1]->IsNumber())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type in dWithin method!"));

  te::gm::Geometry* thisGeom = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  te::gm::Geometry* rhsGeom = te::v8::common::Unwrap<te::gm::Geometry>(args[0]->ToObject());

  if((thisGeom == 0) || (rhsGeom == 0))
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in dWithin method!"));

  double d = args[0]->ToNumber()->Value();

  bool result = thisGeom->dWithin(rhsGeom, d);

  ::v8::Handle<::v8::Boolean> jresult = ::v8::Boolean::New(result);

  return hs.Close(jresult);
}

::v8::Handle<::v8::Value> Geometry_Clone(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if(args.Holder().IsEmpty())
    return ::v8::ThrowException(::v8::String::New("In order to use clone method you must use object notation: \"newGeom = obj.clone();\""));

  te::gm::Geometry* g = te::v8::common::Unwrap<te::gm::Geometry>(args.Holder());

  if(g == 0)
    return ::v8::ThrowException(::v8::String::New("Invalid geometry in clone method!"));

  std::auto_ptr<te::gm::Geometry> ng(g->clone());
  
  ::v8::Local<::v8::Object> jng = te::v8::jsi::Geometry_Make(ng.get(), true);

  ng.release();

  return hs.Close(jng);
}

::v8::Handle<::v8::Value> Geometry_GetGeomFromWKB(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if((args.Length() != 1) || args[0].IsEmpty()    || !args[0]->IsString())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type for getGeomFromWKB method!"));

  v8::String::Utf8Value wkb(args[0]->ToString());

  std::auto_ptr<te::gm::Geometry> geom(te::gm::Geometry::getGeomFromWKB(*wkb));

  ::v8::Local<::v8::Object> jgeom = te::v8::jsi::Geometry_Make(geom.get(), true);

  geom.release();

  return hs.Close(jgeom);
}

::v8::Handle<::v8::Value> Geometry_GetGeomFromHWKB(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if((args.Length() != 1) || args[0].IsEmpty()    || !args[0]->IsString())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type for getGeomFromHWKB method!"));

  v8::String::Utf8Value hwkb(args[0]->ToString());

  std::auto_ptr<te::gm::Geometry> geom(te::gm::Geometry::getGeomFromWKB(*hwkb));

  ::v8::Local<::v8::Object> jgeom = te::v8::jsi::Geometry_Make(geom.get(), true);

  geom.release();

  return hs.Close(jgeom);
}

::v8::Handle<::v8::Value> Geometry_GetGeomFromWKT(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if((args.Length() != 1) || args[0].IsEmpty()    || !args[0]->IsString())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type for getGeomFromWKT method!"));

  v8::String::Utf8Value wkt(args[0]->ToString());

  std::auto_ptr<te::gm::Geometry> geom(te::gm::Geometry::getGeomFromWKT(*wkt));

  ::v8::Local<::v8::Object> jgeom = te::v8::jsi::Geometry_Make(geom.get(), true);

  geom.release();

  return hs.Close(jgeom);
}

::v8::Handle<::v8::Value> Geometry_GetGeomFromEnvelope(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if((args.Length() != 2) || args[0].IsEmpty()  || !args[0]->IsObject() || args[1].IsEmpty() || !args[1]->IsInt32())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type for getGeomFromEnvelope method!"));

  ::v8::Local<::v8::Object> jenvelope = args[0]->ToObject();
  ::v8::Local<::v8::Int32> srid = args[1]->ToInt32();

  te::gm::Envelope* envelope = te::v8::common::Unwrap<te::gm::Envelope>(jenvelope);

  std::auto_ptr<te::gm::Geometry> geom(te::gm::Geometry::getGeomFromEnvelope(envelope, srid->Int32Value()));

  ::v8::Local<::v8::Object> jgeom = te::v8::jsi::Geometry_Make(geom.get(), true);

  geom.release();

  return hs.Close(jgeom);
}

::v8::Handle<::v8::Value> Geometry_StaticGetGeomTypeId(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if((args.Length() != 1) || args[0].IsEmpty()    || !args[0]->IsString())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type for getGeomTypeId method!"));

  v8::String::Utf8Value gt(args[0]->ToString());

  te::gm::GeomType t = te::gm::Geometry::getGeomTypeId(*gt);

  ::v8::Local<::v8::Integer> jt = ::v8::Int32::New(static_cast<int>(t));

  return hs.Close(jt);
}

::v8::Handle<::v8::Value> Geometry_IsGeomType(const ::v8::Arguments& args)
{
  ::v8::HandleScope hs;

  if((args.Length() != 1) || args[0].IsEmpty()    || !args[0]->IsString())
    return ::v8::ThrowException(::v8::String::New("Missing parameter or wrong parameter type for isGeomType method!"));

  v8::String::Utf8Value gt(args[0]->ToString());

  bool is = te::gm::Geometry::isGeomType(*gt);

  ::v8::Handle<::v8::Boolean> jis = ::v8::Boolean::New(is);

  return hs.Close(jis);
}

void te::v8::jsi::RegisterGeometry(::v8::Local<::v8::Object>& global)
{
  ::v8::HandleScope hs;

  ::v8::Local<::v8::ObjectTemplate> jsgeom = ::v8::ObjectTemplate::New();

  jsgeom->Set(::v8::String::NewSymbol("getGeomFromWKB"), ::v8::FunctionTemplate::New(Geometry_GetGeomFromWKB));
  jsgeom->Set(::v8::String::NewSymbol("getGeomFromHWKB"), ::v8::FunctionTemplate::New(Geometry_GetGeomFromHWKB));
  jsgeom->Set(::v8::String::NewSymbol("getGeomFromWKT"), ::v8::FunctionTemplate::New(Geometry_GetGeomFromWKT));
  jsgeom->Set(::v8::String::NewSymbol("getGeomFromEnvelope"), ::v8::FunctionTemplate::New(Geometry_GetGeomFromEnvelope));
  jsgeom->Set(::v8::String::NewSymbol("getGeomTypeId"), ::v8::FunctionTemplate::New(Geometry_StaticGetGeomTypeId));
  jsgeom->Set(::v8::String::NewSymbol("isGeomType"), ::v8::FunctionTemplate::New(Geometry_IsGeomType));

  global->Set(::v8::String::New("TeGeometry"), jsgeom->NewInstance());
}

static ::v8::Persistent<::v8::FunctionTemplate> sg_geometry_template;

::v8::Persistent<::v8::FunctionTemplate>& te::v8::jsi::GetGeometryTemplate()
{
  if(sg_geometry_template.IsEmpty())
  {
    ::v8::Persistent<::v8::FunctionTemplate>& adTpl = GetAbstractDataTemplate();
    ::v8::Local<::v8::FunctionTemplate> result = ::v8::FunctionTemplate::New();
    result->Inherit(adTpl);

    ::v8::Handle<::v8::ObjectTemplate> prototype = result->PrototypeTemplate(); 

    prototype->Set(::v8::String::NewSymbol("getDimension"), ::v8::FunctionTemplate::New(Geometry_GetDimension));
    prototype->Set(::v8::String::NewSymbol("getCoordinateDimension"), ::v8::FunctionTemplate::New(Geometry_GetCoordinateDimension));
    prototype->Set(::v8::String::NewSymbol("getGeometryType"), ::v8::FunctionTemplate::New(Geometry_GetGeometryType));
    prototype->Set(::v8::String::NewSymbol("getGeomTypeId"), ::v8::FunctionTemplate::New(Geometry_GetGeomTypeId));
    prototype->Set(::v8::String::NewSymbol("getSRID"), ::v8::FunctionTemplate::New(Geometry_GetSRID));
    prototype->Set(::v8::String::NewSymbol("setSRID"), ::v8::FunctionTemplate::New(Geometry_SetSRID));
    prototype->Set(::v8::String::NewSymbol("transform"), ::v8::FunctionTemplate::New(Geometry_Transform));
    prototype->Set(::v8::String::NewSymbol("getEnvelope"), ::v8::FunctionTemplate::New(Geometry_GetEnvelope));
    prototype->Set(::v8::String::NewSymbol("getMBR"), ::v8::FunctionTemplate::New(Geometry_GetMBR));
    prototype->Set(::v8::String::NewSymbol("computeMBR"), ::v8::FunctionTemplate::New(Geometry_ComputeMBR));
    prototype->Set(::v8::String::NewSymbol("asText"), ::v8::FunctionTemplate::New(Geometry_AsText));
    prototype->Set(::v8::String::NewSymbol("asBinary"), ::v8::FunctionTemplate::New(Geometry_AsBinary));
    prototype->Set(::v8::String::NewSymbol("getWkbSize"), ::v8::FunctionTemplate::New(Geometry_GetWkbSize));
    prototype->Set(::v8::String::NewSymbol("getWkb"), ::v8::FunctionTemplate::New(Geometry_GetWkb));
    prototype->Set(::v8::String::NewSymbol("getHWkb"), ::v8::FunctionTemplate::New(Geometry_GetHWkb));
    prototype->Set(::v8::String::NewSymbol("isEmpty"), ::v8::FunctionTemplate::New(Geometry_IsEmpty));
    prototype->Set(::v8::String::NewSymbol("isSimple"), ::v8::FunctionTemplate::New(Geometry_IsSimple));
    prototype->Set(::v8::String::NewSymbol("isValid"), ::v8::FunctionTemplate::New(Geometry_IsValid));
    prototype->Set(::v8::String::NewSymbol("is3D"), ::v8::FunctionTemplate::New(Geometry_Is3D));
    prototype->Set(::v8::String::NewSymbol("isMeasured"), ::v8::FunctionTemplate::New(Geometry_IsMeasured));
    prototype->Set(::v8::String::NewSymbol("getBoundary"), ::v8::FunctionTemplate::New(Geometry_GetBoundary));
    prototype->Set(::v8::String::NewSymbol("getNPoints"), ::v8::FunctionTemplate::New(Geometry_GetNPoints));
    prototype->Set(::v8::String::NewSymbol("equals"), ::v8::FunctionTemplate::New(Geometry_Equals));
    prototype->Set(::v8::String::NewSymbol("disjoint"), ::v8::FunctionTemplate::New(Geometry_Disjoint));
    prototype->Set(::v8::String::NewSymbol("intersects"), ::v8::FunctionTemplate::New(Geometry_Intersects));
    prototype->Set(::v8::String::NewSymbol("touches"), ::v8::FunctionTemplate::New(Geometry_Touches));
    prototype->Set(::v8::String::NewSymbol("crosses"), ::v8::FunctionTemplate::New(Geometry_Crosses));
    prototype->Set(::v8::String::NewSymbol("within"), ::v8::FunctionTemplate::New(Geometry_Within));
    prototype->Set(::v8::String::NewSymbol("contains"), ::v8::FunctionTemplate::New(Geometry_Contains));
    prototype->Set(::v8::String::NewSymbol("overlaps"), ::v8::FunctionTemplate::New(Geometry_Overlaps));
    prototype->Set(::v8::String::NewSymbol("relate"), ::v8::FunctionTemplate::New(Geometry_Relate));
    prototype->Set(::v8::String::NewSymbol("covers"), ::v8::FunctionTemplate::New(Geometry_Covers));
    prototype->Set(::v8::String::NewSymbol("coveredBy"), ::v8::FunctionTemplate::New(Geometry_CoveredBy));
    //prototype->Set(::v8::String::NewSymbol("locateAlong"), ::v8::FunctionTemplate::New(0));
    //prototype->Set(::v8::String::NewSymbol("locateBetween"), ::v8::FunctionTemplate::New(0));
    prototype->Set(::v8::String::NewSymbol("distance"), ::v8::FunctionTemplate::New(Geometry_Distance));
    //prototype->Set(::v8::String::NewSymbol("buffer"), ::v8::FunctionTemplate::New(Geometry_Buffer));
    prototype->Set(::v8::String::NewSymbol("convexHull"), ::v8::FunctionTemplate::New(Geometry_ConvexHull));
    prototype->Set(::v8::String::NewSymbol("intersection"), ::v8::FunctionTemplate::New(Geometry_Intersection));
    prototype->Set(::v8::String::NewSymbol("Union"), ::v8::FunctionTemplate::New(Geometry_Union));
    prototype->Set(::v8::String::NewSymbol("difference"), ::v8::FunctionTemplate::New(Geometry_Difference));
    prototype->Set(::v8::String::NewSymbol("symDifference"), ::v8::FunctionTemplate::New(Geometry_SymDifference));
    prototype->Set(::v8::String::NewSymbol("dWithin"), ::v8::FunctionTemplate::New(Geometry_DWithin));
    prototype->Set(::v8::String::NewSymbol("clone"), ::v8::FunctionTemplate::New(Geometry_Clone));

    sg_geometry_template = ::v8::Persistent<::v8::FunctionTemplate>::New(result);
  }

  return sg_geometry_template;
}

::v8::Local<::v8::Object> te::v8::jsi::Geometry_Make(te::gm::Geometry* g, const bool isOwner)
{
  ::v8::HandleScope hs;

  ::v8::Local<::v8::Object> jsgeom;

  switch(g->getGeomTypeId())
  {
    case te::gm::PointType:
    case te::gm::PointZType:
    case te::gm::PointMType:
    case te::gm::PointZMType:
      {
        te::gm::Point* pt = static_cast<te::gm::Point*>(g);
        jsgeom = te::v8::common::Make(pt, GetPointTemplate, isOwner);
        break;
      }

    case te::gm::PolygonType:
    case te::gm::PolygonZType:
    case te::gm::PolygonMType:
    case te::gm::PolygonZMType:
      {
        te::gm::Polygon* poly = static_cast<te::gm::Polygon*>(g);
        jsgeom = te::v8::common::Make(poly, GetPolygonTemplate, isOwner);
        break;
      }

    case te::gm::LineStringType:
    case te::gm::LineStringZType:
    case te::gm::LineStringMType:
    case te::gm::LineStringZMType:
      {
        te::gm::LineString* l = static_cast<te::gm::LineString*>(g);
        jsgeom = te::v8::common::Make(l, GetLineStringTemplate, isOwner);
        break;
      }

    case te::gm::MultiPointType:
    case te::gm::MultiPointZType:
    case te::gm::MultiPointMType:
    case te::gm::MultiPointZMType:
      {
        te::gm::MultiPoint* m = static_cast<te::gm::MultiPoint*>(g);
        jsgeom = te::v8::common::Make(m, GetMultiPointTemplate, isOwner);
        break;
      }

    case te::gm::MultiPolygonType:
    case te::gm::MultiPolygonZType:
    case te::gm::MultiPolygonMType:
    case te::gm::MultiPolygonZMType:
      {
        te::gm::MultiPolygon* m = static_cast<te::gm::MultiPolygon*>(g);
        jsgeom = te::v8::common::Make(m, GetMultiPolygonTemplate, isOwner);
        break;
      }

    case te::gm::MultiLineStringType:
    case te::gm::MultiLineStringZType:
    case te::gm::MultiLineStringMType:
    case te::gm::MultiLineStringZMType:
      {
        te::gm::MultiLineString* m = static_cast<te::gm::MultiLineString*>(g);
        jsgeom = te::v8::common::Make(m, GetMultiLineStringTemplate, isOwner);
        break;
      }

    case te::gm::GeometryCollectionType:
    case te::gm::GeometryCollectionZType:
    case te::gm::GeometryCollectionMType:
    case te::gm::GeometryCollectionZMType:
      {
        te::gm::GeometryCollection* gc = static_cast<te::gm::GeometryCollection*>(g);
        jsgeom = te::v8::common::Make(gc, GetGeometryCollectionTemplate, isOwner);
        break;
      }

    /*case TriangleType:
    case TriangleZType:
    case TriangleMType:
    case TriangleZMType:
      {
        break;
      }

    case TINType:
    case TINZType:
    case TINMType:
    case TINZMType:
      {
        break;
      }

    case PolyhedralSurfaceType:
    case PolyhedralSurfaceZType:
    case PolyhedralSurfaceMType:
    case PolyhedralSurfaceZMType:
      {
        break;
      }*/

    default:
        ::v8::ThrowException(::v8::String::New("The informed geometry type is unknown!"));
  }

  return hs.Close(jsgeom);
}


