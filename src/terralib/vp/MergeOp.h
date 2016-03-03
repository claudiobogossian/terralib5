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
 \file MergeOp.h
 
 \brief Merge operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_MERGE_OP_H
#define __TERRALIB_VP_INTERNAL_MERGE_OP_H

//Terralib
#include "../dataaccess/datasource/DataSource.h"
#include "Config.h"

// STL
#include <memory>

namespace te
{
  namespace da
  {
    class DataSet;
    class DataSetType;
  }

  namespace dt
  {
    class Property;
  }

  namespace vp
  {
    class TEVPEXPORT MergeOp
    {
    public:
      
      MergeOp();
      
      virtual ~MergeOp() {}
      
      virtual bool run() = 0;
      
      virtual bool paramsAreValid();
      
      void setInput(te::da::DataSetType* firstDst, te::da::DataSet* firstDs, te::da::DataSetType* secondDst, te::da::DataSet* secondDs);
      
      void setParams(std::vector<std::pair<std::string, std::string> > properties);
      
      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsname);

    private:

      bool isValidName(const std::string& name);

      std::vector<std::string> checkAttrNames(const std::vector<std::pair<std::string, std::string> >& properties);

      std::auto_ptr<te::dt::Property> getNewPkProperty();

    protected:

      std::auto_ptr<te::da::DataSetType> getOutputDst();

    protected:
      
      te::da::DataSetType* m_firstDst;
      te::da::DataSet*     m_firstDs;
      te::da::DataSetType* m_secondDst;
      te::da::DataSet*     m_secondDs;

      std::vector<std::pair<std::string, std::string> > m_properties;

      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDset;
    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_MERGE_OP_H