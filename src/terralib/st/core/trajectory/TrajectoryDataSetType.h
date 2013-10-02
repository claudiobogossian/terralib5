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
  \file TrajectoryDataSetType.h

  \brief This file contains a class called TrajectoryDataSetType 
*/

#ifndef __TERRALIB_ST_INTERNAL_TRAJECTORYDATASETTYPE_H
#define __TERRALIB_ST_INTERNAL_TRAJECTORYDATASETTYPE_H

//ST
#include "../observation/ObservationDataSetType.h"

//STL
#include <string>
#include <vector>

namespace te
{
  namespace st
  {
    /*!
      \class TrajectoryDataSetType

      \brief  A class that contains complemental information 
              about a DataSet that holds observations of a trajectory. 

      A trajectory represents the variation of spatial locations or extents of an 
      object over time. 

      \sa ObservationDataSet ObservationDataSetType
    */
    class TESTEXPORT TrajectoryDataSetType
    {
      public:

        /*! 
          \brief Constructor. 
          
          \param tPropIdx   The index of the property that contains the times of a trajectory. 
          \param gPropIdx   The index of the property that contains the geometries of a trajectory. 
          \param idPropIdx  The index of the property that contains the identity of the trajectory. 
          \param id         The trajectory id. 
        */
        TrajectoryDataSetType(int tPropIdx, int gPropIdx, int idPropIdx, const std::string& id); 
        
        /*! 
          \brief Constructor. 
          
          \param tPropIdxs       The indexes of the properties that contains the beginning and final times of a trajectory.
          \param gPropIdx        The index of the property that contains the geometries of a trajectory. 
          \param idPropIdx       The index of the property that contains the identity of the trajectory. 
          \param id              The trajectory id. 

          \note This constructor is used when each observation of a trajectory is associated to a period and
                the DataSet uses two properties to store these periods.
        */
        TrajectoryDataSetType(const std::vector<int>& tPropIdxs, int gPropIdx, int idPropIdx, 
                              const std::string& id); 
                
        /*! 
          \brief Constructor. 
          
          \param type         The ObservationSetType   
          \param idPropIdx    The property index that contains the object identification. 
          \param id           The trajectory id. 

          \note idPropIdx equal to "-1" means that the DataSet contains observations of only one trajectory.
        */
        TrajectoryDataSetType(const ObservationDataSetType& type, int idPropIdx, const std::string& id); 
               
        /*!
          \brief It returns the observation set type.

          \return The observation set type.
        */
        const ObservationDataSetType& getType() const;
        
        /*!
          \brief It returns the property index that contains the trajectory geometries. 

          \return The property index that contains the trajectory geometries.
        */
        int getGeomPropIdx() const;

        /*!
          \brief It returns the property index that contains the times associated to the trajectory. 

          \return The property index that contains the times associated to the trajectory. 
        */
        int getTimePropIdx() const;

        /*!
          \brief It returns the property index that contains the begin time associated to the trajectory. 

          \return The property index that contains the begin times associated to the trajectory. 
        */
        int getBeginTimePropIdx() const;

        /*!
          \brief It returns the property index that contains the final time associated to the trajectory. 

          \return The property index that contains the end time associated to the trajectory. 
        */
        int getEndTimePropIdx() const;

        /*!
          \brief It returns the property index that contains the object identifications.

          \return The property index that contains the object identifications.
        */
        int getIdPropIdx() const;

        /*!
          \brief It sets the property index that contains the object identifications.

          \param idx The property index that contains the object identifications.
        */
        void setIdPropIdx(int idx);

        /*!
          \brief It returns the trajectory id.

          \return The trajectory id.
        */
        std::string getId() const;

        /*!
          \brief It sets the trajectory id.

          \param id The trajectory id.
        */
        void setId(const std::string& id);
                
        /*! \brief Virtual destructor. */
        virtual ~TrajectoryDataSetType(); 

      protected:

        ObservationDataSetType  m_obsType;      //!< The internal observation set type
        int                     m_idPropIdx;    //!< The property index that contains the object identification. 
        std::string             m_id;           //!< The trajectory id
    };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TRAJECTORYDATASETTYPE_H

