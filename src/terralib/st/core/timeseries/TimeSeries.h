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
  \file TimeSeries.h

  \brief This file contains a class to represent a time series.
*/

#ifndef __TERRALIB_ST_INTERNAL_TIMESERIES_H
#define __TERRALIB_ST_INTERNAL_TIMESERIES_H

//STL
#include <vector>
#include <map>
#include <memory>

//TerraLib
#include "../../../datatype/Enums.h"

//ST
#include "../../Config.h"
#include "TimeSeriesIterator.h"

// Forward declarations
namespace te { namespace dt { class AbstractData; class DateTime; class DateTimePeriod; } }
namespace te { namespace gm { class Geometry; } }

namespace te
{
  namespace st
  {    
    // Forward declarations
    class AbstractTimeSeriesInterp; 
    
    /*!
      \class TimeSeries

      \brief A class to represent time series.

      A time series represents a variation of a property over time, where 
      this property is of a textual or numerical type (T -> A). 
      It is composed of a set of observations and of an interpolator able
      to estimate values in non-observed times.

      \ingroup st

      \sa ObservationSet AbstractInterpolator
    */
    class TESTEXPORT TimeSeries
    {
      public:

        /*! \name TimeSeries Constructors */
        //@{
        
        /*! 
          \brief Empty constructor. 

          It constructs an empty time series. The user does not indicate the interpolator
          associated to it. Internally, it uses a default interpolator
          that is NearestValueAtTimeInterp.
        */
        TimeSeries();
                
        /*! 
          \brief Empty constructor. 

          It constructs an empty time series. The user does not indicate the interpolator
          associated to it. Internally, it uses a default interpolator
          that is NearestValueAtTimeInterp.

          \param id The time series id
        */
        TimeSeries(const std::string& id);
        
        /*! 
          \brief Empty constructor. 

          It constructs an empty trajectory. 

          \param id     The time series id.     
          \param interp The interpolator associated to the time series.  
        
        */
        TimeSeries(AbstractTimeSeriesInterp* interp, const std::string& id);

        /*! 
          \brief Empty constructor. 

          It constructs an empty trajectory. 

          \param interp The interpolator associated to the time series.  
          \param id     The time series id.     
          \param geom   The time series location.

          \note It will take the ownership of the given geometry.        
        */
        TimeSeries(AbstractTimeSeriesInterp* interp, const std::string& id, 
                   te::gm::Geometry* geom);

        /*! 
          \brief Constructor. 
          
          \param obs    The time series observations.
          \param interp The interpolator associated to the time series.  
          \param id     The time series id.     
          \param geom   The time series location.

          \note It will take the ownership of the given geometry.          
        */
        TimeSeries( const TimeSeriesObservationSet& obs, AbstractTimeSeriesInterp* interp, 
                    const std::string& id, te::gm::Geometry* geom);
        /*! 
          \brief It constructs a time series from a patch

          \param patch  The given patch.
          \param interp The interpolator
          \param id     The time series id
          \param geom   The time series location
        */
        TimeSeries(const TimeSeriesPatch& patch, AbstractTimeSeriesInterp* interp, 
                   const std::string& id, te::gm::Geometry* geom);

        /*! 
          \brief Copy constructor. 
          
          \note The built time series will share the internal pointers of "ts". 
          \note The caller has to use the method "clone" to have a deep copy.  
        */
        TimeSeries(const TimeSeries& ts);
        //@}

        /*! 
          \brief Copy assignment operator

          \note They will share the internal pointers. 
          \note The caller has to use the method "clone" to have a deep copy. 
        */
        TimeSeries& operator=(const TimeSeries& other);

        /*!
          \brief It returns a clone of this time series. 

          \return A new time series. 

          \note The caller will take the ownership of the returned pointer.
        */
        TimeSeries* clone() const;

        /*!
          \brief It returns the time series observations.

          \return A reference to the time series observations.
        */
        const TimeSeriesObservationSet& getObservations() const;
        
        /*!
          \brief It returns the interpolator associated to the time series.

          \return A pointer the interpolator associated to the time series.

          \note The caller will NOT take the ownership of the returned pointer.
          \note The AbstractTimeSeriesInterp points to a singleton. 
        */
        AbstractTimeSeriesInterp* getInterpolator() const;

        /*!
          \brief It sets the interpolator associated to the time series.

          \param interp A interpolator which is able to estimate values at non-observed times.

          \note It will NOT take the ownership of the input pointer.
          \note The AbstractTimeSeriesInterp points to a singleton. 
        */
        void setInterpolator(AbstractTimeSeriesInterp* interp);

        /*!
          \brief It returns the time series identifier.

          \return The time series identifier.
        */
        std::string getId() const;
        
        /*!
          \brief It sets the time series identifier.

          \param id The time series identifier.
        */
        void setId(const std::string& id);    
        
        /*!
          \brief It returns the time series location.

          \return A pointer to the time series location.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::gm::Geometry* getLocation() const;
        
        /*!
          \brief It sets the time series location.

          \param geom The time series location.

          \note It will take the ownership of the given pointer.
        */
        void setLocation(te::gm::Geometry* geom);                    

        /*! 
          \brief It adds an observation (time and attribute value) into the time series. 

          \param time A pointer to the time.
          \param data A pointer to the attribute value.
          
          \note The caller will take the ownership of the returned pointer.    
        */  
        void add(te::dt::DateTime* time, te::dt::AbstractData* value);

        /*! 
          \brief It adds an observation (time and attribute value as double) into the time series. 

          \param time A pointer to the time.
          \param data A pointer to the attribute value as a double.
          
          \note The caller will take the ownership of the given pointer.    
        */  
        void add(te::dt::DateTime* time, double value);

        /*! 
          \brief It adds an observation (time and attribute value as integer) into the time series. 

          \param time A pointer to the time.
          \param data A pointer to the attribute value as a integer.
          
          \note The caller will take the ownership of the given pointer.    
        */  
        void add(te::dt::DateTime* time, int value);

        /*! 
          \brief It adds an observation (time and attribute value as string) into the time series. 

          \param time A pointer to the time.
          \param data A pointer to the attribute value as a string.
          
          \note The caller will take the ownership of the given pointer.    
        */  
        void add(te::dt::DateTime* time, const std::string& value);

        /*! 
          \brief It adds an observation (time and attribute value) into the time series. 

          \param item A time series item.
        */  
        void add(const TimeSeriesObservation& item);
        
        /*!
          \brief It returns the size of the time series observation set.

          \return The observation set size of the time series.
        */
        std::size_t size() const;    
        
        /*! \name TimeSeries Iterator 
                  
            An example of use:

            TimeSeriesIterator it = tj.begin();
            while(it!=tj.end())
            {
              DateTime* t = it.getTime();
              double v = it.getDouble(); //or getValue() or getInt()...                    
              ++it;
            } 
        */
        //@{
        /*!
          \brief It returns an iterator that points to the first observation of the time series.
          
          \return The time series iterator.
        */
        TimeSeriesIterator begin() const;

        /*!
          \brief It returns an iterator that points to the end of the time series.
          
          \return The time series iterator.
        */
        TimeSeriesIterator end() const;

        /*!
          \brief It returns an iterator that points to an observation at a given time.
          
          If there is no observation at this given time, the returned iterator
          points to the end of the trajectory.
          
          \return The time series iterator.
          \note This does not take the ownership of the given pointer.
        */
        TimeSeriesIterator at(te::dt::DateTime* t) const;
        //@}
               
        /*!
          \brief It returns the value associated to a given date and time.

          If there is no value associated to the given date and time, it will
          use internally its interpolation function.

          \param t  A date and time.
           
          \return   A pointer to the value associated to the given date and time.

          \note The caller will take the ownership of the returned pointer.
        */
        std::auto_ptr<te::dt::AbstractData> getValue(te::dt::DateTime* t) const;  

        /*!
          \brief It returns the value as a double associated to a given date and time.

          If there is no value associated to the given date and time, it will
          use internally its interpolation function.

          \param t  A date and time.
           
          \return   The value associated to the given date and time.
        */
        double getDouble(te::dt::DateTime* t) const; 
        
        /*!
          \brief It returns the value as an integer associated to a given date and time.

          If there is no value associated to the given date and time, it will
          use internally its interpolation function.

          \param t  A date and time.
           
          \return   The value associated to the given date and time.
        */
        int getInt(te::dt::DateTime* t) const; 

         /*!
          \brief It returns the value as a string associated to a given date and time.

          If there is no value associated to the given date and time, it will
          use internally its interpolation function.

          \param t  A date and time.
           
          \return   The value associated to the given date and time.
        */
        std::string getString(te::dt::DateTime* t) const; 
        
        /*!
          \brief It returns the temporal extent or range of the time series.

          \return The temporal extent or range of the time series.

          \note The caller will take the ownership of the output pointer.
        */
        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent() const;

        /*!
          \brief It returns the minimal and maximun values of the time series. 

          \param minValue The minumun value of the time series.
          \param maxValue The maximun value of the time series.
        */
        void getValueExtent(double& minValue, double& maxValue);

        /*!
          \brief It returns the minimal and maximun values of the time series. 

          \param minValue The minumun value of the time series.
          \param maxValue The maximun value of the time series.
        */
        void getValueExtent(int& minValue, int& maxValue);

        /*!
          \brief It returns the minimal and maximun values of the time series. 

          \param minValue The minumun value of the time series.
          \param maxValue The maximun value of the time series.
        */
        void getValueExtent(std::string& minValue, std::string& maxValue);
                
        /*! \name Time Series Patch 
            
            These methods return patches of a time series, as TimeSeriesPatch objects,
            that satisfy given restrictions. 
            A TimeSeriesPatch object contains only pointers to the initial and 
            final observations that delimit a patch of a TimeSeries. The 
            end() method of a TimeSeriesPatch object returns an iterator
            that points to the position AFTER the last required observation.

            Example of use:

            TimeSeriesPatch patch = tj->getTimeSeries(period, "DURING");
            TimeSeriesIterator it = patch.begin();
            while(it!=patch.end())
            {
               DateTime* t = it.getTime();
               double v = it.getDouble(); //or getInt() or get Value()
               ++it;
            } 
            
            Other option is to create a new TimeSeries from the returned patch: 
            
            TimeSeriesPatch patch = tj->getTimeSeries(period, "DURING");
            TimeSeries tsnew(patch); 
        */
        //@{
        /*!
          \brief It returns a time series subset that satisfies a given temporal relation. 
          
          The possible temporal relations are: AFTER (2); 
          AFTER | EQUALS (10); 3. BEFORE (1);  4. BEFORE | EQUALS (9); 
          DURING (4); EQUALS (8) 

          \param dt   A given date and time.
          \param r    A given temporal relation.
                   
          \return   A patch of the time series.

          \note When the temporal relation is DURING, dt must be a time period.
        */
        TimeSeriesPatch getPatch(const te::dt::DateTime& dt, te::dt::TemporalRelation r = te::dt::DURING) const;

        /*!
          \brief It returns time series subsets that satisfy a given relation. 
          
          The possible basic relation is: LESS (1); MORE (2); EQUAL (4);
          LESS | EQUAL (5); MORE | EQUAL (6).  

          \param v      A given value.
          \param r      A given basic relation.
          \param result The returned patches.
        */
        void getPatches( const double& v, te::dt::BasicRelation r, 
                            std::vector<TimeSeriesPatch>& result) const;

        /*!
          \brief It returns time series subsets that satisfy a given relation. 
          
          The possible basic relation is: LESS (1); MORE (2); EQUAL (4);
          LESS | EQUAL (5); MORE | EQUAL (6).  

          \param v1      A given first value.
          \param r1      A given relation associated to the first value.
          \param v2      A given second value.
          \param r2      A given relation associated to the second value.
          \param result The returned patches.
        */
        //void getTimeSeries( const double& v1, te::dt::BasicRelation r1, 
        //                    const double& v2, te::dt::BasicRelation r2, 
        //                    std::vector<TimeSeriesPatch>& result) const;

        /*!
          \brief It returns time series subsets that satisfy a the given relations. 
          
          The possible basic relations are: LESS (1); MORE (2); EQUAL (4);
          LESS | EQUAL (5); MORE | EQUAL (6).  

          The possible temporal relations are: AFTER (2); 
          AFTER | EQUALS (10); 3. BEFORE (1);  4. BEFORE | EQUALS (9); 
          DURING (4); EQUALS (8) .

          \param v      A given value.
          \param r      A basic relation.
          \param dt     A given date and time.
          \param tr     A given temporal relation.
          \param result The returned patches.

          \note When the temporal relation is DURING, dt must be a time period.
        */
        void getPatches( const double& v, te::dt::BasicRelation r, 
                            const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                            std::vector<TimeSeriesPatch>& result) const;

        /*!
          \brief It returns time series subsets that satisfy the given relations. 
          
          The possible basic relations are: LESS (1); MORE (2); EQUAL (4);
          LESS | EQUAL (5); MORE | EQUAL (6).  

          The possible temporal relations are: AFTER (2); 
          AFTER | EQUALS (10); 3. BEFORE (1);  4. BEFORE | EQUALS (9); 
          DURING (4); EQUALS (8) . 

          \param v1      A given first value.
          \param r1      A given relation associated to the first value.
          \param v2      A given second value.
          \param r2      A given relation associated to the second value.
          \param dt     A given date and time.
          \param tr     A given temporal relation.
          \param result The returned patches.

           \note When the temporal relation is DURING, dt must be a time period.
        */
        //void getTimeSeries( const double& v1, te::dt::BasicRelation r1, 
        //                    const double& v2, te::dt::BasicRelation r2, 
        //                    const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
         //                   std::vector<TimeSeriesPatch>& result) const;
        //@}
                                                
        /*! \brief Virtual destructor. */
        virtual ~TimeSeries();

      private:
        
        /*! \brief It returns the result of a basic relation ordered by time */
        void getResultOrderedByTime( const double& v, te::dt::BasicRelation r, std::map<te::dt::DateTime*,te::dt::AbstractData*, te::dt::CompareDateTime>& result) const;

      private:

        TimeSeriesObservationSet        m_observations;  //!< The time series observations 
        AbstractTimeSeriesInterp*       m_interpolator;  //!< The interpolator used to estimate values at non-observed times.
        std::auto_ptr<te::gm::Geometry> m_location;      //!< The time series location.  
        std::string                     m_id;            //!< The time series identification.     
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TIMESERIES_H

