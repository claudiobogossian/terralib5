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
 \file VectorToVectorOp.h
 
 \brief Vector To Vector operation.
 
 \ingroup attributefill
 */

#ifndef __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTOROP_H
#define __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTOROP_H

//Terralib
#include "../dataaccess/datasource/DataSource.h"

#include "../maptools/AbstractLayer.h"

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
  namespace attributefill
  {
    class TEATTRIBUTEFILLEXPORT VectorToVectorOp
    {
      public:
      
        VectorToVectorOp();
      
        virtual ~VectorToVectorOp();
      
        virtual bool run() = 0;
      
        virtual bool paramsAreValid();
      
        void setInput(te::map::AbstractLayerPtr fromLayer,
                      te::map::AbstractLayerPtr toLayer);
      
        void setParams(const std::map<std::string, std::vector<te::attributefill::OperationType> >& options);
      
        void setOutput(te::da::DataSourcePtr outDsrc, std::string dsName);
      
      protected:

        bool save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType);

      protected:

        te::map::AbstractLayerPtr m_fromLayer;
        te::map::AbstractLayerPtr m_toLayer;

        std::map<std::string, std::vector<te::attributefill::OperationType> > m_options;

        te::da::DataSourcePtr m_outDsrc;
        std::string m_outDset;
    };
  }
}
#endif // __TERRALIB_ATTRIBUTEFILL_INTERNAL_VECTORTOVECTOROP_H