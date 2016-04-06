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
 \file Difference.h
 
 \brief Difference operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_MERGE_H
#define __TERRALIB_VP_INTERNAL_MERGE_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"

#include "../datatype/Property.h"

#include "../geometry/Enums.h"
#include "../memory/DataSet.h"

#include "Algorithm.h"
#include "AlgorithmParams.h"
#include "Config.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace da { class DataSourceTransactor; }

  namespace dt { class Property; }

  namespace vp
  {
    class TEVPEXPORT Merge : public Algorithm
    {
    public:

      enum MergeStrategy
      {
        PUREMERGE = 0,
        TRYGETMAX = 1,
        AUTOINCREMENT = 2
      };

      Merge();

      virtual ~Merge() {}

      bool executeMemory(te::vp::AlgorithmParams* mainParams);

    protected:

      MergeStrategy checkStrategy();

      std::vector<std::pair<std::string, std::string> > getPropNames(const std::map<std::string, te::dt::AbstractData*>& specificParams);

      std::auto_ptr<te::da::DataSetType> getOutputDataSetType(te::vp::AlgorithmParams* mainParams);

      bool isUpdate(const std::map<std::string, te::dt::AbstractData*>& specificParams);

      int tryGetMax();

      bool isPrimaryKeyProperty(const te::da::DataSetType* dst, const std::string& p);

      void updateInputDst(te::da::DataSourceTransactor* transactor);

      int getPropertyType(const te::dt::Property* p);

    private:

      te::da::DataSourcePtr m_inputSource;
      te::da::DataSetType*  m_inputDst;
      te::da::DataSet*      m_inputDs;
      te::da::DataSourcePtr m_mergeSource;
      te::da::DataSetType*  m_mergeDst;
      te::da::DataSet*      m_mergeDs;

      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDsetName;

      bool m_isUpdate;

      int m_inputSRID;

      std::vector<std::pair<std::string, std::string> > m_properties;

    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_MERGE_H