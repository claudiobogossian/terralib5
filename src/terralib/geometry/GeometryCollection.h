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
  \file terralib/geometry/GeometryCollection.h

  \brief It is a collection of other geometric objects.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYCOLLECTION_H
#define __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYCOLLECTION_H

// TerraLib
#include "Geometry.h"

// STL
#include <vector>

namespace te
{
  namespace gm
  {
    /*!
      \class GeometryCollection

      \brief It is a collection of other geometric objects.

      \ingroup geometry

      \sa Geometry,
          Point,
          Curve, LineString, LinearRing,
          Surface, Polygon, Triangle, PolyhedralSurface, TIN,
          MultiSurface, MultiCurve,
          MultiPoint, MultiLineString, MultiPolygon

      \note All the elements shall be in the same Spatial Reference System as
            the geometry collection. Although the class will not perform this check it
            is very important to use it to build valid geometries.
    */
    class TEGEOMEXPORT GeometryCollection : public Geometry
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer methods on geometric objects
         *  Methods for initializing a geometric object.
         */
        //@{

        /*!
          \brief It initializes the geometry collection with the specified spatial reference system id and envelope.
       
          \param nGeom The number of geometries in the collection. This will reserve same space in the list of geometries.
          \param t     The internal type of the Geometry.
          \param srid  The Spatial Reference System ID associated to the Geometry.
          \param mbr   The minimum bounding rectangle of this geometry (i.e., its envelope). May be a NULL value.

          \note The geometry collection will take the ownership of the given mbr.

          \note When using this constructor you must set all geometries before calling any other method (like getMBR).
        */
        GeometryCollection(std::size_t nGeom, GeomType t, int srid = 0, Envelope* mbr = 0);

        /*!
          \brief Copy constructor.

          \param rhs The other geometry.
        */
        GeometryCollection(const GeometryCollection& rhs);

        /*!
          \brief Assignment operator.

          \param rhs The other geometry.

          \return A reference for this.
        */
        GeometryCollection& operator=(const GeometryCollection& rhs);

        /*! \brief Virtual destructor. */
        virtual ~GeometryCollection();

        //@}

        /** @name Re-Implementation from AbstractData
         *  Methods re-Implementated from AbstractData.
         */
        //@{

        /*!
          \brief It clones the geometry collection.

          \return A copy of the given geometry collection.

          \note The caller of this method will take the ownership of the returned geometry collection.

          \note The cloned geometry collection will not have the
                MBR computed. This will save time when you
                are just cloning a geometry and don't intend
                to waste time computing the bounding box.
                If you have another suggestion, please, let me know.
        */
        virtual te::dt::AbstractData* clone() const;

        //@}

        /** @name Re-Implmentation of methods from Geometry class
         *  Re-Implmentation of basic methods from Geometry class.
         */
        //@{

        /*!
          \brief For non-homogeneous collections this method will return the largest dimension of the contained objects.

          \return The largest dimension of the contained objects.
        */
        virtual Dimensionality getDimension() const throw();

        /*!
          \brief The name of the Geometry subtype is: GeometryCollection.

          \return The name of the Geometry subtype is: GeometryCollection.
         */
        virtual const std::string& getGeometryType() const throw();

        /*!
          \brief It sets the Spatial Reference System ID of the geometry collection and all its parts.

          \param srid The Spatial Reference System ID to be associated to the geometric object.

          \note TerraLib extended method.
        */
        void setSRID(int srid) throw();

        /*!
          \brief It will transform the coordinates of the geometry collection to the new one.

          After calling this method the geometry collection will be associated to the new SRID.

          \param srid The new Spatial Reference System ID used to transform the coordinates of the geometry collection.

          \exception Exception It will throw an exception if it can not do the transformation.

          \note The geometry collection must be associated to a valid SRID before calling this method.

          \note If the geometry collection already has an associated MBR, it will automatically update it (i. e. automatically recompute it).

          \note TerraLib extended method.
        */
        void transform(int srid) throw(te::common::Exception);

        /*!
          \brief It computes the minimum bounding rectangle for the geometry collection.

          \param cascade If true, it will update the MBR of its parts.

          \note You can use this method in order to update the MBR of the geometry collection.

          \note TerraLib extended method.
        */
        void computeMBR(bool cascade) const throw();

        /*!
          \brief it returns the number of points (vertexes) in the geometry.

          \return The number of points (vertexes) in the geometry.

          \note TerraLib extended method.
        */
        std::size_t getNPoints() const throw();

        //@}


        /** @name GeometryCollection Specific Methods
         *  Specific methods for a GeometryCollection.
         */
        //@{

        /*!
          \brief It returns a reference to the internal list of geometries.

          \return A reference to the internal list of geometries.

          \note This method can be used by database drivers to implement its own geometry wrappers!

          \note TerraLib extended method.
        */
        const std::vector<Geometry*>& getGeometries() const { return m_geometries; }

        /*!
          \brief It returns a reference to the internal list of geometries.

          \return A reference to the internal list of geometries.

          \note This method can be used by database drivers to implement its own geometry wrappers!

          \note TerraLib extended method.
        */
        std::vector<Geometry*>& getGeometries() { return m_geometries; }

        /*!
          \brief It returns the number of geometries in this GeometryCollection.

          \return The number of geometries in this GeometryCollection.

          \note If you are using this method for a loop, try to create a temporary
                "const size_t variable" to keep this value; this can optimize your loop.
        */
        std::size_t getNumGeometries() const { return m_geometries.size(); }

        /*!
          \brief It sets the number of geometries in this GeometryCollection.

          \param size The new number of geometries for the GeometryCollection.

          \note If size is smaller than the current GeometryCollection size,
                the content is reduced to its first size elements, the rest being just
                dropped (the deallocation of geometries is done automatically).

          \note If size is greater than the current GeometryCollection size,
                the content is expanded by inserting at the end NULL pointers to geometries
                as needed.

          \note You must assure that all nGeom geometries are set before calling any other method (like getMBR).

          \note TerraLib extended method.
        */
        void setNumGeometries(std::size_t size);

        /*!
          \brief It returns the n-th geometry in this GeometryCollection.

          \param i The index of the given geometry element.

          \return The geometry in the indicated position.

          \note It doesn't check the index range.
        */
        Geometry* getGeometryN(std::size_t i) const;

        /*!
          \brief It returns the n-th geometry in this GeometryCollection.

          \param i The index of the given geometry element.

          \note It doesn't check the index range.
        */
        Geometry* getGeometryN(std::size_t i);

        /*!
          \brief It sets the n-th geometry in this geometry collection.

          GeometryCollection will take the ownership of the informed geometry.
          If there is a geometry in the given position it will be released before stting the new one.

          \param i The index of the given geometry element.
          \param g The geometry that will be placed on the position indicated by index.

          \note It doesn't check the index range.

          \note Geometry collection will take the ownership of the informed geometry.

          \note TerraLib extended method.
        */
        void setGeometryN(std::size_t i, Geometry* g);

        /*!
          \brief It removes the n-th geometry in this geometry collection.

          \param i The index of the geometry element we want to remove.

          \note It doesn't check the index range.

          \note TerraLib extended method.
        */
        void removeGeometryN(std::size_t i);

        /*!
          \brief It adds the geometry into the collection.

          GeometryCollection will take the ownership of the informed geometry.

          \param g The geometry you want to add to the collection.

          \note TerraLib extended method.
        */
        void add(Geometry* g);

        /*!
          \brief It deletes all the elements of the collection.

          After calling this method you can NOT use the setGeometryN method. You will need to call setNumGeometries() again or use add().

          \note TerraLib extended method.
        */
        void clear();

        //@}

      protected:

        std::vector<Geometry*> m_geometries;   //!< The array of geometries that forms the collection.

      private:
        
        static const std::string sm_typeName;     //!< Geometry type name for GeometryCollection.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOMETRYCOLLECTION_H

