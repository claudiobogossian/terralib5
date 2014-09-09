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
 \file RasterToVector.h
 
 \brief Raster to Vector processing.
 
 \ingroup processing
 */

#ifndef __TERRALIB_PROCESSING_INTERNAL_RASTER_TO_VECTOR_H
#define __TERRALIB_PROCESSING_INTERNAL_RASTER_TO_VECTOR_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"

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
  namespace processing
  {
    class TEPROCESSINGEXPORT RasterToVector
    {
    public:
      
      RasterToVector();
      
      ~RasterToVector() {}

      void setInput(te::da::DataSourcePtr inRasterDsrc,
                    std::string inRasterName,
                    std::auto_ptr<te::da::DataSetType> inRasterDsType,
                    te::da::DataSourcePtr inVectorDsrc,
                    std::string inVectorName,
                    std::auto_ptr<te::da::DataSetType> inVectorDsType);

      void setParams(std::vector<unsigned int> bands,
                     std::vector<te::stat::StatisticalSummary> statSum);

      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsName);

      bool paramsAreValid();

      bool run();

    protected:

      std::auto_ptr<te::da::DataSetType> getDataSetType();

      bool save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType);

      te::da::DataSourcePtr m_inRasterDsrc;
      std::string m_inRasterName;
      std::auto_ptr<te::da::DataSetType> m_inRasterDsType;
      te::da::DataSourcePtr m_inVectorDsrc;
      std::string m_inVectorName;
      std::auto_ptr<te::da::DataSetType> m_inVectorDsType;

      std::vector<te::stat::StatisticalSummary> m_statSum;
      std::vector<unsigned int> m_bands;

      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDset;
    };
  }
}
#endif // __TERRALIB_PROCESSING_INTERNAL_RASTER_TO_VECTOR_H