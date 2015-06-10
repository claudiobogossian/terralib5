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
  \file ObservationDataSetType.h

  \brief This file contains a class called ObservationDataSetType
*/

#ifndef __TERRALIB_ST_INTERNAL_OBSERVATIONDATASETTYPE_H
#define __TERRALIB_ST_INTERNAL_OBSERVATIONDATASETTYPE_H

//ST
#include "../../Config.h"

//STL
#include <string>
#include <vector>

//Boost
#include <boost/ptr_container/ptr_vector.hpp>


namespace te { namespace da { class DataSetType; } }
namespace te { namespace dt { class DateTimeProperty; class DateTime;  
                              class DateTimeInstant; class DateTimePeriod;} }
namespace te { namespace gm { class GeometryProperty; class Geometry; } }

namespace te
{
  namespace st
  {
     /*!
      \class ObservationDataSetType

      \brief  A class that contains complemental DataSetType information about a DataSet 
              that holds observations.

      This class contains complemental DataSetType information about a 
      DataSet that holds observations. 

      An observation is a result of a measurement associated to a discrete 
      time instant or period which assigns values to a phenomenon. It follows
      the OGC's Observation and Measurement (O&M) specification.
      An observation is composed of: (1) a phenomenon time, (2) a valid time, 
      (3) a result time and (4) observed values. 
      
      Thus, this class has the indexes of the DataSet properties that contains the
      phenomenon time, the valid time, the result time and the observed values. 

      \sa Observation ObservationDataSet
    */

    class TESTEXPORT ObservationDataSetType
    {
      public:

        /*! 
          \brief Constructor.

          \param dsName       The DataSet name. 
        */
        ObservationDataSetType(const std::string& dsName); 

        /*! \brief Copy constructor. */
        ObservationDataSetType(const ObservationDataSetType& obs); 

        /*! 
          \brief Assignment operator.
        */
        const ObservationDataSetType& operator=(const ObservationDataSetType& rhs);  

        /*! \name Operations to set information */
        //@{
        
        /*!
          \brief It sets the DataSet name that contains the observations

          \param dsName       The DataSet name. 
        */
        void setDataSetName(const std::string& dsName);

        /*!
          \brief It sets the property info that contains the phenomenon beginning time.

          \param tpInfo The property info that contains the phenomenon beginning time.

          \note It will take the ownership of the given pointers.
        */
        void setTimePropInfo(te::dt::DateTimeProperty* tpInfo);

        /*!
          \brief It sets the property info that contains the phenomenon time.

          \param tpInfo1 The property info that contains the phenomenon beginning time.
          \param tpInfo2 The property info that contains the phenomenon beginning time.

          \note It will take the ownership of the given pointers.
        */
        void setTimePropInfo(te::dt::DateTimeProperty* tpInfo1, te::dt::DateTimeProperty* tpInfo2);

        /*!
          \brief It sets the phenomenon time associated to all observations. 

          In this case, the phenomenon time is not inside the DataSet but it is
          informed by the user.

          \param t The phenomenon time.

          \note It will take the ownership of the given pointers.
        */
        void setTime(te::dt::DateTime* t);

        /*!
          \brief It sets the property info that contains the valid time.

          \param tpInfo The property info that contains the valid time.

          \note It will take the ownership of the given pointers.
        */
        void setVlTimePropInfo(te::dt::DateTimeProperty* tpInfo);

        /*!
          \brief It sets the property info that contains the valid time.

          \param tpInfo1 The property info that contains the phenomenon valid time.
          \param tpInfo2 The property info that contains the phenomenon valid time.

          \note It will take the ownership of the given pointers.
        */
        void setVlTimePropInfo(te::dt::DateTimeProperty* tpInfo1, te::dt::DateTimeProperty* tpInfo2);

        /*!
          \brief It sets the valid time associated to all observations. 

          In this case, the valid time is not inside the DataSet but it is
          informed by the user.

          \param t The valid time.

          \note It will take the ownership of the given pointers.
        */
        void setVlTime(te::dt::DateTimePeriod* t);

        /*!
          \brief It sets the property info that contains the result time.

          \param tpInfo The property info that contains the result time.

          \note It will take the ownership of the given pointers.
        */
        void setRsTimePropInfo(te::dt::DateTimeProperty* tpInfo);

        /*!
          \brief It sets the result time associated to all observations. 

          In this case, the result time is not inside the DataSet but it is
          informed by the user.

          \param t The result time.

          \note It will take the ownership of the given pointers.
        */
        void setRsTime(te::dt::DateTimeInstant* t);

        /*!
          \brief It sets the indexes of the DataSet properties that are observed over time. 

          \param props The indexes of the DataSet properties that are observed over time.
        */
        void setObsPropInfo(const std::vector<int>& props);

        /*!
          \brief It sets the names of the DataSet properties that are observed over time. 

          \param props The names of the DataSet properties that are observed over time.
        */
        void setObsPropInfo(const std::vector<std::string>& props);

        /*!
          \brief It sets the info about the property that contains the observation location.  

          In this case, the location of each observation is stored in the DataSet as a 
          geometry property. The geometry property can be observed over time or be the
          same for all observations.

          \param prop The info about the property that contains the observation location.

          \note It will take the ownership of the given pointer.
        */
        void setGeomPropInfo(te::gm::GeometryProperty* prop);

        /*!
          \brief It sets the geometry associated to all observations.   

          In this case, the location of all observations is NOT stored in the DataSet as a 
          geometry property. The observation location is informed
          by the user. 

          \param geom The geometry associated to all observations.

          \note It will take the ownership of the given pointer.
        */
        void setGeometry(te::gm::Geometry* geom);

        /*!
          \brief It sets the index of the DataSet property that contains identifiers that groups the observations. 

          \param idProp The index of the DataSet property that contains identifiers that groups the observations.
        */
        void setIdPropInfo(int idProp);

        /*!
          \brief It sets the name of the DataSet property that contains identifiers that groups the observations. 

          \param idProp The name of the DataSet property that contains identifiers that groups the observations.
        */
        void setIdPropInfo(const std::string& idProp);

        /*!
          \brief It sets the identifier associated to all observations. 

          In this case, the identifier is NOT in the DataSet. It is informed by the user. 

          \param id The identifier associated to all observations.
        */
        void setId(const std::string& id);

        /*!
          \brief It sets the spatial extent of all observations. 

          \param se The spatial extent of all observations.

          \note It will take the ownership of the given pointer.
        */
        void setSpatialExtent(te::gm::Geometry* se);

        /*!
          \brief It sets the temporal extent of all observations. 

          \param te The temporal extent of all observations.

          \note It will take the ownership of the given pointer.
        */
        void setTemporalExtent(te::dt::DateTimePeriod* te);

        //@}

        /*! \name Operations to check information */
        //@{
        bool hasTimeProp() const;
        bool hasTwoTimeProp() const;
        bool hasTime() const;
        bool hasVlTimeProp() const;
        bool hasTwoVlTimeProp() const;
        bool hasVlTime() const;
        bool hasRsTimeProp() const;
        bool hasRsTime() const;
        bool hasGeomProp() const;
        bool hasGeometry() const;
        bool hasIdProp() const;
        bool hasId() const;
        bool hasSpatialExtent() const;
        bool hasTemporalExtent() const;
        //@}

        /*! \name Operations to get information */
        //@{

        /*!
          \brief It returns the DataSet name

          \return The dataSet name
        */
        std::string getDataSetName() const;

        /*!
          \brief It returns the DataSet property information that contains the phenomenon beginning time.

          \return The DataSet property information that contains the phenomenon beginning time.

          \note The user does NOT take the ownership of the returned pointer.
          \note The returned pointer can be NULL, when there is no information.
        */
        const te::dt::DateTimeProperty* getBeginTimePropInfo() const;

        /*!
          \brief It returns the DataSet property information that contains the phenomenon end time.

          \return The DataSet property information that contains the phenomenon end time.

          \note The user does NOT take the ownership of the returned pointer.
          \note The returned pointer can be NULL, when there is no information.
        */
        const te::dt::DateTimeProperty* getEndTimePropInfo() const;

        /*!
          \brief It returns the phenomenon time.

          \return The phenomenon time.

          \note The user does NOT take the ownership of the returned pointer.
          \note The returned pointer can be NULL, when there is no information.
        */
        const te::dt::DateTime* getTime() const;

        /*!
          \brief It returns the DataSet property information that contains the valid beginning time.

          \return The DataSet property information that contains the valid beginning time.

          \note The user does NOT take the ownership of the returned pointer.
          \note The returned pointer can be NULL, when there is no information.
        */
        const te::dt::DateTimeProperty* getVlBeginTimePropInfo() const;

        /*!
          \brief It returns the DataSet property information that contains the valid end time.

          \return The DataSet property information that contains the valid end time.

          \note The user does NOT take the ownership of the returned pointer.
          \note The returned pointer can be NULL, when there is no information.
        */
        const te::dt::DateTimeProperty* getVlEndTimePropInfo() const;

        /*!
          \brief It returns the valid time.

          \return The valid time.

          \note The user does NOT take the ownership of the returned pointer.
          \note The returned pointer can be NULL, when there is no information.
        */
        const te::dt::DateTimePeriod* getVlTime() const;

        /*!
          \brief It returns the DataSet property information that contains the result time.

          \return The DataSet property information that contains the result time.

          \note The user does NOT take the ownership of the returned pointer.
          \note The returned pointer can be NULL, when there is no information.
        */
        const te::dt::DateTimeProperty* getRsTimePropInfo() const;
        
        /*!
          \brief It returns the result time.

          \return The result time.

          \note The user does NOT take the ownership of the returned pointer.
          \note The returned pointer can be NULL, when there is no information.
        */
        const te::dt::DateTimeInstant* getRsTime() const;

        /*!
          \brief It returns the indexes of the DataSet properties that are observed over time. 

          \return The indexes of the DataSet properties that are observed over time. 
        */
        const std::vector<int>& getObsPropIdxs() const;

        /*!
          \brief It returns the names of the DataSet properties that are observed over time. 

          \return The names of the DataSet properties that are observed over time. 
        */
        const std::vector<std::string>& getObsPropNames() const;

        /*!
          \brief It returns the DataSet property information that contains the geometry.

          \return The DataSet property information that contains the geometry.

          \note The user does NOT take the ownership of the returned pointer.
          \note The returned pointer can be NULL, when there is no information.
        */
        const te::gm::GeometryProperty* getGeomPropInfo() const;
        
        /*!
          \brief It returns the geometry associated to all observations.

          \return The geometry associated to all observations.

          \note The user does NOT take the ownership of the returned pointer.
          \note The returned pointer can be NULL, when there is no information.
        */
        const te::gm::Geometry* getGeometry() const;

        /*!
          \brief It returns the index of the property that contains identifiers that groups observations. 

          \return The index of the property that contains identifiers that groups observations. 
        */
        int getIdPropIdx() const;

        /*!
          \brief It returns the name of the property that contains identifiers that groups observations. 

          \return The name of the property that contains identifiers that groups observations. 
        */
        std::string getIdPropName() const;

        /*!
          \brief It returns the identifier associated to all observations. 

          \return The identifier associated to all observations. 
        */
        std::string getObsId() const;

        /*!
          \brief It returns the temporal extent of the observations.

          \return The temporal extent of the observations.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        const te::dt::DateTimePeriod* getTemporalExtent() const;

        /*!
          \brief  It returns the spatial extent of the observations.

          \return The spatial extent of the observations.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        const te::gm::Geometry* getSpatialExtent() const;

        /*!
          \brief It returns the name of the DataSet property that contains the phenomenon beginning time.

          \return The name of the DataSet property that contains the phenomenon beginning time.

          \note It returns "" when there is no a phenomenon beginning time.
        */
        std::string getBeginTimePropName() const;

        /*!
          \brief It returns the name of the DataSet property that contains the phenomenon end time.

          \return The name of the DataSet property that contains the phenomenon end time.

          \note It returns "" when there is no a phenomenon end time.
        */
        std::string getEndTimePropName() const;

        /*!
          \brief It returns the name of the property that is a geometry property.

          \return The name of the property that is a geometry property.

          \note It returns "" when there is no a phenomenon end time.
        */
        std::string getGeomPropName() const;
        //@}    
               
        /*! \brief Virtual destructor. */
        virtual ~ObservationDataSetType(); 
            
      private:
      
        std::string              m_dsName;     //!< The DataSet name
        
        boost::ptr_vector<te::dt::DateTimeProperty>   m_phTimePropInfo; //!< Phenomenon time: information about the DataSet properties that contain the phenomenon time.
        std::auto_ptr<te::dt::DateTime>               m_phTime;         //!< Phenomenon time: the phenomenon time, when the time is not inside a DataSet. 

        boost::ptr_vector<te::dt::DateTimeProperty>   m_validTimePropInfo;  //!< Valid time: information about the DataSet properties that contain the valid time. 
        std::auto_ptr<te::dt::DateTimePeriod>         m_validTime;          //!< Valid time: the valid time, when the time is not inside a DataSet. 

        std::auto_ptr<te::dt::DateTimeProperty>       m_resultTimePropInfo;   //!< Result time: information about the DataSet property that contains the result time.         
        std::auto_ptr<te::dt::DateTimeInstant>        m_resultTime;          //!< Result time: the result time, when the time is not inside a DataSet.

        std::vector<int>            m_observedPropIdx;     //!< Observed properties: the indexes of DataSet properties that are observed. 
        std::vector<std::string>    m_observedPropName;    //!< Observed properties: the names of DataSet properties that are observed. 
        
        std::auto_ptr<te::gm::GeometryProperty>   m_geomPropInfo; //!< Geometry: information about the property that has the observation spatial location
        std::auto_ptr<te::gm::Geometry>           m_geom;         //!< Geometry: the geometry, when the spatial location of the observations are not in the DataSet.

        int               m_idPropIndex;  //!< Id: the index of DataSet property that contain the id of each group of observation.  
        std::string       m_idPropName;   //!< Id: the name of DataSet property that contain the id of each group of observation.   
        std::string       m_id;           //!< Id: the id associated to all observations 

        std::auto_ptr<te::gm::Geometry>           m_spatialExtent;   //!< Spatial extent
        std::auto_ptr<te::dt::DateTimePeriod>     m_temporalExtent;  //!< Temporal extent
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_OBSERVATIONDATASETTYPE_H

