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
 \file GeometricOp.h
 
 \brief Geometric operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_GEOMETRIC_OP_H
#define __TERRALIB_VP_INTERNAL_GEOMETRIC_OP_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
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
    class TEVPEXPORT GeometricOp
    {
    public:
      
      GeometricOp();
      
      virtual ~GeometricOp() {}
      
      virtual bool run() = 0;
      
      virtual bool paramsAreValid();
      
      void setInput(te::da::DataSourcePtr inDsrc,
                    std::string inDsetName,
                    std::auto_ptr<te::da::DataSetType> inDsetType);
      
      void setParams(std::vector<std::string> selectedProps, 
                    std::vector<te::vp::GeometricOperation> operations,
                    te::vp::GeometricOpObjStrategy objStrategy,
                    std::string attribute,
                    bool outputLayer);
      
      void setOutput(std::auto_ptr<te::da::DataSource> outDsrc, std::string dsname);

      std::vector<std::string> GetOutputDSetNames();
      
    protected:

      te::da::DataSetType* GetDataSetType(te::vp::GeometricOpObjStrategy,
                                          bool MultiGeomColumns,
                                          int geomOp = -1);

      te::da::DataSourcePtr m_inDsrc;
      std::string m_inDsetName;
      std::auto_ptr<te::da::DataSetType> m_inDsetType;

      std::vector<std::string> m_selectedProps;
      std::vector<te::vp::GeometricOperation> m_operations;
      te::vp::GeometricOpObjStrategy m_objStrategy;
      std::string m_attribute;
      bool m_outputLayer;
      
      std::auto_ptr<te::da::DataSource> m_outDsrc;
      std::string m_outDsetName;
      std::vector<std::string> m_outDsetNameVec;
    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_GEOMETRIC_OP_H