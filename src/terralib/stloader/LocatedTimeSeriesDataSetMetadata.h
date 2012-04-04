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
  \file LocatedTimeSeriesDataSetMetadata.h

  \brief This file contains a class called LocatedTimeSeriesDataSetMetadata
*/

#ifndef __TERRALIB_STLOADER_INTERNAL_LOCATEDTIMESERIESDATASETMETADATA_H
#define __TERRALIB_STLOADER_INTERNAL_LOCATEDTIMESERIESDATASETMETADATA_H

// STLoader
#include "ObservationDataSetMetadata.h"
#include "Config.h"

namespace te
{
  namespace stloader
  {
     /*!
      \class LocatedTimeSeriesDataSetMetadata

      \brief A class that represents the metadata about a DataSet that contains located time series.

      This class represents information about a DataSet that contains located time series.
     
      \sa DataSet LocatedTimeSeries Observation ObservationSet
    */
    class TESTLOADEREXPORT LocatedTimeSeriesDataSetMetadata : public ObservationDataSetMetadata
    {
      public:
                   
        /*! 
          \brief Empty constructor.
        */
        LocatedTimeSeriesDataSetMetadata(); 

        /*! 
          \brief Constructor. 
          
          \param sLocPropIdx The index of DataSet property that contains the located spatial location. 
          \param idPropIdx   The index of DataSet property that contains the identifier of each located time series.        
        */
        LocatedTimeSeriesDataSetMetadata(int sLocPropIdx, int idPropIdx); 

        /*!
          \brief It returns the index of the DataSet property that contains the spatial location. 

          \return The index of the DataSet property that contains the spatial location. 
        */
        int getSpatialLocationPropertyIdx() const;

        /*!
          \brief It sets the index of the DataSet property that contains the spatial location. 

          \param idx The index of the DataSet property that contains the spatial location. 
        */
        void setSpatialLocationPropertyIdx(int idx);

        /*!
          \brief It returns the index of the DataSet property that contains the identifier. 

          \return The index of the DataSet property that contains the identifier. 
        */
        int getIdPropertyIdx() const;

        /*!
          \brief It sets the index of the DataSet property that contains the identifier. 

          \param idx The index of the DataSet property that contains the identifier. 
        */
        void setIdPropertyIdx(int idx);        

      protected:

        int                 m_sLocPropIdx;    //!< The index of DataSet property that contains the spatial location. 
        int                 m_idPropIdx;      //!< The index of DataSet property that contains the identifier. 
    };
  } // end namespace stloader
}   // end namespace te

#endif  // __TERRALIB_STLOADER_INTERNAL_LOCATEDTIMESERIESDATASETMETADATA_H
