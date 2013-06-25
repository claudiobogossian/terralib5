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
    struct IntersectionMember
    {
      te::da::DataSetType* dt;
      te::da::DataSet* ds;
      std::vector<te::dt::Property*> props;
    };
      
    
    typedef std::vector<size_t> LayerPropertiesPosList;

    typedef std::pair<te::map::AbstractLayerPtr, LayerPropertiesPosList> LayerInputData;

    typedef te::sam::rtree::Index<size_t, 8>* DataSetRTree;

    typedef std::pair<std::string, DataSetRTree> LayerRTree;

    te::da::DataSetType* CreateDataSetType(std::string newName, 
                                           te::da::DataSetType* firstDt,
                                           std::vector<te::dt::Property*> firstProps, 
                                           te::da::DataSetType* secondDt,
                                           std::vector<te::dt::Property*> secondProps);

    DataSetRTree CreateRTree(te::da::DataSetType* dt, te::da::DataSet* ds);

    te::map::AbstractLayerPtr Intersection(const std::string& newLayerName,
                                           const std::vector<LayerInputData>& idata,
                                           const te::da::DataSourceInfoPtr& dsinfo,
                                           size_t outputSRID = 0,
                                           const std::map<std::string, std::string>& options = std::map<std::string, std::string>());
      
    std::pair<te::da::DataSetType*, te::da::DataSet*> PairwiseIntersection(std::string newName, 
                                                                           IntersectionMember firstMember, 
                                                                           IntersectionMember secondMember,
                                                                           std::size_t outputSRID = 0);

  }
}

#endif // __TERRALIB_VP_CORE_INTERNAL_INTERSECTION_H