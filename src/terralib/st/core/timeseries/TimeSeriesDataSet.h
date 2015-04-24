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
  \file TimeSeriesDataSet.h

  \brief This file contains a class to represent a time series data set.
 */

#ifndef __TERRALIB_ST_INTERNAL_TIMESERIESDATASET_H
#define __TERRALIB_ST_INTERNAL_TIMESERIESDATASET_H

//ST
#include "../../Config.h"

// Boost
#include <boost/noncopyable.hpp>

//STL
#include <vector>
#include <memory>

// Forward declarations
namespace te { namespace dt { class AbstractData; class DateTime; class DateTimePeriod; class DateTimeProperty;} }
namespace te { namespace gm { class Geometry; class GeometryProperty; } }

namespace te
{
  namespace st
  {
    // Forward declarations
    class ObservationDataSet;
    class ObservationDataSetType;
    class TimeSeries;
    class AbstractTimeSeriesInterp;
 
    /*!
      \class TimeSeriesDataSet

      \brief A class to represent a time series data set.

      This class represents a view on a DataSet that
      contains observations of time series. A time series 
      represents the variation of the values of a property over time.  

      A TimeSeriesDataSet can contain one or more properties observed
      over time.

      \ingroup st
           
      \sa ObservationDataSet ObservationDatasSetType TimeSeriesDataSetType
    */
    class TESTEXPORT TimeSeriesDataSet : public boost::noncopyable 
    {
      friend class TimeSeriesDataSetLayer;

      public:
        /*! 
          \brief Constructor. 

          \param ds         The data set that contains the time series observations.
          \param type       The observation data set type.
          \param propNames  The names of the properties that contains the time series values
          
          \note It will take the ownership of the input pointer.
        */
        TimeSeriesDataSet(te::da::DataSet* ds, const ObservationDataSetType& type, 
          const std::vector<std::string>& propNames);
        
        /*! 
          \brief Constructor. 

          \param ds         The data set that contains the time series observations.
          \param type       The observation data set type.
          \param propNames  The names of the properties that contains the time series values
          \param id         The time series id
          
          \note It will take the ownership of the input pointer.
        */
        TimeSeriesDataSet(te::da::DataSet* ds, const ObservationDataSetType& type, 
                          const std::vector<std::string>& propNames, const std::string& id);
                                                    
        /*! 
          \brief Constructor. 

          \param obs        The data set that contains the time series observations.
          \param propNames  The names of the properties that contains the time series values
          \param id         The time series id

          \note It will take the ownership of the given pointer.
        */
        TimeSeriesDataSet(ObservationDataSet* obs, const std::vector<std::string>& propNames, 
          const std::string& id);
       //@}

        /*!
          \brief It returns the data set that contains the time series observations.

          \return A pointer to the data set that contains the time series observations.

          \note The caller will NOT take the ownership of the input pointer.
        */
        ObservationDataSet* getObservationSet() const;

        /*!
          \brief It returns the indexes of the DataSet properties that contains the values associated to the time series.

          \return The indexes of the DataSet properties that contains the observed values.
        */
        const std::vector<std::string>& getValuePropNames() const;

        /*!
          \brief It sets the names of the DataSet properties that contains the values associated to the time series.

          \param n The names of the DataSet properties that contains the time series values.
        */
        void setValuePropNames(const std::vector<std::string>& n);
        
        /*!
          \brief It returns the identifier associated to the time series.

          \return The identifier associated to the time series.
        */
        std::string getId() const;
        
        /*!
          \brief It sets the identifier associated to the time series.

          \param id The identifier associated to the time series.
        */
        void setId(const std::string& id);   

        /*!
          \brief It returns the size of the time series observation set.

          \return The observation set size of the time series.
        */
        std::size_t size() const;    
        
        /*! \name Methods to traverse the trajectory observations and to check the 
                  internal cursor pointer*/
        //@{
        bool moveNext();

        bool movePrevious();

        bool moveFirst();

        bool moveBeforeFirst();

        bool moveLast();

        bool isAtBegin() const;

        bool isBeforeBegin() const;

        bool isAtEnd() const;

        bool isAfterEnd() const;
        //@}

        /*! \name Methods to get values pointed by the internal cursor. */
        //@{
        
        /*! 
          \brief It returns the time pointed by the internal cursor.

          \return A pointer to the time pointed by the internal cursor. 
          
          \note The caller will take the ownership of the returned pointer.    
        */  
        std::auto_ptr<te::dt::DateTime> getTime() const; 

        /*! 
          \brief It returns the idx-th observed value pointed by the internal cursor.

          \return A pointer to the idx-th observed value pointed by the internal cursor. 
          
          \note The caller will take the ownership of the returned pointer.   
        */  
        std::auto_ptr<te::dt::AbstractData> getValue(std::size_t idx) const;

        /*! 
          \brief It returns the observed value pointed by the internal cursor.

          \return A pointer to the observed value pointed by the internal cursor. 
          
          \note The caller will take the ownership of the returned pointer.   
        */  
        std::auto_ptr<te::dt::AbstractData> getValue() const;

        /*! 
          \brief It returns the idx-th observed value as double pointed by the internal cursor.

          \return A pointer to the idx-th observed value as double pointed by the internal cursor. 
        */  
        double getDouble(std::size_t idx) const;

        /*! 
          \brief It returns the observed value as double pointed by the internal cursor.

          \return A pointer to the observed value as double pointed by the internal cursor. 
        */  
        double getDouble() const;

        /*! 
          \brief It returns the idx-th observed value as integer pointed by the internal cursor.

          \return A pointer to the idx-th observed value as integer pointed by the internal cursor. 

          \note It returns a 32-bit integer.
        */  
        int getInt(std::size_t idx) const;

        /*! 
          \brief It returns the observed value as double pointed by the internal cursor.

          \return A pointer to the observed value as double pointed by the internal cursor. 

          \note It returns a 32-bit integer.
        */  
        int getInt() const;
        
        /*! 
          \brief It returns the geometry pointed by the internal cursor.

          \return A pointer to the geometry pointed by the internal cursor. 
          
          \note The caller will take the ownership of the returned pointer.    
        */  
        std::auto_ptr<te::gm::Geometry> getGeometry() const;
        //@}
           
        /*!
          \brief It returns the temporal extent of the time series observations.

          \return The temporal extent of the trajectory observations.

          \note The caller will NOT take the ownership of the output pointer.
        */
        const te::dt::DateTimePeriod* getTemporalExtent() const;
                        
        /*!
          \brief  It returns the time series from the DataSet.

          This method encapsulates all observations of this DataSet as a
          TimeSeries type associated to a given interpolator.

          \param interp The time series interpolator.

          \return The time series associated to a given interpolator.

          \note The caller will take the ownership of the returned pointer. 
          \note It uses the first observed property to generate the time series.
        */
        std::auto_ptr<TimeSeries> getTimeSeries(te::st::AbstractTimeSeriesInterp* interp);

        /*!
          \brief  It returns the time series from the DataSet.

          This method encapsulates all observations of this DataSet as a
          TimeSeries type associated to a given interpolator.

          \param propName    The name of the observed property to be considered
          \param interp     The time series interpolator.

          \return The time series associated to a given interpolator.

          \note The caller will take the ownership of the returned pointer. 
        */
        std::auto_ptr<TimeSeries> getTimeSeries(const std::string& propName, te::st::AbstractTimeSeriesInterp* interp);
        
        /*!
          \brief  It returns all time series from the DataSet.

          This method encapsulates all observations of this DataSet as a
          set of TimeSeries associated to a given interpolator.

          \param interp The time series interpolator.
          \param vPropName The property name that contains the attribute values associated to a time series
          \param result The returned set of time series.

          \note The caller will take the ownership of the returned pointers. 
        */
        void getTimeSeriesSet(  te::st::AbstractTimeSeriesInterp* interp,
                                const std::string& vPropName,
                                std::vector<te::st::TimeSeries*>& result);

        /*!
          \brief It returns the trajectory geometry property.

          \return The trajectory geometry property.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        //te::gm::GeometryProperty* getGeometryProperty() const;

        /*!
          \brief It returns the trajectory datetime property.

          \return The trajectory datetime property.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        //te::dt::DateTimeProperty* getTimeProperty() const;
               
        /*! \brief Virtual destructor. */
        virtual ~TimeSeriesDataSet(); 

      protected:

        /*!
          \brief It releases all internal pointers, returning its internal DataSet and invalidating itself
          \return A pointer to the internal DataSet that contains the observations. 

           \note This method is used when the user is interested only in its internal DataSet 
           \note The caller will take the ownership of the returned pointer.
        */
        std::auto_ptr<te::da::DataSet> release();

      private:
        std::auto_ptr<ObservationDataSet>     m_obsDs;         //!< The data set that contains the trajectory observations 
        std::vector<std::string>              m_vlPropNames;   //!< The names of the properties that contain the time series values. 
        std::string                           m_id;            //!< The time series identification.
     };
   } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TIMESERIESDATASET_H

