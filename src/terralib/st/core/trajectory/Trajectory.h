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
  \file Trajectory.h

  \brief This file contains a class to represent a trajectory.
 */

#ifndef __TERRALIB_ST_INTERNAL_TRAJECTORY_H
#define __TERRALIB_ST_INTERNAL_TRAJECTORY_H

//TerraLib
#include "../../../sam/rtree/Index.h"

//ST
#include "../../Config.h"
#include "../../Enums.h"
#include "TrajectoryObservation.h"
#include "TrajectoryIterator.h"

//STL
#include <vector>
#include <map>
#include <memory>

// Boost
#include <boost/shared_ptr.hpp>

// Forward declarations
namespace te { namespace dt { class DateTime; class DateTimePeriod; } }
namespace te { namespace gm { class Geometry; class Envelope; } }

namespace te
{
  namespace st
  {
    // Forward declarations
    class TimeSeries;
    class AbstractTrajectoryInterp;
       
    // Typedef
    typedef boost::shared_ptr<te::sam::rtree::Index<te::dt::DateTime*> >  TjRTreeShrPtr; 
        
    /*!
      \class Trajectory

      \brief A class to represent trajectory.

      A trajectory represents the variation of spatial locations or boundaries of an 
      object over time. It is composed of an observation set and of an interpolator.
      The observations have a fixed object identity and measured geometries at controlled
      times. The interpolator is able to estimate spaces at non-observed times.

      \ingroup st

      \sa ObservationSet AbstractInterpolator
    */
    class TESTEXPORT Trajectory
    {
      public:

        /*! \name Trajectory Constructors */
        //@{
        
        /*! 
          \brief Empty constructor. 

          It constructs an empty trajectory. The user does not indicate the interpolator
          associated to it. Internally, it uses a default interpolator
          that is NearestGeometryAtTimeInterp.
        */
        Trajectory();

        /*! 
          \brief Empty constructor. 

          It constructs an empty trajectory. The user does not indicate the interpolator
          associated to it. Internally, it uses a default interpolator
          that is NearestGeometryAtTimeInterp.

          \param id The trajectory id
        */
        Trajectory(const std::string& id);

        /*! 
          \brief Empty constructor. 

          It constructs an empty trajectory. 

          \param interp The interpolator associated to the trajectory.
          \param id     The trajectory id.       
        
        */
        Trajectory(AbstractTrajectoryInterp* interp, const std::string& id);

        /*! 
          \brief Constructor. 

          The user does not indicate the interpolator
          associated to it. Internally, it uses a default interpolator
          that is NearestGeometryAtTimeInterp.

          \param obs        The trajectory observations.
          \param id         The trajectory id.

          \note This constructor does not build the internal RTree
        */
        Trajectory(const TrajectoryObservationSet& obs, const std::string& id);
                    
        /*! 
          \brief Constructor. 

          \param obs        The trajectory observations.
          \param interp     The interpolator associated to the trajectory.
          \param id         The trajectory id.

          \note This constructor does not build the internal RTree
        */
        Trajectory( const TrajectoryObservationSet& obs, AbstractTrajectoryInterp* interp, 
                    const std::string& id);

        /*! 
          \brief It constructs a trajectory from a patch

          \param patch  The given patch.
          \param interp The interpolator
          \param id     The trajectory id
        */
        Trajectory(const TrajectoryPatch& patch, AbstractTrajectoryInterp* interp, 
                    const std::string& id);
        
        /*! 
          \brief Copy constructor. 

          \note They will share the internal pointers. 
          \note The caller has to use the method "clone" to have a deep copy. 
        */
        Trajectory(const Trajectory& tj);
        //@}

        /*! 
          \brief Copy assignment operator

          \note They will share the internal pointers. 
          \note The caller has to use the method "clone" to have a deep copy. 
        */
        Trajectory& operator=(const Trajectory& other);

        /*!
          \brief It returns a clone of this trajectory.  

          \return A new trajectory

          \note The caller will take the ownership of the returned pointer.
        */
        Trajectory* clone() const; 

        /*!
          \brief It returns the trajectory observations.

          \return A reference to the trajectory observations.
        */
        const TrajectoryObservationSet& getObservations() const;
        
        /*!
          \brief It returns the interpolator associated to the trajectory.

          \return A pointer the interpolator associated to the trajectory.

          \note The caller will NOT take the ownership of the returned pointer.
          \note The AbstractTrajectoryInterp points to a singleton. 
        */
        AbstractTrajectoryInterp* getInterpolator() const;

        /*!
          \brief It sets the interpolator associated to the path.

          \param interp A interpolator which is able to estimate geometries in non-observed times.

          \note It will NOT take the ownership of the input pointer.
          \note The AbstractTrajectoryInterp points to a singleton. 
        */
        void setInterpolator(AbstractTrajectoryInterp* interp);

        /*!
          \brief It returns the trajectory id.

          \return The trajectory id.
        */
        std::string getId() const;
        
        /*!
          \brief It sets the trajectory id.

          \param id The trajectory od. 
        */
        void setId(const std::string& id);   

        /*! 
          \brief It adds an observation (time and geometry) into the trajectory. 

          \param time A pointer to the time.
          \param data A pointer to the geometry.
          
          \note The caller will take the ownership of the given pointers.    
        */  
        void add(te::dt::DateTime* time, te::gm::Geometry* geom);

        /*! 
          \brief It adds an observation (time and geometry) into the trajectory. 

          \param p A pair of date and time. 
          
          \note The caller will take the ownership of the returned pointer.    
        */  
        void add(const TrajectoryObservation& p); 
                
        /*!
          \brief It returns the size of the trajectory observations.

          \return The observations size of the trajectory.
        */
        std::size_t size() const;    
        
        /*! \name Trajectory Iterator 
                  
                  An example of use:

                  TrajectoryIterator it = tj.begin();
                  while(it!=tj.end())
                  {
                    Geometry* g = it.getGeometry();
                    DateTime* t = it.getTime();
                    ++it;
                  } 
        */
        //@{
        /*!
          \brief It returns an iterator that points to the first observation of the trajectory.
          
          \return The trajectory iterator.
        */
        TrajectoryIterator begin() const;

        /*!
          \brief It returns an iterator that points to the end of the trajectory.
          
          \return The trajectory iterator.
        */
        TrajectoryIterator end() const;

        /*!
          \brief It returns an iterator that points to an observation at a given time.
          
          If there is no observation at this given time, the returned iterator
          points to the end of the trajectory.
          
          \return The trajectory iterator.
          \note This does not take the ownership of the given pointer.
        */
        TrajectoryIterator at(te::dt::DateTime* t) const;
        //@}
        
        /*!
          \brief It returns the geometry associated to a given date and time.

          If there is no geometry associated to the given date and time, it will
          use internally its interpolation function.

          \param t  A date and time.
           
          \return   A pointer to the geometry associated to the given date and time.

          \note The caller WILL take the ownership of the returned pointer.
        */
        std::auto_ptr<te::gm::Geometry> getGeometry(te::dt::DateTime* t) const;                                     

        /*!
          \brief It returns the temporal extent of the trajectory observations.

          \return The temporal extent of trajectory observations.

          \note The caller will take the ownership of the output pointer.
        */
        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent() const;

        /*!
          \brief It returns the spatial extent of the trajectory observations.

          \return The spatial extent of the trajectory observations.

          \note The caller will take the ownership of the output pointer.
        */
        te::gm::Envelope getSpatialExtent() const;
                
        /*! \name Trajectory Patch 
            
            These methods return patches of a trajectory, as TrajectoryPatch objects,
            that satisfy given restrictions. 
            A TrajectoryPatch object contains only pointers to the initial and 
            final observations that delimit a patch of a Trajectory. The 
            end() method of a TrajectoryPatch object returns an iterator
            that points to the position AFTER the last required observation.

            Example of use:

            TrajectoryPatch patch = tj->getTrajectory(period, "DURING");
            TrajectoryIterator it = patch.begin();
            while(it!=patch.end())
            {
               Geometry* g = it.getGeometry();
               DateTime* t = it.getTime();
               ++it;
            } 
            
            Other option is to create a new Trajectory from the returned patch: 
            
            TrajectoryPatch patch = tj->getTrajectory(period, "DURING");
            Trajectory tjnew(patch); 
        */
        //@{
        /*!
          \brief It returns a trajectory subset that satisfies a given temporal relation. 
          
          The possible temporal relations are: AFTER (2); 
          AFTER | EQUALS (10); 3. BEFORE (1);  4. BEFORE | EQUALS (9); 
          DURING (4); EQUALS (8) 

          \param dt   A given date and time.
          \param r    A given temporal relation.
                   
          \return   A patch of the trajectory.

          \note When the temporal relation is DURING, dt must be a time period.
        */
        TrajectoryPatch getPatch(const te::dt::DateTime& dt, te::dt::TemporalRelation r = te::dt::DURING) const;

        /*!
          \brief It returns trajectory subsets that satisfy a given spatial relation. 
          
          The possible spatial relation is: 1. INTERSECTS

          \param g      A given envelope.
          \param r      A given spatial relation.
          \param result The returned patches.
        */
        void getPatches(const te::gm::Envelope& g, te::gm::SpatialRelation r, 
                      std::vector<TrajectoryPatch>& result) const;
        
        /*!
          \brief It returns trajectory subsets that satisfy a given spatial relation. 
          
          The possible spatial relation is: 1. INTERSECTS

          \param geom   A given geometry.
          \param r      A given spatial relation.
          \param result The returned patches.
        */
        void getPatches(const te::gm::Geometry& geom, te::gm::SpatialRelation r, 
                      std::vector<TrajectoryPatch>& result) const;

        /*!
          \brief It returns trajectory subsets that satisfy a given spatial 
                 and temporal relation. 
          
          The possible temporal relations are: AFTER (2); 
          AFTER | EQUALS (10); 3. BEFORE (1);  4. BEFORE | EQUALS (9); 
          DURING (4); EQUALS (8) 

          The possible spatial relation is: 1. INTERSECTS

          \param e    A given envelope.
          \param r    A given spatial relation.
          \param dt   A given date and time.
          \param r    A given temporal relation.
          \param result The returned patches.
        */
        void getPatches(const te::gm::Envelope& e, te::gm::SpatialRelation sr, 
                      const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                      std::vector<TrajectoryPatch>& result) const;
        
        /*!
          \brief It returns trajectory subsets that satisfy a given spatial 
                 and temporal relation. 
          
          The possible temporal relations are: AFTER (2); 
          AFTER | EQUALS (10); 3. BEFORE (1);  4. BEFORE | EQUALS (9); 
          DURING (4); EQUALS (8)  

          The possible spatial relation is: 1. INTERSECTS

          \param geom   A given geometry.
          \param r      A given spatial relation.
          \param dt   A given date and time.
          \param r    A given temporal relation.
          \param result The returned patches.
        */
        void getPatches(const te::gm::Geometry& geom, te::gm::SpatialRelation sr, 
                      const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                      std::vector<TrajectoryPatch>& result) const;

        /*!
          \brief It returns trajectory subsets that satisfy a given 
          spatiotemporal relation. 
          
          The possible spatiotemporal relations are: 
          1. REACH; 2. EXITS; 3. REACHES;  4. LEAVES 

          \param geom   A given geometry.
          \param srt    A given spatiotemporal relation.
          \param result The returned patches.
        */
        void getPatches(const te::gm::Geometry& geom, te::st::SpatioTemporalRelation str, 
                           const std::vector<TrajectoryPatch>& result) const;
        //@}
                        
        /*!
          \brief It returns the distance between two trajectories.

          \param other  The other trajectory.
          \return a time series that maps times to the distances.
          
          \note The caller will take the ownership of the returned pointer. 
        */
        std::auto_ptr<TimeSeries> getDistance(const Trajectory& other) const;     
        
        /*!
          \brief It returns the distance between the trajectory and a given geometry. 

          \param geom A given geometry.  
          \return a time series that maps times to the distances.
          
          \note The caller will take the ownership of the returned pointer. 
        */
        std::auto_ptr<TimeSeries> getDistance(const te::gm::Geometry& geom);         
               
        /*! \brief Virtual destructor. */
        virtual ~Trajectory(); 

      private:
               
        TrajectoryObservationSet       m_observations;    //!< The trajectory observations 
        AbstractTrajectoryInterp*      m_interpolator;    //!< The interpolator used to estimate non-observed times.
        std::string                    m_id;              //!< The trajectory identification.
        TjRTreeShrPtr                  m_rtree;           //!< The RTree to index the trajectory geometries
      };
   } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TRAJECTORY_H

