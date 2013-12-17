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
  \file CoverageSeries.h

  \brief This file contains a class to represent a coverage series.
*/

#ifndef __TERRALIB_ST_INTERNAL_COVERAGESERIES_H
#define __TERRALIB_ST_INTERNAL_COVERAGESERIES_H

//TerraLib
#include "../../../datatype/DateTimePeriod.h"
#include "../../../geometry/Geometry.h"

//ST
#include "../../Config.h"
#include "../../Enums.h"
#include "../timeseries/TimeSeries.h"
#include "Coverage.h"
#include "CoverageSeriesIterator.h"

//STL
#include <vector>
#include <map>
#include <memory>

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

// Forward declarations
namespace te { namespace dt { class DateTime; } }

namespace te { namespace gm { class Point; } }

namespace te
{
  namespace st
  {
    // Forward declarations
    class TimeSeries;
    class AbstractCoverageSeriesInterp;
    
    /*!
      \class CoverageSeries

      \brief A class to represent a coverage series.

      A coverage series is a sequence of coverages over time (T -> Coverage)
      
      \ingroup st

      \sa Coverage RasterCoverage PointCoverage TimeSeries
    */
    class TESTEXPORT CoverageSeries 
    {
      public:

        /*!
          \brief A constructor.

          \note Internally, it will use the interpolator NearestCoverageAtTimeInterp
        */
        CoverageSeries();  

        /*!
          \brief A constructor.

          \param obs      The coverage series observations
          \param interp   The interpolator associated to the coverage series.
          \param se       The coverage series spatial extent.
          \param t        The type of the coverages that are in the observation set. 

          \note It will take the ownership of the given pointers.
        */
        CoverageSeries( const CoverageSeriesObservationSet& obs, 
                        AbstractCoverageSeriesInterp* interp, te::gm::Geometry* se, CoverageType t);  
        
        /*!
          \brief It returns the coverage series observations.

          \return A reference to the coverage series observations.
        */
        const CoverageSeriesObservationSet& getObservations() const;
        
        /*!
          \brief It returns the type of the internal coverages.
          
          For while, there are two kinds of Coverages: Point Coverage and Raster Coverage. 

          \return Returns the coverage type.
        */
        CoverageType getType() const;

        /*!
          \brief It returns the spatial extent of the coverage series
          
          \return Returns the coverage series spatial extent.

          \note The caller will NOT take the ownership of the returned geometry.
        */
        te::gm::Geometry* getSpatialExtent() const;

        /*!
          \brief It returns the temporal extent of the coverage series
          
          \return Returns the coverage series temporal extent.

          \note The caller will take the ownership of the returned geometry.
        */
        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent() const;

        /*! 
          \brief It adds an observation (time and coverage) into the coverage series. 

          \param time A pointer to the time.
          \param cv   A coverage.
          
          \note The caller will take the ownership of the given pointers.    
        */  
        void add(te::dt::DateTime* time, te::st::Coverage* cv);

        /*! 
          \brief It adds an observation (time and coverage) into the coverage series. 

          \param o An observation
        */  
        void add(const CoverageSeriesObservation& o); 
                
        /*!
          \brief It returns the size of the coverage series observations.

          \return The observations size of the coverage series.
        */
        std::size_t size() const;    

        /*! \name CoverageSeries Iterator 
                         
            An example of use:

            CoverageSeriesIterator it = cvs.begin();
            while(it!=cvs.end())
            {
              DateTime* t = it.getTime();
              Coverage* c = it.getCoverage(); 
              ++it;
            }      
        */
        //@{
        /*!
          \brief It returns an iterator that points to the first observation of the point coverage
          
          \return The coverage series iterator.
        */
        CoverageSeriesIterator begin() const;

        /*!
          \brief It returns an iterator that points to the end of the time series.
          
          \return The coverage series iterator.
        */
        CoverageSeriesIterator end() const;

        /*!
          \brief It returns an iterator that points to an observation at a given time.
          
          If there is no observation at this given time, the returned iterator
          points to the end of the coverage series.
          
          \return The coverage series iterator.
          \note This does not take the ownership of the given pointer.
        */
        CoverageSeriesIterator at(te::dt::DateTime* t) const;
        //@}
        
        /*!
          \brief It returns the coverage associated to a given date and time.

          If there is no coverage associated to the given date and time, it will
          use internally its interpolation function.

          \param t  A date and time.
           
          \return   A pointer to the coverage associated to the given date and time.

          \note The caller will take the ownership of the returned pointer.
          \note The caller will NOT take the ownership of the given date and time pointer.
        */
        std::auto_ptr<te::st::Coverage> getCoverage(te::dt::DateTime* t) const;  
                
        
        /*! \name Methods to return time series from the coverage series */
        //@{
        /*!
          \brief It returns a time series of the p-th property associated to a given location 

          \param l  A given location.
          \param p  A given property or band
           
          \return   A pointer to the time series of the p-th property associated to a given location

          \note The caller will take the ownership of the returned pointer.
          \note The caller will NOT take the ownership of the given date and time pointer.
        */
        std::auto_ptr<te::st::TimeSeries> getTimeSeries(const te::gm::Point& l, unsigned int p=0) const; 

        /*!
          \brief It returns the time series associated to a given location 

          It returns a set of time series, one for each property of the coverage.

          \param l  A given location.
          \param r  The resulting time series
           
          \note The caller will take the ownership of the returned pointer.
        */
        void getTimeSeries(const te::gm::Point& l, boost::ptr_vector<TimeSeries>& r) const; 

        /*!
          \brief It returns the time series of the p-th property inside the given polygon 

          It returns a set of time series, one for each point inside the polygon

          \param l  A given polygon.
          \param p  A given property.
          \param r  The resulting time series
           
          \note The caller will take the ownership of the returned pointer.
        */
        void getTimeSeries(const te::gm::Polygon& l, unsigned int p, boost::ptr_vector<TimeSeries>& r) const; 

        /*!
          \brief It returns the time series of the p-th property inside the given envelope 

          It returns a set of time series, one for each point inside the envelope

          \param l  A given envelope.
          \param p  A given property.
          \param r  The resulting time series
           
          \note The caller will take the ownership of the returned pointer.
        */
        void getTimeSeries(const te::gm::Envelope& e, unsigned int p, boost::ptr_vector<TimeSeries>& r) const; 

        /*!
          \brief It returns the time series inside the given polygon 

          It returns a set of time series, one for each point inside the polygon and for
          each property of the coverage.

          This method returns the time series of all properties of the coverages, ordered by locations.
          
          An example, if the coverages have two properties:
          The first position of the result vector contains the a time series of the first property of first location.
          The second position of the result vector contains the a time series of the second property of the first location.
          The third position of the result vector contains the a time series of the first property of the second location.
          And so on.

          \param l  A given polygon.
          \param r  The resulting time series
           
          \note The caller will take the ownership of the returned pointer.
        */
        void getTimeSeries(const te::gm::Polygon& l, boost::ptr_vector<TimeSeries>& r) const; 

        /*!
          \brief It returns the time series inside the given envelope 

          It returns a set of time series, one for each point inside the envelope and for
          each property of the coverage.

          This method returns the time series of all properties of the coverages, ordered by locations.
          
          An example, if the coverages have two properties:
          The first position of the result vector contains the a time series of the first property of first location.
          The second position of the result vector contains the a time series of the second property of the first location.
          The third position of the result vector contains the a time series of the first property of the second location.
          And so on.

          \param e  A given envelope.
          \param r  The resulting time series
           
          \note The caller will take the ownership of the returned pointer.
        */
        void getTimeSeries(const te::gm::Envelope& e, boost::ptr_vector<TimeSeries>& r) const; 

        /*!
          \brief  It returns a time series of the p-th property associated to a given location 
                  and considering a given temporal restriction.

          \param l  A given location.
          \param t  A given time
          \param tr A given temporal restriction
          \param p  A given property or band
           
          \return   A pointer to the time series of the p-th property associated to a given location

          \note The caller will take the ownership of the returned pointer.
          \note The caller will NOT take the ownership of the given date and time pointer.
        */
        std::auto_ptr<te::st::TimeSeries> getTimeSeries(const te::gm::Point& l, const te::dt::DateTime& t,
                                                        te::dt::TemporalRelation tr = te::dt::DURING, 
                                                        unsigned int p=0) const; 

        /*!
          \brief  It returns the time series associated to a given location 
                  and considering a given temporal restriction.

          It returns a set of time series, one for each property of the coverage.

          \param l  A given location.
          \param t  A given time
          \param tr A given temporal restriction
          \param r  The resulting time series
           
          \note The caller will take the ownership of the returned pointer.
        */
        void getTimeSeries(const te::gm::Point& l, const te::dt::DateTime& t,
                           te::dt::TemporalRelation tr, boost::ptr_vector<TimeSeries>& r) const; 

        /*!
          \brief It returns the time series of the p-th property inside the given polygon 
                  and considering a given temporal restriction.

          It returns a set of time series, one for each point inside the polygon

          \param l  A given polygon.
          \param p  A given property.
          \param t  A given time
          \param tr A given temporal restriction
          \param r  The resulting time series
           
          \note The caller will take the ownership of the returned pointer.
        */
        void getTimeSeries(const te::gm::Polygon& l, unsigned int p, const te::dt::DateTime& t,
                           te::dt::TemporalRelation tr, boost::ptr_vector<TimeSeries>& r) const; 

        /*!
          \brief It returns the time series of the p-th property inside the given envelope 
                  and considering a given temporal restriction.

          It returns a set of time series, one for each point inside the envelope

          \param l  A given envelope.
          \param p  A given property.
          \param t  A given time
          \param tr A given temporal restriction
          \param r  The resulting time series
           
          \note The caller will take the ownership of the returned pointer.
        */
        void getTimeSeries(const te::gm::Envelope& e, unsigned int p, const te::dt::DateTime& t,
                           te::dt::TemporalRelation tr, boost::ptr_vector<TimeSeries>& r) const; 

        /*!
          \brief It returns the time series inside the given polygon 
                  and considering a given temporal restriction.

          It returns a set of time series, one for each point inside the polygon and for
          each property of the coverage

          \param l  A given polygon.
          \param t  A given time
          \param tr A given temporal restriction
          \param r  The resulting time series
           
          \note The caller will take the ownership of the returned pointer.
        */
        void getTimeSeries(const te::gm::Polygon& l, const te::dt::DateTime& t,
                           te::dt::TemporalRelation tr, boost::ptr_vector<TimeSeries>& r) const; 

        /*!
          \brief It returns the time series inside the given envelope 
                  and considering a given temporal restriction.

          It returns a set of time series, one for each point inside the envelope and for
          each property of the coverage

          \param e  A given envelope.
          \param t  A given time
          \param tr A given temporal restriction
          \param r  The resulting time series
           
          \note The caller will take the ownership of the returned pointer.
        */
        void getTimeSeries(const te::gm::Envelope& e, const te::dt::DateTime& t,
                           te::dt::TemporalRelation tr, boost::ptr_vector<TimeSeries>& r) const; 
        //@}

        /*! \name Methods to return a subset or patch of a coverage series given a spatial and temporal restriction. */
        //@{

        /*!
          \brief It returns a subset or patch of the coverage series considering a given temporal restriction.

          \param dt A given time
          \param r  A given temporal restriction
           
          \note The caller will take the ownership of the returned pointer.
          \note The retorned coverage series will share the internal coverage pointers.
        */
        std::auto_ptr<CoverageSeries> getPatch(const te::dt::DateTime& dt, te::dt::TemporalRelation r = te::dt::DURING) const;

        /*!
          \brief It returns a subset or patch of the coverage series considering a given spatial restriction.

          \param e  A given envelope.
          \param sr   A given spatial restriction
           
          \note The caller will take the ownership of the returned pointer.
          \note The retorned coverage series will NOT share the internal coverage pointers.
        */
        std::auto_ptr<CoverageSeries> getPatch(const te::gm::Envelope& e, te::gm::SpatialRelation sr = te::gm::INTERSECTS) const;

        /*!
          \brief It returns a subset or patch of the coverage series considering a given spatial restriction.

          \param e    A given geometry.
          \param sr   A given spatial restriction
           
          \note The caller will take the ownership of the returned pointer.
          \note The retorned coverage series will NOT share the internal coverage pointers.
        */
        std::auto_ptr<CoverageSeries> getPatch(const te::gm::Geometry& e, te::gm::SpatialRelation sr = te::gm::INTERSECTS) const;

        /*!
          \brief It returns a subset or patch of the coverage series considering a given spatial and temporal restriction.

          \param e    A given envelope.
          \param sr   A given spatial restriction
          \param dt   A given date and time
          \param tr   A given temporal restriction
           
          \note The caller will take the ownership of the returned pointer.
          \note The retorned coverage series will NOT share the internal coverage pointers.
        */
        std::auto_ptr<CoverageSeries> getPatch(const te::gm::Envelope& e, te::gm::SpatialRelation sr, 
                                               const te::dt::DateTime& dt, te::dt::TemporalRelation r = te::dt::DURING) const;

        /*!
          \brief It returns a subset or patch of the coverage series considering a given spatial and temporal restriction.

          \param e    A given geometry.
          \param sr   A given spatial restriction
          \param dt   A given date and time
          \param tr   A given temporal restriction
           
          \note The caller will take the ownership of the returned pointer.
          \note The retorned coverage series will NOT share the internal coverage pointers.
        */
        std::auto_ptr<CoverageSeries> getPatch(const te::gm::Geometry& e, te::gm::SpatialRelation sr, 
                                               const te::dt::DateTime& dt, te::dt::TemporalRelation tr = te::dt::DURING) const;
        //@}
                
        /*! \brief Virtual destructor. */
        virtual ~CoverageSeries();

      protected:

        CoverageSeriesObservationSet            m_observations; //! The coverage series observations
        AbstractCoverageSeriesInterp*           m_interpolator;    //!< The interpolator used to estimate non-observed times.
        CoverageType                            m_cvtype;
        std::auto_ptr<te::gm::Geometry>         m_sextent;
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_COVERAGE_H

