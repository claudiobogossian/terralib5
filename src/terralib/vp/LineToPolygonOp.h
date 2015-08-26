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
 \file LineToPolygonOp.h
 
 \brief Line to polygon operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_LINETOPOLYGON_OP_H
#define __TERRALIB_VP_INTERNAL_LINETOPOLYGON_OP_H

//Terralib
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"

#include "Config.h"

// STL
#include <memory>
#include <string>

namespace te
{
  namespace vp
  {
    class TEVPEXPORT LineToPolygonOp
    {
    public:
      
      LineToPolygonOp();
      
      virtual ~LineToPolygonOp() {}
      
      virtual bool run() = 0;
      
      virtual bool paramsAreValid();
      
      void setInput(te::da::DataSourcePtr inDsrc,
                    std::string inDsetName,
                    std::auto_ptr<te::da::DataSetTypeConverter> converter,
                    const te::da::ObjectIdSet* oidSet = 0);

      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsname);
      
    protected:

      std::auto_ptr<te::da::DataSetType> buildOutDataSetType();

      te::gm::GeomType getGeomResultType(te::gm::GeomType geom);
      
      te::da::DataSourcePtr m_inDsrc;
      std::string m_inDsetName;
      std::auto_ptr<te::da::DataSetTypeConverter> m_converter;
      const te::da::ObjectIdSet* m_oidSet;

      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDset;
    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_LINETOPOLYGON_OP_H