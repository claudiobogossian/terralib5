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
  \file terralib/geometry/Geometry.h

  \brief Geometry is the root class of the geometries hierarchy, it follows OGC and ISO standards.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GEOMETRY_H
#define __TERRALIB_GEOMETRY_INTERNAL_GEOMETRY_H

// TerraLib
#include "../common/BaseVisitable.h"
#include "../common/Enums.h"
#include "../datatype/AbstractData.h"
#include "Enums.h"
#include "Exception.h"
#include "Visitor.h"

// STL
#include <exception>
#include <map>
#include <string>

// Boost
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace gm
  {
// Forward declarations
    class Envelope;

    /*!
      \class Geometry
      
      \brief Geometry is the root class of the geometries hierarchy, it follows OGC and ISO standards.
      
      The geometry model follows OGC Simple Feature
      Specification - SFS (Simple Feature Access - Part 1: Common Architecture)
      and ISO SQL/MM Spatial.

      \ingroup geometry

      \sa AbstractPoint, Point, PointM, PointZ, PointZM, PointKd,
          Curve, LineString, LinearRing, Line, CircularString, CompoundCurve,
          Surface, CurvePolygon, Polygon, Triangle, PolyhedralSurface, TIN,
          GeometryCollection, MultiCurve, MultiSurface,
          MultiPoint, MultiLineString, MultiPolygon

      \note Methods marked as <b>extended</b> are not from OGC or ISO specifications, but they enhances the TerraLib API.
      
      \note Methods performed by another library are marked as <b>Performed by Library-Name</b>.
    */
    class TEGEOMEXPORT Geometry : public te::dt::AbstractData, public te::common::BaseVisitable<Visitor>
    {
      public:        

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the Geometry with the specified spatial reference system id and envelope.
       
          \param t    The internal geometry type.
          \param srid The Spatial Reference System ID associated to the geometry.
          \param mbr  The envelope (minimum bounding rectangle of this geometry).

          \note The geometry will take the ownership of the given mbr.
        */
        Geometry(GeomType t, int srid = -1, Envelope* mbr = 0) throw();

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        Geometry(const Geometry& rhs) throw();

        /*! \brief Virtual destructor. */
        virtual ~Geometry();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side geometry.

          \return A reference for this.
        */
        virtual Geometry& operator=(const Geometry& rhs) throw();

        //@}

        /** @name Basic Geometry Methods
         *  Basic methods on geometric objects.
         */
        //@{

        /*!
          \brief It returns the geometric object dimension (0, 1 or 2).

          For a GeometryCollection it returns the largest topological
          dimension of the contained objects. For Point and MultiPoint this will return 0.
          For Curves and MultiCurves (LineString, LinearRing and MultiLineString) it will
          return 1. For Surfaces (Polygon, Triangle, PolyhedralSurface, TIN) and
          MultiSurfaces (MultiPolygon) it will return 2.

          \return The geometric object dimension (0, 1 or 2).
        */
        virtual Dimensionality getDimension() const throw() = 0;

        /*!
          \brief It returns the number of measurements or axes needed to describe a position in a coordinate system.

          It returns:
          <ul>
          <li>2 for a coordinate with x, y;</li>
          <li>3 for a coordinate with x, y and z or x, y and m;</li>
          <li>4 for a coordinate with x, y, z and m.</li>
          </ul>

          \return The number of measurements or axes needed to describe a position in a coordinate system.

          \note This is NOT the same as getDimension() method!
        */
        int getCoordinateDimension() const throw();

        /*!
          \brief It returns the name of the geometry subclass.

          The name of the geometry subclass may be one of the following:
          <ul>
          <li>Point</li>
          <li>LineString</li>
          <li>Polygon</li>
          <li>GeometryCollection</li>
          <li>MultiPoint</li>
          <li>MultiLineString</li>
          <li>MultiPolygon</li>
          <li>Tin</li>
          </ul>

          \return The name of the geometry subclass type ide.
        */
        virtual const std::string& getGeometryType() const throw() = 0;

        /*!
          \brief It returns the geometry subclass type identifier.          

          \return The geometry subclass type identifier

          \note Please, see GeomType enumeration for possible return values.

          \note TerraLib extended method.
        */
        GeomType getGeomTypeId() const throw() { return m_gType; }

        /*!
          \brief It returns the Spatial Reference System ID associated to this geometric object.
 
          This value can be used to identify the associated Spatial Reference System.

          \return The Spatial Reference System ID associated to this geometric object.

          \note When not set this value will be -1.
        */
        int getSRID() const throw() { return m_srid; }

        /*!
          \brief It sets the Spatial Reference System ID of the geometry and all its parts if it is a GeometryCollection (or a Multi).

          \param srid The Spatial Reference System ID to be associated to the geometric object.

          \note This method just set the srid, it doesn't perform conversions over coordinate values.

          \note TerraLib extended method.
        */
        virtual void setSRID(int srid) throw() = 0;

        /*!
          \brief It converts the coordinate values of the geometry to the new spatial reference system.

          After calling this method the geometry will be associated to the new SRID.

          \param srid The new Spatial Reference System ID used to transform the coordinates of the geometry.

          \exception Exception It will throw an exception if it can not do the transformation.

          \note The geometry must be associated to a valid SRID before calling this method.

          \note If the geometry already has an associated MBR, this method will automatically update it (i. e. automatically recompute it).
        */
        virtual void transform(int srid) throw(te::common::Exception) = 0;

        /*!
          \brief It returns the minimum bounding rectangle (MBR) for the geometry.

          As one can notice, the mbr is returned as a geometry, actually a polygon
          defined by the corner points of the bounding box [(MINX, MINY), (MAXX, MINY), (MAXX, MAXY), (MINX, MAXY), (MINX, MINY)].

          \return The geometry envelope (or mbr).

          \note The caller of this method will take the ownership of the returned geometry.

          \note If the MBR was not computed previously, it will compute it. Successive
                calls to this method will not compute the mbr anymore.
        */
        Geometry* getEnvelope() const throw();

        /*!
          \brief It returns the minimum bounding rectangle for the geometry in an internal representation.

          The mbr can be constructed when reading a geometry from a database or
          it can be computed internally. So, if the mbr is not already set
          it will compute it just when this method is called. Successive
          calls to this method will not compute the mbr anymore.

          \return The envelope of this geometry (i.e., the minimum bounding rectangle).          

          \note It is supposed to be faster and more useful than getting the box as a polygon geometry.

          \note TerraLib extended method.
        */
        const Envelope* getMBR() const throw();

        /*!
          \brief It computes the minimum bounding rectangle for the geometry.

          Subclasses must implement this method in order
          to be able to compute the minimum bounding box of 
          a specific geometry.

          \param cascade If true it will update the MBR for its sub-geometries.

          \note You can use this method in order to update the MBR of the Geometry.

          \note TerraLib extended method.
        */
        virtual void computeMBR(bool cascade) const throw() = 0;

        /*!
          \brief It returns an string with the Well-Known Text Representation for the geometry.

          \return The WKT for the Geometry.
        */
        std::string asText() const throw();

        /*!
          \brief It serializes the geometric object to a Well-known Binary Representation (WKB).

          \param size The size in bytes of the returned WKB.
  
          \return The WKB representation for this object.

          \exception Exception It will throw an exception if the operation could not be performed.

          \note The WKB will be on machine byte order.

          \note The caller of this method will take the ownership of the returned wkb.
                You must use "delete [] pointer" in order to free the memory pointed by returned pointer.
        */
        char* asBinary(std::size_t& size) const throw(Exception);

        /*!
          \brief It returns the size required by a WKB representation for this geometric object.

          This is the preferred method for creating a WKB. First of all,
          it gives you the possibility to use a pre-allocated buffer. So,
          this method can be used in conjunction with the getWkb method.

          \return The size required by a WKB representation for the geometry object.

          \note TerraLib extended method.
        */
        std::size_t getWkbSize() const throw();

        /*!
          \brief It serializes the geometry to a WKB representation into the specified buffer.

          The wkb parameter must have at least getWkbSize() in order to be used. Don't
          pass a NULL pointer or a buffer smaller than the size needed. Note that
          the WKB will be on the specified byte order.

          \param wkb       The buffer where the Geometry will be serialized.
          \param byteOrder The byte order used to store/serialize the geometry.

          \exception Exception It will throw an exception if the operation could not be performed.

          \note TerraLib extended method.
        */
        void getWkb(char* wkb, te::common::MachineByteOrder byteOrder) const throw(Exception);

        /*!
          \brief It returns true if this geometric object is the empty Geometry.
 
          If true, then this geometric object represents the empty point set
          for the coordinate space.

          \return True if this geometric object is the empty Geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \note Performed by GEOS.
        */
        virtual bool isEmpty() const throw(std::exception);

        /*!
          \brief It returns true if this geometric object has no anomalous points, such as self intersection or self tangency.

          See the ISO and OGC documentation for an explanation about specific conditions
          of each type of geometry to be considered not simple.
          
          \return True if this geometric object has no anomalous geometric points.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \note Performed by GEOS.
        */
        virtual bool isSimple() const throw(std::exception);

        /*!
          \brief It tells if the geometry is well formed.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \note TerraLib extended method.

          \note Performed by GEOS.
        */
        virtual bool isValid() const throw(std::exception);

        /*!
          \brief It returns true if this geometric object has z coordinate values.

          \return True if this geometric object has z coordinate values.
        */
        bool is3D() const throw();

        /*!
          \brief It returns true if this geometric object has m coordinate values.

          \return True if this geometric object has m coordinate values.
        */
        bool isMeasured() const throw();

        /*!
          \brief It returns the geometry boundary.

          \return The geometry that makes the boundary of this geometry. The caller of this method will
                  take the ownership of the returned geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \note The caller of this method will take the ownership of the returned Geometry.

          \note Performed by GEOS.
        */
        virtual Geometry* getBoundary() const throw(std::exception);

        /*!
          \brief it returns the number of points (vertexes) in the geometry.

          \return The number of points (vertexes) in the geometry.

          \note TerraLib extended method.
        */
        virtual std::size_t getNPoints() const throw() = 0;

        //@}

        /** @name Spatial Relations
         *  Methods for testing spatial relations between geometric objects.
         *  Please, see OGC specification for a in depth definition of each spatial operation.
         */
        //@{

        /*!
          \brief It returns true if the geometry object is spatially equal to rhs geometry.

          \param rhs   The another geometry to be compared.
          \param exact If true checks if this geometric object has the same vertexes in the same order of rhs geometry.

          \return True if the geometry is spatially equal to the other geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise an exception will be thrown.

          \note Performed by GEOS.
        */
        virtual bool equals(const Geometry* const rhs, const bool exact = false) const throw(std::exception);

        /*!
          \brief It returns true if the geometry object is spatially disjoint from rhs geometry.
 
          \param rhs The other geometry to be compared.

          \return True if the geometry is spatially disjoint from the other geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note Performed by GEOS.
        */
        virtual bool disjoint(const Geometry* const rhs) const throw(std::exception);

        /*!
          \brief It returns true if the geometry object spatially intersects rhs geometry.

          \param rhs The other geometry to be compared.

          \return True if the geometry intersects the other geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note Performed by GEOS.
        */
        virtual bool intersects(const Geometry* const rhs) const throw(std::exception);

        /*!
          \brief It returns true if the geometry object spatially touches rhs geometry.

          \param rhs The other geometry to be compared.

          \return True if the geometry spatially touches the other geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note Performed by GEOS.
        */
        virtual bool touches(const Geometry* const rhs) const throw(std::exception);

        /*!
          \brief It returns true if the geometry object spatially crosses rhs geometry.

          \param rhs The other geometry to be compared.

          \return True if the geometry spatially crosses the other geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note Performed by GEOS.
        */
        virtual bool crosses(const Geometry* const rhs) const throw(std::exception);

        /*!
          \brief It returns true if the geometry object is spatially within rhs geometry.

          \param rhs The other geometry to be compared.

          \return True if the geometry is spatially within the other geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note Performed by GEOS.
        */
        virtual bool within(const Geometry* const rhs) const throw(std::exception);

        /*!
          \brief It returns true if this geometry object spatially contains rhs geometry.

          \param rhs The other geometry to be compared.

          \return True if the geometry spatially contains the other geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note Performed by GEOS.
        */
        virtual bool contains(const Geometry* const rhs) const throw(std::exception);

        /*!
          \brief It returns true if this geometry object spatially overlaps rhs geometry.

          \param rhs The other geometry to be compared.

          \return True if the geometry spatially overlaps the other geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note Performed by GEOS.
        */
        virtual bool overlaps(const Geometry* const rhs) const throw(std::exception);

        /*!
          \brief It returns true if this geometry object is spatially related to rhs geometry according to the pattern expressed by the intersection matrix.

          It does this by testing for intersections between the interior,
          boundary and exterior of the two geometric objects as specified
          by the values in the matrix. 

          \param rhs    The other geometry to be compared.
          \param matrix The intersection matrix.

          \return True if the geometry is spatially related to the other geometry according to the pattern expressed by the intersection matrix.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note Performed by GEOS.
        */
        virtual bool relate(const Geometry* const rhs, const std::string& matrix) const throw(std::exception);

        /*!
          \brief It returns the spatial relation between this geometry object and the rhs geometry.
 
          \param rhs The another geometry to be compared.

          \return A string where each byte is a intersection in the pattern intersection matrix of the relationship of the two objects.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note TerraLib extended method.

          \note This method will be handy when you don't know the spatial relation in advance.

          \note Performed by GEOS.
        */
        virtual std::string relate(const Geometry* const rhs) const throw(std::exception);

        /*!
          \brief It returns true if this geometry object spatially covers the rhs geometry.

          \param rhs The other geometry to be compared.

          \return True if the geometry spatially covers the other geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note TerraLib extended method.

          \note This is not the same as contains. See Max Egenhofer paper on 9-intersection matrix.

          \note Performed by GEOS.
        */
        virtual bool covers(const Geometry* const rhs) const throw(std::exception);
        
        /*!          
          \brief It returns true if this geometry object is spatially covered by rhs geometry.
 
          \param rhs The other geometry to be compared.

          \return True if the geometry is spatially covered by the other geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note TerraLib extended method.

          \note This is not the same as within. See Max Egenhofer paper on 9-intersection matrix.

          \note Performed by GEOS.
        */
        virtual bool coveredBy(const Geometry* const rhs) const throw(std::exception);
   
        /*!
          \brief It returns a derived GeometryCollection value according to the specified coordinate value.

          \param mValue The coordinate value.

          \return A GeometryCollection value.

          \exception Exception It will throw an exception if the operation could not be performed.

          \note The caller of this method will take the ownership of the returned geometry.

          \note This method only applies to Point and Line geometries, including homogeneu collections of points or lines.
                For polygons this will return a NULL value.
        */
        virtual Geometry* locateAlong(const double& mValue) const throw(Exception) { return locateBetween(mValue, mValue); };

        /*!
          \brief It returns a derived geometry collection value according to the range of coordinate values inclusively.

          \param mStart The initial coordinate value.
          \param mEnd   The final coordinate value.

          \return A GeometryCollection value.

          \exception Exception It will throw an exception if the operation could not be performed.

          \note This method only applies to Point and Line geometries, including homogeneous collections of points or lines.
                For polygons this will return a NULL value.

          \note The caller of this method will take the ownership of the returned Geometry.
        */
        virtual Geometry* locateBetween(const double& mStart, const double& mEnd) const throw(Exception);

        //@}

        /** @name Spatial Analysis
         *  Methods that support spatial analysis. 
         */
        //@{

        /*!
          \brief It returns the shortest distance between any two points in the two geometry objects.

          \param rhs The other geometry.

          \return The shortest distance between any two points in the two geometries.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \note Performed by GEOS.
        */
        virtual double distance(const Geometry* const rhs) const throw(std::exception);       
      
        /*!
          \brief This method calculates the buffer of a geometry.

          \param distance Distance value.

          \return A geometry representing all points less than or equal to the specified distance.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \note The caller of this method will take the ownership of the returned geometry.

          \note Performed by GEOS.
        */
        virtual Geometry* buffer(const double& distance) const throw(std::exception);

        /*!
          \brief This method calculates the buffer of a geometry.

          \param distance         Distance value.
          \param quadrantSegments A specified number of segments used to approximate the curves.

          \return A geometry representing all points less than or equal to the specified distance.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \note The caller of this method will take the ownership of the returned geometry.

          \note TerraLib extended method.

          \note Performed by GEOS.
        */
        virtual Geometry* buffer(const double& distance, int quadrantSegments) const throw(std::exception);

        /*!
          \brief This method calculates the buffer of a geometry.

          As in GEOS, the quadrantSegments argument allows controlling the
          accuracy of the approximation by specifying the number of line
          segments used to represent a quadrant of a circle.

          \param distance         Distance value.
          \param quadrantSegments A specified number of segments used to approximate the curves.
          \param endCapStyle      It specifies the shape used at the ends of linestrings.

          \return A geometry representing all points less than or equal to the specified distance.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \note The caller of this method will take the ownership of the returned Geometry.

          \note TerraLib extended method.

          \note Performed by GEOS.
        */
        virtual Geometry* buffer(const double& distance,
                                 int quadrantSegments,
                                 BufferCapStyle endCapStyle) const throw(std::exception);

        /*!
          \brief This method calculates the Convex Hull of a geometry.

          \return A geometry representing the convex hull.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \note The caller of this method will take the ownership of the returned geometry.

          \note Performed by GEOS.
        */
        virtual Geometry* convexHull() const throw(std::exception);

        /*!
          \brief It returns a geometric object that represents the point set intersection with another geometry

          \param rhs The other Geometry whose intersection with this Geometry will be calculated.

          \return A Geometry representing the intersection with this Geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note The caller of this method will take the ownership of the returned Geometry.

          \note Performed by GEOS.
        */
        virtual Geometry* intersection(const Geometry* const rhs) const throw(std::exception);

        /*!
          \brief It returns a geometric object that represents the point set union with another geometry

          \param rhs Another geometry whose union with this geometry will be calculated.

          \return A geometry representing the union with this geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note The caller of this method will take the ownership of the returned Geometry.

          \note Performed by GEOS.
        */
        virtual Geometry* Union(const Geometry* const rhs) const throw(std::exception);       

        /*!
          \brief It returns a geometric object that represents the point set difference with another geometry

          \param rhs Another geometry whose difference with this geometry will be calculated.

          \return A geometry representing the difference between the geometries.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note The caller of this method will take the ownership of the returned Geometry.

          \note Performed by GEOS.
        */
        virtual Geometry* difference(const Geometry* const rhs) const throw(std::exception);

        /*!
          \brief It returns a geometric object that represents the point set symetric difference with another geometry

          \param rhs The other geometry whose symetric difference with this geometry will be calculated.

          \return A geometry representing the symetric difference with this geometry.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \warning Don't call this method for a Heterogeneous GeometryCollection, otherwise, an exception will be thrown.

          \note The caller of this method will take the ownership of the returned Geometry.

          \note Performed by GEOS.
        */
        virtual Geometry* symDifference(const Geometry* const rhs) const throw(std::exception);

        /*!
          \brief It returns true if the geometries are within the specified distance.

          \param rhs      The other geometry whose symetric difference with this geometry will be calculated.
          \param distance The distance.

          \return True if the geometries are within the specified distance.

          \exception std::exception It will throw an exception if the operation could not be performed.

          \note TerraLib extended method.

          \note Performed by GEOS.
        */
        virtual bool dWithin(const Geometry* const rhs, const double& distance) const throw(std::exception);

        //@}

        /** @name Auxiliary Methods
         *  Auxiliary Methods.
         */
        //@{

        /*!
          \brief It returns the TerraLib geometry type id given a type string (the type string must be in capital letters).

          \param stype The geometry type name.

          \return The geometry type id equivalent to the string name.

          \note If the type is unknow it returns UnknownGeometryType.
        */
        static GeomType getGeomTypeId(const std::string& gtype);

        /*!
          \brief It tells if the given string is a geometry data type.

          \param stype The geometry type to be checked.

          \return True if the given string corresponds to a geometry type.
        */
        static bool isGeomType(const std::string& stype);

        /*!
          \brief It loads the internal MAP of geometry type names to geometry type ids.

          \warning Ths method will be automatically called when geometry module is initialized!
        */
        static void loadGeomTypeId();

        //@}

        /** @name AbstractData Re-implementation
         *  Methods re-implemneted from AbstractData.
         */
        //@{

        /*!
          \brief It returns the data type code associated to the data value.

          \return The data type code associated to the data value.
        */
        int getTypeCode() const;

        /*!
          \brief It returns the data value in a WKT representation.

          \return The data value in a WKT representation.
        */
        std::string toString() const { return asText(); }

        //@}

      protected:

        GeomType m_gType;         //!< Internal geometry type.
        int m_srid;               //!< The Spatial Reference System code associated to the Geometry.
        mutable Envelope* m_mbr;  //!< The geometry minimum bounding rectangle.

        static std::map<std::string, GeomType> sm_geomTypeMap;  //!< A set of geometry type names (in UPPER CASE).
    };
	
	//Typedef 
    typedef boost::shared_ptr<Geometry>       GeometryShrPtr;
  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOMETRY_H

