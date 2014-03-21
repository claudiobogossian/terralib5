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
  \file ObservationDataSet.h

  \brief This file contains a class to represent an observation data set. 
*/

#ifndef __TERRALIB_ST_INTERNAL_OBSERVATIONDATASET_H
#define __TERRALIB_ST_INTERNAL_OBSERVATIONDATASET_H

//ST
#include "../../Config.h"
#include "ObservationDataSetType.h"

//TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../datatype/DateTimePeriod.h"

// Boost
#include <boost/noncopyable.hpp>

//STL
#include <memory>

// Forward declarations
namespace te { namespace da { class DataSet; class DataSetType; } }
namespace te { namespace dt { class Property; class DateTime; class DateTimePeriod; } }
namespace te { namespace gm { class Envelope; class Geometry; } }

namespace te
{
  namespace st
  {    

    // Forward declarations
    class ObservationDataSetType;
    class Observation;
    class TrajectoryDataSet;
    class TimeSeriesDataSet;

    /*!
      \class ObservationDataSet

      \brief A class to represent a DataSet that contains observations. 

      This class implements a view on a data set. It encapsulates a DataSet as a
      ObservationDataSet. An observation data set is composed of a finite number 
      of homogeneous observations, that is, 
      every observation has the same number and type of observed values. 

      \ingroup st

      \sa DataSet Observation DataSource
    */

    /*
     \\\Nota: Poderíamos ter internamente um ponteiro para o STDataLoader 
      \\\ que o gerou ou para o Info (para obter o STDataLoader)... isso seria útil para: 
      \\\ (1) calcular o temporalExtent.
      \\\ (2) carregar o dado por partes e não tudo de uma vez
      \\\ mas por enquanto vamos deixar sem esse ponteiro. 
      \\\ No caso do temporalExtent, a pessoa pode informar no construtor ou não
    */
    class TESTEXPORT ObservationDataSet : public boost::noncopyable 
    {
      friend class ObservationDataSetLayer;
      friend class TrajectoryDataSet; 
      friend class TimeSeriesDataSet;

      public:

        /*! \name Constructors */
        //@{
        /*! \brief Constructor. 

            It constructs an observation data set, based on the given
            DataSet and ObservationDataSetType

            \param ds     the DataSet 
            \param obst   extra information about the properies of the DataSet

            \note It will take the ownership of the DataSet pointer
        */
        ObservationDataSet(te::da::DataSet* ds, const ObservationDataSetType& obst); 
        
        /*! \brief Constructor. 

            It constructs an observation data set, based on the given
            DataSet and ObservationDataSetType

            \param ds     The DataSet. 
            \param obst   extra information about the properies of the DataSet
            \param text   the temporal extent

            \note It will take the ownership of the given pointers
        */
        ObservationDataSet(te::da::DataSet* ds, const ObservationDataSetType& obst, 
                           te::dt::DateTimePeriod* text); 
                           
         /*! \brief Constructor. 

            It constructs an observation data set, based on the given
            DataSet and ObservationDataSetType

            \param ds     The DataSet. 
            \param obst   extra information about the properies of the DataSet
            \param text   the temporal extent
            \param sext   the spatial extent

            \note It will take the ownership of the given pointers
        */
        ObservationDataSet(te::da::DataSet* ds, const ObservationDataSetType& obst, 
                           te::dt::DateTimePeriod* text, const te::gm::Envelope& sext); 

        /*! \brief Constructor. 

            \param ds           The DataSet. 
            \param phTimeIdx    The index of the property that contains the phenomenon time.
            \param obsPropIdx   The index of the property that is observed.
            \param geomPropIdx  The index of the observed property that contains the geometry.

            \note It will take the ownership of the DataSet pointer
        */
        ObservationDataSet( te::da::DataSet* ds, int phTimeIdx, int obsPropIdx, 
                            int geomPropIdx = -1);

        /*! \brief Constructor. 

            \param ds     the DataSet 
            \param phTimeIdxs   The indexes of the property that contains the begin and final phenomenon time.
            \param obsPropIdx   The indexes of the properties that are observed.
            \param geomPropIdx  The index of the observed property that contains the geometry.

            \note It will take the ownership of the DataSet pointer
        */
        ObservationDataSet( te::da::DataSet* ds, const std::vector<int>& phTimeIdxs, 
                            const std::vector<int>& obsPropIdxs, int geomPropIdx = -1);
        
        /*! \brief Constructor. 

            \param ds           The DataSet 
            \param phTimeIdx    The index of the property that contains the phenomenon time.
            \param obsPropIdx   The index of the property that is observed.
            \param geomPropIdx  The index of the observed property that contains the geometry.
            \param text         The temporal extent
            \param sext         The spatial extent

            \note It will take the ownership of the DataSet pointer
        */
        ObservationDataSet( te::da::DataSet* ds, int phTimeIdx, int obsPropIdx, 
                            int geomPropIdx, te::dt::DateTimePeriod* text);                    
        
        
        /*! \brief Constructor. 

            \param ds           The DataSet 
            \param phTimeIdx    The index of the property that contains the phenomenon time.
            \param obsPropIdx   The index of the property that is observed.
            \param geomPropIdx  The index of the observed property that contains the geometry.
            \param text         The temporal extent
            \param sext         The spatial extent

            \note It will take the ownership of the DataSet pointer
        */
        ObservationDataSet( te::da::DataSet* ds, int phTimeIdx, int obsPropIdx, 
                            int geomPropIdx, te::dt::DateTimePeriod* text, 
                            const te::gm::Envelope& sext);

        /*! \brief Constructor. 

            \param ds           The DataSet 
            \param phTimeIdxs   The index of the property that contains the begin and end times.
            \param obsPropIdxs  The indexes of the properties that are observed.
            \param geomPropIdx  The index of the observed property that contains the geometry.
            \param text         The temporal extent
            \param sext         The spatial extent

            \note It will take the ownership of the DataSet pointer
        */
        ObservationDataSet( te::da::DataSet* ds, const std::vector<int>& phTimeIdxs, 
                            const std::vector<int>& obsPropIdxs, int geomPropIdx, 
                            te::dt::DateTimePeriod* text);

        /*! \brief Constructor. 

            \param ds           The DataSet 
            \param phTimeIdxs   The index of the property that contains the begin and end times.
            \param obsPropIdxs  The indexes of the properties that are observed.
            \param geomPropIdx  The index of the observed property that contains the geometry.
            \param text         The temporal extent
            \param sext         The spatial extent

            \note It will take the ownership of the DataSet pointer
        */
        ObservationDataSet( te::da::DataSet* ds, const std::vector<int>& phTimeIdxs, 
                            const std::vector<int>& obsPropIdxs, int geomPropIdx, 
                            te::dt::DateTimePeriod* text, const te::gm::Envelope& sext);
        //@}

        /*!
          \brief It returns the DataSet that contains the observations. 

          \return A pointer to the internal DataSet that contains the observations. 

          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::da::DataSet* getData() const;

        /*!
          \brief It returns a reference to the ObservationDataSetType.

          \return A reference to the internal ObservationDataSetType. 
        */
        const ObservationDataSetType& getType() const;

        /*!
          \brief  It returns the spatial extent of the observations, 
                  when there is an observed value of geometry type.

          \return The spatial extent of the observations.
        */
        const te::gm::Envelope& getSpatialExtent();

        /*!
          \brief  It sets the spatial extent of the observations, 
                  when there is an observed value of geometry type.

          \param ext The spatial extent of the observations.
        */
        void setSpatialExtent(const te::gm::Envelope& ext);
        
         /*!
          \brief It returns the temporal extent of the observations.

          \return The temporal extent of the observations.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::dt::DateTimePeriod* getTemporalExtent();

         /*!
          \brief  It sets the temporal extent of the observations, 
                  when there is an observed value of geometry type.

          \param ext The temporal extent of the observations.

          \note It will take the ownership of the given pointer.
        */
        void setTemporalExtent(te::dt::DateTimePeriod* ext);

        /*! \name Methods to traverse the observation set and to check the 
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
           
        /*!
          \brief It returns the observation pointed by the internal cursor. 

          \return The observation pointed by the internal cursor. 

          \note The caller will take the ownership of the output pointer.
        */
        std::auto_ptr<Observation> getObservation() const;

        /*! \name Methods to return the properties associated to observations. */
        //@{
        /*!
          \brief It returns the property of the phenomenon beginning time.

          \return The property of the phenomenon beginning time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        //te::dt::DateTimeProperty* getBeginTimeProperty() const;

         /*!
          \brief It returns the property of the phenomenon end time.

          \return The property of the phenomenon end time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        //te::dt::DateTimeProperty* getEndTimeProperty() const;

        /*!
          \brief It returns the properties of the observed values. 

          \param output the properties of the observed values.

          \note The caller will NOT take the ownership of the output pointers.
        */
        //void getObsProperties(std::vector<te::dt::Property*> output);

        /*!
          \brief It returns the property of the valid beginning time.

          \return The property of the valid beginning time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        //te::dt::DateTimeProperty* getVlBeginTimeProperty() const;

        /*!
          \brief It returns the property of the valid end time.

          \return The property of the valid end time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        //te::dt::DateTimeProperty* getVlEndTimeProperty() const;
        
        /*!
          \brief It returns the property of the result time.

          \return The property of the result time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        //te::dt::DateTimeProperty* getRsTimeProperty() const;
        //@}
        
        /*! \brief Virtual destructor. */
        virtual ~ObservationDataSet(); 

      protected:

        /*!
          \brief It releases all internal pointers, returning its internal DataSet and invalidating itself
          \return A pointer to the internal DataSet that contains the observations. 

           \note This method is used when the user is interested only in its internal DataSet 
           \note The caller will take the ownership of the returned pointer.
        */
        std::auto_ptr<te::da::DataSet> release();

      private:

        std::auto_ptr<te::da::DataSet>          m_ds;             //! A data set that contains observations
        ObservationDataSetType                  m_obst;           //! An observation data set type that refers to m_ds
        std::auto_ptr<te::dt::DateTimePeriod>   m_tpExtent;       //! Its temporal extent
        te::gm::Envelope                        m_spExtent;       //! Its spatial extent
    };

    typedef std::auto_ptr<ObservationDataSet> ObservationDataSetPtr;

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_OBSERVATIONDATASET_H



