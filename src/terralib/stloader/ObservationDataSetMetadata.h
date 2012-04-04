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
  \file ObservationDataSetMetadata.h

  \brief This file contains a class called ObservationDataSetMetadata
*/

#ifndef __TERRALIB_STLOADER_INTERNAL_OBSERVATIONDATASETMETADATA_H
#define __TERRALIB_STLOADER_INTERNAL_OBSERVATIONDATASETMETADATA_H

//ST
#include "Config.h"

//STL
#include <string>
#include <vector>

namespace te
{
  namespace stloader
  {
     /*!
      \class ObservationDataSetMetadata

      \brief A class that represents the metadata about a DataSet that contains observations.

      This class represents information about a DataSet that contains observations.
      An observation is an act associated with a discrete position in time, 
      in space or in space and time through which a number, term or other value 
      is assigned to a phenomenon. So, given a DataSet that contains observations, 
      we need to know some extra information about it, such as what columns contain 
      the phenomenon time and the observed properties.      

      \sa DataSet Observation ObservationSet
    */
    class TESTLOADEREXPORT ObservationDataSetMetadata
    {
      public:

        /*!
          \enum ObservationSetType

          \brief The observation set type.
        */
        enum ObservationSetType
        {
          OBSERVATIONSET,       /*!< Observation set type.      */
          MOVINGOBJECT,         /*!< Moving Object type.        */
          TIMESERIES,           /*!< Time Series type.          */
          LOCATEDTIMESERIES,    /*!< Located Time Series type. */
          COVERAGE              /*!< Coverage type. */
        };
        
        
        /*! 
          \brief Constructor.
          
          \param type The type associated to the DataSet that contains observations. 
        */
        ObservationDataSetMetadata(ObservationSetType type = OBSERVATIONSET); 

        /*! 
          \brief Constructor. 
          
          \param type       The type associated to the DataSet that contains observations.
          \param obsIdxs    The indexes of DataSet properties that contains the observed values.
          \param phTimeIdxs The indexes of DataSet properties that contains the phenomenon time.       
        */
        ObservationDataSetMetadata(ObservationSetType type, const std::vector<int>& obsIdxs,
                               const std::vector<int>& phTimeIdxs); 

         /*! 
          \brief Constructor. 
          
          \param type       The type associated to the DataSet that contains observations.
          \param obsIdxs    The indexes of DataSet properties that contains the observed values.
          \param phTimeIdx  The index of DataSet property that contains the phenomenon time.       
        */
        ObservationDataSetMetadata(ObservationSetType type, const std::vector<int>& obsIdxs,
                               int phTimeIdx); 

        /*! 
          \brief Constructor. 
          
          \param type       The type associated to the DataSet that contains observations.
          \param obsIdxs    The indexes of DataSet properties that contains the observed values.
          \param phTimeIdxs The indexes of DataSet properties that contains the phenomen time.
          \param valTimeIdx The indexes of DataSet properties that contains the valid time.
          \param resTimeIdx The index of DataSet property that contains the result time.
        */
        ObservationDataSetMetadata(ObservationSetType type, const std::vector<int>& obsIdxs,
                               const std::vector<int>& phTimeIdxs, const std::vector<int>& valTimeIdx, 
                               int resTimeIdx);         
        /*!
          \brief It returns the type of the DataSet that contains observations.

          \return The type of the DataSet that contains observations.
        */
        ObservationSetType getType() const;

        /*!
          \brief It sets the type of the DataSet that contains observations.

          \param type The type of the DataSet that contains observations.
        */
        void setType(ObservationSetType type);       

        /*!
          \brief It returns the indexes of the DataSet properties that contains the observed values.

          \return The indexes of the DataSet properties that contains the observed values.
        */
        std::vector<int>& getObservedPropertiesIdx();

        /*!
          \brief It sets the indexes of the DataSet properties that contains the observed values.

          \param idxs The indexes of the DataSet properties that contains the observed values.
        */
        void setObservedPropertiesIdx(const std::vector<int>& idxs);

        /*!
          \brief It returns the indexes of the DataSet properties that contains the phenomenon time.

          \return The indexes of the DataSet properties that contains the phenomenon time.
        */
        std::vector<int>& getPhenomenonTimeIdx();

        /*!
          \brief It sets the indexes of the DataSet properties that contains the phenomenon time.

          \param idxs The indexes of the DataSet properties that contains the phenomenon time.
        */
        void setPhenomenonTimeIdx(const std::vector<int>& idxs);

        /*!
          \brief It sets the index of the DataSet property that contains the phenomenon time.

          \param idx The index of the DataSet property that contains the phenomenon time.
        */
        void setPhenomenonTimeIdx(int idx);

        /*!
          \brief It returns the indexes of the DataSet properties that contains the valid time.

          \return The indexes of the DataSet properties that contains the valid time.
        */
        std::vector<int>& getValidTimeIdx();

        /*!
          \brief It sets the indexes of the DataSet properties that contains the valid time.

          \param idxs The indexes of the DataSet properties that contains the valid time.
        */
        void setValidTimeIdx(const std::vector<int>& idxs);

        /*!
          \brief It returns the index of the DataSet property that contains the result time.

          \return The index of the DataSet properties that contains the result time.
        */
        int getResultTimeIdx() const;

        /*!
          \brief It sets the index of the DataSet property that contains the result time.

          \param idx The index of the DataSet property that contains the result time.
        */
        void setValidTimeIdx(int idx);           
        
        /*! \brief Virtual destructor. */
        virtual ~ObservationDataSetMetadata() { }; 

      protected:

        ObservationSetType  m_type;                 //!< The type of the observation set
        std::vector<int>    m_observedPropIdxs;     //!< The indexes of DataSet properties that contains the observed values.
        
        std::vector<int>    m_phTimePropIdxs;       //!< The indexes of DataSet properties that contains the phenomen time.
        std::vector<int>    m_validTimePropIdxs;    //!< The indexes of DataSet properties that contains the valid time. 
        int                 m_resultTimePropIdx;    //!< The index of DataSet property that contains the result time. 
    };
  } // end namespace stloader
}   // end namespace te

#endif  // __TERRALIB_STLOADER_INTERNAL_OBSERVATIONDATASETTYPE_H

