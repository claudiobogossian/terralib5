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
  \file terralib/geometry/WKTParser.h

  \brief A class that implements the grammar rules for well known text (WKT) for Geometry.
         It is based on boost::spirit V2 and WKT BNF definition available in the OGC Simple Features Specification.

  \warning Do not use this class. It is for TerraLib internal use. Try the te::gm::WKTReader instead.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_WKTPARSER_H
#define __TERRALIB_GEOMETRY_INTERNAL_WKTPARSER_H

// TerraLib
#include "WKTActions.h"

// boost
#include <boost/bind/bind.hpp>
#include <boost/spirit/include/qi.hpp>

// STL
#include <iostream>
#include <string>

// Auxiliaries namespaces
namespace qi    = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

// Using the following boost::spirit definitions
using qi::char_;
using qi::double_;
using ascii::no_case;

namespace te
{
  namespace gm
  {
    /*!
      \class Parser
      
      \brief A class that implements the Grammar Rules for well known text (WKT) format of Geometry.
     */
    template<typename Iterator>
    class WKTParser : public qi::grammar<Iterator, ascii::space_type>
    {
      public:

        /** @name Constructor
         *  Initilizer methods.
         */
        //@{

        /*! \brief Default constructor. */
        WKTParser() : WKTParser::base_type(geometry)
        {
          // Geometries (root)
          geometry = (geometryZMTagged | geometryMTagged | geometryZTagged | geometryTagged)[boost::bind(&WKTActions::end, &m_a)];
            
          // Initializing all rules
          initGeometries();
          initGeometriesZ();
          initGeometriesM();
          initGeometriesZM();
          initCoordinates();
          initPoint();
          initLine();
          initPolygon();
          initMultiPoint();
          initMultiLine();
          initMultiPolygon();
          initPolyhedralSurface();
          initTIN();
          initTriangle();
          initGeometryCollection();

          // Empty key word
          empty = no_case["empty"] >> !char_;
        }

        //@}

      private:
        
        /** @name Initializer methods.
          *  Methods to initialize the grammar rules.
          */
        //@{
          
        /*! 
          \brief Initializes geometry tagged rules.
         */
        void initGeometries()
        {
          // Geometries Tags
          geometryTagged = (pointTagged
                          | lineStringTagged
                          |  polygonTagged
                          | polyhedralSurfaceTagged
                          |  multiPointTagged
                          |  multiLineStringTagged 
                          |  multiPolygonTagged
                          |  tinTagged
                          | triangleTagged
                          |  geometryCollectionTagged)[boost::bind(&WKTActions::endGeometry, &m_a)]
                          ;
        }

        /*! 
          \brief Initializes geometries z tagged rules.
         */
        void initGeometriesZ()
        {
          // Geometries Z Tags
          geometryZTagged = (pointZTagged
                          | lineStringZTagged
                          |  polygonZTagged
                          | polyhedralSurfaceZTagged
                          |  multiPointZTagged
                          |  multiLineStringZTagged 
                          |  multiPolygonZTagged
                          |  tinZTagged
                          | triangleZTagged
                          |  geometryCollectionZTagged)[boost::bind(&WKTActions::endGeometry, &m_a)]
                          ;
        }

        /*! 
          \brief Initializes geometries m tagged rules.
         */
        void initGeometriesM()
        {
          // Geometries M Tags
          geometryMTagged = (pointMTagged
                          | lineStringMTagged
                          |  polygonMTagged
                          | polyhedralSurfaceMTagged
                          |  multiPointMTagged
                          |  multiLineStringMTagged 
                          |  multiPolygonMTagged
                          |  tinMTagged
                          | triangleMTagged
                          |  geometryCollectionMTagged)[boost::bind(&WKTActions::endGeometry, &m_a)]
                          ;
        }

        /*! 
          \brief Initializes geometries zm tagged rules.
         */
        void initGeometriesZM()
        {
          // Geometries ZM Tags
          geometryZMTagged = (pointZMTagged
                          | lineStringZMTagged
                          |  polygonZMTagged
                          | polyhedralSurfaceZMTagged
                          |  multiPointZMTagged
                          |  multiLineStringZMTagged 
                          |  multiPolygonZMTagged
                          |  tinZMTagged
                          | triangleZMTagged
                          |  geometryCollectionZMTagged)[boost::bind(&WKTActions::endGeometry, &m_a)]
                          ;
        }

        /*! 
          \brief Initializes coordinates rules.
         */
        void initCoordinates()
        {
          // (x,y)
          coordinate = (double_ >> double_)[boost::bind(&WKTActions::createPoint, &m_a, ::_1)]; // It is an action!
          // (x,y,z)
          coordinateZ = (double_ >> double_ >> double_)[boost::bind(&WKTActions::createPointZ, &m_a, ::_1)];
          // (x,y,m)
          coordinateM = (double_ >> double_ >> double_)[boost::bind(&WKTActions::createPointM, &m_a, ::_1)];
          // (x,y,z,m)
          coordinateZM = (double_ >> double_ >> double_ >> double_)[boost::bind(&WKTActions::createPointZM, &m_a, ::_1)];
        }

        /*! 
          \brief Initializes point rules.
         */
        void initPoint()
        {
          // Point Tagged Text
          pointTagged = no_case["point"] >> pointTxt;
          // Point Z Tagged
          pointZTagged = no_case["point z"] >> pointZTxt;
          // Point M Tagged
          pointMTagged = no_case["point m"] >> pointMTxt;
          // Point ZM Tagged
          pointZMTagged = no_case["point zm"] >> pointZMTxt;

          // Point
          pointTxt = empty[boost::bind(&WKTActions::createPoint, &m_a)] | '(' >> coordinate >> ')';
          // Point Z
          pointZTxt =  empty[boost::bind(&WKTActions::createPointZ, &m_a)] | '(' >> coordinateZ >> ')';
          // Point M
          pointMTxt =  empty[boost::bind(&WKTActions::createPointM, &m_a)] | '(' >> coordinateM >> ')';
          // Point ZM
          pointZMTxt = empty[boost::bind(&WKTActions::createPointZM, &m_a)] | '(' >> coordinateZM >> ')';
        }

        /*! 
          \brief Initializes line rules.
         */
        void initLine()
        {
          // LineString Tagged Text
          lineStringTagged = no_case["linestring"] >> lineStringTxt;
          // LineString Z Tagged Text
          lineStringZTagged =  no_case["linestring z"] >> lineStringZTxt;
          // LineString M Tagged Text
          lineStringMTagged =  no_case["linestring m"] >> lineStringMTxt;
          // LineString ZM Tagged Text
          lineStringZMTagged = no_case["linestring zm"] >> lineStringZMTxt;

          // LineString
          lineStringTxt =  (empty | '(' >> coordinate >>  *(',' >> coordinate) >>  ')')[boost::bind(&WKTActions::createLine, &m_a)];
          // LineString Z
          lineStringZTxt = (empty | '(' >> coordinateZ >>  *(',' >> coordinateZ) >> ')')[boost::bind(&WKTActions::createLineZ, &m_a)];
          // LineString M
          lineStringMTxt = (empty | '(' >> coordinateM >>  *(',' >> coordinateM) >> ')')[boost::bind(&WKTActions::createLineM, &m_a)];
          // LineString ZM
          lineStringZMTxt =  (empty | '(' >> coordinateZM >>  *(',' >> coordinateZM) >>  ')')[boost::bind(&WKTActions::createLineZM, &m_a)];

          // LinearRing
          linearRingTxt =  (empty | '(' >> coordinate >>  *(',' >> coordinate) >>  ')')[boost::bind(&WKTActions::createLinearRing, &m_a)];
          // LineString Z
          linearRingZTxt = (empty | '(' >> coordinateZ >>  *(',' >> coordinateZ) >>  ')')[boost::bind(&WKTActions::createLinearRingZ, &m_a)];
          // LineString M
          linearRingMTxt = (empty | '(' >> coordinateM >>  *(',' >> coordinateM) >>  ')')[boost::bind(&WKTActions::createLinearRingM, &m_a)];
          // LineString ZM
          linearRingZMTxt =  (empty | '(' >> coordinateZM >>  *(',' >> coordinateZM) >>  ')')[boost::bind(&WKTActions::createLinearRingZM, &m_a)];
        }

        /*! 
          \brief Initializes polygon rules.
         */
        void initPolygon()
        {
          // Polygon Tagged Text
          polygonTagged = no_case["polygon"] >> polygonTxt;
          // Polygon Z Tagged Text
          polygonZTagged = no_case["polygon z"] >> polygonZTxt;
          // Polygon M Tagged Text
          polygonMTagged = no_case["polygon m"] >> polygonMTxt;
          // Polygon ZM Tagged Text
          polygonZMTagged = no_case["polygon zm"] >> polygonZMTxt;

          // Polygon
          polygonTxt = (empty | '(' >> linearRingTxt >> *(',' >> linearRingTxt) >> ')')[boost::bind(&WKTActions::createPolygon, &m_a)];
          // Polygon Z
          polygonZTxt = (empty | '(' >> linearRingZTxt >> *(',' >> linearRingZTxt) >> ')')[boost::bind(&WKTActions::createPolygonZ, &m_a)];
          // Polygon M
          polygonMTxt = (empty | '(' >> linearRingMTxt >> *(',' >> linearRingMTxt) >> ')')[boost::bind(&WKTActions::createPolygonM, &m_a)];
          // Polygon ZM
          polygonZMTxt = (empty | '(' >> linearRingZMTxt >> *(',' >> linearRingZMTxt) >> ')')[boost::bind(&WKTActions::createPolygonZM, &m_a)];
        }

        /*! 
          \brief Initializes multi point rules.
         */
        void initMultiPoint()
        {
          // MultiPoint Tagged Text
          multiPointTagged = (no_case["multipoint"] >> multiPointTxt)[boost::bind(&WKTActions::createMultiPoint, &m_a)];
          // MultiPoint Z Tagged Text
          multiPointZTagged =  (no_case["multipoint z"] >> multiPointZTxt)[boost::bind(&WKTActions::createMultiPointZ, &m_a)];
          // MultiPoint M Tagged Text
          multiPointMTagged =  (no_case["multipoint m"] >> multiPointMTxt)[boost::bind(&WKTActions::createMultiPointM, &m_a)];
          // MultiPoint ZM Tagged Text
          multiPointZMTagged = (no_case["multipoint zm"] >> multiPointZMTxt)[boost::bind(&WKTActions::createMultiPointZM, &m_a)];

          // MultiPoint
          multiPointTxt = empty | '(' >> pointTxt >> *(',' >> pointTxt) >> ')';
          // MultiPoint Z
          multiPointZTxt = empty | '(' >> pointZTxt >> *(',' >> pointZTxt) >> ')';
          // MultiPoint M
          multiPointMTxt = empty | '(' >> pointMTxt >> *(',' >> pointMTxt) >> ')';
          // MultiPoint ZM
          multiPointZMTxt = empty | '(' >> pointZMTxt >> *(',' >> pointZMTxt) >> ')';
        }

        /*! 
          \brief Initializes multi line rules.
         */
        void initMultiLine()
        {
          // MultiLineString Tagged Text
          multiLineStringTagged =  (no_case["multilinestring"] >> multiLineStringTxt)[boost::bind(&WKTActions::createMultiLineString, &m_a)];
          // MultiLineString Z Tagged Text
          multiLineStringZTagged = (no_case["multilinestring z"] >> multiLineStringZTxt)[boost::bind(&WKTActions::createMultiLineStringZ, &m_a)];
          // MultiLineString M Tagged Text
          multiLineStringMTagged = (no_case["multilinestring m"] >> multiLineStringMTxt)[boost::bind(&WKTActions::createMultiLineStringM, &m_a)];
          // MultiLineString ZM Tagged Text
          multiLineStringZMTagged = (no_case["multilinestring zm"] >> multiLineStringZMTxt)[boost::bind(&WKTActions::createMultiLineStringZM, &m_a)];

          // MultiLineString
          multiLineStringTxt = empty | '('>> lineStringTxt >> *(',' >> lineStringTxt) >> ')';
          // MultiLineString Z
          multiLineStringZTxt = empty | '('>> lineStringZTxt >> *(',' >> lineStringZTxt) >> ')';
          // MultiLineString M
          multiLineStringMTxt = empty | '('>> lineStringMTxt >> *(',' >> lineStringMTxt) >> ')';
          // MultiLineString ZM
          multiLineStringZMTxt = empty | '('>> lineStringZMTxt >> *(',' >> lineStringZMTxt) >> ')';
        }

        /*! 
          \brief Initializes multi polygon rules.
         */
        void initMultiPolygon()
        {
          // MultiPolygon Tagged Text
          multiPolygonTagged = (no_case["multipolygon"] >> multiPolygonTxt)[boost::bind(&WKTActions::createMultiPolygon, &m_a)];
          // MultiPolygon Z Tagged Text
          multiPolygonZTagged = (no_case["multipolygon z"] >> multiPolygonZTxt)[boost::bind(&WKTActions::createMultiPolygonZ, &m_a)];
          // MultiPolygon M Tagged Text
          multiPolygonMTagged = (no_case["multipolygon m"] >> multiPolygonMTxt)[boost::bind(&WKTActions::createMultiPolygonM, &m_a)];
          // MultiPolygon ZM Tagged Text
          multiPolygonZMTagged = (no_case["multipolygon zm"] >> multiPolygonZMTxt)[boost::bind(&WKTActions::createMultiPolygonZM, &m_a)];

          // MultiPolygon
          multiPolygonTxt = empty |'(' >> polygonTxt >> *(',' >> polygonTxt) >> ')';
          // MultiPolygon Z
          multiPolygonZTxt = empty |'(' >> polygonZTxt >> *(',' >> polygonZTxt) >> ')';
          // MultiPolygon M
          multiPolygonMTxt = empty |'(' >> polygonMTxt >> *(',' >> polygonMTxt) >> ')';
          // MultiPolygon ZM
          multiPolygonZMTxt = empty |'(' >> polygonZMTxt >> *(',' >> polygonZMTxt) >> ')';
        }

        /*! 
          \brief Initializes polyhedral surface rules.
         */
        void initPolyhedralSurface()
        {
          // Polyhedral Surface Tagged Text
          polyhedralSurfaceTagged = no_case["polyhedralsurface"] >> polyhedralSurfaceTxt;
          // Polyhedral Z Surface Tagged Text
          polyhedralSurfaceZTagged = no_case["polyhedralsurface z"] >> polyhedralSurfaceZTxt;
          // Polyhedral M Surface Tagged Text
          polyhedralSurfaceMTagged = no_case["polyhedralsurface m"] >> polyhedralSurfaceMTxt;
          // Polyhedral ZM Surface Tagged Text
          polyhedralSurfaceZMTagged = no_case["polyhedralsurface zm"] >> polyhedralSurfaceZMTxt; 

          // Polyhedral Surface
          polyhedralSurfaceTxt = (empty | '(' >> polygonTxt >> *(',' >> polygonTxt) >> ')')[boost::bind(&WKTActions::createPolyhedralSurface, &m_a)];
          // Polyhedral Surface Z
          polyhedralSurfaceZTxt = (empty | '(' >> polygonZTxt >> *(',' >> polygonZTxt) >> ')')[boost::bind(&WKTActions::createPolyhedralSurfaceZ, &m_a)];
          // Polyhedral Surface M
          polyhedralSurfaceMTxt = (empty | '(' >> polygonMTxt >> *(',' >> polygonMTxt) >> ')')[boost::bind(&WKTActions::createPolyhedralSurfaceM, &m_a)];
          // Polyhedral Surface ZM
          polyhedralSurfaceZMTxt = (empty | '(' >> polygonZMTxt >> *(',' >> polygonZMTxt) >> ')')[boost::bind(&WKTActions::createPolyhedralSurfaceZM, &m_a)];
        }

        /*! 
          \brief Initializes TIN rules.
         */
        void initTIN()
        {
          // TIN Tagged Text
          tinTagged =  (no_case["tin"] >> tinTxt)[boost::bind(&WKTActions::createTIN, &m_a)];
          // TIN Z Tagged Text
          tinZTagged = (no_case["tin z"] >> tinZTxt)[boost::bind(&WKTActions::createTINZ, &m_a)];
          // TIN M Tagged Text
          tinMTagged = (no_case["tin m"] >> tinMTxt)[boost::bind(&WKTActions::createTINM, &m_a)];
          // TIN ZM Tagged Text
          tinZMTagged = (no_case["tin zm"] >> tinZMTxt)[boost::bind(&WKTActions::createTINZM, &m_a)];

          // TIN
          tinTxt = empty | '(' >> polygonTxt >> *(',' >> polygonTxt) >> ')';
          // TIN Z
          tinZTxt = empty | '(' >> polygonZTxt >> *(',' >> polygonZTxt) >> ')';
          // TIN M
          tinMTxt = empty | '(' >> polygonMTxt >> *(',' >> polygonMTxt) >> ')';
          // TIN ZM
          tinZMTxt = empty | '(' >> polygonZMTxt >> *(',' >> polygonZMTxt) >> ')';
        }

        /*! 
          \brief Initializes triangle rules.
         */
        void initTriangle()
        {
          // Triangle Tagged Text
          triangleTagged = (no_case["triangle"] >> triangleTxt)[boost::bind(&WKTActions::createTriangle, &m_a)];
          // Triangle Z Tagged Text
          triangleZTagged = (no_case["triangle z"] >> triangleZTxt)[boost::bind(&WKTActions::createTriangleZ, &m_a)];
          // Triangle M Tagged Text
          triangleMTagged = (no_case["triangle m"] >> triangleMTxt)[boost::bind(&WKTActions::createTriangleM, &m_a)];
          // Triangle ZM Tagged Text
          triangleZMTagged = (no_case["triangle zm"] >> triangleZMTxt)[boost::bind(&WKTActions::createTriangleZM, &m_a)];

          // Polygon
          triangleTxt = empty | '(' >> linearRingTxt >> *(',' >> linearRingTxt) >> ')';
          // Polygon Z
          triangleZTxt = empty | '(' >> linearRingZTxt >> *(',' >> linearRingZTxt) >> ')';
          // Polygon M
          triangleMTxt = empty | '(' >> linearRingMTxt >> *(',' >> linearRingMTxt) >> ')';
          // Polygon ZM
          triangleZMTxt = empty | '(' >> linearRingZMTxt >> *(',' >> linearRingZMTxt) >> ')';
        }

        /*! 
          \brief Initializes geometry collection rules.
         */
        void initGeometryCollection()
        {
          // GeometryCollection Tagged Text
          geometryCollectionTagged = (no_case["geometrycollection"] >> geometryCollectionTxt)[boost::bind(&WKTActions::createGeometryCollection, &m_a)];
          // GeometryCollection Z Tagged Text
          geometryCollectionZTagged =  (no_case["geometrycollection z"] >> geometryCollectionZTxt)[boost::bind(&WKTActions::createGeometryCollectionZ, &m_a)];
          // GeometryCollection M Tagged Text
          geometryCollectionMTagged =  (no_case["geometrycollection m"] >> geometryCollectionMTxt)[boost::bind(&WKTActions::createGeometryCollectionM, &m_a)];
          // GeometryCollection ZM Tagged Text
          geometryCollectionZMTagged = (no_case["geometrycollection zm"] >> geometryCollectionZMTxt)[boost::bind(&WKTActions::createGeometryCollectionZM, &m_a)];

          // Geometry Collection
          geometryCollectionTxt = empty | '(' >> geometryTagged >> *(',' >> geometryTagged) >> ')';
          // GeometryCollection Z
          geometryCollectionZTxt = empty | '(' >> geometryZTagged >> *(',' >> geometryZTagged) >> ')';
          // GeometryCollection M
          geometryCollectionMTxt = empty | '(' >> geometryMTagged >> *(',' >> geometryMTagged) >> ')';
          // GeometryCollection ZM
          geometryCollectionZMTxt = empty | '(' >> geometryZMTagged >> *(',' >> geometryZMTagged) >> ')';
        }

        //@}

      public:
        
        /** @name Access method.
        * Method to access the geometry generated.
        */
        //@{

        /*!
          \brief It returns the geometry generated by the parser process.

          \note The caller of this method will take the ownership of the geometry.
        */
        Geometry* getGeometry() { return m_a.getGeometry(); }

        //@}

        /*! 
          \brief This method resets the Parser to original state.

          \note Should be called case the parser processing faill.
          \note Basically, it is responsable to free the memory.
         */
        void reset() { m_a.reset(); }

      private:

        WKTActions m_a; //<! Semanthic actions to grammar rules.

        /** @name Rules of WKT Grammar.
          */
        //@{

        //<! Geometry type (root)
        qi::rule<Iterator, ascii::space_type> geometry;

        //<! Geometries
        qi::rule<Iterator, ascii::space_type> geometryTagged, geometryZTagged, geometryMTagged, geometryZMTagged;
      
        //<! Coordinates
        qi::rule<Iterator, ascii::space_type> coordinate, coordinateZ, coordinateM, coordinateZM;

        //<! Points
        qi::rule<Iterator, ascii::space_type> pointTagged, pointZTagged, pointMTagged, pointZMTagged;

        //<! Lines
        qi::rule<Iterator, ascii::space_type> lineStringTagged, lineStringZTagged, lineStringMTagged, lineStringZMTagged;

        //<! Polygons
        qi::rule<Iterator, ascii::space_type> polygonTagged, polygonZTagged, polygonMTagged, polygonZMTagged;

        //<! Polyhedral Surfaces
        qi::rule<Iterator, ascii::space_type> polyhedralSurfaceTagged, polyhedralSurfaceZTagged, polyhedralSurfaceMTagged, polyhedralSurfaceZMTagged;

        //<! MultiPoints
        qi::rule<Iterator, ascii::space_type> multiPointTagged, multiPointZTagged, multiPointMTagged, multiPointZMTagged;
      
        //<! MultiLines
        qi::rule<Iterator, ascii::space_type> multiLineStringTagged, multiLineStringZTagged, multiLineStringMTagged, multiLineStringZMTagged;
      
        //<! MultiPolygons
        qi::rule<Iterator, ascii::space_type> multiPolygonTagged, multiPolygonZTagged, multiPolygonMTagged, multiPolygonZMTagged;

        //<! Triangle
        qi::rule<Iterator, ascii::space_type> triangleTagged, triangleZTagged, triangleMTagged, triangleZMTagged;

        //<! TIN
        qi::rule<Iterator, ascii::space_type> tinTagged, tinZTagged, tinMTagged, tinZMTagged;

        //<! Geometry Collection
        qi::rule<Iterator, ascii::space_type> geometryCollectionTagged, geometryCollectionZTagged, geometryCollectionMTagged, geometryCollectionZMTagged;

        //<! Text format for Points
        qi::rule<Iterator, ascii::space_type> pointTxt, pointZTxt, pointMTxt, pointZMTxt;

        //<! Text format for Lines
        qi::rule<Iterator, ascii::space_type> lineStringTxt, lineStringZTxt, lineStringMTxt, lineStringZMTxt;
        
        //<! Text format for LinearRing
        qi::rule<Iterator, ascii::space_type> linearRingTxt, linearRingZTxt, linearRingMTxt, linearRingZMTxt;

        //<! Text format for Polygons
        qi::rule<Iterator, ascii::space_type> polygonTxt, polygonZTxt, polygonMTxt, polygonZMTxt;

        //<! Text format for Polyhedral Surfaces
        qi::rule<Iterator, ascii::space_type> polyhedralSurfaceTxt, polyhedralSurfaceZTxt, polyhedralSurfaceMTxt, polyhedralSurfaceZMTxt;

        //<! Text format for MultiPoints
        qi::rule<Iterator, ascii::space_type> multiPointTxt, multiPointZTxt, multiPointMTxt, multiPointZMTxt;

        //<! Text format for MultiLines
        qi::rule<Iterator, ascii::space_type> multiLineStringTxt, multiLineStringZTxt, multiLineStringMTxt, multiLineStringZMTxt;

        //<! Text format for MultiPolygons
        qi::rule<Iterator, ascii::space_type> multiPolygonTxt, multiPolygonZTxt, multiPolygonMTxt, multiPolygonZMTxt;

        //<! Text format for TIN
        qi::rule<Iterator, ascii::space_type> tinTxt, tinZTxt, tinMTxt, tinZMTxt;

        //<! Text format for Triangles
        qi::rule<Iterator, ascii::space_type> triangleTxt, triangleZTxt, triangleMTxt, triangleZMTxt;

        //<! Text format for GeometryCollections
        qi::rule<Iterator, ascii::space_type> geometryCollectionTxt, geometryCollectionZTxt, geometryCollectionMTxt, geometryCollectionZMTxt;

        //<! Empty keyword rule
        qi::rule<Iterator, ascii::space_type> empty;

        //@}

    }; // WKTParser

  } // namespace gm
}   // namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_WKTPARSER_H

