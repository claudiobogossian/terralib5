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
 \file GroupThreadManager.cpp
 */

// Terralib
#include "../common/Logger.h"
#include "../common/progress/TaskProgress.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../dataaccess/utils/Utils.h"

#include "../geometry/GeometryProperty.h"

#include "GroupThreadManager.h"

//Boost
#include <boost/thread.hpp>

namespace te
{
  namespace vp
  {
    GroupThreadManager::GroupThreadManager( const std::map<std::string, std::vector<int> >& groups
                                            , te::da::DataSet* dataSet
                                            , te::da::DataSetType* dataSetType
                                            , te::mem::DataSet* outputDataSet
                                            , te::da::DataSetType* outputDataSetType
                                            , te::da::DataSource* outputDataSource
                                            , std::map<std::string, te::dt::AbstractData*> specificParams)
      : m_groups(groups)
      , m_savedCount(0)
      , m_dataSet(dataSet)
      , m_dataSetType(dataSetType)
      , m_outputDataSet(outputDataSet)
      , m_outputDataSetType(outputDataSetType)
      , m_outputDataSource(outputDataSource)
      , m_specificParams(specificParams)
    {
      m_groupsIterator = m_groups.begin();
    }


    bool GroupThreadManager::getNextGroup(std::vector< te::mem::DataSetItem*>& nextGroup)
    {
      boost::lock_guard<boost::mutex> lock(m_mtx);
      
      if (hasMoreGroups() == false)
      {
        return false;
      }
      
      nextGroup.clear();

      for (std::size_t i = 0; i < m_groupsIterator->second.size(); ++i)
      {
        m_dataSet->move(m_groupsIterator->second[i]);
        te::mem::DataSetItem* item = new te::mem::DataSetItem(m_dataSet);

        for (std::size_t j = 0; j < m_dataSetType->size(); ++j)
        {
          if (!m_dataSet->isNull(j))
            item->setValue(j, m_dataSet->getValue(j).release());
        }

        nextGroup.push_back(item);
      }

      ++m_groupsIterator;

      return true;
    }

    bool GroupThreadManager::getNextOutput(std::vector< te::mem::DataSetItem*>& nextOutput)
    {
      boost::lock_guard<boost::mutex> lock(m_mtxOutput);

      if (m_outputQueue.empty())
      {
        if (m_savedCount == m_groups.size())
        {
          return false;
        }

        return true;
      }

      nextOutput = *m_outputQueue.begin();

      m_outputQueue.erase(m_outputQueue.begin());

      ++m_savedCount;

      return true;
    }

    te::da::DataSetType* GroupThreadManager::getDataSetType()
    {
      return m_dataSetType;
    }

    te::da::DataSource* GroupThreadManager::getOutputDataSource()
    {
      return m_outputDataSource;
    }

    te::da::DataSetType* GroupThreadManager::getOutputDataSetType()
    {
      return m_outputDataSetType;
    }

    std::map<std::string, te::dt::AbstractData*> GroupThreadManager::getSpecificParameters()
    {
      return m_specificParams;
    }

    te::mem::DataSet* GroupThreadManager::getClearOutputDataSet()
    {
      m_outputDataSet->clear();

      return m_outputDataSet;
    }

    std::vector<std::string> GroupThreadManager::getWarnings()
    {
      return m_warnings;
    }

    te::mem::DataSetItem* GroupThreadManager::createOutputItem()
    {
      te::mem::DataSetItem* item = new te::mem::DataSetItem(m_outputDataSet);

      return item;
    }

    void GroupThreadManager::addOutput(std::vector<te::mem::DataSetItem*>& itemGroup)
    {
      boost::lock_guard<boost::mutex> lock(m_mtxOutput);

      m_outputQueue.push_back(itemGroup);
    }

    void GroupThreadManager::addWarning(const std::string& warning, const bool& appendIfExists)
    {
      boost::lock_guard<boost::mutex> lock(m_mtxWarning);

      if (!appendIfExists)
      {
        std::vector<std::string>::iterator it = std::find(m_warnings.begin(), m_warnings.end(), warning);
        if (it == m_warnings.end())
          m_warnings.push_back(warning);
      }
      else
      {
        m_warnings.push_back(warning);
      }

#ifdef TERRALIB_LOGGER_ENABLED
      te::common::Logger::logDebug("vp", warning.c_str());
#endif // TERRALIB_LOGGER_ENABLED
    }

    bool GroupThreadManager::hasMoreGroups()
    {
      if (m_groupsIterator == m_groups.end())
      {
        return false;
      }

      return true;
    }

    GroupThreadManager::GroupThreadManager(const GroupThreadManager&)
    {
    }

    const GroupThreadManager& GroupThreadManager::operator=(const GroupThreadManager&)
    {
      return *this;
    }
  }// end namespace vp
}// end namespace te
