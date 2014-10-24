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
 \file VectorToRaster.h
 
 \brief Vector to Raster processing.
 
 \ingroup attributefill
 */

#ifndef __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTOR_TO_RASTER_H
#define __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTOR_TO_RASTER_H

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
  namespace attributefill
  {
    class TEATTRIBUTEFILLEXPORT VectorToRaster
    {
    public:

      VectorToRaster();

      ~VectorToRaster() {}

      void setInput(te::da::DataSourcePtr inVectorDsrc,
                    std::string inVectorName,
                    std::auto_ptr<te::da::DataSetType> inVectorDsType);

      void setParams( std::vector<std::string> selectedAttVec,
                      double resolutionX,
                      double resolutionY,
                      int columns,
                      int rows,
                      bool useDummy,
                      int dummy = 0);

      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsName);

      bool paramsAreValid();

      bool run();

    protected:

      te::da::DataSourcePtr m_inVectorDsrc;
      std::string m_inVectorName;
      std::auto_ptr<te::da::DataSetType> m_inVectorDsType;

      std::vector<std::string> m_selectedAttVec;
      double m_resolutionX;
      double m_resolutionY;
      int m_columns;
      int m_rows;
      bool m_setDummy;
      int m_dummy;

      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDset;
    };
  }
}
#endif // __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTOR_TO_RASTER_H