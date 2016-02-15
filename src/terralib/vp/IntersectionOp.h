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
 \file IntersectionOp.h
 
 \brief Intersection operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_INTERSECTION_OP_H
#define __TERRALIB_VP_INTERNAL_INTERSECTION_OP_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
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
                    std::string inFirstDsetName,
                    std::auto_ptr<te::da::DataSetTypeConverter> firstConverter,
                    te::da::DataSourcePtr inSecondDsrc,
                    std::string inSecondDsetName,
                    std::auto_ptr<te::da::DataSetTypeConverter> secondConverter,
                    const te::da::ObjectIdSet* firstOidSet = 0,
                    const te::da::ObjectIdSet* secondOidSet = 0);

      void setInput(te::da::DataSourcePtr inFirstDsrc,
                    std::string inFirstDsetName,
                    std::auto_ptr<te::da::DataSetType> firstDsType,
                    std::auto_ptr<te::da::DataSet> firstDs,
                    std::auto_ptr<te::da::DataSetTypeConverter> firstConverter,
                    te::da::DataSourcePtr inSecondDsrc,
                    std::string inSecondDsetName,
                    std::auto_ptr<te::da::DataSetType> secondDsType,
                    std::auto_ptr<te::da::DataSet> secondDs,
                    std::auto_ptr<te::da::DataSetTypeConverter> secondConverter,
                    const te::da::ObjectIdSet* firstOidSet = 0,
                    const te::da::ObjectIdSet* secondOidSet = 0);

      void setParams(const std::vector<std::pair<std::string, std::string> >& attributeVec);

      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsname);

      te::da::DataSetType* getOutputDsType();

      void setIsFirstQuery();

      void setIsSecondQuery();

    protected:

      std::vector<te::dt::Property*> getTabularProps(te::da::DataSetType* dsType);
      
      // it defines the type of the result considering the input geometries.
      te::gm::GeomType setGeomResultType(te::gm::GeomType firstGeom, te::gm::GeomType secondGeom);
      
      bool isSelectedProperty(const std::string& dsName, te::dt::Property* prop);

    protected:
      te::da::DataSourcePtr m_inFirstDsrc;
      std::string m_inFirstDsetName;
      std::auto_ptr<te::da::DataSetTypeConverter> m_firstConverter;
      te::da::DataSourcePtr m_inSecondDsrc;
      std::string m_inSecondDsetName;
      std::auto_ptr<te::da::DataSetTypeConverter> m_secondConverter;
      const te::da::ObjectIdSet* m_firstOidSet;
      const te::da::ObjectIdSet* m_secondOidSet;

      std::auto_ptr<te::da::DataSetType> m_firstDsType;
      std::auto_ptr<te::da::DataSet> m_firstDs;
      std::auto_ptr<te::da::DataSetType> m_secondDsType;
      std::auto_ptr<te::da::DataSet> m_secondDs;

      bool m_isFistQuery;
      bool m_isSecondQuery;

      std::vector<std::pair<std::string, std::string> > m_attributeVec;

      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDsetName;
    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_INTERSECTION_OP_H