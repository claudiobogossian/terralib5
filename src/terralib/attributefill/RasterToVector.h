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
 \file RasterToVector.h
 
 \brief Raster to Vector processing.
 
 \ingroup attributefill
 */

#ifndef __TERRALIB_ATTRIBUTEFILL_INTERNAL_RASTER_TO_VECTOR_H
#define __TERRALIB_ATTRIBUTEFILL_INTERNAL_RASTER_TO_VECTOR_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"

#include "../datatype/Property.h"

#include "../memory/DataSet.h"
#include "../raster/Raster.h"
#include "../rp/Texture.h"
#include "../statistics/core/Enums.h"


#include "Config.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace attributefill
  {
    class TEATTRIBUTEFILLEXPORT RasterToVector
    {
    public:
      
      RasterToVector();
      
      ~RasterToVector() {}

      void setInput(te::rst::Raster* inRaster,
                    te::da::DataSourcePtr inVectorDsrc,
                    std::string inVectorName,
                    std::auto_ptr<te::da::DataSetTypeConverter> inVectorDsType,
                    const te::da::ObjectIdSet* oidSet = 0);

      void setParams(std::vector<unsigned int> bands,
                     std::vector<te::stat::StatisticalSummary> statSum,
                     bool texture);

      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsName);

      bool paramsAreValid();

      bool run();

    protected:

      std::vector<std::set<int> > getPixelDistinct(te::rst::Raster* rst, std::vector<unsigned int> bands);

      std::auto_ptr<te::da::DataSetType> getDataSetType(std::vector<std::set<int> > pixelDistinct = std::vector<std::set<int> >());

      std::vector<te::rp::Texture> getTexture(te::rst::Raster* rst,
                                              te::gm::Geometry* geom,
                                              int bands);

      bool save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType);

      te::rst::Raster* m_inRaster;
      te::da::DataSourcePtr m_inVectorDsrc;
      std::string m_inVectorName;
      std::auto_ptr<te::da::DataSetTypeConverter> m_inVectorDsType;
      const te::da::ObjectIdSet* m_oidSet;

      std::vector<te::stat::StatisticalSummary> m_statSum;
      std::vector<unsigned int> m_bands;
      bool m_texture;

      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDset;
    };
  }
}
#endif // __TERRALIB_ATTRIBUTEFILL_INTERNAL_RASTER_TO_VECTOR_H
