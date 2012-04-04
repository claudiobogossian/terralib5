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
  \file Observations.h

  \brief This file contains a class to represent an observation.
 */

#ifndef __TERRALIB_ST_INTERNAL_OBSERVATION_H
#define __TERRALIB_ST_INTERNAL_OBSERVATION_H

//STL
#include <vector>

// DyGeo
#include "../Config.h"

// Forward declarations
namespace te 
{ 
  namespace dt 
  { 
    class AbstractData; 
    class DateTime; 
    class DateTimePeriod;
    class DateTimeInstant;
   } 
}

namespace te
{
  namespace st
  {
    // Forward declarations
    class ObservationSet;

    /*!
      \class Observation

      \brief A class to represent an observation.

      An observation is a result of a measurement associated to a discrete 
      time instant or period which assigns values to a phenomenon. It follows
      the OGC's Observation and Measurement (O&M) specification.
      An observation is composed of: (1) a phenomenon time, (2) a valid time, 
      (3) a result time, (4) observed values. 

      \sa AbstractData DateTime 
    */
    class TESTEXPORT Observation
    {
      public:

        /*! \brief Constructor. */
        Observation(); 
        
        /*! \brief Copy constructor. */
        Observation(const Observation& obs); 
        
        /*! 
          \brief Constructor. 
          
          \param phTime  A pointer to phenomenon time. 
          \param parent  A pointer to its parent.

          \note It will take the ownership of the DateTime input pointer.
          \note It will not take the ownership of the ObservationSet input pointer.
        */
        Observation(te::dt::DateTime* phTime, ObservationSet* parent);
        
        /*! 
          \brief Constructor. 
          
          \param phTime   A pointer to phenomenon time. 
          \param obsValue A observed value
          \param parent   A pointer to its parent.

          \note It will take the ownership of the DateTime and AbstractData input pointers.
          \note It will not take the ownership of the ObservationSet input pointer.
        */
        Observation(te::dt::DateTime* phTime, te::dt::AbstractData* obsValue, ObservationSet* parent = 0);

        /*! 
          \brief Constructor. 
          
          \param phTime     A pointer to phenomenon time.
          \param obsValues  The observed values. 
          \param parent     A pointer to its parent.

          \note It will take the ownership of the input pointers.
          \note It will not take the ownership of the ObservationSet input pointer.
        */
        Observation(te::dt::DateTime* phTime, const std::vector<te::dt::AbstractData*>& obsValues, ObservationSet* parent);

        /*! 
          \brief Constructor. 
          
          \param phTime     A pointer to phenomenon time.
          \param resTime    A pointer to the result time.
          \param valTime    A pointer to the valid time.
          \param obsValues  The observed values. 
          \param parent     A pointer to its parent.

         \note It will take the ownership of the input pointers.
         \note It will not take the ownership of the ObservationSet input pointer.
        */
        Observation(te::dt::DateTime* phTime, te::dt::DateTimeInstant* resTime, 
                    te::dt::DateTimePeriod* valTime, const std::vector<te::dt::AbstractData*>& obsValues,
                    ObservationSet* parent);           

        /*! 
          \brief Assignment operator.
        */
        const Observation& operator=(const Observation& rhs);
        
        /*!
          \brief It returns the phenomenon time.

          \return A pointer to the phenomenon time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::dt::DateTime* getTime() const;
        
        /*!
          \brief It sets the phenomenon time.

          \param A pointer to the the phenomenon time.

          \note It will take the ownership of the input pointer.
        */
        void setTime(te::dt::DateTime* phTime);

        /*!
          \brief It returns the result time.

          \return A pointer to the result time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::dt::DateTimeInstant* getResultTime() const;
        
        /*!
          \brief It sets the result time.

          \param A pointer to the result time.

          \note It will take the ownership of the input pointer.
         */
        void setResultTime(te::dt::DateTimeInstant* resTime);

         /*!
          \brief It returns the valid time.

          \return A pointer to the valid time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::dt::DateTimePeriod* getValidTime() const;
        
        /*!
          \brief It sets the valid time.

          \param A pointer to the valid time.

          \note It will take the ownership of the input pointer.
         */
        void setValidTime(te::dt::DateTimePeriod* valTime);

        /*!
          \brief It returns the observed values.

          \return A reference to the observed values.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        std::vector<te::dt::AbstractData*>& getObservedValues();

        /*!
          \brief It returns the idx-th observed value.

          \return A pointer to the idx-th observed value.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::dt::AbstractData* getObservedValue(int idx = 0) const;        

        /*!
          \brief It adds an observed value.

          \param A pointer to the observed value to be added.

          \note It will take the ownership of the input pointer.
        */
        void addValue(te::dt::AbstractData* value);

        /*!
          \brief It sets the observed values.

          \param A reference to the observed values.

          \note It will take the ownership of the input pointer.
        */
        void setValues(const std::vector<te::dt::AbstractData*>& values);

        /*!
          \brief It returns the observation set parent.

          \return A pointer to the observation set parent.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        ObservationSet* getParent() const;

        /*!
          \brief It sets the observation set parent.

          \param A pointer to the parent.

          \note It will take the ownership of the input pointer 
        */
        void setParent(ObservationSet* parent);

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.

          \note The caller will take the ownership of the input pointer.  
        */
        Observation* clone() const;
        
        /*! \brief Virtual destructor. */
        virtual ~Observation();        

      private:

        te::dt::DateTime*                     m_phTime;               //!< The phenomenon time
        te::dt::DateTimeInstant*              m_resultTime;           //!< The result time
        te::dt::DateTimePeriod*               m_validTime;            //!< The valid time
        std::vector<te::dt::AbstractData*>    m_observedValues;       //!< The observed values
        ObservationSet*                       m_parent;               //!< The pointer to the parent observation set.
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_OBSERVATION_H

