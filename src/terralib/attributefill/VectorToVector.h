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
 \file VectorToVector.h
 
 \brief Vector to Vector processing.
 
 \ingroup attributefill
 */

#ifndef __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTOR_H
#define __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTOR_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../maptools/AbstractLayer.h"

#include "../datatype/Property.h"

#include "../memory/DataSet.h"
#include "../statistics/core/Enums.h"

#include "Config.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace stat
  {
    struct NumericStatisticalSummary;
  }

  namespace attributefill
  {
    class TEATTRIBUTEFILLEXPORT VectorToVector
    {
      public:

        VectorToVector();
      
        ~VectorToVector();

        void setInput(te::map::AbstractLayerPtr fromLayer,
                      te::map::AbstractLayerPtr toLayer);

        void setParams(std::map<te::dt::Property*, std::vector<std::string> >& options,
                       std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >&statSum);

        void setOutput(te::da::DataSourcePtr outDsrc, std::string dsName);

        bool paramsAreValid();

        bool run();

      protected:

        te::sam::rtree::Index<size_t, 8>* getRtree(te::da::DataSet* data);

        bool save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType);

        te::da::DataSetType* getOutputDataSetType();

        std::string getPropertyName(te::dt::Property* prop, const std::string& func);

        bool isStatistical(const std::string& funcName);

        std::vector<std::size_t> getIntersections(te::da::DataSet* toDs,
                                                  te::da::DataSet* fromDs,
                                                  te::sam::rtree::Index<size_t, 8>* rtree);

        std::vector<double> getValues(te::da::DataSet* fromDs, std::vector<std::size_t> dsPos, const std::string& function);

        double getValue(te::stat::NumericStatisticalSummary ss, const std::string& function);

        std::vector<std::string> getSelectedFunctions();

      protected:

        te::map::AbstractLayerPtr m_fromLayer;
        te::map::AbstractLayerPtr m_toLayer;

        std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > m_statSum;
        std::map<te::dt::Property*, std::vector<std::string> > m_options;

        te::da::DataSourcePtr m_outDsrc;
        std::string m_outDset;
    };
  }
}
#endif // __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTOR_H