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
  \file BufferMemory.h
   
  \brief 
 
  \ingroup vp
*/

#ifndef __TERRALIB_VP_INTERNAL_BUFFER_MEMORY_H
#define __TERRALIB_VP_INTERNAL_BUFFER_MEMORY_H

//Terralib
#include "../common/Exception.h"
#include "../datatype/Property.h"
#include "../dataaccess/dataset/DataSetType.h"
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
      \class BufferMemory

      \brief A concrete class to compute the buffer operation.

      \ingroup vp
    */
    class TEVPEXPORT BufferMemory : public BufferOp
    {
    
    public:
      
      /*! \brief Default constructor.*/
      BufferMemory();
      
      /*! \brief Destructor. */
      ~BufferMemory();
      
      /*!
        \brief It executes the operation.

        \return A Boolean value that means if the operation successfully completed or not.
      */
      bool run() throw(te::common::Exception);

    protected:

      /*!
        \brief It executes the buffer operator.

        \param geom       The input geometry.
        \param distance   The distance to set buffer.
        \param level      How many levels the buffer will have.
        \param auxGeom    The auxiliar geometry that represents the previous buffer result.
                          This parameter is important when the buffer result has more than one level.

        \return A geometry with buffer result.
      */
      te::gm::Geometry* setBuffer(te::gm::Geometry* geom,
                                  const double& distance,
                                  const int& level,
                                  te::gm::Geometry*& auxGeom);

      /*!
        \brief It dissolves the bounders between buffers.

        \param outDSet  The dataSet with buffers.
        \param levels   The number of levels the dataset has.

        \note The outDSet is updated with dissolved buffers.
      */
      void dissolveMemory(te::mem::DataSet* outDSet,
                          const int& levels);

      /*!
        \brief It returns the distance of buffer by the attribute chosen.

        \param dataSet  The input dataSet with the value.
        \param position The position of attribute in dataSet.

        \return The distance.
      */
      double getDistanceByAttribute(te::da::DataSet& dataSet,
                                    const int& position);

    }; // end class
  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_BUFFER_MEMORY_H

