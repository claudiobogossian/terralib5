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
    class Observation;
    class ObservationDataSetType;
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
     \\\Nota: Poder�amos ter internamente um ponteiro para o STDataLoader 
      \\\ que o gerou ou para o Info (para obter o STDataLoader)... isso seria �til para: 
      \\\ (1) calcular o temporalExtent.
      \\\ (2) carregar o dado por partes e n�o tudo de uma vez
      \\\ mas por enquanto vamos deixar sem esse ponteiro. 
      \\\ No caso do temporalExtent, a pessoa pode informar no construtor ou n�o
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
                           te::dt::DateTimePeriod* text, te::gm::Geometry* sext); 
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

          \note The caller will NOT take the ownership of the returned pointer.
        */
        //const te::gm::Envelope& getSpatialExtent();
        const te::gm::Geometry* getSpatialExtent();

        /*!
          \brief  It sets the spatial extent of the observations, 
                  when there is an observed value of geometry type.

          \param ext The spatial extent of the observations.

          \note It will take the ownership of the given pointer.
        */
        //void setSpatialExtent(const te::gm::Envelope& ext);
        void setSpatialExtent(te::gm::Geometry* ext);
        
         /*!
          \brief It returns the temporal extent of the observations.

          \return The temporal extent of the observations.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        const te::dt::DateTimePeriod* getTemporalExtent();

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
        const te::dt::DateTimeProperty* getBeginTimeProperty() const;

         /*!
          \brief It returns the property of the phenomenon end time.

          \return The property of the phenomenon end time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        const te::dt::DateTimeProperty* getEndTimeProperty() const;

        /*!
          \brief It returns the property of the valid beginning time.

          \return The property of the valid beginning time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        const te::dt::DateTimeProperty* getVlBeginTimeProperty() const;

        /*!
          \brief It returns the property of the valid end time.

          \return The property of the valid end time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        const te::dt::DateTimeProperty* getVlEndTimeProperty() const;
        
        /*!
          \brief It returns the property of the result time.

          \return The property of the result time.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        const te::dt::DateTimeProperty* getRsTimeProperty() const;
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
   };

    typedef std::auto_ptr<ObservationDataSet> ObservationDataSetPtr;

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_OBSERVATIONDATASET_H



