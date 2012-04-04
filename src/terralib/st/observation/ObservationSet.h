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
  \file ObservationSet.h

  \brief This file contains a class to represent an observation set.
 */

#ifndef __TERRALIB_ST_INTERNAL_OBSERVATIONSET_H
#define __TERRALIB_ST_INTERNAL_OBSERVATIONSET_H

//TerraLib
#include "../../datatype/DateTime.h"

//ST
#include "../Config.h"

// STL
#include <map>
#include <vector>

// Forward declarations
namespace te { namespace dt { class Property; class DateTimePeriod; } }
namespace te { namespace gm { class Envelope; } }

namespace te
{
  namespace st
  {    
    // Forward declarations
    class Observation;
    class ObservationSetIterator;

    // Typedef
    typedef std::multimap<te::dt::DateTime*, te::st::Observation*, te::dt::CompareDateTime> TimeToObservationMap;
    
    /*!
      \class ObservationSet

      \brief A class to represent an observation set.

      An observation set is composed of a finite number of homogeneous observations, that is, 
      every observation has the same number and type of observed values. 

      \TODO: 
      \TODO: 

      \sa Observation
    */
    class TESTEXPORT ObservationSet
    {
      public:

        /*! \brief Constructor. */
        ObservationSet(); 

        /*! 
          \brief Constructor. 
          
          \param obs    A set of observations
          \param props  The properties associated to the observed values

          \note It will take the ownership of the input pointers.
        */
        ObservationSet(const std::vector<Observation*>& obs , const std::vector<te::dt::Property*>& props);

        /*! 
          \brief Constructor. 
          
          \param props  The properties associated to the observed values
         
          \note It will take the ownership of the input pointers.
        */
        ObservationSet(const std::vector<te::dt::Property*>& props);

        /*!
          \brief It returns the observations.

          \return A reference to the internal observations.

          \note The caller will NOT take the ownership of the returned pointers.
        */
        TimeToObservationMap& getObservations();

        /*!
          \brief It returns an observationset iterator that points to its first observation.

          \return An observationset  iterator.        
        */
        ObservationSetIterator begin();

        /*!
          \brief It returns an observationset iterator that points to the end of its observation set.

          \return An observationset iterator.      
        */
        ObservationSetIterator end();

        /*!
          \brief It sets the observations.

          \param obs The observations to be set.

          \note It will take the ownership of the input pointers.
        */
        void setObservations(const std::vector<Observation*>& obs);

        /*!
          \brief It clears the observation set.
        */
        virtual void clear();

        /*!
          \brief It returns the size of the observation set.

          \return The observation set size.
        */
        std::size_t size() const;
        
        /*!
          \brief It adds an observation to the set.

          \param ob The observation to be added.

          \note It will take the ownership of the input pointer.
        */
        virtual void addObservation(Observation* ob);

        /*!
          \brief It adds a new observed property

          \param prop The new observed property. 

          \note It will take the ownership of the input pointer.
        */
        void addProperty(te::dt::Property* prop);

        /*!
          \brief It returns the properties of the observed values.

          \return A reference to the internal properties of the observed values.

          \note The caller will not take the ownership of the returned pointers.
        */
        std::vector<te::dt::Property*>& getProperties();

        /*!
          \brief It returns the property of the idx-th observed value.

          \param idx  The property index.

          \return A pointer to the property of the idx-th observed value.

          \note The caller will not take the ownership of the returned pointer.
        */
        te::dt::Property* getProperty(int idx);

        /*!
          \brief It sets the properties of the observed values.

          \param props The observed values properties.

          \note It will take the ownership of the input pointer.
        */
        void setProperties(const std::vector<te::dt::Property*>& props);       
        
        /*! \name Temporal Operations */
        //@{
        
        /*!
          \brief It returns the temporal extent of the observations.

          \return The temporal extent of the observations.

          \note The caller will take the ownership of the output pointer.
        */
        te::dt::DateTimePeriod* temporalExtent();

        /*! \name Temporal selection based on temporal predicates */
        //@{
        
        /*!
          \brief Returns the observations that occurred after a given datetime. 

          It returns the internal observations 
          whose phenomenon time occurrs after a given datetime. 

          \param  dt      A given datetime.
          \param  result  A vector to put the resulted observations (OUTPUT). 

          \note The caller will NOT take the ownership of the output pointer.
          \note The given datetime must be of the same type of the observation phenomenon time.
        */
        void after(te::dt::DateTime* dt, std::vector<Observation*>& result);

        /*!
          \brief Returns the observations that occurred before a given datetime. 

          It returns the internal observations 
          whose phenomenon time occurrs before a given datetime. 

          \param  dt A given datetime.
          \param  result  A vector to put the resulted observations (OUTPUT). 

          \note The caller will NOT take the ownership of the output pointer.
          \note The given datetime must be of the same type of the observation phenomenon time.
        */
        void before(te::dt::DateTime* dt, std::vector<Observation*>& result);

        /*!
          \brief Returns the observations that occurred at a given datetime. 

          It returns the internal observations 
          whose phenomenon time occurrs at a given datetime. 

          \param  dt A given datetime.
          \param  result  A vector to put the resulted observations (OUTPUT). 

          \note The caller will NOT take the ownership of the output pointer.
          \note The given datetime must be of the same type of the observation phenomenon time.
        */
        void at(te::dt::DateTime* dt, std::vector<Observation*>& result);

        /*!
          \brief Returns the observations that occurred during a given time period. 

          It returns the internal observations 
          whose phenomenon time occurrs during a given time period. 

          \param  initialTime The initial time of a period.
          \param  finalTime   The final time of a period.
          \param  result      A vector to put the resulted observations (OUTPUT). 

          \note The caller will NOT take the ownership of the output pointer.
          \note The given initial and final datetime must be of the same type of the observation phenomenon time.
        */
        void during(te::dt::DateTime* initialTime, te::dt::DateTime* finalTime, std::vector<Observation*>& result);
        //@}

        //@}

        /*!
          \brief It returns the minimal value of the observations, when the value is of a basic type.

          \return The minimal value of the observations.
        */
        //te::dt::AbstractData* minValue();

        /*!
          \brief It returns the maximal value of the observations, when the value is of a basic type.

          \return The maximal value of the observations.
        */
        //te::dt::AbstractData* maxValue();

        /*! \brief Virtual destructor. */
        virtual ~ObservationSet(); 

      protected:

        TimeToObservationMap            m_observations; //!< A set of observations, ordered by phenomenon time 
        std::vector<te::dt::Property*>  m_properties;   //!< The properties of the observed data          
    };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_OBSERVATIONSET_H

