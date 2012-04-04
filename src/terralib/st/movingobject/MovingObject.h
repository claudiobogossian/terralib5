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
  \file MovingObject.h

  \brief This file contains a class to represent a moving object
 */

#ifndef __TERRALIB_ST_INTERNAL_MOVINGOBJECT_H
#define __TERRALIB_ST_INTERNAL_MOVINGOBJECT_H

//STL
#include <vector>

//ST
#include "../Config.h"

// Forward declarations
namespace te { namespace dt { class DateTime; class DateTimePeriod; } }
namespace te { namespace gm { class Geometry; class GeometryProperty; } }

namespace te
{
  namespace st
  {
    // Forward declarations
    class TimeSeries;
    class AbstractInterpolator;
    class MovingObjectIterator;
    class SpatioTemporalObservationSet;
    class Trajectory;

    /*!
      \class MovingObject

      \brief A class to represent moving object.

      A moving object is an entity whose spatial positions or extents 
      change continuously over time (T -> S). 
      It is composed of a set of observations and of an interpolator able
      to estimate spaces at non-observed times.

      \TODO operations: distance, intersection, reaches, leaves, etc...

      \sa ObservationSet AbstractInterpolator
    */
    class TESTEXPORT MovingObject
    {
      public:

        /*! \name MovingObject Constructors */
        //@{
        
        /*! \brief Constructor. */
        MovingObject(); 

        /*! 
          \brief Constructor. 
          
          \param obs        A pointer to moving object observations.
         
          \note It will take the ownership of the input pointer.
        */
        MovingObject(SpatioTemporalObservationSet* obs);

        /*! 
          \brief Constructor. 
          
          \param obs        A pointer to moving object observations.
          \param id         The moving object identifier
         
          \note It will take the ownership of the input pointer.
        */
        MovingObject(SpatioTemporalObservationSet* obs, const std::string& id);

         /*! 
          \brief Constructor. 
          
          \param obs        A pointer to moving object observations.
          \param id         The moving object identifier.
          \param interp     A pointer to an interpolator.         

          \note It will take the ownership of the first input pointer (SpatioTemporalObservationSet* ).
          \note It will NOT take the ownership of the last input pointer (AbstractInterpolator*).
        
        */
        MovingObject(SpatioTemporalObservationSet* obs, const std::string& id, AbstractInterpolator* interp);

        /*! 
          \brief Constructor. 
          
          \param prop The geometry property.  
          \param id   The moving object identifier.  
        
          \note It will take the ownership of the input pointer.
        */
        MovingObject(te::gm::GeometryProperty* prop, const std::string& id = "-1");
        //@}

        /*!
          \brief It returns a moving object iterator that points to its first observation.

          \return A moving object iterator.        
        */
        MovingObjectIterator begin();

        /*!
          \brief It returns a moving object iterator that points to the end of its observation set.

          \return A moving object iterator.        
        */
        MovingObjectIterator end();

         /*!
          \brief It returns the moving object geometry property.

          \return The moving object geometry property.

          \note The caller will not take the ownership of the returned pointer.
        */
        te::gm::GeometryProperty* getProperty();

        /*!
          \brief It sets the moving object observations.

          \param obs      A pointer to the moving object observations.
        
          \note It will take the ownership of the input pointer.
         */
        void setObservationSet(SpatioTemporalObservationSet* obs);

        /*!
          \brief It returns the moving object observations.

          \return A pointer to the moving object observations.
        
          \note The caller will NOT take the ownership of the returned pointer.
        */
        SpatioTemporalObservationSet* getObservationSet();

         /*!
          \brief It adds an observation to moving object.

          \param phTime   The phenomenon time. 
          \param geom     The observed geometry.
         
          \note It will take the ownership of the input pointer.
        */
        void addObservation(te::dt::DateTime* phTime, te::gm::Geometry* gem);

        /*!
          \brief It returns the size of the moving object observation set.

          \return The observation set size of the moving object.
        */
        std::size_t size() const;        

        /*!
          \brief It returns the geometry associated to a given date and time.

          If there is no geometry associated to the given date and time, it will
          use internally its interpolation function.

          \param t  A date and time.
           
          \return   A pointer to the geometry associated to the given date and time.

          \note The caller will take the ownership of the returned pointer.
        */
        te::gm::Geometry* getGeometry(te::dt::DateTime* t);

        /*!
          \brief It returns the interpolator associated to the moving object.

          \return A pointer the interpolator associated to the moving object.
        */
        AbstractInterpolator* getInterpolator() const;

        /*!
          \brief It sets the interpolator associated to the moving object.

          \param interp A interpolator which is able to estimate geometries in non-observed times.

          \note It will NOT take the ownership of the input pointer.
        */
        void setInterpolator(AbstractInterpolator* interp);
       
        /*!
          \brief It returns the moving object identifier.

          \return The moving object identifier.
        */
        std::string getId() const;
        
        /*!
          \brief It sets the moving object identifier.

          \param id The moving object identifier.
        */
        void setId(const std::string& id);          
        
        /*!
          \brief It returns the temporal extent of the moving object.

          \return The temporal extent of the moving object.

          \note The caller will take the ownership of the output pointer.
        */
        te::dt::DateTimePeriod* temporalExtent();

        /*! \name MovingObject Operations */
        //@{

        /*!
          \brief It returns the distance between two moving objects.

          \param mo The other moving object.
          \return a time series that relates the distance at each time.
          
          \note The caller will take the ownership of the returned pointer. 
        */
        TimeSeries* distance(MovingObject* other);

        /*!
          \brief It returns the moving object trajectories whose geometries intersect a given geometry. 

          \param geom A given geometry.
          \param 
          \return a time series that relates the distance at each time.
          
          \note The caller will take the ownership of the returned pointer. 
        */
        void intersection(te::gm::Geometry* geom, std::vector<Trajectory*>& output);

        /*!
          \brief It returns the moving object trajectories whose geometries do not intersect a given geometry. 

          \param geom A given geometry.
          \param 
          \return a time series that relates the distance at each time.
          
          \note The caller will take the ownership of the returned pointer. 
        */
        void difference(te::gm::Geometry* geom, std::vector<Trajectory*>& output);
        //@}
               
        /*! \brief Virtual destructor. */
        virtual ~MovingObject(); 

      private:

        SpatioTemporalObservationSet*   m_observations;    //!< The moving object observations 
        int                             m_geomPropIdx;     //!< The geometry attribute index of the moving object observation set.
        AbstractInterpolator*           m_interpolator;    //!< The interpolators used to estimate non-observed times.
        std::string                     m_id;              //!< The moving object identification.
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_MOVINGOBJECT_H

