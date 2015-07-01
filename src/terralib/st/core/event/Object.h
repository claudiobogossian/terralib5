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
  \file Object.h

  \brief This file contains a class to represent an object.
 */

#ifndef __TERRALIB_ST_INTERNAL_OBJECT_H
#define __TERRALIB_ST_INTERNAL_OBJECT_H

//STL
#include <vector>

//ST
#include "../../Config.h"

// Forward declarations
namespace te { namespace dt { class AbstractData; class DateTime;  } }
namespace te { namespace gm { class Geometry;  } }

namespace te
{
  namespace st
  {
    // Forward declarations
    class TimeSeries;
    class Trajectory;
   
    /*!
      \class Object

      \brief A class to represent an object.

      An object is an identifiable entity whose spatial and non-spatial properties
      can change. The spatial property variation is represented by a trajectory
      and the non-spatial property variation by a time series.
      This first version will consider only a non-spatial property.

      \sa TimeSeries Trajectory
    */
    class TESTEXPORT Object
    {
      public:

        /*! \name Object Constructors */
        //@{
        
        /*! 
          \brief Constructor. 

          \param id     The identification of the object. 
          \param ts     The time series that represents the variation of a non-spatial property. 
          \param tj     The trajectory that represents the variation of the spatial property. 
                
          \note It will take the ownership of the input pointers.
        */
        Object(const std::string& id, TimeSeries* ts, Trajectory* tj);
        
        /*! 
          \brief Constructor. 

          \param id     The identification of the object. 
          \param ts     The time series that represents the variation of a non-spatial property. 
                        
          \note It will take the ownership of the input pointers.
        */
        Object(const std::string& id, TimeSeries* ts);

        /*! 
          \brief Constructor. 

          \param id     The identification of the object. 
          \param tj     The trajectory that represents the variation of the spatial property. 
                
          \note It will take the ownership of the input pointers.
        */
        Object(const std::string& id, Trajectory* tj);
        
        /*! 
          \brief Copy constructor. 
        */
        Object(const Object& obj);
        //@}

        /*! 
          \brief Copy assignment operator
        */
        Object& operator=(const Object& other);

        /*!
          \brief It returns a clone of this object. 

          \return A new object. 

          \note The caller will take the ownership of the returned pointer.
        */
        Object* clone() const;

        /*!
          \brief It returns the object identification.

          \return The object identification.
        */
        std::string getId() const;
        
        /*!
          \brief It sets the object identification.

          \param id The object identification.
        */
        void setId(const std::string& id);

        /*!
          \brief It returns the time series associated to the object.

          \return A pointer to the time series associated to the object.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        TimeSeries* getTimeSeries() const;

        /*!
          \brief It sets the time series associated to the object.

          \param ts The time series to be set. 

          \note It will take the ownership of the input pointer.
        */
        void setTimeSeries(TimeSeries* ts);

        /*!
          \brief It returns the trajectory associated to the object.

          \return A pointer to the trajectory associated to the object.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        Trajectory* getTrajectory() const;

        /*!
          \brief It sets the trajectory associated to the object.

          \param ts The trajectory associated to the object.

          \note It will take the ownership of the input pointer.
        */
        void setTrajectory(Trajectory* tj);

        /*!
          \brief It returns the state of the object at a given date and time. 

          \param t The given date and time. 

          \return  A pair of the value and geometry of the object at the given time. 

          \note The caller will take the ownership of the returned pointers.
        */
        std::pair<te::dt::AbstractData*,te::gm::Geometry*> getState(te::dt::DateTime* t) const;
          
        /*! \brief Virtual destructor. */
        virtual ~Object(); 

      private:
                  
        std::string                           m_id;           //!< The time series identification.
        std::auto_ptr<Trajectory>             m_trajectory;   //!< The variation of the spatial property of the object
        std::auto_ptr<TimeSeries>             m_timeseries;   //!< The variation of a property of the object
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_OBJECT_H

