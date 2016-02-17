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
  \file WKTParser.h

  \brief A class that implements the grammar rules for well known text (WKT) of Spatial Reference Systems.
         It is based on boost::spirit V2 and WKT BNF definition available in the OGC Coordinate Transformation
         Services Specification - Revision 1.00 - pages 18 and 19.

  \warning Do not use this class. It is for TerraLib internal use. Try the te::srs::WKTReader instead.
 */

#ifndef __TERRALIB_SRS_INTERNAL_WKTPARSER_H
#define __TERRALIB_SRS_INTERNAL_WKTPARSER_H

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
using qi::as_string;
using qi::char_;
using qi::double_;
using ascii::no_case;

namespace te
{
  namespace srs
  {
    /*!
      \class WKTParser
      
      \brief A class that implements the Grammar Rules for well known text (WKT) format of Spatial Reference Systems.
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
        WKTParser() : WKTParser::base_type(srs)
        {
          srs = geographicCS | projectedCS;

          // Initializing all rules
          initTags();
          initSpatialReferenceSystems();
          initDatum();
          initToWGS84();
          initProjection();
          initParameter();
          initAxis();
          initSpheroid();
          initPrimeMeridian();
          initUnits();
          initDoubleValues();
          initQuotedNames();
          initAuthority();
          initDelimiters();
        }

        //@}

      private:
        
        /** @name Initializer methods.
          *  Methods to initialize the grammar rules.
          */
        //@{

        void initTags()
        {
          projectedCSTag = "PROJCS";
          geographicCSTag = "GEOGCS";
          datumTag = "DATUM";
          toWGS84Tag = "TOWGS84";
          spheroidTag = "SPHEROID";
          projectionTag = "PROJECTION";
          parameterTag = "PARAMETER";
          primeMeridianTag = "PRIMEM";
          unitTag = "UNIT";
          authorityTag = "AUTHORITY";
          axisTag = "AXIS";
          northTag = "NORTH";
          southTag = "SOUTH";
          eastTag = "EAST";
          westTag = "WEST";
          upTag = "UP";
          downTag = "DOWN";
          otherTag = "OTHER";
        }

        void initSpatialReferenceSystems()
        {
          projectedCS = (projectedCSTag[boost::bind(&WKTActions::createProjectedCoordinateSystem, &m_a)]
                        >> leftDelimiter
                        >> csName >> ',' 
                        >> geographicCS >> ',' 
                        >> projection >> *(',' >> parameter) >> *(',' >> axis)
                        >> ',' >> unit >> -(',' >> authority)
                        >> rigthDelimiter)[boost::bind(&WKTActions::endProjectedCoordinateSystem, &m_a)];

          geographicCS = (geographicCSTag[boost::bind(&WKTActions::createGeographicCoordinateSystem, &m_a)]
                         >> leftDelimiter 
                         >> csName >> ',' 
                         >> datum >> ',' 
                         >> primeMeridian >> ',' 
                         >> unit >> -(',' >> authority)
                         >> rigthDelimiter)[boost::bind(&WKTActions::endGeographicCoordinateSystem, &m_a)];
        }

        void initDatum()
        {
          datum = (datumTag[boost::bind(&WKTActions::createDatum, &m_a)]
                  >> leftDelimiter
                  >> datumName >> ','
                  >> spheroid >> -(',' >> toWGS84) >> -(',' >> authority)
                  >> rigthDelimiter)[boost::bind(&WKTActions::endDatum, &m_a)];
        }

        void initToWGS84()
        {
          toWGS84 = (toWGS84Tag >> leftDelimiter >> sevenParams >> rigthDelimiter)[boost::bind(&WKTActions::endToWGS84, &m_a)];
          sevenParams = dx >> ',' >> dy >> ',' >> dz >> ',' >> ex >> ',' >> ey >> ',' >> ez >> ',' >> ppm;
          dx = double_[boost::bind(&WKTActions::setDx, &m_a, boost::placeholders::_1)];
          dy = double_[boost::bind(&WKTActions::setDy, &m_a, boost::placeholders::_1)];
          dz = double_[boost::bind(&WKTActions::setDz, &m_a, boost::placeholders::_1)];
          ex = double_[boost::bind(&WKTActions::setEx, &m_a, boost::placeholders::_1)];
          ey = double_[boost::bind(&WKTActions::setEy, &m_a, boost::placeholders::_1)];
          ez = double_[boost::bind(&WKTActions::setEz, &m_a, boost::placeholders::_1)];
          ppm = double_[boost::bind(&WKTActions::setPPM, &m_a, boost::placeholders::_1)];
        }

        void initProjection()
        {
          projection = projectionTag
                       >> leftDelimiter
                       >> projectionName >> -(',' >> authority)
                       >> rigthDelimiter;
        }

        void initParameter()
        {
          parameter = parameterTag
                      >> leftDelimiter
                      >> parameterName >> ','
                      >> value
                      >> rigthDelimiter;
        }

        void initSpheroid()
        {
          spheroid = (spheroidTag[boost::bind(&WKTActions::createSpheroid, &m_a)]
                     >> leftDelimiter
                     >> spheroidName >> ','
                     >> semiMajorAxis >> ','
                     >> invFlattening >> -(',' >> authority)
                     >> rigthDelimiter)[boost::bind(&WKTActions::endSpheroid, &m_a)];
        }

        void initPrimeMeridian()
        {
          primeMeridian = primeMeridianTag
                          >> leftDelimiter
                          >> primeMeridianName >> ','
                          >> longitude >> -(',' >> authority)
                          >> rigthDelimiter;
        }

        void initUnits()
        {
          unit = unitTag
                 >> leftDelimiter
                 >> unitName >> ','
                 >> conversionFactor >> -(',' >> authority)
                 >> rigthDelimiter;
        }

        void initQuotedNames()
        {
          unitName = '"' >> as_string[(+(char_ - '"'))][boost::bind(&WKTActions::setUnitName, &m_a, boost::placeholders::_1)] >> '"';
          
          spheroidName = '"' >> as_string[(+(char_ - '"'))][boost::bind(&WKTActions::setSpheroidName, &m_a, boost::placeholders::_1)] >> '"';
          
          projectionName = '"' >> as_string[(+(char_ - '"'))][boost::bind(&WKTActions::setProjectionName, &m_a, boost::placeholders::_1)] >> '"';
          
          datumName = '"' >> as_string[(+(char_ - '"'))][boost::bind(&WKTActions::setDatumName, &m_a, boost::placeholders::_1)] >> '"';
          
          axisName = '"' >> as_string[(+(char_ - '"'))][boost::bind(&WKTActions::setAxisName, &m_a, boost::placeholders::_1)] >> '"';
          
          axisValue = as_string[(+(char_ - ']'))][boost::bind(&WKTActions::setAxisValue, &m_a, boost::placeholders::_1)];

          csName = '"' >> as_string[(+(char_ - '"'))][boost::bind(&WKTActions::setName, &m_a, boost::placeholders::_1)] >> '"';
          
          parameterName = '"' >> as_string[(+(char_ - '"'))][boost::bind(&WKTActions::setParameter, &m_a, boost::placeholders::_1)] >> '"';
          
          primeMeridianName = '"' >> (+(char_ - '"')) >> '"';
        }

        void initDoubleValues()
        {
          value = double_[boost::bind(&WKTActions::setParameterValue, &m_a, boost::placeholders::_1)];
          
          semiMajorAxis = double_[boost::bind(&WKTActions::setSemiMajorAxis, &m_a, boost::placeholders::_1)];
          
          longitude = double_[boost::bind(&WKTActions::setPrimeMeridian, &m_a, boost::placeholders::_1)];
          
          invFlattening = double_[boost::bind(&WKTActions::setInverseFlattening, &m_a, boost::placeholders::_1)];
          
          conversionFactor = double_[boost::bind(&WKTActions::setConversionFactor, &m_a, boost::placeholders::_1)];
        }

        void initAuthority()
        {
          authority = authorityTag >> leftDelimiter >> authorityName >> ',' >> authorityCode >> rigthDelimiter;
          authorityName = '"' >> as_string[(+(char_ - '"'))][boost::bind(&WKTActions::setAuthorityName, &m_a, boost::placeholders::_1)] >> '"';
          authorityCode = '"' >> as_string[(+(char_ - '"'))][boost::bind(&WKTActions::setAuthorityCode, &m_a, boost::placeholders::_1)] >> '"';
        }

        void initAxis()
        {
          axisValue = northTag | southTag | eastTag | westTag| upTag | downTag | otherTag;

          axis = axisTag
            >> leftDelimiter
            >> axisName >> ','
            >> axisValue
            >> rigthDelimiter;
        }
        

        void initDelimiters()
        {
          leftDelimiter  = char_('(') | char_('['); 
          rigthDelimiter = char_(')') | char_(']'); 
        }

        //@}
          
      public:
        
        /** @name Access method.
        * Method to access the SRS generated.
        */
        //@{

        /*!
          \brief It returns the SRS generated by the parser process.

          \note The caller of this method will take the ownership of the SRS.
         */
        SpatialReferenceSystem* getSRS() { return m_a.getSRS(); }
        
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

        qi::rule<Iterator, ascii::space_type> srs;

        qi::rule<Iterator, ascii::space_type> projectedCS, geographicCS;

        qi::rule<Iterator, ascii::space_type> projectedCSTag, geographicCSTag, datumTag, spheroidTag, projectionTag, 
          parameterTag, primeMeridianTag, unitTag, toWGS84Tag, authorityTag, axisTag, northTag, southTag, eastTag,
          westTag, upTag, downTag, otherTag;

        qi::rule<Iterator, ascii::space_type> datum;

        qi::rule<Iterator, ascii::space_type> toWGS84, sevenParams;

        qi::rule<Iterator, ascii::space_type> dx, dy, dz, ex, ey, ez, ppm;

        qi::rule<Iterator, ascii::space_type> projection;

        qi::rule<Iterator, ascii::space_type> parameter;

        qi::rule<Iterator, ascii::space_type> spheroid;

        qi::rule<Iterator, ascii::space_type> primeMeridian;

        qi::rule<Iterator, ascii::space_type> unit;

        qi::rule<Iterator, ascii::space_type> axis;

        qi::rule<Iterator, ascii::space_type> axisValue;

        qi::rule<Iterator, ascii::space_type> value, semiMajorAxis, longitude, invFlattening, conversionFactor;

        qi::rule<Iterator> unitName, spheroidName, projectionName, primeMeridianName, parameterName, datumName, csName, axisName;

        qi::rule<Iterator, ascii::space_type> authority, authorityCode;

        qi::rule<Iterator> authorityName;

        qi::rule<Iterator, ascii::space_type> leftDelimiter, rigthDelimiter;

        //@}

    }; // WKTParser

  } // namespace srs
}   // namespace te

#endif  // __TERRALIB_SRS_INTERNAL_WKTPARSER_H
