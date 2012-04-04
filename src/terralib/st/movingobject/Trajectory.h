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

  \brief This file contains a class to a moving object trajectory
 */

#ifndef __TERRALIB_ST_INTERNAL_TRAJECTORY_H
#define __TERRALIB_ST_INTERNAL_TRAJECTORY_H

//STL
#include <vector>

// ST
#include "../Config.h"
#include "MovingObjectIterator.h"
#include "TrajectoryIterator.h"

namespace te
{
  namespace st
  {
    // Forward declarations
    class MovingObject;

    /*!
      \class Trajectory

      \brief A class to represent a trajectory of a moving object. 
      
      A trajectory is a journey of a moving object...

      \sa MovingObject
    */
    class TESTEXPORT Trajectory
    {
      public:

        /*! \name Trajectory Constructors */
        //@{
        
        /*! \brief Constructor. */
        Trajectory(); 

        /*! 
          \brief Trajectory. 
          
          \param itBegin A pointer to the trajectory begin.
          \param itEnd   A pointer to a position after the trajectory end. 
          \param parent  A pointer to its moving object parent. 

          \note It will NOT take the ownership of the input pointer (MovingObject*).
        */
        Trajectory(MovingObjectIterator itBegin, MovingObjectIterator itEnd, MovingObject* parent);
        //@}
        
        /*!
          \brief It returns a pointer to the first trajectory observation.

          \return A trajectory iterator.         
        */
        TrajectoryIterator begin();

        /*!
          \brief It returns a pointer to a position after the final trajectory observation. 

          \return A trajectory iterator.        
        */
        TrajectoryIterator end();

        /*!
          \brief It returns the moving object associated to this trajectory. 

          \return A pointer to its associated moving object. 

          \note The caller WILL NOT take the ownership of the returned pointer.
        */
        te::st::MovingObject* getParent();         
        
        /*!
          \brief It returns a new moving object composed of all trajectory observations. 

          \return A new moving object. 

          \note The caller WILL take the ownership of the returned pointer.
        */
        te::st::MovingObject* getMovingObject();    
               
        /*! \brief Virtual destructor. */
        virtual ~Trajectory() { }  

      private:

        MovingObjectIterator     m_begin;   //!< The moving object trajectory begin 
        MovingObjectIterator     m_end;     //!< The moving object trajectory end
        MovingObject*            m_parent;  //!< The interpolators used to estimate non-observed times.
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TRAJECTORY_H

