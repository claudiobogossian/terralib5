/*
 * File: Geometry.i
 *
 */

#define TEGEOMEXPORT

%import "terralib/common/BaseVisitable.h"

%ignore accept;

#ifdef SWIGPYTHON
%apply SWIGTYPE *DISOWN { te::gm::Curve* ring };
%apply SWIGTYPE *DISOWN { te::gm::Geometry* g };
#endif

%{
#include "terralib/geometry/Config.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/geometry/Enums.h"
#include "terralib/geometry/AbstractPoint.h"
#include "terralib/geometry/CircularString.h"
#include "terralib/geometry/CompoundCurve.h"
#include "terralib/geometry/Coord2D.h"
#include "terralib/geometry/Curve.h"
#include "terralib/geometry/CurvePolygon.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/geometry/Geometry.h"
#include "terralib/geometry/Line.h"
#include "terralib/geometry/LinearRing.h"
#include "terralib/geometry/LineString.h"
#include "terralib/geometry/MultiCurve.h"
#include "terralib/geometry/MultiLineString.h"
#include "terralib/geometry/MultiPoint.h"
#include "terralib/geometry/MultiPolygon.h"
#include "terralib/geometry/MultiSurface.h"
#include "terralib/geometry/Point.h"
#include "terralib/geometry/PointKd.h"
#include "terralib/geometry/PointM.h"
#include "terralib/geometry/PointZ.h"
#include "terralib/geometry/PointZM.h"
#include "terralib/geometry/Polygon.h"
#include "terralib/geometry/PolyhedralSurface.h"
#include "terralib/geometry/Surface.h"

%}

//Python doesn't work with namespaces
#ifndef SWIGPYTHON

%nspace te::gm::GeomType;
%nspace te::gm::SpatialRelation;
%nspace te::gm::Dimensionality;
%nspace te::gm::BufferCapStyle;
%nspace te::gm::Geometry;
%nspace te::gm::Coord2D;
%nspace te::gm::AbstractPoint;
%nspace te::gm::Point;
%nspace te::gm::Envelope;
%nspace te::gm::Curve;
%nspace te::gm::CircularString;
%nspace te::gm::CompoundCurve;
%nspace te::gm::Envelope;
%nspace te::gm::LineString;
%nspace te::gm::Line;
%nspace te::gm::LinearRing;
%nspace te::gm::Surface;
%nspace te::gm::CurvePolygon;
%nspace te::gm::GeometryCollection;
%nspace te::gm::MultiCurve;
%nspace te::gm::MultiLineString;
%nspace te::gm::MultiPoint;
%nspace te::gm::MultiSurface;
%nspace te::gm::MultiPolygon;
%nspace te::gm::PointKd;
%nspace te::gm::PointM;
%nspace te::gm::PointZ;
%nspace te::gm::PointZM;
%nspace te::gm::Polygon;
%nspace te::gm::PolyhedralSurface;

#endif // SWIGPYTHON

%include "terralib/geometry/Enums.h"
%include "terralib/geometry/Geometry.h"
%include "terralib/geometry/Coord2D.h"
%include "terralib/geometry/AbstractPoint.h"
%include "terralib/geometry/Point.h"
%include "terralib/geometry/Envelope.h"
%include "terralib/geometry/Curve.h"
%include "terralib/geometry/CircularString.h"
%include "terralib/geometry/CompoundCurve.h"
%include "terralib/geometry/Envelope.h"
%include "terralib/geometry/LineString.h"
%include "terralib/geometry/Line.h"
%include "terralib/geometry/LinearRing.h"
%include "terralib/geometry/Surface.h"
%include "terralib/geometry/CurvePolygon.h"
%include "terralib/geometry/GeometryCollection.h"
%include "terralib/geometry/MultiCurve.h"
%include "terralib/geometry/MultiLineString.h"
%include "terralib/geometry/MultiPoint.h"
%include "terralib/geometry/MultiSurface.h"
%include "terralib/geometry/MultiPolygon.h"
%include "terralib/geometry/PointKd.h"
%include "terralib/geometry/PointM.h"
%include "terralib/geometry/PointZ.h"
%include "terralib/geometry/PointZM.h"
%include "terralib/geometry/Polygon.h"
%include "terralib/geometry/PolyhedralSurface.h"