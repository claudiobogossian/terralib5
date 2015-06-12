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
  \file Geometry.h
   
  \brief JavaScript exporting routine for the TerraLib Geometry module.
 */

#ifndef __TERRALIB_BINDING_V8_JSI_GEOMETRY_INTERNAL_GEOMETRY_H
#define __TERRALIB_BINDING_V8_JSI_GEOMETRY_INTERNAL_GEOMETRY_H

// TerraLib
#include "../Config.h"

// Google V8
#include <v8.h>

namespace te
{
// Forward declaration
  namespace gm { class Geometry; }

  namespace v8
  {
    namespace jsi
    {
      /*!
        \brief It register the Coord2D class.

        You can use the Coord2D class like:
        \code
        var c = new TeCoord2D(1.0, 2.0);
        \endcode       

        \param global The global object that will be used to register the Coord2D class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterCoord2D(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a Coord2D object.

        \return A reference to the persistent template of a Coord2D object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetCoord2DTemplate();

      /*!
        \brief It register the Envelope class.

        You can use the Envelope class like:
        \code
        var e = new TeEnvelope(1.0, 1.0, 5.0, 5.0);
        \endcode       

        \param global The global object that will be used to register the Envelope class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterEnvelope(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a Envelope object.

        \return A reference to the persistent template of a Envelope object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetEnvelopeTemplate();

      /*!
        \brief It registers the GeometryFactory class.

        You can use the GeometryFactory class as a singleton using the following syntax:
        \code
        var mygeom = TeGeometryFactory.make(TE_OGC_POINT, srid);
        \endcode       

        \param global The global object that will be used to register the GeometryFactory class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterGeometryFactory(::v8::Local<::v8::Object>& global);

      /*!
        \brief It register a singleton for the Geometry class.

        You can use the GeometryFactory class like:
        \code
        var mygeom = TeGeometry.getGeomFromWKB(wkb);
        \endcode       

        \param global The global object that will be used to register the Geometry class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterGeometry(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a Geometry object.

        \return A reference to the persistent template of a Geometry object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetGeometryTemplate();

      /*!
        \brief Given a C++ geometry this function creates a new JavaScript geometry cast to the right geometry subtype.

        \param g The C++ geometry object. The caller will loose the ownership of the given geometry.

        \return A JavaScript geometry object.
       */
      ::v8::Local<::v8::Object> Geometry_Make(te::gm::Geometry* g, const bool isOwner);

      /*!
        \brief It register the Point class.

        You can use the Point class like:
        \code
        var mypt = new TePoint(1.0, 2.0, 4326);
        \endcode       

        \param global The global object that will be used to register the Point class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterPoint(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a Point object.

        \return A reference to the persistent template of a Point object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetPointTemplate();

      /*!
        \brief It returns a reference to the persistent template of a Surface object.

        \return A reference to the persistent template of a Surface object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetSurfaceTemplate();

      /*!
        \brief It register the Polygon class.

        You can use the Polygon class like:
        \code
        var mypoly = new TePolygon(2, TE_OGC_POLYGON, 4326);
        \endcode

        \param global The global object that will be used to register the Polygon class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterPolygon(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a Polygon object.

        \return A reference to the persistent template of a Polygon object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetPolygonTemplate();

      /*!
        \brief It returns a reference to the persistent template of a Curve object.

        \return A reference to the persistent template of a Curve object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetCurveTemplate();

      /*!
        \brief It register the LineString class.

        You can use the LineString class like:
        \code
        var mygc = new TeLineString(2, TE_OGC_LINESTRING, 4326);
        \endcode

        \param global The global object that will be used to register the LineString class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterLineString(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a LineString object.

        \return A reference to the persistent template of a LineString object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetLineStringTemplate();

      /*!
        \brief It register the LinearRing class.

        You can use the LinearRing class like:
        \code
        var mygc = new LinearRing(2, TE_OGC_LINESTRING, 4326);
        \endcode

        \param global The global object that will be used to register the LinearRing class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterLinearRing(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a LinearRing object.

        \return A reference to the persistent template of a LinearRing object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetLinearRingTemplate();

      /*!
        \brief It register the GeometryCollection class.

        You can use the GeometryCollection class like:
        \code
        var mygc = new TeGeometryCollection(2, TE_OGC_GEOMETRYCOLLECTION, 4326);
        \endcode

        \param global The global object that will be used to register the GeometryCollection class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterGeometryCollection(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a  GeometryCollection object.

        \return A reference to the persistent template of a  GeometryCollection object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetGeometryCollectionTemplate();

      /*!
        \brief It returns a reference to the persistent template of a  MultiSurface object.

        \return A reference to the persistent template of a MultiSurface object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetMultiSurfaceTemplate();

      /*!
        \brief It returns a reference to the persistent template of a  MultiCurve object.

        \return A reference to the persistent template of a MultiCurve object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetMultiCurveTemplate();

      /*!
        \brief It register the MultiPoint class.

        You can use the MultiPoint class like:
        \code
        var mympt = new TeMultiPoint(2, TE_OGC_MULTIPOINT, 4326);
        \endcode

        \param global The global object that will be used to register the Point class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterMultiPoint(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a  MultiPoint object.

        \return A reference to the persistent template of a MultiPoint object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetMultiPointTemplate();

      /*!
        \brief It register the MultiLineString class.

        You can use the MultiLineString class like:
        \code
        var mygc = new TeMultiLineString(2, TE_OGC_MULTIINESTRING, 4326);
        \endcode

        \param global The global object that will be used to register the MultiLineString class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterMultiLineString(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a  MultiLineString object.

        \return A reference to the persistent template of a MultiLineString object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetMultiLineStringTemplate();

      /*!
        \brief It register the MultiPolygon class.

        You can use the MultiPolygon class like:
        \code
        var mygc = new TeMultiPolygon(2, TE_OGC_MULTIPOLYGON, 4326);
        \endcode

        \param global The global object that will be used to register the MultiPolygon class

        \exception Exception It may throws an exception if it is not possible to register the class.
       */
      void RegisterMultiPolygon(::v8::Local<::v8::Object>& global);

      /*!
        \brief It returns a reference to the persistent template of a  MultiPolygon object.

        \return A reference to the persistent template of a MultiPolygon object.
       */
      ::v8::Persistent<::v8::FunctionTemplate>& GetMultiPolygonTemplate();


    } // end namespace jsi
  }   // end namespace v8
}     // end namespace te

#endif  // __TERRALIB_BINDING_V8_JSI_GEOMETRY_INTERNAL_GEOMETRY_H

