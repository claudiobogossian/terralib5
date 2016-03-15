/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
\file src/terraMobilePlugin/core/GeoPackageSynchronizer.h

\brief This file is used to Synchronizer operation.
*/

#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../datatype/SimpleData.h"
#include "../../../../memory/DataSet.h"
#include "../../../../memory/DataSetItem.h"

#include "../Config.h"
#include "GeopackageSynchronizer.h"


// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


te::qt::plugins::terramobile::GeoPackageSynchronizer::GeoPackageSynchronizer() :
  m_inputDataSource(0),
  m_inputDataSet(""),
  m_outputDataSource(0),
  m_outputDataset("")
{

}

te::qt::plugins::terramobile::GeoPackageSynchronizer::~GeoPackageSynchronizer()
{

}

void te::qt::plugins::terramobile::GeoPackageSynchronizer::setInputParameters(te::da::DataSource* inputDataSource, std::string inputDataSet, te::da::DataSource* outputDataSource, std::string outputDataset)
{
  m_inputDataSource = inputDataSource;
  m_inputDataSet = inputDataSet;

  m_outputDataSource = outputDataSource;
  m_outputDataset = outputDataset;
}

void te::qt::plugins::terramobile::GeoPackageSynchronizer::synchronize()
{
  checkParameters();

  //get dataset type
  std::auto_ptr<te::da::DataSetType> gpkgDsType = m_inputDataSource->getDataSetType(m_inputDataSet);

  //get data from gpkg
  std::auto_ptr<te::da::DataSet> gpkgDataSet = m_inputDataSource->getDataSet(m_inputDataSet);

  //get gpkg ds attr pk index
  int gpkgDataSetAttrPKIdx = -1;
  if (gpkgDsType->getPrimaryKey())
  {
    te::da::PrimaryKey* pk = gpkgDsType->getPrimaryKey();
    if (!pk->getProperties().empty())
    {
      gpkgDataSetAttrPKIdx = te::da::GetPropertyIndex(gpkgDataSet.get(), pk->getProperties()[0]->getName());
    }
  }

  int gpkgDataSetObjIdIdx = te::da::GetPropertyIndex(gpkgDataSet.get(), LAYER_GATHERING_OBJID_COLUMN);


  //create dataset memory to insert into output datasource
  te::mem::DataSet* insertDataSetDataSource = new te::mem::DataSet(gpkgDsType.get());

  //create dataset memory to insert into output datasource
  te::mem::DataSet* updateDataSetDataSource = new te::mem::DataSet(gpkgDsType.get());

  //create dataset memory to update into gpkg
  te::mem::DataSet* updateDataSetGPKG = new te::mem::DataSet(gpkgDsType.get());

  std::vector<te::dt::Property*> props = gpkgDsType->getProperties();

  static boost::uuids::basic_random_generator<boost::mt19937> gen;

  gpkgDataSet->moveBeforeFirst();

  while (gpkgDataSet->moveNext())
  {
    if (gpkgDataSet->isNull(LAYER_GATHERING_OBJID_COLUMN))
    {
      //set obj id info
      boost::uuids::uuid u = gen();
      std::string id = boost::uuids::to_string(u);

      //create memory items
      te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(insertDataSetDataSource);
      te::mem::DataSetItem* gpkgItem = new te::mem::DataSetItem(updateDataSetGPKG);

      for (std::size_t t = 0; t < props.size(); ++t)
      {
        if (props[t]->getName() == LAYER_GATHERING_OBJID_COLUMN)
        {
          dsItem->setValue(LAYER_GATHERING_OBJID_COLUMN, new te::dt::SimpleData<std::string, te::dt::STRING_TYPE>(id));
          gpkgItem->setValue(LAYER_GATHERING_OBJID_COLUMN, new te::dt::SimpleData<std::string, te::dt::STRING_TYPE>(id));
        }
        else
        {
          dsItem->setValue(props[t]->getName(), gpkgDataSet->getValue(props[t]->getName()).release());
          gpkgItem->setValue(props[t]->getName(), gpkgDataSet->getValue(props[t]->getName()).release());
        }
      }

      insertDataSetDataSource->add(dsItem);
      updateDataSetGPKG->add(gpkgItem);
    }
    else
    {
      //create memory items
      te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(updateDataSetDataSource);

      for (std::size_t t = 0; t < props.size(); ++t)
      {
        dsItem->setValue(props[t]->getName(), gpkgDataSet->getValue(props[t]->getName()).release());
      }

      updateDataSetDataSource->add(dsItem);
    }
  }

  //INSERT into output datasource
  {
    std::unique_ptr<te::da::DataSourceTransactor> transactor = m_outputDataSource->getTransactor();

    std::map<std::string, std::string> op;

    try
    {
      transactor->add(m_outputDataset, insertDataSetDataSource, op);
    }
    catch (const te::common::Exception& e)
    {
      transactor->rollBack();

      return;
    }
    catch (const std::exception& e)
    {
      transactor->rollBack();

      return;
    }
    catch (...)
    {
      transactor->rollBack();

      return;
    }

    transactor->commit();
  }

  //UPDATE into output datasource
  {
    std::unique_ptr<te::da::DataSourceTransactor> transactor = m_outputDataSource->getTransactor();

    try
    {
      std::vector<size_t> ids;
      ids.push_back(0);

      std::vector< std::set<int> > properties;
      std::size_t dsSize = updateDataSetDataSource->size();

      for (std::size_t t = 0; t < dsSize; ++t)
      {
        std::set<int> setPos;

        for (std::size_t p = 0; p < updateDataSetDataSource->getNumProperties(); ++p)
        {
          setPos.insert(p);
        }

        properties.push_back(setPos);
      }

      updateDataSetDataSource->moveBeforeFirst();

      transactor->update(m_outputDataset, updateDataSetDataSource, properties, ids);
    }
    catch (const te::common::Exception& e)
    {
      transactor->rollBack();

      return;
    }
    catch (const std::exception& e)
    {
      transactor->rollBack();

      return;
    }
    catch (...)
    {
      transactor->rollBack();

      return;
    }

    transactor->commit();
  }

  //UPDATE into gpkg
  {
    std::unique_ptr<te::da::DataSourceTransactor> transactor = m_inputDataSource->getTransactor();

    std::map<std::string, std::string> op;

    try
    {
      std::vector<size_t> ids;
      ids.push_back(0);

      std::vector< std::set<int> > properties;
      std::size_t dsSize = updateDataSetDataSource->size();

      for (std::size_t t = 0; t < dsSize; ++t)
      {
        std::set<int> setPos;
        setPos.insert(gpkgDataSetObjIdIdx);

        properties.push_back(setPos);
      }

      transactor->update(m_outputDataset, updateDataSetGPKG, properties, ids);
    }
    catch (const te::common::Exception& e)
    {
      transactor->rollBack();

      return;
    }
    catch (const std::exception& e)
    {
      transactor->rollBack();

      return;
    }
    catch (...)
    {
      transactor->rollBack();

      return;
    }

    transactor->commit();
  }
}

void te::qt::plugins::terramobile::GeoPackageSynchronizer::checkParameters()
{
  if (!m_inputDataSource)
    throw;

  if (m_inputDataSet.empty())
    throw;

  if (!m_outputDataSource)
    throw;

  if (m_outputDataset.empty())
    throw;
}
