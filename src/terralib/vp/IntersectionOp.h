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
 \file IntersectionOp.h
 
 \brief Intersection operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_INTERSECTION_OP_H
#define __TERRALIB_VP_INTERNAL_INTERSECTION_OP_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"

#include "../datatype/Property.h"

#include "../geometry/Enums.h"
#include "../memory/DataSet.h"

#include "IntersectionOp.h"
#include "Config.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace vp
  {
    class TEVPEXPORT IntersectionOp
    {
    public:
      
      IntersectionOp();
      
      virtual ~IntersectionOp() {}
      
      virtual bool run() = 0;
      
      virtual bool paramsAreValid();
      
      void setInput(te::da::DataSourcePtr inFirstDsrc,
                    std::auto_ptr<te::da::DataSet> inFirstDset,
                    std::auto_ptr<te::da::DataSetType> inFirstDsetType,
                    te::da::DataSourcePtr inSecondDsrc,
                    std::auto_ptr<te::da::DataSet> inSecondDset,
                    std::auto_ptr<te::da::DataSetType> inSecondDsetType);

      void setParams(const bool& copyInputColumns, std::size_t inSRID);

      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsname);

    protected:

      bool save(std::auto_ptr<te::da::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType);

      std::vector<te::dt::Property*> getTabularProps(te::da::DataSetType* dsType);


      // it defines the type of the result considering the input geometries being aggregated
      te::gm::GeomType getGeomResultType(te::gm::GeomType geom);
      
      te::da::DataSourcePtr m_inFirstDsrc;
      std::auto_ptr<te::da::DataSet> m_inFirstDset;
      std::auto_ptr<te::da::DataSetType> m_inFirstDsetType;
      te::da::DataSourcePtr m_inSecondDsrc;
      std::auto_ptr<te::da::DataSet> m_inSecondDset;
      std::auto_ptr<te::da::DataSetType> m_inSecondDsetType;

      bool m_copyInputColumns;
      std::size_t m_SRID;

      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDsetName;
    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_INTERSECTION_OP_H