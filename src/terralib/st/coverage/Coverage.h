/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Coverage.h

  \brief This file contains an abstract class to represent a coverage.
 */

#ifndef __TERRALIB_ST_INTERNAL_COVERAGE_H
#define __TERRALIB_ST_INTERNAL_COVERAGE_H

//ST
#include "../Config.h"

//STL
#include <vector>

// Forward declarations
namespace te { namespace dt { class DateTime;} }

namespace te { namespace gm { class Geometry; class Point; } }

namespace te { namespace rst { class Raster; } }

namespace te
{
  namespace st
  {
    // Forward declarations
    class Observation;
    class ObservationSet;
    class TimeSeries;
    class AbstractInterpolator;

    /*!
      \class Coverage

      \brief An abstract class to represent coverage.

      A coverage represents a variation of a property 
      in a spatiotemporal extent { (S,T) -> A }. Internally, a coverage
      is composed of a set of observations (ObservationSet), an
      interpolation function and a boundary.
      
      \TODO operations: setXXX para os atributos, ... 

      \sa ObservationSet AbstractInterpolator
    */
    class TESTEXPORT Coverage
    {
      public:

        /*! \name Coverage Constructors */
        //@{
        
        /*! \brief Constructor. */
        Coverage() { }    

        /*!
          \brief It returns the coverage observations.

          \return A pointer to the coverage observations.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        virtual ObservationSet* getObservationSet() const = 0;

        /*!
          \brief It returns the interpolator associated to the coverage.

          \return A pointer the interpolator associated to the coverage.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        virtual AbstractInterpolator* getInterpolator() const = 0;

        /*!
          \brief It returns the spatial extent associated to the coverage.

          \return A pointer to the spatial extent associated to the coverage.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        virtual te::gm::Geometry* getBoundary() const = 0;        
        
        /*!
          \brief It returns a snapshot of the coverage in a given date and time. 

          \param t        A given date and time. 
          \param output   The returned snapshot as a raster data type.  
           
          \note The caller will take the ownership of the returned raster output.
        */
        virtual void snapshot(te::dt::DateTime* t, te::rst::Raster* output) const = 0;
        
        /*!
          \brief It returns a set of time series associated to a coverage point.

          It returns one time series for each coverage dimention. For instance,
          if it is a raster coverage, it will return one time series for
          each raster band. If it is a point coverage, it will return one
          time series for each observed property associated to the given point.

          \param p      A point inside the coverage spatial extent.
          \param result The time series set associated to the given point.

          \note The caller will take the ownership of the returned pointers.
        */
        virtual void timeSeries(const te::gm::Point& p, std::vector<TimeSeries*>& result) const = 0;
                
        /*! \brief Virtual destructor. */
        virtual ~Coverage() { }       
        
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_COVERAGE_H

