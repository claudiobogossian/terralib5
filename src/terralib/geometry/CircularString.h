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
  \file terralib/geometry/CircularString.h

  \brief CircularString is a curve with circular interpolation between points.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_CIRCULARSTRING_H
#define __TERRALIB_GEOMETRY_INTERNAL_CIRCULARSTRING_H

// TerraLib
#include "Coord2D.h"
#include "Curve.h"

// STL
#include <vector>

namespace te
{
  namespace gm
  {
    /*!
      \class CircularString

      \brief CircularString is a curve with circular interpolation between points.

      The first fragment of a circular string is defined by three points.

      \note A circular string must have an odd number of points greated than 1.

      \ingroup geometry

      \sa Geometry,
          AbstractPoint, Point, PointM, PointZ, PointZM, PointKd,
          Curve, Line, LineString, LinearRing, CompoundCurve,
          Surface, Polygon, Triangle, CurvePolygon, PolyhedralSurface, TIN,
          GeometryCollection, MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon
    */
    class TEGEOMEXPORT CircularString : public Curve
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the circularstring with the specified spatial reference system id and envelope.
       
          \param t    The internal type of the circularstring.
          \param srid The Spatial Reference System ID associated to the circularstring.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The circularstring will take the ownership of the given mbr.
        */
        CircularString(GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief It initializes the circularstring with the specified spatial reference system id and envelope.
       
          \param size The number of points in the CircularString. It must be a value greater than 0.
          \param t    The internal type of the circularstring.
          \param srid The Spatial Reference System ID associated to the circularstring.
          \param mbr  The minimum bounding rectangle of this geometry (i.e., its envelope).

          \note The circularstring will take the ownership of the given mbr.
        */
        CircularString(std::size_t size, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        CircularString(const CircularString& rhs);

        /*! \brief Virtual destructor. */
        ~CircularString();

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        CircularString& operator=(const CircularString& rhs);

        //@}

        /** @name Re-Implementation from AbstractData
         *  Methods re-Implementated from AbstractData.
         */
        //@{

        /*!
          \brief It clones the circularstring.

          \return A copy of the given circularstring.

          \note The caller of this method will take the ownership of the returned circularstring.

          \note The cloned circularstring will not have the
                MBR computed. This will save time when you
                are just cloning a geometry and don't intend
                to waste time computing the bounding box.
                If you have another suggestion, please, let me know.
        */
        te::dt::AbstractData* clone() const;

        //@}

        /** @name Re-Implmentation of methods from Geometry class
         *  Re-Implmentation of basic methods from Geometry class.
         */
        //@{

        /*!
          \brief The name of instantiable subtype is: CircularString.

          \return The name of instantiable subtype is: CircularString.
        */
        const std::string& getGeometryType() const throw();

        /*!
          \brief It sets the Spatial Reference System ID of the circularstring.

          \param srid The Spatial Reference System ID to be associated to the circularstring.

          \note TerraLib extended method.
        */
        void setSRID(int srid) throw();

        /*!
          \brief It converts the coordinate values of the circularstring to the new spatial reference system.

          After calling this method the circularstring will be associated to the new SRID.

          \param srid The new Spatial Reference System ID used to transform the coordinates of the circularstring.

          \exception Exception It will throw an exception if it can not do the transformation.

          \note The circularstring must be associated to a valid SRID before calling this method.

          \note If the circularstring already has an associated MBR, this method will automatically update it (i. e. automatically recompute it).
        */
        void transform(int srid) throw(te::common::Exception);

        /*!
          \brief It computes the minimum bounding rectangle for the circularstring.

          \param cascade For circularstring this flag doesn't have effect.

          \note You can use this method in order to update the MBR of the circularstring.

          \note TerraLib extended method.
        */
        void computeMBR(bool cascade) const throw();

        /*!
          \brief It returns the number of points (vertexes) in the circularstring.

          \return The number of points (vertexes) in the circularstring.

          \note TerraLib extended method.
        */
        std::size_t getNPoints() const throw() { return m_coords.size(); }

        /*!
          \brief It returns a derived geometry collection value according to the range of coordinate values inclusively.

          \param mStart The initial coordinate value.
          \param mEnd   The final coordinate value.

          \return A GeometryCollection value.

          \note The caller of this method will take the ownership of geometry.
        */
        Geometry* locateBetween(const double& mStart, const double& mEnd) const throw(Exception);

        //@}

        /** @name Re-Implementation from Curve
         *  Methods re-impleented from Curve.
         */
        //@{

        /*!
          \brief The length of this Curve in its associated spatial reference.

          \return The length of this Curve in its associated spatial reference.
        */
        double getLength() const;

        /*!
          \brief It returns the curve start point.

          \return The curve start point.

          \note The caller of this method will take the ownership of the point geometry.
        */
        Point* getStartPoint() const;

        /*!
          \brief It returns the curve end point.

          \return The curve end point.

          \note The caller of this method will take the ownership of the point geometry.
        */
        Point* getEndPoint() const;

        /*!
          \brief It returns true if the curve is closed (startPoint = endPoint).

          \return True if the curve is closed (startPoint = endPoint).

          \warning The line must have at least 2 points.
        */
        bool isClosed() const;

        //@}

        /** @name CircularString Specific Methods
         *  Specific methods for a CircularString.
         */
        //@{

        /*!
          \brief It returns the number of points (vertexes) in the geometry.

          \return The number of points (vertexes) in the geometry.

          \note TerraLib extended method.
        */
        std::size_t size() const { return m_coords.size(); }

        /*!
          \brief It reserves room for the number of coordinates in this CircularString.

          \param size The number of coordinates reserved for the CircularString.

          \note TerraLib extended method.
        */
        void setNumCoordinates(std::size_t size);

        /*!
          \brief It clears all the coordinates.

          After calling this method you can not use the set methods.
          You will need to use the setNumCoordinates method in order
          to use the set methods.

          \note TerraLib extended method.
        */
        void makeEmpty();

        /*!
          \brief It returns the specified point in this CircularString.

          \param i The index of the point you want to retrieve.

          \return The specified point in this CircularString.

          \note The caller of this method will take the ownership of the point geometry.

          \note This method doesn't check the index range.
        */
        Point* getPointN(std::size_t i) const;

        /*!
          \brief It sets the value of the specified point to this new one.

          \param i The index where the point will be set to the new value.
          \param p The new point value.

          \note TerraLib extended method.
        */
        void setPointN(std::size_t i, const Point& p);

        /*!
          \brief It sets the value of the specified point.

          \param i The index where the point will be set to the new value.
          \param x The x coordinate value.
          \param y The y coordinate value.

          \note TerraLib extended method.
        */
        void setPoint(std::size_t i, const double& x, const double& y);

        /*!
          \brief It sets the value of the specified point.

          \param i The index where the point will be set to the new value.
          \param x The x coordinate value.
          \param y The y coordinate value.
          \param z The z coordinate value.

          \note TerraLib extended method.
        */
        void setPointZ(std::size_t i, const double& x, const double& y, const double& z);

        /*!
          \brief It sets the value of the specified point.

          \param i The index where the point will be set to the new value.
          \param x The x coordinate value.
          \param y The y coordinate value.
          \param m The m coordinate value.

          \note TerraLib extended method.
        */
        void setPointM(std::size_t i, const double& x, const double& y, const double& m);

        /*!
          \brief It sets the value of the specified point.

          \param i The index where the point will be set to the new value.
          \param x The x coordinate value.
          \param y The y coordinate value.
          \param z The z coordinate value.
          \param m The m coordinate value.

          \note TerraLib extended method.
        */
        void setPointZM(std::size_t i, const double& x, const double& y, const double& z, const double& m);

        /*!
          \brief It returns the n-th x coordinate value.

          \param i The n-th coordinate.

          \return The n-th x coordinate value.

          \note TerraLib extended method.
        */
        const double& getX(std::size_t i) const;

        /*!
          \brief It returns the n-th y coordinate value.

          \param i The n-th coordinate.

          \return The n-th y coordinate value.

          \note TerraLib extended method.
        */
        const double& getY(std::size_t i) const;

        /*!
          \brief It returns the n-th z coordinate value.

          \param i The n-th coordinate.

          \return The n-th z coordinate value.

          \note Don't call this method if the CircularString doesn't have Z coordinate values.

          \note TerraLib extended method.
        */
        const double& getZ(std::size_t i) const;

        /*!
          \brief It returns the n-th m measure value.

          \param i The n-th coordinate.

          \return The n-th m measure value.

          \note Don't call this method if the CircularString doesn't have M coordinate values.

          \note TerraLib extended method.
        */
        const double& getM(std::size_t i) const;

        /*!
          \brief It sets the n-th x coordinate value.

          \param i The coordinate index.
          \param x The x value.

          \note TerraLib extended method.
        */
        void setX(std::size_t i, const double& x);
        
        /*!
          \brief It sets the n-th y coordinate value.

          \param i The coordinate index.
          \param y The y value.

          \note TerraLib extended method.
        */
        void setY(std::size_t i, const double& y);

        /*!
          \brief It sets the n-th z coordinate value.

          \param i The coordinate index.
          \param z The z value.

          \note Don't call this method if the CircularString doesn't have Z coordinate values.

          \note TerraLib extended method.
        */
        void setZ(std::size_t i, const double& z);

        /*!
          \brief It sets the n-th m measure value.

          \param i The coordinate index.
          \param m The m value.

          \note Don't call this method if the CircularString doesn't have M coordinate values.

          \note TerraLib extended method.
        */
        void setM(std::size_t i, const double& m);

        /*!
          \brief It returns a pointer to the internal array of coordinates.

          \return A pointer to the internal array of coordinates.

          \warning Don't use this method unless you know exactly what you're doing!

          \note TerraLib extended method.
        */
        const std::vector<Coord2D>& getCoordinates() const
        {
          return m_coords;
        }

        /*!
          \brief It returns a pointer to the internal array of coordinates.

          \return A pointer to the internal array of coordinates.

          \warning Don't use this method unless you know exactly what you're doing!

          \note TerraLib extended method.
        */
        std::vector<Coord2D>& getCoordinates()
        {
          return m_coords;
        }

        /*!
          \brief It returns a pointer to the internal array of z-values.

          \return A pointer to the internal array of z-values.

          \warning Don't use this method unless you know exactly what you're doing!

          \note TerraLib extended method.
        */
        const std::vector<double>& getZ() const
        {
          return m_zA;
        }

        /*!
          \brief It returns a pointer to the internal array of z-values.

          \return A pointer to the internal array of z-values.

          \warning Don't use this method unless you know exactly what you're doing!

          \note TerraLib extended method.
        */
        std::vector<double>& getZ()
        {
          return m_zA;
        }

        /*!
          \brief It returns a pointer to the internal array of m-values.

          \return A pointer to the internal array of m-values.

          \warning Don't use this method unless you know exactly what you're doing!

          \note TerraLib extended method.
        */
        const std::vector<double>& getM() const
        {
          return m_mA;
        }

        /*!
          \brief It returns a pointer to the internal array of m-values.

          \return A pointer to the internal array of m-values.

          \warning Don't use this method unless you know exactly what you're doing!

          \note TerraLib extended method.
        */
        std::vector<double>& getM()
        {
          return m_mA;
        }

        //@}

      protected:
        
        std::vector<Coord2D> m_coords;   //!< A pointer to x, y values.
        std::vector<double> m_zA;        //!< A pointer to z values.
        std::vector<double> m_mA;        //!< A pointer to m values.

      private:

        static const std::string sm_typeName; //! Geometry type name for CircularString.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_CIRCULARSTRING_H

