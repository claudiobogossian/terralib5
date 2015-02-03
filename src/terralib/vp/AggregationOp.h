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
 \file AggregationOp.h
 
 \brief Aggregation operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_AGGREGATION_OP_H
#define __TERRALIB_VP_INTERNAL_AGGREGATION_OP_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"

#include "../datatype/Property.h"

#include "../memory/DataSet.h"
#include "../statistics/core/Enums.h"

#include "Config.h"
#include "Enums.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace vp
  {
    class TEVPEXPORT AggregationOp
    {
    public:
      
      AggregationOp();
      
      virtual ~AggregationOp() {}
      
      virtual bool run() = 0;
      
      virtual bool paramsAreValid();
      
      void setInput(te::da::DataSourcePtr inDsrc,
                    std::string inDsetName,
                    std::auto_ptr<te::da::DataSetType> inDsetType,
                    const te::da::ObjectIdSet* oidSet = 0);
      
      void setParams(std::vector<te::dt::Property*>& groupProps,
                     std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >&statSum);
      
      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsname);
      
    protected:
      
      // it defines the type of the result considering the input geometries being aggregated
      te::gm::GeomType getGeomResultType(te::gm::GeomType geom);
      
      te::da::DataSourcePtr m_inDsrc;
      std::string m_inDsetName;
      std::auto_ptr<te::da::DataSetType> m_inDsetType;
      const te::da::ObjectIdSet* m_oidSet;

      std::vector<te::dt::Property*> m_groupProps;
      std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > m_statSum;
      
      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDset;
    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_AGGREGATION_OP_H