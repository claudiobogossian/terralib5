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
  \file MovingObjectDataSetMetadata.h

  \brief This file contains a class called MovingObjectDataSetMetadata
*/

#ifndef __TERRALIB_STLOADER_INTERNAL_MOVINGOBJECTDATASETMETADATA_H
#define __TERRALIB_STLOADER_INTERNAL_MOVINGOBJECTDATASETMETADATA_H

//STLoader
#include "ObservationDataSetMetadata.h"
#include "Config.h"

namespace te
{
  namespace stloader
  {
     /*!
      \class MovingObjectDataSetMetadata

      \brief A class that represents the metadata about a DataSet that contains moving objects.

      This class represents information about a DataSet that contains moving objects.
     
      \sa DataSet MovingObject Observation ObservationSet
    */
    class TESTLOADEREXPORT MovingObjectDataSetMetadata : public ObservationDataSetMetadata
    {
      public:
                   
        /*! 
          \brief Empty constructor.
        */
        MovingObjectDataSetMetadata(); 

        /*! 
          \brief Constructor. 
          
          \param phTimePropIdx  The index of the DataSet property that contains the moving object phenomenon time. 
          \param geomPropIdx    The index of DataSet property that contains the moving object geometry. 
          \param idPropIdx      The index of DataSet property that contains the moving object identifier.        
        */
        MovingObjectDataSetMetadata(int phTimePropIdx, int geomPropIdx, int idPropIdx = -1); 

        /*!
          \brief It returns the index of the DataSet property that contains the moving object geometry. 

          \return The index of the DataSet property that contains the moving object geometry. 
        */
        int getGeometryPropertyIdx() const;

        /*!
          \brief It sets the index of the DataSet property that contains the moving object geometry. 

          \param idx The index of the DataSet property that contains the moving object geometry. 
        */
        void setGeometryPropertyIdx(int idx);

        /*!
          \brief It returns the index of the DataSet property that contains the moving object identifier. 

          \return The index of the DataSet property that contains the moving object identifier. 
        */
        int getIdPropertyIdx() const;

        /*!
          \brief It sets the index of the DataSet property that contains the moving object identifier. 

          \param idx The index of the DataSet property that contains the moving object identifier. 
        */
        void setIdPropertyIdx(int idx);        

      protected:

        int                 m_geomPropIdx;    //!< The index of DataSet property that contains the moving object geometry. 
        int                 m_idPropIdx;      //!< The index of DataSet property that contains the moving object identifier. 
    };
  } // end namespace stloader
}   // end namespace te

#endif  // __TERRALIB_STLOADER_INTERNAL_MOVINGOBJECTDATASETMETADATA_H



