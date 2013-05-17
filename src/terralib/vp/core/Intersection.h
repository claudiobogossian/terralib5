/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/core/IntersectionDialog.h

  \brief Group of functions to intersection operation
*/

#ifndef __TERRALIB_VP_CORE_INTERNAL_INTERSECTION_H
#define __TERRALIB_VP_CORE_INTERNAL_INTERSECTION_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "../../sam/rtree.h"
#include "Config.h"

// STL
#include <memory>

namespace te
{
  namespace vp
  {
      /*!
        \enum MemoryUse

        \brief Define possible states for memory use.
      */
      enum MemoryUse
      {
        WHOLE_MEM = 0,    //!< Whole memory use.
        PARTIALLY_MEM = 1,//!< Partially memory use.
        LOW_MEM = 2       //!< Low memory use.
      };

      /*!
        \brief Creates a new DataSetType based on two DataSetType and theirs groups of properties

        \param newName The name of the new DataSetType.
        \param firstDt The first DataSetType.
        \param firstProps The first DataSetType vector of property.
        \param secondDt The second DataSetType.
        \param secondProps The second DataSetType vector of property.

        \return The new DataSetType.
      */
      te::da::DataSetType* CreateDataSetType(std::string newName, te::da::DataSetType* firstDt,
                                             std::vector<te::dt::Property*>& firstProps, te::da::DataSetType* secondDt,
                                             std::vector<te::dt::Property*>& secondProps);

      /*!
        \brief Create a RTree box of geometries and position of the geometry in the DataSet

        \param dt The DataSetType
        \param ds The DataSet

        \return The RTree
      */
      te::sam::rtree::Index<size_t, 8>* CreateRTree(te::da::DataSetType* dt, te::da::DataSet* ds);

      /*!
        \brief Creates a new DataSetType based on two DataSetType and theirs groups of properties

        \param newName The name of the new DataSetType.
        \param layers Map of layers and properties.

        \return The DataSetType and DataSet
      */
      std::pair<te::da::DataSetType*, te::da::DataSet*> Intersect(std::string newName,
                                                                  std::map<te::map::AbstractLayerPtr,
                                                                  std::vector<te::dt::Property*> > layers);
      
      /*!
        \brief Make the intersection of two DataSets

        \param newName The new name of DataSetType
        \param firstDt The first DataSetType
        \param firstDs The first DataSet
        \param firstProp The first DataSetType vector of properties
        \param secondDt The second DataSetType
        \param secondDs The second DataSet
        \param secondProp The second DataSetType vector of properties

        \return The DataSetType and DataSet
      */
      std::pair<te::da::DataSetType*, te::da::DataSet*> PairwiseIntersection(std::string newName, 
                                                                             te::da::DataSetType* firstDt, 
                                                                             te::da::DataSet* firstDs,
                                                                             std::vector<te::dt::Property*> firstProps, 
                                                                             te::da::DataSetType* secondDt, 
                                                                             te::da::DataSet* secondDs, 
                                                                             std::vector<te::dt::Property*> secondProps);

  }
}

#endif // __TERRALIB_VP_CORE_INTERNAL_INTERSECTION_H