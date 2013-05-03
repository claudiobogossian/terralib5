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
  \file terralib/vp/core/Intersection.h
   
  \brief 
*/

#ifndef __TERRALIB_VP_CORE_INTERNAL_INTERSECTION_H
#define __TERRALIB_VP_CORE_INTERNAL_INTERSECTION_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "Config.h"

// STL
#include <memory>

namespace te
{
  namespace vp
  {
    class TEVPEXPORT Intersection
    {
      public:

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

        Intersection();

        ~Intersection();

        void setInputDatasource(te::da::DataSourceInfoPtr& inputDatasource);

        void setOutputDatasource(te::da::DataSourceInfoPtr& outputDatasource);

        void setInputLayers(const std::map<te::map::AbstractLayerPtr, std::vector<te::dt::Property*>>& layers);

      private:

        void wholeMemIntersection();

        void partiallyMemIntersection();

        void lowMemIntersection();

      private:

        te::da::DataSourceInfoPtr m_inputDatasource;
        te::da::DataSourceInfoPtr m_outputDatasource;
        std::list<te::map::AbstractLayerPtr> m_layers;

    };
  }
}

#endif // __TERRALIB_VP_CORE_INTERNAL_INTERSECTION_H