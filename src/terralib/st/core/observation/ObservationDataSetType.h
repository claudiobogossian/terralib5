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

namespace te { namespace da { class DataSetType; } }
namespace te { namespace dt { class Property; class DateTimeProperty; } }

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
          
          \param phTimeIdx    The index of the property that contains the phenomenon time.
          \param obsPropIdx   The index of the property that is observed.
          \param geomPropIdx  The index of the observed property that contains the geometry.
        */
        ObservationDataSetType(int phTimeIdx, int obsPropIdx, int geomPropIdx = -1); 

        /*! 
          \brief Constructor. 
          
          \param phTimeIdx      The index of the property that contains the phenomenon time.
          \param obsPropIdx     The indexes of the properties that are observed.
          \param geomPropIdx    The index of the observed property that contains the geometry.
        */
        ObservationDataSetType(int phTimeIdx, const std::vector<int>& obsPropIdx, int geomPropIdx = -1); 
        
        /*! 
          \brief Constructor. 

          This constructor is used when the DataSet uses two properties to store a period,
          one for its begining and the other for its end.

          \param phTimeIdx    The indexes of the properties that contain the phenomenon time.
          \param obsPropIdx   The indexes of the properties that are observed.
          \param geomPropIdx  The index of the observed property that contains the geometry.
        */
        ObservationDataSetType( const std::vector<int>& phTimeIdx, const std::vector<int>& obsPropIdx, 
                                int geomPropIdx = -1); 

        /*! 
          \brief Constructor. 
          
          \param phTimeIdx    The indexes of the properties that contains the phenomen time.
          \param obsPropIdx   The indexes of the property that are observed.
          \param geomPropIdx  The index of the observed property that contains the geometry.
          \param valTimeIdx   The indexes of the properties that contains the valid time.
          \param resTimeIdx   The index of the property that contains the result time.
        */
        ObservationDataSetType( const std::vector<int>& phTimeIdx, const std::vector<int>& obsPropIdx, 
                                int geomPropIdx, const std::vector<int>& valTimeIdx, int resTimeIdx); 
                                              
        /*!
          \brief It returns the indexes of the DataSetType properties that contains the phenomenon time.

          \return The indexes of the DataSetType properties that contains the phenomenon time.
        */
        const std::vector<int>& getTimePropIdxs() const;

        /*!
          \brief It returns the index of the DataSet property that contains the phenomenon beginning time.

          \return The index of the DataSet property that contains the phenomenon beginning time.
        */
        int getBeginTimePropIdx() const;
        
        /*!
          \brief It returns the index of the DataSet property that contains the phenomenon end time.

          \return The index of the DataSet property that contains the phenomenon end time.

          \note It returns -1 when there is no a phenomenon end time.
        */
        int getEndTimePropIdx() const;

        /*!
          \brief It sets the indexes of the DataSet properties that contains the phenomenon time.

          \param idxs The indexes of the DataSet properties that contains the phenomenon time.
        */
        void setTimePropIdxs(const std::vector<int>& idxs);

        /*!
          \brief It sets the index of the DataSet property that contains the phenomenon beginning time.

          \param idx The index of the DataSet property that contains the phenomenon beginning time.
        */
        void setBeginTimePropIdx(int idx);

        /*!
          \brief It sets the index of the DataSet property that contains the phenomenon end time.

          \param idx The index of the DataSet property that contains the phenomenon end time.
        */
        void setEndTimePropIdx(int idx);

        /*!
          \brief It returns the indexes of the DataSetType properties that are observed over time. 

          \return The indexes of the DataSetType properties that are observed over time. 
        */
        const std::vector<int>& getObsPropIdxs() const;
        
        /*!
          \brief It sets the indexes of the DataSetType properties that are observed over time. 

          \param props The indexes of the DataSetType properties that are observed over time. 
        */
        void setObsPropIdxs(const std::vector<int>& props);
                
        /*!
          \brief It returns the index of the observed property that is a geometry property.

          \return The index of the observed property that is a geometry property.
        */
        int getGeomPropIdx() const;

        /*!
          \brief It sets the index of the observed property that is a geometry property.

          \param  idx The index of the observed property that is a geometry property.
        */
        void setGeomPropIdx(int idx);

        /*!
          \brief It returns the index of the DataSet property that contains the valid beginning time.

          \return The index of the DataSet property that contains the valid beginning time.

          \note It returns -1 when there is no a valid beginning time.
        */
        int getVlBeginTimePropIdx() const;
        
        /*!
          \brief It returns the index of the DataSet property that contains the valid end time.

          \return The index of the DataSet property that contains the valid end time.

          \note It returns -1 when there is no a valid end time.
        */
        int getVlEndTimePropIdx() const;    
        
        /*!
          \brief It returns the indexes of the DataSet properties that contains the valid time.

          \return The indexes of the DataSet properties that contains the valid time.
        */
        const std::vector<int>& getVlTimePropIdxs() const;

        /*!
          \brief It sets the index of the DataSet property that contains the valid beginning time.

          \param idx The index of the DataSet property that contains the valid beginning time.
        */
        void setVlBeginTimePropIdx(int idx);

        /*!
          \brief It sets the index of the DataSet property that contains the valid end time.

          \param idx The index of the DataSet property that contains the valid end time.
        */
        void setVlEndTimePropIdx(int idx);
        
        /*!
          \brief It sets the indexes of the DataSet properties that contains the valid time.

          \param idxs The indexes of the DataSet properties that contains the valid time.
        */
        void setVlTimePropIdxs(const std::vector<int>& idxs);

        /*!
          \brief It returns the index of the DataSet property that contains the result time.

          \return The index of the DataSet properties that contains the result time.
        */
        int getRsTimePropIdx() const;
        
        /*!
          \brief It sets the index of the DataSet property that contains the result time.

          \param idx The index of the DataSet property that contains the result time.
        */
        void setRsTimePropIdx(int idx);    
               
        /*! \brief Virtual destructor. */
        virtual ~ObservationDataSetType(); 
            
      private:
      
        std::vector<int>         m_phTimePropIdx;       //!< The indexes of DataSet properties that contain the phenomenon time.
        std::vector<int>         m_observedPropIdx;     //!< The indexes of DataSet properties that are observed. 
        int                      m_geomPropIdx;         //!< The index of the observed property that contains the spatial information.
        std::vector<int>         m_validTimePropIdx;    //!< The indexes of DataSet properties that contain the valid time. 
        int                      m_resultTimePropIdx;   //!< The index of DataSet property that contains the result time.         
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_OBSERVATIONDATASETTYPE_H

