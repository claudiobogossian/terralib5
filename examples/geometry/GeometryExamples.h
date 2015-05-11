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
  \file GeometryExamples.h

  \brief A set of geometry examples.
 */

#ifndef __TERRALIB_EXAMPLES_GEOMETRY_INTERNAL_GEOMETRYEXAMPLES_H
#define __TERRALIB_EXAMPLES_GEOMETRY_INTERNAL_GEOMETRYEXAMPLES_H

// TerraLib
#include <terralib/geometry.h>

/** @name Geometry constructors methods.
		Methods used to create differentes types of geometries.
	*/
//@{

/*
  \brief It creates a Point 2D at (x, y).

  \param x The x coordinate
  \param y The y coordinate

  \return A pointer to the created geometry point.
 */
te::gm::Point* createPoint(const double& x, const double& y);

/*
  \brief It creates a Line 2D with initial coordinate at (xi, yi) and final coordinate (xf,yf).

  \param xi The initial point x coordinate
	\param yi The initial point y coordinate
  \param xf The final point x coordinate
	\param yf The final point y coordinate

  \return A pointer to the created geometry line.
 */
te::gm::LineString* createLineString(const double& xi, const double& yi,
																		 const double& xf, const double& yf);

/*
  \brief It creates a Linear Ring that represents a square.

	\param xc   The center point x coordinate
	\param yc   The center point y coordinate
  \param size The size of square side.

  \return A pointer to the created geometry linear ring.
 */
te::gm::LinearRing* createSquare(const double& xc, const double& yc, const double& size);

/*
  \brief It creates a Polygon that represents a square.

  \return A pointer to the created geometry polygon.
 */
te::gm::Polygon* createPolygon();

/*
  \brief It creates a Polygon that represents a square with hole.

  \return A pointer to the created geometry polygon.
 */
te::gm::Polygon* createPolygonWithHole();

/*
  \brief It creates a Geometry Collection that contains a point, a line, a polygon and a polygon with hole.

  \return A pointer to the created geometry collection.
 */
te::gm::GeometryCollection* createGeometryCollection();

//@}

/** @name Spatial relations methods.
		Method used to calculate the spatial relation between geometries.
	*/
//@{

/*
  \brief This method calculates the spatial relation between two geometries.

	\param g1				The first geometry
	\param g2				The second geometry
  \param relation The spatial relation that will be evaluated.

  \return ture if the spatial relation is satisfied or false otherwise.
 */

bool spatialRelation(te::gm::Geometry* g1, te::gm::Geometry* g2, te::gm::SpatialRelation relation);

//@}

/** @name Set operations methods.
		Method used to execute set operations between geometries.
	*/
//@{

/*
  \brief This method executes the diferrence between two geometries.

	\param g1 The first geometry
	\param g2 The second geometry

  \return a geometry representing the diferrence.
 */
te::gm::Geometry* difference(te::gm::Geometry* g1, te::gm::Geometry* g2);

/*
  \brief This method executes the intersection between two geometries.

	\param g1 The first geometry
	\param g2 The second geometry

  \return a geometry representing the intersection.
 */
te::gm::Geometry* intersection(te::gm::Geometry* g1, te::gm::Geometry* g2);

/*
  \brief This method executes the Union between two geometries.

	\param g1 The first geometry
	\param g2 The second geometry

  \return a geometry representing the union.
 */
te::gm::Geometry* Union(te::gm::Geometry* g1, te::gm::Geometry* g2);

//@}

/** @name Measurement operations methods.
		Methods used to measurement some properties of geometries.
	*/
//@{

	// methods goes here

//@}

/** @name Wkt / Wkb manipulation methods.
		Example methods that deal with Wkt and Wkb for geometries.
	*/
//@{

/*
  \brief Read a txt file that contains a set of Wkt geometries and convert each to the appropriate Geometry object.

	\param filePath The path to txt file that contains a set of Wkt geometries.
 */
void readWkts(const std::string& filePath);

//@}

#endif  // __TERRALIB_EXAMPLES_GEOMETRY_INTERNAL_GEOMETRYEXAMPLES_H
