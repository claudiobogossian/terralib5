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
  \file BufferQuery.h
   
  \brief 
 
  \ingroup vp
*/

#ifndef __TERRALIB_VP_INTERNAL_BUFFER_QUERY_H
#define __TERRALIB_VP_INTERNAL_BUFFER_QUERY_H

//Terralib
#include "../common/Exception.h"
#include "../datatype/Property.h"
#include "../geometry/Geometry.h"
#include "../memory/DataSet.h"
#include "../statistics/core/Enums.h"

#include "BufferOp.h"
#include "Config.h"
#include "Enums.h"
#include "Exception.h"


// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace vp
  {
    /*!
      \class BufferQuery

      \brief A concrete class to compute the buffer operation.

      \ingroup vp
    */
    class TEVPEXPORT BufferQuery : public BufferOp
    {
    
    public:
      
      /*! \brief Default constructor.*/
      BufferQuery();
      
      /*! \brief Destructor. */
      ~BufferQuery();
      
      /*!
        \brief It executes the operation.

        \return A Boolean value that means if the operation successfully completed or not.
      */
      bool run() throw(te::common::Exception);

    protected:

      /*!
        \brief It dissolves the bounders between buffers.

        \param outDSet  The dataSet with buffers.
        \param levels   The number of levels the dataset has.

        \note The outDSet is updated with dissolved buffers.
      */
      std::vector<std::vector<te::gm::Geometry*> > dissolveQuery( te::da::DataSet* dsQuery,
                                                                  const int& levels);

      /*!
        \brief It prepare the dataset to be persisted.

        \param dataSetQuery   The dataSet that was returned by query.
        \param outputDataSet  The dataSet prepared to be persisted.
      */
      void prepareDataSet(te::da::DataSet* dataSetQuery,
                          te::mem::DataSet* outputDataSet);

      /*!
        \brief It prepare the dataset to be persisted with dissolved geometries.

        \param vecDissolvedGeom   A vecter with dissolved geometries by level.
        \param outputDataSet      The dataSet prepared to be persisted.
        \param distance           The buffer distance.

        \note The outDSet is updated with dissolved buffers.
      */
      void prepareDataSet(std::vector<std::vector<te::gm::Geometry*> > vecDissolvedGeom,
                          te::mem::DataSet* outputDataSet,
                          const double &distance);
      
    }; // end class
  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_BUFFER_QUERY_H

