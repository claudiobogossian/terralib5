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
 \file GroupThreadManager.h
 
 \brief Class to manager group values in threads.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_GROUP_THREAD_MANAGER_H
#define __TERRALIB_VP_INTERNAL_GROUP_THREAD_MANAGER_H

#include "Config.h"

// STL
#include <map>
#include <vector>

// Boost
#include <boost/thread/mutex.hpp>

namespace te
{
  namespace common
  {
    class TaskProgress;
  }

  namespace da
  {
    class DataSet;
    class DataSetType;
    class DataSource;
  }

  namespace mem
  {
    class DataSet;
    class DataSetItem;
  }

  namespace gm
  {
    class Geometry;
  }

  namespace vp
  {
    class TEVPEXPORT GroupThreadManager
    {
    public:
      
      GroupThreadManager( const std::map<std::string, std::vector<int> >& groups
                        , te::da::DataSet* dataSet
                        , te::da::DataSetType* dataSetType
                        , te::mem::DataSet* outputDataSet
                        , te::da::DataSetType* outputDataSetType
                        , te::da::DataSource* outputDataSource
                        , std::map<std::string, te::dt::AbstractData*> specificParams);
      
      virtual ~GroupThreadManager() {}
      
      bool getNextGroup(std::vector< te::mem::DataSetItem*>& nextGroup);

      bool getNextOutput(std::vector< te::mem::DataSetItem*>& nextOutput);

      te::da::DataSetType* getDataSetType();

      te::da::DataSource* getOutputDataSource();

      te::da::DataSetType* getOutputDataSetType();

      std::map<std::string, te::dt::AbstractData*> getSpecificParameters();

      te::mem::DataSet* getClearOutputDataSet();

      std::vector<std::string> getWarnings();

      te::mem::DataSetItem* createOutputItem();

      void addOutput(std::vector<te::mem::DataSetItem*>& itemGroup);
    
      void addWarning(const std::string& warning, const bool& appendIfExists = false);

    private:

      bool hasMoreGroups();

      GroupThreadManager(const GroupThreadManager&);

      const GroupThreadManager& operator=(const GroupThreadManager&);

      std::map<std::string, std::vector<int> > m_groups;
      std::size_t m_savedCount;
      te::da::DataSet* m_dataSet;
      te::da::DataSetType* m_dataSetType;
      std::map<std::string, std::vector<int> >::iterator m_groupsIterator;

      te::da::DataSetType* m_outputDataSetType;
      te::da::DataSource* m_outputDataSource;
      te::mem::DataSet* m_outputDataSet;
      std::vector< std::vector<te::mem::DataSetItem*> > m_outputQueue;

      std::map<std::string, te::dt::AbstractData*> m_specificParams;

      std::vector<std::string> m_warnings;

      boost::mutex m_mtx;
      boost::mutex m_mtxOutput;
      boost::mutex m_mtxWarning;
    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_GROUP_THREAD_MANAGER_H