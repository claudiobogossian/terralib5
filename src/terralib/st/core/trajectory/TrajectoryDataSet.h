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
  \file TrajectoryDataSet.h

  \brief This file contains a class to represent a trajectory data set.
 */

#ifndef __TERRALIB_ST_INTERNAL_TRAJECTORYDATASET_H
#define __TERRALIB_ST_INTERNAL_TRAJECTORYDATASET_H

//ST
#include "../../Config.h"
#include "TrajectoryDataSetType.h"

// Boost
#include <boost/noncopyable.hpp>

//STL
#include <vector>
#include <memory>

// Forward declarations
namespace te { namespace dt { class DateTime; class DateTimePeriod; class DateTimeProperty;} }
namespace te { namespace gm { class Geometry; class Envelope; class GeometryProperty; } }

namespace te
{
  namespace st
  {
    // Forward declarations
    class ObservationDataSet;
    class Trajectory;
    class AbstractTrajectoryInterp; 
 
    /*!
      \class TrajectoryDataSet

      \brief A class to represent a trajectory data set.

      This class represents a view on a DataSet that
      contains observations of a trajectory.

      A trajectory represents the variation of spatial locations or boundaries of an 
      object over time. It is composed of an observation data set where one
      observed property is a geometry.
      The observations have a fixed object identity and measured geometries at controlled
      times. 
      
      \ingroup st

      \sa ObservationDataSet ObservationDatasSetType TrajectoryDataSetType
    */
    class TESTEXPORT TrajectoryDataSet : public boost::noncopyable 
    {
      public:

        /*! \name Constructor */
        //@{
        /*! 
          \brief Constructor. 

          \param ds        The data set that contains the trajectory observations.
          \param tPropIdx  The index of the property that contains the times of a trajectory. 
          \param gPropIdx  The index of the property that contains the geometries of a trajectory. 
          \param idPropIdx  The index of the property that contains the identity of the trajectory. 
          \param id         The trajectory id. 
          
          \note It will take the ownership of the given pointer.
        */
        TrajectoryDataSet(te::da::DataSet* ds, int tPropIdx, int gPropIdx, int idPropIdx, 
                          const std::string& id);

        /*! 
          \brief Constructor. 

          \param ds        The data set that contains the trajectory observations.
          \param tPropIdx  The index of the property that contains the times of a trajectory. 
          \param gPropIdx  The index of the property that contains the geometries of a trajectory. 
          \param idPropIdx  The index of the property that contains the identity of the trajectory. 
          \param id         The trajectory id. 
          \param text       The temporal extent.
          \param sext       The spatial extent.
          
          \note It will take the ownership of the given pointer.
        */
        TrajectoryDataSet(te::da::DataSet* ds, int tPropIdx, int gPropIdx, int idPropIdx, 
                          const std::string& id, te::dt::DateTimePeriod* text, 
                          const te::gm::Envelope& sext);
                
        /*! 
          \brief Constructor. 

          \param ds        The data set that contains the trajectory observations.
          \param tPropIdxs The indexes of the properties that contains the begin and end times of a trajectory. 
          \param gPropIdx  The index of the property that contains the geometries of a trajectory. 
          \param idPropIdx  The index of the property that contains the identity of the trajectory. 
          \param id         The trajectory id. 
          
          \note It will take the ownership of the input pointer.
          \note This constructor is used when each observation of a trajectory is associated to a period and
                the DataSet uses two properties to store these periods.
        */
        TrajectoryDataSet(te::da::DataSet* ds, const std::vector<int>& tPropIdxs, int gPropIdx, 
                          int idPropIdx, const std::string& id);

        /*! 
          \brief Constructor. 

          \param ds        The data set that contains the trajectory observations.
          \param tPropIdxs The indexes of the properties that contains the begin and end times of a trajectory. 
          \param gPropIdx  The index of the property that contains the geometries of a trajectory. 
          \param idPropIdx  The index of the property that contains the identity of the trajectory. 
          \param id         The trajectory id. 
          \param text       The temporal extent.
          \param sext       The spatial extent.
          
          \note It will take the ownership of the input pointer.
          \note This constructor is used when each observation of a trajectory is associated to a period and
                the DataSet uses two properties to store these periods.
        */
        TrajectoryDataSet(te::da::DataSet* ds, const std::vector<int>& tPropIdxs, int gPropIdx, 
                          int idPropIdx, const std::string& id,  te::dt::DateTimePeriod* text, 
                          const te::gm::Envelope& sext);

        /*! 
          \brief Constructor. 

          \param ds         The data set that contains the trajectory observations.
          \param type       The trajectory data set type. 
          \param text       The temporal extent.
          \param sext       The spatial extent.
          
          \note It will take the ownership of the input pointer.
          \note This constructor is used when each observation of a trajectory is associated to a period and
                the DataSet uses two properties to store these periods.
        */
        TrajectoryDataSet(te::da::DataSet* ds, const TrajectoryDataSetType& type, 
                          te::dt::DateTimePeriod* text, const te::gm::Envelope& sext);
                          
        /*! 
          \brief Constructor. 

          \param obs        The data set that contains the trajectory observations.
          \param type       The trajectory data set type.
          \param id         The trajectory id

          \note It will take the ownership of the input pointer.
        */
        TrajectoryDataSet(ObservationDataSet* obs, const TrajectoryDataSetType& type, const std::string& id);
       //@}

        /*!
          \brief It returns the data set that contains the trajectory observations.

          \return A reference to the data set that contains the trajectory observations.

          \note The caller will NOT take the ownership of the input pointer.
        */
        ObservationDataSet* getObservationSet() const;
        
        /*!
          \brief It returns a reference to the internal trajectory data set type.

          \return A reference to the internal trajectory data set type.
        */
        const TrajectoryDataSetType& getType() const;

        /*!
          \brief It returns the identifier associated to the trajectory.

          \return The identifier associated to the trajectory.
        */
        std::string getId() const;
        
        /*!
          \brief It sets the identifier associated to the trajectory.

          \param id The identifier associated to the trajectory.
        */
        void setId(const std::string& id);   

        /*!
          \brief It returns the size of the trajectory observation set.

          \return The observation set size of the trajectory.
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

        /*! \name Methods to get values pointed by the internal cursor.
        //@{
        /*! 
          \brief It returns the geometry pointed by the internal cursor.

          \return A pointer to the geometry pointed by the internal cursor. 
          
          \note The caller will take the ownership of the returned pointer.    
        */  
        std::auto_ptr<te::gm::Geometry> getGeometry() const;

        /*! 
          \brief It returns the time pointed by the internal cursor.

          \return A pointer to the time pointed by the internal cursor. 
          
          \note The caller will take the ownership of the returned pointer.    
        */  
        std::auto_ptr<te::dt::DateTime> getTime() const;  
        //@}
           
        /*!
          \brief It returns the temporal extent of the trajectory observations.

          \return The temporal extent of the trajectory observations.

          \note The caller will NOT take the ownership of the output pointer.
        */
        te::dt::DateTimePeriod* getTemporalExtent() const;

        /*!
          \brief  It returns the spatial extent of the trajectory observations.

          \return The spatial extent of the trajectory observations.                   
        */
        const te::gm::Envelope&  getSpatialExtent() const;

        /*!
          \brief  It returns the trajectory from the DataSet.

          This method encapsulates all observations of this DataSet as a
          Trajectory type associated to a given interpolator.

          \return The trajectory associated to a given interpolator.

          \note The caller will take the ownership of the returned pointer. 
          \note It uses the method moveNext() internally. So, after calling this method,
                the internal cursor will point to the end of the DataSet. 
        */
        std::auto_ptr<Trajectory>  getTrajectory(AbstractTrajectoryInterp* interp);

        /*!
          \brief  It returns the trajectory from the DataSet.

          This method encapsulates all observations of this DataSet as a
          Trajectory type associated to a NearestGeometryAtTimeInterp interpolator.

          \return The trajectory associated to a NearestGeometryAtTimeInterp interpolator.

          \note The caller will take the ownership of the returned pointer. 
          \note It uses the method moveNext() internally. So, after calling this method,
                the internal cursor will point to the end of the DataSet. 
        */
        std::auto_ptr<Trajectory>  getTrajectory();
        
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
        virtual ~TrajectoryDataSet(); 

      private:
        std::auto_ptr<ObservationDataSet>   m_obsDs;    //!< The data set that contains the trajectory observations 
        TrajectoryDataSetType               m_type;     //!< The trajectory type.
        std::string                         m_id;       //!< The trajectory identification.
     };
   } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TRAJECTORYDATASET_H

