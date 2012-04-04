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
  \file SpatioTemporalObservationSet.h

  \brief This file contains an observation set that has one geometry observed property. 
*/

#ifndef __TERRALIB_ST_INTERNAL_SPATIOTEMPORALOBSERVATIONSET_H
#define __TERRALIB_ST_INTERNAL_SPATIOTEMPORALOBSERVATIONSET_H

//TerraLib
#include "../../sam/rtree/Index.h"

//ST
#include "ObservationSet.h"
#include "../Config.h"

// Forward declarations
namespace te { namespace gm { class Envelope; class Geometry; } }

namespace te
{
  namespace st
  {    
       
    /*!
      \class SpatioTemporalObservationSet

      \brief A class to represent an observation set that has one spatial observed property. 

      \TODO: 
      \TODO: 

      \sa Observation
    */
    class TESTEXPORT SpatioTemporalObservationSet : public ObservationSet
    {
      public:

        /*! \brief Constructor. */
        SpatioTemporalObservationSet(); 

        /*! 
          \brief Constructor. 
          
          \param obs          A set of observations
          \param props        The properties associated to the observed values
          \param geomPropIdx  The index of the geometry property

          \note It will take the ownership of the input pointers.
        */
        SpatioTemporalObservationSet( const std::vector<Observation*>& obs , 
                                      const std::vector<te::dt::Property*>& props, int geomPropIdx);

        /*! 
          \brief Constructor. 
          
          \param props        The properties associated to the observed values
          \param geomPropIdx  The index of the geometry property
         
          \note It will take the ownership of the input pointers.
        */
        SpatioTemporalObservationSet(const std::vector<te::dt::Property*>& props, int geomPropIdx);

        /*!
          \brief It returns the geometry property index.

          \return ob The geometry property index.
        */
        int getGeometryPropIdx() const;

        /*!
          \brief It sets the geometry property index.

          \param idx The geometry property index.
        */
        void setGeometryPropIdx(int idx);        
        
        /*!
          \brief It clears the observation set.
        */
        void clear();

        /*!
          \brief It adds an observation to the set.

          \param ob The observation to be added.

          \note It will take the ownership of the input pointer.
        */
        void addObservation(Observation* ob);       

        /*!
          \brief It returns the spatial extent of the observations, when there is an observed value of geometry type.

          \return The spatial extent of the observations.

          \note The caller will take the ownership of the returned pointer. 
        */
        te::gm::Envelope* spatialExtent();

        /*!
          \brief Returns the observations whose geometry intersects a given geometry. 

          It returns the internal observations 
          whose geometry intersects a given geometry.

          \param  geom    A given geometry.
          \param  result  A vector to put the resulted observations (OUTPUT). 

          \note The caller will NOT take the ownership of the output pointer.
        */
        void intersection(te::gm::Geometry* geom, std::vector<Observation*>& result); 

        /*!
          \brief Returns the observations whose geometry is disjoint to a given geometry. 

          It returns the internal observations 
          whose geometry is disjoint to a given geometry.

          \param  geom    A given geometry.
          \param  result  A vector to put the resulted observations (OUTPUT). 

          \note The caller will NOT take the ownership of the output pointer.
        */
        void difference(te::gm::Geometry* geom, std::vector<Observation*>& result); 

        /*! \brief Virtual destructor. */
        virtual ~SpatioTemporalObservationSet(); 

      private:
             
        int                                   m_geomPropIdx;    //!< The index of the geometry property. 
        te::sam::rtree::Index<Observation*>*  m_RTree;          //!< A RTree index created over a geometry property            
    };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_SPATIOTEMPORALOBSERVATIONSET_H

